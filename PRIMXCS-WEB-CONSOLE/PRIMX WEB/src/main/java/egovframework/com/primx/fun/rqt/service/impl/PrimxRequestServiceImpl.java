package egovframework.com.primx.fun.rqt.service.impl;

import java.sql.SQLException;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Optional;

import javax.annotation.Resource;

import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import egovframework.com.cmm.LoginVO;
import egovframework.com.cmm.util.EgovHttpRequestHelper;
import egovframework.com.cmm.util.EgovUserDetailsHelper;
import egovframework.com.primx.fun.cmm.service.AlarmVO;
import egovframework.com.primx.fun.cmm.service.impl.AlarmDAO;
import egovframework.com.primx.fun.dpt.service.impl.DepartmentDAO;
import egovframework.com.primx.fun.file.service.ExArchivedFileInfoVO;
import egovframework.com.primx.fun.file.service.FileVO;
import egovframework.com.primx.fun.file.service.impl.FileDAO;
import egovframework.com.primx.fun.rqt.service.PrimxRequestService;
import egovframework.com.primx.fun.rqt.service.RequestApproverResultDTO;
import egovframework.com.primx.fun.rqt.service.RequestApproversVO;
import egovframework.com.primx.fun.rqt.service.RequestDTO;
import egovframework.com.primx.fun.rqt.service.RequestModifiedVO;
import egovframework.com.primx.fun.rqt.service.RequestResultDTO;
import egovframework.com.primx.fun.rqt.service.RequestSearchVO;
import egovframework.com.utl.sim.service.EgovFileScrty;
import egovframework.com.primx.fun.rqt.service.RequestUserApprovalDTO;
import egovframework.com.primx.fun.usr.service.UserRegistrationDTO;
import egovframework.com.primx.fun.rqt.service.NoticeSettingVO;
import lombok.Synchronized;
import lombok.extern.slf4j.Slf4j;

@Slf4j
@Service("primxRequestService")
public class PrimxRequestServiceImpl implements PrimxRequestService {

	@Resource(name = "requestDAO")
	private RequestDAO requestDAO;

	@Resource(name = "fileDAO")
	private FileDAO fileDAO;

	@Resource(name = "departmentDAO")
	private DepartmentDAO departmentDAO;
	
    @Resource(name = "alarmDAO")
    private AlarmDAO alarmDAO;

	@Override
	public Map<String, Object> selectListRequestInfo(RequestSearchVO requestSearchVO) throws SQLException {
		Map<String, Object> rtnMap = new HashMap<String, Object>();

		// System.out.println("#########################################################################");
		LoginVO loginVo = (LoginVO) EgovUserDetailsHelper.getAuthenticatedUser();
		System.out.println(loginVo.toString());
		List<String> authList = EgovUserDetailsHelper.getAuthorities();
		// for(String auth: authList) {
		// 	System.out.println("auth:"+ auth);
		// }
		// System.out.println("#########################################################################");
		
		rtnMap.put("draw", requestSearchVO.getDraw());
		try {
			int totalCnt = requestDAO.selectListRequestInfoCnt(requestSearchVO, loginVo.getId(), authList.get(0));
			rtnMap.put("recordsTotal", totalCnt);
			rtnMap.put("recordsFiltered", totalCnt);
			List<RequestResultDTO> lists = requestDAO.selectListRequestInfo(requestSearchVO, loginVo.getId(), authList.get(0));
			
            for (RequestResultDTO dto : lists) {
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
	public Map<String, Object> selectRequestInfo(int requestId) throws SQLException {
		Map<String, Object> rtnMap = new HashMap<String, Object>();
		try {
			// 0. 내 로그인 아이디를 조회
			LoginVO loginVo = (LoginVO) EgovUserDetailsHelper.getAuthenticatedUser();
			String userId = loginVo.getId();
			rtnMap.put("approvalUserId", userId);
			
			// 1. request 정보 조회
			RequestResultDTO requestResultDTO = requestDAO.selectRequestInfo(requestId);
			rtnMap.put("request", requestResultDTO);

			if (requestResultDTO != null) {
				// 2. approval 정보 조회
				List<RequestApproverResultDTO> requestApproverResultDTO = requestDAO.selectListRequestApprover(requestResultDTO.getRequestCode());
				rtnMap.put("approvers", requestApproverResultDTO);
				

				if(!"M".equals(requestResultDTO.getRequestApprovalCode())) {
					// 4. 신청분류에 따라 T_REQUEST_USER_APPROVAL 테이블 조회
					RequestUserApprovalDTO requestUserApprovalDTO = requestDAO.selectRequestUserApproval(requestResultDTO.getRequestCode());
					
					// 회사명 조회
					String companyName = departmentDAO.selectDeptName(requestUserApprovalDTO.getCompanyId());
					requestUserApprovalDTO.setCompanyName(companyName);
					
					// 부서명 조회
					String deptName = departmentDAO.selectDeptName(requestUserApprovalDTO.getDeptId());
					requestUserApprovalDTO.setDeptName(deptName);
					
					rtnMap.put("requestUserApproval", requestUserApprovalDTO);
					System.out.println("requestUserApprovalDTO: " + requestUserApprovalDTO.toString());
				}
				else {
					// 3. 등록파일 정보 조회(isDeleted = N)
					List<FileVO> fileLists = fileDAO.selectByFileGrpIdAndIsDeleted(requestResultDTO.getFileGroupId(), "N");
					rtnMap.put("files", fileLists);
				}
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
	@Transactional
	@Synchronized
	public Map<String, Object> updateRequestInfo(RequestModifiedVO requestModifiedVO) throws SQLException {
		Map<String, Object> rtnMap = new HashMap<String, Object>();
		try {
			// 1. request 정보 조회
			RequestResultDTO requestResultDTO = requestDAO.selectRequestInfo((int) requestModifiedVO.getRequestId());
			if ("".equalsIgnoreCase(requestResultDTO.getResultCd()) || requestResultDTO.getResultCd() == null) {
				// 2. request 정보 수정
				LoginVO loginVo = (LoginVO) EgovUserDetailsHelper.getAuthenticatedUser();
				String clientIp = EgovHttpRequestHelper.getRequestIp();

				// 수정자 ID 세팅
				requestModifiedVO.setModifiedBy(loginVo.getId());
				// 수정자 IP 세팅
				requestModifiedVO.setModifiedIp(clientIp);

				requestDAO.updateRequestInfo(requestModifiedVO);
				
				// 4. 결재선 정보 저장
				if (requestModifiedVO.getApprovalLines() != null) {
					System.out.println("requestModifiedVO.getApprovalLines().size(): " + requestModifiedVO.getApprovalLines().size());
					System.out.println("삭제 실행 RequestCode: "+ requestModifiedVO.getRequestCode());
					// 기존 결재선을 전부 지우고 새로 생성한다
					requestDAO.deleteRequestApprover(requestModifiedVO.getRequestCode());

					// 내 결재선 처리
					int targetOrderNumber = 0;
					List<RequestApproversVO> approvalLines = requestModifiedVO.getApprovalLines();
					for(RequestApproversVO approvalLine : approvalLines) {
						System.out.println("approvalLine1: "+ approvalLine.toString());
						if(approvalLine.getUserId().equals(loginVo.getId())) {
							approvalLine.setRequestCode(requestModifiedVO.getRequestCode());
							approvalLine.setResultCd("AP");
							approvalLine.setApprovalStatus(2);
							targetOrderNumber = approvalLine.getOrderNumber() + 1;
							requestDAO.insertRequestApprover(approvalLine);
							break;
						}
					}

					System.out.println("targetOrderNumber: "+ targetOrderNumber);
					// 내 결재선 이후 결재선 처리
					for(int i = targetOrderNumber; i < approvalLines.size(); i++) {
						RequestApproversVO approvalLine = approvalLines.get(i);
						System.out.println("approvalLine2: "+ approvalLine.toString());
						if(approvalLine.getOrderNumber() == targetOrderNumber) {
							approvalLine.setApprovalStatus(1);
						}
						approvalLine.setRequestCode(requestModifiedVO.getRequestCode());
						requestDAO.insertRequestApprover(approvalLine);
					}
				}

				rtnMap.put("status", "success");
				rtnMap.put("message", "success");
			} else {
				rtnMap.put("status", "failed");
				rtnMap.put("message", "결재 진행중");
			}

		} catch (SQLException e) {
			rtnMap.put("status", "error");
			rtnMap.put("message", "error");
		}
		return rtnMap;
	}

	@Override
	public Map<String, Object> selectListRequestApprovalInfo(RequestSearchVO requestSearchVO) throws SQLException {
		Map<String, Object> rtnMap = new HashMap<String, Object>();
		
		// System.out.println("#########################################################################");
		LoginVO loginVo = (LoginVO) EgovUserDetailsHelper.getAuthenticatedUser();
		// System.out.println(loginVo.toString());
		List<String> authList = EgovUserDetailsHelper.getAuthorities();
		// for(String auth: authList) {
		// 	System.out.println("auth:"+ auth);
		// }
		// System.out.println("#########################################################################");
		
		rtnMap.put("draw", requestSearchVO.getDraw());
		try {

			// 결재선에 포함된 결재들을 조회
			// List<String> requestCodes = requestDAO.selectListRequestApprovalById(loginVo.getId(), authList.get(0));

			int totalCnt = requestDAO.selectListRequestApprovalInfoCnt(requestSearchVO, loginVo.getId(), authList.get(0));

			List<RequestResultDTO> lists = requestDAO.selectListRequestApprovalInfo(requestSearchVO, loginVo.getId(), authList.get(0));
			for(RequestResultDTO dto : lists) {
				dto.convertUTCtoUserTime();
			}
			rtnMap.put("recordsTotal", totalCnt);
			rtnMap.put("recordsFiltered", totalCnt);
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
	public Map<String, Object> selectRequestApprovalInfo(int requestId) throws SQLException {
		Map<String, Object> rtnMap = new HashMap<String, Object>();
		try {
			// 0. 내 로그인 아이디를 조회
			LoginVO loginVo = (LoginVO) EgovUserDetailsHelper.getAuthenticatedUser();
			String userId = loginVo.getId();
			rtnMap.put("approvalUserId", userId);
			// 1. request 정보 조회
			RequestResultDTO requestResultDTO = requestDAO.selectRequestInfo(requestId);
			requestResultDTO.convertUTCtoUserTime();
			rtnMap.put("request", requestResultDTO);

			if (requestResultDTO != null) {
				// 2. approval 정보 조회
				List<RequestApproverResultDTO> requestApproverResultDTO = requestDAO.selectListRequestApprover(requestResultDTO.getRequestCode());
				for(RequestApproverResultDTO dto : requestApproverResultDTO) {
					dto.convertUTCtoUserTime();
				}
				rtnMap.put("approvers", requestApproverResultDTO);
				
				if(!"M".equals(requestResultDTO.getRequestApprovalCode())) {
					// 4. 신청분류에 따라 T_REQUEST_USER_APPROVAL 테이블 조회
					RequestUserApprovalDTO requestUserApprovalDTO = requestDAO.selectRequestUserApproval(requestResultDTO.getRequestCode());
					requestUserApprovalDTO.convertUTCtoUserTime();

					// 회사명 조회
					String companyName = departmentDAO.selectDeptName(requestUserApprovalDTO.getCompanyId());
					requestUserApprovalDTO.setCompanyName(companyName);
					
					// 부서명 조회
					String deptName = departmentDAO.selectDeptName(requestUserApprovalDTO.getDeptId());
					requestUserApprovalDTO.setDeptName(deptName);
					
					rtnMap.put("requestUserApproval", requestUserApprovalDTO);
					System.out.println("requestUserApprovalDTO: " + requestUserApprovalDTO.toString());
				}
				else {
					// 3. 등록파일 정보 조회(isDeleted = N)
					// List<FileVO> fileLists = fileDAO.selectByFileGrpIdAndIsDeleted(requestResultDTO.getFileGroupId(), "N");
					List<ExArchivedFileInfoVO> fileLists = fileDAO.selectExArchivedFileInfoByFileGrpId(requestResultDTO.getFileGroupId());
					for(ExArchivedFileInfoVO dto : fileLists) {
						dto.convertUTCtoUserTime();
					}
					rtnMap.put("files", fileLists);
				}
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
	@Transactional
	@Synchronized
	public Map<String, Object> updateRequestApprovalInfo(RequestApproversVO requestApproversVO) throws Exception {
		Map<String, Object> rtnMap = new HashMap<String, Object>();

		// 1. request 정보 조회
		RequestResultDTO requestResultDTO = requestDAO.selectRequestInfo((int) requestApproversVO.getRequestId());
		// 2. request 정보 수정
		LoginVO loginVo = (LoginVO) EgovUserDetailsHelper.getAuthenticatedUser();

		// 결재자 ID 세팅
		requestApproversVO.setUserId(loginVo.getId());

		// 결재 위임 여부 체크
		if(requestApproversVO.getDelegatedFlag() == 0) {
			
			// 결재선 개수를 체크 requestCode로 전체 개수를 조회
			List<RequestApproverResultDTO> approvalLineDTOs = requestDAO.selectListRequestApprover(requestResultDTO.getRequestCode());
			
			int targetOrderNumber = 0;
			for (RequestApproverResultDTO approvalLine : approvalLineDTOs) {
				// 이 업데이트는 자신이 승인한 결과에 대한 업데이트
				if(approvalLine.getEmplyrId().equals(loginVo.getId())) {
					requestApproversVO.setApprovalStatus(2);
					targetOrderNumber = approvalLine.getOrderNumber() + 1;
					requestDAO.updateRequestApprovalInfo(requestApproversVO);
					break;
				}
			}
			if(requestApproversVO.getResultCd().equals("RE")) {
				// 결재 반려인 경우 결재 처리 종료
				RequestResultDTO updateRequestResultDTO = RequestResultDTO.builder()
					.requestId(requestResultDTO.getRequestId())
					.requestCode(requestResultDTO.getRequestCode())
					.resultCd("RE")
					.statusCd("CO")
					.build();
				requestDAO.updateRequestResult(updateRequestResultDTO);
				rtnMap.put("status", "success");
				rtnMap.put("message", "success");
				return rtnMap;
			}

			System.out.println("targetOrderNumber:"+ targetOrderNumber);
			if(approvalLineDTOs.size() > targetOrderNumber) {
				// 이 업데이트는 다음 대상을 위한 ApprovalStatus 업데이트
				RequestApproverResultDTO approvalLine = approvalLineDTOs.get(targetOrderNumber);
				RequestApproversVO updateRequestApproversVO = RequestApproversVO.builder()
					.userId(approvalLine.getEmplyrId())
					.requestCode(requestResultDTO.getRequestCode())
					.orderNumber(targetOrderNumber)
					.approvalStatus(1)
					.build();
				requestDAO.updateApprovalStatus(updateRequestApproversVO);
				// 상대 업데이트 이후 결재 진행중으로 변경
				RequestResultDTO updateRequestResultDTO = RequestResultDTO.builder()
					.requestId(requestResultDTO.getRequestId())
					.requestCode(requestResultDTO.getRequestCode())
					.statusCd("DR")
					.build();
				requestDAO.updateRequestResult(updateRequestResultDTO);

				// 알람 저장
				AlarmVO alarm = AlarmVO.builder()
					.userId(requestResultDTO.getUserId())
					.type(requestResultDTO.getRequestApprovalCode())
					.path("/primx/fun/rqt/requestApproval/view.do")
					.userRole("USER")
					.targetId(approvalLine.getEmplyrId())
					.build();
				alarmDAO.insertAlarm(alarm);
			}
			else {
				// 결재 성공처리
				RequestResultDTO updateRequestResultDTO = RequestResultDTO.builder()
					.requestId(requestResultDTO.getRequestId())
					.requestCode(requestResultDTO.getRequestCode())
					.resultCd("AP")
					.statusCd("CO")
					.build();
				requestDAO.updateRequestResult(updateRequestResultDTO);

				RequestUserApprovalDTO requestUserApprovalDTO = requestDAO.selectRequestUserApproval(requestResultDTO.getRequestCode());

				// 모든 결재가 완료되었기 때문에 분류에 따른 처리를 실행한다
				if("A".equals(requestResultDTO.getRequestApprovalCode())) {
					requestUserApprovalDTO.convertUserTimeToUTC();
					UserRegistrationDTO userRegistrationDTO = UserRegistrationDTO.builder()
						.emplyrId(requestUserApprovalDTO.getUserId())
						.userName(requestUserApprovalDTO.getUserName())
						.email(requestUserApprovalDTO.getEmail())
						.companyId(requestUserApprovalDTO.getCompanyId())
						.deptId(requestUserApprovalDTO.getDeptId())
						.userRole(requestUserApprovalDTO.getUserRole())
						.expiredDate(requestUserApprovalDTO.getExpiredDate())
						.pwdExpiredDate(requestUserApprovalDTO.getExpiredDate())
						.usePeriod("1")
						.build();
					// 사용자 가입을 진행
					rtnMap.put("order", "addUser");
					rtnMap.put("data", userRegistrationDTO);
				}
				else if("C".equals(requestResultDTO.getRequestApprovalCode())) {
					requestUserApprovalDTO.convertUserTimeToUTC();
					UserRegistrationDTO userRegistrationDTO = UserRegistrationDTO.builder()
						.emplyrId(requestResultDTO.getUserId())
						.expiredDate(requestUserApprovalDTO.getExpiredDate())
						.build();
					// 신청한 사용자의 ID가 들어가야 한다
					rtnMap.put("order", "setPassword");
					rtnMap.put("data", userRegistrationDTO);

				}
			}
		}
		else {
			// 결재 위임 처리

			List<RequestApproverResultDTO> approvalLineDTOs = requestDAO.selectListRequestApproverByApprovalStatus(requestResultDTO.getRequestCode());			
			for(RequestApproverResultDTO approvalLine : approvalLineDTOs) {
			// 결재 라인 성공 처리
				RequestApproversVO updateRequestApproversVO = RequestApproversVO.builder()
					.userId(approvalLine.getEmplyrId())
					.resultCd("AP")
					.requestCode(requestResultDTO.getRequestCode())
					.description(requestApproversVO.getDescription())
					.approvalStatus(2)
					.build();
				requestDAO.updateRequestApprovalDelegatedInfo(updateRequestApproversVO);

				// 알람처리
				AlarmVO alarm = AlarmVO.builder()
					.userId(requestResultDTO.getUserId())
					.type("MD")
					.path("/primx/fun/rqt/requestApproval/view.do")
					.userRole("USER")
					.targetId(approvalLine.getEmplyrId())
					.build();
				alarmDAO.insertAlarm(alarm);
			}

			// 결재 성공처리
			RequestResultDTO updateRequestResultDTO = RequestResultDTO.builder()
				.requestId(requestResultDTO.getRequestId())
				.requestCode(requestResultDTO.getRequestCode())
				.resultCd("AP")
				.statusCd("CO")
				.build();
			requestDAO.updateRequestResult(updateRequestResultDTO);

		}

		
		rtnMap.put("status", "success");
		rtnMap.put("message", "success");

		return rtnMap;
	}

	@Override
	@Transactional
	public Map<String, Object> saveRequest(RequestUserApprovalDTO requestUserApprovalDTO) throws Exception {
		System.out.println("=== saveRequest 시작 ===");
		requestUserApprovalDTO.convertUserTimeToUTC();
		System.out.println("전체 DTO: {"+ requestUserApprovalDTO.toString() +"}");

		Map<String, Object> rtnMap = new HashMap<>();
		
		// 1. request_code 생성 (R + yyyyMMdd + 4자리 순번)
		String today = new SimpleDateFormat("yyyyMMdd").format(new Date());
		int nextSeq = requestDAO.getNextRequestSeq();
		String requestCode = "R" + today + String.format("%04d", nextSeq);
		System.out.println("생성된 requestCode: {"+ requestCode +"}");
		
		// 2. t_request 테이블에 저장
		LoginVO loginVo = (LoginVO) EgovUserDetailsHelper.getAuthenticatedUser();
		String clientIp = EgovHttpRequestHelper.getRequestIp();
		
		RequestDTO request = new RequestDTO();
		request.setRequestCode(requestCode);
		request.setRequestApprovalCode(requestUserApprovalDTO.getRequestApprovalCode());
		request.setStatusCd("PE"); // 진행중으로 시작
		request.setTitle(requestUserApprovalDTO.getRegiestUserInfo().getRequestTitle());
		request.setReason(requestUserApprovalDTO.getRegiestUserInfo().getRequestReason());

		request.setCUserId(loginVo.getId());
		request.setCreatedIp(clientIp);
		
		System.out.println("저장할 Request 데이터: {"+ request.toString() +"}");
		
		// 이동파일 생성 신청인 경우 기존 신청 정보 업데이트
		int insertResult = 0;
		if("M".equals(requestUserApprovalDTO.getRequestApprovalCode())){
			request.setRequestCode(requestUserApprovalDTO.getRequestCode());
			request.setModifiedBy(loginVo.getId());
			request.setModifiedIp(clientIp);
			requestCode = requestUserApprovalDTO.getRequestCode();
			insertResult = requestDAO.updateRequest(request);
		}
		else {
			insertResult = requestDAO.insertRequest(request);
		}
		System.out.println("Request 저장 결과: {"+ insertResult +"}");

		// 비밀번호 설정
		if("A".equals(requestUserApprovalDTO.getRequestApprovalCode())) {
			String newpassword = requestUserApprovalDTO.getUserId();
			String enpassword = EgovFileScrty.encryptPassword(newpassword, requestUserApprovalDTO.getUserId());
			requestUserApprovalDTO.setUserPassword(enpassword);
		}
		else {
			// 필수값 없는경우 채우기
			requestUserApprovalDTO.setUserId("NON_"+requestCode);
			requestUserApprovalDTO.setEmail("NON_"+requestCode);
			requestUserApprovalDTO.setCompanyId("NON");
			requestUserApprovalDTO.setDeptId("NON");
			requestUserApprovalDTO.setUserPassword("NON");
			requestUserApprovalDTO.setUserName("NON");
		}

		// 3. 신청 형식에 따라 추가 처리
		if ("A".equals(requestUserApprovalDTO.getRequestApprovalCode()) || 
			"C".equals(requestUserApprovalDTO.getRequestApprovalCode())) {
			
			requestUserApprovalDTO.setRequestCode(requestCode);
			int userApprovalResult = requestDAO.insertUserApproval(requestUserApprovalDTO);
			System.out.println("UserApproval 저장 결과: {"+ userApprovalResult +"}");
		}

		
		// 4. 결재선 정보 저장
		if (requestUserApprovalDTO.getApprovalLines() != null) {


			// 이동파일 생성 신청인 경우 기존 결재선 삭제 후 생성
			if("M".equals(requestUserApprovalDTO.getRequestApprovalCode())) {
				requestDAO.deleteRequestApprover(requestCode);
			}

			// 내 결재선 처리
			int targetOrderNumber = 0;
			List<RequestApproversVO> approvalLines = requestUserApprovalDTO.getApprovalLines();

			for(RequestApproversVO approvalLine : approvalLines) {
				approvalLine.convertUserTimeToUTC();

				if(approvalLine.getUserId().equals(loginVo.getId())) {
					approvalLine.setRequestCode(requestCode);
					approvalLine.setResultCd("AP");
					approvalLine.setApprovalStatus(2);
					targetOrderNumber = approvalLine.getOrderNumber() + 1;
					requestDAO.insertRequestApprover(approvalLine);
					break;
				}
			}

			// 알람 플래그 다음 사용자에게만 적용
			boolean isAlarm = true;
			// 내 결재선 이후 결재선 처리
			for(int i = targetOrderNumber; i < approvalLines.size(); i++) {
				RequestApproversVO approvalLine = approvalLines.get(i);
				if(approvalLine.getOrderNumber() == targetOrderNumber) {
					approvalLine.setApprovalStatus(1);
				}
				approvalLine.setRequestCode(requestCode);
				requestDAO.insertRequestApprover(approvalLine);

				if(isAlarm) {
					// 알람 저장
					AlarmVO alarm = new AlarmVO();
					alarm.setUserId(loginVo.getId());
					alarm.setType(requestUserApprovalDTO.getRequestApprovalCode());
					alarm.setPath("/primx/fun/rqt/requestApproval/view.do");
					alarm.setUserRole("USER");
					alarm.setTargetId(approvalLine.getUserId());
					alarmDAO.insertAlarm(alarm);
					isAlarm = false;
				}
			}
		}
		
		rtnMap.put("status", "success");
		rtnMap.put("message", "저장되었습니다.");
		
		return rtnMap;
	}

	@Override
	public Map<String, Object> selectFileList(int fileGroupId) throws SQLException {
		List<ExArchivedFileInfoVO> fileLists = fileDAO.selectExArchivedFileInfoByFileGrpId(fileGroupId);
		Map<String, Object> rtnMap = new HashMap<>();
		rtnMap.put("data", fileLists);
		rtnMap.put("status", "success");
		rtnMap.put("message", "success");
		return rtnMap;
	}

	@Override
	public Map<String, Object> selectApprovalLine(String requestCode) throws SQLException {
		List<RequestApproverResultDTO> requestApproverResultDTO = requestDAO.selectListRequestApprover(requestCode);
		Map<String, Object> rtnMap = new HashMap<>();
		rtnMap.put("data", requestApproverResultDTO);
		rtnMap.put("status", "success");
		rtnMap.put("message", "success");
		return rtnMap;
	}

	@Override
	public Map<String, Object> getCreateRequestSettingList() throws SQLException {
		List<NoticeSettingVO> noticeSettings = requestDAO.selectListCreateRequestSetting();
		for(NoticeSettingVO noticeSetting : noticeSettings) {
			noticeSetting.convertUTCtoUserTime();
		}
		Map<String, Object> rtnMap = new HashMap<>();
		rtnMap.put("data", noticeSettings);
		rtnMap.put("status", "success");
		rtnMap.put("message", "success");
		return rtnMap;
	}

	@Override
	public Map<String, Object> saveCreateRequestSetting(NoticeSettingVO noticeSettingVO) throws SQLException {
		System.out.println("saveCreateRequestSetting 들어옴");
		Map<String, Object> rtnMap = new HashMap<>();
		try {
			System.out.println("saveCreateRequestSetting 저장 시작");
			System.out.println("noticeSettingVO: " + noticeSettingVO.toString());
			requestDAO.updateCreateRequestSetting(noticeSettingVO);
			System.out.println("saveCreateRequestSetting 저장 완료");
			rtnMap.put("status", "success");
			rtnMap.put("message", "저장되었습니다.");
		} catch (SQLException e) {
			rtnMap.put("status", "error");
			rtnMap.put("message", "저장 중 오류가 발생했습니다: " + e.getMessage());
		}
		return rtnMap;
	}

	@Override
	@Transactional
	public Map<String, Object> batchApproval(List<String> requestCodes, String description) throws Exception {
		
		Map<String, Object> rtnMap = new HashMap<>();
		ArrayList<UserRegistrationDTO> rqtList = new ArrayList<>();

		// 2. request 정보 수정
		LoginVO loginVo = (LoginVO) EgovUserDetailsHelper.getAuthenticatedUser();

		for(String requestCode : requestCodes) {

			RequestResultDTO requestResultDTO = requestDAO.selectRequestInfoByRequestCode(requestCode);
		
			// 결재선 개수를 체크 requestCode로 전체 개수를 조회
			List<RequestApproverResultDTO> approvalLineDTOs = requestDAO.selectListRequestApprover(requestResultDTO.getRequestCode());
			
			int targetOrderNumber = 0;

			// 첫 번째 결재자인 경우 처리
			Optional<RequestApproverResultDTO> firstApprover = approvalLineDTOs.stream()
				.filter(approvalLine -> approvalLine.getEmplyrId().equals(loginVo.getId()) && approvalLine.getApprovalStatus() == 1)
				.findFirst();
				
			if (firstApprover.isPresent()) {
				
				RequestApproversVO updateRequestApproversVO = RequestApproversVO.builder()
					.requestCode(requestResultDTO.getRequestCode())
					.resultCd("AP")
					.userId(loginVo.getId())
					.approvalStatus(2)
					.orderNumber(firstApprover.get().getOrderNumber())
					.description(description)
					.build();
					targetOrderNumber = firstApprover.get().getOrderNumber() + 1;

				requestDAO.updateRequestApprovalInfo(updateRequestApproversVO);
			}
			else {
				continue;
			}

			
			System.out.println("targetOrderNumber:"+ targetOrderNumber);
			if(approvalLineDTOs.size() > targetOrderNumber) {
				// 이 업데이트는 다음 대상을 위한 ApprovalStatus 업데이트
				RequestApproverResultDTO approvalLine = approvalLineDTOs.get(targetOrderNumber);
				RequestApproversVO updateRequestApproversVO = RequestApproversVO.builder()
					.userId(approvalLine.getEmplyrId())
					.requestCode(requestResultDTO.getRequestCode())
					.orderNumber(targetOrderNumber)
					.approvalStatus(1)
					.build();
				requestDAO.updateApprovalStatus(updateRequestApproversVO);
				// 상대 업데이트 이후 결재 진행중으로 변경
				RequestResultDTO updateRequestResultDTO = RequestResultDTO.builder()
					.requestId(requestResultDTO.getRequestId())
					.requestCode(requestResultDTO.getRequestCode())
					.statusCd("DR")
					.build();
				requestDAO.updateRequestResult(updateRequestResultDTO);

				// 알람 저장
				AlarmVO alarm = AlarmVO.builder()
					.userId(requestResultDTO.getUserId())
					.type(requestResultDTO.getRequestApprovalCode())
					.path("/primx/fun/rqt/requestApproval/view.do")
					.userRole("USER")
					.targetId(approvalLine.getEmplyrId())
					.build();
				alarmDAO.insertAlarm(alarm);
			}
			else {
				// 결재 성공처리
				RequestResultDTO updateRequestResultDTO = RequestResultDTO.builder()
					.requestId(requestResultDTO.getRequestId())
					.requestCode(requestResultDTO.getRequestCode())
					.resultCd("AP")
					.statusCd("CO")
					.build();
				requestDAO.updateRequestResult(updateRequestResultDTO);

				RequestUserApprovalDTO requestUserApprovalDTO = requestDAO.selectRequestUserApproval(requestResultDTO.getRequestCode());

				// 모든 결재가 완료되었기 때문에 분류에 따른 처리를 실행한다
				if("A".equals(requestResultDTO.getRequestApprovalCode())) {
					requestUserApprovalDTO.convertUserTimeToUTC();
					UserRegistrationDTO userRegistrationDTO = UserRegistrationDTO.builder()
						.approvalCode("A")
						.emplyrId(requestUserApprovalDTO.getUserId())
						.userName(requestUserApprovalDTO.getUserName())
						.email(requestUserApprovalDTO.getEmail())
						.companyId(requestUserApprovalDTO.getCompanyId())
						.deptId(requestUserApprovalDTO.getDeptId())
						.userRole(requestUserApprovalDTO.getUserRole())
						.expiredDate(requestUserApprovalDTO.getExpiredDate())
						.pwdExpiredDate(requestUserApprovalDTO.getExpiredDate())
						.usePeriod("1")
						.build();
					// 사용자 가입을 진행
					rqtList.add(userRegistrationDTO);
				}
				else if("C".equals(requestResultDTO.getRequestApprovalCode())) {
					requestUserApprovalDTO.convertUserTimeToUTC();
					UserRegistrationDTO userRegistrationDTO = UserRegistrationDTO.builder()
						.approvalCode("C")
						.emplyrId(requestResultDTO.getUserId())
						.expiredDate(requestUserApprovalDTO.getExpiredDate())
						.build();
					// 사용자 계정 연장 진행
					rqtList.add(userRegistrationDTO);
				}
			}
		}
		rtnMap.put("rqtList", rqtList);

		
		rtnMap.put("status", "success");
		rtnMap.put("message", "success");
		return rtnMap;
	}
}
