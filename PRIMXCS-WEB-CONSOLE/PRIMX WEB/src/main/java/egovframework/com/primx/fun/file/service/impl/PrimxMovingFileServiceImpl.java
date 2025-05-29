package egovframework.com.primx.fun.file.service.impl;

import javax.annotation.Resource;
import org.springframework.stereotype.Service;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import egovframework.com.cmm.util.EgovUserDetailsHelper;
import egovframework.com.primx.fun.file.service.MoveFileSearchDTO;
import egovframework.com.primx.fun.file.service.ExFileAuthLogVO;
import egovframework.com.primx.fun.file.service.MoveFileInfoDTO;
import egovframework.com.primx.fun.file.service.PrimxMovingFileService;
import egovframework.com.primx.fun.sys.service.impl.SystemDAO;
import egovframework.com.cmm.LoginVO;

@Service("primxMovingFileService")
public class PrimxMovingFileServiceImpl implements PrimxMovingFileService {

	@Resource(name = "fileDAO")
	private FileDAO fileDAO;

	@Resource(name = "systemDAO")
	private SystemDAO systemDAO;

	@Override
	public Map<String,Object> loadMoveFileHistory(MoveFileSearchDTO searchDTO) throws Exception {
		Map<String,Object> result = new HashMap<>();
		// 권한을 조회
		
		// System.out.println("#########################################################################");
		LoginVO loginVo = (LoginVO) EgovUserDetailsHelper.getAuthenticatedUser();
		System.out.println(loginVo.toString());
		List<String> authList = EgovUserDetailsHelper.getAuthorities();
		// for(String auth: authList) {
		// 	System.out.println("auth:"+ auth);
		// }
		// System.out.println("#########################################################################");
		
		List<MoveFileInfoDTO> moveFileInfoList = fileDAO.selectMoveFileList(searchDTO, loginVo.getId(), authList.get(0));	
		int totalCnt = fileDAO.selectMoveFileListCnt(searchDTO, loginVo.getId(), authList.get(0));
		
		for(MoveFileInfoDTO dto : moveFileInfoList) {
			dto.convertUTCtoUserTime();
		}
		
		result.put("page", searchDTO.getPageIndex());
		result.put("pageSize", searchDTO.getRecordCountPerPage());
		result.put("draw", searchDTO.getDraw());
		result.put("recordsTotal", totalCnt);
		result.put("recordsFiltered", totalCnt);
		result.put("data", moveFileInfoList);
		result.put("status", "success");
		return result;
	}

	@Override
	public Map<String,Object> loadOriginalFileInfo(String fileKey) throws Exception {
		Map<String,Object> result = new HashMap<>();
		List<MoveFileInfoDTO> originalFileInfoList = fileDAO.selectOriginalFileInfo(fileKey);	
		for(MoveFileInfoDTO dto : originalFileInfoList) {
			dto.convertUTCtoUserTime();
		}
		result.put("originalFileInfoList", originalFileInfoList);
		result.put("status", "success");
		return result;
	}

	@Override
	public Map<String,Object> loadMoveFileUsageHistory(String fileKey) throws Exception {
		Map<String,Object> result = new HashMap<>();
		List<ExFileAuthLogVO> moveFileUsageHistoryList = fileDAO.selectMoveFileUsageHistory(fileKey);	
		for(ExFileAuthLogVO dto : moveFileUsageHistoryList) {
			dto.convertUTCtoUserTime();
		}
		result.put("moveFileUsageHistoryList", moveFileUsageHistoryList);
		result.put("status", "success");
		return result;
	}
}

