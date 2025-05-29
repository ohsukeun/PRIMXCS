package egovframework.com.primx.fun.sys.service;

import lombok.Data;

/*
 * CREATE TABLE `T_POLICY_FILE` (
  `C_FILE_ID` int(11) NOT NULL AUTO_INCREMENT COMMENT '자동 순번',
  `C_FILE_TYPE` char(1) NOT NULL DEFAULT 'M' COMMENT '파일종류 (이동파일 = M, 일반파일 = C)',
  `C_EXPIRED_DATE` int(11) NOT NULL DEFAULT 10 COMMENT '유효기간',
  `C_DEC_COUNT` int(11) NOT NULL DEFAULT 10 COMMENT '복호화 횟수',
  `C_CERT_URL` varchar(255) DEFAULT NULL COMMENT '인증서버주소',
  PRIMARY KEY (`C_FILE_ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;
 * 
 */
@Data
public class FileMovePolicySettingVO {
    private int fileId;
    private String fileType;
    private int expiredDate;
    private int decCount;
    private String certUrl;
}
