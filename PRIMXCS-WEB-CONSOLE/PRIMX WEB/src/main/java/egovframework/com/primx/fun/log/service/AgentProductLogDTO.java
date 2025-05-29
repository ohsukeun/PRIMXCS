package egovframework.com.primx.fun.log.service;

import egovframework.com.primx.fun.cmm.util.DateUtil;
import lombok.Data;

@Data
public class AgentProductLogDTO {

	private static final long serialVersionUID = 1L;

	/** 로그 순번 */
    private long seqNo;

    /** 제품 이름 */
    private String productName;

    /** 로그 레벨 */
    private String logLevel;

    /** 로그 이름 */
    private String logName;

    /** 로그 텍스트 */
    private String logText;

    /** 컴퓨터 이름 */
    private String computerName;

    /** 사용자 이름 */
    private String userName;

	/** 부서 이름 */
	private String deptName;

    /** 작업자 이름 */
    private String workerName;
    
    /** 로그 날짜 */
    private String logDate;

    /** 로그 시간 */
    private String logTime;

    public void convertUTCtoUserTime() {
        this.logDate = DateUtil.convertUTCtoUserTime(this.logDate);
        this.logTime = DateUtil.convertUTCtoUserTime(this.logTime);
    }
}