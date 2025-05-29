package egovframework.com.primx.fun.lic.service;

import java.io.Serializable;

import lombok.Data;

/**
 * @Class Name : LicenseGeneralVO.java
 * @Description : 라이선스 테이블 (T_LICENSE_GENERAL)
 * <pre>
 * 주요 컬럼:
 * - id : ID
 * - company : 회사
 * - mac_address : MAC address
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
public class LicenseGeneralVO implements Serializable {
    private static final long serialVersionUID = 1L;

    /** ID */
    private int id;

    /** 회사 */
    private String company;

    /** MAC address */
    private String macAddress;
}
