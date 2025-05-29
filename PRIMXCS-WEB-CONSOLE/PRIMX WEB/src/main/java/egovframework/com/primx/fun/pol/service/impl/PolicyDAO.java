package egovframework.com.primx.fun.pol.service.impl;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.stereotype.Repository;

import egovframework.com.cmm.service.impl.EgovComAbstractDAO;
import egovframework.com.primx.fun.pol.service.AgentPolicyMasterVO;
import egovframework.com.primx.fun.pol.service.AgentPolicySearchDTO;
import egovframework.com.primx.fun.pol.service.AgentPolicySystemMatchVO;
import egovframework.com.primx.fun.pol.service.DistributionPolicySearchDTO;
import egovframework.com.primx.fun.pol.service.PolicyDetailInfoVO;
import egovframework.com.primx.fun.pol.service.PolicySystemMapVO;
import egovframework.com.primx.fun.pol.service.PolicyVO;
import egovframework.com.primx.fun.pol.service.ProductPolicySearchDTO;
import egovframework.com.primx.fun.pol.service.ReleasePolicyVO;

@Repository("policyDAO")
public class PolicyDAO extends EgovComAbstractDAO {
	
	public AgentPolicyMasterVO selectDefaultPolicyYn(String defaultPolicy) {

		Map<String, String> paramMap = new HashMap<>();
		paramMap.put("defaultPolicy", defaultPolicy);

		return selectOne("policyDAO.selectDefaultPolicyYn", paramMap);
	}

	public AgentPolicyMasterVO selectAgentPolicyCode(String agentPolicyCode) {

		Map<String, String> paramMap = new HashMap<>();
		paramMap.put("agentPolicyCode", agentPolicyCode);

		return selectOne("policyDAO.selectAgentPolicyCode", paramMap);
	}

	public AgentPolicySystemMatchVO selectAgentPolicyCodeAndAgentId(String agentPolicyCode, String agentId) {

		Map<String, String> paramMap = new HashMap<>();
		paramMap.put("agentPolicyCode", agentPolicyCode);
		paramMap.put("agentId", agentId);

		return selectOne("policyDAO.selectAgentPolicyCodeAndAgentId", paramMap);
	}

	public List<AgentPolicySystemMatchVO> selectListAgentId(String agentId) {

		Map<String, String> paramMap = new HashMap<>();
		paramMap.put("agentId", agentId);

		return selectList("PolicyDAO.selectListAgentId", paramMap);
	}

	public AgentPolicySystemMatchVO selectOneByAgentId(String agentId) {

		Map<String, String> paramMap = new HashMap<>();
		paramMap.put("agentId", agentId);

		return selectOne("PolicyDAO.selectOneByAgentId", paramMap);
	}
	
	public PolicyVO selectPolicyById(Long id) {

		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("id", id);

		return selectOne("policyDAO.selectPolicyById", paramMap);
	}

	public PolicyVO selectPolicyCode(String policyCode) {

		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("policyCode", policyCode);

		return selectOne("policyDAO.selectPolicyCode", paramMap);
	}

	public List<PolicyVO> selectIsDefault(String isDefault) {

		Map<String, String> paramMap = new HashMap<>();
		paramMap.put("isDefault", isDefault);

		return selectList("PolicyDAO.selectIsDefault", paramMap);
	}

	public PolicyVO selectIsDefaultAndProductId(String isDefault, Long productId) {

		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("isDefault", isDefault);
		paramMap.put("productId", productId);

		return selectOne("policyDAO.selectIsDefaultAndProductId", paramMap);
	}

//	  @Query(value = "select policy_code from T_POLICY where product_id = :productId and is_deleted = 'N'", nativeQuery = true)
	public List<String> selectListPolicyById(Long id) {

		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("id", id);

		return selectList("PolicyDAO.selectListPolicyById", paramMap);
	}

	public List<PolicySystemMapVO> selectPolicyCodeList(String policyCode) {

		Map<String, String> paramMap = new HashMap<>();
		paramMap.put("policyCode", policyCode);

		return selectList("PolicyDAO.selectPolicyCodeList", paramMap);
	}

	public List<PolicySystemMapVO> selectAgentId2(String agentId) {

		Map<String, String> paramMap = new HashMap<>();
		paramMap.put("agentId", agentId);

		return selectList("PolicyDAO.selectAgentId2", paramMap);
	}

	public PolicySystemMapVO selectPolicyCodeAndAgentId(String policyCode, String agentId) {

		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("policyCode", policyCode);
		paramMap.put("agentId", agentId);

		return selectOne("policyDAO.selectIsDefaultAndProductId", paramMap);
	}

	public int countByPolicyCodeIn(List<String> policyCodeList) {

		return (Integer)selectOne("policyDAO.countByPolicyCodeIn", policyCodeList);
	}
	
	public int insertAgentPolicy(AgentPolicyMasterVO agentPolicyMasterVO) {
		return (Integer)insert("policyDAO.insertAgentPolicy", agentPolicyMasterVO);
	}

	public int updateAgentPolicy(AgentPolicyMasterVO agentPolicyMasterVO) {
		return (Integer)update("policyDAO.updateAgentPolicy", agentPolicyMasterVO);
	}
	
	public int deleteAgentPolicy(int policyId) {

		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("policyId", policyId);

		return (Integer)delete("policyDAO.deleteAgentPolicy", paramMap);
	}
	
	public List<AgentPolicySystemMatchVO> selectPolicySystemMapList(String agentPolicyCode) {

		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("agentPolicyCode", agentPolicyCode);

		return selectList("policyDAO.selectPolicySystemMapList", paramMap);
	}
	
	public int updateAgentPolicySystemMatch(AgentPolicySystemMatchVO agentPolicySystemMatchVO) {
		return (Integer)update("policyDAO.updateAgentPolicySystemMatch", agentPolicySystemMatchVO);
	}
	
    public List<AgentPolicyMasterVO> selectAgentPolicyList(AgentPolicySearchDTO searchDTO) throws Exception {
        return selectList("policyDAO.selectAgentPolicyList", searchDTO);
    }

	public int selectAgentPolicyListCnt(AgentPolicySearchDTO searchDTO) {
		return selectOne("policyDAO.selectAgentPolicyListCnt", searchDTO);
	}

	public List<AgentPolicyMasterVO> selectAgentPolicyListByPolicyName(String policyName) throws Exception {
		Map<String, String> paramMap = new HashMap<>();
		paramMap.put("policyName", policyName);
		return selectList("policyDAO.selectAgentPolicyListByPolicyName", paramMap);
	}

	public List<PolicySystemMapVO> selectPolicySystemMapListByAgentId(String agentId) {

		Map<String, String> paramMap = new HashMap<>();
		paramMap.put("agentId", agentId);

		return selectList("policyDAO.selectPolicySystemMapListByAgentId", paramMap);
	}

	public PolicyVO selectPolicyInfo(String policyCode) {

		Map<String, String> paramMap = new HashMap<>();
		paramMap.put("policyCode", policyCode);

		return selectOne("policyDAO.selectPolicyInfo", paramMap);
	}

	public int deletePolicySystemMap(int id) {

		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("id", id);

		return (Integer)delete("policyDAO.deletePolicySystemMap", paramMap);
	}
	// selectAgentPolicySystemMatchList
	public List<AgentPolicySystemMatchVO> selectAgentPolicySystemMatchList(String agentId) {

		Map<String, String> paramMap = new HashMap<>();
		paramMap.put("agentId", agentId);

		return selectList("policyDAO.selectAgentPolicySystemMatchList", paramMap);	
	}

	public int deleteAgentPolicySystemMatch(long seqNo) {

		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("seqNo", seqNo);

		return (Integer)delete("policyDAO.deleteAgentPolicySystemMatch", paramMap);
	}

	public List<PolicyVO> selectPolicyList(ProductPolicySearchDTO searchDTO) {

		return selectList("policyDAO.selectPolicyList", searchDTO);
	}

	public int selectPolicyListCnt(ProductPolicySearchDTO searchDTO) {
		return selectOne("policyDAO.selectPolicyListCnt", searchDTO);
	}

	public List<PolicyDetailInfoVO> selectDefaultPolicyList() {
		return selectList("policyDAO.selectDefaultPolicyList");
	}

	public List<PolicyDetailInfoVO> selectMandatoryPolicyList() {
		return selectList("policyDAO.selectMandatoryPolicyList");
	}

	public int updateDefaultPolicy(PolicyDetailInfoVO policyDetailInfoVO) {
		return update("policyDAO.updateDefaultPolicy", policyDetailInfoVO);
	}

	public PolicyVO selectPolicyByCode(String policyCode) {
		Map<String, String> paramMap = new HashMap<>();
		paramMap.put("policyCode", policyCode);
		return selectOne("policyDAO.selectPolicyByCode", paramMap);
	}

	public List<PolicyDetailInfoVO> selectPolicyDetailList(String policyCode) {
		Map<String, String> paramMap = new HashMap<>();
		paramMap.put("policyCode", policyCode);
		return selectList("policyDAO.selectPolicyDetailList", paramMap);
	}

	public int insertPolicy(PolicyVO policy) {
		return insert("policyDAO.insertPolicy", policy);
	}

	public int insertPolicyDetail(PolicyDetailInfoVO policyDetail) {
		return insert("policyDAO.insertPolicyDetail", policyDetail);
	}

	public PolicyVO selectDefaultPolicy() {
		return selectOne("policyDAO.selectDefaultPolicy");
	}

	public int deletePolicy(String policyCode) {
		Map<String, String> paramMap = new HashMap<>();
		paramMap.put("policyCode", policyCode);
		return delete("policyDAO.deletePolicy", paramMap);
	}

	public int updatePolicySystemMap(String policyCode, String defaultPolicyCode) {
		Map<String, String> paramMap = new HashMap<>();
		paramMap.put("policyCode", policyCode);
		paramMap.put("defaultPolicyCode", defaultPolicyCode);
		return update("policyDAO.updatePolicySystemMap", paramMap);
	}

	public int deletePolicyDetail(String policyCode) {
		Map<String, String> paramMap = new HashMap<>();
		paramMap.put("policyCode", policyCode);
		return delete("policyDAO.deletePolicyDetail", paramMap);
	}

	public int updateDefaultPolicyFlag(String policyCode, String isDefault) {
		Map<String, String> paramMap = new HashMap<>();
		paramMap.put("policyCode", policyCode);
		paramMap.put("isDefault", isDefault);
		return update("policyDAO.updateDefaultPolicyFlag", paramMap);
	}

	public int updatePolicySystem(String agentId, String policyCode) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("agentId", agentId);
		paramMap.put("policyCode", policyCode);
		return update("policyDAO.updatePolicySystem", paramMap);
	}

	public int deleteSystem(String agentId) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("agentId", agentId);
		return delete("policyDAO.deleteSystem", paramMap);
	}

	public int addSystem(String agentId, String policyCode) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("agentId", agentId);
		paramMap.put("policyCode", policyCode);
		return insert("policyDAO.addSystem", paramMap);
	}

	public String selectLastPolicyCodeByDate(String prefix) {
		Map<String, String> paramMap = new HashMap<>();
		paramMap.put("prefix", prefix);
		return selectOne("policyDAO.selectLastPolicyCodeByDate", paramMap);
	}

	public int insertFileZone(int zoneId, int policyId) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("zoneId", zoneId);
		paramMap.put("policyId", policyId);
		return insert("policyDAO.insertFileZone", paramMap);
	}

	public int updateAgentPolicySystemMatch(String agentPolicyCode, String defaultPolicyCode) {
		Map<String, String> paramMap = new HashMap<>();
		paramMap.put("agentPolicyCode", agentPolicyCode);
		paramMap.put("defaultPolicyCode", defaultPolicyCode);
		return update("policyDAO.updateAgentPolicySystemMatch", paramMap);
	}

	public AgentPolicyMasterVO selectAgentDefaultPolicy() {
		return selectOne("policyDAO.selectAgentDefaultPolicy");
	}

	public int updateAgentDefaultPolicyFlag(String agentPolicyCode, String isDefault) {
		Map<String, String> paramMap = new HashMap<>();
		paramMap.put("agentPolicyCode", agentPolicyCode);
		paramMap.put("isDefault", isDefault);
		return update("policyDAO.updateAgentDefaultPolicyFlag", paramMap);
	}

	public int deleteAgentSystem(String agentId) {
		Map<String, String> paramMap = new HashMap<>();
		paramMap.put("agentId", agentId);
		return delete("policyDAO.deleteAgentSystem", paramMap);
	}

	public int updateAgentPolicySystem(String agentId, String agentPolicyCode) {
		Map<String, String> paramMap = new HashMap<>();
		paramMap.put("agentId", agentId);
		paramMap.put("agentPolicyCode", agentPolicyCode);
		return update("policyDAO.updateAgentPolicySystem", paramMap);
	}

	public int addAgentSystem(String agentId, String agentPolicyCode) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("agentId", agentId);
		paramMap.put("agentPolicyCode", agentPolicyCode);
		return insert("policyDAO.addAgentSystem", paramMap);
	}

	public int updatePolicy(PolicyVO policy) {
		return update("policyDAO.updatePolicy", policy);
	}

	public int updatePolicyDetail(PolicyDetailInfoVO policyDetail) {
		return update("policyDAO.updatePolicyDetail", policyDetail);
	}

	public PolicyDetailInfoVO selectPolicyDetailByCodeAndId(String policyCode) {
		Map<String, String> paramMap = new HashMap<>();
		paramMap.put("policyCode", policyCode);
		return selectOne("policyDAO.selectPolicyDetailByCodeAndId", paramMap);
	}

	public ReleasePolicyVO selectDistributionPolicyByDeptId(String deptId) {
		Map<String, String> paramMap = new HashMap<>();
		paramMap.put("deptId", deptId);
		return selectOne("policyDAO.selectDistributionPolicyByDeptId", paramMap);
	}

	public List<ReleasePolicyVO> selectDistributionPolicyList(DistributionPolicySearchDTO searchDTO, List<String> deptIds) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("searchDTO", searchDTO);
		paramMap.put("deptIds", deptIds);
		System.out.println("paramMap: " + paramMap);
		return selectList("policyDAO.selectDistributionPolicyList", paramMap);
	}

	public int selectDistributionPolicyListCnt(DistributionPolicySearchDTO searchDTO, List<String> deptIds) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("searchDTO", searchDTO);
		paramMap.put("deptIds", deptIds);
		return selectOne("policyDAO.selectDistributionPolicyListCnt", paramMap);
	}

	public int insertDistributionPolicy(String deptId, String beginDate, String endDate, String deptPath, String releaseYn) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("deptId", deptId);
		paramMap.put("beginDate", beginDate);
		paramMap.put("endDate", endDate);
		paramMap.put("deptPath", deptPath);
		paramMap.put("releaseYn", releaseYn);
		return insert("policyDAO.insertDistributionPolicy", paramMap);
	}

	public int updateDistributionPolicy(int regId, String beginDate, String endDate, String deptPath, String releaseYn) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("regId", regId);
		paramMap.put("beginDate", beginDate);
		paramMap.put("endDate", endDate);
		paramMap.put("deptPath", deptPath);
		paramMap.put("releaseYn", releaseYn);
		return update("policyDAO.updateDistributionPolicy", paramMap);
	}

	public int updateDeptPathDistributionPolicy(int regId, String deptPath) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("regId", regId);
		paramMap.put("deptPath", deptPath);
		return update("policyDAO.updateDeptPathDistributionPolicy", paramMap);
	}

	public int deleteDistributionPolicy(int regId) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("regId", regId);
		return delete("policyDAO.deleteDistributionPolicy", paramMap);
	}

	public List<PolicyVO> selectIndividualPolicyList() {
		return selectList("policyDAO.selectIndividualPolicyList");
	}
	
}