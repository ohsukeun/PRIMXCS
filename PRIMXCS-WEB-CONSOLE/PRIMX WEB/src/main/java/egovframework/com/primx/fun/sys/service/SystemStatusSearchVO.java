package egovframework.com.primx.fun.sys.service;

import egovframework.com.primx.fun.cmm.service.DataTableBaseVO;
import egovframework.com.primx.fun.cmm.util.DateUtil;
import lombok.Data;

/**
 * 현황 > 시스템 현황 검색 VO
 * @author 장기훈
 *
 */
@Data
public class SystemStatusSearchVO extends DataTableBaseVO {

	/**
	 * 
	 */
	private static final long serialVersionUID = 3873218388357182423L;

	// 검색 시작일
	private String startDt;
	// 검색 종료일
	private String endDt;
	// 검색 미접속 기간(7d: 7일이상, 1m: 1개월 이상, 3m: 3개월이상, 6m: 6개월 이상)
	private String lastKaDateType;
	// 검색 시스템 종류(Y: 공용, N: 사용자)
	private String commonSystemYn;
	
	// 검색 설치 제품 명(1: ZONECENTRAL, 2: ORIZON)
	private int productId;
	// 검색 설치 제품 명(ZONECENTRAL, ORIZON)
	private String productName;
	// 검색 타입(신청자: user, 제목: title, 부서: dept)
	private String searchType;
	// 검색 문자열
	private String searchWord;
	// 검색 정책 코드
	private String policyCode;
	// 검색 정책 코드
	private String agentPolicyCode;

	public void convertUTCtoUserTime() {
		this.startDt = DateUtil.convertUTCtoUserTime(this.startDt);
		this.endDt = DateUtil.convertUTCtoUserTime(this.endDt);
	}

	public void convertUserTimeToUTC() {
		this.startDt = DateUtil.convertUserTimeToUTC(this.startDt);
		this.endDt = DateUtil.convertUserTimeToUTC(this.endDt);
	}
}
