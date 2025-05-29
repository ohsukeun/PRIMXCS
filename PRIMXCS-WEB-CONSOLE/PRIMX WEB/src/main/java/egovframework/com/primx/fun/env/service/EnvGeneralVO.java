package egovframework.com.primx.fun.env.service;

import java.io.Serializable;

import lombok.Data;

/**
 * @Class Name : EnvGeneralVO.java
 * @Description : 환경 일반 정보 테이블 (T_ENV_GENERAL)
 * <pre>
 * 주요 컬럼:
 * - id : ID
 * - company_name : 회사 이름
 * - admin_console_version : 관리자 콘솔 버전
 * - user_console_version : 사용자 콘솔 버전
 * - database_version : 데이터베이스 버전
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
public class EnvGeneralVO implements Serializable {
    private static final long serialVersionUID = 1L;

    /** ID */
    private int id;

    /** 회사 이름 */
    private String companyName;

    /** 관리자 콘솔 버전 */
    private String adminConsoleVersion;

    /** 사용자 콘솔 버전 */
    private String userConsoleVersion;

    /** 데이터베이스 버전 */
    private String databaseVersion;

    /** 회사 ID */
    private String companyId;   
}
