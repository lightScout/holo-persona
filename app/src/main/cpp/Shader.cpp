#include "Shader.h"

#include "AndroidOut.h"
#include "Model.h"
#include "Utility.h"

Shader *Shader::loadShader(
        const std::string &vertexSource,
        const std::string &fragmentSource,
        const std::string &positionAttributeName,
        const std::string &uvAttributeName,
        const std::string &mvpMatrixUniformName) {
    Shader *shader = nullptr;

    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexSource);
    if (!vertexShader) {
        return nullptr;
    }

    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentSource);
    if (!fragmentShader) {
        glDeleteShader(vertexShader);
        return nullptr;
    }

    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);

        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint logLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

            // If we fail to link the shader program, log the result for debugging
            if (logLength) {
                GLchar *log = new GLchar[logLength];
                glGetProgramInfoLog(program, logLength, nullptr, log);
                aout << "Failed to link program with:\n" << log << std::endl;
                delete[] log;
            }

            glDeleteProgram(program);
        } else {
            // Get the attribute and uniform locations by name. You may also choose to hardcode
            // indices with layout= in your shader, but it is not done in this sample
            GLint positionAttribute = glGetAttribLocation(program, positionAttributeName.c_str());
            GLint uvAttribute = glGetAttribLocation(program, uvAttributeName.c_str());
            GLint mvpMatrixUniform = glGetUniformLocation(program, mvpMatrixUniformName.c_str());

            // Only create a new shader if all the attributes are found.
            if (positionAttribute != -1
                && uvAttribute != -1
                && mvpMatrixUniform != -1) {

                shader = new Shader(
                        program,
                        positionAttribute,
                        uvAttribute,
                        mvpMatrixUniform);
            } else {
                aout << "Failed to find shader attributes/uniforms:" << std::endl;
                aout << "  Position: " << positionAttribute << std::endl;
                aout << "  UV: " << uvAttribute << std::endl;
                aout << "  MVP: " << mvpMatrixUniform << std::endl;
                glDeleteProgram(program);
            }
        }
    }

    // The shaders are no longer needed once the program is linked. Release their memory.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shader;
}

GLuint Shader::loadShader(GLenum shaderType, const std::string &shaderSource) {
    Utility::assertGlError();
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        auto *shaderRawString = (GLchar *) shaderSource.c_str();
        GLint shaderLength = shaderSource.length();
        glShaderSource(shader, 1, &shaderRawString, &shaderLength);
        glCompileShader(shader);

        GLint shaderCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderCompiled);

        // If the shader doesn't compile, log the result to the terminal for debugging
        if (!shaderCompiled) {
            GLint infoLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);

            if (infoLength) {
                auto *infoLog = new GLchar[infoLength];
                glGetShaderInfoLog(shader, infoLength, nullptr, infoLog);
                aout << "Failed to compile with:\n" << infoLog << std::endl;
                delete[] infoLog;
            }

            glDeleteShader(shader);
            shader = 0;
        }
    }
    return shader;
}

void Shader::activate() const {
    glUseProgram(program_);
}

void Shader::deactivate() const {
    glUseProgram(0);
}

void Shader::drawModel(const Model &model) const {
    // Debug: Check if we have valid data
    if (model.getVertexData() == nullptr || model.getIndexData() == nullptr) {
        aout << "ERROR: Model has null vertex or index data!" << std::endl;
        return;
    }
    
    // Debug: Check attribute locations
    static bool attributesLogged = false;
    if (!attributesLogged) {
        aout << "DEBUG: Shader attributes - position: " << position_ << ", uv: " << uv_ << std::endl;
        aout << "DEBUG: Vertex data pointer: " << model.getVertexData() << std::endl;
        aout << "DEBUG: Index data pointer: " << model.getIndexData() << std::endl;
        aout << "DEBUG: Vertex count: " << model.getVertexCount() << ", Index count: " << model.getIndexCount() << std::endl;
        attributesLogged = true;
    }
    
    // The position attribute is 3 floats
    glVertexAttribPointer(
            position_, // attrib
            3, // elements
            GL_FLOAT, // of type float
            GL_FALSE, // don't normalize
            sizeof(Vertex), // stride is Vertex bytes
            model.getVertexData() // pull from the start of the vertex data
    );
    glEnableVertexAttribArray(position_);

    // The uv attribute is 2 floats
    glVertexAttribPointer(
            uv_, // attrib
            2, // elements
            GL_FLOAT, // of type float
            GL_FALSE, // don't normalize
            sizeof(Vertex), // stride is Vertex bytes
            ((uint8_t *) model.getVertexData()) + sizeof(Vector3) // offset Vector3 from the start
    );
    glEnableVertexAttribArray(uv_);

    // Setup the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, model.getTexture().getTextureID());
    
    // Debug: Check texture binding
    GLint currentTexture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
    static bool textureLogged = false;
    if (!textureLogged) {
        aout << "DEBUG: Texture ID: " << model.getTexture().getTextureID() << ", Bound texture: " << currentTexture << std::endl;
        textureLogged = true;
    }

    // Draw as indexed triangles
    glDrawElements(GL_TRIANGLES, model.getIndexCount(), GL_UNSIGNED_SHORT, model.getIndexData());

    glDisableVertexAttribArray(uv_);
    glDisableVertexAttribArray(position_);
}

void Shader::setMVPMatrix(float *mvpMatrix) const {
    glUniformMatrix4fv(mvpMatrix_, 1, false, mvpMatrix);
}