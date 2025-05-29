package egovframework.com.primx.fun.pol.service;

import java.io.Serializable;
import java.util.Date;

import lombok.Data;

/**
 * @Class Name : AgentPolicySystemMatchVO.java
 * @Description : Agent 정책 시스템 매칭 테이블 (T_AGENT_POLICY_SYSTEM_MATCH)
 * <pre>
 * 주요 컬럼:
 * - C_SEQ_NO : ID
 * - C_AGENT_POLICY_CODE : Agent 정책 Code
 * - C_AGENT_ID : Agent ID
 * - C_POLICY_TIMESTAMP : 정책 적용 시각
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
 *   2024.12.11        김영우     테이블 컬럼 수정(C_POLICY_ID -> C_AGENT_POLICY_CODE)
 *</pre>
 *
 *  @author 김영우
 *  @since 2024.12.03
 *  @version 1.0
 *  @see
 *  
 */
@Data
public class AgentPolicySystemMatchVO implements Serializable {
    private static final long serialVersionUID = 1L;

    /** ID */
    private long seqNo;

    /** Agent 정책 Code */
    private String agentPolicyCode;

    /** Agent ID */
    private String agentId;

    /** 정책 적용 시각 */
    private Date policyTimestamp;

    /** 생성 일자 */
    private Date insertDate;

    /** 수정 일자 */
    private Date updateDate;

    /** 조인된 정책 정보 (AgentPolicyMasterVO) */
    private AgentPolicyMasterVO agentPolicyMaster;
    
}
