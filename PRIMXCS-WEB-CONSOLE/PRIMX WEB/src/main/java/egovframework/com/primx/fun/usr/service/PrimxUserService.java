package egovframework.com.primx.fun.usr.service;

import java.util.List;
import java.util.Map;

import egovframework.com.primx.fun.cmm.service.MessageException;
import egovframework.com.primx.fun.dpt.service.DeptSearchVO;
import egovframework.com.primx.fun.sys.service.PasswordSettingVO;

public interface PrimxUserService {

	public boolean resetPassword(String emplyrId, PasswordSettingVO passwordSettingVO) throws MessageException;
	public boolean isUserAvailable(String emplyrId) throws Exception;
	public boolean checkMail(String mail) throws Exception;
	public void registerUser(UserRegistrationDTO userRegistrationDTO) throws Exception;
	public boolean deleteUser(String emplyrId) throws Exception;
	public UserRegistrationDTO getUserDetails(String emplyrId) throws Exception;
	public void deleteUserIp(String emplyrId) throws Exception;
	public void updateUser(UserRegistrationDTO userRegistrationDTO) throws Exception;
	public void insertUserIp(UserRegistrationDTO userRegistrationDTO) throws Exception;
	public boolean updatePassword(String emplyrId, String currentPassword, String newPassword, PasswordSettingVO passwordSettingVO) throws MessageException;
	public Map<String, Object> selectUsersByDeptIds(List<String> deptIds, DeptSearchVO searchVO) throws Exception;
	public Map<String, Object> getMySystemInfo() throws Exception;
	public void updateUserExpiredDate(String emplyrId, String expiredDate) throws Exception;
	public boolean checkUserUseYn(String emplyrId) throws Exception;
	public int checkUserPasswordExpiredDate(String emplyrId) throws Exception;
	public int checkUserExpiredDate(String emplyrId) throws Exception;
	public Map<String, Object> selectSmartContractUserList(List<String> deptIds, DeptSearchVO searchVO) throws Exception;
}
