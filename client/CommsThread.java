package phd.remoteaircondition;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.util.ArrayList;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;

import android.util.Log;

public class CommsThread extends Thread {
    
    public static final String TAG = "COMMS_THREAD";
    
    // in ms
    private static final int RECEIVE_MSG_TIMER = 1000;
    
    private final Socket socket;
    
    private final InputStream inputStream;
    
    private final OutputStream outputStream;
    
    private CircularBuffer circularBuffer;
    
    public static final int RECEIVE_BUFFER_SIZE = 1024 * 8;
    
    public ArrayList<NetworkMessage> receivePipeline;

    private Semaphore receivePipeSemaphore;
    private Semaphore receivedMsgs;
    
    public CommsThread(Socket sock) throws IOException{
        
        socket = sock;
        
        inputStream = socket.getInputStream();

        outputStream = socket.getOutputStream();
        
        circularBuffer = new CircularBuffer(RECEIVE_BUFFER_SIZE);
        
        receivePipeline = new ArrayList<NetworkMessage>();
        
        receivePipeSemaphore = new Semaphore(1);
        receivedMsgs = new Semaphore(0);
    }
    
    public NetworkMessage readMessage() throws InterruptedException{
        NetworkMessage networkMessage = new NetworkMessage();

        if(receivedMsgs.tryAcquire(RECEIVE_MSG_TIMER, TimeUnit.MILLISECONDS) == false){
            return networkMessage;            
        }
        
        receivePipeSemaphore.acquire();
        if(receivePipeline.size() > 0){
            networkMessage = receivePipeline.get(0);
            receivePipeline.remove(0);
        }
        receivePipeSemaphore.release();
        
        return networkMessage;
    }

    public void run() {
        // ---buffer store for the stream---
        byte[] receiveBuffer = new byte[1024];
        
        // ---bytes returned from read()---
        int bytes;
        
        // ---keep listening to the InputStream until an
        // exception occurs---
        while (true) {
            try {
                // ---read from the inputStream---
                bytes = inputStream.read(receiveBuffer);
                if(bytes == -1){
                    break;
                }
                
                //! DEBUG
                String msg = "Receive: ";
                for(int i = 0; i < bytes; i++){
                    msg += "0x" + Integer.toHexString(receiveBuffer[i]) + " ";
                }
                Log.i(TAG, msg);
                //! DEBUG
                
                circularBuffer.write(receiveBuffer, bytes);
                
                NetworkMessage networkMessage;
                
                while(true){
                    networkMessage = new NetworkMessage(circularBuffer);
                    if(networkMessage.isConstructorComplete() == true){
                        receivePipeSemaphore.acquire();
                        receivePipeline.add(networkMessage);
                        receivePipeSemaphore.release();
                        
                        receivedMsgs.release();
                    }
                    else{
                        break;
                    }
                }
            } 
            catch (IOException e) {
                break;
            } 
            catch(InterruptedException e){
                break;
            }
        }
    }

    public void write(NetworkMessage msg) throws IOException{    
        outputStream.write(intToBytes(msg.getId()));
        outputStream.write(intToBytes(msg.getSize()));
        
        if(msg.getData() != null)
            outputStream.write(msg.getData());
        
        //! DEBUG
        String log = "Send: ";
        byte [] m1 = intToBytes(msg.getId());
        byte [] m2 = intToBytes(msg.getSize());
        log += "m1: ";
        for(int i = 0; i < m1.length; i++){
            log += "0x" + Integer.toHexString(m1[i]) + " ";
        }
        log += "m2: ";
        for(int i = 0; i < m2.length; i++){
            log += "0x" + Integer.toHexString(m2[i]) + " ";
        }
        Log.i(TAG, log);
    }

    public void close() throws IOException{
        socket.close();
    }
    
    public static byte[] intToBytes(int my_int) throws IOException {
        byte [] ret = new byte[4];
        
        ret[0] = (byte) ((my_int >> 0) & 0xff);
        ret[1] = (byte) ((my_int >> 8) & 0xff);
        ret[2] = (byte) ((my_int >> 16) & 0xff);
        ret[3] = (byte) ((my_int >> 24) & 0xff);
        
        return ret;
    }
    
    public static int bytesToInt(byte[] int_bytes) throws IOException {
        int ret = 0;
        
        ret += int_bytes[0];
        ret += int_bytes[1] >> 8;
        ret += int_bytes[2] >> 16;
        ret += int_bytes[3] >> 24;
                
        return ret;
    }
    
    /*
     * public static byte[] intToBytes(int my_int) throws IOException {
     * ByteArrayOutputStream bos = new ByteArrayOutputStream(); ObjectOutput out
     * = new ObjectOutputStream(bos); out.writeInt(my_int); out.close(); byte[]
     * int_bytes = bos.toByteArray(); bos.close(); return int_bytes; }
     * 
     * public static int bytesToInt(byte[] int_bytes) throws IOException {
     * ByteArrayInputStream bis = new ByteArrayInputStream(int_bytes);
     * ObjectInputStream ois = new ObjectInputStream(bis); int my_int =
     * ois.readInt(); ois.close(); return my_int; }
     */
}
