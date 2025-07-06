#ifndef HOLOPERSONA_OBJLOADER_H
#define HOLOPERSONA_OBJLOADER_H

#include "Model.h"
#include <vector>
#include <string>
#include <android/asset_manager.h>

/*!
 * A class for loading 3D models from OBJ files.
 * Supports basic OBJ format with vertices, texture coordinates, and faces.
 */
class ObjLoader {
public:
    /*!
     * Loads an OBJ file from Android assets
     * @param assetManager Android asset manager
     * @param filename Path to OBJ file in assets folder
     * @param vertices Output vector for vertex data
     * @param indices Output vector for index data
     * @return true if successful, false otherwise
     */
    static bool loadFromAssets(AAssetManager* assetManager,
                              const std::string& filename,
                              std::vector<Vertex>& vertices,
                              std::vector<Index>& indices);

    /*!
     * Loads an OBJ file from a string buffer
     * @param objData String containing OBJ file content
     * @param vertices Output vector for vertex data
     * @param indices Output vector for index data
     * @return true if successful, false otherwise
     */
    static bool loadFromString(const std::string& objData,
                              std::vector<Vertex>& vertices,
                              std::vector<Index>& indices);

private:
    struct ObjVertex {
        float x, y, z;
    };
    
    struct ObjTexCoord {
        float u, v;
    };
    
    struct ObjFace {
        int v1, v2, v3;      // Vertex indices
        int vt1, vt2, vt3;   // Texture coordinate indices
    };
    
    /*!
     * Parses a single line from an OBJ file
     */
    static void parseLine(const std::string& line,
                         std::vector<ObjVertex>& objVertices,
                         std::vector<ObjTexCoord>& objTexCoords,
                         std::vector<ObjFace>& objFaces);
    
    /*!
     * Converts parsed OBJ data to our Vertex/Index format
     */
    static void convertToModel(const std::vector<ObjVertex>& objVertices,
                              const std::vector<ObjTexCoord>& objTexCoords,
                              const std::vector<ObjFace>& objFaces,
                              std::vector<Vertex>& vertices,
                              std::vector<Index>& indices);
};

#endif //HOLOPERSONA_OBJLOADER_H 