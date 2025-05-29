package egovframework.com.primx.fun.rqt.service;

import java.io.Serializable;
import java.util.Date;

import lombok.Data;

@Data
public class RequestDTO implements Serializable {
    private static final long serialVersionUID = 1L;

    private int requestId;
    private String requestCode;
    private String requestApprovalCode;
    private String statusCd;
    private String resultCd;
    private String title;
    private String reason;
    private String description;
    private Integer fileGroupId;
    private String CUserId;
    private String createdIp;
    private String modifiedBy;
    private Date modifiedDate;
    private String modifiedIp;
    
}
