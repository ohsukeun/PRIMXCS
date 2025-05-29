package egovframework.com.primx.fun.sup.service;

import java.io.Serializable;
import java.util.Date;
import lombok.Data;

@Data
public class SignUpVO implements Serializable { // implements Serializable 추가
    private static final long serialVersionUID = 1L;

    /** 사용자 ID */
    private String userId;

    /** 사용자 이름 */
    private String userName;

    /** 비밀번호 */
    private String password;

    /** 이메일 주소 */
    private String email;

    /** 전화번호 */
    private String phoneNumber;

    /** 회사 코드 */
    private String companyCode;

    /** 등록일자 */
    private Date insertDate;
    
    /** 사용권한 */
    private String userRole;
    
    /** 고유ID */
    private String uniqId;

    /** 사용기간 */
    private int usePeriod;

    /** 만료일자 */
    private Date expiredDate;
    
}
