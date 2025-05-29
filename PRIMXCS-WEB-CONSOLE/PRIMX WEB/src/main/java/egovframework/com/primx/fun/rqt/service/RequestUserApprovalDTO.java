package egovframework.com.primx.fun.rqt.service;

import java.util.List;

import egovframework.com.primx.fun.cmm.util.DateUtil;
import lombok.Data;

@Data
public class RequestUserApprovalDTO {
    private String requestCode;
    private String requestApprovalCode; // 신청 형식 (M: 이동파일 생성, C: 계정 연장, A: 사용자 등록)
    private List<RequestApproversVO> approvalLines; // 결재선 정보
    private RegisterUserInfoDTO regiestUserInfo; // 신청자 정보
    
    // 계정 연장 신청 관련
    private String usePeriod;
    
    // 사용자 등록 신청 관련
    private String userId;
    private String userName;
    private String email;
    private String companyId;
    private String companyName;
    private String deptId;
    private String deptName;
    private String userRole;
    private String userPassword;
    private String expiredDate;

    public void convertUTCtoUserTime() {
        this.expiredDate = DateUtil.convertUTCtoUserTime(this.expiredDate);
    }

    public void convertUserTimeToUTC() {
        this.expiredDate = DateUtil.convertUserTimeToUTC(this.expiredDate);
    }
}
