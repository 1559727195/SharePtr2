package com.massky.shareptr;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.DatePicker;


public class MainActivity extends AppCompatActivity {
    private Button startButton;

    // Used to load the 'native-lib' library on application startup.
//    static {
//        System.loadLibrary("native-lib");
//    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.dev_model_time_select);
        nativeInit();
        startButton = (Button) findViewById(R.id.startButton);
        startButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                int threads = 3;
                int interations = 2;
                if (threads > 0 && interations > 0) {
                    startThreads(threads, interations);
                }
            }
        });

    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    private native  void nativeInit();

    private native  void nativeFree();

    private native  void nativeWorker(int id, int interations);

    static {
        System.loadLibrary("one-lib");
    }

    private void startThreads (int threads, int iterations) {

    }

    @Override
    protected void onDestroy() {
        nativeFree();
        super.onDestroy();

    }


    private void onNativeMessage (final String message) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                //
            }
        });
    }
}
