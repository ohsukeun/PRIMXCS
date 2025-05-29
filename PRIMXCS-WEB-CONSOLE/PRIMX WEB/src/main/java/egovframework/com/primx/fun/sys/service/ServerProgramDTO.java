package egovframework.com.primx.fun.sys.service;

import egovframework.com.primx.fun.cmm.util.DateUtil;
import lombok.Data;

@Data
public class ServerProgramDTO {
	private int seqNo;
	private String program;
	private String version;
	private String installDate;

	public void convertUTCtoUserTime() {
		this.installDate = DateUtil.convertUTCtoUserTime(this.installDate);
	}
}

