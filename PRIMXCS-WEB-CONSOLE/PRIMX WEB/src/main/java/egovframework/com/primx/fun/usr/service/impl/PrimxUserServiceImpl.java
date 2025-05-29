package egovframework.com.primx.fun.usr.service.impl;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.annotation.Resource;

import org.egovframe.rte.fdl.idgnr.EgovIdGnrService;
import org.springframework.stereotype.Service;

import egovframework.com.cmm.LoginVO;
import egovframework.com.cmm.util.EgovUserDetailsHelper;
import egovframework.com.primx.fun.usr.service.PrimxUserService;
import egovframework.com.uat.uia.service.impl.LoginDAO;
import egovframework.com.utl.sim.service.EgovFileScrty;
import egovframework.com.primx.fun.cmm.service.MessageException;
import egovframework.com.primx.fun.cmm.service.RoleVO;
import egovframework.com.primx.fun.cmm.service.SecurityVO;
import egovframework.com.primx.fun.cmm.service.SmartContractVO;
import egovframework.com.primx.fun.cmm.service.impl.RoleDAO;
import egovframework.com.primx.fun.dpt.service.DeptSearchVO;
import egovframework.com.primx.fun.sys.service.PasswordSettingVO;
import egovframework.com.primx.fun.sys.service.impl.SystemDAO;
import egovframework.com.primx.fun.usr.service.UserListDTO;
import egovframework.com.primx.fun.usr.service.UserRegistrationDTO;
import egovframework.com.primx.fun.usr.service.IpInfoDTO;


@Service("primxUserService")
public class PrimxUserServiceImpl implements PrimxUserService {

	@Resource(name = "userDAO")
	private UserDAO userDAO;

    @Resource(name="loginDAO")
    private LoginDAO loginDAO;

	@Resource(name = "roleDAO")
	private RoleDAO roleDAO;

	@Resource(name = "systemDAO")
	private SystemDAO systemDAO;
	
	/** egovUsrCnfrmIdGnrService */
	@Resource(name="egovUsrCnfrmIdGnrService")
	private EgovIdGnrService idgenService;

	@Override
	public boolean resetPassword(String emplyrId, PasswordSettingVO passwordSettingVO) throws MessageException {

		try {
			String newpassword = emplyrId;
		
			// 변경할 비밀번호
			String enNewPassword = EgovFileScrty.encryptPassword(newpassword, emplyrId);
			System.out.println("enNewPassword: ["+enNewPassword+"]");
	
			int passExpireDays = passwordSettingVO.getPassExpireDays();
	
			userDAO.updatePassword(emplyrId, enNewPassword, passExpireDays, "N");
	
		} catch (Exception e) {
			throw new MessageException(e.getMessage());
		}
    	
		return true;
	}

	@Override	
	public boolean updatePassword(String emplyrId, String currentPassword, String newPassword, PasswordSettingVO passwordSettingVO) throws MessageException {
		
		try {
			if(emplyrId == null || emplyrId.equals("")) {
				LoginVO loginVO = (LoginVO) EgovUserDetailsHelper.getAuthenticatedUser();
				emplyrId = loginVO.getId();
			}
			
			// 입력받은 비밀번호
			String enCurrentPassword = EgovFileScrty.encryptPassword(currentPassword, emplyrId);
			System.out.println("enCurrentPassword: ["+enCurrentPassword+"]");
			// DB에 저장된 비밀번호와 일치하는지 검사
			String enOriginalPassword = userDAO.searchPassword(emplyrId, enCurrentPassword);
			System.out.println("enOriginalPassword: ["+enOriginalPassword+"]");
			if(enOriginalPassword == null) {
				throw new MessageException("기존 패스워드가 일치하지 않습니다.");
			}
			
			// 변경할 비밀번호
			String enNewPassword = EgovFileScrty.encryptPassword(newPassword, emplyrId);
			System.out.println("enNewPassword: ["+enNewPassword+"]");
			// 현재 패스워드와 일치하는지 검증
			if(enNewPassword.equals(enCurrentPassword)) {
				throw new MessageException("기존 패스워드와 동일합니다.");
			}

			int passExpireDays = passwordSettingVO.getPassExpireDays();

			userDAO.updatePassword(emplyrId, enNewPassword, passExpireDays, "Y");

		} catch (Exception e) {
			throw new MessageException(e.getMessage());
		}

		return true;

	}

	@Override
	public boolean isUserAvailable(String emplyrId) throws Exception {
		int count = userDAO.countByUserId(emplyrId);
		count += userDAO.countByRequestUserId(emplyrId);
		return count == 0;
	}

	@Override
	public boolean checkMail(String mail) throws Exception {
		int count = userDAO.countByMail(mail);
		count += userDAO.countByRequestMail(mail);
		return count == 0;
	}

	@Override
	public void registerUser(UserRegistrationDTO userRegistrationDTO) throws Exception {
		// 사용자 정보 등록
		String uniqId = idgenService.getNextStringId();
		userRegistrationDTO.setUniqId(uniqId);

		String newpassword = userRegistrationDTO.getEmplyrId();
		String enpassword = EgovFileScrty.encryptPassword(newpassword, userRegistrationDTO.getEmplyrId());
		userRegistrationDTO.setPassword(enpassword);
		userDAO.insertUser(userRegistrationDTO);
		
		// 권한 코드 확인
		String userRole = userRegistrationDTO.getUserRole();
		RoleVO roleVO = roleDAO.selectRoleById(userRole);

		// 기본 권한 부여
		if (roleVO == null) {
			userRegistrationDTO.setUserRole("USER"); // 기본 권한 설정
		}

		// 사용자 보안 설정 등록
		SecurityVO securityData = new SecurityVO();
		securityData.setSecurityTargetId(userRegistrationDTO.getUniqId());
		securityData.setAuthorCode(userRegistrationDTO.getUserRole());
		securityData.setMemberTypeCode("USR03"); // 기본 회원 유형 코드
		System.out.println("securityData: "+securityData.toString());
		roleDAO.insertSecurity(securityData); // RoleDAO를 사용하여 보안 설정 등록

	}

	@Override
	public boolean deleteUser(String emplyrId) throws Exception {
		// INSERTED_BY_HR_YN이 'N'인 경우에만 USE_YN을 'N'으로 변경
		int updatedRows = userDAO.updateUserStatus(emplyrId);
		return updatedRows > 0;
	}

	@Override
	public UserRegistrationDTO getUserDetails(String emplyrId) throws Exception {
		UserRegistrationDTO userRegistrationDTO = userDAO.selectUserDetails(emplyrId);
		userRegistrationDTO.convertUTCtoUserTime();
		List<IpInfoDTO> ipList = userDAO.selectUserIpList(emplyrId); // IP 정보 가져오기
		for(IpInfoDTO ip : ipList) {
			ip.convertUTCtoUserTime();
		}
		userRegistrationDTO.setIpList(ipList); // IP 리스트 설정
		return userRegistrationDTO;
	}

	@Override
	public void deleteUserIp(String emplyrId) throws Exception {
		userDAO.deleteUserIp(emplyrId); // DAO를 통해 IP 정보 삭제
	}

	@Override
	public void updateUser(UserRegistrationDTO userRegistrationDTO) throws Exception {
		// 사용자 정보 업데이트 로직
		userDAO.updateUser(userRegistrationDTO); // 사용자 정보를 업데이트하는 DAO 메서드 호출
		if(userRegistrationDTO.getUserRole() != null) {
			String esntlId = userDAO.selectEsntlId(userRegistrationDTO.getEmplyrId());
			// 롤 업데이트
			roleDAO.updateUserAuthorCode(esntlId, userRegistrationDTO.getUserRole());
		}
	}

	@Override
	public void insertUserIp(UserRegistrationDTO userRegistrationDTO) throws Exception {
		// IP 정보 등록
		if (userRegistrationDTO.getIpList() != null) {
			for (IpInfoDTO ip : userRegistrationDTO.getIpList()) {
				// 중복 체크
				int count = userDAO.countUserIp(userRegistrationDTO.getEmplyrId(), ip.getAllowedIp());
				if (count > 0) {
					throw new Exception("중복된 IP 주소가 존재합니다: " + ip);
				}
				userDAO.insertUserIp(userRegistrationDTO.getEmplyrId(), ip.getAllowedIp(), userRegistrationDTO.getUsePeriod(), userRegistrationDTO.getExpiredDate());
			}
		}
	}

	@Override
	public Map<String, Object> selectUsersByDeptIds(List<String> deptIds, DeptSearchVO searchVO) throws Exception {
		Map<String, Object> response = new HashMap<>();
		List<UserListDTO> result = userDAO.selectUsersByDeptIds(deptIds, searchVO.getUserType(), searchVO.getSyncType(), searchVO.getSearchKeyword(), searchVO.getStart(), searchVO.getLength());
		for(UserListDTO dto : result) {
			dto.convertUTCtoUserTime();
		}
		int totalCnt = userDAO.selectUsersByDeptIdsCnt(deptIds, searchVO.getUserType(), searchVO.getSyncType(), searchVO.getSearchKeyword());
		response.put("draw", searchVO.getDraw());
		response.put("recordsTotal", totalCnt);
		response.put("recordsFiltered", totalCnt);
		response.put("data", result);
		response.put("status", "success");
		return response;
	}
	
	@Override
	public Map<String, Object> selectSmartContractUserList(List<String> deptIds, DeptSearchVO searchVO) throws Exception {
		Map<String, Object> response = new HashMap<>();
		List<SmartContractVO> result = userDAO.selectSmartContractUserList(deptIds, searchVO.getUserType(), searchVO.getSyncType(), searchVO.getSearchKeyword(), searchVO.getStart(), searchVO.getLength());
		int cnt = userDAO.selectSmartContractUserListCnt(deptIds, searchVO.getUserType(), searchVO.getSyncType(), searchVO.getSearchKeyword());
		response.put("draw", searchVO.getDraw());
		response.put("recordsTotal", cnt);
		response.put("recordsFiltered", cnt);
		response.put("data", result);
		response.put("status", "success");
		return response;
	}

	@Override
	public Map<String, Object> getMySystemInfo() throws Exception {

		LoginVO loginVO = (LoginVO) EgovUserDetailsHelper.getAuthenticatedUser();

		Map<String, Object> response = new HashMap<>();
		response.put("status", "success");
		// response.put("data", userDAO.selectMySystemInfo(loginVO.getId()));
		return response;
	}

	@Override
	public void updateUserExpiredDate(String emplyrId, String expiredDate) throws Exception {
		userDAO.updateUserExpiredDate(emplyrId, expiredDate);
	}

	// emplyrId를 입력받아 useYn 여부를 체크 boolean 값으로 반환 ( true: 사용가능, false: 사용불가능)
	@Override
	public boolean checkUserUseYn(String emplyrId) throws Exception {
		int count = userDAO.checkUserUseYn(emplyrId);
		boolean result = false;
		if(count > 0) {
			result = true;
		}
		return result;
	}

	// emplyrId를 입력받아 비밀번호 만료일자를 체크 boolean 값으로 반환 ( true: 만료, false: 만료안됨)
	@Override
	public int checkUserPasswordExpiredDate(String emplyrId) throws Exception {
		return userDAO.checkUserPasswordExpiredDate(emplyrId);
	}

	// emplyrId를 입력받아 now() 부터 EXPIRED_DATE 까지 남은 일수를 반환
	@Override
	public int checkUserExpiredDate(String emplyrId) throws Exception {
		return userDAO.checkUserExpiredDate(emplyrId);
	}
}
