package egovframework.com.primx.fun.pol.service;

import egovframework.com.cmm.ComDefaultVO;
import egovframework.com.primx.fun.cmm.util.DateUtil;
import lombok.Builder;
import lombok.Data;
import lombok.AllArgsConstructor;
import lombok.NoArgsConstructor;

@Data
@Builder
@AllArgsConstructor
@NoArgsConstructor
public class AgentPolicyMasterVO extends ComDefaultVO {
    private static final long serialVersionUID = 1L;

    /** 정책 ID */
    private int policyId;

    /** 정책 이름 */
    private String policyName;

    /** 등록자 */
    private String insertedBy;

    /** 에이전트 정책 코드 */
    private String agentPolicyCode;

    /** 기본 정책 여부 */
    private String defaultPolicyYn;

    /** 반출 파일 생성 권한 */
    private int exportFileAuth;

    /** 반출 신청 결재 권한 */
    private int webManagerAuth;

    /** 비 암호화 영역 설정 권한 */
    private int notEncryptionPathAuth;

    /** PRIMX 제품의 빠른 메뉴 표시 여부 */
    private int productMenuEnable;

    /** 사용자 콘솔 메뉴 표시 여부 */
    private int webManagerConsoleMenuEnable;

    /** 이동파일 생성 메뉴 표시 여부 */
    private int exportFileMenuEnable;

    /** ZONECENTRAL 메뉴 표시 여부 */
    private int ztryMenuEnable;

    /** ZONECENTRAL 오픈 영역 표시 여부 */
    private int zopenArea;

    /** ZONECENTRAL 개인 접근 표시 여부 */
    private int zpersonalAccess;

    /** ZONECENTRAL 옵션 표시 여부 */
    private int zoptionDisplay;

    /** ZONECENTRAL SafetyZone 표시 여부 */
    private int zsafetyZone;

    /** ZONECENTRAL 모든 영역 닫기 표시 여부 */
    private int zareaClose;

    /** ZONECENTRAL 암호화 확인 표시 여부 */
    private int zenc;

    /** 생성 일자 */
    private String insertDate;

    /** 수정 일자 */
    private String updateDate;

	/** 적용 개수 */
	private int systemMapCount;

	public void convertUTCtoUserTime() {
		this.insertDate = DateUtil.convertUTCtoUserTime(this.insertDate);
		this.updateDate = DateUtil.convertUTCtoUserTime(this.updateDate);
	}

	public void convertUserTimeToUTC() {
		this.insertDate = DateUtil.convertUserTimeToUTC(this.insertDate);
		this.updateDate = DateUtil.convertUserTimeToUTC(this.updateDate);
	}
}
