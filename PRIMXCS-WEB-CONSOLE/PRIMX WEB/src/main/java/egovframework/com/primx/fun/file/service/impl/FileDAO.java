package egovframework.com.primx.fun.file.service.impl;

import java.sql.SQLException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.stereotype.Repository;

import egovframework.com.cmm.service.impl.EgovComAbstractDAO;
import egovframework.com.primx.fun.file.service.ExArchivedFileInfoVO;
import egovframework.com.primx.fun.file.service.ExFileAuthLogVO;
import egovframework.com.primx.fun.file.service.FileVO;
import egovframework.com.primx.fun.file.service.MoveFileInfoDTO;
import egovframework.com.primx.fun.file.service.MoveFileSearchDTO;

@Repository("fileDAO")
public class FileDAO extends EgovComAbstractDAO {

	  public List<FileVO> selectByFileGrpIdAndIsDeleted(Long fileGrpId, String isDeleted) throws SQLException {

			Map<String, Object> paramMap = new HashMap<>();
			paramMap.put("fileGrpId", fileGrpId);
			paramMap.put("isDeleted", isDeleted);

			return selectList("fileDAO.selectByFileGrpIdAndIsDeleted", paramMap);
	  }
	  
	  public FileVO selectByFileNmAndIsDeleted(String fileNm, String isDeleted) {

			Map<String, Object> paramMap = new HashMap<>();
			paramMap.put("fileNm", fileNm);
			paramMap.put("isDeleted", isDeleted);

			return selectOne("fileDAO.selectByFileNmAndIsDeleted", paramMap);
	  }
	  
	public int insertUploadedFile(FileVO file) {
		return (Integer)insert("fileDAO.insertUploadedFile", file);
	} 

	public int selectLastFileId() {
		return (Integer)selectOne("fileDAO.selectLastFileId");
	}

	public int insertFileGrp() {
		return (Integer)insert("fileDAO.insertFileGrp");
	}

	public int selectLastFileGrpId() {
		return (Integer)selectOne("fileDAO.selectLastFileGrpId");
	}

	public List<FileVO> selectByFileGrpId(int fileGroupId) {
		Map<String, Object> paramMap = new HashMap<>();	
		paramMap.put("fileGroupId", fileGroupId);
		return selectList("fileDAO.selectByFileGrpId", paramMap);
	}

	public List<ExArchivedFileInfoVO> selectExArchivedFileInfoByFileGrpId(long fileGroupId) {
		Map<String, Object> paramMap = new HashMap<>();	
		paramMap.put("fileGroupId", fileGroupId);
		return selectList("fileDAO.selectExArchivedFileInfoByFileGrpId", paramMap);
	}

	public List<MoveFileInfoDTO> selectMoveFileList(MoveFileSearchDTO searchDTO, String userId, String auth) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("searchDTO", searchDTO);
		paramMap.put("userId", userId);
		paramMap.put("auth", auth);
		return selectList("fileDAO.selectMoveFileList", paramMap);
	}
	
	public int selectMoveFileListCnt(MoveFileSearchDTO searchDTO, String userId, String auth) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("searchDTO", searchDTO);
		paramMap.put("userId", userId);
		paramMap.put("auth", auth);
		return (Integer)selectOne("fileDAO.selectMoveFileListCnt", paramMap);
	}

	public List<MoveFileInfoDTO> selectOriginalFileInfo(String fileKey) {
		return selectList("fileDAO.selectOriginalFileInfo", fileKey);
	}

	public List<ExFileAuthLogVO> selectMoveFileUsageHistory(String fileKey) {
		return selectList("fileDAO.selectMoveFileUsageHistory", fileKey);
	}

	public List<MoveFileInfoDTO> selectMoveFileStatus() {
		return selectList("fileDAO.selectMoveFileStatus");
	}
}