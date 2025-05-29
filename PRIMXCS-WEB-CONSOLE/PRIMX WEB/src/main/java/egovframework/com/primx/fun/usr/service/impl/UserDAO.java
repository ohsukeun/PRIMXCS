package egovframework.com.primx.fun.usr.service.impl;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.stereotype.Repository;

import egovframework.com.cmm.service.impl.EgovComAbstractDAO;
import egovframework.com.primx.fun.usr.service.UserRegistrationDTO;
import egovframework.com.primx.fun.usr.service.UserListDTO;
import egovframework.com.primx.fun.cmm.service.SmartContractVO;
import egovframework.com.primx.fun.usr.service.IpInfoDTO;

@Repository("userDAO")
public class UserDAO extends EgovComAbstractDAO {

	public int countByUserId(String userId) {
		return selectOne("userDAO.countByUserId", userId);
	}

	public int countByRequestUserId(String userId) {
		return selectOne("userDAO.countByRequestUserId", userId);
	}

	public int countByMail(String mail) {
		return selectOne("userDAO.countByMail", mail);
	}

	public int countByRequestMail(String mail) {
		return selectOne("userDAO.countByRequestMail", mail);
	}

	public void insertUser(UserRegistrationDTO userRegistrationDTO) {
		insert("userDAO.insertUser", userRegistrationDTO);
	}

	public void insertUserIp(String userId, String ip, String usePeriod, String expiredDate) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("userId", userId);
		paramMap.put("ip", ip);
		paramMap.put("usePeriod", usePeriod);
		paramMap.put("expiredDate", expiredDate);
		insert("userDAO.insertUserIp", paramMap);
	}

	public int countUserIp(String userId, String ip) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("userId", userId);
		paramMap.put("ip", ip);
		return selectOne("userDAO.countUserIp", paramMap);
	}

	public int updateUserStatus(String emplyrId) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("emplyrId", emplyrId);
		return update("userDAO.updateUserStatus", paramMap);
	}

	public List<IpInfoDTO> selectUserIpList(String emplyrId) {
		return selectList("userDAO.selectUserIpList", emplyrId);
	}

	public UserRegistrationDTO selectUserDetails(String emplyrId) throws Exception {
		return selectOne("userDAO.selectUserDetails", emplyrId);
	}

	public void deleteUserIp(String userId) {
		delete("userDAO.deleteUserIp", userId);
	}

	public void updateUser(UserRegistrationDTO userRegistrationDTO) {
		update("userDAO.updateUser", userRegistrationDTO);
	}

	public void updatePassword(String emplyrId, String enNewPassword, int passExpireDays, String pwdChanged) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("emplyrId", emplyrId);
		paramMap.put("enNewPassword", enNewPassword);
		paramMap.put("passExpireDays", passExpireDays);
		paramMap.put("pwdChanged", pwdChanged);
		update("userDAO.updatePassword", paramMap);
	}

	public String searchPassword(String emplyrId, String enCurrentPassword) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("emplyrId", emplyrId);
		paramMap.put("enCurrentPassword", enCurrentPassword);
		return selectOne("userDAO.searchPassword", paramMap);
	}	

	public List<UserListDTO> selectUsersByDeptIds(List<String> deptIds, String userType, String syncType, String searchKeyword, int start, int length) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("deptIds", deptIds);
		paramMap.put("userType", userType);
		paramMap.put("syncType", syncType);
		paramMap.put("searchKeyword", searchKeyword);
		paramMap.put("start", start);
		paramMap.put("length", length);
		return selectList("userDAO.selectUsersByDeptIds", paramMap);

	}

	public int selectUsersByDeptIdsCnt(List<String> deptIds, String userType, String syncType, String searchKeyword) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("deptIds", deptIds);
		paramMap.put("userType", userType);
		paramMap.put("syncType", syncType);
		paramMap.put("searchKeyword", searchKeyword);
		return (Integer) selectOne("userDAO.selectUsersByDeptIdsCnt", paramMap);
	}

	public List<SmartContractVO> selectSmartContractUserList(List<String> deptIds, String userType, String syncType, String searchKeyword, int start, int length) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("deptIds", deptIds);
		paramMap.put("userType", userType);
		paramMap.put("syncType", syncType);
		paramMap.put("searchKeyword", searchKeyword);
		paramMap.put("start", start);
		paramMap.put("length", length);
		return selectList("userDAO.selectSmartContractUserList", paramMap);
	}

	public int selectSmartContractUserListCnt(List<String> deptIds, String userType, String syncType, String searchKeyword) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("deptIds", deptIds);
		paramMap.put("userType", userType);
		paramMap.put("syncType", syncType);
		paramMap.put("searchKeyword", searchKeyword);
		return (Integer) selectOne("userDAO.selectSmartContractUserListCnt", paramMap);
	}

	public String selectEsntlId(String emplyrId) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("emplyrId", emplyrId);
		return selectOne("userDAO.selectEsntlId", paramMap);
	}

	public void updateUserExpiredDate(String emplyrId, String expiredDate) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("emplyrId", emplyrId);
		paramMap.put("expiredDate", expiredDate);
		update("userDAO.updateUserExpiredDate", paramMap);
	}

	public int selectUserSeqById(String emplyrId) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("emplyrId", emplyrId);
		return selectOne("userDAO.selectUserSeqById", paramMap);
	}

	public int checkUserUseYn(String emplyrId) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("emplyrId", emplyrId);
		return selectOne("userDAO.checkUserUseYn", paramMap);
	}

	public int checkUserPasswordExpiredDate(String emplyrId) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("emplyrId", emplyrId);
		return selectOne("userDAO.checkUserPasswordExpiredDate", paramMap);
	}

	public int checkUserExpiredDate(String emplyrId) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("emplyrId", emplyrId);
		return selectOne("userDAO.checkUserExpiredDate", paramMap);
	}
}
