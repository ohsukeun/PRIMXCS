package egovframework.com.primx.fun.cmm.service.impl;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.annotation.Resource;

import org.springframework.stereotype.Service;

import egovframework.com.cmm.util.EgovUserDetailsHelper;
import egovframework.com.primx.fun.cmm.service.PrimxRoleService;
import egovframework.com.primx.fun.cmm.service.RoleVO;
import egovframework.com.primx.fun.cmm.service.SecurityVO;

@Service("primxRoleService")
public class PrimxRoleServiceImpl implements PrimxRoleService {

    @Resource(name = "roleDAO")
    private RoleDAO roleDAO;

    @Override
    public RoleVO selectRoleById(String authorCode) throws Exception {
        return roleDAO.selectRoleById(authorCode);
    }

    @Override
    public Map<String, Object> selectRoleList(int searchType) throws Exception {

        // 사용자의 권한을 가져온다
        List<String> authList = EgovUserDetailsHelper.getAuthorities();
        String userAuth = authList.get(0);
        List<String> searchAuth = new ArrayList<>();
        // 지정되어있음
        if(userAuth.equals("ROLE_PRIMX_SUPER_ADMIN")){
            searchAuth.add("ROLE_PRIMX_SUPER_ADMIN");
            searchAuth.add("ROLE_PRIMX_ADMIN");
            searchAuth.add("ROLE_PRIMX_USER");
        } else if(userAuth.equals("ROLE_PRIMX_ADMIN")) {
            searchAuth.add("ROLE_PRIMX_ADMIN");
            searchAuth.add("ROLE_PRIMX_USER");
        } else {
            searchAuth.add("ROLE_PRIMX_USER");
        }

        Map<String, Object> response = new HashMap<>();
        try {
            List<RoleVO> roleList = new ArrayList<>();
            if(searchType == 2){
                roleList = roleDAO.selectAllRoleList("PRIMX");
            } else if(searchType == 1){
                roleList = roleDAO.selectRoleList(searchAuth);
            } else {
                roleList.add(roleDAO.selectOneRole(userAuth));
            }

            response.put("status", "success");
            response.put("data", roleList);

        } catch (Exception e) {
            response.put("status", "error");
            response.put("message", e.getMessage());
        }
        return response;
    }

    @Override
    public void insertRole(RoleVO roleData) throws Exception {
        roleDAO.insertRole(roleData);
    }

    @Override
    public void updateRole(RoleVO roleData) throws Exception {
        roleDAO.updateRole(roleData);
    }

    @Override
    public void deleteRole(String authorCode) throws Exception {
        roleDAO.deleteRole(authorCode);
    }

    @Override
    public SecurityVO selectSecurityById(String securityTargetId) throws Exception {
        return roleDAO.selectSecurityById(securityTargetId);
    }

    @Override
    public List<SecurityVO> selectSecurityList() throws Exception {
        return roleDAO.selectSecurityList();
    }

    @Override
    public void insertSecurity(SecurityVO securityData) throws Exception {
        roleDAO.insertSecurity(securityData);
    }

    @Override
    public void updateSecurity(SecurityVO securityData) throws Exception {
        roleDAO.updateSecurity(securityData);
    }

    @Override
    public void deleteSecurity(String securityTargetId) throws Exception {
        roleDAO.deleteSecurity(securityTargetId);
    }

    @Override
    public String getUserAuth() throws Exception {
        return EgovUserDetailsHelper.getAuthorities().get(0);
    }
} 