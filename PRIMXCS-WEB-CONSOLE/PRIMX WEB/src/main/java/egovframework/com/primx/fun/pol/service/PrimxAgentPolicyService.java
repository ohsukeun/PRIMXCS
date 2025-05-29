package egovframework.com.primx.fun.pol.service;

import java.util.List;
import java.util.Map;

import egovframework.com.primx.fun.sys.service.FileZoneVO;

public interface PrimxAgentPolicyService {

	public Map<String, Object> selectAgentPolicyList(AgentPolicySearchDTO searchDTO) throws Exception;

	public Map<String, Object> selectAgentPolicyListByPolicyName(String policyName) throws Exception;

	public Map<String, Object> insertAgentPolicy(AgentPolicyMasterVO agentPolicyMasterVO, List<FileZoneVO> fileZoneTableData) throws Exception;

	public Map<String, Object> deleteAgentPolicy(int policyId, String agentPolicyCode) throws Exception;

	public Map<String, Object> applyAgentDefaultPolicy(String agentPolicyCode) throws Exception;

	public Map<String, Object> deleteAgentSystem(List<String> agentIdList) throws Exception;

	public Map<String, Object> addAgentSystem(List<String> agentIdList, String agentPolicyCode) throws Exception;

	public Map<String, Object> updateAgentPolicy(AgentPolicyMasterVO agentPolicyMasterVO, List<FileZoneVO> fileZoneTableData) throws Exception;

	public Map<String, Object> changeAgentSystem(String agentIdList, String agentPolicyCode) throws Exception;
}
