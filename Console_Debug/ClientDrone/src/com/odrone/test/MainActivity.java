package com.odrone.test;

import java.util.Arrays;
import java.util.List;
import org.apache.http.message.BasicNameValuePair;
import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;
import android.widget.Button;
import android.widget.EditText;
import android.view.View;
import android.os.Handler;
import android.os.Message;
import java.net.URI;

public class MainActivity extends Activity implements View.OnClickListener
{

    WebSocketClient _client;
    TextView _logger;
    Button _connectionButton;
    Button _messageButton;
    EditText _message;
    Handler _messageQueue;

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        _logger = (TextView)findViewById(R.id.logger);
        _logger.setText("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");


        _connectionButton = (Button) findViewById(R.id.connectionButton);
        _connectionButton.setOnClickListener(this);

        _messageButton = (Button) findViewById(R.id.messageButton);
        _messageButton.setOnClickListener(this);

        _message = (EditText) findViewById(R.id.message);

        _messageQueue = new Handler() {
            @Override
            public void handleMessage(Message message) {

                _logger.setText("Receive : " + message.obj + "\n" + _logger.getText());
            }
        };

        List<BasicNameValuePair> extraHeaders = Arrays.asList(
            new BasicNameValuePair("Sec-WebSocket-Protocol", "odrone-protocol")
        );
        _client = new WebSocketClient(URI.create("ws://192.168.1.100:9000/drone"), new WebSocketClient.Listener() {
            @Override
            public void onConnect() {
                Log.d("Receiver", "Connected!");
                _client.send("auth");
            }

            @Override
            public void onMessage(String msg) {
                Log.d("Receiver", String.format("Got string message! %s", msg));
                Message message = new Message();
                message.obj = msg;
                _messageQueue.sendMessage(message);
            }

            @Override
            public void onMessage(byte[] data) {
            }

            @Override
            public void onDisconnect(int code, String reason) {
                Log.d("Receiver", String.format("Disconnected! Code: %d Reason: %s", code, reason));
            }

            @Override
            public void onError(Exception error) {
                Log.e("Receiver", "Error!", error);
            }
        }, extraHeaders);
    }


    @Override
    public void onClick(View button) {

        if (_connectionButton.equals(button)) {

            if (_connectionButton.getText().equals("Connect")) {

                _client.connect();
                _connectionButton.setText("Disconnect");
            }
            else {
                _client.disconnect();
                _connectionButton.setText("Connect");
            }
        }
        else if (_messageButton.equals(button)) {

            String message = _message.getText().toString();
            _client.send(message);
            _logger.setText("Send : " + message + "\n" + _logger.getText());
        }
    }
}
