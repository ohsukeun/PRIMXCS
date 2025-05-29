package egovframework.com.primx.fun.sys.service;

import java.util.Date;

import lombok.Data;

@Data
public class UnusedSystemDTO {
    private int seqNo;
    private String companyName;
    private String deptName;
    private String userNm;
    private Date lastKaDate;
}
