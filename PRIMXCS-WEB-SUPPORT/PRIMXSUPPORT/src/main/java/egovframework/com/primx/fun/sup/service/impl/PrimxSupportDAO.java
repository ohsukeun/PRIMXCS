package egovframework.com.primx.fun.sup.service.impl;

import java.util.HashMap;
import java.util.Map;

import org.springframework.stereotype.Repository;

import egovframework.com.cmm.service.impl.EgovComAbstractDAO;
import egovframework.com.primx.fun.sup.service.PasswordSettingVO;
import egovframework.com.primx.fun.sup.service.SignUpVO;


@Repository("primxSupportDAO")
public class PrimxSupportDAO extends EgovComAbstractDAO {

    // userId 중복 여부 확인
    public int countUserId(String userId) {
        return (Integer) selectOne("primxSupportDAO.countUserId", userId);
    }
    // email 중복 여부 확인
    public int countEmail(String email) {
        return (Integer) selectOne("primxSupportDAO.countEmail", email);
    }
    // companyCode 존재 여부 확인
    public int countCompanyCode(String companyCode) {
        return (Integer) selectOne("primxSupportDAO.countCompanyCode", companyCode);
    }
    // 회원 가입
    public void insertUser(SignUpVO signUpVO) {
        insert("primxSupportDAO.insertUser", signUpVO);
    }

	public PasswordSettingVO selectPasswordSettingInfo(String userRole) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("userRole", userRole);
		return selectOne("primxSupportDAO.selectPasswordSettingInfo", paramMap);
	}
}
