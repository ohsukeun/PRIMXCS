package egovframework.com.primx.fun.log.service;

import egovframework.com.primx.fun.cmm.service.DataTableBaseVO;
import egovframework.com.primx.fun.cmm.util.DateUtil;
import lombok.Data;

/**
 * @author 김영우
 *
 */
@Data
public class ManageLogEntityDTO extends DataTableBaseVO {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	// 컬럼명 그대로 사용
	private String userId;
	private String userIp;
	private String agentId;
	private String agentIp;
	private String productPolicyId;
	private String productPolicyName;
	private String agentPolicyId;
	private String agentPolicyName;
	private String reserved1;
	private String reserved2;
	private String reserved3;
	private String reserved4;
	private String reserved5;
	private String reserved6;

	// AS로 명시된 컬럼
	private Long seqNo; // tml.C_SEQ_NO as seqNo
	private String logTypeName; // trlt.C_LOG_TYPE_NAME
	private String logName; // trml.C_LOG_NAME as logName
	private String message; // trml.C_MESSAGE as message
	private String columnGuide; // trml.C_COLUMN_GUIDE as columnGuide
	private String computerName; // tml.C_COMPUTER_NAME as computerName
	private String userName; // tum.C_USER_NAME as userName
	private String deptName; // cdm.DEPT_NAME as deptName
	private String manageName; // tmm.C_MANAGER_NAME as manageName
	private String logDate; // tml.C_LOG_TIME as logTime
	private String logTime; // tml.C_LOG_TIME as logTime

	public void convertUTCtoUserTime() {
		this.logDate = DateUtil.convertUTCtoUserTime(this.logDate);
		this.logTime = DateUtil.convertUTCtoUserTime(this.logTime);
	}
}
