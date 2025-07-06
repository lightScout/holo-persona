package org.lightscout.holopersona

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.AttributeSet
import android.view.MotionEvent
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class HoloPersonaGLSurfaceView
@JvmOverloads
constructor(context: Context, attrs: AttributeSet? = null) : GLSurfaceView(context, attrs) {

    private val renderer: HoloPersonaRenderer

    init {
        // Create an OpenGL ES 3.0 context
        setEGLContextClientVersion(3)

        // Set up the renderer
        renderer = HoloPersonaRenderer(context)
        setRenderer(renderer)

        // Render the view only when there is a change in the drawing data
        renderMode = RENDERMODE_CONTINUOUSLY
    }

    override fun onTouchEvent(e: MotionEvent): Boolean {
        // Handle touch events for camera control
        when (e.action) {
            MotionEvent.ACTION_DOWN -> {
                renderer.onTouchDown(e.x, e.y)
            }
            MotionEvent.ACTION_MOVE -> {
                renderer.onTouchMove(e.x, e.y)
            }
            MotionEvent.ACTION_UP -> {
                renderer.onTouchUp()
            }
        }
        return true
    }

    private class HoloPersonaRenderer(private val context: Context) : GLSurfaceView.Renderer {

        private var surfaceWidth: Int = 0
        private var surfaceHeight: Int = 0
        private var lastTouchX: Float = 0f
        private var lastTouchY: Float = 0f
        private var touchDown: Boolean = false

        external fun nativeOnSurfaceCreated()
        external fun nativeOnSurfaceChanged(width: Int, height: Int)
        external fun nativeOnDrawFrame()
        external fun nativeOnTouchDown(x: Float, y: Float)
        external fun nativeOnTouchMove(x: Float, y: Float)
        external fun nativeOnTouchUp()
        external fun nativeSetSkeletonType(skeletonType: Int)
        external fun nativeSetAssetManager(assetManager: android.content.res.AssetManager)
        external fun nativeSetUseObjLoader(useObjLoader: Boolean)

        override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
            // Initialize AssetManager in native code
            nativeSetAssetManager(context.assets)
            nativeOnSurfaceCreated()
        }

        override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
            surfaceWidth = width
            surfaceHeight = height
            nativeOnSurfaceChanged(width, height)
        }

        override fun onDrawFrame(gl: GL10?) {
            nativeOnDrawFrame()
        }

        fun onTouchDown(x: Float, y: Float) {
            lastTouchX = x
            lastTouchY = y
            touchDown = true
            nativeOnTouchDown(x, y)
        }

        fun onTouchMove(x: Float, y: Float) {
            if (touchDown) {
                nativeOnTouchMove(x, y)
                lastTouchX = x
                lastTouchY = y
            }
        }

        fun onTouchUp() {
            touchDown = false
            nativeOnTouchUp()
        }

        fun setSkeletonType(skeletonType: Int) {
            nativeSetSkeletonType(skeletonType)
        }

        fun setUseObjLoader(useObjLoader: Boolean) {
            nativeSetUseObjLoader(useObjLoader)
        }
    }

    fun setSkeletonType(skeletonType: Int) {
        renderer.setSkeletonType(skeletonType)
    }

    fun setUseObjLoader(useObjLoader: Boolean) {
        renderer.setUseObjLoader(useObjLoader)
    }

    companion object {
        init {
            System.loadLibrary("holopersona")
        }
    }
}
