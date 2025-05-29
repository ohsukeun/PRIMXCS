package egovframework.com.primx.fun.sup.service.impl;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.annotation.Resource;

import org.egovframe.rte.fdl.idgnr.EgovIdGnrService;
import org.springframework.stereotype.Service;

import egovframework.com.cmm.util.EgovUserDetailsHelper;
import egovframework.com.primx.fun.cmm.service.RoleVO;
import egovframework.com.primx.fun.cmm.service.SecurityVO;
import egovframework.com.primx.fun.cmm.service.impl.RoleDAO;
import egovframework.com.primx.fun.sup.service.PasswordSettingVO;
import egovframework.com.primx.fun.sup.service.PrimxSupportService;
import egovframework.com.primx.fun.sup.service.SignUpVO;
import egovframework.com.utl.sim.service.EgovFileScrty;

@Service("primxSupportService")
public class PrimxSupportServiceImpl implements PrimxSupportService {

    @Resource(name = "primxSupportDAO")
    private PrimxSupportDAO primxSupportDAO;
    
    @Resource(name = "roleDAO")
    private RoleDAO roleDAO;
    
	/** egovUsrCnfrmIdGnrService */
	@Resource(name="egovUsrCnfrmIdGnrService")
	private EgovIdGnrService idgenService;

    @Override
    public Map<String, Object> checkUserId(String userId) throws Exception {
        Map<String, Object> response = new HashMap<>();

        // 데이터베이스에서 userId 조회 (중복 체크)
        int count = primxSupportDAO.countUserId(userId);
        
        // count가 0이면 사용 가능(true), 1 이상이면 중복(false)
        boolean isAvailable = (count == 0);

        response.put("isAvailable", isAvailable);
        return response;
    }
    
    @Override
    public Map<String, Object> checkEmail(String email) throws Exception {
        Map<String, Object> response = new HashMap<>();

        // 데이터베이스에서 email 조회 (중복 체크)
        int count = primxSupportDAO.countEmail(email);
        
        // count가 0이면 사용 가능(true), 1 이상이면 중복(false)
        boolean isAvailable = (count == 0);

        response.put("isAvailable", isAvailable);
        return response;
    }
    
    @Override
    public boolean checkCompanyCode(String companyCode) throws Exception {
        // DAO에서 회사 코드 존재 여부 확인
        int count = primxSupportDAO.countCompanyCode(companyCode);
        return count > 0; // 1 이상이면 유효한 코드
    }

    @Override
    public Map<String, Object> signUp(SignUpVO signUpVO) throws Exception {

		// 사용자 정보 등록
		String uniqId = idgenService.getNextStringId();
		signUpVO.setUniqId(uniqId);
		
        String newpassword = signUpVO.getPassword();
		String enpassword = EgovFileScrty.encryptPassword(newpassword, signUpVO.getUserId());
		signUpVO.setPassword(enpassword);

		// 권한 코드 확인
		String userRole = "ROLE_PRIMX_USER";
		RoleVO roleVO = roleDAO.selectRoleById(userRole);
		signUpVO.setUserRole(userRole);
		
        // DAO에서 회원 가입 처리
        primxSupportDAO.insertUser(signUpVO);
        
		// 기본 권한 부여
		if (roleVO == null) {
			signUpVO.setUserRole("USER"); // 기본 권한 설정
		}

		// 사용자 보안 설정 등록
		SecurityVO securityData = new SecurityVO();
		securityData.setSecurityTargetId(signUpVO.getUniqId());
		securityData.setAuthorCode(signUpVO.getUserRole());
		securityData.setMemberTypeCode("USR03"); // 기본 회원 유형 코드
		System.out.println("securityData: "+securityData.toString());
		roleDAO.insertSecurity(securityData); // RoleDAO를 사용하여 보안 설정 등록
        
        // 회원 가입 성공 시 응답 메시지 반환
        Map<String, Object> response = new HashMap<>();
        response.put("message", "회원 가입이 완료되었습니다.");
        return response;
    }
    

	@Override
	public Map<String, Object> loadPasswordSettingInfo() {
		Map<String, Object> rtnMap = new HashMap<>();
		try {
			List<String> authList = EgovUserDetailsHelper.getAuthorities();
        	String userAuth = authList.get(0);
			String userRole = "USER";
			// ADMIN이라는 글자가 들어가는 변경
			if(userAuth.contains("ADMIN")) {
				userRole = "ADMIN";
			}
			PasswordSettingVO passwordSettingVO = primxSupportDAO.selectPasswordSettingInfo(userRole);
			rtnMap.put("status", "success");
			rtnMap.put("message", "success");
			rtnMap.put("data", passwordSettingVO);
		} catch (Exception e) {
			rtnMap.put("status", "error");
			rtnMap.put("message", "error");
		}
		return rtnMap;
	}
}

