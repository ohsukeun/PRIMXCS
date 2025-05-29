package egovframework.com.primx.fun.rqt.service;

import java.io.Serializable;

import egovframework.com.primx.fun.cmm.util.DateUtil;
import lombok.Data;

/**
 * 신청 및 결재 > 결재 목록 DTO
 * @author 장기훈
 *
 */

@Data
public class RequestApproverResultDTO implements Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 518751690114783085L;

	private int requestId;
	private String requestCode;
	private String userNm;
	private String emplyrId;
	private int orderNumber;
	private String resultCd;
	private String description;
	private String resultDate;
	private String isAutoRejected;
	private String deptName;
	private String deptId;
	private int approvalStatus;
	private String userRole;

	public void convertUTCtoUserTime() {
		this.resultDate = DateUtil.convertUTCtoUserTime(this.resultDate);
	}
}
