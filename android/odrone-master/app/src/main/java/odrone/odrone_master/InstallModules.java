package odrone.odrone_master;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.content.Context;
import android.text.util.Linkify;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.os.AsyncTask;
import java.net.URL;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.ProtocolException;
import java.io.IOException;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import org.json.JSONObject;
import org.json.JSONArray;
import org.json.JSONException;

public class InstallModules extends AppCompatActivity {
    final Context self = this;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_install_modules);

        try {
            new HttpAsyncTask().execute(new URL("http://api.odrone.eu"));
        } catch (MalformedURLException e) {
            e.printStackTrace();
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_install_modules, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    public static String GET(URL url) {
        try {
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
            connection.setRequestMethod("GET");
            connection.setDoOutput(true);
            connection.setReadTimeout(10000);
            connection.connect();

            int status = connection.getResponseCode();

            switch (status) {
                case 200:
                case 201:
                    BufferedReader br = new BufferedReader(new InputStreamReader(connection.getInputStream()));
                    StringBuilder sb = new StringBuilder();
                    String line;
                    while ((line = br.readLine()) != null) {
                        sb.append(line+"\n");
                    }
                    br.close();
                    return sb.toString();
            }


        } catch (ProtocolException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return null;
    }


    private class HttpAsyncTask extends AsyncTask<URL, Void, String> {
        @Override
        protected String doInBackground(URL... urls) {
            return GET(urls[0]);
        }

        protected void onPostExecute(String res) {
            try {
                JSONObject json = new JSONObject(res);
                JSONArray array = json.getJSONArray("modules");

                LinearLayout layout = (LinearLayout)findViewById(R.id.install_modules_layout);

                System.out.printf("%d\n", array.length());

                for (int i = 0; i < array.length(); ++i) {
                    JSONObject one_module = array.getJSONObject(i);

                    TextView view = new TextView(self);
                    view.setText("Name: " + one_module.getString("Name"));
                    layout.addView(view);
                    view = new TextView(self);
                    view.setText("Description: " + one_module.getString("Desc"));
                    layout.addView(view);
                    view = new TextView(self);
                    view.setText(one_module.getString("apk"));
                    view.setAutoLinkMask(Linkify.WEB_URLS);
                    layout.addView(view);
                }

            } catch (JSONException e) {
                e.printStackTrace();;
            }

        }
    }
}
