# HoloPersona - 3D Human Figure with Jetpack Compose

This project demonstrates how to integrate a 3D human figure renderer (using OpenGL ES 3.0) with Jetpack Compose UI. It's based on the Android Studio Game Activity template but modified to work with standard Activity and Compose.

## What This App Does

- **3D Human Figure Background**: Renders a 3D human figure using native OpenGL ES 3.0
- **Jetpack Compose UI Overlay**: Modern Android UI built with Compose overlays the 3D scene
- **Interactive Controls**: Switch between different skeleton types (Basic, Detailed, Athletic, Slim)
- **Touch Interaction**: Rotate the 3D figure with touch gestures
- **Immersive Experience**: Full-screen rendering with hideable UI controls

## Key Modifications Made

### 1. Activity Architecture Change

- **From**: `GameActivity` (full-screen native)
- **To**: `ComponentActivity` with Jetpack Compose support
- **Result**: Enables UI overlay on top of 3D content

### 2. 3D Rendering Integration

- **Created**: `HoloPersonaGLSurfaceView` - Custom GLSurfaceView for 3D rendering
- **Created**: `GLSurfaceViewRenderer.cpp` - JNI bridge between Kotlin and native renderer
- **Bridge**: Native 3D renderer now works within Android View system

### 3. Skeleton System

- **4 Humanoid Types**: Basic, Detailed, Athletic, Slim
- **Real-time Switching**: Change skeleton types via Compose UI
- **Proper Proportions**: Anatomically correct human proportions

### 4. UI Design

- **Material Design 3**: Modern Compose UI with proper theming
- **Adaptive Controls**: Show/hide UI controls with floating action button
- **Semi-transparent Cards**: UI overlays with proper alpha blending

## Architecture

```
┌─────────────────────────────────────────────────────┐
│                  MainActivity                        │
│                 (ComponentActivity)                  │
├─────────────────────────────────────────────────────┤
│                Jetpack Compose UI                    │
│  ┌─────────────────────────────────────────────────┐│
│  │              UI Overlay                         ││
│  │  • Skeleton Type Selector                      ││
│  │  • Controls (Show/Hide)                        ││
│  │  • Material Design 3 Cards                     ││
│  └─────────────────────────────────────────────────┘│
├─────────────────────────────────────────────────────┤
│            HoloPersonaGLSurfaceView                  │
│  ┌─────────────────────────────────────────────────┐│
│  │           3D Rendering System                   ││
│  │  • OpenGL ES 3.0 Renderer                      ││
│  │  • Touch Input Handling                        ││
│  │  • Camera Controls                             ││
│  └─────────────────────────────────────────────────┘│
├─────────────────────────────────────────────────────┤
│              Native C++ Layer                        │
│  • GLSurfaceViewRenderer.cpp (JNI Bridge)           │
│  • SkeletonAsset.cpp (Human Figure Generation)      │
│  • Shader.cpp (OpenGL Shaders)                      │
│  • TextureAsset.cpp (Texture Management)            │
│  • Utility.cpp (3D Math)                            │
└─────────────────────────────────────────────────────┘
```

## Key Features

### 3D Rendering

- **OpenGL ES 3.0**: Modern graphics pipeline
- **Perspective Camera**: 3D camera with orbital controls
- **Real-time Rendering**: Smooth 60fps animation
- **Touch Controls**: Rotate camera by dragging
- **Proper Depth Testing**: Correct 3D depth rendering

### Human Figure System

- **Box-based Anatomy**: Humanoid figures built from box primitives
- **Proper Proportions**: Anatomically correct scaling
- **Multiple Body Types**: Different builds (athletic, slim, etc.)
- **Textured Rendering**: Simple checkerboard texture applied

### Compose Integration

- **AndroidView**: Embeds GLSurfaceView in Compose
- **State Management**: Reactive UI updates
- **Material Design**: Modern Android UI patterns
- **Responsive Layout**: Adapts to different screen sizes

## Files Modified/Created

### New Files

- `HoloPersonaGLSurfaceView.kt` - Custom GLSurfaceView for 3D rendering
- `GLSurfaceViewRenderer.cpp` - JNI bridge for native rendering
- `ui/theme/Theme.kt` - Jetpack Compose theme
- `ui/theme/Type.kt` - Typography definitions

### Modified Files

- `MainActivity.kt` - Complete rewrite for Compose integration
- `build.gradle.kts` - Added Compose dependencies
- `gradle/libs.versions.toml` - Updated dependency versions
- `CMakeLists.txt` - Updated native build configuration
- `TextureAsset.h/.cpp` - Added simple texture creation

### Removed Dependencies

- `androidx.games.activity` - No longer needed
- `game-activity` native libraries - Replaced with standard GLSurfaceView

## How It Works

1. **MainActivity** sets up Compose UI with `HoloPersonaApp` composable
2. **HoloPersonaApp** creates a `Box` layout with:
   - `AndroidView` containing `HoloPersonaGLSurfaceView` (full-screen background)
   - Compose UI overlay with controls
3. **HoloPersonaGLSurfaceView** manages OpenGL rendering and touch input
4. **GLSurfaceViewRenderer.cpp** bridges Kotlin calls to native 3D rendering
5. **SkeletonAsset** generates different human figure types
6. **User interaction** updates both 3D scene and Compose UI reactively

## Building and Running

```bash
# Build the project
./gradlew build

# Install on device
./gradlew installDebug
```

## Requirements

- Android SDK 26+ (Android 8.0+)
- OpenGL ES 3.0 support
- Kotlin 1.9.24
- Jetpack Compose BOM 2024.04.01

## Future Enhancements

- **Asset Loading**: Load actual 3D models instead of box primitives
- **Animation System**: Add skeletal animation support
- **Advanced Materials**: PBR materials and lighting
- **UI Improvements**: More interactive controls
- **Performance**: LOD system and optimization

This demonstrates a successful integration of native 3D rendering with modern Android UI, providing a foundation for apps requiring both 3D graphics and rich user interfaces.
# holo-persona
