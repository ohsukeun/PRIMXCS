package egovframework.com.primx.fun.pol.service;

import java.io.Serializable;
import java.util.Date;

import lombok.Data;

/**
 * @Class Name : PolicySystemMapVO.java
 * @Description : 시스템에 정책 적용 테이블 (T_POLICY_SYSTEM_MAP)
 * <pre>
 * 주요 컬럼:
 * - id : ID
 * - policy_code : 정책 코드
 * - agent_id : 시스템 ID
 * - modified_date : 수정일
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
public class PolicySystemMapVO implements Serializable {
    private static final long serialVersionUID = 1L;

    /** ID */
    private int id;

    /** 정책 코드 */
    private String policyCode;

    /** 시스템 ID */
    private String agentId;

    /** 수정일 */
    private Date modifiedDate;
    
}
