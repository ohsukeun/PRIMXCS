package egovframework.com.primx.fun.file.service;

import java.io.Serializable;
import java.util.Date;

import egovframework.com.utl.fcc.service.EgovFormBasedFileVo;
import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@Builder
@AllArgsConstructor
@NoArgsConstructor
public class FileVO implements Serializable {
    private static final long serialVersionUID = 1L;

    /** ID */
    private int fileId;

    /** 파일 그룹 ID */
    private int fileGrpId;

    /** 파일명 */
    private String fileNm;

    /** 파일 확장자 */
    private String fileExt;

    /** 파일 크기 */
    private long fileSize;

    /** 다운로드 카운트 */
    private int downCnt;

    /** 저장 이름 */
    private String storedNm;

    /** 저장 경로 */
    private String storedPath;

    /** URL */
    private String url;

    /** 다운로드 URL */
    private String downloadUrl;

    /** 삭제 URL */
    private String deleteUrl;

    /** 삭제 여부 */
    private String isDeleted;

    /** 생성 날짜 */
    private Date createdDate;

    /** 파일 원본명 */
    private String fileOriginalNm;

	public void copyFrom(String uploadDir, EgovFormBasedFileVo egovFormBasedFileVo) {
        // fileNm은 확장자를 제거
		this.fileNm = egovFormBasedFileVo.getPhysicalName().substring(0, egovFormBasedFileVo.getPhysicalName().lastIndexOf("."));
		this.storedNm = egovFormBasedFileVo.getPhysicalName();
		this.fileOriginalNm = egovFormBasedFileVo.getFileName();
		this.fileExt = egovFormBasedFileVo.getFileName().substring(egovFormBasedFileVo.getFileName().lastIndexOf("."));
		this.storedPath = uploadDir + egovFormBasedFileVo.getFileName();
		this.fileSize = egovFormBasedFileVo.getSize();
	}
    
}
