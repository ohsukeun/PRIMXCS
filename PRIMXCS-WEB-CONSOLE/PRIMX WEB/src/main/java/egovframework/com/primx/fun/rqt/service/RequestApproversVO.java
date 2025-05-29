package egovframework.com.primx.fun.rqt.service;

import java.io.Serializable;

import egovframework.com.primx.fun.cmm.util.DateUtil;
import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * @Class Name : RequestApproversVO.java
 * @Description : 신청 결재 테이블 (T_REQUEST_APPROVERS)
 * <pre>
 * 주요 컬럼:
 * - id : ID
 * - request_id : 신청 ID
 * - C_USER_ID : 사용자 ID
 * - order_number : 결재 순서
 * - result_cd : 결과 코드
 * - description : 설명
 * - result_date : 결과 날짜
 * - is_auto_rejected : 자동으로 반려 여부
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
@Builder
@AllArgsConstructor
@NoArgsConstructor
public class RequestApproversVO implements Serializable {
    private static final long serialVersionUID = 1L;

    /** ID */
    private int id;

    /** 신청 ID */
    private int requestId;

    /** 신청 CODE */
    private String requestCode;

    /** 사용자 ID */
    private String userId;

    /** 결재 순서 */
    private int orderNumber;

    /** 결재 결과 코드 AP : 결재 승인, RE : 결재 반려 */
    private String resultCd;

    /** 설명 */
    private String description;

    /** 결과 날짜 */
    private String resultDate;

    /** 자동으로 반려 여부 */
    private String isAutoRejected;

    /** 결재 상태 0 : 결재 비대상, 1 : 결재 대상, 2 : 결재 완료 */
    private int approvalStatus;

    /** 결재 위임 여부(전결 0: 미위임, 1: 위임) */
    private int delegatedFlag;

    public void convertUTCtoUserTime() {
        this.resultDate = DateUtil.convertUTCtoUserTime(this.resultDate);
    }

    public void convertUserTimeToUTC() {
        this.resultDate = DateUtil.convertUserTimeToUTC(this.resultDate);
    }

}
