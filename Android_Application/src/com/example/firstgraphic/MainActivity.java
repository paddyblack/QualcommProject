package com.example.firstgraphic;



import java.net.URL; 
import java.util.StringTokenizer;

import org.json.JSONException ;
import org.json.JSONObject;
import org.json.JSONArray;

import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.series.DataPoint;
import com.jjoe64.graphview.series.LineGraphSeries;


import android.support.v7.app.ActionBarActivity;
import android.support.v7.app.ActionBar;
import android.support.v4.app.Fragment;
import android.app.ProgressDialog;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.widget.FrameLayout;
import android.widget.TextView;
import android.widget.Toast;
import android.os.Build;

public class MainActivity extends ActionBarActivity {


	private boolean paused=false;
	private Runnable runnable;
	public int i =1;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		//Every 10 seconds this thread collects the latest Data package made available by the second Qualcomm group
		runnable = new Runnable() {
			public void run() {
				while(!paused){

					new DataOccupancy().execute();
					synchronized(runnable){
						try {
							runnable.wait(5000);
						} catch (InterruptedException e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
					}
				}
			} 
		};

		Thread thread = new Thread(runnable);
		thread.start();

	}

	@Override
	public void onPause() {
		super.onPause();  
		paused=true;
	}

	@Override
	public void onResume() { 
		super.onResume();  
		paused=false;

	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {

		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
	}

	//This Asynctask fetches and retrieves the information contained in a JsonArray on the background thread.
	private class DataOccupancy extends AsyncTask<Void, Void, String[]> {

		@Override
		protected void onPreExecute() {
			super.onPreExecute();
		}

		@Override
		protected String[] doInBackground(Void... arg0) {
			// Creating service handler class instance
			ServiceHandler sh = new ServiceHandler();

			// Making a request to url and getting response
			String jsonStr = sh.makeServiceCall("http://qualoccupancy.azurewebsites.net/pollrooms.php", ServiceHandler.GET);

			// This array will be storing the different informations collected by the response
			String []data =new String[3];
			JSONObject c =null;

			try {		
				JSONArray jsonarray = new JSONArray(jsonStr);
				c=jsonarray.getJSONObject(0);
				//retrieves the relevant information in both these cases the Room ID and the number of people in it
				data[0]=(c.get("RoomID")+"");
				data[1]=(c.get("NumberOfPeople")+"");
			} catch (JSONException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			return data;
		}

		@Override
		protected void onPostExecute(String []result) {
			super.onPostExecute(result);
			// When doInbackground has finished executing this function is called and in our case updates the front end 

			TextView text =(TextView)findViewById(R.id.textnum);
			text.setText("Number of people in Room "+result[0]+" : "+result[1]);
			if(result[1]!=null){
				compareResults(Integer.parseInt(result[1]));
			}
		}

	}

	//It is not possible to directly save an array of integers therefore in SharedPreferences we break our occupancy data into a long string of values
	private void saveData(String result, int [] value){

		SharedPreferences sharedpreferences = getSharedPreferences("MyPrefs", MainActivity.MODE_PRIVATE);
		Editor editor = sharedpreferences.edit();

		String str="";
		for (int i = 0; i < value.length; i++) {
			str+=value[i]+",";
		}
		str+=result+",";
		Log.d("app", "RESULTS being saved "+str);
		editor.putString("RoomOccupancy", str.toString());
		editor.commit();  

	}

	//This function is in charge of comparing the last and current value of occupancy
	private void compareResults(int current){

		int []previous = retrieveData();
		if(current!=previous[previous.length-1])
			saveData(current+"",previous);
		int diff=current-previous[previous.length-1];
		if(diff>0){
			Toast.makeText(getApplicationContext(), "+"+diff+" people entering",Toast.LENGTH_SHORT).show();
		}else if(diff<0){
			Toast.makeText(getApplicationContext(), diff+" people leaving",Toast.LENGTH_SHORT).show();
		}else{
			Toast.makeText(getApplicationContext(), "No change",Toast.LENGTH_SHORT).show();
		}
		drawGraphs(previous);

	} 
	//Retrieves the past occupancy values for a given room
	private int[] retrieveData(){

		SharedPreferences sharedpreferences = getSharedPreferences("MyPrefs", MainActivity.MODE_PRIVATE);
		String savedString =sharedpreferences.getString("RoomOccupancy", "0,");
		Log.d("app", "SAVED String"+savedString);
		int length = (savedString.split(",").length); 
		StringTokenizer st = new StringTokenizer(savedString, ",");
		String[] parts = savedString.split(",");

		int[] savedList = new int[length];
		for (int j = 0; j < length; j++) {
			savedList[j] = Integer.parseInt(parts[j]+""); 
		}
		return savedList;
	}
	//This function is based on the external JArs of GraphView that have been added to our project 
	private void drawGraphs(int []data){
		i++;
		GraphView graph = new GraphView(MainActivity.this);
		//Create a list of DataPoint
		DataPoint [] datapoint= new DataPoint[data.length];
		for(int j = 0;j<data.length;j++){
			datapoint[j]=new DataPoint(j,data[j]);
		}
		//Adds the previous points to a Graph
		LineGraphSeries<DataPoint> series = new LineGraphSeries<DataPoint>(datapoint);
		graph.addSeries(series);
		FrameLayout frameview =(FrameLayout)findViewById(R.id.frameview);
		frameview.removeAllViews();
		frameview.addView(graph);
	}
} 


