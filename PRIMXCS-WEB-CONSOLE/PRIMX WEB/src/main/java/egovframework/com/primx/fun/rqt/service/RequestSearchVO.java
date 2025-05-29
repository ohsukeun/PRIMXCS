package egovframework.com.primx.fun.rqt.service;

import egovframework.com.primx.fun.cmm.service.DataTableBaseVO;
import egovframework.com.primx.fun.cmm.util.DateUtil;
import lombok.Data;

/**
 * 신청 및 결재 > 신청 검색 VO
 * @author 장기훈
 *
 */
@Data
public class RequestSearchVO extends DataTableBaseVO {

	/**
	 * 
	 */
	private static final long serialVersionUID = 3873218388357182423L;

	// 검색 시작일
	private String startDt;
	// 검색 종료일
	private String endDt;
	// 검색 상태
	private String statusCd;
	// 검색 결과
	private String resultCd;
	// 검색 타입(신청자: user, 제목: title, 부서: dept)
	private String searchType;
	// 검색 문자열
	private String searchWord;
	// 신청 분류
	private String requestApprovalCode;

	public void convertUTCtoUserTime() {
		this.startDt = DateUtil.convertUTCtoUserTime(this.startDt);
		this.endDt = DateUtil.convertUTCtoUserTime(this.endDt);
	}

	public void convertUserTimeToUTC() {
		this.startDt = DateUtil.convertUserTimeToUTC(this.startDt);
		this.endDt = DateUtil.convertUserTimeToUTC(this.endDt);
	}
}
