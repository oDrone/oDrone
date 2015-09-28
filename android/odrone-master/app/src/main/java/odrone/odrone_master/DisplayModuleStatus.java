package odrone.odrone_master;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.widget.TextView;

public class DisplayModuleStatus extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_display_module_status);

        Intent intent = getIntent();
        final PackageInfo pack = intent.getParcelableExtra("package_info");
        final String package_name = intent.getStringExtra("package_name");

        setTitle("Module " + package_name);

        /* fill the information table */
        TextView version = (TextView)findViewById(R.id.module_version);
        TextView description = (TextView)findViewById(R.id.module_desc);
        TextView status = (TextView)findViewById(R.id.module_status);

        version.setText(new Integer(pack.versionCode).toString());
        status.setText("Installed / Running");

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_display_module_status, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        return super.onOptionsItemSelected(item);
    }
}
