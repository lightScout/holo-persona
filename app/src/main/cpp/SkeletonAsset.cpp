#include "SkeletonAsset.h"

void SkeletonAsset::createSkeleton(SkeletonType type, 
                                  std::vector<Vertex>& vertices, 
                                  std::vector<Index>& indices) {
    switch (type) {
        case SkeletonType::BASIC_HUMANOID:
            createBasicHumanoid(vertices, indices);
            break;
        case SkeletonType::DETAILED_HUMANOID:
            createDetailedHumanoid(vertices, indices);
            break;
        case SkeletonType::ATHLETIC_HUMANOID:
            createAthleticHumanoid(vertices, indices);
            break;
        case SkeletonType::SLIM_HUMANOID:
            createSlimHumanoid(vertices, indices);
            break;
    }
}

void SkeletonAsset::createBasicHumanoid(std::vector<Vertex>& vertices, 
                                       std::vector<Index>& indices) {
    // Simple humanoid with basic proportions
    // HEAD
    addBox(vertices, indices, 0.0f, 7.0f, 0.0f, 1.0f, 1.0f, 1.0f);
    
    // TORSO
    addBox(vertices, indices, 0.0f, 4.0f, 0.0f, 1.5f, 2.5f, 0.8f);
    
    // ARMS
    addBox(vertices, indices, -2.0f, 4.5f, 0.0f, 1.0f, 0.3f, 0.3f);
    addBox(vertices, indices, 2.0f, 4.5f, 0.0f, 1.0f, 0.3f, 0.3f);
    
    // LEGS
    addBox(vertices, indices, -0.4f, 0.0f, 0.0f, 0.4f, 4.0f, 0.4f);
    addBox(vertices, indices, 0.4f, 0.0f, 0.0f, 0.4f, 4.0f, 0.4f);
}

void SkeletonAsset::createDetailedHumanoid(std::vector<Vertex>& vertices, 
                                          std::vector<Index>& indices) {
    // Detailed humanoid with proper anatomy (current implementation)
    // HEAD
    addBox(vertices, indices, 0.0f, 7.5f, 0.0f, 1.2f, 1.2f, 1.0f);
    
    // NECK
    addBox(vertices, indices, 0.0f, 6.0f, 0.0f, 0.5f, 1.0f, 0.5f);
    
    // TORSO (chest/ribcage)
    addBox(vertices, indices, 0.0f, 4.0f, 0.0f, 2.0f, 3.0f, 1.0f);
    
    // PELVIS (at origin - center of character)
    addBox(vertices, indices, 0.0f, 0.0f, 0.0f, 1.8f, 1.0f, 1.0f);
    
    // LEFT ARM
    addBox(vertices, indices, -2.5f, 4.5f, 0.0f, 1.5f, 0.4f, 0.4f); // Upper arm
    addBox(vertices, indices, -4.5f, 4.5f, 0.0f, 1.5f, 0.3f, 0.3f); // Forearm
    addBox(vertices, indices, -6.0f, 4.5f, 0.0f, 0.8f, 0.3f, 0.2f); // Hand
    
    // RIGHT ARM
    addBox(vertices, indices, 2.5f, 4.5f, 0.0f, 1.5f, 0.4f, 0.4f);  // Upper arm
    addBox(vertices, indices, 4.5f, 4.5f, 0.0f, 1.5f, 0.3f, 0.3f);  // Forearm
    addBox(vertices, indices, 6.0f, 4.5f, 0.0f, 0.8f, 0.3f, 0.2f);  // Hand
    
    // LEFT LEG
    addBox(vertices, indices, -0.6f, -2.5f, 0.0f, 0.6f, 3.0f, 0.6f); // Thigh
    addBox(vertices, indices, -0.6f, -6.5f, 0.0f, 0.5f, 3.0f, 0.5f); // Shin
    addBox(vertices, indices, -0.6f, -8.5f, 0.8f, 0.4f, 0.4f, 1.5f); // Foot
    
    // RIGHT LEG
    addBox(vertices, indices, 0.6f, -2.5f, 0.0f, 0.6f, 3.0f, 0.6f);  // Thigh
    addBox(vertices, indices, 0.6f, -6.5f, 0.0f, 0.5f, 3.0f, 0.5f);  // Shin
    addBox(vertices, indices, 0.6f, -8.5f, 0.8f, 0.4f, 0.4f, 1.5f);  // Foot
}

void SkeletonAsset::createAthleticHumanoid(std::vector<Vertex>& vertices, 
                                          std::vector<Index>& indices) {
    // Athletic build with broader shoulders and chest
    // HEAD
    addBox(vertices, indices, 0.0f, 7.5f, 0.0f, 1.3f, 1.3f, 1.1f);
    
    // NECK (slightly thicker)
    addBox(vertices, indices, 0.0f, 6.0f, 0.0f, 0.6f, 1.0f, 0.6f);
    
    // TORSO (broader chest)
    addBox(vertices, indices, 0.0f, 4.0f, 0.0f, 2.4f, 3.2f, 1.2f);
    
    // PELVIS
    addBox(vertices, indices, 0.0f, 0.0f, 0.0f, 1.8f, 1.0f, 1.0f);
    
    // LEFT ARM (more muscular)
    addBox(vertices, indices, -2.7f, 4.5f, 0.0f, 1.8f, 0.5f, 0.5f); // Upper arm
    addBox(vertices, indices, -4.8f, 4.5f, 0.0f, 1.6f, 0.4f, 0.4f); // Forearm
    addBox(vertices, indices, -6.2f, 4.5f, 0.0f, 0.9f, 0.4f, 0.3f); // Hand
    
    // RIGHT ARM (more muscular)
    addBox(vertices, indices, 2.7f, 4.5f, 0.0f, 1.8f, 0.5f, 0.5f);  // Upper arm
    addBox(vertices, indices, 4.8f, 4.5f, 0.0f, 1.6f, 0.4f, 0.4f);  // Forearm
    addBox(vertices, indices, 6.2f, 4.5f, 0.0f, 0.9f, 0.4f, 0.3f);  // Hand
    
    // LEFT LEG (more muscular)
    addBox(vertices, indices, -0.7f, -2.5f, 0.0f, 0.8f, 3.2f, 0.8f); // Thigh
    addBox(vertices, indices, -0.7f, -6.5f, 0.0f, 0.6f, 3.0f, 0.6f); // Shin
    addBox(vertices, indices, -0.7f, -8.5f, 0.8f, 0.5f, 0.5f, 1.6f); // Foot
    
    // RIGHT LEG (more muscular)
    addBox(vertices, indices, 0.7f, -2.5f, 0.0f, 0.8f, 3.2f, 0.8f);  // Thigh
    addBox(vertices, indices, 0.7f, -6.5f, 0.0f, 0.6f, 3.0f, 0.6f);  // Shin
    addBox(vertices, indices, 0.7f, -8.5f, 0.8f, 0.5f, 0.5f, 1.6f);  // Foot
}

void SkeletonAsset::createSlimHumanoid(std::vector<Vertex>& vertices, 
                                      std::vector<Index>& indices) {
    // Slim build with narrower proportions
    // HEAD (slightly smaller)
    addBox(vertices, indices, 0.0f, 7.5f, 0.0f, 1.0f, 1.0f, 0.9f);
    
    // NECK (thinner)
    addBox(vertices, indices, 0.0f, 6.0f, 0.0f, 0.4f, 1.0f, 0.4f);
    
    // TORSO (narrower)
    addBox(vertices, indices, 0.0f, 4.0f, 0.0f, 1.6f, 2.8f, 0.8f);
    
    // PELVIS (narrower)
    addBox(vertices, indices, 0.0f, 0.0f, 0.0f, 1.4f, 0.8f, 0.8f);
    
    // LEFT ARM (slimmer)
    addBox(vertices, indices, -2.3f, 4.5f, 0.0f, 1.2f, 0.3f, 0.3f); // Upper arm
    addBox(vertices, indices, -4.2f, 4.5f, 0.0f, 1.2f, 0.25f, 0.25f); // Forearm
    addBox(vertices, indices, -5.6f, 4.5f, 0.0f, 0.7f, 0.25f, 0.15f); // Hand
    
    // RIGHT ARM (slimmer)
    addBox(vertices, indices, 2.3f, 4.5f, 0.0f, 1.2f, 0.3f, 0.3f);  // Upper arm
    addBox(vertices, indices, 4.2f, 4.5f, 0.0f, 1.2f, 0.25f, 0.25f);  // Forearm
    addBox(vertices, indices, 5.6f, 4.5f, 0.0f, 0.7f, 0.25f, 0.15f);  // Hand
    
    // LEFT LEG (slimmer)
    addBox(vertices, indices, -0.5f, -2.5f, 0.0f, 0.5f, 2.8f, 0.5f); // Thigh
    addBox(vertices, indices, -0.5f, -6.5f, 0.0f, 0.4f, 2.8f, 0.4f); // Shin
    addBox(vertices, indices, -0.5f, -8.5f, 0.7f, 0.35f, 0.35f, 1.3f); // Foot
    
    // RIGHT LEG (slimmer)
    addBox(vertices, indices, 0.5f, -2.5f, 0.0f, 0.5f, 2.8f, 0.5f);  // Thigh
    addBox(vertices, indices, 0.5f, -6.5f, 0.0f, 0.4f, 2.8f, 0.4f);  // Shin
    addBox(vertices, indices, 0.5f, -8.5f, 0.7f, 0.35f, 0.35f, 1.3f);  // Foot
}

void SkeletonAsset::addBox(std::vector<Vertex>& vertices, 
                          std::vector<Index>& indices,
                          float centerX, float centerY, float centerZ,
                          float width, float height, float depth) {
    Index baseIndex = vertices.size();
    
    float halfW = width * 0.5f;
    float halfH = height * 0.5f;
    float halfD = depth * 0.5f;
    
    // Add 24 vertices (4 per face) for this box
    // Front face
    vertices.emplace_back(Vector3{centerX - halfW, centerY + halfH, centerZ + halfD}, Vector2{0.0f, 0.0f});
    vertices.emplace_back(Vector3{centerX + halfW, centerY + halfH, centerZ + halfD}, Vector2{1.0f, 0.0f});
    vertices.emplace_back(Vector3{centerX + halfW, centerY - halfH, centerZ + halfD}, Vector2{1.0f, 1.0f});
    vertices.emplace_back(Vector3{centerX - halfW, centerY - halfH, centerZ + halfD}, Vector2{0.0f, 1.0f});
    
    // Back face
    vertices.emplace_back(Vector3{centerX - halfW, centerY + halfH, centerZ - halfD}, Vector2{1.0f, 0.0f});
    vertices.emplace_back(Vector3{centerX + halfW, centerY + halfH, centerZ - halfD}, Vector2{0.0f, 0.0f});
    vertices.emplace_back(Vector3{centerX + halfW, centerY - halfH, centerZ - halfD}, Vector2{0.0f, 1.0f});
    vertices.emplace_back(Vector3{centerX - halfW, centerY - halfH, centerZ - halfD}, Vector2{1.0f, 1.0f});
    
    // Right face
    vertices.emplace_back(Vector3{centerX + halfW, centerY + halfH, centerZ + halfD}, Vector2{0.0f, 0.0f});
    vertices.emplace_back(Vector3{centerX + halfW, centerY + halfH, centerZ - halfD}, Vector2{1.0f, 0.0f});
    vertices.emplace_back(Vector3{centerX + halfW, centerY - halfH, centerZ - halfD}, Vector2{1.0f, 1.0f});
    vertices.emplace_back(Vector3{centerX + halfW, centerY - halfH, centerZ + halfD}, Vector2{0.0f, 1.0f});
    
    // Left face
    vertices.emplace_back(Vector3{centerX - halfW, centerY + halfH, centerZ - halfD}, Vector2{0.0f, 0.0f});
    vertices.emplace_back(Vector3{centerX - halfW, centerY + halfH, centerZ + halfD}, Vector2{1.0f, 0.0f});
    vertices.emplace_back(Vector3{centerX - halfW, centerY - halfH, centerZ + halfD}, Vector2{1.0f, 1.0f});
    vertices.emplace_back(Vector3{centerX - halfW, centerY - halfH, centerZ - halfD}, Vector2{0.0f, 1.0f});
    
    // Top face
    vertices.emplace_back(Vector3{centerX - halfW, centerY + halfH, centerZ - halfD}, Vector2{0.0f, 0.0f});
    vertices.emplace_back(Vector3{centerX + halfW, centerY + halfH, centerZ - halfD}, Vector2{1.0f, 0.0f});
    vertices.emplace_back(Vector3{centerX + halfW, centerY + halfH, centerZ + halfD}, Vector2{1.0f, 1.0f});
    vertices.emplace_back(Vector3{centerX - halfW, centerY + halfH, centerZ + halfD}, Vector2{0.0f, 1.0f});
    
    // Bottom face
    vertices.emplace_back(Vector3{centerX - halfW, centerY - halfH, centerZ + halfD}, Vector2{0.0f, 0.0f});
    vertices.emplace_back(Vector3{centerX + halfW, centerY - halfH, centerZ + halfD}, Vector2{1.0f, 0.0f});
    vertices.emplace_back(Vector3{centerX + halfW, centerY - halfH, centerZ - halfD}, Vector2{1.0f, 1.0f});
    vertices.emplace_back(Vector3{centerX - halfW, centerY - halfH, centerZ - halfD}, Vector2{0.0f, 1.0f});
    
    // Add indices for this box (12 triangles, 2 per face)
    for (int face = 0; face < 6; face++) {
        Index faceBase = baseIndex + face * 4;
        // Triangle 1
        indices.push_back(faceBase + 0);
        indices.push_back(faceBase + 1);
        indices.push_back(faceBase + 2);
        // Triangle 2
        indices.push_back(faceBase + 0);
        indices.push_back(faceBase + 2);
        indices.push_back(faceBase + 3);
    }
} 