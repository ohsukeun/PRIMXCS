package egovframework.com.primx.fun.pol.service;

import java.util.List;
import java.util.Map;

public interface PrimxPolicyService {

	public Map<String, Object> loadProductPolicyList(ProductPolicySearchDTO searchDTO) throws Exception;
	public Map<String, Object> loadDefaultPolicyList() throws Exception;
	public Map<String, Object> loadMandatoryPolicyList() throws Exception;
	public Map<String, Object> saveDefaultPolicyList(List<PolicyDetailInfoVO> resultList) throws Exception;
	public Map<String, Object> getPolicyInfo(String policyCode) throws Exception;
	public Map<String, Object> saveProductPolicy(String policyName, String productName, List<PolicyDetailInfoVO> copyPolicyData) throws Exception;
	public Map<String, Object> updateProductPolicy(String policyCode, String policyName, String productName, List<PolicyDetailInfoVO> copyPolicyData) throws Exception;
	public Map<String, Object> deletePolicy(String policyCode, String productName) throws Exception;
	public Map<String, Object> applyDefaultPolicy(String policyCode) throws Exception;
	public Map<String, Object> deleteSystem(List<String> agentIdList) throws Exception;
	public Map<String, Object> addSystem(List<String> agentIdList, String policyCode) throws Exception;
	public Map<String, Object> changePolicy(String agentIdList, String policyCode) throws Exception;
	public Map<String, Object> loadDistributionPolicyList(DistributionPolicySearchDTO searchDTO) throws Exception;
	public Map<String, Object> saveDistributionPolicy(String deptId, String beginDate, String endDate, String releaseYn, String includeSubDept) throws Exception;
	public Map<String, Object> loadIndividualPolicyList() throws Exception;
}
