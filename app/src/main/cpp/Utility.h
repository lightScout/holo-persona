#ifndef ANDROIDGLINVESTIGATIONS_UTILITY_H
#define ANDROIDGLINVESTIGATIONS_UTILITY_H

#include <cassert>

class Utility {
public:
    static bool checkAndLogGlError(bool alwaysLog = false);

    static inline void assertGlError() { assert(checkAndLogGlError()); }

    /**
     * Generates an orthographic projection matrix given the half height, aspect ratio, near, and far
     * planes
     *
     * @param outMatrix the matrix to write into
     * @param halfHeight half of the height of the screen
     * @param aspect the width of the screen divided by the height
     * @param near the distance of the near plane
     * @param far the distance of the far plane
     * @return the generated matrix, this will be the same as @a outMatrix so you can chain calls
     *     together if needed
     */
    static float *buildOrthographicMatrix(
            float *outMatrix,
            float halfHeight,
            float aspect,
            float near,
            float far);

    /**
     * Generates a perspective projection matrix
     *
     * @param outMatrix the matrix to write into
     * @param fovY field of view in radians
     * @param aspect aspect ratio (width/height)
     * @param near distance to near plane
     * @param far distance to far plane
     * @return the generated matrix
     */
    static float *buildPerspectiveMatrix(
            float *outMatrix,
            float fovY,
            float aspect,
            float near,
            float far);

    /**
     * Generates a view matrix (look-at matrix)
     *
     * @param outMatrix the matrix to write into
     * @param eyeX camera position X
     * @param eyeY camera position Y
     * @param eyeZ camera position Z
     * @param centerX look-at target X
     * @param centerY look-at target Y
     * @param centerZ look-at target Z
     * @param upX up vector X
     * @param upY up vector Y
     * @param upZ up vector Z
     * @return the generated matrix
     */
    static float *buildViewMatrix(
            float *outMatrix,
            float eyeX, float eyeY, float eyeZ,
            float centerX, float centerY, float centerZ,
            float upX, float upY, float upZ);

    /**
     * Generates a model transformation matrix with rotation
     *
     * @param outMatrix the matrix to write into
     * @param rotationY rotation around Y axis in radians
     * @param posX position X
     * @param posY position Y
     * @param posZ position Z
     * @param scale uniform scale factor
     * @return the generated matrix
     */
    static float *buildModelMatrix(
            float *outMatrix,
            float rotationY,
            float posX, float posY, float posZ,
            float scale);

    /**
     * Multiplies two 4x4 matrices: result = matA * matB
     *
     * @param result output matrix
     * @param matA first matrix
     * @param matB second matrix
     * @return the result matrix
     */
    static float *multiplyMatrices(
            float *result,
            const float *matA,
            const float *matB);

    static float *buildIdentityMatrix(float *outMatrix);
};

#endif //ANDROIDGLINVESTIGATIONS_UTILITY_H