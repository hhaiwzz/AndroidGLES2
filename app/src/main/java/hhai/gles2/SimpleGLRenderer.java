package hhai.gles2;

import android.app.Activity;
import android.app.Application;
import android.content.res.AssetManager;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.util.Log;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by hhai on 3/17/18.
 */

public class SimpleGLRenderer implements GLSurfaceView.Renderer {

    private final String SIMPLE_GL_RENDERER_TAG = "SimpleGLRenderer";

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        Log.i(SIMPLE_GL_RENDERER_TAG, "onSurfaceCreated");
        GLES20.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
//        Log.i(SIMPLE_GL_RENDERER_TAG, "onDrawFrame");
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        Log.i(SIMPLE_GL_RENDERER_TAG, "onSurfaceChanged");
        GLES20.glViewport(0, 0, width, height);
    }

    private native void Render();
}
