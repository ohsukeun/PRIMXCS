package egovframework.com.primx.fun.sys.service;

import egovframework.com.primx.fun.cmm.service.DataTableBaseVO;
import egovframework.com.primx.fun.cmm.util.DateUtil;
import lombok.Data;

/**
 * @Class Name : ExServerInfoVO.java
 * @Description : 외부 다운로드 서버 정보 테이블 (T_EX_SERVER_INFO)
 * <pre>
 * 주요 컬럼:
 * - C_SEQ_NO : ID
 * - C_SERVER_NAME : 외부 서버의 이름(관리용)
 * - C_SERVER_URL : 업데이트 파일 다운로드 URL
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
public class ExServerInfoVO extends DataTableBaseVO {
    private static final long serialVersionUID = 1L;

    /** ID */
    private Long seqNo;

    /** 외부 서버의 이름(관리용) */
    private String serverName;

    /** 업데이트 파일 다운로드 URL */
    private String serverUrl;

    /** 생성 일자 */
    private String insertDate;

    /** 수정 일자 */
    private String updateDate;

	public void convertUTCtoUserTime() {
		this.insertDate = DateUtil.convertUTCtoUserTime(this.insertDate);
		this.updateDate = DateUtil.convertUTCtoUserTime(this.updateDate);
	}
}
