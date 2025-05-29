package egovframework.com.primx.fun.file.service;

import java.io.Serializable;

import egovframework.com.primx.fun.cmm.util.DateUtil;
import lombok.Data;

/**
 * @Class Name : ExFileAuthLogVO.java
 * @Description : 이동파일 인증 로그 테이블 (T_EX_FILE_AUTH_LOG)
 * <pre>
 * 주요 컬럼:
 * - C_SEQ_NO : ID
 * - C_LOG_TIME : 로그 발생 시각
 * - C_FILE_KEY : 파일 고유키
 * - C_RESULT_CODE : 결과 코드
 * - C_USER_ID : 이동파일 생성 사용자 ID
 * - C_USER_NAME : 이동파일 생성 사용자 이름
 * - C_AGENT_ID : Agent ID
 * - C_REQUEST_IP : 이동파일 인증 요청 PC IP 주소
 * - C_REQUEST_MAC : 이동파일 인증 요청 PC MAC 주소
 * - C_FILE_NAME : 이동파일 이름
 * - C_FILE_SIZE : 이동파일 크기
 * - C_FILE_HASH : 이동파일 해시
 * - C_DECRYPT_CNT : 복호화 남은 횟수
 * - C_EXPIRE_DATE : 복호화 만료일
 * - C_COMMENT : 상세설명
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
public class ExFileAuthLogVO implements Serializable {
    private static final long serialVersionUID = 1L;

    /** ID */
    private long seqNo;

    /** 로그 발생 시각 */
    private String logTime;

    /** 파일 고유키 */
    private String fileKey;

    /** 결과 코드 */
    private String resultCode;

    /** 이동파일 생성 사용자 ID */
    private String userId;

    /** 이동파일 생성 사용자 이름 */
    private String userName;

    /** Agent ID */
    private String agentId;

    /** 이동파일 인증 요청 PC IP 주소 */
    private String requestIp;

    /** 이동파일 인증 요청 PC MAC 주소 */
    private String requestMac;

    /** 이동파일 이름 */
    private String fileName;

    /** 이동파일 크기 */
    private long fileSize;

    /** 이동파일 해시 */
    private String fileHash;

    /** 복호화 남은 횟수 */
    private int decryptCnt;

    /** 복호화 만료일 */
    private String expireDate;

    /** 상세설명 */
    private String comment;

    public void convertUTCtoUserTime() {
        this.logTime = DateUtil.convertUTCtoUserTime(this.logTime);
        this.expireDate = DateUtil.convertUTCtoUserTime(this.expireDate);
    }
}
