package egovframework.com.primx.fun.usr.web;

import java.util.Map;
import java.util.HashMap;
import java.util.List;

import javax.annotation.Resource;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.PostMapping;

import egovframework.com.cmm.EgovMessageSource;
import egovframework.com.cmm.LoginVO;
import egovframework.com.cmm.util.EgovUserDetailsHelper;
import egovframework.com.primx.fun.cmm.service.MessageException;
import egovframework.com.primx.fun.sys.service.PasswordSettingVO;
import egovframework.com.primx.fun.sys.service.PrimxSystemService;
import egovframework.com.primx.fun.usr.service.UserRegistrationDTO;
import egovframework.com.primx.fun.usr.service.PrimxUserService;

/**
 * 사용자 관리 관련 기능
 * @author 김영우
 * @since 2024.12.3
 * @version 1.0
 * @see
 *
 * <pre>
 * << 개정이력(Modification Information) >>
 *
 *   수정일      수정자          수정내용
 *  -------    --------    ---------------------------
 *  2024.12.03  김영우          최초 생성
 *
 *  </pre>
 */

@Controller
@RequestMapping(value = "/primx/fun/usr")
public class PrimxUserController {

	@Autowired
	private EgovMessageSource egovMessageSource;

	@Resource(name = "primxUserService")
	private PrimxUserService primxUserService;

	@Resource(name = "primxSystemService")
	private PrimxSystemService primxSystemService;

	@ResponseBody
	@RequestMapping(value = "/myInfo.do", method = RequestMethod.GET)
	public Map<String, Object> getMyInfo() {

		System.out.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 111");
		// 1. 현재 인증된 사용자 정보를 가져옴
		LoginVO user = (LoginVO) EgovUserDetailsHelper.getAuthenticatedUser();
		if(user == null)
			System.out.println("user is null !!");
		System.out.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ID: ["+user.getId()+"]");
		
		List<String> authList = EgovUserDetailsHelper.getAuthorities();

		Map<String, Object> response = new HashMap<>();
		try {
			UserRegistrationDTO emplyrDetails = primxUserService.getUserDetails(user.getId());
			emplyrDetails.setUserRole(authList.get(0));
			response.put("data", emplyrDetails);
			response.put("success", true);
			response.put("message", egovMessageSource.getMessage("message.user.details.success"));
		} catch (Exception e) {
			response.put("success", false);
			response.put("message", egovMessageSource.getMessage("message.user.details.error"));
		}
		return response;
	}
	
	//'ADMIN','SUPER'
	@ResponseBody
	@RequestMapping(value = "/details.do", method = RequestMethod.GET)
	public Map<String, Object> getUserDetails(@RequestParam("emplyrId") String emplyrId) {
		Map<String, Object> response = new HashMap<>();
		try {
			UserRegistrationDTO emplyrDetails = primxUserService.getUserDetails(emplyrId);
			response.put("data", emplyrDetails);
			response.put("success", true);
			response.put("message", egovMessageSource.getMessage("message.user.details.success"));
		} catch (Exception e) {
			response.put("success", false);
			response.put("message", egovMessageSource.getMessage("message.user.details.error"));
		}
		return response;
	}

	@ResponseBody
	@RequestMapping(value = "/resetPassword.do")
	public Map<String, Object> resetPassword(@RequestParam("emplyrId") String emplyrId) {
		Map<String, Object> response = new HashMap<>();
		try {
			Map<String, Object> passwordSettingInfo = primxSystemService.loadPasswordSettingInfo();
			PasswordSettingVO passwordSettingVO = (PasswordSettingVO) passwordSettingInfo.get("data");

			boolean result = primxUserService.resetPassword(emplyrId, passwordSettingVO);
			if (result) {
				response.put("success", true);
				response.put("message", egovMessageSource.getMessage("message.user.password.update.success"));
			} else {
				response.put("success", false);
				response.put("message", egovMessageSource.getMessage("message.user.password.update.fail"));
			}
		} catch (MessageException e) {
			response.put("error", true);
			response.put("message", e.getMessage());
		}
		return response;
	}

	@ResponseBody
	@RequestMapping(value = "/updatePassword.do", method = RequestMethod.POST)
	public Map<String, Object> updatePassword(@RequestParam("emplyrId") String emplyrId, 
	@RequestParam("currentPassword") String currentPassword, @RequestParam("newPassword") String newPassword) {	
		Map<String, Object> response = new HashMap<>();
		try {
			Map<String, Object> passwordSettingInfo = primxSystemService.loadPasswordSettingInfo();
			PasswordSettingVO passwordSettingVO = (PasswordSettingVO) passwordSettingInfo.get("data");

			boolean result = primxUserService.updatePassword(emplyrId, currentPassword, newPassword, passwordSettingVO);
			if (result) {
				response.put("success", true);
				response.put("message", egovMessageSource.getMessage("message.user.password.update.success"));
			} else {
				response.put("success", false);
				response.put("message", egovMessageSource.getMessage("message.user.password.update.fail"));
			}
		} catch (MessageException e) {
			response.put("error", true);
			response.put("message", e.getMessage());
		}
		return response;
	}


	@RequestMapping(value = "/system/view.do")
	public String mySystemInfo(ModelMap model) throws Exception {
		
		return "egovframework/com/primx/fun/usr/mySystemInfo";
	}

	//'ADMIN','SUPER'
	@ResponseBody
	@RequestMapping(value = "/deleteUser.do", method = RequestMethod.POST)
	public Map<String, Object> deleteUser(@RequestParam("emplyrId") String emplyrId) {
		Map<String, Object> response = new HashMap<>();
		try {
			boolean result = primxUserService.deleteUser(emplyrId);
			if (result) {
				response.put("success", true);
				response.put("message", egovMessageSource.getMessage("message.user.delete.success"));
			} else {
				response.put("success", false);
				response.put("message", egovMessageSource.getMessage("message.user.delete.fail"));
			}
		} catch (Exception e) {
			response.put("success", false);
			response.put("errorCode", "SYSTEM_ERROR");
			response.put("message", egovMessageSource.getMessage("message.system.error"));
		}
		return response;
	}

	@ResponseBody
	@RequestMapping(value = "/registerUser.do", method = RequestMethod.POST)
	public Map<String, Object> registerUser(@RequestBody UserRegistrationDTO userRegistrationDTO) {
		Map<String, Object> response = new HashMap<>();
		try {
			// 사용자 등록
			primxUserService.registerUser(userRegistrationDTO);
			// 새 IP 정보 삽입
			primxUserService.insertUserIp(userRegistrationDTO);
			response.put("success", true);
			response.put("message", egovMessageSource.getMessage("message.user.register.success"));
		} catch (Exception e) {
			response.put("success", false);
			response.put("message", egovMessageSource.getMessage("message.user.register.error"));
		}
		return response;
	}

	// 사용자 수정 요청 처리
	@PostMapping("/updateUser.do")
	@ResponseBody
	public Map<String, Object> updateUser(@RequestBody UserRegistrationDTO userRegistrationDTO) {
		Map<String, Object> response = new HashMap<>();
		try {
			userRegistrationDTO.convertUTCtoUserTime();
			// 기존 IP 정보 삭제
			primxUserService.deleteUserIp(userRegistrationDTO.getEmplyrId());

			// 사용자 정보 업데이트
			primxUserService.updateUser(userRegistrationDTO);

			// 새 IP 정보 삽입
			primxUserService.insertUserIp(userRegistrationDTO);

			response.put("success", true);
			response.put("message", egovMessageSource.getMessage("message.user.update.success"));
		} catch (Exception e) {
			response.put("success", false);
			response.put("message", egovMessageSource.getMessage("message.user.update.error"));
		}
		return response;
	}

	@ResponseBody
	@RequestMapping(value = "/mySystemInfo.do", method = RequestMethod.POST)
	public Map<String, Object> getMySystemInfo() {
		Map<String, Object> response = new HashMap<>();
		try {
			response = primxSystemService.selectMySystemStatusInfo();
		} catch (Exception e) {
			response.put("success", false);
			response.put("message", egovMessageSource.getMessage("message.system.error"));
		}
		return response;
	}
}
