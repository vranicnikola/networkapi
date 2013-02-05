
package com.networkapi;

import java.util.HashMap;

public class Messages {

    public static final int NM_MSG_ID = 0;
    public static final int NM_UNKNOWN_MESSAGE = 1;

    public static final int NM_EXAMPLE_REQUEST = 2;
    public static final int NM_EXAMPLE_ON_DONE = 3;
    public static final int NM_EXAMPLE_FAILD = 4;

    private static HashMap<Integer, String> hashMap;

    public Messages() {
        hashMap = new HashMap<Integer, String>();
        hashMap.put(NM_MSG_ID, "No message");
        hashMap.put(NM_UNKNOWN_MESSAGE, "Unknown message");

        hashMap.put(NM_EXAMPLE_REQUEST, "Example request");
        hashMap.put(NM_EXAMPLE_ON_DONE, "Example done");
        hashMap.put(NM_EXAMPLE_FAILD, "Example faild");
    }

    public static String add(int type, String description) {
        return hashMap.put(type, description);
    }

    public static String get(int type) {
        return hashMap.get(type);
    }
}
