package egovframework.com.primx.fun.sys.service.impl;

import java.sql.SQLException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.stereotype.Repository;

import egovframework.com.cmm.service.impl.EgovComAbstractDAO;
import egovframework.com.primx.fun.env.service.ServerConfigVO;
import egovframework.com.primx.fun.file.service.FileVO;
import egovframework.com.primx.fun.lic.service.LicenseDetailVO;
import egovframework.com.primx.fun.pol.service.NicInfoVO;
import egovframework.com.primx.fun.sys.service.ClientProgramDTO;
import egovframework.com.primx.fun.sys.service.DiskInfoVO;
import egovframework.com.primx.fun.sys.service.ExServerInfoVO;
import egovframework.com.primx.fun.sys.service.PasswordSettingVO;
import egovframework.com.primx.fun.sys.service.PrimxSetupInfoDTO;
import egovframework.com.primx.fun.sys.service.ProductsVO;
import egovframework.com.primx.fun.sys.service.RoleDTO;
import egovframework.com.primx.fun.sys.service.ServerProgramDTO;
import egovframework.com.primx.fun.sys.service.SystemMasterVO;
import egovframework.com.primx.fun.sys.service.SystemStatusResultDTO;
import egovframework.com.primx.fun.sys.service.SystemStatusSearchVO;
import egovframework.com.primx.fun.sys.service.UnusedSystemDTO;
import egovframework.com.primx.fun.sys.service.UpdateInfoVO;
import egovframework.com.primx.fun.sys.service.UserSystemMatchVO;
import egovframework.com.primx.fun.sys.service.FileMovePolicySettingVO;
import egovframework.com.primx.fun.sys.service.FileZoneVO;
import egovframework.com.primx.fun.sys.service.PolicyScriptDTO;

@Repository("systemDAO")
public class SystemDAO extends EgovComAbstractDAO {

//	@Query("select p from  Products p where p.productName like %:productName%")
	public ProductsVO selectProductNameLikeIgnoreCase(String productName) {

		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("productName", productName);

		return selectOne("systemDAO.selectProductNameLikeIgnoreCase", paramMap);
	}

	public int updateSystem(String adminId, String clientIp, Long seqNo) {

		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("adminId", adminId);
		paramMap.put("clientIp", clientIp);
		paramMap.put("seqNo", seqNo);

		return update("systemDAO.updateSystem", paramMap);
	}


	public List<UpdateInfoVO> selectByProductType(Long productType) throws Exception {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("productType", productType);

		return selectList("systemDAO.selectByProductType", paramMap);
	}
	
	public String selectLastDeptCode(String prefix) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("prefix", prefix);
		
		return selectOne("systemDAO.selectLastDeptCode", paramMap);
	}

	/**
	 * 시스템 현황 갯수 조회
	 * @param systemMasterVO
	 * @return
	 * @throws SQLException
	 */
	public int selectSystemStatusInfoCnt(SystemStatusSearchVO systemStatusSearchVO) throws SQLException {
		return selectOne("systemDAO.selectSystemStatusInfoCnt", systemStatusSearchVO);
	}

	/**
	 * 시스템 현황 목록 조회
	 * @param systemMasterVO
	 * @return
	 * @throws SQLException
	 */
	public List<SystemStatusResultDTO> selectListSystemStatusInfo(SystemStatusSearchVO systemStatusSearchVO) throws SQLException {
		return selectList("systemDAO.selectListSystemStatusInfo", systemStatusSearchVO);
	}

	/**
	 * 시스템 현황 정보 조회
	 * @param systemId
	 * @return
	 * @throws SQLException
	 */
	public SystemStatusResultDTO selectSystemStatusInfo(int systemId) throws SQLException {
		return selectOne("systemDAO.selectSystemStatusInfo", systemId);
	}

	public SystemStatusResultDTO selectSystemStatusInfoById(String emplyrId) throws SQLException {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("emplyrId", emplyrId);
		return selectOne("systemDAO.selectSystemStatusInfoById", paramMap);
	}



	/**
	 * Disk 정보 목록 조회
	 * @param systemStatusResultDTO
	 * @return
	 * @throws SQLException
	 */

	public List<DiskInfoVO> selectListDiskInfo(SystemStatusResultDTO systemStatusResultDTO) throws SQLException {
		return selectList("systemDAO.selectListDiskInfo", systemStatusResultDTO);
	}

	/**
	 * NIC 정보 목록 조회
	 * @param systemStatusResultDTO
	 * @return
	 */
	public List<NicInfoVO> selectListNicInfo(SystemStatusResultDTO systemStatusResultDTO) {
		return selectList("systemDAO.selectListNicInfo", systemStatusResultDTO);
	}

	/**
	 * 설치 제품 목록 조회
	 * @param systemStatusResultDTO
	 * @return
	 */
	public List<PrimxSetupInfoDTO> selectListPrimxSetupInfo(SystemStatusResultDTO systemStatusResultDTO) {
		return selectList("systemDAO.selectListPrimxSetupInfo", systemStatusResultDTO);
	}

	public List<ClientProgramDTO> selectClientProgramList() {
		// SQL 쿼리를 통해 클라이언트 프로그램 목록을 조회
		return selectList("systemDAO.selectClientProgramList");
	}

	public List<String> selectProductList() {
		return selectList("systemDAO.selectProductList");
	}

	public List<PolicyScriptDTO> selectPolicyScriptList(String productName) {
		System.out.println("##########################################productName3: "+productName);
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("productName", productName);
		
		return selectList("systemDAO.selectPolicyScriptList", paramMap);
	}

	public List<RoleDTO> selectRoleList() {
		return selectList("systemDAO.selectRoleList");
	}

	public int saveClientProgram(List<FileVO> selectedFiles) {
		return update("systemDAO.saveClientProgram", selectedFiles);
	}

	public int updateClientProgram(UpdateInfoVO dto) {
		return update("systemDAO.updateClientProgram", dto);
	}

	public int updatePolicyScript(int rowId, int fileId) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("id", rowId);
		paramMap.put("fileId", fileId);
		return update("systemDAO.updatePolicyScript", paramMap);
	}

	public int insertUpdateInfo(UpdateInfoVO vo) {
		return update("systemDAO.insertUpdateInfo", vo);
	}

	public List<ExServerInfoVO> selectFileDownloadServerList() {
		return selectList("systemDAO.selectFileDownloadServerList");
	}

	public int deleteFileDownloadServer(int seqNo) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("seqNo", seqNo);
		return delete("systemDAO.deleteFileDownloadServer", paramMap);
	}

	public int insertFileDownloadServer(String serverName, String serverUrl) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("serverName", serverName);
		paramMap.put("serverUrl", serverUrl);
		return insert("systemDAO.insertFileDownloadServer", paramMap);
	}

	public List<ServerProgramDTO> selectServerProgramList() {
		return selectList("systemDAO.selectServerProgramList");
	}

	public int updateFileDownloadServer(String serverName, String serverUrl) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("serverName", serverName);
		paramMap.put("serverUrl", serverUrl);
		return update("systemDAO.updateFileDownloadServer", paramMap);
	}

	public List<LicenseDetailVO> selectLicenseDetailList() {
		return selectList("systemDAO.selectLicenseDetailList");
	}

	public PasswordSettingVO selectPasswordSettingInfo(String userRole) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("userRole", userRole);
		return selectOne("systemDAO.selectPasswordSettingInfo", paramMap);
	}

	public int insertPasswordSetting(PasswordSettingVO passwordSettingVO) {
		return insert("systemDAO.insertPasswordSetting", passwordSettingVO);
	}

	public int updatePasswordSetting(PasswordSettingVO passwordSettingVO) {
		return update("systemDAO.updatePasswordSetting", passwordSettingVO);
	}

	public FileMovePolicySettingVO selectFileMovePolicySettingInfo(String fileType) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("fileType", fileType);
		return selectOne("systemDAO.selectFileMovePolicySettingInfo", paramMap);
	}

	public int insertFileMovePolicySetting(FileMovePolicySettingVO fileMovePolicySettingVO) {
		return insert("systemDAO.insertFileMovePolicySetting", fileMovePolicySettingVO);
	}

	public int updateFileMovePolicySetting(FileMovePolicySettingVO fileMovePolicySettingVO) {
		return update("systemDAO.updateFileMovePolicySetting", fileMovePolicySettingVO);
	}
	
	//updateSystemType
	public int updateSystemType(int seqNo, String agentId, String commonSystemYn) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("seqNo", seqNo);
		paramMap.put("agentId", agentId);
		paramMap.put("commonSystemYn", commonSystemYn);
		return update("systemDAO.updateSystemType", paramMap);
	}

	public UserSystemMatchVO selectUserSystemMatchInfo(int userSystemMatchSeqNo) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("userSystemMatchSeqNo", userSystemMatchSeqNo);
		return selectOne("systemDAO.selectUserSystemMatchInfo", paramMap);
	}

	public int deleteUserSystemMatch(long userSystemMatchSeqNo) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("userSystemMatchSeqNo", userSystemMatchSeqNo);
		return delete("systemDAO.deleteUserSystemMatch", paramMap);
	}

	public int insertUserSystemMatch(UserSystemMatchVO userSystemMatchVO) {
		return insert("systemDAO.insertUserSystemMatch", userSystemMatchVO);
	}

	//selectSystemMasterInfo
	public SystemMasterVO selectSystemMasterInfo(int seqNo) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("seqNo", seqNo);
		return selectOne("systemDAO.selectSystemMasterInfo", paramMap);
	}
	// selectUserSystemMatchList
	public List<UserSystemMatchVO> selectUserSystemMatchList(String agentId) {
		Map<String, String> paramMap = new HashMap<>();
		paramMap.put("agentId", agentId);
		return selectList("systemDAO.selectUserSystemMatchList", paramMap);
	}

	// updateSystemMaster
	public int updateSystemMaster(int seqNo, String adminId, String clientIp, String useYn, String uninstallYn) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("seqNo", seqNo);
		paramMap.put("adminId", adminId);
		paramMap.put("clientIp", clientIp);
		paramMap.put("useYn", useYn);
		paramMap.put("uninstallYn", uninstallYn);
		return update("systemDAO.updateSystemMaster", paramMap);
	}

	public List<FileZoneVO> selectFileZoneList() {
		return selectList("systemDAO.selectFileZoneList");
	}

	public List<FileZoneVO> selectFileZone(int policyId) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("policyId", policyId);
		return selectList("systemDAO.selectFileZone", paramMap);
	}

	public int insertFileZone(FileZoneVO fileZoneVO) {
		return insert("systemDAO.insertFileZone", fileZoneVO);
	}

	public int updateFileZone(FileZoneVO fileZoneVO) {
		return update("systemDAO.updateFileZone", fileZoneVO);
	}

	public int deleteFileZone(int zoneId) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("zoneId", zoneId);
		return delete("systemDAO.deleteFileZone", paramMap);
	}

	public ProductsVO selectProductInfo(String productName) {
		Map<String, String> paramMap = new HashMap<>();
		paramMap.put("productName", productName);
		return selectOne("systemDAO.selectProductInfo", paramMap);
	}

	public List<SystemStatusResultDTO> selectSystemStatusPolicyList(SystemStatusSearchVO systemStatusSearchVO) {
		return selectList("systemDAO.selectSystemStatusPolicyList", systemStatusSearchVO);
	}

	public int selectSystemStatusPolicyListCnt(SystemStatusSearchVO systemStatusSearchVO) {
		return selectOne("systemDAO.selectSystemStatusPolicyListCnt", systemStatusSearchVO);
	}

	public int deleteFileZoneByAgentPolicyId(int policyId) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("policyId", policyId);
		return delete("systemDAO.deleteFileZoneByAgentPolicyId", paramMap);
	}

	public ServerConfigVO selectServerConfig(String keyName) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("keyName", keyName);
		return selectOne("systemDAO.selectServerConfig", paramMap);
	}

	public int insertServerConfig(ServerConfigVO serverConfigVO) {
		return insert("systemDAO.insertServerConfig", serverConfigVO);
	}

	public int updateServerConfig(ServerConfigVO serverConfigVO) {
		return update("systemDAO.updateServerConfig", serverConfigVO);
	}

	public Map<String, Object> selectSystemStat() throws Exception {
		return selectOne("systemDAO.selectSystemStat");
	}
	

	public List<UnusedSystemDTO> selectUnusedSystemList(String term, String time, int start, int recordCountPerPage) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("term", term);
		paramMap.put("time", time);
		paramMap.put("start", start);
		paramMap.put("recordCountPerPage", recordCountPerPage);
		return selectList("systemDAO.selectUnusedSystemList", paramMap);
	}

	public int selectUnusedSystemListCnt(String term, String time) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("term", term);
		paramMap.put("time", time);
		return selectOne("systemDAO.selectUnusedSystemListCnt", paramMap);
	}

	public List<String> selectCommonSystemList() throws Exception {
		return selectList("systemDAO.selectCommonSystemList");
	}

	public List<String> selectRequestList() throws Exception {
		return selectList("systemDAO.selectRequestList");
	}

	public List<String> selectApprovalList() throws Exception {
		return selectList("systemDAO.selectApprovalList");
	}
	
	public List<String> selectLogFilterList(String userId) throws Exception {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("userId", userId);
		return selectList("systemDAO.selectLogFilterList", paramMap);
	}
	
	public int insertLogFilter(String userId, String logCode) throws Exception {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("userId", userId);
		paramMap.put("logCode", logCode);
		return insert("systemDAO.insertLogFilter", paramMap);
	}
	
	public int deleteLogFilter(String userId, String logCode) throws Exception {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("userId", userId);
		paramMap.put("logCode", logCode);
		return delete("systemDAO.deleteLogFilter", paramMap);
	}
}
