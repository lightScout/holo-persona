#include "ObjLoader.h"
#include "AndroidOut.h"
#include <sstream>
#include <algorithm>

bool ObjLoader::loadFromAssets(AAssetManager* assetManager,
                              const std::string& filename,
                              std::vector<Vertex>& vertices,
                              std::vector<Index>& indices) {
    if (!assetManager) {
        aout << "ERROR: AssetManager is null" << std::endl;
        return false;
    }
    
    // Open the asset file
    AAsset* asset = AAssetManager_open(assetManager, filename.c_str(), AASSET_MODE_BUFFER);
    if (!asset) {
        aout << "ERROR: Could not open OBJ file: " << filename << std::endl;
        return false;
    }
    
    // Get file size and read content
    size_t fileSize = AAsset_getLength(asset);
    std::string objData;
    objData.resize(fileSize);
    
    int bytesRead = AAsset_read(asset, &objData[0], fileSize);
    AAsset_close(asset);
    
    if (bytesRead != fileSize) {
        aout << "ERROR: Could not read complete OBJ file: " << filename << std::endl;
        return false;
    }
    
    aout << "DEBUG: Loaded OBJ file " << filename << " (" << fileSize << " bytes)" << std::endl;
    
    // Parse the OBJ data
    return loadFromString(objData, vertices, indices);
}

bool ObjLoader::loadFromString(const std::string& objData,
                              std::vector<Vertex>& vertices,
                              std::vector<Index>& indices) {
    vertices.clear();
    indices.clear();
    
    std::vector<ObjVertex> objVertices;
    std::vector<ObjTexCoord> objTexCoords;
    std::vector<ObjFace> objFaces;
    
    // Parse line by line
    std::istringstream stream(objData);
    std::string line;
    int lineNumber = 0;
    
    while (std::getline(stream, line)) {
        lineNumber++;
        
        // Remove leading/trailing whitespace
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        try {
            parseLine(line, objVertices, objTexCoords, objFaces);
        } catch (const std::exception& e) {
            aout << "ERROR: Failed to parse line " << lineNumber << ": " << line << std::endl;
            aout << "Exception: " << e.what() << std::endl;
            return false;
        }
    }
    
    aout << "DEBUG: Parsed " << objVertices.size() << " vertices, " 
         << objTexCoords.size() << " texture coords, " 
         << objFaces.size() << " faces" << std::endl;
    
    if (objVertices.empty() || objFaces.empty()) {
        aout << "ERROR: OBJ file contains no geometry" << std::endl;
        return false;
    }
    
    // Convert to our format
    convertToModel(objVertices, objTexCoords, objFaces, vertices, indices);
    
    aout << "DEBUG: Converted to " << vertices.size() << " vertices, " 
         << indices.size() << " indices" << std::endl;
    
    return true;
}

void ObjLoader::parseLine(const std::string& line,
                         std::vector<ObjVertex>& objVertices,
                         std::vector<ObjTexCoord>& objTexCoords,
                         std::vector<ObjFace>& objFaces) {
    std::istringstream stream(line);
    std::string prefix;
    stream >> prefix;
    
    if (prefix == "v") {
        // Vertex: v x y z
        ObjVertex vertex;
        stream >> vertex.x >> vertex.y >> vertex.z;
        objVertices.push_back(vertex);
        
    } else if (prefix == "vt") {
        // Texture coordinate: vt u v
        ObjTexCoord texCoord;
        stream >> texCoord.u >> texCoord.v;
        objTexCoords.push_back(texCoord);
        
    } else if (prefix == "f") {
        // Face: f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
        // We support both f v1/vt1 v2/vt2 v3/vt3 and f v1//vn1 v2//vn2 v3//vn3
        // For simplicity, we'll extract just vertex and texture indices
        
        std::string faceData;
        std::vector<std::string> faceVertices;
        
        // Read all face vertex data
        while (stream >> faceData) {
            faceVertices.push_back(faceData);
        }
        
        if (faceVertices.size() < 3) {
            throw std::runtime_error("Face must have at least 3 vertices");
        }
        
        // Parse first 3 vertices (we assume triangulated faces)
        ObjFace face;
        
        for (int i = 0; i < 3; i++) {
            std::string vertexData = faceVertices[i];
            std::istringstream vertexStream(vertexData);
            std::string indexStr;
            
            // Parse vertex index (required)
            if (std::getline(vertexStream, indexStr, '/')) {
                int vertexIndex = std::stoi(indexStr);
                // OBJ indices are 1-based, convert to 0-based
                vertexIndex = (vertexIndex > 0) ? vertexIndex - 1 : vertexIndex + objVertices.size();
                
                if (i == 0) face.v1 = vertexIndex;
                else if (i == 1) face.v2 = vertexIndex;
                else face.v3 = vertexIndex;
            }
            
            // Parse texture coordinate index (optional)
            if (std::getline(vertexStream, indexStr, '/') && !indexStr.empty()) {
                int texIndex = std::stoi(indexStr);
                // OBJ indices are 1-based, convert to 0-based
                texIndex = (texIndex > 0) ? texIndex - 1 : texIndex + objTexCoords.size();
                
                if (i == 0) face.vt1 = texIndex;
                else if (i == 1) face.vt2 = texIndex;
                else face.vt3 = texIndex;
            } else {
                // No texture coordinates - use default
                if (i == 0) face.vt1 = -1;
                else if (i == 1) face.vt2 = -1;
                else face.vt3 = -1;
            }
        }
        
        objFaces.push_back(face);
    }
    // Ignore other prefixes (vn, g, s, etc.)
}

void ObjLoader::convertToModel(const std::vector<ObjVertex>& objVertices,
                              const std::vector<ObjTexCoord>& objTexCoords,
                              const std::vector<ObjFace>& objFaces,
                              std::vector<Vertex>& vertices,
                              std::vector<Index>& indices) {
    vertices.clear();
    indices.clear();
    
    // For each face, create vertices
    for (const auto& face : objFaces) {
        // Get vertex positions
        const ObjVertex& v1 = objVertices[face.v1];
        const ObjVertex& v2 = objVertices[face.v2];
        const ObjVertex& v3 = objVertices[face.v3];
        
        // Get texture coordinates (or use defaults)
        Vector2 uv1{0.0f, 0.0f}, uv2{0.5f, 1.0f}, uv3{1.0f, 0.0f};
        
        if (face.vt1 >= 0 && face.vt1 < objTexCoords.size()) {
            uv1 = {objTexCoords[face.vt1].u, objTexCoords[face.vt1].v};
        }
        if (face.vt2 >= 0 && face.vt2 < objTexCoords.size()) {
            uv2 = {objTexCoords[face.vt2].u, objTexCoords[face.vt2].v};
        }
        if (face.vt3 >= 0 && face.vt3 < objTexCoords.size()) {
            uv3 = {objTexCoords[face.vt3].u, objTexCoords[face.vt3].v};
        }
        
        // Create vertices
        Index baseIndex = vertices.size();
        vertices.emplace_back(Vector3{v1.x, v1.y, v1.z}, uv1);
        vertices.emplace_back(Vector3{v2.x, v2.y, v2.z}, uv2);
        vertices.emplace_back(Vector3{v3.x, v3.y, v3.z}, uv3);
        
        // Create indices for triangle
        indices.push_back(baseIndex + 0);
        indices.push_back(baseIndex + 1);
        indices.push_back(baseIndex + 2);
    }
} 