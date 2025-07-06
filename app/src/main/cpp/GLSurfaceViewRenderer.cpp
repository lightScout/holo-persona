#include <jni.h>
#include <memory>
#include <GLES3/gl3.h>
#include <cmath>

#include "AndroidOut.h"
#include "Renderer.h"
#include "Shader.h"
#include "Utility.h"
#include "TextureAsset.h"
#include "SkeletonAsset.h"

// Global variables to manage the renderer
static std::unique_ptr<Shader> gShader;
static std::vector<Model> gModels;
static int gWidth = 0;
static int gHeight = 0;
static int gCurrentSkeletonType = 1; // Default to DETAILED_HUMANOID
static int gRequestedSkeletonType = 1; // Thread-safe skeleton type switching
static bool gSkeletonTypeChanged = false; // Flag to indicate model recreation needed
static float gCharacterRotationY = 0.0f;
static float gCameraRotationY = 0.0f;
static bool gTouchActive = false;
static float gLastTouchX = 0.0f;
static float gLastTouchY = 0.0f;

// 3D rendering constants
static constexpr float kCameraDistance = 25.0f;     // Move camera back for better view
static constexpr float kCameraHeight = 0.0f;
static constexpr float kFieldOfView = 60.0f * M_PI / 180.0f;  // Wider field of view
static constexpr float kNearPlane = 0.1f;
static constexpr float kFarPlane = 100.0f;
static constexpr float kCharacterScale = 1.0f;     // Normal size character

// Simple test triangle for debugging
void createTestTriangle(std::vector<Vertex>& vertices, std::vector<Index>& indices) {
    vertices.clear();
    indices.clear();
    
    // Create a large triangle in front of camera
    vertices.emplace_back(Vector3{0.0f, 2.0f, 0.0f}, Vector2{0.5f, 0.0f});   // Top
    vertices.emplace_back(Vector3{-2.0f, -2.0f, 0.0f}, Vector2{0.0f, 1.0f}); // Bottom left  
    vertices.emplace_back(Vector3{2.0f, -2.0f, 0.0f}, Vector2{1.0f, 1.0f});  // Bottom right
    
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    
    aout << "DEBUG: Created test triangle with 3 vertices" << std::endl;
}

// Vertex shader (same as original)
static const char *vertex = R"vertex(#version 300 es
in vec3 inPosition;
in vec2 inUV;

out vec2 fragUV;

uniform mat4 uMVP;

void main() {
    fragUV = inUV;
    gl_Position = uMVP * vec4(inPosition, 1.0);
}
)vertex";

// Fragment shader (same as original)
static const char *fragment = R"fragment(#version 300 es
precision mediump float;

in vec2 fragUV;

uniform sampler2D uTexture;

out vec4 outColor;

void main() {
    outColor = texture(uTexture, fragUV);
}
)fragment";

void createModels() {
    gModels.clear();
    
    try {
        // Create vertex and index arrays
        std::vector<Vertex> vertices;
        std::vector<Index> indices;
        
        // Use the skeleton model (restored from original)
        SkeletonAsset::SkeletonType skeletonType = static_cast<SkeletonAsset::SkeletonType>(gCurrentSkeletonType);
        SkeletonAsset::createSkeleton(skeletonType, vertices, indices);
        
        aout << "DEBUG: Created skeleton with " << vertices.size() << " vertices, " << indices.size() << " indices" << std::endl;
        
        if (vertices.empty() || indices.empty()) {
            aout << "ERROR: Skeleton creation produced empty geometry!" << std::endl;
            return;
        }
    
    // Debug: Print first few vertices to check coordinates
    if (vertices.size() > 0) {
        aout << "DEBUG: First vertex: (" << vertices[0].position.x << ", " << vertices[0].position.y << ", " << vertices[0].position.z << ")" << std::endl;
        if (vertices.size() > 1) {
            aout << "DEBUG: Second vertex: (" << vertices[1].position.x << ", " << vertices[1].position.y << ", " << vertices[1].position.z << ")" << std::endl;
        }
    }
    
    // Create texture asset using a simple colored texture
    auto spTexture = TextureAsset::createSimpleTexture();
    
    if (!spTexture) {
        aout << "ERROR: Failed to create texture!" << std::endl;
        return;
    }
    
            // Create model
        gModels.emplace_back(std::move(vertices), std::move(indices), std::move(spTexture));
        aout << "DEBUG: Model created successfully" << std::endl;
        
    } catch (const std::exception& e) {
        aout << "ERROR: Exception during model creation: " << e.what() << std::endl;
        gModels.clear(); // Ensure we don't have partial models
    } catch (...) {
        aout << "ERROR: Unknown exception during model creation" << std::endl;
        gModels.clear(); // Ensure we don't have partial models
    }
}

extern "C" {

JNIEXPORT void JNICALL
Java_org_lightscout_holopersona_HoloPersonaGLSurfaceView_00024HoloPersonaRenderer_nativeOnSurfaceCreated(
        JNIEnv *env, jobject thiz) {
    
    aout << "GLSurfaceView: Surface created" << std::endl;
    
    // Initialize OpenGL state
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    // Temporarily disable face culling to see if triangles are facing wrong way
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    // glFrontFace(GL_CCW);
    
    aout << "DEBUG: OpenGL state initialized, face culling disabled" << std::endl;
    
    // Set clear color to cornflower blue
    glClearColor(100/255.f, 149/255.f, 237/255.f, 1.0f);
    
    // Create shader using the proper static method
    auto* shaderPtr = Shader::loadShader(vertex, fragment, "inPosition", "inUV", "uMVP");
    gShader = std::unique_ptr<Shader>(shaderPtr);
    
    if (!gShader) {
        aout << "ERROR: Failed to create shader!" << std::endl;
        return;
    }
    aout << "GLSurfaceView: Shader created successfully" << std::endl;
    
    // Set the texture sampler uniform to use texture unit 0
    gShader->activate();
    GLint textureUniform = glGetUniformLocation(gShader->getProgram(), "uTexture");
    if (textureUniform != -1) {
        glUniform1i(textureUniform, 0);  // Use texture unit 0
        aout << "DEBUG: Set texture uniform to texture unit 0" << std::endl;
    } else {
        aout << "ERROR: Could not find uTexture uniform!" << std::endl;
    }
    gShader->deactivate();
    
    // Create initial models
    createModels();
    
    aout << "GLSurfaceView: Created " << gModels.size() << " models" << std::endl;
    aout << "GLSurfaceView: Initialization complete" << std::endl;
}

JNIEXPORT void JNICALL
Java_org_lightscout_holopersona_HoloPersonaGLSurfaceView_00024HoloPersonaRenderer_nativeOnSurfaceChanged(
        JNIEnv *env, jobject thiz, jint width, jint height) {
    
    aout << "GLSurfaceView: Surface changed to " << width << "x" << height << std::endl;
    
    gWidth = width;
    gHeight = height;
    
    glViewport(0, 0, width, height);
}

JNIEXPORT void JNICALL
Java_org_lightscout_holopersona_HoloPersonaGLSurfaceView_00024HoloPersonaRenderer_nativeOnDrawFrame(
        JNIEnv *env, jobject thiz) {
    
    // Check if skeleton type needs to be changed (thread-safe)
    if (gSkeletonTypeChanged) {
        gCurrentSkeletonType = gRequestedSkeletonType;
        gSkeletonTypeChanged = false;
        if (gShader) {
            aout << "DEBUG: Recreating models for skeleton type " << gCurrentSkeletonType << std::endl;
            createModels();
        }
    }
    
    if (!gShader || gModels.empty() || gWidth == 0 || gHeight == 0) {
        // Debug output to see why rendering is skipped
        if (!gShader) {
            aout << "DEBUG: Skipping render - no shader" << std::endl;
        }
        if (gModels.empty()) {
            aout << "DEBUG: Skipping render - no models" << std::endl;
        }
        if (gWidth == 0 || gHeight == 0) {
            aout << "DEBUG: Skipping render - invalid size: " << gWidth << "x" << gHeight << std::endl;
        }
        return;
    }
    
    // Debug output (only print occasionally to avoid spam)
    static int frameCount = 0;
    if (frameCount % 120 == 0) {  // Print every 2 seconds at 60fps
        aout << "DEBUG: Rendering frame " << frameCount << ", models: " << gModels.size() << std::endl;
        aout << "DEBUG: Camera distance: " << kCameraDistance << ", Character scale: " << kCharacterScale << std::endl;
    }
    frameCount++;
    
    // Enable rotation for skeleton model
    gCharacterRotationY += 0.01f;
    if (gCharacterRotationY > 2.0f * M_PI) {
        gCharacterRotationY -= 2.0f * M_PI;
    }
    
    // Build perspective projection matrix
    float projectionMatrix[16] = {0};
    Utility::buildPerspectiveMatrix(
            projectionMatrix,
            kFieldOfView,
            float(gWidth) / float(gHeight),
            kNearPlane,
            kFarPlane);
    
    // Build view matrix (camera looking at triangle)
    float viewMatrix[16] = {0};
    
    // Simple fixed camera position for testing
    float cameraX = 0.0f;                      // Centered
    float cameraY = 0.0f;                      // At same height
    float cameraZ = kCameraDistance;           // In front of triangle
    
    // Debug camera position occasionally
    if (frameCount % 120 == 0) {
        aout << "DEBUG: Camera position: (" << cameraX << ", " << cameraY << ", " << cameraZ << ")" << std::endl;
    }
    
    Utility::buildViewMatrix(
            viewMatrix,
            cameraX, cameraY, cameraZ,          // Camera position
            0.0f, 0.0f, 0.0f,                   // Look at origin (triangle position)
            0.0f, 1.0f, 0.0f);                  // Up vector
    
    // Build model matrix (with scaling and rotation for skeleton)
    float modelMatrix[16] = {0};
    Utility::buildModelMatrix(modelMatrix, gCharacterRotationY, 0.0f, 0.0f, 0.0f, kCharacterScale);
    
    // Combine matrices: MVP = Projection * View * Model
    float viewProjectionMatrix[16] = {0};
    float mvpMatrix[16] = {0};
    
    Utility::multiplyMatrices(viewProjectionMatrix, projectionMatrix, viewMatrix);
    Utility::multiplyMatrices(mvpMatrix, viewProjectionMatrix, modelMatrix);
    
    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Check for OpenGL errors before rendering
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        aout << "OpenGL error before rendering: " << error << std::endl;
    }
    
    // Activate the shader program
    gShader->activate();
    
    // Check if shader program is active
    GLint currentProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    if (frameCount % 120 == 0) {
        aout << "DEBUG: Current shader program: " << currentProgram << std::endl;
    }
    
    // Set the combined MVP matrix
    gShader->setMVPMatrix(mvpMatrix);
    
    // Debug: Print matrix values occasionally
    if (frameCount % 120 == 0) {
        aout << "DEBUG: MVP Matrix:" << std::endl;
        for (int i = 0; i < 4; i++) {
            aout << "  [" << mvpMatrix[i*4] << ", " << mvpMatrix[i*4+1] << ", " << mvpMatrix[i*4+2] << ", " << mvpMatrix[i*4+3] << "]" << std::endl;
        }
    }
    
    // Render all the models
    for (const auto &model : gModels) {
        if (frameCount % 120 == 0) {
            aout << "DEBUG: Drawing model with " << model.getIndexCount() << " indices" << std::endl;
        }
        gShader->drawModel(model);
        
        // Check for OpenGL errors after drawing
        error = glGetError();
        if (error != GL_NO_ERROR) {
            aout << "OpenGL error after drawing model: " << error << std::endl;
        }
    }
    
    // Deactivate the shader program
    gShader->deactivate();
}

JNIEXPORT void JNICALL
Java_org_lightscout_holopersona_HoloPersonaGLSurfaceView_00024HoloPersonaRenderer_nativeOnTouchDown(
        JNIEnv *env, jobject thiz, jfloat x, jfloat y) {
    
    gTouchActive = true;
    gLastTouchX = x;
    gLastTouchY = y;
}

JNIEXPORT void JNICALL
Java_org_lightscout_holopersona_HoloPersonaGLSurfaceView_00024HoloPersonaRenderer_nativeOnTouchMove(
        JNIEnv *env, jobject thiz, jfloat x, jfloat y) {
    
    if (gTouchActive) {
        // Calculate touch delta
        float deltaX = x - gLastTouchX;
        float deltaY = y - gLastTouchY;
        
        // Update camera rotation based on horizontal movement
        gCameraRotationY += deltaX * 0.005f;
        
        // Clamp rotation to prevent overflow
        if (gCameraRotationY > 2.0f * M_PI) {
            gCameraRotationY -= 2.0f * M_PI;
        } else if (gCameraRotationY < -2.0f * M_PI) {
            gCameraRotationY += 2.0f * M_PI;
        }
        
        gLastTouchX = x;
        gLastTouchY = y;
    }
}

JNIEXPORT void JNICALL
Java_org_lightscout_holopersona_HoloPersonaGLSurfaceView_00024HoloPersonaRenderer_nativeOnTouchUp(
        JNIEnv *env, jobject thiz) {
    
    gTouchActive = false;
}

JNIEXPORT void JNICALL
Java_org_lightscout_holopersona_HoloPersonaGLSurfaceView_00024HoloPersonaRenderer_nativeSetSkeletonType(
        JNIEnv *env, jobject thiz, jint skeletonType) {
    
    aout << "GLSurfaceView: Requesting skeleton type change to " << skeletonType << std::endl;
    
    // Thread-safe skeleton type switching - defer model creation to render thread
    gRequestedSkeletonType = skeletonType;
    gSkeletonTypeChanged = true;
}

} 