package egovframework.com.primx.fun.rqt.service.impl;

import java.sql.SQLException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.stereotype.Repository;

import egovframework.com.cmm.service.impl.EgovComAbstractDAO;
import egovframework.com.primx.fun.rqt.service.RequestApproverResultDTO;
import egovframework.com.primx.fun.rqt.service.RequestApproversVO;
import egovframework.com.primx.fun.rqt.service.RequestDTO;
import egovframework.com.primx.fun.rqt.service.RequestModifiedVO;
import egovframework.com.primx.fun.rqt.service.RequestResultDTO;
import egovframework.com.primx.fun.rqt.service.RequestSearchVO;
import egovframework.com.primx.fun.rqt.service.RequestUserApprovalDTO;
import egovframework.com.primx.fun.rqt.service.NoticeSettingVO;


@Repository("requestDAO")
public class RequestDAO extends EgovComAbstractDAO {

	public int selectListRequestInfoCnt(RequestSearchVO requestSearchVO, String userId, String auth) throws SQLException {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("requestSearchVO", requestSearchVO);
		paramMap.put("userId", userId);
		paramMap.put("auth", auth);
		return selectOne("requestDAO.selectListRequestInfoCnt", paramMap);
	}

	public List<RequestResultDTO> selectListRequestInfo(RequestSearchVO requestSearchVO, String userId, String auth) throws SQLException {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("requestSearchVO", requestSearchVO);
		paramMap.put("userId", userId);
		paramMap.put("auth", auth);
		return selectList("requestDAO.selectListRequestInfo", paramMap);
	}

	public RequestResultDTO selectRequestInfo(int requestId) throws SQLException {
		return selectOne("requestDAO.selectRequestInfo", requestId);
	}

	public RequestResultDTO selectRequestInfoByRequestCode(String requestCode) throws SQLException {
		return selectOne("requestDAO.selectRequestInfoByRequestCode", requestCode);
	}

	public List<RequestApproverResultDTO> selectListRequestApprover(String requestCode) throws SQLException {
		return selectList("requestDAO.selectListRequestApprover", requestCode);
	}

	public List<RequestApproverResultDTO> selectListRequestApproverByApprovalStatus(String requestCode) throws SQLException {
		return selectList("requestDAO.selectListRequestApproverByApprovalStatus", requestCode);
	}

	public int updateRequestInfo(RequestModifiedVO requestModifiedVO) throws SQLException {
		return update("requestDAO.updateRequestInfo", requestModifiedVO);
	}

	public int selectListRequestApprovalInfoCnt(RequestSearchVO requestSearchVO, String userId, String auth) throws SQLException {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("requestSearchVO", requestSearchVO);
		paramMap.put("userId", userId);
		paramMap.put("auth", auth);
		return selectOne("requestDAO.selectListRequestApprovalInfoV2Cnt", paramMap);
	}

	public List<RequestResultDTO> selectListRequestApprovalInfo(RequestSearchVO requestSearchVO, String userId, String auth) throws SQLException {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("requestSearchVO", requestSearchVO);
		paramMap.put("userId", userId);
		paramMap.put("auth", auth);
		return selectList("requestDAO.selectListRequestApprovalInfoV2", paramMap);
	}

	public int updateRequestApprovalInfo(RequestApproversVO requestApproversVO) throws SQLException {
		return update("requestDAO.updateRequestApprovalInfo", requestApproversVO);
	}

	public int updateRequestApprovalDelegatedInfo(RequestApproversVO requestApproversVO) throws SQLException {
		return update("requestDAO.updateRequestApprovalDelegatedInfo", requestApproversVO);
	}

	public int updateRequestResult(RequestResultDTO requestResultDTO) {
		return update("requestDAO.updateRequestResult", requestResultDTO);
	}

	public int insertUserApproval(RequestUserApprovalDTO requestUserApprovalDTO) throws SQLException {
		return insert("requestDAO.insertUserApproval", requestUserApprovalDTO);
	}


	public int insertRequestApprover(RequestApproversVO approvalLine) throws SQLException {
		return insert("requestDAO.insertRequestApprover", approvalLine);
	}

	public int insertRequest(RequestDTO request) throws SQLException {
		return insert("requestDAO.insertRequest", request);
	}
	
	public int getNextRequestSeq() throws SQLException {
		return selectOne("requestDAO.getNextRequestSeq");
	}

	public int updateRequest(RequestDTO request) throws SQLException {
		return update("requestDAO.updateRequest", request);
	}

	public int deleteRequestApprover(String requestCode) throws SQLException {
		return delete("requestDAO.deleteRequestApprover", requestCode);
	}

	public RequestUserApprovalDTO selectRequestUserApproval(String requestCode) throws SQLException {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("requestCode", requestCode);
		return selectOne("requestDAO.selectRequestUserApproval", paramMap);
	}

	public List<String> selectListRequestApprovalById(String userId, String auth) throws SQLException {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("userId", userId);
		paramMap.put("auth", auth);
		return selectList("requestDAO.selectListRequestApprovalById", paramMap);
	}

	public List<RequestApproversVO> selectListRequestApprovalByUserId(String userId) throws SQLException {
		return selectList("requestDAO.selectListRequestApprovalByUserId", userId);
	}

	public int selectRequestApproverCnt(String requestCode) throws SQLException {
		return selectOne("requestDAO.selectRequestApproverCnt", requestCode);
	}

	public int updateApprovalStatus(RequestApproversVO requestApproversVO) throws SQLException {
		return update("requestDAO.updateApprovalStatus", requestApproversVO);
	}

	public List<NoticeSettingVO> selectListCreateRequestSetting() throws SQLException {
		return selectList("requestDAO.selectListCreateRequestSetting");
	}

	public int updateCreateRequestSetting(NoticeSettingVO noticeSettingVO) throws SQLException {
		return update("requestDAO.updateCreateRequestSetting", noticeSettingVO);
	}
}
