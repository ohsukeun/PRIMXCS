package egovframework.com.primx.fun.sys.service;

import java.io.Serializable;
import java.util.Date;

import lombok.Data;

/**
 * @Class Name : UpdateInfoVO.java
 * @Description : PRIMX 제품 업데이트 정보 테이블 (T_UPDATE_INFO)
 * <pre>
 * 주요 컬럼:
 * - C_SEQ_NO : ID
 * - C_PRODUCT_TYPE : 제품 종류
 * - C_PROCESSOR_ARCHITECTURE : OS 프로세서 아키텍처
 * - C_PRODUCT_VERSION : 업데이트 제품 버전
 * - C_UPDATE_FILE_HASH : 업데이트 파일 해시
 * - C_EX_SERVER_USE : 외부 다운로드 서버 사용 여부
 * - C_UPDATE_FILE_PATH : 업데이트 파일 경로
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
 *   2024.12.04        김영우     최초 생성
 *</pre>
 *
 *  @author 김영우
 *  @since 2024.12.04
 *  @version 1.0
 *  @see
 *  
 */
@Data
public class UpdateInfoVO implements Serializable {
    private static final long serialVersionUID = 1L;

    /** ID */
    private Long seqNo;

    /** 제품 종류 */
    private Long productType;

	/** 파일 이름 */
	private String fileName;

    /** OS 프로세서 아키텍처 */
    private Long processorArchitecture;

    /** 업데이트 제품 버전 */
    private String productVersion;

    /** 업데이트 파일 해시 */
    private String updateFileHash;
	
	/** 업데이트 파일 경로 */
	private String updateFilePath;

    /** 외부 다운로드 서버 사용 여부 */
    private int exServerUse;

    /** 생성 일자 */
    private Date insertDate;

    /** 수정 일자 */
    private Date updateDate;

}
