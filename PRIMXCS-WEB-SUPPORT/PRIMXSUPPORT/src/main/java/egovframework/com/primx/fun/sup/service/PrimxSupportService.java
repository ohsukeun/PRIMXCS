package egovframework.com.primx.fun.sup.service;

import java.util.Map;

public interface PrimxSupportService {
	
	public Map<String, Object> checkUserId(String id) throws Exception;

	public Map<String, Object> checkEmail(String email) throws Exception;
	
	public boolean checkCompanyCode(String companyCode) throws Exception;
	/* public Map<String, Object> checkComapany(String company) throws Exception; */
	
	// 회원가입
	public Map<String, Object> signUp(SignUpVO signUpVO) throws Exception;

	public Map<String, Object> loadPasswordSettingInfo();
	   
    
}
