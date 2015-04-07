package eu.odrone.communication;

import android.app.Activity;
import android.os.Bundle;
import android.os.Messenger;
import android.os.Message;
import android.os.IBinder;
import android.os.Handler;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.ComponentName;
import android.os.Bundle;
import android.os.RemoteException;
import android.util.Log;
import android.widget.TextView;

import eu.odrone.R;

public class MainActivity extends Activity {

    private Messenger _serviceMessenger;
    private Messenger _messenger;
    private boolean _isBound;
    private TextView _countText;

    class IncomingHandler extends Handler {

        @Override
        public void handleMessage(Message msg) {

            String count = msg.getData().getString("count");
            _countText.setText(count + " loaded modules");
        }
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.communication_main);

        _countText = (TextView)findViewById(R.id.count);

        Intent intent = new Intent("eu.odrone.communication.CommunicationService");
        bindService(intent, _serviceConnection, BIND_AUTO_CREATE);

        _messenger = new Messenger(new IncomingHandler());
    }

    @Override
    public void onResume() {

        Bundle data = new Bundle();
        data.putString("action", "count");
        sendMessage(data);
        super.onResume();
    }

    private void sendMessage(Bundle bundle) {

        if(_isBound) {

            Message newMessage = Message.obtain();
            newMessage.replyTo = _messenger;
            
            newMessage.setData(bundle);
            
            try {

                _serviceMessenger.send(newMessage);
            } catch (RemoteException e) {

                e.printStackTrace();
            }
        }
    }

    private ServiceConnection _serviceConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName className, IBinder binder) {
            
            Log.d("ODRONE_COM", "Communication connected !");
            _isBound = true;
            _serviceMessenger = new Messenger(binder);

            Bundle data = new Bundle();
            data.putString("action", "add");
            data.putString("name", "communication");
            data.putShort("id", (short)123);
            sendMessage(data);

            Bundle data2 = new Bundle();
            data2.putString("action", "count");
            sendMessage(data2);
        }

        @Override
        public void onServiceDisconnected(ComponentName className) {
            
            _serviceMessenger = null;
            _isBound = false;
        }
    };
}
