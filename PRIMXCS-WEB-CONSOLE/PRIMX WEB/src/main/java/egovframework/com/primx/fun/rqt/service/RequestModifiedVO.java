package egovframework.com.primx.fun.rqt.service;

import java.io.Serializable;
import java.util.List;

import lombok.Data;

/**
 * 신청 및 결재 > 신청 정보 수정 VO
 * @author 장기훈
 *
 */
@Data
public class RequestModifiedVO implements Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 6855994563124497426L;
	
	private long requestId;
	private String requestCode;
	private String statusCd;
	private String title;
	private String reason;
	private String modifiedBy;
	private String modifiedIp;
	private boolean apprChg = false;
	private List<RequestApproversVO> approvalLines;
}
