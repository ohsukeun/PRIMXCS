package egovframework.com.primx.fun.usr.service;

import egovframework.com.primx.fun.cmm.util.DateUtil;
import lombok.Data;

@Data
public class UserListDTO {
    private String emplyrId;        // 사용자 ID
    private String userRole;        // 사용자 레벨
    private String userRoleNm;    // 사용자 레벨 명
    private String userNm;          // 사용자명
    private String emailAdres;      // 이메일
    private String deptName;        // 부서명
    private String insertByYn;      // 구분(연동/수동)
    private String userYn;          // 상태(사용/중지)
    private String pwdChanged;      // 비밀번호(초기/변경)
    private String pwdExpiredDate;   // 사용기간
    private String expiredDate;     // 사용기간
    private String insertDate;      // 등록날짜

    public void convertUTCtoUserTime() {
        this.insertDate = DateUtil.convertUTCtoUserTime(this.insertDate);
        this.expiredDate = DateUtil.convertUTCtoUserTime(this.expiredDate);
        this.pwdExpiredDate = DateUtil.convertUTCtoUserTime(this.pwdExpiredDate);
    }
} 