package egovframework.com.primx.fun.pol.service.impl;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.annotation.Resource;

import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import egovframework.com.cmm.LoginVO;
import egovframework.com.cmm.util.EgovUserDetailsHelper;
import egovframework.com.primx.fun.pol.service.AgentPolicyMasterVO;
import egovframework.com.primx.fun.pol.service.AgentPolicySearchDTO;
import egovframework.com.primx.fun.pol.service.PrimxAgentPolicyService;
import egovframework.com.primx.fun.sys.service.FileZoneVO;
import egovframework.com.primx.fun.sys.service.impl.SystemDAO;

@Service("primxAgentPolicyService")
public class PrimxAgentPolicyServiceImpl implements PrimxAgentPolicyService {

	@Resource(name = "policyDAO")
	private PolicyDAO policyDAO;

	@Resource(name = "systemDAO")
	private SystemDAO systemDAO;

	@Override
	public Map<String, Object> selectAgentPolicyList(AgentPolicySearchDTO searchDTO) throws Exception {
		System.out.println("searchDTO: " + searchDTO.toString());
		List<AgentPolicyMasterVO> result = policyDAO.selectAgentPolicyList(searchDTO);
		for(AgentPolicyMasterVO agentPolicyMasterVO : result) {
			agentPolicyMasterVO.convertUTCtoUserTime();
		}

		int totalCnt = policyDAO.selectAgentPolicyListCnt(searchDTO);
		
		Map<String, Object> map = new HashMap<>();
		map.put("resultList", result);
		map.put("recordsTotal", totalCnt);
		map.put("recordsFiltered", totalCnt);
		map.put("status", "success");
		map.put("message", "success");
		return map;
	}

	@Override
	public Map<String, Object> selectAgentPolicyListByPolicyName(String policyName) throws Exception {
		List<AgentPolicyMasterVO> result = policyDAO.selectAgentPolicyListByPolicyName(policyName);
		Map<String, Object> map = new HashMap<>();
		map.put("agentPolicyList", result);
		return map;
	}

	@Override
	@Transactional
	public Map<String, Object> insertAgentPolicy(AgentPolicyMasterVO agentPolicyMasterVO, List<FileZoneVO> fileZoneTableData) throws Exception {
		Map<String, Object> map = new HashMap<>();
		
		LoginVO loginVO = (LoginVO) EgovUserDetailsHelper.getAuthenticatedUser();
		agentPolicyMasterVO.setInsertedBy(loginVO.getId());

		// 정책 코드 만들기
		String agentPolicyCode = generateFileName("AGN-");
		agentPolicyMasterVO.setAgentPolicyCode(agentPolicyCode);

		// 정책 등록
		policyDAO.insertAgentPolicy(agentPolicyMasterVO);

		// T_AGENT_POLICY_MASTER 테이블에서 정책 코드 조회
		AgentPolicyMasterVO insertedPolicy = policyDAO.selectAgentPolicyCode(agentPolicyCode);

		System.out.println("insertedPolicy: " + insertedPolicy.toString());

		// T_ZONE_MAP에 인서트 fileZoneTableData 사이즈만큼 반복
		for(FileZoneVO fileZoneVO : fileZoneTableData) {
			policyDAO.insertFileZone(fileZoneVO.getZoneId(), insertedPolicy.getPolicyId());
		}

		map.put("status", "success");
		map.put("message", "정책 등록 성공");
		return map;
	}
	
	@Override
	@Transactional
	public Map<String, Object> updateAgentPolicy(AgentPolicyMasterVO agentPolicyMasterVO, List<FileZoneVO> fileZoneTableData) throws Exception {
		Map<String, Object> map = new HashMap<>();
		
		// 정책 수정
		policyDAO.updateAgentPolicy(agentPolicyMasterVO);

		// T_AGENT_POLICY_MASTER 테이블에서 정책 코드 조회
		AgentPolicyMasterVO insertedPolicy = policyDAO.selectAgentPolicyCode(agentPolicyMasterVO.getAgentPolicyCode());

		// T_ZONE_MAP에서 기존 agentPolicyCode로 연결되어있는 데이터 삭제
		systemDAO.deleteFileZoneByAgentPolicyId(insertedPolicy.getPolicyId());

		// T_ZONE_MAP에 인서트 fileZoneTableData 사이즈만큼 반복
		for(FileZoneVO fileZoneVO : fileZoneTableData) {
			policyDAO.insertFileZone(fileZoneVO.getZoneId(), insertedPolicy.getPolicyId());
		}

		map.put("status", "success");
		map.put("message", "정책 등록 성공");
		return map;
	}

	private String generateFileName(String prefix) {
		String dateStr = new SimpleDateFormat("yyyyMMdd").format(new Date());
		String baseFileName = prefix + dateStr;
		
		// 오늘 날짜의 마지막 정책 코드 조회
		String lastPolicyCode = policyDAO.selectLastPolicyCodeByDate(baseFileName);
		
		if (lastPolicyCode == null) {
			return baseFileName + "00001";
		} else {
			// 마지막 5자리 추출 후 1 증가
			int sequence = Integer.parseInt(lastPolicyCode.substring(lastPolicyCode.length() - 5)) + 1;
			return String.format("%s%05d", baseFileName, sequence);
		}
	}

	@Override
	@Transactional
	public Map<String, Object> deleteAgentPolicy(int policyId, String agentPolicyCode) throws Exception {
		Map<String, Object> map = new HashMap<>();
		System.out.println("policyId: " + policyId);
		System.out.println("agentPolicyCode: " + agentPolicyCode);

		// agentPolicy 기본 정책 가져오기
		AgentPolicyMasterVO defaultPolicy = policyDAO.selectAgentDefaultPolicy();

		// 기존 agentPolicyCode로 연결되어있는 T_AGENT_POLICY_SYSTEM_MATCH의 C_AGENT_POLICY_CODE를 defaultPolicy.agentPolicyCode로 변경
		policyDAO.updateAgentPolicySystemMatch(agentPolicyCode, defaultPolicy.getAgentPolicyCode());
		
		// T_ZONE_MAP에서 기존 agentPolicyCode로 연결되어있는 데이터 삭제
		systemDAO.deleteFileZoneByAgentPolicyId(policyId);

		// agentPolicy 삭제
		policyDAO.deleteAgentPolicy(policyId);

		map.put("status", "success");
		map.put("message", "success");
		return map;
	}

	@Override
	@Transactional
	public Map<String, Object> applyAgentDefaultPolicy(String agentPolicyCode) throws Exception {
		Map<String, Object> map = new HashMap<>();

		// 기본 정책 가져오기
		AgentPolicyMasterVO defaultPolicy = policyDAO.selectAgentDefaultPolicy();
		if(defaultPolicy != null) {
			String defaultPolicyCode = defaultPolicy.getAgentPolicyCode();
			policyDAO.updateAgentDefaultPolicyFlag(defaultPolicyCode, "N");

		}
		policyDAO.updateAgentDefaultPolicyFlag(agentPolicyCode, "Y");

		map.put("status", "success");
		map.put("message", "success");
		return map;
	}	

	@Override
	@Transactional
	public Map<String, Object> deleteAgentSystem(List<String> agentIdList) throws Exception {
		Map<String, Object> map = new HashMap<>();

		// agentPolicy 기본 정책 가져오기
		AgentPolicyMasterVO defaultPolicy = policyDAO.selectAgentDefaultPolicy();

		for(String agentId : agentIdList) {
			policyDAO.updateAgentPolicySystem(agentId, defaultPolicy.getAgentPolicyCode());
		}
		map.put("status", "success");
		map.put("message", "success");
		return map;
	}

	@Override
	@Transactional
	public Map<String, Object> addAgentSystem(List<String> agentIdList, String agentPolicyCode) throws Exception {
		Map<String, Object> map = new HashMap<>();

		for(String agentId : agentIdList) {
			// 기존 시스템 삭제
			policyDAO.deleteAgentSystem(agentId);
			// 시스템 추가
			policyDAO.addAgentSystem(agentId, agentPolicyCode);
		}
		
		map.put("status", "success");
		map.put("message", "success");
		return map;
	}

	@Override
	@Transactional
	public Map<String, Object> changeAgentSystem(String agentIdList, String agentPolicyCode) throws Exception {
		Map<String, Object> map = new HashMap<>();
		String[] agentIdArray = agentIdList.split(",");
		for(String agentId : agentIdArray) {
			policyDAO.deleteAgentSystem(agentId);
			policyDAO.addAgentSystem(agentId, agentPolicyCode);
		}
		map.put("status", "success");
		map.put("message", "success");
		return map;
	}
}
