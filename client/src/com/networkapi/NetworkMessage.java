
package com.networkapi;

import java.io.IOException;

public class NetworkMessage {

    private int id;

    private int size;

    private byte[] data;

    private boolean constructorComplete;

    public NetworkMessage() {
        constructorComplete = true;
        id = Messages.NM_MSG_ID;
        size = 0;
        data = null;
    }

    public NetworkMessage(int id, int size, byte[] data) {
        this.id = id;
        this.size = size;
        this.data = data;
        constructorComplete = true;
    }

    public NetworkMessage(CircularBuffer circularBuffer) throws InterruptedException, IOException {
        constructorComplete = false;

        byte values[] = circularBuffer.get();

        if (values.length >= 4) {
            id = CommsThread.bytesToInt(get4bytes(values, 0));
        }

        if (values.length >= 8) {
            size = CommsThread.bytesToInt(get4bytes(values, 4));
        }

        if (values.length >= 8 + size) {
            data = new byte[size];

            for (int i = 0; i < size; i++) {
                data[i] = values[i + 8];
            }

            circularBuffer.erase(8 + size);

            constructorComplete = true;
        }
    }

    public boolean isConstructorComplete() {
        return constructorComplete;
    }

    private byte[] get4bytes(byte[] buffer, int pos) {
        byte ret[] = new byte[4];

        ret[0] = buffer[pos];
        ret[1] = buffer[pos + 1];
        ret[2] = buffer[pos + 2];
        ret[3] = buffer[pos + 3];

        return ret;
    }

    public int getId() {
        return id;
    }

    public String getIdDes() {
        return Messages.get(id);
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getSize() {
        return size;
    }

    public void setSize(int size) {
        this.size = size;
    }

    public byte[] getData() {
        return data;
    }

    public void setData(byte[] data) {
        this.data = data;
    }
}
