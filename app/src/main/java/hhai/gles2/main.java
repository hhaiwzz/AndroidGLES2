package hhai.gles2;

import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Surface;
import android.widget.TextView;

import java.io.File;

public class main extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
//        System.loadLibrary("JniHelper");
//        System.loadLibrary("JniCall");
        System.loadLibrary("JniLib");
    }

    private GLSurfaceView surfaceView;
    private SimpleGLRenderer simpleRenderer = new SimpleGLRenderer();

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());

        // setup GLES2 surface view
        surfaceView = (GLSurfaceView) findViewById(R.id.surface_view);
        surfaceView.setRenderer(simpleRenderer);
        surfaceView.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);

        String currentPath = getFilesDir().getAbsolutePath();

        // create shaders folder
        File folder = new File(currentPath + File.separator + "Shaders");
        if (!folder.exists())
        {
            if (folder.mkdir())
            {
                Log.i("Main Activity", "Create Shaders folder");
            }
            else
            {
                Log.e("Main Activity", "Shaders folder not created");
            }
        }

        // create jniHelper object in native code
        AssetManager assetManager = getAssets();
        Log.i("Main Activity", "Initialize JniHelper");
        Init(assetManager, currentPath);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    private native void Init(AssetManager assetManager, String currentPath);
}
