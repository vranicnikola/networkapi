package phd.remoteaircondition;

public class NetworkException extends Exception{
    
    public enum NetworkErrorType{
        NM_UNKNOWN_MESSAGE,
        
        NM_INCREASE_TEMPERATURE_FAILD,
        NM_DECREASE_TEMPERATURE_FAILD,
        NM_INCREASE_EXHAUST_FAILD,
        NM_DECREASE_EXHAUST_FAILD,

        NM_POWER_ON_FAILD,
        NM_POWER_OFF_FAILD,
    };
    
    private static final long serialVersionUID = 1L;

    private NetworkErrorType type;
    private String exMessage;
    
    public NetworkException(NetworkErrorType type){
        this.type = type;
        
        switch(type){
            case NM_UNKNOWN_MESSAGE : {
                exMessage = "unknown message!";
                break;
            }
            case NM_INCREASE_TEMPERATURE_FAILD:{
                exMessage = "increase temperature faild!";
                break;
            }
            case NM_DECREASE_TEMPERATURE_FAILD:{
                exMessage = "decrease temperature faild!";
                break;
            }
            case NM_INCREASE_EXHAUST_FAILD:{
                exMessage = "increase exhaust faild!";
                break;
            }
            case NM_DECREASE_EXHAUST_FAILD:{
                exMessage = "decrease exhaust faild!";
                break;
            }
            case NM_POWER_ON_FAILD:{
                exMessage = "Power On faild!";
                break;
            }
            case NM_POWER_OFF_FAILD:{
                exMessage = "Power Off faild!";
                break;
            }
            default : {
                exMessage = "unknown error type!";
            }           
        }
    }
    
    public NetworkErrorType getErrorType(){
        return type;
    }
    
    public String getMessage(){
        return exMessage;
    }
}

