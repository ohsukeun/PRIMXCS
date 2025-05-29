package egovframework.com.primx.fun.rqt.service;

import java.io.Serializable;

import egovframework.com.primx.fun.cmm.util.DateUtil;
import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * 신청 및 결재 > 신청 목록 DTO
 * @author 장기훈
 *
 */

@Data
@Builder
@AllArgsConstructor
@NoArgsConstructor
public class RequestResultDTO implements Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = -3470319527737787679L;
	private long requestId;
	private String userId;
	private String requestApprovalCode;
	private String requestCode;
	/**
	 * 결재 상태 코드 
	 * DR : 결재 진행중
	 * CO : 결재 완료
	 * RE : 결재 반려
	 */
	private String statusCd;
	/**
	 * 결재 결과 코드
	 * AP : 결재 승인
	 * RE : 결재 반려
	 */
	private String resultCd;
	private String userNm;
	private String title;

	private String reason;
	private String createdDate;
	private String modifiedDate;
	private long fileGroupId;
	private String resultDate;
	private String deptName;
	private String deptId;
	private String userRole;

	public void convertUTCtoUserTime() {
		this.createdDate = DateUtil.convertUTCtoUserTime(this.createdDate);
		this.modifiedDate = DateUtil.convertUTCtoUserTime(this.modifiedDate);
		this.resultDate = DateUtil.convertUTCtoUserTime(this.resultDate);
	}
}
