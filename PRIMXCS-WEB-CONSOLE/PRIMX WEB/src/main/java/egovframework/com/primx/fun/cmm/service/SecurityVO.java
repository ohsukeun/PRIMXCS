package egovframework.com.primx.fun.cmm.service;

import lombok.Data;

@Data
public class SecurityVO {
    private String securityTargetId; // 보안설정대상ID
    private String memberTypeCode; // 회원유형코드
    private String authorCode; // 권한코드

} 