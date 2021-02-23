package com.example.android.simplejni;
 
import java.io.File;
import java.io.FileInputStream;
import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceHolder.Callback;
import android.view.SurfaceView;
 
public class MainActivity extends Activity {
 
	final private String TAG = "VideoCapture";
	private int width = 1280;
	private int height = 720;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		SurfaceView surfaceview = (SurfaceView) findViewById(R.id.surfaceView);
		SurfaceHolder holder = surfaceview.getHolder();
		holder.addCallback(new Callback(){
 
			@Override
			public void surfaceCreated(SurfaceHolder holder) {
				// TODO Auto-generated method stub
				Log.d(TAG,"surfaceCreated");
				nativeSetVideoSurface(holder.getSurface());

			}
 
			@Override
			public void surfaceChanged(SurfaceHolder holder, int format,
					int width, int height) {
				// TODO Auto-generated method stub
				
			}
 
			@Override
			public void surfaceDestroyed(SurfaceHolder holder) {
				// TODO Auto-generated method stub
				
			}});
	}
	

	private native boolean nativeSetVideoSurface(Surface surface);
	static {
        System.loadLibrary("vcapture");
    }
}