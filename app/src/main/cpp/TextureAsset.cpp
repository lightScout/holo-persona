#define STB_IMAGE_IMPLEMENTATION
#include "TextureAsset.h"
#include "AndroidOut.h"
#include "Utility.h"
#include <GLES3/gl3.h>

// Single header image loading library - works with all Android API levels
#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG
#include "stb_image.h"

std::shared_ptr<TextureAsset>
TextureAsset::loadAsset(AAssetManager *assetManager, const std::string &assetPath) {
    // Get the image from asset manager
    auto pAsset = AAssetManager_open(
            assetManager,
            assetPath.c_str(),
            AASSET_MODE_BUFFER);
    
    if (!pAsset) {
        aout << "Failed to open asset: " << assetPath << std::endl;
        return nullptr;
    }

    // Get the asset size and data
    off_t assetSize = AAsset_getLength(pAsset);
    const void* assetData = AAsset_getBuffer(pAsset);
    
    if (!assetData) {
        aout << "Failed to get asset buffer: " << assetPath << std::endl;
        AAsset_close(pAsset);
        return nullptr;
    }

    // Use stb_image to decode the image data
    int width, height, channels;
    unsigned char* imageData = stbi_load_from_memory(
        static_cast<const stbi_uc*>(assetData),
        static_cast<int>(assetSize),
        &width, &height, &channels, 4  // Force RGBA format
    );

    // Close the asset as we've copied the data
    AAsset_close(pAsset);
    
    if (!imageData) {
        aout << "Failed to decode image: " << assetPath << std::endl;
        return nullptr;
    }

    // Get an opengl texture
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    // Clamp to the edge, you'll get odd results alpha blending if you don't
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load the texture into VRAM
    glTexImage2D(
            GL_TEXTURE_2D, // target
            0, // mip level
            GL_RGBA, // internal format
            width, // width of the texture
            height, // height of the texture
            0, // border (always 0)
            GL_RGBA, // format
            GL_UNSIGNED_BYTE, // type
            imageData // Data to upload
    );

    // generate mip levels. Not really needed for 2D, but good to do
    glGenerateMipmap(GL_TEXTURE_2D);

    // cleanup the image data
    stbi_image_free(imageData);

    // Create a shared pointer so it can be cleaned up easily/automatically
    return std::shared_ptr<TextureAsset>(new TextureAsset(textureId));
}

std::shared_ptr<TextureAsset> TextureAsset::createSimpleTexture() {
    // Create a simple 2x2 checkerboard pattern
    unsigned char textureData[16] = {
        255, 200, 150, 255,  // Light orange
        200, 150, 100, 255,  // Darker orange
        200, 150, 100, 255,  // Darker orange
        255, 200, 150, 255   // Light orange
    };
    
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    // Load the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
    
    // Create a shared pointer using the private constructor
    return std::shared_ptr<TextureAsset>(new TextureAsset(textureId));
}

TextureAsset::~TextureAsset() {
    // return texture resources
    glDeleteTextures(1, &textureID_);
    textureID_ = 0;
}