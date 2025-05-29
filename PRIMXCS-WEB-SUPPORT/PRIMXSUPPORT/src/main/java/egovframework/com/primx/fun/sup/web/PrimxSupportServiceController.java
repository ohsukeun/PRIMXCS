package egovframework.com.primx.fun.sup.web;

import java.util.HashMap;
import java.util.Map;

import javax.annotation.Resource;

import org.springframework.stereotype.Controller;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.RequestBody;


import egovframework.com.cmm.web.EgovComAbstractController;
import egovframework.com.primx.fun.sup.service.PrimxSupportService;
import egovframework.com.primx.fun.sup.service.SignUpVO;



@Controller
@RequestMapping(value = "/primx/fun/sup")
public class PrimxSupportServiceController extends EgovComAbstractController {
    
    @Resource(name = "primxSupportService")
    private PrimxSupportService primxSupportService;
    
	/**
	 * 회원가입 페이지를 출력한다.
	 */
	@RequestMapping(value = "/action"
			+ "SignUp.do")
	public String actionSignUp(ModelMap model) throws Exception {
	    return "egovframework/com/primx/fun/sup/signUp"; // 회원가입 JSP 경로
	}
    
    @ResponseBody
    @RequestMapping(value = "/checkUserId.do", method = RequestMethod.GET)
    public Map<String, Object> checkUserId(@RequestParam("userId") String userId) {
        System.out.println("### userId 파라미터 값: " + userId);
        Map<String, Object> response = new HashMap<>();

        try {
            // 서비스 호출 후 결과를 Map으로 받음
            Map<String, Object> result = primxSupportService.checkUserId(userId);
            System.out.println("### 서비스 호출 결과: " + result);

            // Map에서 "isAvailable" 키의 값을 boolean 타입으로 변환
            boolean isAvailable = Boolean.TRUE.equals(result.get("isAvailable"));
            System.out.println("### isAvailable 값: " + isAvailable);

            response.put("isAvailable", isAvailable);
            response.put("message", isAvailable ? "사용 가능한 아이디입니다." : "이미 사용 중인 아이디입니다.");

        } catch (Exception e) {
            System.out.println("### 에러 발생: " + e.getMessage());
            e.printStackTrace();
            response.put("isAvailable", false);
            response.put("message", "아이디 중복 확인 중 오류가 발생했습니다.");
        }

        System.out.println("### 최종 응답: " + response);
        return response;
    }
    
    @ResponseBody
    @RequestMapping(value = "/checkEmail.do", method = RequestMethod.GET)
    public Map<String, Object> checkEmail(@RequestParam("email") String email) {
        System.out.println("### email 파라미터 값: " + email);
        Map<String, Object> response = new HashMap<>();

        try {
            // 서비스 호출 후 결과를 Map으로 받음
            Map<String, Object> result = primxSupportService.checkEmail(email);
            System.out.println("### 서비스 호출 결과: " + result);

            // Map에서 "isAvailable" 키의 값을 boolean 타입으로 변환
            boolean isAvailable = Boolean.TRUE.equals(result.get("isAvailable"));
            System.out.println("### isAvailable 값: " + isAvailable);

            response.put("isAvailable", isAvailable);
            response.put("message", isAvailable ? "사용 가능한 이메일입니다." : "이미 사용 중인 이메일입니다.");

        } catch (Exception e) {
            System.out.println("### 에러 발생: " + e.getMessage());
            e.printStackTrace();
            response.put("isAvailable", false);
            response.put("message", "이메일 중복 확인 중 오류가 발생했습니다.");
        }

        System.out.println("### 최종 응답: " + response);
        return response;
    }
    /**
     * 회사 코드 확인
     */
    @ResponseBody
    @RequestMapping(value = "/checkCompany.do", method = RequestMethod.GET)
    public Map<String, Object> checkCompany(@RequestParam("companyCode") String companyCode) {
        System.out.println("### companyCode 파라미터 값: " + companyCode);
        
        Map<String, Object> response = new HashMap<>();

        try {
            // 서비스 호출 후 결과를 boolean으로 받음
            boolean isValid = primxSupportService.checkCompanyCode(companyCode);
            System.out.println("### isValid 값: " + isValid);

            response.put("isValid", isValid);
            response.put("message", isValid ? "회사 코드가 확인되었습니다." : "유효하지 않은 회사 코드입니다.");

        } catch (Exception e) {
            System.out.println("### 에러 발생: " + e.getMessage());
            e.printStackTrace();
            response.put("isValid", false);
            response.put("message", "회사 코드 확인 중 오류가 발생했습니다.");
        }

        System.out.println("### 최종 응답: " + response);
        return response;
    }

    @ResponseBody
    @RequestMapping(value = "/signUp.do", method = RequestMethod.POST)
    public Map<String, Object> signUp(@RequestBody SignUpVO signUpVO) {
        System.out.println("### 회원가입 요청 데이터: " + signUpVO);

        Map<String, Object> response = new HashMap<>();

        try {
            // 서비스 호출 후 결과를 Map으로 받음
            Map<String, Object> result = primxSupportService.signUp(signUpVO);
            System.out.println("### 서비스 호출 결과: " + result);

            response.put("success", true);
            response.put("message", "회원가입이 완료되었습니다.");
        } catch (Exception e) {
            System.out.println("### 에러 발생: " + e.getMessage());
            e.printStackTrace();
            response.put("success", false);
            response.put("message", "회원가입 중 오류가 발생했습니다.");
        }

        System.out.println("### 최종 응답: " + response);
        return response;
    }


	@ResponseBody
	@RequestMapping(value = "/loadPasswordSettingInfo.do", method = RequestMethod.GET)
	public Map<String, Object> loadPasswordSettingInfo() throws Exception {
		return primxSupportService.loadPasswordSettingInfo();
	}
}
