
package com.networkapi;

import java.util.concurrent.Semaphore;

public class CircularBuffer {

    private byte[] buffer;

    private int head, tail, lenght;

    private boolean bufferFull;

    private Semaphore sem;

    public CircularBuffer(int size) {
        this.lenght = size;

        buffer = new byte[size];

        head = tail = 0;

        bufferFull = false;

        sem = new Semaphore(1);
    }

    public void write(byte[] values) throws InterruptedException {
        write(values, values.length);
    }

    public void write(byte[] values, int size) throws InterruptedException {
        sem.acquire();

        for (int i = 0; i < size; i++) {
            buffer[tail++] = values[i];

            tail = tail % lenght;

            if (tail > head)
                bufferFull = (tail - head) == size;
            else
                bufferFull = (tail + lenght - head) == size;

            if (bufferFull == true)
                break;
        }

        sem.release();
    }

    public byte[] read() throws InterruptedException {

        sem.acquire();

        byte ret[];

        if (tail > head)
            ret = new byte[tail - head];
        else
            ret = new byte[tail + lenght - head];

        for (int i = 0; i < ret.length; i++) {
            ret[i] = buffer[head++];

            head = head % lenght;
        }

        sem.release();

        return ret;
    }

    /**
     * @brief Just read values from circular buffer. Don't change head or tail
     *        values.
     * @throws InterruptedException
     */
    public byte[] get() throws InterruptedException {
        sem.acquire();

        byte ret[];
        int headCopy = head;
        int iSize;

        if (tail == head) {
            iSize = 0;
        }
        else if (tail > head) {
            iSize = tail - head;
        }
        else {
            iSize = tail + lenght - head;
        }

        ret = new byte[iSize];

        for (int i = 0; i < ret.length; i++) {
            ret[i] = buffer[headCopy++];
            headCopy = headCopy % lenght;
        }

        sem.release();

        return ret;
    }

    public void erase(int howMuch) throws InterruptedException {
        sem.acquire();

        head += howMuch;
        head = head % lenght;

        sem.release();
    }

    public boolean isFull() {
        return bufferFull;
    }
}
