package egovframework.com.primx.fun.dpt.web;

import java.util.Map;
import java.util.HashMap;

import javax.annotation.Resource;

import org.springframework.stereotype.Controller;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

import egovframework.com.cmm.EgovMessageSource;
import egovframework.com.cmm.web.EgovComAbstractController;
import egovframework.com.primx.fun.dpt.service.PrimxDepartmentService;
import egovframework.com.primx.fun.sys.service.PrimxSystemService;
import egovframework.com.primx.fun.cmm.service.MessageException;
import egovframework.com.primx.fun.dpt.service.DeptSearchVO;
import egovframework.com.primx.fun.dpt.service.DeptVO;

/**
 * 부서 관리 관련 기능
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
@RequestMapping(value = "/primx/fun/dpt")
public class PrimxDepartmentController extends EgovComAbstractController {

	@Resource(name = "egovMessageSource")
	private EgovMessageSource egovMessageSource;

	@Resource(name = "primxSystemService")
	private PrimxSystemService primxSystemService;

	@Resource(name = "primxDepartmentService")
	private PrimxDepartmentService primxDepartmentService;


	@ResponseBody
	@RequestMapping(value = "/create.do")
	public Map<String, Object> createDepartment(@ModelAttribute("deptVO") DeptVO deptVO) {
		Map<String, Object> response = new HashMap<>();
		
		try {
			// 부서 코드 생성
			String deptCode = primxSystemService.generateCode("D");
			
			System.out.println("####################deptCode:" + deptCode);
			// 부서 생성
			DeptVO newDeptVO = primxDepartmentService.insertDepartment(deptVO, deptCode);
			
			response.put("success", true);
			response.put("data", newDeptVO);
			response.put("message", egovMessageSource.getMessage("message.dept.create.success"));
		} catch (MessageException e) {
			response.put("success", false);
			response.put("message", "부서 등록에 실패했습니다: " + e.getMessage());
		} catch (Exception e) {
			response.put("success", false);
			response.put("errorCode", "SYSTEM_ERROR");
			
			response.put("message", egovMessageSource.getMessage("message.system.error"));
		}
		
		return response;
	}

	@RequestMapping(value = "/view.do")
	public String getView(ModelMap model) throws Exception {

		return "egovframework/com/primx/fun/dpt/departmentUserSettings";
	}

	@ResponseBody
	@RequestMapping(value = "/tree.do")
	public Map<String, Object> getDeptTree(@ModelAttribute("searchVO") DeptSearchVO searchVO) throws Exception {
		
		System.out.println("##################tree.do##########searchVO:" + searchVO);
		Map<String, Object> response = new HashMap<>();
		try {
			Map<String, Object> treeData = primxDepartmentService.selectDeptTree(searchVO);
			response.put("status", "success");
			response.put("data", treeData.get("data"));
		} catch (Exception e) {
			response.put("status", "error");
			response.put("message", e.getMessage());
		}
		return response;
	}

	@ResponseBody
	@RequestMapping(value = "/details.do")
	public Map<String, Object> getDetails(@RequestParam("deptId") String deptId) {
		Map<String, Object> response = new HashMap<>();
		
		try {
			DeptVO deptVO = primxDepartmentService.selectDepartmentByDeptId(deptId);

			response.put("success", true);
			response.put("data", deptVO);
			response.put("message", "부서 정보를 성공적으로 조회했습니다.");
		} catch (MessageException e) {
			response.put("success", false);
			response.put("errorCode", e.getErrorCode());
			response.put("message", egovMessageSource.getMessage(e.getMessageKey()));
		} catch (Exception e) {
			response.put("success", false);
			response.put("message", e.getMessage());
		}
		
		return response;
	}

	@ResponseBody
	@RequestMapping(value = "/update.do")
	public Map<String, Object> updateDepartment(@ModelAttribute("deptVO") DeptVO deptVO) {
		Map<String, Object> response = new HashMap<>();
		
		try {
			// 부서 수정
			DeptVO updatedDeptVO = primxDepartmentService.updateDepartment(deptVO);
			
			response.put("success", true);
			response.put("data", updatedDeptVO);
			response.put("message", "부서가 성공적으로 수정되었습니다.");
		} catch (MessageException e) {
			response.put("success", false);
			response.put("errorCode", e.getErrorCode());
			response.put("message", egovMessageSource.getMessage(e.getMessageKey()));
		} catch (Exception e) {
			response.put("success", false);
			response.put("message", e.getMessage());
		}
		
		return response;
	}

	@ResponseBody
	@RequestMapping(value = "/delete.do")
	public Map<String, Object> deleteDepartment(@RequestParam("deptId") String deptId) {
		Map<String, Object> response = new HashMap<>();
		
		try {
			String messageKey = primxDepartmentService.deleteDepartment(deptId);
			response.put("success", true);
			response.put("message", egovMessageSource.getMessage(messageKey));
		} catch (MessageException e) {
			response.put("success", false);
			response.put("errorCode", e.getErrorCode());
			response.put("message", egovMessageSource.getMessage(e.getMessageKey()));
		} catch (Exception e) {
			response.put("success", false);
			response.put("errorCode", "SYSTEM_ERROR");
			
			response.put("message", egovMessageSource.getMessage("message.system.error"));
		}
		
		return response;
	}

	@ResponseBody
	@RequestMapping(value = "/checkCompanyCode.do", method = RequestMethod.POST)
	public Map<String, Object> checkCompanyCode(@RequestParam("companyCode") String companyCode) {
		Map<String, Object> response = new HashMap<>();

		try {
			response = primxDepartmentService.checkCompanyCode(companyCode);
		} catch (Exception e) {
			response.put("success", false);
			response.put("message", e.getMessage());
		}
		
		return response;
	}

	@ResponseBody
	@RequestMapping(value = "/loadRootCompanyInfo.do", method = RequestMethod.GET)
	public Map<String, Object> loadRootCompanyInfo() {
		Map<String, Object> response = new HashMap<>();
		try {
			response = primxDepartmentService.loadRootCompanyInfo();
		} catch (Exception e) {
			response.put("success", false);
			response.put("message", e.getMessage());
		}
		return response;
	}

	@ResponseBody
	@RequestMapping(value = "/createCompany.do", method = RequestMethod.POST)
	public Map<String, Object> createCompany(@ModelAttribute("deptVO") DeptVO deptVO) {
		Map<String, Object> response = new HashMap<>();
		
		try {
			DeptVO newDeptVO = primxDepartmentService.insertDepartment(deptVO, deptVO.getDeptId());
			response.put("success", true);
			response.put("data", newDeptVO);
			response.put("message", egovMessageSource.getMessage("message.dept.create.success"));
		} catch (Exception e) {
			response.put("success", false);
			response.put("message", e.getMessage());
		}
		return response;
	}

	@ResponseBody
	@RequestMapping(value = "/loadCompanyCodeList.do", method = RequestMethod.GET)
	public Map<String, Object> loadCompanyCodeList() {
		Map<String, Object> response = new HashMap<>();
		
		try {
			response = primxDepartmentService.loadCompanyCodeList();
		} catch (Exception e) {
			response.put("success", false);
			response.put("message", e.getMessage());
		}
		return response;
	}
}
