package egovframework.com.primx.fun.sys.service.impl;

import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStream;
import java.math.BigInteger;
import java.net.NetworkInterface;
import java.nio.file.CopyOption;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.security.MessageDigest;
import java.sql.SQLException;
import java.text.SimpleDateFormat;
import java.time.LocalDate;
import java.util.Base64;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import javax.annotation.Resource;
import javax.crypto.Cipher;
import javax.crypto.SecretKey;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

import org.apache.commons.io.FileUtils;
import org.apache.commons.io.FilenameUtils;
import org.ini4j.Ini;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.multipart.MultipartFile;
import org.springframework.web.multipart.MultipartHttpServletRequest;

import egovframework.com.cmm.LoginVO;
import egovframework.com.cmm.service.EgovProperties;
import egovframework.com.cmm.util.EgovHttpRequestHelper;
import egovframework.com.cmm.util.EgovUserDetailsHelper;
import egovframework.com.primx.fun.cmm.service.TextUtils;
import egovframework.com.primx.fun.dpt.service.DeptSearchVO;
import egovframework.com.primx.fun.dpt.service.DeptVO;
import egovframework.com.primx.fun.dpt.service.impl.DepartmentDAO;
import egovframework.com.primx.fun.env.service.EnvGeneralVO;
import egovframework.com.primx.fun.env.service.EnvSystemSettingsVO;
import egovframework.com.primx.fun.env.service.ServerConfigVO;
import egovframework.com.primx.fun.env.service.impl.EnvironmentDAO;
import egovframework.com.primx.fun.file.service.FileVO;
import egovframework.com.primx.fun.file.service.MoveFileInfoDTO;
import egovframework.com.primx.fun.file.service.MoveFileInfoDashboardDTO;
import egovframework.com.primx.fun.file.service.impl.FileDAO;
import egovframework.com.primx.fun.lic.service.LicenseDetailVO;
import egovframework.com.primx.fun.lic.service.LicenseGeneralVO;
import egovframework.com.primx.fun.log.service.AgentProductLogDTO;
import egovframework.com.primx.fun.log.service.impl.LogDAO;
import egovframework.com.primx.fun.pol.service.AgentPolicySystemMatchVO;
import egovframework.com.primx.fun.pol.service.NicInfoVO;
import egovframework.com.primx.fun.pol.service.PolicySystemMapVO;
import egovframework.com.primx.fun.pol.service.PolicyVO;
import egovframework.com.primx.fun.pol.service.impl.PolicyDAO;
import egovframework.com.primx.fun.sys.service.DiskInfoVO;
import egovframework.com.primx.fun.sys.service.ExServerInfoVO;
import egovframework.com.primx.fun.sys.service.PasswordSettingVO;
import egovframework.com.primx.fun.sys.service.PrimxSetupInfoDTO;
import egovframework.com.primx.fun.sys.service.PrimxSystemService;
import egovframework.com.primx.fun.sys.service.ProductsVO;
import egovframework.com.primx.fun.sys.service.RoleDTO;
import egovframework.com.primx.fun.sys.service.ServerProgramDTO;
import egovframework.com.primx.fun.sys.service.SystemMasterVO;
import egovframework.com.primx.fun.sys.service.SystemStatusResultDTO;
import egovframework.com.primx.fun.sys.service.SystemStatusSearchVO;
import egovframework.com.primx.fun.sys.service.UnusedSystemDTO;
import egovframework.com.primx.fun.sys.service.UpdateInfoVO;
import egovframework.com.primx.fun.sys.service.UserSystemMatchVO;
import egovframework.com.primx.fun.usr.service.UserRegistrationDTO;
import egovframework.com.primx.fun.usr.service.impl.UserDAO;
import egovframework.com.primx.fun.sys.service.FileMovePolicySettingVO;
import egovframework.com.primx.fun.sys.service.FileZoneVO;
import egovframework.com.primx.fun.sys.service.ClientProgramDTO;
import egovframework.com.primx.fun.sys.service.PolicyScriptDTO;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;

@Service("primxSystemService")
public class PrimxSystemServiceImpl implements PrimxSystemService {

	@Resource(name = "systemDAO")
	private SystemDAO systemDAO;
	
	@Resource(name = "environmentDAO")
	private EnvironmentDAO environmentDAO;
	
	@Resource(name = "departmentDAO")
	private DepartmentDAO departmentDAO;
	
	@Resource(name = "userDAO")
	private UserDAO userDAO;
	
	@Resource(name = "policyDAO")
	private PolicyDAO policyDAO;

	@Resource(name = "fileDAO")
	private FileDAO fileDAO;

	@Resource(name = "logDAO")
	private LogDAO logDAO;
	
	@Override
	public String generateCode(String prefix) throws Exception {
		String baseCode = prefix + new SimpleDateFormat("yyyyMMdd").format(new Date());
		
		// 오늘 날짜로 생성된 마지막 코드 조회
		String lastCode = systemDAO.selectLastDeptCode(prefix);
		
		int sequence = 1;
		if (lastCode != null && lastCode.startsWith(baseCode)) {
			// 마지막 3자리를 숫자로 변환하여 1 증가
			sequence = Integer.parseInt(lastCode.substring(lastCode.length() - 3)) + 1;
		}
		
		// 3자리 숫자로 포맷팅 (001, 002, ...)
		return baseCode + String.format("%03d", sequence);
	}

	/**
	 * 시스템 현황 목록 조회
	 */
	@Override
	public Map<String, Object> selectListSystemStatusInfo(SystemStatusSearchVO systemStatusSearchVO) throws SQLException {
		
		
		Map<String, Object> rtnMap = new HashMap<String, Object>();
		rtnMap.put("draw", systemStatusSearchVO.getDraw());
		try {
			int totalCnt = systemDAO.selectSystemStatusInfoCnt(systemStatusSearchVO);
			rtnMap.put("recordsTotal", totalCnt);
			rtnMap.put("recordsFiltered", totalCnt);
			rtnMap.put("page", systemStatusSearchVO.getPageIndex());
			rtnMap.put("pageSize", systemStatusSearchVO.getRecordCountPerPage());
			List<SystemStatusResultDTO> lists = systemDAO.selectListSystemStatusInfo(systemStatusSearchVO);
			for(SystemStatusResultDTO dto : lists) {
				dto.convertUTCtoUserTime();
			}
			
			rtnMap.put("data", lists);
			rtnMap.put("status", "success");
			rtnMap.put("message", "success");
		} catch (SQLException e) {
			rtnMap.put("status", "error");
			rtnMap.put("message", "error");
		}
		return rtnMap;
	}

	@Override
	public Map<String, Object> selectSystemStatusInfo(int systemId) throws SQLException {
		Map<String, Object> rtnMap = new HashMap<String, Object>();
		try {
			// 1. system 정보 조회
			SystemStatusResultDTO systemStatusResultDTO = systemDAO.selectSystemStatusInfo(systemId);
			systemStatusResultDTO.convertUTCtoUserTime();
			rtnMap.put("system", systemStatusResultDTO);

			if (systemStatusResultDTO != null) {
				// 2. disk 정보 목록 조회
				List<DiskInfoVO> diskInfoDTO = systemDAO.selectListDiskInfo(systemStatusResultDTO);
				for(DiskInfoVO dto : diskInfoDTO) {
					dto.convertUTCtoUserTime();
				}
				rtnMap.put("diskInfo", diskInfoDTO);
				
				// 3. nic 정보 목록 조회
				List<NicInfoVO> nicInfoDTO = systemDAO.selectListNicInfo(systemStatusResultDTO);
				for(NicInfoVO dto : nicInfoDTO) {
					dto.convertUTCtoUserTime();
				}
				rtnMap.put("nicInfo", nicInfoDTO);
				
				// 4. 설치 제품 목록 조회
				List<PrimxSetupInfoDTO> primxSetupInfoDTO = systemDAO.selectListPrimxSetupInfo(systemStatusResultDTO);
				for(PrimxSetupInfoDTO dto : primxSetupInfoDTO) {
					dto.convertUTCtoUserTime();
				}

				rtnMap.put("primxSetupInfo", primxSetupInfoDTO);
			}
			rtnMap.put("status", "success");
			rtnMap.put("message", "success");
		} catch (SQLException e) {
			rtnMap.put("status", "error");
			rtnMap.put("message", "error");
		}
		return rtnMap;
	}

	
	@Override
	public Map<String, Object> selectMySystemStatusInfo() throws SQLException {

		LoginVO loginVO = (LoginVO) EgovUserDetailsHelper.getAuthenticatedUser();
		Map<String, Object> rtnMap = new HashMap<String, Object>();
		try {
			// 1. system 정보 조회
			SystemStatusResultDTO systemStatusResultDTO = systemDAO.selectSystemStatusInfoById(loginVO.getId());
			systemStatusResultDTO.convertUTCtoUserTime();
			rtnMap.put("system", systemStatusResultDTO);

			if (systemStatusResultDTO != null) {
				// 2. disk 정보 목록 조회
				List<DiskInfoVO> diskInfoDTO = systemDAO.selectListDiskInfo(systemStatusResultDTO);
				for(DiskInfoVO dto : diskInfoDTO) {
					dto.convertUTCtoUserTime();
				}
				rtnMap.put("diskInfo", diskInfoDTO);
				
				// 3. nic 정보 목록 조회
				List<NicInfoVO> nicInfoDTO = systemDAO.selectListNicInfo(systemStatusResultDTO);
				for(NicInfoVO dto : nicInfoDTO) {
					dto.convertUTCtoUserTime();
				}
				rtnMap.put("nicInfo", nicInfoDTO);
				
				// 4. 설치 제품 목록 조회
				List<PrimxSetupInfoDTO> primxSetupInfoDTO = systemDAO.selectListPrimxSetupInfo(systemStatusResultDTO);
				for(PrimxSetupInfoDTO dto : primxSetupInfoDTO) {
					dto.convertUTCtoUserTime();
				}
				rtnMap.put("primxSetupInfo", primxSetupInfoDTO);
			}
			rtnMap.put("status", "success");
			rtnMap.put("message", "success");
		} catch (SQLException e) {
			rtnMap.put("status", "error");
			rtnMap.put("message", "error");
		}
		return rtnMap;
	}

	@Override
	public List<ClientProgramDTO> getClientProgramList() {
		// DAO를 통해 클라이언트 프로그램 목록을 조회
		List<ClientProgramDTO> clientProgramList = systemDAO.selectClientProgramList();
		for(ClientProgramDTO dto : clientProgramList) {
			dto.convertUTCtoUserTime();
		}
		return clientProgramList;
	}

	@Override
	public List<String> getProductList() {
		return systemDAO.selectProductList();
	}

	@Override
	public List<PolicyScriptDTO> getPolicyScriptList(String productName) {
		System.out.println("##########################################productName2: "+productName);
		return systemDAO.selectPolicyScriptList(productName);
	}

	@Override
	public Map<String, Object> selectRoleList() {
		Map<String, Object> rtnMap = new HashMap<String, Object>();
		try {	
			List<RoleDTO> roleList = systemDAO.selectRoleList();
			rtnMap.put("status", "success");
			rtnMap.put("message", "success");
			rtnMap.put("data", roleList);
		} catch (Exception e) {
			rtnMap.put("status", "error");
			rtnMap.put("message", "error");
			return rtnMap;
		}
		
		return rtnMap;
	}

	@Override
	public Map<String, Object> saveClientProgram(Map<String,FileVO> uploadedFiles, MultipartHttpServletRequest mRequest) {
		Map<String, Object> rtnMap = new HashMap<String, Object>();
		try {
			String[] rowIds = mRequest.getParameterValues("rowIds[]");
			String[] seqNos = mRequest.getParameterValues("seqNos[]");
			String[] programs = mRequest.getParameterValues("programs[]");
			String[] fileNames = mRequest.getParameterValues("fileNames[]");
			String[] productVersions = mRequest.getParameterValues("productVersion[]");
			String[] insertDates = mRequest.getParameterValues("insertDates[]");

			System.out.println("##########################################");
			System.out.println("rowIds: " + String.join(", ", rowIds));
			System.out.println("seqNos: " + String.join(", ", seqNos));
			System.out.println("programs: " + String.join(", ", programs));
			System.out.println("fileNames: " + String.join(", ", fileNames));
			System.out.println("productVersions: " + String.join(", ", productVersions));
			System.out.println("insertDates: " + String.join(", ", insertDates));
			System.out.println("##########################################");

			if (mRequest != null) {
				Iterator<?> fileIter = mRequest.getFileNames();
				int i = 0;
				while (fileIter.hasNext()) {
					MultipartFile file = mRequest.getFile((String)fileIter.next());
					System.out.println("file: "+file.toString());

					UpdateInfoVO dto = new UpdateInfoVO();
					dto.setSeqNo(Long.parseLong(seqNos[i]));
					System.out.println("dto.getSeqNo(): "+dto.getSeqNo());

					FileVO fileVO = uploadedFiles.get(fileNames[i]);
					dto.setFileName(fileVO.getFileOriginalNm());
					System.out.println("dto.getFileName(): "+dto.getFileName());

					Path targetLocation = Paths.get(fileVO.getStoredPath());
					byte[] data = Files.readAllBytes(targetLocation);
					System.out.println("data: "+data.toString());
					byte[] hash = MessageDigest.getInstance("SHA-512").digest(data);
					System.out.println("hash: "+hash.toString());
					String hashCode = (new BigInteger(1, hash)).toString(16);
					System.out.println("hashCode: "+hashCode);

					dto.setUpdateFileHash(hashCode);
					dto.setUpdateFilePath(fileVO.getStoredPath());
					dto.setProductVersion(productVersions[i]);

					systemDAO.updateClientProgram(dto);

					i++;
				}
			}

			rtnMap.put("status", "success");
			rtnMap.put("message", "success");
		} catch (Exception e) {
			rtnMap.put("status", "error");
			rtnMap.put("message", "error");
		}
		return rtnMap;
	}

	@Override
	public Map<String, Object> updatePolicyScript(Map<String,FileVO> uploadedFiles, MultipartHttpServletRequest mRequest) {
		Map<String, Object> rtnMap = new HashMap<String, Object>();

		try {
			String[] rowIds = mRequest.getParameterValues("rowIds[]");
			String[] fileNames = mRequest.getParameterValues("fileNames[]");

			for(int i = 0; i < rowIds.length; i++) {
				System.out.println("rowIds: "+Integer.parseInt(rowIds[i]));
				System.out.println("fileNames: "+fileNames[i]);
			}

			if (mRequest != null) {
				Iterator<?> fileIter = mRequest.getFileNames();
				int i = 0;
				while (fileIter.hasNext()) {
					FileVO fileVO = uploadedFiles.get(fileNames[i]);
					System.out.println("fileVO: "+fileVO.toString());

					systemDAO.updatePolicyScript(Integer.parseInt(rowIds[i]), fileVO.getFileId());
					i++;
				}
			}
		} catch (Exception e) {
			rtnMap.put("status", "error");
			rtnMap.put("message", "error");
		}
		return rtnMap;
	}



	@Override
	@Transactional
	public Map<String, Object> saveLicense(MultipartFile encryptedFile, MultipartFile keyFile) throws Exception {
		Map<String, Object> rtnMap = new HashMap<>();
		System.out.println("라이선스 등록 시작");
		System.out.println("##########################################encryptedFile: " + encryptedFile.toString());
		System.out.println("##########################################keyFile: " + keyFile.toString());
		
		String licenseFileDir = EgovProperties.getProperty("PRIMX.product.licenses.path");
		System.out.println("##########################################licenseFileDir: " + licenseFileDir);
		Path targetDir = Paths.get(licenseFileDir);
		System.out.println("##########################################targetDir: " + targetDir);
		String encryptedFileName = TextUtils.getRendomString(20);
		System.out.println("##########################################encryptedFileName: " + encryptedFileName);
		String keyFileName = TextUtils.getRendomString(15);
		System.out.println("##########################################keyFileName: " + keyFileName);
		String encryptedFileExtension = FilenameUtils.getExtension(encryptedFile.getOriginalFilename());
		System.out.println("##########################################encryptedFileExtension: " + encryptedFileExtension);
		
		if (encryptedFileExtension.isEmpty()) {
			throw new Exception("error.invalid.file.extension " + encryptedFileExtension);
		}
		
		String keyFileExtension = getExtension(keyFile, encryptedFileExtension);
		System.out.println("##########################################keyFileExtension: " + keyFileExtension);
		Path encTargetLocation = targetDir.resolve(encryptedFileName + "." + encryptedFileName);
		System.out.println("##########################################encTargetLocation: " + encTargetLocation);
		Path keyTargetLocation = targetDir.resolve(keyFileName + "." + keyFileName);
		System.out.println("##########################################keyTargetLocation: " + keyTargetLocation);
		Files.copy(encryptedFile.getInputStream(), encTargetLocation, new CopyOption[] { StandardCopyOption.REPLACE_EXISTING });
		System.out.println("##########################################keyFile.getInputStream(): " + keyFile.getInputStream());
		Files.copy(keyFile.getInputStream(), keyTargetLocation, new CopyOption[] { StandardCopyOption.REPLACE_EXISTING });
		System.out.println("##########################################keyFile.getInputStream(): " + keyFile.getInputStream());
		File keyFileToParse = keyTargetLocation.toFile();
		System.out.println("##########################################keyFileToParse: " + keyFileToParse);
		File encFileToParse = encTargetLocation.toFile();
		System.out.println("##########################################encFileToParse: " + encFileToParse);
		Ini keyIniFile = new Ini(keyFileToParse);
		System.out.println("##########################################keyIniFile: " + keyIniFile);
		String encFileString = FileUtils.readFileToString(encFileToParse, "UTF-8");
		System.out.println("##########################################encFileString: " + encFileString);
		Ini decryptedFile = checkIniFiles(keyIniFile, encFileString);
		System.out.println("##########################################decryptedFile: " + decryptedFile);
		insertLicense(decryptedFile, keyFileToParse);

		rtnMap.put("status", "success");
		rtnMap.put("message", "success");
		
		return rtnMap;
	}

	
	private void insertLicense(Ini ini, File keyFile) throws Exception {
		System.out.println("라이선스 처리 시작");
		String licenseFileDir = EgovProperties.getProperty("PRIMX.package.path");
		Path decryptedFilePath = Paths.get(licenseFileDir, "decrypted.ini");
		File decryptedFile = decryptedFilePath.toFile();
		Date today = new Date();
		LicenseGeneralVO licenseGeneral = new LicenseGeneralVO();
		EnvGeneralVO optionalEnvGeneral = environmentDAO.selectEnvGeneralById(Long.valueOf(1L));
		// System.out.println("##########################################optionalEnvGeneral: " + optionalEnvGeneral.toString());
		for (String sectionName : ini.keySet()) {
			// System.out.println("##########################################sectionName: " + sectionName);
			if (sectionName.equalsIgnoreCase("license")) {
				System.out.println("라이선스 섹션 처리 시작");
				Boolean isValid = checkGeneral(sectionName, ini);
				if (!isValid.booleanValue())
				throw new Exception("error.invalid.license.file.wrong.macAddress"); 
				if (optionalEnvGeneral != null) {
				EnvGeneralVO envGeneral = optionalEnvGeneral;
				envGeneral.setCompanyName(ini.get(sectionName, "company"));
				envGeneral.setCompanyId(ini.get(sectionName, "companyid"));
				environmentDAO.updateEnvGeneral(envGeneral);
				} 
				
				updateCompanyId(ini.get(sectionName, "companyid"), ini.get(sectionName, "company"));
				licenseGeneral.setCompany(ini.get(sectionName, "company"));
				licenseGeneral.setMacAddress(ini.get(sectionName, "mac").toUpperCase());

				// 라이선스 id 조회 
				int licenseId = environmentDAO.selectLicenseGeneralByMacAddress(licenseGeneral.getMacAddress());
				licenseGeneral.setId(licenseId);
				if(licenseId == 0) {
					environmentDAO.insertLicenseGeneral(licenseGeneral);
					// 라이선스 id 조회 
					licenseId = environmentDAO.selectLicenseGeneralByMacAddress(licenseGeneral.getMacAddress());
					licenseGeneral.setId(licenseId);
				}
				else {
					environmentDAO.updateLicenseGeneral(licenseGeneral);
				}

				System.out.println("라이선스 섹션 처리 완료");
				continue;
			} 
			if (sectionName.equalsIgnoreCase("ZONECENTRAL") || sectionName
				.equalsIgnoreCase("ORIZON")) {
				System.out.println("ZONECENTRAL 또는 ORIZON 섹션 처리 시작");
				LicenseGeneralVO optionalLicenseGeneral = environmentDAO.selectLicenseGeneralById(Long.valueOf(licenseGeneral.getId()));
				if (optionalLicenseGeneral == null) {
					throw new Exception("error.invalid.license.file"); 
				}
				if (ini.get(sectionName, "quantity") != null && ini.get(sectionName, "startdate") != null 
					&& ini.get(sectionName, "enddate") != null) {
					List<LicenseDetailVO> licenseDetailList = environmentDAO.findByProductName(sectionName);
					for (LicenseDetailVO deleteLicenses : licenseDetailList) {
						System.out.println("라이선스 상세 정보 삭제 시작");
						environmentDAO.deleteById(Long.valueOf(deleteLicenses.getId())); 
						System.out.println("라이선스 상세 정보 삭제 완료");
					}
					LicenseDetailVO newLicense = new LicenseDetailVO();
					System.out.println("라이선스 상세 정보 등록 시작");
					newLicense.setProductName(sectionName);
					newLicense.setQuantity(Integer.parseInt(ini.get(sectionName, "quantity")));
					newLicense.setStartDate(ini.get(sectionName, "startdate"));
					newLicense.setEndDate(
						ini.get(sectionName, "enddate").equals("9999-12-31") ? 
						"Unlimited" : 
						ini.get(sectionName, "enddate"));
					newLicense.setStatusCd(
						java.sql.Date.valueOf(ini.get(sectionName, "enddate")).after(today) ? 
						"USING" : 
						"EXPIRED");
					newLicense.setGeneralId(licenseGeneral.getId());
					newLicense.setProductKey(ini.get(sectionName, "key"));
					environmentDAO.insertLicenseDetail(newLicense);
					System.out.println("라이선스 상세 정보 등록 완료");
					ProductsVO optionalProducts = systemDAO.selectProductNameLikeIgnoreCase(sectionName);
				
					if (optionalProducts == null) {
						throw new Exception("message.not.found.product"); 
					}
					List<UpdateInfoVO> updateInfoList = systemDAO.selectByProductType(optionalProducts.getId());
					if (updateInfoList.isEmpty()) {
						System.out.println("Client 프로그램 목록에 ["+sectionName+"] 제품 없음 - 라이선스 등록 시작");
						// UpdateInfoVO updateInfo32 = new UpdateInfoVO();
						// updateInfo32.setProductType(optionalProducts.getId());
						// updateInfo32.setProcessorArchitecture(0L);
						// updateInfo32.setInsertDate(new Date());
						// systemDAO.insertUpdateInfo(updateInfo32);
						UpdateInfoVO updateInfo64 = new UpdateInfoVO();
						updateInfo64.setProductType(optionalProducts.getId());
						updateInfo64.setProcessorArchitecture(9L);
						updateInfo64.setInsertDate(new Date());
						systemDAO.insertUpdateInfo(updateInfo64);
						System.out.println("Client 프로그램 목록에 ["+sectionName+"] 제품 등록 완료");
					} else {
						System.out.println("Client 프로그램 목록에 ["+sectionName+"] 제품 있음 - 라이선스 등록 패스");
					} 
				} else {
				throw new Exception("error.invalid.license.file quantity, startdate, enddate");
				} 
				if (keyFile.delete() && decryptedFile.delete()) {
				System.out.println("Deleted the keyFile: " + keyFile.getName());
				System.out.println("Deleted the decryptedFile: " + decryptedFile.getName());
				continue;
				} 
				System.out.println("Failed to delete files.");
				continue;
			} 
			throw new Exception("error.invalid.license.file");
		} 
	}
	
	
	private void updateCompanyId(String companyId, String companyName) throws Exception {
		System.out.println("회사 ID 수정 시작");
		try {
			// 업데이트 전 회사 ID 조회
			String preCompanyId = environmentDAO.selectServerConfigCompanyId();
			environmentDAO.updateServerConfigCompanyId(companyId);
			// 회사가 존재하지 않는 경우
			if(preCompanyId == null) {
				DeptVO department = new DeptVO();
				department.setDeptName(companyName);
				department.setDeptId(companyId);
				department.setParentId(companyId);
				department.setInsertedByHrYn("Y");
				department.setCompanyId(companyId);
				department.setLevel(1L);
				department.setCreatedDate(new Date());
				department.setSortOrder(1L);
				departmentDAO.insertDepartment(department);
		
				// 부서 경로 조회
				String deptPath = departmentDAO.selectDeptPath(companyId);
				// 부서경로가 안나오는 경우 회사 경로를 확인한다
				if(deptPath == null) {
					deptPath = departmentDAO.selectCompanyPath(companyId);
				}
				
				// 등록
				policyDAO.insertDistributionPolicy(companyId, null, "9999-12-31", deptPath, "N");
		
			}
			else {
				departmentDAO.updateDeptMasterCompanyId(companyId, preCompanyId);
			}
			System.out.println("회사 ID 수정 완료");
		} catch (Exception e) {
			throw new Exception("Cannot update companyId");
		} 
	}
	
	private static String getExtension(MultipartFile keyFile, String encryptedFileExtension) throws Exception {
		String keyFileExtension = FilenameUtils.getExtension(keyFile.getOriginalFilename());
		if (!encryptedFileExtension.equalsIgnoreCase("dat"))
		throw new Exception("error.invalid.file.extension " + encryptedFileExtension); 
		assert keyFileExtension != null;
		if (!keyFileExtension.equalsIgnoreCase("ini"))
		throw new Exception("error.invalid.file.extension " + keyFileExtension); 
		return keyFileExtension;
	}
	
	private Ini checkIniFiles(Ini keyIniFile, String encFileString) throws Exception {
		String licenseFileDir = EgovProperties.getProperty("PRIMX.product.licenses.path");
		Path iVKeyFilePath = Paths.get(licenseFileDir, "pxIVKey.ini");
		File iVKeyFile = iVKeyFilePath.toFile();
		Ini iVKeyIni = new Ini(iVKeyFile);
		String key = "";
		String iVKey = "";
		for (String sectionName : iVKeyIni.keySet()) {
		if (sectionName.equalsIgnoreCase("info")) {
			iVKey = iVKeyIni.get(sectionName, "IVkey");
			continue;
		} 
		throw new Exception("error.invalid.licenseKey.file");
		} 
		for (String sectionName : keyIniFile.keySet()) {
		if (sectionName.equalsIgnoreCase("info")) {
			key = keyIniFile.get(sectionName, "Enckey");
			continue;
		} 
		throw new Exception("error.invalid.licenseKey.file");
		} 
		return toDecrypt(key, iVKey, encFileString);
	}

	private Ini toDecrypt(String key, String iVKey, String encFileString) throws Exception {
		String licenseFileDir = EgovProperties.getProperty("PRIMX.package.path");
		byte[] aesKey = hexStringToByteArray(key);
		byte[] iv = hexStringToByteArray(iVKey);
		SecretKey secretKey = new SecretKeySpec(aesKey, "AES");
		Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
		cipher.init(2, secretKey, new IvParameterSpec(iv));
		byte[] encryptedBytes = Base64.getDecoder().decode(encFileString);
		byte[] decryptedBytes = cipher.doFinal(encryptedBytes);
		String decryptedText = new String(decryptedBytes, "EUC-KR");
		System.out.println("Decrypted Text: " + decryptedText);
		File file = new File(licenseFileDir + "/decrypted.ini");
		OutputStream os = new FileOutputStream(file);
		os.write(decryptedText.getBytes());
		os.close();
		return new Ini(file);
	}

	
	public static byte[] hexStringToByteArray(String hexString) {
		String[] hexValues = hexString.split(",");
		byte[] byteArray = new byte[hexValues.length];
		for (int i = 0; i < hexValues.length; i++) {
		String trimmedHex = hexValues[i].trim();
		if (trimmedHex.startsWith("0x"))
			trimmedHex = trimmedHex.substring(2); 
		byteArray[i] = (byte)Integer.parseInt(trimmedHex, 16);
		} 
		return byteArray;
	}

	private Boolean checkGeneral(String section, Ini ini) throws Exception {
		System.out.println("checkGeneral 시작");
		String macAddress = insertMacAddress();
		if (section.equalsIgnoreCase("license") && 
		ini.get(section, "company") != null && !ini.get(section, "company").equalsIgnoreCase("") && 
		ini.get(section, "mac") != null && !ini.get(section, "mac").equalsIgnoreCase(""))
		return Boolean.valueOf(macAddress.equalsIgnoreCase(ini.get(section, "mac"))); 
		return Boolean.valueOf(false);
	}
	
	private String insertMacAddress() throws Exception {
		try {
			String interfaceName = EgovProperties.getProperty("PRIMX.interface");
			NetworkInterface ni = NetworkInterface.getByName(interfaceName);
			if (ni == null)
				throw new Exception("error.interface.name.did.not.match"); 
		System.out.println("NetworkInterface::" + ni);
		byte[] hardwareAddress = ni.getHardwareAddress();
		String[] hexadecimal = new String[hardwareAddress.length];
		for (int i = 0; i < hardwareAddress.length; i++) {
			hexadecimal[i] = String.format("%02X", new Object[] { Byte.valueOf(hardwareAddress[i]) });
		} 
		String macAddress = String.join(":", (CharSequence[])hexadecimal);
		System.out.println("MACADDRESS::" + macAddress);
		EnvSystemSettingsVO optional = environmentDAO.selectMacAddress(macAddress);
		if (optional == null) {
			EnvSystemSettingsVO systemSettings = new EnvSystemSettingsVO();
			systemSettings.setMacAddress(macAddress);
			environmentDAO.insertEnvSystemSettings(systemSettings);
		} 
		return macAddress;
		} catch (Exception e) {
		throw new Exception("error.interface.name.did.not.match");
		} 
	}

	@Override
	public Map<String, Object> getFileDownloadServerList() {
		Map<String, Object> rtnMap = new HashMap<>();
		try {
			List<ExServerInfoVO> fileDownloadServerList = systemDAO.selectFileDownloadServerList();
			for(ExServerInfoVO dto : fileDownloadServerList) {
				dto.convertUTCtoUserTime();
			}
			rtnMap.put("status", "success");
			rtnMap.put("message", "success");
			rtnMap.put("data", fileDownloadServerList);
		} catch (Exception e) {
			rtnMap.put("status", "error");
			rtnMap.put("message", "error");
		}
		return rtnMap;

	}

	@Override
	public Map<String, Object> deleteFileDownloadServer(int seqNo) {
		Map<String, Object> rtnMap = new HashMap<>();
		int result = systemDAO.deleteFileDownloadServer(seqNo);
		if(result > 0) {
			rtnMap.put("status", "success");
			rtnMap.put("message", "success");
		} else {
			rtnMap.put("status", "error");
			rtnMap.put("message", "error");
		}
		return rtnMap;
	}

	@Override
	public Map<String, Object> getServerProgramList() {
		Map<String, Object> rtnMap = new HashMap<>();
		try {
			List<ServerProgramDTO> serverProgramList = systemDAO.selectServerProgramList();
			for(ServerProgramDTO dto : serverProgramList) {
				dto.convertUTCtoUserTime();
			}

			rtnMap.put("status", "success");
			rtnMap.put("message", "success");
			rtnMap.put("data", serverProgramList);
		} catch (Exception e) {
			rtnMap.put("status", "error");
			rtnMap.put("message", "error");
		}
		return rtnMap;
	}

	@Override
	public Map<String, Object> insertFileDownloadServer(String serverName, String serverUrl) {
		Map<String, Object> rtnMap = new HashMap<>();
		int result = systemDAO.insertFileDownloadServer(serverName, serverUrl);
		if(result > 0) {
			rtnMap.put("status", "success");
			rtnMap.put("message", "success");
		} else {
			rtnMap.put("status", "error");	
		}
		return rtnMap;
	}

	@Override
	public Map<String, Object> loadLicenseInfo() {
		Map<String, Object> rtnMap = new HashMap<>();
		try {
			List<LicenseDetailVO> licenseDetailList = systemDAO.selectLicenseDetailList();
			for(LicenseDetailVO licenseDetail : licenseDetailList) {
				licenseDetail.convertUTCtoUserTime();
			}
			rtnMap.put("status", "success");
			rtnMap.put("message", "success");
			rtnMap.put("data", licenseDetailList);
		} catch (Exception e) {
			rtnMap.put("status", "error");
			rtnMap.put("message", "error");
		}
		return rtnMap;
	}

	@Override
	public Map<String, Object> loadPasswordSettingInfo() {
		Map<String, Object> rtnMap = new HashMap<>();
		try {
			// List<String> authList = EgovUserDetailsHelper.getAuthorities();
        	// String userAuth = authList.get(0);
			String userRole = "USER";
			// ADMIN이라는 글자가 들어가는 변경
			// if(userAuth.contains("ADMIN")) {
			// 	userRole = "ADMIN";
			// }
			PasswordSettingVO passwordSettingVO = systemDAO.selectPasswordSettingInfo(userRole);
			rtnMap.put("status", "success");
			rtnMap.put("message", "success");
			rtnMap.put("data", passwordSettingVO);
		} catch (Exception e) {
			rtnMap.put("status", "error");
			rtnMap.put("message", "error");
		}
		return rtnMap;
	}

	@Override
	public Map<String, Object> updatePasswordSetting(PasswordSettingVO passwordSettingVO) {
		Map<String, Object> rtnMap = new HashMap<>();
		try {
			int result = 0;
			// 기존 데이터가 있는지 확인하고 id가 asc 인 값을 가져온다
			PasswordSettingVO existingPasswordSetting = systemDAO.selectPasswordSettingInfo(passwordSettingVO.getUserRole());
			if(existingPasswordSetting != null) {
				passwordSettingVO.setId(existingPasswordSetting.getId());
				result = systemDAO.updatePasswordSetting(passwordSettingVO);
			}
			else {
				result = systemDAO.insertPasswordSetting(passwordSettingVO);
			}
			if(result > 0) {
				rtnMap.put("status", "success");
				rtnMap.put("message", "success");
			} else {
				rtnMap.put("status", "error");
				rtnMap.put("message", "error");
			}
		} catch (Exception e) {
			rtnMap.put("status", "error");
			rtnMap.put("message", "error");
		}
		return rtnMap;
	}

	@Override
	public Map<String, Object> loadFileMovePolicySetting(String fileType) {
		Map<String, Object> rtnMap = new HashMap<>();
		try {
			FileMovePolicySettingVO fileMovePolicySettingVO = systemDAO.selectFileMovePolicySettingInfo(fileType);
			rtnMap.put("status", "success");
			rtnMap.put("message", "success");
			rtnMap.put("data", fileMovePolicySettingVO);
		} catch (Exception e) {
			rtnMap.put("status", "error");
			rtnMap.put("message", "error");
		}
		return rtnMap;
	}

	@Override
	public Map<String, Object> saveFileMovePolicySetting(FileMovePolicySettingVO fileMovePolicySettingVO) {
		Map<String, Object> rtnMap = new HashMap<>();
		try {
			int result = 0;
			// 기존 데이터가 있는지 확인하고 id가 asc 인 값을 가져온다
			FileMovePolicySettingVO existingFileMovePolicySetting = systemDAO.selectFileMovePolicySettingInfo(fileMovePolicySettingVO.getFileType());
			if(existingFileMovePolicySetting != null) {
				fileMovePolicySettingVO.setFileId(existingFileMovePolicySetting.getFileId());
				result = systemDAO.updateFileMovePolicySetting(fileMovePolicySettingVO);
			} else {
				result = systemDAO.insertFileMovePolicySetting(fileMovePolicySettingVO);
			}
			if(result > 0) {
				rtnMap.put("status", "success");
				rtnMap.put("message", "success");
			} else {
				rtnMap.put("status", "error");
				rtnMap.put("message", "error");
			}
		} catch (Exception e) {
			rtnMap.put("status", "error");
			rtnMap.put("message", "error");
		}
		return rtnMap;
	}

	// 실패시 전체 실패처리
	@Transactional
	@Override
	public Map<String, Object> updateSystemType(int seqNo, String agentId, int userSystemMatchSeqNo, String commonSystemYn) {
		Map<String, Object> rtnMap = new HashMap<>();
		try {
			String clientIp = EgovHttpRequestHelper.getRequestIp();
			LoginVO user = (LoginVO) EgovUserDetailsHelper.getAuthenticatedUser();

			if(commonSystemYn.equals("N")) {
				// EMPLYR_ID가 GUEST_PC 인 사용자를 조회
				UserRegistrationDTO userRegistrationDTO = userDAO.selectUserDetails("GUEST_PC");				
				if(userRegistrationDTO != null) {
					UserSystemMatchVO userSystemMatchVO = UserSystemMatchVO.builder()
						.userId(userRegistrationDTO.getEmplyrId())
						.agentId(agentId)
						.companyId(userRegistrationDTO.getCompanyId())
						.modifiedBy(user.getId())
						.modifiedIp(clientIp)
						.build();
					System.out.println("userSystemMatchVO: " + userSystemMatchVO.toString());
					systemDAO.insertUserSystemMatch(userSystemMatchVO);
				}
			}
			else {
				UserSystemMatchVO userSystemMatchVO = systemDAO.selectUserSystemMatchInfo(userSystemMatchSeqNo);
				if(userSystemMatchVO != null) {
					System.out.println("userSystemMatchVO: " + userSystemMatchVO.toString());
					systemDAO.deleteUserSystemMatch(userSystemMatchSeqNo);
					
				}
			}
			System.out.println("seqNo: " + seqNo);
			System.out.println("agentId: " + agentId);
			System.out.println("commonSystemYn: " + commonSystemYn);
			int result = systemDAO.updateSystemType(seqNo, agentId, commonSystemYn);
			System.out.println("result: " + result);
			if(result > 0) {
				rtnMap.put("status", "success");
				rtnMap.put("message", "success");
			} else {
				rtnMap.put("status", "error");
				rtnMap.put("message", "error");
			}
		} catch (Exception e) {
			rtnMap.put("status", "error");
			rtnMap.put("message", "error");
		}
		return rtnMap;
	}

	@Transactional
	@Override
	public Map<String, Object> deleteSystem(int seqNo) {
		Map<String, Object> rtnMap = new HashMap<>();
		LoginVO user = (LoginVO) EgovUserDetailsHelper.getAuthenticatedUser();
		String clientIp = EgovHttpRequestHelper.getRequestIp();
		try {
			// 시스템id로 T_SYSTEM_MASTER 조회
			SystemMasterVO systemMasterVO = systemDAO.selectSystemMasterInfo(seqNo);
			if(systemMasterVO == null) {
				// 시스템id가 없으면 에러
				rtnMap.put("status", "error");
				rtnMap.put("message", "message.not.found");
			} else {
				System.out.println("\n\n####################################### systemMasterVO: " + systemMasterVO.toString());
				// 조회된 T_SYSTEM_MASTER에서 AgentId로 T_POLICY_SYSTEM_MAP List 조회
				List<PolicySystemMapVO> policySystemMapList = policyDAO.selectPolicySystemMapListByAgentId(systemMasterVO.getAgentId());
				for(PolicySystemMapVO policySystemMapVO : policySystemMapList) {
					System.out.println("\n\n####################################### policySystemMapVO: " + policySystemMapVO.toString());
					// 조회된 T_SYSTEM_MASTER에서 PolicyCode로 T_POLICY 조회
					PolicyVO policyVO = policyDAO.selectPolicyInfo(policySystemMapVO.getPolicyCode());
					if(policyVO != null) {
						System.out.println("\n\n####################################### policyVO: " + policyVO.toString());
						String indexName = "null";
						String targetProduct = targetProduct(policyVO, false);
						System.out.println("\n\n####################################### targetProduct: " + targetProduct);
						String productName = targetProduct(policyVO, true);
						System.out.println("\n\n####################################### productName: " + productName);
						if(policyVO.getProduct() != null) {
							if (policyVO.getProduct().getProductName().equalsIgnoreCase("zonecentral")) {
								indexName = "zc";
							} else {
								indexName = "oz";
							}
						}
						System.out.println("\n\n####################################### indexName: " + indexName);
						// 이전 policy.xml을 가져옴
						// String oldRedirect = getRedirect(productName, policyVO.getPolicyCode());
						// xml에서 수정
						// xmlEditor.removeTagByAttributeName(targetProduct, oldRedirect, systemMasterVO.getComputerName(), false, indexName);
						// // T_POLICY_SYSTEM_MAP에서 id로 삭제
					}
					System.out.println("\n\n####################################### policySystemMapVO.getId(): " + policySystemMapVO.getId());
					System.out.println("\n\n####################################### deletePolicySystemMap 실행");
					policyDAO.deletePolicySystemMap(policySystemMapVO.getId());
				}
				// 조회된 T_SYSTEM_MASTER에서 AgentId로 T_AGENT_POLICY_SYSTEM_MATCH 조회
				List<AgentPolicySystemMatchVO> agentPolicySystemMatchList = policyDAO.selectAgentPolicySystemMatchList(systemMasterVO.getAgentId());
				// 있으면 반복해서 T_AGENT_POLICY_SYSTEM_MATCH의 Id로 삭제
				for(AgentPolicySystemMatchVO agentPolicySystemMatchVO : agentPolicySystemMatchList) {
					System.out.println("\n\n####################################### agentPolicySystemMatchVO: " + agentPolicySystemMatchVO.toString());
					System.out.println("\n\n####################################### deleteAgentPolicySystemMatch 실행");
					policyDAO.deleteAgentPolicySystemMatch(agentPolicySystemMatchVO.getSeqNo());
				}
				// 조회된 T_SYSTEM_MASTER에서 AgentId로 T_USER_SYSTEM_MATCH 조회
				List<UserSystemMatchVO> userSystemMatchList = systemDAO.selectUserSystemMatchList(systemMasterVO.getAgentId());
				// 있는 경우 T_USER_SYSTEM_MATCH의 Id로 삭제
				for(UserSystemMatchVO userSystemMatchVO : userSystemMatchList) {
					System.out.println("\n\n####################################### userSystemMatchVO: " + userSystemMatchVO.toString());
					System.out.println("\n\n####################################### deleteUserSystemMatch 실행");
					systemDAO.deleteUserSystemMatch(userSystemMatchVO.getSeqNo());
				}
				// 실행: update T_SYSTEM_MASTER set C_MODIFIED_BY = :adminId, C_UPDATE_DATE = now(), C_MODIFIED_IP = :clientIp, C_USE_YN = 'N', C_UNINSTALL_YN = 'Y' WHERE C_SEQ_NO = :seqNo
				System.out.println("\n\n#######################################seqNo: " + seqNo);
				System.out.println("\n\n#######################################	user.getId(): " + user.getId());
				System.out.println("\n\n#######################################	clientIp: " + clientIp);
				System.out.println("\n\n#######################################	updateSystemMaster 실행");
				systemDAO.updateSystemMaster(seqNo, user.getId(), clientIp, "N", "Y");
			}
		} catch (Exception e) {
			rtnMap.put("status", "error");
			rtnMap.put("message", e.getMessage());
		}
		return rtnMap;
	}
	
	private String localHostName = "PRIMX-localhost";
	private String policyFileDir = "PRIMX-policyFileDir";

	public String getRedirect(String productName, String policyCode) {
		return localHostName + "PRIMX/" + localHostName + "/POLICY/" + productName + ".xml";
	  }

	public String targetProduct(PolicyVO policyVO, boolean qty) {
		String productName = null;
		if (policyVO.getProductId() == 1) {
			productName = "ZONECENTRAL";
		} else if (policyVO.getProductId() == 2) {
			productName = "ORIZON";
		}
		if (!qty) {
			return policyFileDir + "/" + policyFileDir; 
		}
		return productName;
	}

	@Override
	public Map<String, Object> getFileZoneList() { 
		Map<String, Object> rtnMap = new HashMap<>();
		try {
			List<FileZoneVO> fileZoneList = systemDAO.selectFileZoneList();
			rtnMap.put("status", "success");
			rtnMap.put("message", "success");
			rtnMap.put("data", fileZoneList);
		} catch (Exception e) {
			rtnMap.put("status", "error");
			rtnMap.put("message", e.getMessage());
		}
		return rtnMap;
	}

	@Override
	public Map<String, Object> selectFileZone(int policyId) {
		Map<String, Object> rtnMap = new HashMap<>();
		try {
			List<FileZoneVO> fileZoneList = systemDAO.selectFileZone(policyId);
			rtnMap.put("status", "success");
			rtnMap.put("message", "success");
			rtnMap.put("data", fileZoneList);
		} catch (Exception e) {
			rtnMap.put("status", "error");
			rtnMap.put("message", e.getMessage());
		}
		return rtnMap;
	}



	@Override
	@Transactional
	public Map<String, Object> saveFileZone(FileZoneVO fileZoneVO) {
		Map<String, Object> rtnMap = new HashMap<>();
		try {
			int result = 0;
			if(fileZoneVO.getZoneId() != 0) {
				result = systemDAO.updateFileZone(fileZoneVO);
			}
			else {
				result = systemDAO.insertFileZone(fileZoneVO);
			}

			if(result > 0) {
				rtnMap.put("status", "success");
				rtnMap.put("message", "success");
			} else {
				rtnMap.put("status", "error");
				rtnMap.put("message", "error");
			}
		} catch (Exception e) {
			rtnMap.put("status", "error");
			rtnMap.put("message", e.getMessage());
		}
		return rtnMap;
	}

	@Override
	public Map<String, Object> deleteFileZone(int zoneId) {
		Map<String, Object> rtnMap = new HashMap<>();
		try {
			int result = systemDAO.deleteFileZone(zoneId);
			if(result > 0) {
				rtnMap.put("status", "success");
				rtnMap.put("message", "success");
			} else {
				rtnMap.put("status", "error");
				rtnMap.put("message", "error");
			}
		} catch (Exception e) {
			rtnMap.put("status", "error");
			rtnMap.put("message", e.getMessage());
		}
		return rtnMap;
	}

	@Override
	public Map<String, Object> getSystemStatusPolicyList(SystemStatusSearchVO systemStatusSearchVO) {
		Map<String, Object> rtnMap = new HashMap<>();
		rtnMap.put("draw", systemStatusSearchVO.getDraw());
		try {
			int totalCnt = systemDAO.selectSystemStatusPolicyListCnt(systemStatusSearchVO);
			rtnMap.put("recordsTotal", totalCnt);
			rtnMap.put("recordsFiltered", totalCnt);
			List<SystemStatusResultDTO> systemStatusResultDTOList = systemDAO.selectSystemStatusPolicyList(systemStatusSearchVO);
			for(SystemStatusResultDTO systemStatusResultDTO : systemStatusResultDTOList) {
				systemStatusResultDTO.convertUTCtoUserTime();
			}
			rtnMap.put("page", systemStatusSearchVO.getPageIndex());
			rtnMap.put("pageSize", systemStatusSearchVO.getRecordCountPerPage());
			rtnMap.put("status", "success");
			rtnMap.put("message", "success");
			rtnMap.put("data", systemStatusResultDTOList);
		} catch (Exception e) {
			rtnMap.put("status", "error");
			rtnMap.put("message", e.getMessage());
		}
		return rtnMap;
	}

	@Override
	public Map<String, Object> loadServerConfig(String keyName) {
		Map<String, Object> rtnMap = new HashMap<>();
		try {
			ServerConfigVO serverConfigVO = systemDAO.selectServerConfig(keyName);
			serverConfigVO.convertUTCtoUserTime();
			rtnMap.put("status", "success");
			rtnMap.put("message", "success");
			rtnMap.put("data", serverConfigVO);
		} catch (Exception e) {
			rtnMap.put("status", "error");
			rtnMap.put("message", e.getMessage());
		}
		return rtnMap;
	}

	@Override
	@Transactional
	public Map<String, Object> saveServerConfig(List<ServerConfigVO> serverConfigVOList) {

		Map<String, Object> rtnMap = new HashMap<>();
		for(ServerConfigVO serverConfigVO : serverConfigVOList) {
			// 기존 keyName이 있는지 확인한다
			int result = 0;
			ServerConfigVO existingServerConfigVO = systemDAO.selectServerConfig(serverConfigVO.getKeyName());
			if(existingServerConfigVO != null) {
				// 있는경우 업데이트
				serverConfigVO.setSeqNo(existingServerConfigVO.getSeqNo());
				result = systemDAO.updateServerConfig(serverConfigVO);
			}
			else {
				// 없는경우 생성
				result = systemDAO.insertServerConfig(serverConfigVO);
			}
		}
		rtnMap.put("status", "success");
		rtnMap.put("message", "success");
		return rtnMap;
	}

	@Override
	public Map<String, Object> loadUnusedSystemList(DeptSearchVO searchVO) {
		Map<String, Object> rtnMap = new HashMap<>();
		try {
			// InactiveDel 설정 조회
			ServerConfigVO serverConfigVO = systemDAO.selectServerConfig("InactiveDel");
			
			if (serverConfigVO != null && serverConfigVO.getValueData() != null) {
				// valueData JSON 파싱
				ObjectMapper mapper = new ObjectMapper();
				JsonNode jsonNode = mapper.readTree(serverConfigVO.getValueData());
				
				// term과 time 값 추출
				String term = jsonNode.get("term").asText();
				String time = jsonNode.get("time").asText();
				
				// 미접속 시스템 조회 - term과 time 전달
				List<UnusedSystemDTO> systemMasterList = systemDAO.selectUnusedSystemList(term, time, searchVO.getStart(), searchVO.getRecordCountPerPage());
				int totalCnt = systemDAO.selectUnusedSystemListCnt(term, time);
				
				rtnMap.put("status", "success");
				rtnMap.put("message", "success");
				rtnMap.put("data", systemMasterList);
				rtnMap.put("draw", searchVO.getDraw());
				rtnMap.put("recordsTotal", totalCnt);
				rtnMap.put("recordsFiltered", totalCnt);
			} else {
				rtnMap.put("status", "error");
				rtnMap.put("message", "InactiveDel config not found");
			}
		} catch (Exception e) {
			rtnMap.put("status", "error");
			rtnMap.put("message", e.getMessage());
		}
		return rtnMap;
	}

	@Override
	public Map<String, Object> getPolicyStatus() {
		Map<String, Object> rtnMap = new HashMap<>();
		SystemStatusSearchVO systemStatusSearchVO = new SystemStatusSearchVO();

		int totalCnt = systemDAO.selectSystemStatusPolicyListCnt(systemStatusSearchVO);
		systemStatusSearchVO.setRecordCountPerPage(totalCnt);
		List<SystemStatusResultDTO> systemStatusResultDTOList = systemDAO.selectSystemStatusPolicyList(systemStatusSearchVO);
		
		// 전체 기본 정책 수 계산
		int totalDefaultPolicyCnt = (int) systemStatusResultDTOList.stream()
			.filter(dto -> "Y".equals(dto.getDefaultPolicyYn()))
			.count();

		// 정책 미적용 수 계산
		int unappliedDefaultPolicyCnt = (int) systemStatusResultDTOList.stream()
			.filter(dto -> dto.getPolicyCode() == null)
			.count();
			
		// 적용된 개별 정책 수 계산
		int appliedCustomPolicyCnt = (int) systemStatusResultDTOList.stream()
			.filter(dto -> "N".equals(dto.getDefaultPolicyYn()))
			.filter(dto -> dto.getModifiedDate() != null)
			.count();
		
		// 미적용된 개별 정책 수 계산
		int unappliedCustomPolicyCnt = (int) systemStatusResultDTOList.stream()
			.filter(dto -> "N".equals(dto.getDefaultPolicyYn()))
			.filter(dto -> dto.getModifiedDate() == null)
			.count();
		
		Map<String, Object> data = new HashMap<>();
		data.put("totalDefaultPolicyCnt", totalDefaultPolicyCnt);
		data.put("unappliedDefaultPolicyCnt", unappliedDefaultPolicyCnt);
		data.put("appliedCustomPolicyCnt", appliedCustomPolicyCnt);
		data.put("unappliedCustomPolicyCnt", unappliedCustomPolicyCnt);
		
		rtnMap.put("status", "success");
		rtnMap.put("message", "success");
		rtnMap.put("data", data);
		return rtnMap;
	}

	@Override
	public Map<String, Object> getLicenseStatus() {
		Map<String, Object> rtnMap = new HashMap<>();
		SystemStatusSearchVO systemStatusSearchVO = new SystemStatusSearchVO();

		int totalUsedCnt = systemDAO.selectSystemStatusPolicyListCnt(systemStatusSearchVO);

		List<LicenseDetailVO> licenseDetailList = systemDAO.selectLicenseDetailList();
		for(LicenseDetailVO licenseDetail : licenseDetailList) {
			licenseDetail.convertUTCtoUserTime();
		}

		// null 처리
		LicenseDetailVO zcLicenseDetail = licenseDetailList.stream()
			.filter(dto -> dto.getProductName().equals("ZONECENTRAL"))
			.findFirst().orElse(new LicenseDetailVO());
		
		Map<String, Object> data = new HashMap<>();
		data.put("zcLicenseCnt", zcLicenseDetail.getQuantity());
		data.put("totalUsedCnt", totalUsedCnt);

		rtnMap.put("status", "success");
		rtnMap.put("message", "success");
		rtnMap.put("data", data);
		return rtnMap;
	}
	
	//시스템현황 조회
	@Override
	public Map<String, Object> getSystemStatus() throws Exception {
		Map<String, Object> rtnMap = new HashMap<>();

		Map<String, Object> sysStat = systemDAO.selectSystemStat();
		double cpuStat = Double.parseDouble(sysStat.get("cpu_stat").toString());
		double memStat = Double.parseDouble(sysStat.get("mem_stat").toString());
		double memTotal = Double.parseDouble(sysStat.get("mem_total").toString());
		double diskStat = Double.parseDouble(sysStat.get("disk_stat").toString());
		double diskTotal = Double.parseDouble(sysStat.get("disk_total").toString());

		List<String> commonYnList = systemDAO.selectCommonSystemList();

		// 공용 PC 개수
		int commonPcCount = (int) commonYnList.stream()
			.filter("Y"::equalsIgnoreCase)
			.count();

		// 사용자 PC 개수
		int userPcCount = (int) commonYnList.stream()
			.filter("N"::equalsIgnoreCase)
			.count();

		int notConnectedPcCount = 0;

		

		// InactiveDel 설정 조회
		ServerConfigVO serverConfigVO = systemDAO.selectServerConfig("InactiveDel");
			
		if (serverConfigVO != null && serverConfigVO.getValueData() != null) {
			// valueData JSON 파싱
			ObjectMapper mapper = new ObjectMapper();
			JsonNode jsonNode = mapper.readTree(serverConfigVO.getValueData());
			
			// term과 time 값 추출
			String term = jsonNode.get("term").asText();
			String time = jsonNode.get("time").asText();
			
			// 미접속 시스템 조회 - term과 time 전달
			notConnectedPcCount = systemDAO.selectUnusedSystemListCnt(term, time);
			
		}
		// 필요한 데이터만 구성
		Map<String, Object> data = new HashMap<>();
		data.put("cpuStat", cpuStat);
		data.put("memStat", memStat);
		data.put("memTotal", memTotal);
		data.put("diskStat", diskStat);
		data.put("diskTotal", diskTotal);
		data.put("commonPcCount", commonPcCount);
		data.put("userPcCount", userPcCount);
		data.put("notConnectedPcCount", notConnectedPcCount);

		rtnMap.put("status", "success");
		rtnMap.put("message", "success");
		rtnMap.put("data", data);

		return rtnMap;
	}
	
	//신청 및 결재 현황 조회
	@Override
	public Map<String, Object> getRequestApprovalStatus() throws Exception {
		Map<String, Object> rtnMap = new HashMap<>();

		List<String> requestList = systemDAO.selectRequestList();

		// 신청 전체 개수 
		int requestCount = (int) requestList.stream()
			.count();

		//  사용자 등록 신청 개수
		int userRegistCount = (int) requestList.stream()
			.filter("A"::equalsIgnoreCase)
			.count();

		// 계정 연장 신청 개수
		int accountExtendCount = (int) requestList.stream()
			.filter("C"::equalsIgnoreCase)
			.count();

		// 이동 파일 신청 개수
		int moveFileCount = (int) requestList.stream()
			.filter("M"::equalsIgnoreCase)
			.count();
		
		List<String> approvalList = systemDAO.selectApprovalList();

		// 결재 전체 개수 
		int approvalCount = (int) approvalList.stream()
			.count();

		//  결재 승인 개수
		int approvalAllowCount = (int) approvalList.stream()
			.filter("AP"::equalsIgnoreCase)
			.count();

		// 결재 반려 개수
		int rejectCount = (int) approvalList.stream()
			.filter("RE"::equalsIgnoreCase)
			.count();

		Map<String, Object> data = new HashMap<>();
		data.put("requestCount", requestCount);
		data.put("userRegistCount", userRegistCount);
		data.put("accountExtendCount", accountExtendCount);
		data.put("moveFileCount", moveFileCount);
		data.put("approvalCount", approvalCount);
		data.put("approvalAllowCount", approvalAllowCount);
		data.put("rejectCount", rejectCount);

		rtnMap.put("status", "success");
		rtnMap.put("message", "success");
		rtnMap.put("data", data);

		return rtnMap;
	}

			


	
	@Override
	public Map<String, Object> getMoveFileStatus() {
		Map<String, Object> rtnMap = new HashMap<>();
		
		// InactiveDel 설정 조회
		ServerConfigVO serverConfigVO = systemDAO.selectServerConfig("FileDecryptCountLimit");
		int maxCnt = 0;
		if (serverConfigVO != null && serverConfigVO.getValueData() != null) {
			maxCnt = Integer.parseInt(serverConfigVO.getValueData());
		}

		List<MoveFileInfoDTO> moveFileInfoList = fileDAO.selectMoveFileStatus();
		moveFileInfoList.stream().forEach(dto -> dto.convertUTCtoUserTime());
		
		MoveFileInfoDashboardDTO moveFileInfoDashboardDTO = 
			MoveFileInfoDashboardDTO.convertMoveFileStatus(moveFileInfoList, maxCnt);

		rtnMap.put("status", "success");
		rtnMap.put("message", "success");
		rtnMap.put("data", moveFileInfoDashboardDTO);

		return rtnMap;
	}
	
	@Override
	public Map<String, Object> getProductLogStatus() {
		Map<String, Object> rtnMap = new HashMap<>();
		return rtnMap;
	}

	@Override
	public Map<String, Object> getLogFilter() throws Exception {
		Map<String, Object> rtnMap = new HashMap<>();

		LoginVO loginVO = (LoginVO) EgovUserDetailsHelper.getAuthenticatedUser();
		String userId = loginVO.getId();

		List<String> logFilterList = systemDAO.selectLogFilterList(userId);

		rtnMap.put("status", "success");
		rtnMap.put("message", "success");
		rtnMap.put("data", logFilterList);
		return rtnMap;
	}


	@Override
	public Map<String, Object> updateProductLogStatus(String code, boolean checked) throws Exception {
		Map<String, Object> rtnMap = new HashMap<>();

		LoginVO loginVO = (LoginVO) EgovUserDetailsHelper.getAuthenticatedUser();
		String userId = loginVO.getId();

		List<String> logFilterList = systemDAO.selectLogFilterList(userId);

		if(checked) {
			// logFilterList에 code가 없는 경우 insert 
			if(!logFilterList.contains(code)) {
				systemDAO.insertLogFilter(userId, code);
			}
		}
		else {
			systemDAO.deleteLogFilter(userId, code);
		}

		rtnMap.put("status", "success");
		rtnMap.put("message", "success");
		return rtnMap;
	}
	
	@Override
	public Map<String, Object> getProductLogList() throws Exception {
		Map<String, Object> rtnMap = new HashMap<>();

		LoginVO loginVO = (LoginVO) EgovUserDetailsHelper.getAuthenticatedUser();
		String userId = loginVO.getId();

		List<AgentProductLogDTO> result = logDAO.selectListProductLog(userId);
		result.stream().forEach(dto -> dto.convertUTCtoUserTime());
		
		rtnMap.put("status", "success");
		rtnMap.put("message", "success");
		rtnMap.put("data", result);
		return rtnMap;
	}
}
