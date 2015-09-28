package odrone.odrone_master;

import android.content.DialogInterface;
import android.content.Intent;
import android.content.Context;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;
import android.content.pm.PackageManager;
import android.content.pm.PackageInfo;
import android.view.View.OnClickListener;

import java.util.List;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        PackageManager pm = getPackageManager();

        List<PackageInfo> packages = pm.getInstalledPackages(PackageManager.GET_UNINSTALLED_PACKAGES);

        LinearLayout layout = (LinearLayout)findViewById(R.id.main_layout);

        String module_pfx;

        module_pfx = "odrone.module";
        module_pfx = "com.";
        module_pfx = "odrone.";

        for (final PackageInfo pack : packages) {

            if (pack.packageName.startsWith(module_pfx)) {
                Button button_module = new Button(this);

                final String name = pack.packageName.substring(module_pfx.length());

                button_module.setText("Module " + name);
                final Context context_backup = this;

                button_module.setOnClickListener(new OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        Intent intent = new Intent(context_backup, DisplayModuleStatus.class);

                        intent.putExtra("package_info", pack);
                        intent.putExtra("package_name", name);

                        startActivity(intent);
                    }
                });

                layout.addView(button_module);
            }
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
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
        } else if (id == R.id.action_install_modules) {
            Intent intent = new Intent(this, InstallModules.class);

            startActivity(intent);
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
