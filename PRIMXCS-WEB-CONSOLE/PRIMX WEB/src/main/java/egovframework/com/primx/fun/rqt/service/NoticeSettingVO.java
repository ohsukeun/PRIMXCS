package egovframework.com.primx.fun.rqt.service;


/**
 * CREATE TABLE `T_NOTICE_SETTING` (
  `NOTICE_SEQ` int(11) NOT NULL AUTO_INCREMENT COMMENT '공지 ID. 자동 생성',
  `REQUEST_USER_ROLE` varchar(255) DEFAULT NULL COMMENT 'ADMIN(관리자), USER(사용자)',
  `REQUEST_APPROVAL_CODE` char(1) DEFAULT 'C' COMMENT '이동파일 생성 신청 : M, 사용자 등록 신청 : A, 계정연장신청 : C',
  `APPROV_TARGET` char(2) DEFAULT NULL COMMENT '결재대상 U:사용자, A:관리자, AP : 결재자',
  `NOTICE_CONTENT` varchar(255) DEFAULT NULL COMMENT '공지 내용',
  `NOTICE_REASON` varchar(255) DEFAULT NULL COMMENT '사유',
  `USE_YN` char(1) DEFAULT NULL COMMENT '사용 유무 Y=사용, N=미사용',
  `CREATED_DATE` datetime DEFAULT NULL COMMENT '생성일',
  `MODIFIED_DATE` datetime DEFAULT NULL COMMENT '수정일',
  PRIMARY KEY (`NOTICE_SEQ`)
) ENGINE=InnoDB AUTO_INCREMENT=19 DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci COMMENT='공지 등록 테이블';
 */

import egovframework.com.primx.fun.cmm.util.DateUtil;
import lombok.Data;

@Data
public class NoticeSettingVO {
    private int noticeSeq; 
    private String requestUserRole;
    private String requestApprovalCode;
    private String approvTarget;
    private String delegatedYn;
    private String noticeContent;
    private String noticeReason;
    private String reasonUsedYn;
    private String useYn;
    private String createdDate;
    private String modifiedDate;

    public void convertUTCtoUserTime() {
        this.createdDate = DateUtil.convertUTCtoUserTime(this.createdDate);
        this.modifiedDate = DateUtil.convertUTCtoUserTime(this.modifiedDate);
    }
    
    
}