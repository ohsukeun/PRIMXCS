package egovframework.com.primx.fun.pol.service;

import egovframework.com.primx.fun.cmm.util.DateUtil;
import lombok.Builder;
import lombok.Data;
import lombok.AllArgsConstructor;
import lombok.NoArgsConstructor;

@Data
@Builder
@AllArgsConstructor
@NoArgsConstructor
public class PolicyDetailInfoVO {
    private int polSeq;
    private String polCode;
    private int polId;
    private String polData;
    private String createdDate;
    private String updatedDate;

    public void convertUTCtoUserTime() {
        this.createdDate = DateUtil.convertUTCtoUserTime(this.createdDate);
        this.updatedDate = DateUtil.convertUTCtoUserTime(this.updatedDate);
    }

    public void convertUserTimeToUTC() {
        this.createdDate = DateUtil.convertUserTimeToUTC(this.createdDate);
        this.updatedDate = DateUtil.convertUserTimeToUTC(this.updatedDate);
    }
}