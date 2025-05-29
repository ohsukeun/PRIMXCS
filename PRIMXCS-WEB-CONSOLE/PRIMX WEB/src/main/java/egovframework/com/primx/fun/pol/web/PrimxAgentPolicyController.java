package egovframework.com.primx.fun.pol.web;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

import egovframework.com.primx.fun.pol.service.PrimxAgentPolicyService;
import egovframework.com.primx.fun.sys.service.FileZoneVO;
import egovframework.com.primx.fun.pol.service.AgentPolicyMasterVO;
import egovframework.com.primx.fun.pol.service.AgentPolicySearchDTO;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * 에이전트 정책 관리 관련 기능
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
//'ADMIN','SUPER'
@Controller
@RequestMapping(value = "/primx/fun/pol/agent")
public class PrimxAgentPolicyController {

	@Autowired
	private PrimxAgentPolicyService primxAgentPolicyService;

	
	@RequestMapping(value = "/policy/view.do")
	public String getList(ModelMap model) throws Exception {
		return "egovframework/com/primx/fun/pol/agentStatus";
	}

	@ResponseBody
	@RequestMapping(value = "/policy/list.do", method = RequestMethod.POST)
	public Map<String, Object> getPolicyList(AgentPolicySearchDTO searchDTO) throws Exception {
		searchDTO.convertUserTimeToUTC();
		return primxAgentPolicyService.selectAgentPolicyList(searchDTO);
	}

	@RequestMapping(value = "/policy/loadAgentPolicy.do")
	@ResponseBody
	public Map<String, Object> loadAgentPolicy(@RequestParam(value = "policyName", required = true) String policyName) throws Exception {
		return primxAgentPolicyService.selectAgentPolicyListByPolicyName(policyName);
	}

	@ResponseBody
	@RequestMapping(value = "/policy/save.do", method = RequestMethod.POST)
	public Map<String, Object> saveAgentPolicy(@RequestBody Map<String, Object> paramMap) throws Exception {
		// Transactional exception 처리
		Map<String, Object> result = new HashMap<>();
		try {
			@SuppressWarnings("unchecked")
			Map<String, Object> voMap = (Map<String, Object>) paramMap.get("agentPolicyMasterVO");
			
			AgentPolicyMasterVO agentPolicyMasterVO = AgentPolicyMasterVO.builder()
				.policyName((String) voMap.get("policyName"))
				.defaultPolicyYn((String) voMap.get("defaultPolicyYn"))
				.exportFileAuth((Integer) voMap.get("exportFileAuth"))
				.webManagerAuth((Integer) voMap.get("webManagerAuth"))
				.notEncryptionPathAuth((Integer) voMap.get("notEncryptionPathAuth"))
				.productMenuEnable((Integer) voMap.get("productMenuEnable"))
				.webManagerConsoleMenuEnable((Integer) voMap.get("webManagerConsoleMenuEnable"))
				.exportFileMenuEnable((Integer) voMap.get("exportFileMenuEnable"))
				.ztryMenuEnable((Integer) voMap.get("ztryMenuEnable"))
				.zopenArea((Integer) voMap.get("zopenArea"))
				.zpersonalAccess((Integer) voMap.get("zpersonalAccess"))
				.zoptionDisplay((Integer) voMap.get("zoptionDisplay"))
				.zsafetyZone((Integer) voMap.get("zsafetyZone"))
				.zareaClose((Integer) voMap.get("zareaClose"))
				.zenc((Integer) voMap.get("zenc"))
				.build();
			
			@SuppressWarnings("unchecked")
			List<Map<String, Object>> zoneList = (List<Map<String, Object>>) paramMap.get("fileZoneTableData");
			List<FileZoneVO> fileZoneTableData = new ArrayList<>();
			
			for(Map<String, Object> zone : zoneList) {
				FileZoneVO fileZoneVO = FileZoneVO.builder()
					.zoneId((Integer) zone.get("zoneId"))
					.zoneNm((String) zone.get("zoneNm"))
					.proto((String) zone.get("proto"))
					.build();
				fileZoneTableData.add(fileZoneVO);
			}
			
			result = primxAgentPolicyService.insertAgentPolicy(agentPolicyMasterVO, fileZoneTableData);
		} catch (Exception e) {
			e.printStackTrace();
			result.put("status", "error");
			result.put("message", e.getMessage());
		}
		return result;
	}

	@ResponseBody
	@RequestMapping(value = "/policy/update.do", method = RequestMethod.POST)
	public Map<String, Object> updateAgentPolicy(@RequestBody Map<String, Object> paramMap) throws Exception {
		// Transactional exception 처리
		Map<String, Object> result = new HashMap<>();
		try {
			@SuppressWarnings("unchecked")
			Map<String, Object> voMap = (Map<String, Object>) paramMap.get("agentPolicyMasterVO");
			
			AgentPolicyMasterVO agentPolicyMasterVO = AgentPolicyMasterVO.builder()
				.policyName((String) voMap.get("policyName"))
				.agentPolicyCode((String) voMap.get("agentPolicyCode"))
				.defaultPolicyYn((String) voMap.get("defaultPolicyYn"))
				.exportFileAuth((Integer) voMap.get("exportFileAuth"))
				.webManagerAuth((Integer) voMap.get("webManagerAuth"))
				.notEncryptionPathAuth((Integer) voMap.get("notEncryptionPathAuth"))
				.productMenuEnable((Integer) voMap.get("productMenuEnable"))
				.webManagerConsoleMenuEnable((Integer) voMap.get("webManagerConsoleMenuEnable"))
				.exportFileMenuEnable((Integer) voMap.get("exportFileMenuEnable"))
				.ztryMenuEnable((Integer) voMap.get("ztryMenuEnable"))
				.zopenArea((Integer) voMap.get("zopenArea"))
				.zpersonalAccess((Integer) voMap.get("zpersonalAccess"))
				.zoptionDisplay((Integer) voMap.get("zoptionDisplay"))
				.zsafetyZone((Integer) voMap.get("zsafetyZone"))
				.zareaClose((Integer) voMap.get("zareaClose"))
				.zenc((Integer) voMap.get("zenc"))
				.build();

			@SuppressWarnings("unchecked")
			List<Map<String, Object>> zoneList = (List<Map<String, Object>>) paramMap.get("fileZoneTableData");
			List<FileZoneVO> fileZoneTableData = new ArrayList<>();
			
			for(Map<String, Object> zone : zoneList) {
				FileZoneVO fileZoneVO = FileZoneVO.builder()
					.zoneId((Integer) zone.get("zoneId"))
					.zoneNm((String) zone.get("zoneNm"))
					.proto((String) zone.get("proto"))
					.build();
				fileZoneTableData.add(fileZoneVO);
			}
			
			result = primxAgentPolicyService.updateAgentPolicy(agentPolicyMasterVO, fileZoneTableData);
		} catch (Exception e) {
			e.printStackTrace();
			result.put("status", "error");
			result.put("message", e.getMessage());
		}
		return result;
	}

	@ResponseBody
	@RequestMapping(value = "/policy/delete.do", method = RequestMethod.POST)
	public Map<String, Object> deleteAgentPolicy(@RequestParam(value = "policyId", required = true) int policyId, 
	@RequestParam(value = "agentPolicyCode", required = true) String agentPolicyCode) throws Exception {
		Map<String, Object> result = new HashMap<>();
		try {
			result = primxAgentPolicyService.deleteAgentPolicy(policyId, agentPolicyCode);
		} catch (Exception e) {
			e.printStackTrace();
			result.put("status", "error");
			result.put("message", e.getMessage());
		}
		return result;
	}

	@ResponseBody
	@RequestMapping(value = "/policy/applyAgentDefaultPolicy.do", method = RequestMethod.POST)
	public Map<String, Object> applyAgentDefaultPolicy(@RequestParam(value = "agentPolicyCode", required = true) String agentPolicyCode) throws Exception {
		Map<String, Object> result = new HashMap<>();
		try {
			result = primxAgentPolicyService.applyAgentDefaultPolicy(agentPolicyCode);
		} catch (Exception e) {
			e.printStackTrace();
			result.put("status", "error");
			result.put("message", e.getMessage());
		}
		return result;
	}

	@ResponseBody
	@RequestMapping(value = "/policy/deleteSystem.do", method = RequestMethod.POST)
	public Map<String, Object> deleteAgentSystem(
		@RequestParam(value = "agentIdList", required = true) List<String> agentIdList
	) throws Exception {
		Map<String, Object> result = new HashMap<>();
		try {
			result = primxAgentPolicyService.deleteAgentSystem(agentIdList);
		} catch (Exception e) {
			e.printStackTrace();
			result.put("status", "error");
			result.put("message", e.getMessage());
		}
		return result;
	}

	@ResponseBody
	@RequestMapping(value = "/policy/addSystem.do", method = RequestMethod.POST)
	public Map<String, Object> addAgentSystem(
		@RequestParam(value = "agentIdList", required = true) List<String> agentIdList, 
		@RequestParam(value = "agentPolicyCode", required = true) String agentPolicyCode
	) throws Exception {
		Map<String, Object> result = new HashMap<>();
		try {
			result = primxAgentPolicyService.addAgentSystem(agentIdList, agentPolicyCode);
		} catch (Exception e) {
			e.printStackTrace();
			result.put("status", "error");
			result.put("message", e.getMessage());
		}
		return result;
	}

	@ResponseBody
	@RequestMapping(value = "/policy/changeSystem.do", method = RequestMethod.POST)
	public Map<String, Object> changeAgentSystem(@RequestParam(value = "agentIdList", required = true) String agentIdList, @RequestParam(value = "agentPolicyCode", required = true) String agentPolicyCode) throws Exception {
		Map<String, Object> result = new HashMap<>();
		try {
			result = primxAgentPolicyService.changeAgentSystem(agentIdList, agentPolicyCode);
		} catch (Exception e) {
			e.printStackTrace();
			result.put("status", "error");
			result.put("message", e.getMessage());
		}
		return result;
	}
}
