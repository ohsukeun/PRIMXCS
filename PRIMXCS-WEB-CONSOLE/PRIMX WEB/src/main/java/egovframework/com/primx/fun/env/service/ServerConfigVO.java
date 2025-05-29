package egovframework.com.primx.fun.env.service;

import java.io.Serializable;

import egovframework.com.primx.fun.cmm.util.DateUtil;
import lombok.Data;

/**
 * @Class Name : ServerConfigVO.java
 * @Description : 시스템 환경설정 정보 테이블 (T_SERVER_CONFIG)
 * <pre>
 * 주요 컬럼:
 * - C_SEQ_NO : ID
 * - C_KEY_NAME : 키 이름
 * - C_VALUE_DATA : 설정 값
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
 *</pre>
 *
 *  @author 김영우
 *  @since 2024.12.03
 *  @version 1.0
 *  @see
 *  
 */
@Data
public class ServerConfigVO implements Serializable {
    private static final long serialVersionUID = 1L;

    /** ID */
    private long seqNo;

    /** 키 이름 */
    private String keyName;

    /** 설정 값 */
    private String valueData;

    /** 생성 일자 */
    private String insertDate;

    /** 수정 일자 */
    private String updateDate;

	public void convertUTCtoUserTime() {
		this.insertDate = DateUtil.convertUTCtoUserTime(this.insertDate);
		this.updateDate = DateUtil.convertUTCtoUserTime(this.updateDate);
	}

	public void convertUserTimeToUTC() {
		this.insertDate = DateUtil.convertUserTimeToUTC(this.insertDate);
		this.updateDate = DateUtil.convertUserTimeToUTC(this.updateDate);
	}
}
