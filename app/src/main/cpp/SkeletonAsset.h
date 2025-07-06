#ifndef HOLOPERSONA_SKELETONASSET_H
#define HOLOPERSONA_SKELETONASSET_H

#include "Model.h"
#include <vector>
#include <memory>

/*!
 * A class for creating and managing skeletal mesh assets for 3D characters.
 * Provides predefined skeleton types and utilities for creating humanoid figures.
 */
class SkeletonAsset {
public:
    enum class SkeletonType {
        BASIC_HUMANOID,     // Simple humanoid with basic proportions
        DETAILED_HUMANOID,  // Detailed humanoid with proper anatomy
        ATHLETIC_HUMANOID,  // Athletic build with broader shoulders
        SLIM_HUMANOID      // Slim build with narrower proportions
    };

    /*!
     * Creates a skeletal mesh of the specified type
     * @param type The type of skeleton to create
     * @param vertices Output vector for vertex data
     * @param indices Output vector for index data
     */
    static void createSkeleton(SkeletonType type, 
                              std::vector<Vertex>& vertices, 
                              std::vector<Index>& indices);

    /*!
     * Creates a basic humanoid skeleton with standard proportions
     */
    static void createBasicHumanoid(std::vector<Vertex>& vertices, 
                                   std::vector<Index>& indices);

    /*!
     * Creates a detailed humanoid skeleton with proper anatomy
     */
    static void createDetailedHumanoid(std::vector<Vertex>& vertices, 
                                      std::vector<Index>& indices);

    /*!
     * Creates an athletic humanoid skeleton with broader build
     */
    static void createAthleticHumanoid(std::vector<Vertex>& vertices, 
                                      std::vector<Index>& indices);

    /*!
     * Creates a slim humanoid skeleton with narrower proportions
     */
    static void createSlimHumanoid(std::vector<Vertex>& vertices, 
                                  std::vector<Index>& indices);

private:
    /*!
     * Helper function to add a box primitive to the mesh
     * @param vertices Vertex array to append to
     * @param indices Index array to append to
     * @param centerX X position of box center
     * @param centerY Y position of box center
     * @param centerZ Z position of box center
     * @param width Box width
     * @param height Box height
     * @param depth Box depth
     */
    static void addBox(std::vector<Vertex>& vertices, 
                      std::vector<Index>& indices,
                      float centerX, float centerY, float centerZ,
                      float width, float height, float depth);
};

#endif //HOLOPERSONA_SKELETONASSET_H 