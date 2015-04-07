package eu.odrone.stabilization;

import android.app.Activity;
import android.os.Bundle;
import android.os.Messenger;
import android.os.Message;
import android.os.IBinder;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.ComponentName;
import android.os.Bundle;
import android.os.RemoteException;
import android.util.Log;

import eu.odrone.R;

public class MainActivity extends Activity {

    private Messenger _messenger;
    private boolean _isBound;

    @Override
    public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.stabilization_main);
    }

    @Override
    public void onStart() {

        super.onStart();

        Intent intent = new Intent("eu.odrone.communication.CommunicationService");
        bindService(intent, _serviceConnection, BIND_AUTO_CREATE);
    }

    private void sendMessage() {

        if(_isBound) {

            Message newMessage = Message.obtain();
            
            Bundle data = new Bundle();
            data.putString("action", "add");
            data.putString("name", "stabilization");
            data.putShort("id", (short)101);
            newMessage.setData(data);
            
            try {

                _messenger.send(newMessage);
            } catch (RemoteException e) {

                e.printStackTrace();
            }
        }
    }

    private ServiceConnection _serviceConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName className, IBinder binder) {
            
            Log.d("ODRONE_COM", "Stabilization connected !");
            _messenger = new Messenger(binder);
            _isBound = true;
            sendMessage();
        }

        @Override
        public void onServiceDisconnected(ComponentName className) {
            
            _messenger = null;
            _isBound = false;
        }
    };
}
