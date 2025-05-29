package egovframework.com.primx.fun.usr.service;

import egovframework.com.primx.fun.cmm.util.DateUtil;
import lombok.Data;

@Data
public class IpInfoDTO {
    private String allowedIp;
    private String accountExpiryDate;

    public void convertUTCtoUserTime() {
        this.accountExpiryDate = DateUtil.convertUTCtoUserTime(this.accountExpiryDate);
    }
} 