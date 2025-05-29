package egovframework.com.primx.fun.sys.service;

import java.io.Serializable;

import egovframework.com.primx.fun.cmm.util.DateUtil;
import lombok.Data;

/**
 * @Class Name : DiskInfoVO.java
 * @Description : 디스크 드라이브 테이블 (T_DISK_INFO)
 * <pre>
 * 주요 컬럼:
 * - C_SEQ_NO : ID
 * - C_AGENT_ID : Agent ID
 * - C_DISK_NAME : 디스크 이름
 * - C_DISK_TYPE : 디스크 종류
 * - C_VOLUMN_NAME : 볼륨명
 * - C_FILE_SYSTEM_NAME : 파일시스템
 * - C_TOTAL_SIZE : 전체 사이즈
 * - C_FREE_SIZE : 저장 가능 사이즈
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
public class DiskInfoVO implements Serializable {
    private static final long serialVersionUID = 1L;

    /** ID */
    private long seqNo;

    /** Agent ID */
    private String agentId;

    /** 디스크 이름 */
    private String diskName;

    /** 디스크 종류 */
    private int diskType;

    /** 볼륨명 */
    private String volumnName;

    /** 파일시스템 */
    private String fileSystemName;

    /** 전체 사이즈 */
    private long totalSize;

    /** 저장 가능 사이즈 */
    private long freeSize;

    /** 생성 일자 */
    private String insertDate;

    /** 수정 일자 */
    private String updateDate;

    public void convertUTCtoUserTime() {
        this.insertDate = DateUtil.convertUTCtoUserTime(this.insertDate);
        this.updateDate = DateUtil.convertUTCtoUserTime(this.updateDate);
    }
    
}
