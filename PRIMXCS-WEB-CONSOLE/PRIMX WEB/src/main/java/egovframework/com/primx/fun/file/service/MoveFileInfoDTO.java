package egovframework.com.primx.fun.file.service;

import egovframework.com.primx.fun.cmm.util.DateUtil;
import lombok.Data;

@Data
public class MoveFileInfoDTO {
    
    private String fileKey;
    private String fileName;
    private String fileSize;
    private String decryptCnt;
    private String computerName;
    private String userNm;
    private String insertDate;
    private String expireDate;
    private String fileMapCount;

    public void convertUTCtoUserTime() {
        this.insertDate = DateUtil.convertUTCtoUserTime(this.insertDate);
        this.expireDate = DateUtil.convertUTCtoUserTime(this.expireDate);
    }
}
