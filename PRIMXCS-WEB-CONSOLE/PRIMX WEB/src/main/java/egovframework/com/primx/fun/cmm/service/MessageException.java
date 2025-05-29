package egovframework.com.primx.fun.cmm.service;

public class MessageException extends RuntimeException {
    private static final long serialVersionUID = 1L;
    
    private final String errorCode;
    private final String messageKey;
    
    public MessageException(String message) {
        super(message);
        this.errorCode = "ERROR";
        this.messageKey = message;
    }
    
    public MessageException(String errorCode, String message) {
        super(message);
        this.errorCode = errorCode;
        this.messageKey = message;
    }
    
    public String getErrorCode() {
        return errorCode;
    }
    
    public String getMessageKey() {
        return messageKey;
    }
} 