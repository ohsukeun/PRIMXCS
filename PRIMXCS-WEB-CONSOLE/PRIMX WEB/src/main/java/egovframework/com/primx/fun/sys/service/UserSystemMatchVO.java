package egovframework.com.primx.fun.sys.service;

import java.io.Serializable;
import java.util.Date;

import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * @Class Name : UserSystemMatchVO.java
 * @Description : 사용자 시스템 정보 매칭 테이블 (T_USER_SYSTEM_MATCH)
 * <pre>
 * 주요 컬럼:
 * - C_SEQ_NO : ID
 * - C_COMPANY_ID : 회사 ID
 * - C_USER_ID : 사용자 ID
 * - C_AGENT_ID : Agent ID
 * - C_USER_REG_DATE : 사용자 등록 일자
 * - C_MODIFIED_BY : 수정자
 * - C_MODIFIED_IP : 수정 IP
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
@Builder
@AllArgsConstructor
@NoArgsConstructor
public class UserSystemMatchVO implements Serializable {
    private static final long serialVersionUID = 1L;

    /** ID */
    private long seqNo;

    /** 회사 ID */
    private String companyId;

    /** 사용자 ID */
    private String userId;

    /** Agent ID */
    private String agentId;

    /** 사용자 등록 일자 */
    private Date userRegDate;

    /** 수정자 */
    private String modifiedBy;

    /** 수정 IP */
    private String modifiedIp;

    /** 생성 일자 */
    private Date insertDate;

    /** 수정 일자 */
    private Date updateDate;

    
}
