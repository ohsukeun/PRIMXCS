package egovframework.com.primx.fun.file.service;

import java.io.Serializable;
import java.util.Date;

import lombok.Data;

/**
 * @Class Name : ExFileAuthInfoVO.java
 * @Description : 반출파일 인증 정보 테이블 (T_EX_FILE_AUTH_INFO)
 * <pre>
 * 주요 컬럼:
 * - C_SEQ_NO : ID
 * - C_FILE_KEY : 파일 고유키
 * - C_COMPANY_ID : 회사 ID
 * - C_AGENT_ID : Agent ID
 * - C_USER_ID : 사용자 ID
 * - C_FILE_HASH : 파일 해시
 * - C_DECRYPT_CNT : 복호화 남은 횟수
 * - C_EXPIRE_DATE : 복호화 만료일
 * - C_FILE_NAME : 이동파일 이름
 * - C_FILE_SIZE : 이동파일 크기
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
public class ExFileAuthInfoVO implements Serializable {
    private static final long serialVersionUID = 1L;

    /** ID */
    private long seqNo;

    /** 파일 고유키 */
    private String fileKey;

    /** 회사 ID */
    private String companyId;

    /** Agent ID */
    private String agentId;

    /** 사용자 ID */
    private String userId;

    /** 파일 해시 */
    private String fileHash;

    /** 복호화 남은 횟수 */
    private int decryptCnt;

    /** 복호화 만료일 */
    private Date expireDate;

    /** 이동파일 이름 */
    private String fileName;

    /** 이동파일 크기 */
    private long fileSize;

    /** 생성 일자 */
    private Date insertDate;

    /** 수정 일자 */
    private Date updateDate;

}

