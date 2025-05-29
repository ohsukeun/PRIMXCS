package egovframework.com.primx.fun.sys.web;

import java.util.List;
import java.util.Map;
import java.util.HashMap;

import javax.annotation.Resource;

import org.springframework.stereotype.Controller;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.multipart.MultipartFile;
import org.springframework.web.multipart.MultipartHttpServletRequest;

import egovframework.com.cmm.EgovMessageSource;
import egovframework.com.cmm.service.EgovProperties;
import egovframework.com.primx.fun.sys.service.ClientProgramDTO;
import egovframework.com.primx.fun.sys.service.ExServerInfoVO;
import egovframework.com.primx.fun.sys.service.PasswordSettingVO;
import egovframework.com.primx.fun.sys.service.PolicyScriptDTO;
import egovframework.com.primx.fun.sys.service.PrimxSystemService;
import egovframework.com.primx.fun.sys.service.SystemStatusSearchVO;
import egovframework.com.primx.fun.dpt.service.DeptSearchVO;
import egovframework.com.primx.fun.env.service.ServerConfigVO;
import egovframework.com.primx.fun.file.service.FileVO;
import egovframework.com.primx.fun.file.service.PrimxFileService;
import egovframework.com.primx.fun.pol.service.PrimxPolicyService;
import egovframework.com.primx.fun.rqt.service.PrimxRequestService;
import egovframework.com.primx.fun.rqt.service.NoticeSettingVO;
import egovframework.com.primx.fun.sys.service.FileMovePolicySettingVO;
import egovframework.com.primx.fun.sys.service.FileZoneVO;

/**
 * 시스템 관리 관련 기능
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
@RequestMapping(value = "/primx/fun/sys")
public class PrimxSystemController {

	@Resource(name = "egovMessageSource")
	private EgovMessageSource egovMessageSource;

	@Resource(name = "primxSystemService")
	private PrimxSystemService primxSystemService;

	@Resource(name = "primxFileService")
	private PrimxFileService primxFileService;

	@Resource(name = "primxRequestService")
	private PrimxRequestService primxRequestService;
	
	@Resource(name = "primxPolicyService")
	private PrimxPolicyService primxPolicyService;

	
	@RequestMapping(value = "/product/policySettings/view.do")
	public String getProductPolicySettingsView(ModelMap model) throws Exception {
	
		return "egovframework/com/primx/fun/sys/productPolicySettings";
	}
	
	@RequestMapping(value = "/dptUserSetting/view.do")
	public String getView(ModelMap model) throws Exception {

		return "egovframework/com/primx/fun/sys/departmentUserSettings";
	}

	@RequestMapping(value = "/update/view.do")
	public String updateView(ModelMap model) throws Exception {
		
		return "egovframework/com/primx/fun/sys/update";
	}
	
	@RequestMapping(value = "/createRequestSetting/view.do")
	public String getCreateRequestSettingView(ModelMap model) throws Exception {
		
		return "egovframework/com/primx/fun/sys/createRequestSetting";
	}
	
	@RequestMapping(value = "/fileTransferZoneSetting/view.do")
	public String getFileTransferZoneSettingView(ModelMap model) throws Exception {
		
		return "egovframework/com/primx/fun/sys/fileTransferZoneSetting";
	}
	
	@RequestMapping(value = "/fileMovePolicySetting/view.do")
	public String getFileMovePolicySettingView(ModelMap model) throws Exception {
		
		return "egovframework/com/primx/fun/sys/fileMovePolicySetting";
	}

	@RequestMapping(value = "/systemStatus/view.do")
	public String getList(ModelMap model) throws Exception {
		
		return "egovframework/com/primx/fun/sys/systemStatus";
	}

	@RequestMapping(value = "/dashboard/view.do")
	public String getDashboardView(ModelMap model) throws Exception {
		
		return "egovframework/com/primx/fun/sys/dashboard";
	}

	@ResponseBody
	@RequestMapping(value = "/systemStatus/list.do")
	public Map<String, Object> getSystemStatusList(SystemStatusSearchVO systemStatusSearchVO) throws Exception {
		systemStatusSearchVO.convertUserTimeToUTC();

		// JSON 형식 응답 구성
        Map<String, Object> rtnMap = primxSystemService.selectListSystemStatusInfo(systemStatusSearchVO);
    	
		return rtnMap;
	}
	
	@ResponseBody
	@RequestMapping(value = "/systemStatus/info.do")
	public Map<String, Object> getSystemStatusInfo(@RequestParam(value = "systemId", required = true) int systemId) throws Exception {
		
		// JSON 형식 응답 구성
		Map<String, Object> rtnMap = primxSystemService.selectSystemStatusInfo(systemId);
		
		return rtnMap;
	}

	@ResponseBody
	@RequestMapping(value = "/clientProgram/list.do", method = RequestMethod.GET)
	public List<ClientProgramDTO> getClientProgramList() throws Exception {
		// 클라이언트 프로그램 목록을 가져오는 서비스 호출
		List<ClientProgramDTO> clientPrograms = primxSystemService.getClientProgramList();
		return clientPrograms;
	}

	@ResponseBody
	@RequestMapping(value = "/product/list.do", method = RequestMethod.GET)
	public Map<String, Object> getProductList() throws Exception {
		List<String> products = primxSystemService.getProductList();
		
		// JSON 응답을 감싸는 객체 생성
		Map<String, Object> response = new HashMap<>();
		response.put("data", products); // DataTables가 기대하는 형식으로 "data" 키에 제품 목록을 추가

		return response; // JSON 형식으로 반환
	}

	@ResponseBody
	@RequestMapping(value = "/policy/script/list.do")
	public Map<String, Object> getPolicyScriptList(@RequestParam("productName") String productName) throws Exception {
		System.out.println("##########################################productName1: "+productName);
		// JSON 응답을 감싸는 객체 생성
		Map<String, Object> response = new HashMap<>();
		
		List<PolicyScriptDTO> scripts = primxSystemService.getPolicyScriptList(productName);

		response.put("data", scripts); // DataTables가 기대하는 형식으로 "data" 키에 스크립트 목록을 추가

		return response; // JSON 형식으로 반환
	}

	@ResponseBody
	@RequestMapping(value = "/clientProgram/update.do", method = RequestMethod.POST)
	public Map<String, Object> updateFiles(MultipartHttpServletRequest mRequest) throws Exception {
		Map<String, Object> response = new HashMap<>();
		

		try {

			System.out.println("###################### updateFiles 들어옴");
			boolean isGroup = Boolean.parseBoolean(mRequest.getParameter("isGroup"));
			System.out.println("###################### isGroup: "+isGroup);
			
			String[] programs = mRequest.getParameterValues("programs[]");
			System.out.println("################## programs: "+String.join(", ", programs));

			// 업로드 디렉토리 경로
			String uploadDir = EgovProperties.getProperty("PRIMX.package.path");
			String extWhiteList = "";
			if(programs[0].toUpperCase().equals("AGENT")) {
				uploadDir += EgovProperties.getProperty("PRIMX.package.agent");
				extWhiteList = EgovProperties.getProperty("PRIMX.package.agent.Extensions");
			}
			else {
				// 제품의 설치파일 이기때문에 폴더명을 제품명으로 설정
				uploadDir += programs[0].toUpperCase() + "/";
				extWhiteList = EgovProperties.getProperty("PRIMX.package.product.Extensions");
			}
			System.out.println("################## uploadDir: "+uploadDir);
			System.out.println("################## extWhiteList: "+extWhiteList);

			// 파일 업로드 처리
			Map<String,FileVO> uploadedFiles = primxFileService.uploadFiles(isGroup, uploadDir, extWhiteList, mRequest);
			System.out.println("###################### uploadedFiles.size(): "+uploadedFiles.size());
			primxSystemService.saveClientProgram(uploadedFiles, mRequest);

		} catch (Exception e) {
			e.printStackTrace();
			response.put("status", "error");
			return response;
		}

		response.put("status", "success");
		return response;
	}

	@ResponseBody
	@RequestMapping(value = "/policy/script/update.do", method = RequestMethod.POST)
	public Map<String, Object> updatePolicyScript(MultipartHttpServletRequest mRequest) throws Exception {
		Map<String, Object> response = new HashMap<>();
		try {
			boolean isGroup = Boolean.parseBoolean(mRequest.getParameter("isGroup"));
			
			System.out.println("###################### updateFiles 들어옴");
			System.out.println("###################### isGroup: "+isGroup);
			// 업로드 디렉토리 경로
			String uploadDir = EgovProperties.getProperty("PRIMX.policies.path");
			uploadDir += EgovProperties.getProperty("PRIMX.policies.script");
			String extWhiteList = EgovProperties.getProperty("PRIMX.policies.script.Extensions");

			System.out.println("################## uploadDir: "+uploadDir);
			System.out.println("################## extWhiteList: "+extWhiteList);
			// 파일 업로드 처리
			Map<String,FileVO> uploadedFiles = primxFileService.uploadFiles(isGroup, uploadDir, extWhiteList, mRequest);
			System.out.println("###################### uploadedFiles.size(): "+uploadedFiles.size());

			primxSystemService.updatePolicyScript(uploadedFiles, mRequest);
		} catch (Exception e) {
			e.printStackTrace();
			response.put("status", "error");
			return response;
		}
		
		response.put("status", "success");
		return response;
	}


	@ResponseBody
	@RequestMapping(value = "/updateLicense.do", method = RequestMethod.POST)
	public Map<String, Object> saveLicense(
	    @RequestParam("encryptedFile") MultipartFile encryptedFile,
	    @RequestParam("keyFile") MultipartFile keyFile) throws Exception {
	    
			System.out.println("saveLicense 들어옴");
	    Map<String, Object> response = new HashMap<>();

	    try {
	        // 파일 업로드 처리
	        primxSystemService.saveLicense(encryptedFile, keyFile);
	    } catch (Exception e) {
	        e.printStackTrace();
	        response.put("status", "error");
	        return response;
	    }

	    response.put("status", "success");
	    return response;
	}

	@ResponseBody
	@RequestMapping(value = "/fileDownloadServer/list.do", method = RequestMethod.GET)
	public Map<String, Object> getFileDownloadServerList() throws Exception {
		return primxSystemService.getFileDownloadServerList();
	}

	@ResponseBody
	@RequestMapping(value = "/fileDownloadServer/delete.do", method = RequestMethod.POST)
	public Map<String, Object> deleteFileDownloadServer(@RequestParam(value = "seqNo", required = true) int seqNo) throws Exception {
		return primxSystemService.deleteFileDownloadServer(seqNo);
	}

	@ResponseBody
	@RequestMapping(value = "/serverProgram/list.do", method = RequestMethod.GET)
	public Map<String, Object> getServerProgramList() throws Exception {
		return primxSystemService.getServerProgramList();
	}

	@ResponseBody
	@RequestMapping(value = "/fileDownloadServer/insert.do", method = RequestMethod.POST)
	public Map<String, Object> insertFileDownloadServer(
		@RequestBody List<ExServerInfoVO> exServerInfoVOList) throws Exception {
			Map<String, Object> response = new HashMap<>();
    
			for (ExServerInfoVO serverInfo : exServerInfoVOList) {
				// 각 서버 정보를 처리하는 로직
				System.out.println("Server Name: " + serverInfo.getServerName());
				System.out.println("Server URL: " + serverInfo.getServerUrl());
				// 서버 정보를 DB에 저장하는 메서드 호출
				primxSystemService.insertFileDownloadServer(serverInfo.getServerName(), serverInfo.getServerUrl());
			}
			
			response.put("status", "success");
			return response;
	}	

	@ResponseBody
	@RequestMapping(value = "/loadLicenseInfo.do", method = RequestMethod.GET)
	public Map<String, Object> loadLicenseInfo() throws Exception {
		return primxSystemService.loadLicenseInfo();
	}


	@ResponseBody
	@RequestMapping(value = "/createRequestSetting/save.do", method = RequestMethod.POST)
	public Map<String, Object> saveCreateRequestSetting(@RequestBody NoticeSettingVO noticeSettingVO) throws Exception {
		System.out.println("saveCreateRequestSetting 들어옴");
		System.out.println("noticeSettingVO: " + noticeSettingVO.toString());
		return primxRequestService.saveCreateRequestSetting(noticeSettingVO);
	}

	@ResponseBody
	@RequestMapping(value = "/updatePasswordSetting.do", method = RequestMethod.POST)
	public Map<String, Object> updatePasswordSetting(@RequestBody PasswordSettingVO passwordSettingVO) throws Exception {
		return primxSystemService.updatePasswordSetting(passwordSettingVO);
	}

	@ResponseBody
	@RequestMapping(value = "/fileMovePolicySetting/loadFileMovePolicySetting.do", method = RequestMethod.GET)
	public Map<String, Object> loadFileMovePolicySetting(@RequestParam("fileType") String fileType) throws Exception {
		return primxSystemService.loadFileMovePolicySetting(fileType);
	}

	@ResponseBody
	@RequestMapping(value = "/fileMovePolicySetting/saveFileMovePolicySetting.do", method = RequestMethod.POST)
	public Map<String, Object> saveFileMovePolicySetting(@RequestBody FileMovePolicySettingVO fileMovePolicySettingVO) throws Exception {
		return primxSystemService.saveFileMovePolicySetting(fileMovePolicySettingVO);
	}

	@ResponseBody
	@RequestMapping(value = "/updateSystemType.do", method = RequestMethod.POST)
	public Map<String, Object> updateSystemType(
		@RequestParam("seqNo") int seqNo,
		@RequestParam("agentId") String agentId,
		@RequestParam("userSystemMatchSeqNo") int userSystemMatchSeqNo,
		@RequestParam("commonSystemYn") String commonSystemYn) throws Exception {
		return primxSystemService.updateSystemType(seqNo, agentId, userSystemMatchSeqNo, commonSystemYn);
	}

	@ResponseBody
	@RequestMapping(value = "/deleteSystem.do", method = RequestMethod.POST)
	public Map<String, Object> deleteSystem(@RequestParam("seqNo") int seqNo) throws Exception {
		return primxSystemService.deleteSystem(seqNo);
	}

	@ResponseBody
	@RequestMapping(value = "/fileZone/list.do", method = RequestMethod.GET)
	public Map<String, Object> getFileZoneList() throws Exception {
		return primxSystemService.getFileZoneList();
	}

	@ResponseBody
	@RequestMapping(value = "/fileZone/selectFileZone.do", method = RequestMethod.GET)
	public Map<String, Object> selectFileZone(@RequestParam("policyId") int policyId) throws Exception {
		return primxSystemService.selectFileZone(policyId);
	}

	@ResponseBody
	@RequestMapping(value = "/fileZone/save.do", method = RequestMethod.POST)
	public Map<String, Object> saveFileZone(@RequestBody FileZoneVO fileZoneVO) throws Exception {
		System.out.println("fileZoneVO: " + fileZoneVO.toString());
		return primxSystemService.saveFileZone(fileZoneVO);
	}

	@ResponseBody
	@RequestMapping(value = "/fileZone/delete.do", method = RequestMethod.POST)
	public Map<String, Object> deleteFileZone(@RequestParam("zoneId") int zoneId) throws Exception {
		return primxSystemService.deleteFileZone(zoneId);
	}

	@ResponseBody
	@RequestMapping(value = "/systemStatusPolicy/list.do", method = RequestMethod.POST)
	public Map<String, Object> getSystemStatusPolicyList(SystemStatusSearchVO systemStatusSearchVO) throws Exception {
		return primxSystemService.getSystemStatusPolicyList(systemStatusSearchVO);
	}

	@ResponseBody
	@RequestMapping(value = "/loadServerConfig.do", method = RequestMethod.POST)
	public Map<String, Object> loadServerConfig(@RequestParam("keyName") String keyName) throws Exception {
		return primxSystemService.loadServerConfig(keyName);
	}

	// 서버 설정 저장 리스트로 전달
	@ResponseBody
	@RequestMapping(value = "/saveServerConfig.do", method = RequestMethod.POST)
	public Map<String, Object> saveServerConfig(@RequestBody List<ServerConfigVO> serverConfigVOList) throws Exception {
		Map<String, Object> response = new HashMap<>();
		try {
			response = primxSystemService.saveServerConfig(serverConfigVOList);
		} catch (Exception e) {
			e.printStackTrace();
			response.put("status", "error");
			response.put("message", e.getMessage());
		}
		return response;
	}

	@ResponseBody
	@RequestMapping(value = "/loadUnusedSystemList.do", method = RequestMethod.POST)
	public Map<String, Object> loadUnusedSystemList(@ModelAttribute("searchVO") DeptSearchVO searchVO) throws Exception {
		return primxSystemService.loadUnusedSystemList(searchVO);
	}

	@ResponseBody
	@RequestMapping(value = "/dashboard/policyStatus.do", method = RequestMethod.POST)
	public Map<String, Object> getPolicyStatus() throws Exception {
		return primxSystemService.getPolicyStatus();
	}
	
	@ResponseBody
	@RequestMapping(value = "/dashboard/licenseStatus.do", method = RequestMethod.POST)
	public Map<String, Object> getLicenseStatus() throws Exception {
		return primxSystemService.getLicenseStatus();
	}
	
	@ResponseBody
	@RequestMapping(value = "/dashboard/systemStatus.do", method = RequestMethod.POST)
	public Map<String, Object> getSystemStatus() throws Exception {
		return primxSystemService.getSystemStatus();
	}
	
	
	@ResponseBody
	@RequestMapping(value = "/dashboard/requestApprovalStatus.do", method = RequestMethod.POST)
	public Map<String, Object> getRequestApprovalStatus() throws Exception {
		return primxSystemService.getRequestApprovalStatus();
	}
	
	@ResponseBody
	@RequestMapping(value = "/dashboard/moveFileStatus.do", method = RequestMethod.POST)
	public Map<String, Object> getMoveFileStatus() throws Exception {
		return primxSystemService.getMoveFileStatus();
	}
	
	@ResponseBody
	@RequestMapping(value = "/dashboard/productLogStatus.do", method = RequestMethod.POST)
	public Map<String, Object> getProductLogStatus() throws Exception {
		return primxSystemService.getProductLogStatus();
	}
	
	@ResponseBody
	@RequestMapping(value = "/dashboard/getLogFilter.do", method = RequestMethod.POST)
	public Map<String, Object> getLogFilter() throws Exception {
		return primxSystemService.getLogFilter();
	}
	
	@ResponseBody
	@RequestMapping(value = "/dashboard/updateProductLogStatus.do", method = RequestMethod.POST)
	public Map<String, Object> updateProductLogStatus(@RequestParam("code") String code, @RequestParam("checked") boolean checked) throws Exception {
		return primxSystemService.updateProductLogStatus(code, checked);
	}

	@ResponseBody
	@RequestMapping(value = "/dashboard/productLog/list.do", method = RequestMethod.POST)
	public Map<String, Object> getProductLogList() throws Exception {
		return primxSystemService.getProductLogList();
	}
	
	@ResponseBody
	@RequestMapping(value = "/dashboard/individualPolicyList.do", method = RequestMethod.POST)
	public Map<String, Object> loadIndividualPolicyList() throws Exception {
		return primxPolicyService.loadIndividualPolicyList();
	}
}
