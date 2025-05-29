package egovframework.com.primx.fun.sys.service;

import java.io.Serializable;

import egovframework.com.primx.fun.cmm.util.DateUtil;
import lombok.Data;

/**
 * 현황 > 시스템 현황 목록 DTO
 * @author 장기훈
 *
 */
@Data
public class SystemStatusResultDTO implements Serializable {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1616655949063613520L;
	// 시스템 일련번호(C_SEQ_NO)
	private long systemId;
	// Agent ID
	private String agentId;
	// PC 명
	private String computerName;
	// 정책 정보
	private String policys;
	// Agent 정책 정보
	private String agentPolicys;
	// 공용 시스템 여부(시스템 종류)
	private String commonSystemYn;
	// 작업그룹 명
	private String workgroupName;
	// Agent 버전 정보
	private String agentVersion;
	// Agent 정책 정보
	private String agentPolicyName;
	// Agent 정책 코드
	private String agentPolicyCode;
	// 정책 코드
	private String policyCode;
	// 정책 명
	private String policyName;
	// 윈도우 버전 정보
	private String winVersionString;
	// 윈도우 UUID
	private String winUuid;
	// OS 주 버전
	private int osMajorVersion;
	// OS 빌드 번호
	private String osBuildNumber;
	// OS 서비스팩 주 버전
	private int osServicepackMajor;
	// OS 서비스팩 부 버전
	private int osServicepackMinor;
	// 등록일시
	private String insertDate;
	// 최종 접속 일시
	private String lastKaDate;	
	// 사용자 명
	private String userNm;
	// 부서 명
	private String deptName;
	// 회사 코드
	private String companyId;
	// 사용자 시스템 정보 매칭 일련번호
	private long userSystemMatchSeqNo;
	// 기본 정책 여부
	private String defaultPolicyYn;
	// 수정일시
	private String modifiedDate;

	public void convertUTCtoUserTime() {
		this.insertDate = DateUtil.convertUTCtoUserTime(this.insertDate);
		this.lastKaDate = DateUtil.convertUTCtoUserTime(this.lastKaDate);
	}
}
