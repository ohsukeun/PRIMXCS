package egovframework.com.primx.fun.cmm.web;

import java.util.HashMap;
import java.util.Map;

import javax.annotation.Resource;
import javax.servlet.http.HttpServletRequest;

import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Controller;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;

import egovframework.com.cmm.EgovMessageSource;
import egovframework.com.cmm.LoginVO;
import egovframework.com.cmm.util.EgovUserDetailsHelper;
import egovframework.com.cmm.web.EgovComAbstractController;
import egovframework.com.primx.fun.cmm.service.MyPageResponseDTO;
import egovframework.com.primx.fun.cmm.service.MyPageUpdateDTO;
import egovframework.com.primx.fun.cmm.service.PrimxAdminService;
import egovframework.com.primx.fun.cmm.service.PrimxCommonService;
import egovframework.com.primx.fun.cmm.service.UserInfoVO;

@Controller
@RequestMapping(value = "/primx/fun/cmm")
public class PrimxCommonServiceController extends EgovComAbstractController {

	@Resource(name = "egovMessageSource")
	private EgovMessageSource egovMessageSource;
	
	@Resource(name = "PrimxCommonService")
	private PrimxCommonService primxCommonService;
	
	@Resource(name = "primxAdminService")
	private PrimxAdminService primxAdminService;

	@RequestMapping(value = "/view.do")
	public String getView(ModelMap model) throws Exception {

		return "egovframework/com/primx/fun/cmm/index";
	}
	
	@RequestMapping(value = "/myPage.do")
	public String getMypage(ModelMap model) throws Exception {

		return "egovframework/com/primx/fun/cmm/myPage";
	}
	
	@RequestMapping(value = "/download.do")
	public String getDownload(ModelMap model) throws Exception {

		return "egovframework/com/primx/fun/cmm/download";
	}


	 @GetMapping("/getMyInfo.do")
	    public ResponseEntity<MyPageResponseDTO> getMyPage(HttpServletRequest request) {
	        // 요청 헤더에서 Authorization 추출
	        String authHeader = request.getHeader("Authorization");
	        
	        if (authHeader == null || !authHeader.startsWith("Bearer ")) {
	            return ResponseEntity.status(HttpStatus.UNAUTHORIZED).build();
	        }
			
			LoginVO user = (LoginVO) EgovUserDetailsHelper.getAuthenticatedUser();
	        MyPageResponseDTO response = new MyPageResponseDTO();
			UserInfoVO userInfo = new UserInfoVO();
			try {
				userInfo = primxCommonService.getMyPageInfo(user.getId());
			} catch (Exception e) {
				e.printStackTrace();
			}
	        response.setUserId(userInfo.getUserId());
	        response.setEmail(userInfo.getEmail());
	        response.setPhoneNumber(userInfo.getPhoneNumber());
	        response.setUserName(userInfo.getUserName());
	        response.setCompanyCode(userInfo.getCompanyCode());

	        return ResponseEntity.ok(response);
	    }
	// 회원 정보 수정
	@PutMapping("/myPageEdit.do")
	public ResponseEntity<Map<String, Object>> updateMyPage(@RequestBody MyPageUpdateDTO updateDTO) {
		Map<String, Object> response = new HashMap<>();
		try {
			response = primxCommonService.updateMyPageInfo(updateDTO);
			if (response.get("success").equals(true)) {
				return ResponseEntity.ok(response);
			} else {
				return ResponseEntity.badRequest().body(response);
			}
		} catch (Exception e) {
			e.printStackTrace();
			response.put("success", false);
			response.put("message", "회원 정보 수정에 실패하였습니다.");
			return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR).body(response);
		}
	}
}
