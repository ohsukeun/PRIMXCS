package egovframework.com.primx.fun.file.service;

import java.io.Serializable;

import egovframework.com.primx.fun.cmm.util.DateUtil;
import lombok.Data;

/**
 * @Class Name : ExArchivedFileInfoVO.java
 * @Description : 반출 원본 파일 정보 테이블 (T_EX_ARCHIVED_FILE_INFO)
 * <pre>
 * 주요 컬럼:
 * - C_SEQ_NO : ID
 * - C_FILE_KEY : 파일 고유키
 * - C_FILE_NAME : 원본 파일 이름
 * - C_FILE_SIZE : 원본 파일 사이즈
 * - C_INSERT_DATE : 생성 일자
 * - C_UPDATE_DATE : 수정 일자
 * </pre>
 * @Modification Information
 *
 *<pre>
 * << 개정이력(Modification Information) >>
 *
 *   수정일              수정자          수정내용
 *   ----------       --------  ---------------------------
 *   2024.12.03        김영우     최초 생성
 *</pre>
 *
 *  @author 김영우
 *  @since 2024.12.03
 *  @version 1.0
 *  @see
 *  
 */
@Data
public class ExArchivedFileInfoVO implements Serializable {
    private static final long serialVersionUID = 1L;

    /** ID */
    private long seqNo;

    /** 파일 고유키 */
    private String fileKey;

    /** 원본 파일 이름 */
    private String fileName;

    /** 원본 파일 사이즈 */
    private long fileSize;

    /** 생성 일자 */
    private String insertDate;

    /** 수정 일자 */
    private String updateDate;

    public void convertUTCtoUserTime() {
        this.insertDate = DateUtil.convertUTCtoUserTime(this.insertDate);
        this.updateDate = DateUtil.convertUTCtoUserTime(this.updateDate);
    }
}
