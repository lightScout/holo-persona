# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

cmake_minimum_required(VERSION 3.22.1)

project("holopersona")

# Creates your game shared library. The name must be the same as the
# one used for loading in your Kotlin/Java or AndroidManifest.txt files.
add_library(holopersona SHARED
        GLSurfaceViewRenderer.cpp
        AndroidOut.cpp
        Shader.cpp
        TextureAsset.cpp
        Utility.cpp
        SkeletonAsset.cpp
        ObjLoader.cpp)

# Configure libraries CMake uses to link your target library.
target_link_libraries(holopersona
        # EGL and other dependent libraries required for drawing
        # and interacting with Android system
        EGL
        GLESv3
        jnigraphics
        android
        log)