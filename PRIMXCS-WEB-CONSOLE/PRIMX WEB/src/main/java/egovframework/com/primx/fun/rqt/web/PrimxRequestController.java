package egovframework.com.primx.fun.rqt.web;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import javax.annotation.Resource;

import org.springframework.stereotype.Controller;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

import egovframework.com.primx.fun.rqt.service.PrimxRequestService;
import egovframework.com.primx.fun.rqt.service.RequestApproversVO;
import egovframework.com.primx.fun.rqt.service.RequestModifiedVO;
import egovframework.com.primx.fun.rqt.service.RequestSearchVO;
import egovframework.com.primx.fun.rqt.service.RequestUserApprovalDTO;
import egovframework.com.primx.fun.sys.service.PrimxSystemService;
import egovframework.com.primx.fun.usr.service.PrimxUserService;
import egovframework.com.primx.fun.usr.service.UserRegistrationDTO;
import lombok.extern.slf4j.Slf4j;


/**
 * 요청(Request) 관리 관련 기능
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
//'USER'
@Slf4j
@Controller
@RequestMapping(value = "/primx/fun/rqt")
public class PrimxRequestController {

	@Resource(name = "primxRequestService")
	PrimxRequestService primxRequestService;

	@Resource(name = "primxSystemService")
	PrimxSystemService primxSystemService;
	
	@Resource(name = "primxUserService")
	private PrimxUserService primxUserService;

	@RequestMapping(value = "/createRequest/view.do")
	public String getRequestView(ModelMap model) throws Exception {
		return "egovframework/com/primx/fun/rqt/createRequest";
	}

	@ResponseBody
	@RequestMapping(value = "/createRequest/list.do", method = RequestMethod.POST)
	public Map<String, Object> getRequestList(RequestSearchVO requestSearchVO) throws Exception {
		log.debug("createRequest list call >>> {}", requestSearchVO);
		
		requestSearchVO.convertUserTimeToUTC();
		
		// JSON 형식 응답 구성
        Map<String, Object> rtnMap = primxRequestService.selectListRequestInfo(requestSearchVO);
    	
		return rtnMap;
	}
	
	@ResponseBody
	@RequestMapping(value = "/createRequest/info.do", method = RequestMethod.GET)
	public Map<String, Object> getRequestInfo(@RequestParam(value = "requestId", required = true) int requestId) throws Exception {
		log.debug("createRequest info call >>> {}", requestId);
		
		// JSON 형식 응답 구성
		Map<String, Object> rtnMap = primxRequestService.selectRequestInfo(requestId);
		
		return rtnMap;
	}
	
	@ResponseBody
	@RequestMapping(value = "/createRequest/modified.do", method = RequestMethod.POST)
	public Map<String, Object> updateRequestInfo(@RequestBody RequestModifiedVO requestModifiedVO) throws Exception {
		log.debug("createRequest modified call >>> {}", requestModifiedVO);
		
		// JSON 형식 응답 구성
		Map<String, Object> rtnMap = primxRequestService.updateRequestInfo(requestModifiedVO);
		
		return rtnMap;
	}

	@RequestMapping(value = "/requestApproval/view.do")
	public String getRequestApprovalView(ModelMap model) throws Exception {
		return "egovframework/com/primx/fun/rqt/approvalPage";
	}

	@ResponseBody
	@RequestMapping(value = "/requestApproval/list.do")
	public Map<String, Object> getRequestApprovalList(RequestSearchVO requestSearchVO) throws Exception {
		log.debug("requestApproval list call >>> {}", requestSearchVO);
		
		requestSearchVO.convertUserTimeToUTC();
		
		// JSON 형식 응답 구성
        Map<String, Object> rtnMap = primxRequestService.selectListRequestApprovalInfo(requestSearchVO);
    	
		return rtnMap;
	}

	@ResponseBody
	@RequestMapping(value = "/requestApproval/info.do")
	public Map<String, Object> getRequestApprovalInfo(@RequestParam(value = "requestId", required = true) int requestId) throws Exception {
		log.debug("requestApproval info call >>> {}", requestId);
		
		// JSON 형식 응답 구성
		Map<String, Object> rtnMap = primxRequestService.selectRequestApprovalInfo(requestId);
		
		return rtnMap;
	}

	@ResponseBody
	@RequestMapping(value = "/requestApproval/modified.do", method = RequestMethod.POST)
	public Map<String, Object> updateRequestApprovalInfo(RequestApproversVO requestApproversVO) throws Exception {
		log.debug("requestApproval modified call >>> {}", requestApproversVO);
		requestApproversVO.convertUserTimeToUTC();
		System.out.println("############################## requestApproversVO: " + requestApproversVO.toString());
		// JSON 형식 응답 구성
		Map<String, Object> rtnMap = primxRequestService.updateRequestApprovalInfo(requestApproversVO);
		
		String order = (String) rtnMap.get("order");
		if("addUser".equals(order)) {
			UserRegistrationDTO userRegistrationDTO = (UserRegistrationDTO) rtnMap.get("data");
			primxUserService.registerUser(userRegistrationDTO);
		}
		else if("setPassword".equals(order)) {
			UserRegistrationDTO userRegistrationDTO = (UserRegistrationDTO) rtnMap.get("data");
			primxUserService.updateUserExpiredDate(userRegistrationDTO.getEmplyrId(), userRegistrationDTO.getExpiredDate());
		}
		return rtnMap;



	}

	@ResponseBody
	@RequestMapping(value = "/createRequest/getRole.do")
	public Map<String, Object> getRole() throws Exception {
		return primxSystemService.selectRoleList();
	}

	@ResponseBody
	@RequestMapping(value = "/createRequest/save.do", method = RequestMethod.POST)
	public Map<String, Object> saveRequest(@RequestBody RequestUserApprovalDTO requestUserApprovalDTO) throws Exception {
		log.debug("createRequest save call >>> {}", requestUserApprovalDTO);
		return primxRequestService.saveRequest(requestUserApprovalDTO);
	}


	@ResponseBody
	@RequestMapping(value = "/createRequest/loadFileList.do", method = RequestMethod.POST)
	public Map<String, Object> loadFileList(@RequestParam(value = "fileGroupId", required = true) int fileGroupId) throws Exception {
		return primxRequestService.selectFileList(fileGroupId);
	}

	@ResponseBody
	@RequestMapping(value = "/createRequest/loadApprovalLine.do", method = RequestMethod.POST)
	public Map<String, Object> loadApprovalLine(@RequestParam(value = "requestCode", required = true) String requestCode) throws Exception {
		return primxRequestService.selectApprovalLine(requestCode);
	}

	@ResponseBody
	@RequestMapping(value = "/requestApproval/batchApproval.do", method = RequestMethod.POST)
	public Map<String, Object> batchApproval(
		@RequestParam(value = "requestCodes[]") List<String> requestCodes,
		@RequestParam(value = "description") String description
	) throws Exception {
		Map<String, Object> rtnMap = primxRequestService.batchApproval(requestCodes, description);

		ArrayList<UserRegistrationDTO> rqtList = (ArrayList<UserRegistrationDTO>) rtnMap.get("rqtList");
		for(UserRegistrationDTO userRegistrationDTO : rqtList) {
			if("A".equals(userRegistrationDTO.getApprovalCode())) {
				primxUserService.registerUser(userRegistrationDTO);
			}
			else if("C".equals(userRegistrationDTO.getApprovalCode())) {
				primxUserService.updateUserExpiredDate(userRegistrationDTO.getEmplyrId(), userRegistrationDTO.getExpiredDate());
			}
		}
		
		return rtnMap;
	}
}
