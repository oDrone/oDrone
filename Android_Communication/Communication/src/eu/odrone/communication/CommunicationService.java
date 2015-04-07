package eu.odrone.communication;

import android.app.Service;
import android.widget.Toast;
import android.os.Messenger;
import android.os.Message;
import android.os.Handler;
import android.os.IBinder;
import android.os.Bundle;
import android.content.Intent;
import android.util.Log;
import java.util.HashMap;
import android.os.RemoteException;

public class CommunicationService extends Service {

    private HashMap<Short, Messenger> _modules = new HashMap<Short, Messenger>();

    class IncomingHandler extends Handler {

        @Override
        public void handleMessage(Message msg) {

            Bundle data = msg.getData();
            String action = data.getString("action");

            if (action.equals("add")) {
            
                _modules.put(data.getShort("id", (short)0), msg.replyTo);
                Log.d("ODRONE_COM", data.getString("name") + "[" + data.getShort("id") + "] added !");
            }
            else if (action.equals("count")) {

                Message newMessage = Message.obtain();

                Bundle data2 = new Bundle();
                data2.putString("count", "" + _modules.size());
                newMessage.setData(data2);

                try {
                    if (msg.replyTo != null) {
                        msg.replyTo.send(newMessage);
                    }
                } catch (RemoteException e) {

                    e.printStackTrace();
                }
                Log.d("ODRONE_COM", "Count action");
            }
            else {

                Log.d("ODRONE_COM", "Unknown action : " + action);
            }
        }
    }

    final Messenger _messenger = new Messenger(new IncomingHandler());

    @Override
    public IBinder onBind(Intent intent) {

        Log.d("ODRONE_COM", "Binding...");
        return _messenger.getBinder();
    }
}
