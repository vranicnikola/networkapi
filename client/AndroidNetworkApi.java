package phd.remoteaircondition;

import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

import android.util.Log;

import phd.remoteaircondition.NetworkException.NetworkErrorType;

public class AndroidNetworkApi {

    public static final int SERVER_DEFAULT_PORT = 500;
    
    public static final String TAG = "ANDROID_NETWORK_API";
    
    private InetAddress serverAddress;

    private Socket socket;

    private CommsThread commsThread;
    
    private boolean connected;
    
    public AndroidNetworkApi(){
        connected = false;
    }
    
    public void connect(String serverIp, int serverPort)throws UnknownHostException, IOException{
        
        // ---create a socket---
        serverAddress = InetAddress.getByName(serverIp);
    
        // --remember to change the IP address above to match your own--
        //socket = new Socket(serverAddress, serverPort);
        socket = new Socket(serverIp, serverPort);
        commsThread = new CommsThread(socket);
        commsThread.start();
        
        connected = true;
    }
    
    public void disconect()throws IOException{
        commsThread.close();
        connected = false;
    }
    
    public boolean isConnected(){
        return connected;
    }
    
    public void powerOn() throws IOException, InterruptedException, NetworkException{
        NetworkMessage request = new NetworkMessage(NetwrokMessagesID.NM_POWER_ON_REQUEST, 0, null);
        commsThread.write(request);
        NetworkMessage response = commsThread.readMessage();
        if(response.getIdEnum() == NetwrokMessagesID.NM_POWER_ON_DONE){
            return;
        }
        else if(response.getIdEnum() == NetwrokMessagesID.NM_POWER_ON_FAILD){
            throw new NetworkException(NetworkErrorType.NM_POWER_ON_FAILD);
        }
        else{
            Log.e(TAG, response.getIdDes());
            throw new NetworkException(NetworkErrorType.NM_UNKNOWN_MESSAGE);
        }
    }
    
    public void powerOff() throws IOException, InterruptedException, NetworkException{
        NetworkMessage request = new NetworkMessage(NetwrokMessagesID.NM_POWER_OFF_REQUEST, 0, null);
        commsThread.write(request);
        NetworkMessage response = commsThread.readMessage();
        if(response.getIdEnum() == NetwrokMessagesID.NM_POWER_OFF_DONE){
            return;
        }
        else if(response.getIdEnum() == NetwrokMessagesID.NM_POWER_OFF_FAILD){
            throw new NetworkException(NetworkErrorType.NM_POWER_OFF_FAILD);
        }
        else{
            Log.e(TAG, response.getIdDes());
            throw new NetworkException(NetworkErrorType.NM_UNKNOWN_MESSAGE);
        }
    }
    
    public void increaseTemperature() throws IOException, InterruptedException, NetworkException{
        NetworkMessage request = new NetworkMessage(NetwrokMessagesID.NM_INCREASE_TEMPERATURE_REQUEST, 0, null);
        commsThread.write(request);
        NetworkMessage response = commsThread.readMessage();
        if(response.getIdEnum() == NetwrokMessagesID.NM_INCREASE_TEMPERATURE_DONE){
            return;
        }
        else if(response.getIdEnum() == NetwrokMessagesID.NM_INCREASE_TEMPERATURE_FAILD){
            throw new NetworkException(NetworkErrorType.NM_INCREASE_TEMPERATURE_FAILD);
        }
        else{
            Log.e(TAG, response.getIdDes());
            throw new NetworkException(NetworkErrorType.NM_UNKNOWN_MESSAGE);
        }
    }
    
    public void decreaseTemperature() throws IOException, InterruptedException, NetworkException{
        NetworkMessage request = new NetworkMessage(NetwrokMessagesID.NM_DECREASE_TEMPERATURE_REQUEST, 0, null);
        commsThread.write(request);
        NetworkMessage response = commsThread.readMessage();
        if(response.getIdEnum() == NetwrokMessagesID.NM_DECREASE_TEMPERATURE_DONE){
            return;
        }
        else if(response.getIdEnum() == NetwrokMessagesID.NM_DECREASE_TEMPERATURE_FAILD){
            throw new NetworkException(NetworkErrorType.NM_DECREASE_TEMPERATURE_FAILD);
        }
        else{
            Log.e(TAG, response.getIdDes());
            throw new NetworkException(NetworkErrorType.NM_UNKNOWN_MESSAGE);
        }
    }

    public void increaseExhaust() throws IOException, InterruptedException, NetworkException{
        NetworkMessage request = new NetworkMessage(NetwrokMessagesID.NM_INCREASE_EXHAUST_REQUEST, 0, null);
        commsThread.write(request);
        NetworkMessage response = commsThread.readMessage();
        if(response.getIdEnum() == NetwrokMessagesID.NM_INCREASE_EXHAUST_DONE){
            return;
        }
        else if(response.getIdEnum() == NetwrokMessagesID.NM_INCREASE_EXHAUST_FAILD){
            throw new NetworkException(NetworkErrorType.NM_INCREASE_EXHAUST_FAILD);
        }
        else{
            Log.e(TAG, response.getIdDes());
            throw new NetworkException(NetworkErrorType.NM_UNKNOWN_MESSAGE);
        }
    }
    
    public void decreaseExhaust() throws IOException, InterruptedException, NetworkException{
        NetworkMessage request = new NetworkMessage(NetwrokMessagesID.NM_DECREASE_EXHAUST_REQUEST, 0, null);
        commsThread.write(request);
        NetworkMessage response = commsThread.readMessage();
        if(response.getIdEnum() == NetwrokMessagesID.NM_DECREASE_EXHAUST_DONE){
            return;
        }
        else if(response.getIdEnum() == NetwrokMessagesID.NM_DECREASE_EXHAUST_FAILD){
            throw new NetworkException(NetworkErrorType.NM_DECREASE_EXHAUST_FAILD);
        }
        else{
            Log.e(TAG, response.getIdDes());
            throw new NetworkException(NetworkErrorType.NM_UNKNOWN_MESSAGE);
        }
    }
        
    public enum NetwrokMessagesID{
        NM_NO_ID,
        
        NM_POWER_ON_REQUEST,
        NM_POWER_ON_DONE,
        NM_POWER_ON_FAILD,

        NM_POWER_OFF_REQUEST,
        NM_POWER_OFF_DONE,
        NM_POWER_OFF_FAILD,
        
        NM_INCREASE_TEMPERATURE_REQUEST,
        NM_INCREASE_TEMPERATURE_DONE,
        NM_INCREASE_TEMPERATURE_FAILD,

        NM_DECREASE_TEMPERATURE_REQUEST,
        NM_DECREASE_TEMPERATURE_DONE,
        NM_DECREASE_TEMPERATURE_FAILD,

        NM_INCREASE_EXHAUST_REQUEST,
        NM_INCREASE_EXHAUST_DONE,
        NM_INCREASE_EXHAUST_FAILD,

        NM_DECREASE_EXHAUST_REQUEST,
        NM_DECREASE_EXHAUST_DONE,
        NM_DECREASE_EXHAUST_FAILD,
    };
    
    public static final String NETWORK_MESSAGE_ID_DES [] = {
        "NO MESSAGE",
        
        "NM_POWER_ON_REQUEST",
        "NM_POWER_ON_DONE",
        "NM_POWER_ON_FAILD",

        "NM_POWER_OFF_REQUEST",
        "NM_POWER_OFF_DONE",
        "NM_POWER_OFF_FAILD",
        
        "NM_INCREASE_TEMPERATURE_REQUEST",
        "NM_INCREASE_TEMPERATURE_DONE",
        "NM_INCREASE_TEMPERATURE_FAILD",

        "NM_DECREASE_TEMPERATURE_REQUEST",
        "NM_DECREASE_TEMPERATURE_DONE",
        "NM_DECREASE_TEMPERATURE_FAILD",

        "NM_INCREASE_EXHAUST_REQUEST",
        "NM_INCREASE_EXHAUST_DONE",
        "NM_INCREASE_EXHAUST_FAILD",

        "NM_DECREASE_EXHAUST_REQUEST",
        "NM_DECREASE_EXHAUST_DONE",
        "NM_DECREASE_EXHAUST_FAILD",
    };
}
