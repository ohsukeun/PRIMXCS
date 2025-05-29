package egovframework.com.primx.fun.lic.service;

import java.io.Serializable;

import egovframework.com.primx.fun.cmm.util.DateUtil;
import lombok.Data;

/**
 * @Class Name : LicenseDetailVO.java
 * @Description : 라이선스 등록 테이블 (T_LICENSE_DETAIL)
 * <pre>
 * 주요 컬럼:
 * - id : ID
 * - status_cd : 상태 코드
 * - product_name : 제품명
 * - quantity : 개수
 * - start_date : 시작 날짜
 * - end_date : 만료 날짜
 * - general_id : General ID
 * - product_key : 제품키
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
public class LicenseDetailVO implements Serializable {
    private static final long serialVersionUID = 1L;

    /** ID */
    private int id;

    /** 상태 코드 */
    private String statusCd;

    /** 제품명 */
    private String productName;

    /** 개수 */
    private int quantity;

    /** 시작 날짜 */
    private String startDate;

    /** 만료 날짜 */
    private String endDate;

    /** General ID */
    private int generalId;

    /** 제품키 */
    private String productKey;
   
    public void convertUTCtoUserTime() {
        this.startDate = DateUtil.convertUTCtoUserTime(this.startDate);
        this.endDate = DateUtil.convertUTCtoUserTime(this.endDate);
    }
}
