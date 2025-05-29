package egovframework.com.primx.fun.sys.service;

import egovframework.com.primx.fun.cmm.util.DateUtil;

import lombok.Data;

@Data
public class ClientProgramDTO {
    private int seqNo; // 번호
    private String programName; // 프로그램 이름
    private int processorArchitecture; // 프로세서 아키텍처
    private String fileName; // 파일명
    private String productVersion; // 프로그램 버전
    private String insertDate; // 등록일시
    private String updateDate; // 수정일시

	public void convertUTCtoUserTime() {
		this.insertDate = DateUtil.convertUTCtoUserTime(this.insertDate);
		this.updateDate = DateUtil.convertUTCtoUserTime(this.updateDate);
	}
} 