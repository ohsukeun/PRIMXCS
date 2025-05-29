package egovframework.com.primx.fun.sys.service;

import java.io.Serializable;

import egovframework.com.primx.fun.cmm.util.DateUtil;
import lombok.Data;

/**
 * 설치제품 정보 DTO
 * @author 장기훈
 *
 */
@Data
public class PrimxSetupInfoDTO implements Serializable {

    private static final long serialVersionUID = 8020022440479659005L;

	/** 제품 종류 */
    private int productType;
    /** 제품 명 */
    private String productName;
    /** 제품 버전 */
    private String productVersion;
    /** 마지막 정책 반영 일자 */
    private String updateDate;
    /** 정책 명 */
    private String policyName;
    
	public void convertUTCtoUserTime() {
		this.updateDate = DateUtil.convertUTCtoUserTime(this.updateDate);
	}
}
