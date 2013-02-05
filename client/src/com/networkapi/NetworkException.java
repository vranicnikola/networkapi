
package com.networkapi;

public class NetworkException extends Exception {

    private static final long serialVersionUID = 1L;

    private String exMessage;
    private int type;

    public NetworkException(int type) {
        this.type = type;
        exMessage = Messages.get(type);
    }

    public int getErrorType() {
        return type;
    }

    public String getMessage() {
        return exMessage;
    }
}
