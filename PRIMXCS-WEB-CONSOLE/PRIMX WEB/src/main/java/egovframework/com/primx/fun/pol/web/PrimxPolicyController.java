package egovframework.com.primx.fun.pol.web;

import java.util.List;
import java.util.Map;
import java.util.ArrayList;
import java.util.HashMap;

import javax.annotation.Resource;

import org.springframework.stereotype.Controller;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

import egovframework.com.cmm.EgovMessageSource;
import egovframework.com.primx.fun.pol.service.DistributionPolicySearchDTO;
import egovframework.com.primx.fun.pol.service.PolicyDetailInfoVO;
import egovframework.com.primx.fun.pol.service.PrimxAgentPolicyService;
import egovframework.com.primx.fun.pol.service.PrimxPolicyService;
import egovframework.com.primx.fun.pol.service.ProductPolicySearchDTO;


/**
 * 정책 관리 관련 기능
 * 
 * @author 김영우
 * @since 2024.12.3
 * @version 1.0
 * @see
 *
 *      <pre>
 * << 개정이력(Modification Information) >>
 *
 *   수정일      수정자          수정내용
 *  -------    --------    ---------------------------
 *  2024.12.03  김영우          최초 생성
 *
 *      </pre>
 */
//'ADMIN','SUPER'
@Controller
@RequestMapping(value = "/primx/fun/pol")
public class PrimxPolicyController {

	@Resource(name = "egovMessageSource")
	private EgovMessageSource egovMessageSource;

	@Resource(name = "primxAgentPolicyService")
	private PrimxAgentPolicyService primxAgentPolicyService;

	@Resource(name = "primxPolicyService")
	private PrimxPolicyService primxPolicyService;

	@RequestMapping(value = "/distribute/view.do")
	public String getDistributeList(ModelMap model) throws Exception {

		return "egovframework/com/primx/fun/pol/distributionPolicy";
	}

	@RequestMapping(value = "/product/view.do")
	public String getProductList(ModelMap model) throws Exception {
		return "egovframework/com/primx/fun/pol/productPolicy";
	}

	@ResponseBody
	@RequestMapping(value = "/loadProductPolicy.do", method = RequestMethod.POST)
	public Map<String, Object> loadProductPolicy(@RequestParam(value = "product", required = true) String product) throws Exception {
		ProductPolicySearchDTO searchDTO = new ProductPolicySearchDTO();
		searchDTO.setSearchProduct(product);
		return primxPolicyService.loadProductPolicyList(searchDTO);
	}

	@ResponseBody
	@RequestMapping(value = "/product/policy/list.do", method = RequestMethod.POST)
	public Map<String, Object> loadProductPolicyList(ProductPolicySearchDTO searchDTO) throws Exception {
		searchDTO.convertUserTimeToUTC();
		return primxPolicyService.loadProductPolicyList(searchDTO);
	}

	@ResponseBody
	@RequestMapping(value = "/default/list.do", method = RequestMethod.POST)
	public Map<String, Object> loadDefaultPolicyList() throws Exception {
		return primxPolicyService.loadDefaultPolicyList();
	}
	
	@ResponseBody
	@RequestMapping(value = "/mandatory/list.do", method = RequestMethod.POST)
	public Map<String, Object> loadMandatoryPolicyList() throws Exception {
		return primxPolicyService.loadMandatoryPolicyList();
	}

	@ResponseBody
	@RequestMapping(value = "/default/save.do", method = RequestMethod.POST)
	public Map<String, Object> saveDefaultPolicyList(@RequestBody List<PolicyDetailInfoVO> resultList) {
		Map<String, Object> result = new HashMap<>();
		try {
			return primxPolicyService.saveDefaultPolicyList(resultList);
		} catch (Exception e) {
			result.put("status", "error");
			result.put("message", e.getMessage());
			return result;
		}
	}

	@ResponseBody
	@RequestMapping(value = "/product/getPolicyInfo.do", method = RequestMethod.POST)
	public Map<String, Object> getPolicyInfo(@RequestParam(value = "policyCode", required = true) String policyCode) throws Exception {
		return primxPolicyService.getPolicyInfo(policyCode);
	}

	@ResponseBody
	@RequestMapping(value = "/product/saveProductPolicy.do", method = RequestMethod.POST)
	public Map<String, Object> saveProductPolicy(@RequestBody Map<String, Object> requestData) throws Exception {
		String productName = (String) requestData.get("productName");
		String policyName = (String) requestData.get("policyName");
		List<Map<String, Object>> rawList = (List<Map<String, Object>>) requestData.get("copyPolicyData");
		
		List<PolicyDetailInfoVO> copyPolicyData = new ArrayList<>();
		for (Map<String, Object> item : rawList) {
			PolicyDetailInfoVO vo = PolicyDetailInfoVO.builder()
				.polId((Integer) item.get("polId"))
				.polCode((String) item.get("polCode"))
				.polData((String) item.get("polData"))
				.build();
			copyPolicyData.add(vo);
		}
		
		return primxPolicyService.saveProductPolicy(policyName, productName, copyPolicyData);
	}

	@ResponseBody
	@RequestMapping(value = "/product/updateProductPolicy.do", method = RequestMethod.POST)
	public Map<String, Object> updateProductPolicy(@RequestBody Map<String, Object> requestData) throws Exception {
		String productName = (String) requestData.get("productName");
		String policyName = (String) requestData.get("policyName");
		String policyCode = (String) requestData.get("policyCode");
		List<Map<String, Object>> rawList = (List<Map<String, Object>>) requestData.get("copyPolicyData");
		
		List<PolicyDetailInfoVO> copyPolicyData = new ArrayList<>();
		for (Map<String, Object> item : rawList) {
			PolicyDetailInfoVO vo = PolicyDetailInfoVO.builder()
				.polId((Integer) item.get("polId"))
				.polCode((String) item.get("polCode"))
				.polData((String) item.get("polData"))
				.build();
			copyPolicyData.add(vo);
		}
		
		return primxPolicyService.updateProductPolicy(policyCode, policyName, productName, copyPolicyData);
	}

	@ResponseBody
	@RequestMapping(value = "/product/deletePolicy.do", method = RequestMethod.POST)
	public Map<String, Object> deletePolicy(@RequestParam(value = "policyCode", required = true) String policyCode, @RequestParam(value = "productName", required = true) String productName) throws Exception {
		// 에러 발생시 예외 처리
		Map<String, Object> result = new HashMap<>();
		try {
			result = primxPolicyService.deletePolicy(policyCode, productName);
		} catch (Exception e) {
			// 에러 발생시 예외 처리
			e.printStackTrace();
			result.put("status", "error");
			result.put("message", e.getMessage());
		}
		return result;
	}

	@ResponseBody
	@RequestMapping(value = "/product/applyDefaultPolicy.do", method = RequestMethod.POST)
	public Map<String, Object> applyDefaultPolicy(@RequestParam(value = "policyCode", required = true) String policyCode) throws Exception {
		Map<String, Object> result = new HashMap<>();
		try {
			result = primxPolicyService.applyDefaultPolicy(policyCode);
		} catch (Exception e) {
			e.printStackTrace();
			result.put("status", "error");
			result.put("message", e.getMessage());
		}
		return result;
	}

	@ResponseBody
	@RequestMapping(value = "/product/deleteSystem.do", method = RequestMethod.POST)
	public Map<String, Object> deleteSystem(
		@RequestParam(value = "agentIdList", required = true) List<String> agentIdList
	) throws Exception {
		Map<String, Object> result = new HashMap<>();
		try {
			result = primxPolicyService.deleteSystem(agentIdList);
		} catch (Exception e) {
			e.printStackTrace();
			result.put("status", "error");
			result.put("message", e.getMessage());
		}
		return result;
	}

	@ResponseBody
	@RequestMapping(value = "/product/addSystem.do", method = RequestMethod.POST)
	public Map<String, Object> addSystem(
		@RequestParam(value = "agentIdList", required = true) List<String> agentIdList, 
		@RequestParam(value = "policyCode", required = true) String policyCode) throws Exception {
		Map<String, Object> result = new HashMap<>();
		try {
			result = primxPolicyService.addSystem(agentIdList, policyCode);
		} catch (Exception e) {
			e.printStackTrace();
			result.put("status", "error");
			result.put("message", e.getMessage());
		}
		return result;
	}

	@ResponseBody
	@RequestMapping(value = "/product/changePolicy.do", method = RequestMethod.POST)
	public Map<String, Object> changePolicy(
		@RequestParam(value = "agentIdList", required = true) String agentIdList, 
		@RequestParam(value = "policyCode", required = true) String policyCode
	) throws Exception {
		Map<String, Object> result = new HashMap<>();
		try {
			result = primxPolicyService.changePolicy(agentIdList, policyCode);
		} catch (Exception e) {
			e.printStackTrace();
			result.put("status", "error");
			result.put("message", e.getMessage());
		}
		return result;
	}

	@ResponseBody
	@RequestMapping(value = "/distribution/list.do", method = RequestMethod.POST)
	public Map<String, Object> loadDistributionPolicyList(DistributionPolicySearchDTO searchDTO) throws Exception {

		System.out.println("searchDTO.getSearchDeploymentStatus(): " + searchDTO.getSearchDeploymentStatus());
		System.out.println("searchDTO.getSearchDateType(): " + searchDTO.getSearchDateType());	
		System.out.println("searchDTO.getSearchStartDate(): " + searchDTO.getSearchStartDate());
		System.out.println("searchDTO.getSearchEndDate(): " + searchDTO.getSearchEndDate());
		System.out.println("searchDTO.getSearchDeptId(): " + searchDTO.getSearchDeptId());
		System.out.println("searchDTO.getSearchDeptName(): " + searchDTO.getSearchDeptName());
		System.out.println("searchDTO.getSearchProductName(): " + searchDTO.getSearchProductName());
		System.out.println("searchDTO.getSearchProcessorArchitecture(): " + searchDTO.getSearchProcessorArchitecture());
		
		return primxPolicyService.loadDistributionPolicyList(searchDTO);
	}

	@ResponseBody
	@RequestMapping(value = "/distribution/save.do", method = RequestMethod.POST)
	public Map<String, Object> saveDistributionPolicy(@RequestParam(value = "deptId", required = true) String deptId, 
	@RequestParam(value = "beginDate", required = true) String beginDate, 
	@RequestParam(value = "endDate", required = true) String endDate,
	@RequestParam(value = "releaseYn", required = true) String releaseYn,
	@RequestParam(value = "includeSubDept", required = true) String includeSubDept) throws Exception {
		return primxPolicyService.saveDistributionPolicy(deptId, beginDate, endDate, releaseYn, includeSubDept);
	}
}
