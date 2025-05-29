package egovframework.com.primx.fun.file.service.impl;

import javax.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.multipart.MultipartHttpServletRequest;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import egovframework.com.utl.fcc.service.EgovFileUploadUtil;
import egovframework.com.utl.fcc.service.EgovFormBasedFileVo;
import egovframework.com.primx.fun.cmm.util.FileSizeCheck;
import egovframework.com.primx.fun.file.service.FileVO;
import egovframework.com.primx.fun.file.service.PrimxFileService;
import egovframework.com.primx.fun.sys.service.impl.SystemDAO;

@Service("primxFileService")
public class PrimxFileServiceImpl implements PrimxFileService {

	@Resource(name = "fileDAO")
	private FileDAO fileDAO;

	@Resource(name = "systemDAO")
	private SystemDAO systemDAO;

	private long maxFileSize = FileSizeCheck.getMaxFileSize();

	@Override
	@Transactional
	public Map<String,FileVO> uploadFiles(boolean isGroup, String uploadDir, String extWhiteList, MultipartHttpServletRequest mRequest) throws Exception {

		List<EgovFormBasedFileVo> fileList = EgovFileUploadUtil.uploadFilesExt(mRequest, uploadDir, maxFileSize, extWhiteList);
		System.out.println("################## fileList.size(): "+fileList.size());
		Map<String,FileVO> fileVOList = new HashMap<>();

		int fileGrpId = 0;
		if (isGroup) {
			System.out.println("################## isGroup: "+isGroup);
			fileDAO.insertFileGrp();
			fileGrpId = fileDAO.selectLastFileGrpId();
			System.out.println("################## fileGrpId: "+fileGrpId);
		}
		System.out.println("################## fileList: "+fileList);
		for (EgovFormBasedFileVo egovFormBasedFileVo : fileList) {
			System.out.println("################## egovFormBasedFileVo: "+egovFormBasedFileVo.toString());
			FileVO fileVO = FileVO.builder()
				.fileGrpId(fileGrpId)
				.build();
			fileVO.copyFrom(uploadDir, egovFormBasedFileVo);
			fileVOList.put(egovFormBasedFileVo.getFileName(), fileVO);
			System.out.println("################## fileVO: "+fileVO.toString());
			fileDAO.insertUploadedFile(fileVO);
			// 마지막 인서트 된 T_FILE의 file_id를 가져온다
			fileVO.setFileId(fileDAO.selectLastFileId());
		}
		
		return fileVOList;
	}
}
