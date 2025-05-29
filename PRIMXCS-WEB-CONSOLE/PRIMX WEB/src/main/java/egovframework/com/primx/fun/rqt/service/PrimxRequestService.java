package egovframework.com.primx.fun.rqt.service;

import java.sql.SQLException;
import java.util.List;
import java.util.Map;

public interface PrimxRequestService {

	Map<String, Object> selectListRequestInfo(RequestSearchVO requestSearchVO) throws SQLException;

	Map<String, Object> selectRequestInfo(int requestId) throws SQLException;

	Map<String, Object> updateRequestInfo(RequestModifiedVO requestModifiedVO) throws SQLException;

	Map<String, Object> selectListRequestApprovalInfo(RequestSearchVO requestSearchVO) throws SQLException;

	Map<String, Object> selectRequestApprovalInfo(int requestId) throws SQLException;

	Map<String, Object> updateRequestApprovalInfo(RequestApproversVO requestApproversVO) throws Exception;

	Map<String, Object> saveRequest(RequestUserApprovalDTO requestUserApprovalDTO) throws Exception;

	Map<String, Object> selectFileList(int fileGroupId) throws SQLException;

	Map<String, Object> selectApprovalLine(String requestCode) throws SQLException;

	Map<String, Object> getCreateRequestSettingList() throws SQLException;

	Map<String, Object> saveCreateRequestSetting(NoticeSettingVO noticeSettingVO) throws SQLException;

	Map<String, Object> batchApproval(List<String> requestCodes, String description) throws Exception;
}
