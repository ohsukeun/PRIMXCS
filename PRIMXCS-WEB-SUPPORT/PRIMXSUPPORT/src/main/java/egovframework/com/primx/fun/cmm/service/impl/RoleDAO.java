package egovframework.com.primx.fun.cmm.service.impl;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.stereotype.Repository;

import egovframework.com.cmm.service.impl.EgovComAbstractDAO;
import egovframework.com.primx.fun.cmm.service.RoleVO;
import egovframework.com.primx.fun.cmm.service.SecurityVO;

@Repository("roleDAO")
public class RoleDAO extends EgovComAbstractDAO {

    // 권한 정보 단일 조회
    public RoleVO selectRoleById(String authorCode) {
        return selectOne("roleDAO.selectRoleById", authorCode);
    }

    // 권한 정보 전체 리스트 조회
    public List<RoleVO> selectAllRoleList(String authorCode) {
        Map<String, Object> param = new HashMap<>();
        param.put("authorCode", authorCode);
        return selectList("roleDAO.selectAllRoleList", param);
    }

    // 권한 정보 리스트 조회

    public List<RoleVO> selectRoleList(List<String> searchAuth) {
        Map<String, Object> param = new HashMap<>();
        param.put("searchAuth", searchAuth);
        return selectList("roleDAO.selectRoleList", param);
    }

    // 권한 정보 단일 조회
    public RoleVO selectOneRole(String authorCode) {
        Map<String, Object> param = new HashMap<>();
        param.put("authorCode", authorCode);   
        return selectOne("roleDAO.selectOneRole", param);
    }


    // 권한 정보 등록
    public void insertRole(RoleVO roleData) {
        insert("roleDAO.insertRole", roleData);
    }

    // 권한 정보 수정
    public void updateRole(RoleVO roleData) {
        update("roleDAO.updateRole", roleData);
    }

    // 권한 정보 삭제
    public void deleteRole(String authorCode) {
        delete("roleDAO.deleteRole", authorCode);
    }

    // 사용자 보안 설정 단일 조회
    public SecurityVO selectSecurityById(String securityTargetId) {
        return selectOne("roleDAO.selectSecurityById", securityTargetId);
    }

    // 사용자 보안 설정 리스트 조회
    public List<SecurityVO> selectSecurityList() {
        return selectList("roleDAO.selectSecurityList");
    }

    // 사용자 보안 설정 등록
    public void insertSecurity(SecurityVO securityData) {
        insert("roleDAO.insertSecurity", securityData);
    }

    // 사용자 보안 설정 수정
    public void updateSecurity(SecurityVO securityData) {
        update("roleDAO.updateSecurity", securityData);
    }

    // 사용자 보안 설정 삭제
    public void deleteSecurity(String securityTargetId) {
        delete("roleDAO.deleteSecurity", securityTargetId);
    }

    // 사용자 롤 업데이트
    public void updateUserAuthorCode(String esntlId, String userRole) {
        Map<String, Object> paramMap = new HashMap<>();
        paramMap.put("esntlId", esntlId);
        paramMap.put("userRole", userRole); 
        update("roleDAO.updateUserAuthorCode", paramMap);
    }

} 