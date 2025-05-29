package egovframework.com.primx.fun.pol.service.impl;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.annotation.Resource;
import javax.transaction.Transactional;

import org.springframework.stereotype.Service;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import java.io.File;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import javax.xml.parsers.*;
import javax.xml.transform.*;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import java.io.StringWriter;

import egovframework.com.cmm.LoginVO;
import egovframework.com.cmm.service.EgovProperties;
import egovframework.com.cmm.util.EgovUserDetailsHelper;
import egovframework.com.primx.fun.dpt.service.DeptSearchVO;
import egovframework.com.primx.fun.dpt.service.DeptVO;
import egovframework.com.primx.fun.dpt.service.impl.DepartmentDAO;
import egovframework.com.primx.fun.pol.service.DistributionPolicySearchDTO;
import egovframework.com.primx.fun.pol.service.PolicyDetailInfoVO;
import egovframework.com.primx.fun.pol.service.PolicyVO;
import egovframework.com.primx.fun.pol.service.PrimxPolicyService;
import egovframework.com.primx.fun.pol.service.ProductPolicySearchDTO;
import egovframework.com.primx.fun.pol.service.ReleasePolicyVO;
import egovframework.com.primx.fun.sys.service.ProductsVO;
import egovframework.com.primx.fun.sys.service.impl.SystemDAO;
import egovframework.com.primx.fun.usr.service.impl.UserDAO;

@Service("primxPolicyService")
public class PrimxPolicyServiceImpl implements PrimxPolicyService {

	@Resource(name = "policyDAO")
	private PolicyDAO policyDAO;

	@Resource(name = "systemDAO")
	private SystemDAO systemDAO;

	@Resource(name = "userDAO")
	private UserDAO userDAO;

	@Resource(name = "departmentDAO")
	private DepartmentDAO departmentDAO;
	
	@Override
	public Map<String, Object> loadProductPolicyList(ProductPolicySearchDTO searchDTO) throws Exception {
		Map<String, Object> result = new HashMap<>();
		List<PolicyVO> policyList = policyDAO.selectPolicyList(searchDTO);
		for(PolicyVO policy : policyList) {
			policy.convertUTCtoUserTime();
		}
		int totalCnt = policyDAO.selectPolicyListCnt(searchDTO);

		result.put("resultList", policyList);
		result.put("recordsTotal", totalCnt);
		result.put("recordsFiltered", totalCnt);
		result.put("status", "success");
		result.put("message", "success");
		return result;
	}

	@Override
	public Map<String, Object> loadDefaultPolicyList() throws Exception {
		Map<String, Object> result = new HashMap<>();
		List<PolicyDetailInfoVO> policyList = policyDAO.selectDefaultPolicyList();
		for(PolicyDetailInfoVO policy : policyList) {
			policy.convertUTCtoUserTime();
		}
		result.put("resultList", policyList);
		result.put("status", "success");
		result.put("message", "success");
		return result;
	}
	
	@Override
	public Map<String, Object> loadMandatoryPolicyList() throws Exception {
		Map<String, Object> result = new HashMap<>();
		List<PolicyDetailInfoVO> policyList = policyDAO.selectMandatoryPolicyList();
		for(PolicyDetailInfoVO policy : policyList) {
			policy.convertUTCtoUserTime();
		}
		result.put("resultList", policyList);
		result.put("status", "success");
		result.put("message", "success");
		return result;
	}

	@Override
	@Transactional
	public Map<String, Object> saveDefaultPolicyList(List<PolicyDetailInfoVO> resultList) throws Exception {
		Map<String, Object> result = new HashMap<>();
		
		if (resultList == null || resultList.isEmpty()) {
			result.put("status", "error");
			result.put("message", "저장할 정책 데이터가 없습니다.");
			return result;
		}

		try {
			int successCount = 0;
			StringBuilder errorMessages = new StringBuilder();

		for (PolicyDetailInfoVO policy : resultList) {
			try {
				int updateCount = policyDAO.updateDefaultPolicy(policy);
				
				if (updateCount > 0) {
					successCount++;
				} else {
					errorMessages.append("정책 ID ").append(policy.getPolId())
							   .append(" 업데이트 실패. ");
				}
			} catch (Exception e) {
				errorMessages.append("정책 ID ").append(policy.getPolId())
						   .append(" 처리 중 오류 발생: ").append(e.getMessage()).append(". ");
			}
		}

		// 모든 정책이 성공적으로 업데이트된 경우에만 성공 처리
		if (successCount == resultList.size()) {
			result.put("status", "success");
			result.put("message", "success");
			return result;
		} else {
			// 실패 시 예외를 던져 트랜잭션 롤백
			String errorMessage = errorMessages.toString();
			result.put("status", "error");
			result.put("message", errorMessage);
				throw new RuntimeException(errorMessage);  // 이 예외는 컨트롤러에서 처리됨
			}
		} catch (Exception e) {
			result.put("status", "error");
			result.put("message", e.getMessage());
			throw e;
		}
	}

	@Override
	public Map<String, Object> getPolicyInfo(String policyCode) throws Exception {
		Map<String, Object> result = new HashMap<>();

		PolicyVO policy = policyDAO.selectPolicyByCode(policyCode);
		List<PolicyDetailInfoVO> policyDetailList = policyDAO.selectPolicyDetailList(policyCode);
		for(PolicyDetailInfoVO policyDetail : policyDetailList) {
			policyDetail.convertUTCtoUserTime();
		}

		result.put("policy", policy);
		result.put("policyDetailList", policyDetailList);
		result.put("status", "success");
		result.put("message", "success");
		return result;
	}

	@Override
	@Transactional
	public Map<String, Object> saveProductPolicy(String policyName, String productName, List<PolicyDetailInfoVO> copyPolicyData) throws Exception {
		Map<String, Object> result = new HashMap<>();
		
		String xmlContent = makePolicyXml(copyPolicyData, productName);
		String policyCode = savePolicyXml(xmlContent, "POL-", productName, null);

		LoginVO user = (LoginVO) EgovUserDetailsHelper.getAuthenticatedUser();
		int createdBy = userDAO.selectUserSeqById(user.getId());
		
		ProductsVO product = systemDAO.selectProductInfo(productName);

		// policyCode .xml 확장자 제거
		policyCode = policyCode.replace(".xml", "");

		// 정책 저장
		PolicyVO policy = PolicyVO.builder()
			.policyCode(policyCode)
			.policyName(policyName)
			.policyXml(xmlContent.getBytes())
			.productId(product.getId().intValue())
			.createdBy(createdBy)
			.build();
		policyDAO.insertPolicy(policy);

		// List<PolicyDetailInfoVO> 저장
		for (PolicyDetailInfoVO policyDetail : copyPolicyData) {
			policyDetail.setPolCode(policyCode);
			policyDAO.insertPolicyDetail(policyDetail);
		}

		result.put("policyCode", policyCode);
		result.put("policyName", policyName);
		result.put("status", "success");
		result.put("message", "success");
		return result;
	}

	@Override
	@Transactional
	public Map<String, Object> updateProductPolicy(String policyCode, String policyName, String productName, List<PolicyDetailInfoVO> copyPolicyData) throws Exception {
		Map<String, Object> result = new HashMap<>();
		
		String xmlContent = makePolicyXml(copyPolicyData, productName);
		String fileName = savePolicyXml(xmlContent, "POL-", productName, policyCode);
		System.out.println("fileName: " + fileName);

		LoginVO user = (LoginVO) EgovUserDetailsHelper.getAuthenticatedUser();
		int modifiedBy = userDAO.selectUserSeqById(user.getId());
		
		ProductsVO product = systemDAO.selectProductInfo(productName);


		// 정책 저장
		PolicyVO policy = PolicyVO.builder()
			.policyCode(policyCode)
			.policyName(policyName)
			.policyXml(xmlContent.getBytes())
			.productId(product.getId().intValue())
			.modifiedBy(modifiedBy)
			.build();
		policyDAO.updatePolicy(policy);

		PolicyDetailInfoVO existingPolicyDetail = policyDAO.selectPolicyDetailByCodeAndId(policyCode);
		if (existingPolicyDetail == null) {
			existingPolicyDetail = PolicyDetailInfoVO.builder().build();
		}

		// 해제되는 정책에 대해서 일괄처리하기 위해 모든 정책을 삭제하고 신규로 넣는다
		policyDAO.deletePolicyDetail(policyCode);

		// List<PolicyDetailInfoVO> 저장
		for (PolicyDetailInfoVO policyDetail : copyPolicyData) {
			policyDetail.setPolCode(policyCode);
			policyDetail.setCreatedDate(existingPolicyDetail.getCreatedDate());
			policyDAO.updatePolicyDetail(policyDetail);
		}

		result.put("policyCode", policyCode);
		result.put("policyName", policyName);
		result.put("status", "success");
		result.put("message", "success");
		return result;
	}

	
	private String cleanXmlString(String xml) {
		if (xml == null) return null;
		// XML에서 허용되지 않는 제어 문자 제거
		return xml.replaceAll("[\\x00-\\x1F\\x7F]", "")  // 제어 문자 제거
				 .replaceAll("\\s+", " ")  // 연속된 공백을 하나로
				 .trim();  // 앞뒤 공백 제거
	}

	private String generateFileName(String targetPath, String prefix, String fileName) {
		String dateStr = new SimpleDateFormat("yyyyMMdd").format(new Date());
		String baseFileName = prefix + dateStr;
		
		// 해당 경로의 모든 파일 목록을 가져옴
		File dir = new File(targetPath);
		File[] files = dir.listFiles((d, name) -> name.startsWith(baseFileName));
		
		int maxSeq = 0;
		if (files != null) {
			for (File file : files) {
				try {
					// 파일명에서 시퀀스 번호 추출 (POL-2025021700001.xml -> 00001)
					String seqStr = file.getName().substring(baseFileName.length(), baseFileName.length() + 5);
					int seq = Integer.parseInt(seqStr);
					maxSeq = Math.max(maxSeq, seq);
				} catch (Exception e) {
					// 파일명 형식이 맞지 않는 경우 무시
					continue;
				}
			}
		}
		
		// 다음 시퀀스 번호 생성
		String seqNum = String.format("%05d", maxSeq + 1);
		if(fileName == null || fileName.isEmpty()) {
			return baseFileName + seqNum + ".xml";
		} else {
			return fileName + ".xml";
		}
	}



	public String makePolicyXml(List<PolicyDetailInfoVO> policyDetailList, String productName) throws Exception {
		String policiesPath = EgovProperties.getProperty("PRIMX.policies.path");
		String protectionPolPath = EgovProperties.getProperty("PRIMX.policies.product.protection");
		String protectionPath = policiesPath + productName + protectionPolPath;
		
		String firstFileName = EgovProperties.getProperty("PRIMX.protection.fileName");
		String fileExtension = EgovProperties.getProperty("PRIMX.protection.fileExtension");
		String fileName = firstFileName + productName + fileExtension;
		
		System.out.println("XML 생성 시작");
		System.out.println("원본 경로: " + protectionPath);
		System.out.println("원본 파일명: " + fileName);
		
		// XML 파일 읽기
		File originalFile = new File(protectionPath, fileName);
		DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
		DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
		
		Document originalDoc;
		Element rootElement;
		
		if (originalFile.exists()) {
			System.out.println("기존 XML 파일 발견. 파일을 읽어옵니다.");
			// 기존 파일이 있으면 읽어옴
			originalDoc = dBuilder.parse(originalFile);
			rootElement = originalDoc.getDocumentElement();
			
			// policies 태그 찾아서 제거 (나중에 새로 만들 예정)
			NodeList policiesNodes = rootElement.getElementsByTagName("policies");
			if (policiesNodes.getLength() > 0) {
				System.out.println("기존 policies 태그를 제거합니다.");
				rootElement.removeChild(policiesNodes.item(0));
			}
		} else {
			System.out.println("기존 " + fileName + " 파일이 없습니다. ");
			throw new Exception("기존 " + fileName + " 파일이 없습니다. ");
		}
		
		// policies 태그 생성 및 정책 데이터 추가
		System.out.println("policies 태그를 생성하고 정책 데이터를 추가합니다.");
		Element policiesElement = originalDoc.createElement("policies");
		rootElement.appendChild(policiesElement);
		
		// 각 정책의 polData를 policies 태그 안에 추가
		if (policyDetailList != null && !policyDetailList.isEmpty()) {
			for (PolicyDetailInfoVO policy : policyDetailList) {
				if (policy.getPolData() != null && !policy.getPolData().isEmpty()) {
					try {
						// XML 데이터 정제
						String cleanXml = cleanXmlString(policy.getPolData());
						System.out.println("정책 ID " + policy.getPolId() + "의 정제된 XML:\n" + cleanXml);
						
						// polData를 파싱하여 policies 태그에 추가
						Document policyDoc = dBuilder.parse(new org.xml.sax.InputSource(new java.io.StringReader(cleanXml)));
						Element policyElement = policyDoc.getDocumentElement();
						Node importedNode = originalDoc.importNode(policyElement, true);
						policiesElement.appendChild(importedNode);
						
						System.out.println("정책 ID " + policy.getPolId() + "의 데이터가 추가되었습니다.");
					} catch (Exception e) {
						System.out.println("정책 ID " + policy.getPolId() + " 처리 중 오류 발생: " + e.getMessage());
						e.printStackTrace();
					}
				}
			}
		}
		
		// XML 문자열로 변환
		TransformerFactory tf = TransformerFactory.newInstance();
		Transformer transformer = tf.newTransformer();
		transformer.setOutputProperty(OutputKeys.OMIT_XML_DECLARATION, "yes");  // XML 선언부 제거
		transformer.setOutputProperty(OutputKeys.ENCODING, "utf-16");
		transformer.setOutputProperty(OutputKeys.INDENT, "yes");
		transformer.setOutputProperty("{http://xml.apache.org/xslt}indent-amount", "2");
		
		StringWriter writer = new StringWriter();
		transformer.transform(new DOMSource(originalDoc), new StreamResult(writer));
		
		String xmlString = writer.toString();
		System.out.println("\n=== 생성된 XML 내용 ===\n" + xmlString + "\n==================");
		
		return xmlString;
	}

	public String savePolicyXml(String xmlContent, String prefix, String productName, String fileName) throws Exception {
		String policiesPath = EgovProperties.getProperty("PRIMX.policies.path");
		String policyPath = EgovProperties.getProperty("PRIMX.policies.product.policy");
		String targetPath = policiesPath + productName + policyPath;

		
		if (prefix == null || prefix.isEmpty()) {
			prefix = "POL-"; // 기본값 설정
		}
		
		System.out.println("XML 파일 저장 시작");
		System.out.println("저장 경로: " + targetPath);
		
		// 새로운 파일명 생성
		String newFileName = generateFileName(targetPath, prefix, fileName);
		System.out.println("새로운 파일명: " + newFileName);
		
		// XML 파일로 저장
		File newFile = new File(targetPath, newFileName);
		
		// 문자열을 XML 문서로 파싱
		DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
		DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
		Document doc = dBuilder.parse(new org.xml.sax.InputSource(new java.io.StringReader(xmlContent)));
		
		// XML 파일 저장
		TransformerFactory transformerFactory = TransformerFactory.newInstance();
		Transformer transformer = transformerFactory.newTransformer();
		transformer.setOutputProperty(OutputKeys.ENCODING, "utf-16");
		transformer.setOutputProperty(OutputKeys.INDENT, "yes");
		transformer.setOutputProperty("{http://xml.apache.org/xslt}indent-amount", "2");
		
		DOMSource source = new DOMSource(doc);
		StreamResult result = new StreamResult(newFile);
		transformer.transform(source, result);
		
		System.out.println("XML 파일 저장이 완료되었습니다: " + newFileName);

		return newFileName;
	}

	@Override
	@Transactional
	public Map<String, Object> deletePolicy(String policyCode, String productName) throws Exception {
		Map<String, Object> result = new HashMap<>();

		String policiesPath = EgovProperties.getProperty("PRIMX.policies.path");
		String policyPath = EgovProperties.getProperty("PRIMX.policies.product.policy");
		String fileExtension = EgovProperties.getProperty("PRIMX.protection.fileExtension");
		String targetPath = policiesPath + productName + policyPath;

		// 수정할 정책 코드 가져오기 
		PolicyVO defaultPolicy = policyDAO.selectDefaultPolicy();
		String defaultPolicyCode = defaultPolicy.getPolicyCode();

		// T_POLICY_SYSTEM_MAP에 policyCode로 되어있는 데이터 defaultPolicyCode로 변경
		policyDAO.updatePolicySystemMap(policyCode, defaultPolicyCode);

		// T_POLICY_DETAIL_INFO에 policyCode로 되어있는 데이터 삭제
		policyDAO.deletePolicyDetail(policyCode);

		// 정책 삭제
		policyDAO.deletePolicy(policyCode);

		// 정책 경로에서 xml 파일 삭제제
		String fileName = targetPath + policyCode + fileExtension;
		File file = new File(fileName);
		if(file.exists()) {
			file.delete();
		}
		
		result.put("status", "success");
		result.put("message", "success");
		
		return result;
	}

	@Override
	@Transactional
	public Map<String, Object> applyDefaultPolicy(String policyCode) throws Exception {
		Map<String, Object> result = new HashMap<>();
		
		// 수정할 정책 코드 가져오기 
		PolicyVO defaultPolicy = policyDAO.selectDefaultPolicy();
		String defaultPolicyCode = defaultPolicy.getPolicyCode();

		// T_POLICY에 is_default 'N'으로 변경
		policyDAO.updateDefaultPolicyFlag(defaultPolicyCode, "N");

		// 선택한 정책의 is_default 'Y'로 변경
		policyDAO.updateDefaultPolicyFlag(policyCode, "Y");

		result.put("status", "success");
		result.put("message", "success");

		return result;
	}

	@Override
	@Transactional
	public Map<String, Object> deleteSystem(List<String> agentIdList) throws Exception {
		Map<String, Object> result = new HashMap<>();
		
		// 수정할 정책 코드 가져오기 
		PolicyVO defaultPolicy = policyDAO.selectDefaultPolicy();
		
		for (String agentId : agentIdList) {
			// 시스템 삭제
			policyDAO.updatePolicySystem(agentId, defaultPolicy.getPolicyCode());
		}

		result.put("status", "success");
		result.put("message", "success");
		return result;
	}

	@Override
	@Transactional
	public Map<String, Object> addSystem(List<String> agentIdList, String policyCode) throws Exception {
		Map<String, Object> result = new HashMap<>();

		for (String agentId : agentIdList) {
			// 시스템 정책 변경
			policyDAO.deleteSystem(agentId);
			policyDAO.addSystem(agentId, policyCode);
		}

		result.put("status", "success");
		result.put("message", "success");
		return result;
	}

	@Override
	@Transactional
	public Map<String, Object> changePolicy(String agentIdList, String policyCode) throws Exception {
		Map<String, Object> result = new HashMap<>();

		// agentIdList를 배열로 변환
		String[] agentIdArray = agentIdList.split(",");
		for (String agentId : agentIdArray) {
			System.out.println("agentId: " + agentId);
			System.out.println("policyCode: " + policyCode);
			// 시스템 정책 변경
			policyDAO.deleteSystem(agentId);
			policyDAO.addSystem(agentId, policyCode);
		}

		result.put("status", "success");
		result.put("message", "success");

		return result;
	}

	@Override
	public Map<String, Object> loadDistributionPolicyList(DistributionPolicySearchDTO searchDTO) throws Exception {
		Map<String, Object> result = new HashMap<>();

		DeptSearchVO searchVO = DeptSearchVO.builder()
			.deptId(searchDTO.getSearchDeptId())
			.includeSubDept(true)
			.build();
		// 하위 부서 리스트 조회
		List<DeptVO> deptList = departmentDAO.selectDeptTreeByDeptId(searchVO);
		List<String> deptIds = new ArrayList<>();
		for (DeptVO dept : deptList) {
			deptIds.add(dept.getDeptId());
		}

		searchDTO.setSearchProcessorArchitecture(9);
		if(searchDTO.getSearchDeploymentStatus() != null) {
			if(searchDTO.getSearchDeploymentStatus().equals("Y")) {
				searchDTO.setSearchDeploymentStatus("running");
			}
			else if(searchDTO.getSearchDeploymentStatus().equals("N")) {
				searchDTO.setSearchDeploymentStatus("stop");
			}
		}
		System.out.println("searchDTO.getSearchDeploymentStatus(): " + searchDTO.getSearchDeploymentStatus());
		System.out.println("searchDTO.getSearchDateType(): " + searchDTO.getSearchDateType());	
		System.out.println("searchDTO.getSearchStartDate(): " + searchDTO.getSearchStartDate());
		System.out.println("searchDTO.getSearchEndDate(): " + searchDTO.getSearchEndDate());
		System.out.println("searchDTO.getSearchDeptId(): " + searchDTO.getSearchDeptId());
		System.out.println("searchDTO.getSearchDeptName(): " + searchDTO.getSearchDeptName());
		System.out.println("searchDTO.getSearchProductName(): " + searchDTO.getSearchProductName());
		System.out.println("searchDTO.getSearchProcessorArchitecture(): " + searchDTO.getSearchProcessorArchitecture());
		
		List<ReleasePolicyVO> distributionPolicyList = policyDAO.selectDistributionPolicyList(searchDTO, deptIds);
		int totalCnt = policyDAO.selectDistributionPolicyListCnt(searchDTO, deptIds);
		
		result.put("draw", searchDTO.getDraw());
		result.put("recordsTotal", totalCnt);
		result.put("recordsFiltered", totalCnt);
		result.put("resultList", distributionPolicyList);
		result.put("status", "success");
		result.put("message", "success");
		return result;
	}

	@Override
	@Transactional
	public Map<String, Object> saveDistributionPolicy(String deptId, String beginDate, String endDate, String releaseYn, String includeSubDept) throws Exception {
		Map<String, Object> result = new HashMap<>();
		
		int insertCnt = 0;
		List<DeptVO> deptList = new ArrayList<>();
		if(includeSubDept.equals("Y")) {
			deptList = departmentDAO.selectIncludeSubDeptByDeptId(deptId);
		}
		else {
			DeptVO dept = DeptVO.builder()
				.deptId(deptId)
				.build();
			deptList.add(dept);
		}

		for (DeptVO dept : deptList) {
			
			// 부서 경로 조회
			String deptPath = departmentDAO.selectDeptPath(dept.getDeptId());
			// 부서경로가 안나오는 경우 회사 경로를 확인한다
			if(deptPath == null) {
				deptPath = departmentDAO.selectCompanyPath(dept.getDeptId());
			}
			// 회사도 없는 경우 에러처리
			if(deptPath == null) {
				result.put("status", "error");
				result.put("message", "부서 경로를 찾을 수 없습니다.");
				return result;
			}

			ReleasePolicyVO releasePolicy = policyDAO.selectDistributionPolicyByDeptId(dept.getDeptId());
			if(releasePolicy != null) {
				// 기존 정책 시작 종료 기간 변경
				insertCnt = policyDAO.updateDistributionPolicy(releasePolicy.getRegId(), beginDate, endDate, deptPath, releaseYn);
			}
			else {
				// 등록
				insertCnt = policyDAO.insertDistributionPolicy(dept.getDeptId(), beginDate, endDate, deptPath, releaseYn);
			}
			
		}

		if(insertCnt > 0) {
			result.put("status", "success");
			result.put("message", "success");
		} else {
			result.put("status", "error");
			result.put("message", "error");
		}

		return result;
	}

	@Override
	public Map<String, Object> loadIndividualPolicyList() throws Exception {
		Map<String, Object> result = new HashMap<>();

		List<PolicyVO> policyList = policyDAO.selectIndividualPolicyList();

		result.put("status", "success");
		result.put("message", "success");
		result.put("data", policyList);
		return result;
	}
}
