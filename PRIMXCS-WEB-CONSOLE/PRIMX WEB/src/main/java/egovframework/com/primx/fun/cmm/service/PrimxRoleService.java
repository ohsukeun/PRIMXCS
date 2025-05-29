package egovframework.com.primx.fun.cmm.service;

import java.util.List;
import java.util.Map;

public interface PrimxRoleService {
    // 권한 정보 단일 조회
    RoleVO selectRoleById(String authorCode) throws Exception;

    // 권한 정보 리스트 조회
    Map<String, Object> selectRoleList(int searchType) throws Exception;

    // 권한 정보 등록
    void insertRole(RoleVO roleData) throws Exception;

    // 권한 정보 수정
    void updateRole(RoleVO roleData) throws Exception;

    // 권한 정보 삭제
    void deleteRole(String authorCode) throws Exception;

    // 사용자 보안 설정 단일 조회
    SecurityVO selectSecurityById(String securityTargetId) throws Exception;

    // 사용자 보안 설정 리스트 조회
    List<SecurityVO> selectSecurityList() throws Exception;

    // 사용자 보안 설정 등록
    void insertSecurity(SecurityVO securityData) throws Exception;

    // 사용자 보안 설정 수정
    void updateSecurity(SecurityVO securityData) throws Exception;

    // 사용자 보안 설정 삭제
    void deleteSecurity(String securityTargetId) throws Exception;

    // 사용자 권한 리턴
    public String getUserAuth() throws Exception;
}
