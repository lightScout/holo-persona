#include "Utility.h"
#include "AndroidOut.h"

#include <GLES3/gl3.h>
#include <cmath>

#define CHECK_ERROR(e) case e: aout << "GL Error: "#e << std::endl; break;

bool Utility::checkAndLogGlError(bool alwaysLog) {
    GLenum error = glGetError();
    if (error == GL_NO_ERROR) {
        if (alwaysLog) {
            aout << "No GL error" << std::endl;
        }
        return true;
    } else {
        switch (error) {
            CHECK_ERROR(GL_INVALID_ENUM);
            CHECK_ERROR(GL_INVALID_VALUE);
            CHECK_ERROR(GL_INVALID_OPERATION);
            CHECK_ERROR(GL_INVALID_FRAMEBUFFER_OPERATION);
            CHECK_ERROR(GL_OUT_OF_MEMORY);
            default:
                aout << "Unknown GL error: " << error << std::endl;
        }
        return false;
    }
}

float *
Utility::buildOrthographicMatrix(float *outMatrix, float halfHeight, float aspect, float near,
                                 float far) {
    float halfWidth = halfHeight * aspect;

    // column 1
    outMatrix[0] = 1.f / halfWidth;
    outMatrix[1] = 0.f;
    outMatrix[2] = 0.f;
    outMatrix[3] = 0.f;

    // column 2
    outMatrix[4] = 0.f;
    outMatrix[5] = 1.f / halfHeight;
    outMatrix[6] = 0.f;
    outMatrix[7] = 0.f;

    // column 3
    outMatrix[8] = 0.f;
    outMatrix[9] = 0.f;
    outMatrix[10] = -2.f / (far - near);
    outMatrix[11] = 0.f;

    // column 4
    outMatrix[12] = 0.f;
    outMatrix[13] = 0.f;
    outMatrix[14] = -(far + near) / (far - near);
    outMatrix[15] = 1.f;

    return outMatrix;
}

float *
Utility::buildPerspectiveMatrix(float *outMatrix, float fovY, float aspect, float near, float far) {
    float f = 1.0f / tanf(fovY * 0.5f);
    
    buildIdentityMatrix(outMatrix);
    
    // column 1
    outMatrix[0] = f / aspect;
    
    // column 2
    outMatrix[5] = f;
    
    // column 3
    outMatrix[10] = (far + near) / (near - far);
    outMatrix[11] = -1.0f;
    
    // column 4
    outMatrix[14] = (2.0f * far * near) / (near - far);
    outMatrix[15] = 0.0f;
    
    return outMatrix;
}

float *
Utility::buildViewMatrix(float *outMatrix, 
                        float eyeX, float eyeY, float eyeZ,
                        float centerX, float centerY, float centerZ,
                        float upX, float upY, float upZ) {
    // Calculate forward vector (from eye to center)
    float forwardX = centerX - eyeX;
    float forwardY = centerY - eyeY;
    float forwardZ = centerZ - eyeZ;
    
    // Normalize forward vector
    float forwardLength = sqrtf(forwardX * forwardX + forwardY * forwardY + forwardZ * forwardZ);
    forwardX /= forwardLength;
    forwardY /= forwardLength;
    forwardZ /= forwardLength;
    
    // Calculate right vector (cross product of forward and up)
    float rightX = forwardY * upZ - forwardZ * upY;
    float rightY = forwardZ * upX - forwardX * upZ;
    float rightZ = forwardX * upY - forwardY * upX;
    
    // Normalize right vector
    float rightLength = sqrtf(rightX * rightX + rightY * rightY + rightZ * rightZ);
    rightX /= rightLength;
    rightY /= rightLength;
    rightZ /= rightLength;
    
    // Calculate actual up vector (cross product of right and forward)
    float newUpX = rightY * forwardZ - rightZ * forwardY;
    float newUpY = rightZ * forwardX - rightX * forwardZ;
    float newUpZ = rightX * forwardY - rightY * forwardX;
    
    // Build view matrix
    // column 1
    outMatrix[0] = rightX;
    outMatrix[1] = newUpX;
    outMatrix[2] = -forwardX;
    outMatrix[3] = 0.0f;
    
    // column 2
    outMatrix[4] = rightY;
    outMatrix[5] = newUpY;
    outMatrix[6] = -forwardY;
    outMatrix[7] = 0.0f;
    
    // column 3
    outMatrix[8] = rightZ;
    outMatrix[9] = newUpZ;
    outMatrix[10] = -forwardZ;
    outMatrix[11] = 0.0f;
    
    // column 4 (translation)
    outMatrix[12] = -(rightX * eyeX + rightY * eyeY + rightZ * eyeZ);
    outMatrix[13] = -(newUpX * eyeX + newUpY * eyeY + newUpZ * eyeZ);
    outMatrix[14] = forwardX * eyeX + forwardY * eyeY + forwardZ * eyeZ;
    outMatrix[15] = 1.0f;
    
    return outMatrix;
}

float *
Utility::buildModelMatrix(float *outMatrix, float rotationY, 
                         float posX, float posY, float posZ, float scale) {
    float cosY = cosf(rotationY);
    float sinY = sinf(rotationY);
    
    // Build transformation matrix with rotation around Y-axis and translation
    // column 1
    outMatrix[0] = cosY * scale;
    outMatrix[1] = 0.0f;
    outMatrix[2] = sinY * scale;
    outMatrix[3] = 0.0f;
    
    // column 2
    outMatrix[4] = 0.0f;
    outMatrix[5] = scale;
    outMatrix[6] = 0.0f;
    outMatrix[7] = 0.0f;
    
    // column 3
    outMatrix[8] = -sinY * scale;
    outMatrix[9] = 0.0f;
    outMatrix[10] = cosY * scale;
    outMatrix[11] = 0.0f;
    
    // column 4 (translation)
    outMatrix[12] = posX;
    outMatrix[13] = posY;
    outMatrix[14] = posZ;
    outMatrix[15] = 1.0f;
    
    return outMatrix;
}

float *
Utility::multiplyMatrices(float *result, const float *matA, const float *matB) {
    // Fixed for column-major matrices used by OpenGL
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result[i * 4 + j] = 
                matA[0 * 4 + j] * matB[i * 4 + 0] +
                matA[1 * 4 + j] * matB[i * 4 + 1] +
                matA[2 * 4 + j] * matB[i * 4 + 2] +
                matA[3 * 4 + j] * matB[i * 4 + 3];
        }
    }
    return result;
}

float *Utility::buildIdentityMatrix(float *outMatrix) {
    // column 1
    outMatrix[0] = 1.f;
    outMatrix[1] = 0.f;
    outMatrix[2] = 0.f;
    outMatrix[3] = 0.f;

    // column 2
    outMatrix[4] = 0.f;
    outMatrix[5] = 1.f;
    outMatrix[6] = 0.f;
    outMatrix[7] = 0.f;

    // column 3
    outMatrix[8] = 0.f;
    outMatrix[9] = 0.f;
    outMatrix[10] = 1.f;
    outMatrix[11] = 0.f;

    // column 4
    outMatrix[12] = 0.f;
    outMatrix[13] = 0.f;
    outMatrix[14] = 0.f;
    outMatrix[15] = 1.f;

    return outMatrix;
}