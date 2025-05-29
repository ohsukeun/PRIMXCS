package egovframework.com.primx.fun.env.service;

import java.io.Serializable;

import lombok.Data;

/**
 * @Class Name : EnvSystemSettingsVO.java
 * @Description : 라이선스 업로드 시 시스템의 MAC 주소 등록 테이블 (T_ENV_SYSTEM_SETTINGS)
 * <pre>
 * 주요 컬럼:
 * - id : ID
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
public class EnvSystemSettingsVO implements Serializable {
    private static final long serialVersionUID = 1L;

    /** ID */
    private int id;

    /** MAC address */
    private String macAddress;
    
}
