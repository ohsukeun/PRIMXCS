package egovframework.com.primx.fun.cmm.web;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.annotation.Resource;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

import egovframework.com.cmm.EgovMessageSource;
import egovframework.com.cmm.util.EgovUserDetailsHelper;
import egovframework.com.primx.fun.dpt.service.PrimxDepartmentService;
import egovframework.com.primx.fun.rqt.service.PrimxRequestService;
import egovframework.com.primx.fun.sys.service.PrimxSystemService;
import egovframework.com.primx.fun.usr.service.PrimxUserService;
import egovframework.com.primx.fun.cmm.service.PrimxAlarmService;
import egovframework.com.primx.fun.cmm.service.PrimxRoleService;
import egovframework.com.primx.fun.dpt.service.DeptSearchVO;

@Controller
@RequestMapping(value = "/primx/fun/cmm")
public class PrimxCommonController {

	@Resource(name = "egovMessageSource")
	private EgovMessageSource egovMessageSource;

	@Resource(name = "primxUserService")
	private PrimxUserService primxUserService;

	@Resource(name = "primxSystemService")
	private PrimxSystemService primxSystemService;

	@Resource(name = "primxDepartmentService")
	private PrimxDepartmentService primxDepartmentService;

	@Resource(name = "primxRoleService")
	private PrimxRoleService primxRoleService;
	
	@Resource(name = "primxRequestService")
	private PrimxRequestService primxRequestService;

	@Resource(name = "primxAlarmService")
	private PrimxAlarmService primxAlarmService;
	
	@ResponseBody
	@RequestMapping(value = "/checkUserId.do", method = RequestMethod.POST)
	public Map<String, Object> checkUserId(@RequestParam("emplyrId") String emplyrId) {
		System.out.println("emplyrId: ["+emplyrId +"]");
		Map<String, Object> response = new HashMap<>();
		try {
			boolean isAvailable = primxUserService.isUserAvailable(emplyrId);
			response.put("available", isAvailable);
			response.put("message", egovMessageSource.getMessage("message.user.check.success"));
		} catch (Exception e) {
			response.put("available", false);
			response.put("message", egovMessageSource.getMessage("message.user.check.error"));
		}
		return response;
	}

	@ResponseBody
	@RequestMapping(value = "/checkMail.do", method = RequestMethod.POST)
	public Map<String, Object> checkMail(@RequestParam("mail") String mail) {
		Map<String, Object> response = new HashMap<>();
		try {
			boolean isAvailable = primxUserService.checkMail(mail);
			response.put("available", isAvailable);
			response.put("message", egovMessageSource.getMessage("message.user.check.success"));
		} catch (Exception e) {
			response.put("available", false);
			response.put("message", egovMessageSource.getMessage("message.user.check.error"));
		}
		return response;
	}

	@ResponseBody
	@RequestMapping(value = "/roleInfo.do", method = RequestMethod.GET)
	public Map<String, Object> getRoleInfo(@RequestParam("searchType") int searchType) {
		Map<String, Object> response = new HashMap<>();
		try {
			response = primxRoleService.selectRoleList(searchType);

		} catch (Exception e) {
			response.put("status", "error");
			response.put("message", e.getMessage());
		}
		return response;
	}

	@ResponseBody
	@RequestMapping(value = "/companyInfo.do", method = RequestMethod.GET)
	public Map<String, Object> getCompanyInfo() {
		Map<String, Object> response = new HashMap<>();
		try {	
			response = primxDepartmentService.selectCompanyList();
		} catch (Exception e) {
			response.put("status", "error");
			response.put("message", e.getMessage());
		}
		return response;
	}

	@ResponseBody
	@RequestMapping(value = "/getDeptUserIdList.do", method = RequestMethod.POST)
	public Map<String, Object> getDeptUserIdList(@ModelAttribute("searchVO") DeptSearchVO searchVO) {
		Map<String, Object> response = new HashMap<>();
		try {

			// 선택된 deptId로 검색
			// 선택된 deptId와 parentId가 deptId인 부서 검색
			List<String> deptIds = primxDepartmentService.selectDeptUserIdList(searchVO);
			for(String dptId : deptIds) {
				System.out.println("--------------------------dptId: ["+dptId+"]");
			}
			response = primxUserService.selectUsersByDeptIds(deptIds, searchVO);

		} catch (Exception e) {
			response.put("status", "error");
			response.put("message", e.getMessage());
		}
		return response;
	}

	@ResponseBody
	@RequestMapping(value = "/smartContractUserList.do", method = RequestMethod.POST)
	public Map<String, Object> smartContractUserList(@ModelAttribute("searchVO") DeptSearchVO searchVO) {
		Map<String, Object> response = new HashMap<>();
		try {

			// 선택된 deptId로 검색
			// 선택된 deptId와 parentId가 deptId인 부서 검색
			List<String> deptIds = primxDepartmentService.selectDeptUserIdList(searchVO);
			for(String dptId : deptIds) {
				System.out.println("--------------------------dptId: ["+dptId+"]");
			}
			response = primxUserService.selectSmartContractUserList(deptIds, searchVO);

		} catch (Exception e) {
			response.put("status", "error");
			response.put("message", e.getMessage());
		}
		return response;
	}

	@ResponseBody
	@RequestMapping(value = "/createRequestSetting/list.do", method = RequestMethod.GET)
	public Map<String, Object> getCreateRequestSettingList() throws Exception {
		return primxRequestService.getCreateRequestSettingList();
	}
	
	@ResponseBody
	@RequestMapping(value = "/loadPasswordSettingInfo.do", method = RequestMethod.GET)
	public Map<String, Object> loadPasswordSettingInfo() throws Exception {
		return primxSystemService.loadPasswordSettingInfo();
	}

	@ResponseBody
	@RequestMapping(value = "/alarm/list.do", method = RequestMethod.GET)
	public Map<String, Object> loadAlarm() throws Exception {
		return primxAlarmService.selectAlarmList();
	}

	@ResponseBody
	@RequestMapping(value = "/alarm/read.do", method = RequestMethod.POST)
	public Map<String, Object> readAlarm(@RequestParam("seqNo") String seqNo) throws Exception {
		return primxAlarmService.readAlarm(seqNo);
	}

	@ResponseBody
	@RequestMapping(value = "/alarm/readAll.do", method = RequestMethod.POST)
	public Map<String, Object> readAllAlarm() throws Exception {
		return primxAlarmService.readAllAlarm();
	}
	
    /**
     * 세션 유효성을 체크합니다.
     * @param request HttpServletRequest
     * @return Map<String, Object> 세션 유효 여부를 담은 결과
     */
    @RequestMapping("/checkSession.do")
    @ResponseBody
    public Map<String, Object> checkSession(HttpServletRequest request) {
        Map<String, Object> result = new HashMap<>();
        
        // 로그인 정보 확인
        Boolean isAuthenticated = EgovUserDetailsHelper.isAuthenticated();
        
        if (isAuthenticated) {
            HttpSession session = request.getSession(false);
            // 세션이 존재하고 유효한 경우
            result.put("isValid", session != null && !session.isNew());
        } else {
            result.put("isValid", false);
        }
        
        return result;
    }

    @RequestMapping("/sessionExpired.do")
    @ResponseBody
    public Map<String, Object> sessionExpired(HttpServletRequest request) {
        Map<String, Object> result = new HashMap<>();
        
        try {
            // 세션 무효화
            HttpSession session = request.getSession(false);
            if (session != null) {
                session.invalidate();
            }
            
            // 로그인 정보 초기화
            request.getSession().setAttribute("loginVO", null);
            request.getSession().setAttribute("accessUser", null);
            
            result.put("success", true);
            result.put("message", "세션이 만료되었습니다.");
        } catch (Exception e) {
            result.put("success", false);
            result.put("message", "세션 만료 처리 중 오류가 발생했습니다.");
        }
        
        return result;
    }
}
