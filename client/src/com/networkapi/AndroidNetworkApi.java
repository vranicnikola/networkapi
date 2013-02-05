
package com.networkapi;

import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

public class AndroidNetworkApi {

    public static final int SERVER_DEFAULT_PORT = 500;

    public static final String TAG = "ANDROID_NETWORK_API";

    private InetAddress serverAddress;

    private Socket socket;

    protected CommsThread commsThread;

    private boolean connected;

    public static Messages messages;

    public AndroidNetworkApi() {
        connected = false;
    }

    public void connect(String serverIp, int serverPort)
            throws UnknownHostException, IOException {
        messages = new Messages();
        serverAddress = InetAddress.getByName(serverIp);
        socket = new Socket(serverIp, serverPort);
        commsThread = new CommsThread(socket);
        commsThread.start();

        connected = true;
    }

    public void disconect() throws IOException {
        commsThread.close();
        connected = false;
    }

    public boolean isConnected() {
        return connected;
    }

    public void example() throws IOException, InterruptedException,
            NetworkException {
        NetworkMessage request = new NetworkMessage(
                Messages.NM_EXAMPLE_REQUEST, 0, null);
        commsThread.write(request);
        NetworkMessage response = commsThread.readMessage();
        if (response.getId() == Messages.NM_EXAMPLE_ON_DONE) {
            return;
        } else if (response.getId() == Messages.NM_EXAMPLE_FAILD) {
            throw new NetworkException(Messages.NM_EXAMPLE_FAILD);
        } else {
            throw new NetworkException(Messages.NM_UNKNOWN_MESSAGE);
        }
    }
}
