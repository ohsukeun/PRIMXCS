package egovframework.com.primx.fun.sys.service;

import java.sql.SQLException;
import java.util.List;
import java.util.Map;

import org.springframework.web.multipart.MultipartFile;
import org.springframework.web.multipart.MultipartHttpServletRequest;

import egovframework.com.primx.fun.dpt.service.DeptSearchVO;
import egovframework.com.primx.fun.env.service.ServerConfigVO;
import egovframework.com.primx.fun.file.service.FileVO;

public interface PrimxSystemService {
	public String generateCode(String codeKey) throws Exception;
	public Map<String, Object> selectListSystemStatusInfo(SystemStatusSearchVO systemStatusSearchVO) throws SQLException;
	public Map<String, Object> selectSystemStatusInfo(int systemId) throws SQLException;
	public Map<String, Object> selectMySystemStatusInfo() throws SQLException;
	public List<ClientProgramDTO> getClientProgramList();
	public List<String> getProductList();
	public List<PolicyScriptDTO> getPolicyScriptList(String productName);
	public Map<String, Object> selectRoleList();
	public Map<String, Object> saveClientProgram(Map<String,FileVO> uploadedFiles, MultipartHttpServletRequest mRequest);
	public Map<String, Object> updatePolicyScript(Map<String,FileVO> uploadedFiles, MultipartHttpServletRequest mRequest);
	public Map<String, Object> saveLicense(MultipartFile encryptedFile, MultipartFile keyFile) throws Exception;
	public Map<String, Object> getFileDownloadServerList();
	public Map<String, Object> deleteFileDownloadServer(int seqNo);
	public Map<String, Object> getServerProgramList();
	public Map<String, Object> insertFileDownloadServer(String serverName, String serverUrl);
	public Map<String, Object> loadLicenseInfo();
	public Map<String, Object> loadPasswordSettingInfo();
	public Map<String, Object> updatePasswordSetting(PasswordSettingVO passwordSettingVO);
	public Map<String, Object> loadFileMovePolicySetting(String fileType);
	public Map<String, Object> saveFileMovePolicySetting(FileMovePolicySettingVO fileMovePolicySettingVO);
	public Map<String, Object> updateSystemType(int seqNo, String agentId, int userSystemMatchSeqNo, String commonSystemYn);
	public Map<String, Object> deleteSystem(int seqNo);
	public Map<String, Object> getFileZoneList();
	public Map<String, Object> selectFileZone(int policyId);
	public Map<String, Object> saveFileZone(FileZoneVO fileZoneVO);
	public Map<String, Object> deleteFileZone(int zoneId);
	public Map<String, Object> getSystemStatusPolicyList(SystemStatusSearchVO systemStatusSearchVO);
	public Map<String, Object> loadServerConfig(String keyName);
	public Map<String, Object> saveServerConfig(List<ServerConfigVO> serverConfigVOList);
	public Map<String, Object> loadUnusedSystemList(DeptSearchVO searchVO);
	public Map<String, Object> getPolicyStatus();
	public Map<String, Object> getLicenseStatus();
	public Map<String, Object> getSystemStatus() throws Exception;
	public Map<String, Object> getRequestApprovalStatus() throws Exception;
	public Map<String, Object> getMoveFileStatus();
	public Map<String, Object> getProductLogStatus();
	public Map<String, Object> getLogFilter() throws Exception;
	public Map<String, Object> updateProductLogStatus(String code, boolean checked) throws Exception;
	public Map<String, Object> getProductLogList() throws Exception;
	
}
