package egovframework.com.primx.fun.usr.service;

import java.util.List;

import egovframework.com.primx.fun.cmm.util.DateUtil;
import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@Builder
@AllArgsConstructor
@NoArgsConstructor
public class UserRegistrationDTO {

	private String approvalCode;

	private String uniqId;
	private String emplyrId;
	private String password;
	private String userName;
	private String email;
    private String useYn;
    private String userRole;
	private String deptId;
	private String companyId;
    private String deptName;
	private String usePeriod;
	private String expiredDate;
	private String pwdExpiredDate;
	private String pwdChanged;
    private List<IpInfoDTO> ipList; // IP 정보 리스트

	public void convertUTCtoUserTime() {
		this.expiredDate = DateUtil.convertUTCtoUserTime(this.expiredDate);
		this.pwdExpiredDate = DateUtil.convertUTCtoUserTime(this.pwdExpiredDate);
	}
    
	public void convertUserTimeToUTC() {
		this.expiredDate = DateUtil.convertUserTimeToUTC(this.expiredDate);
		this.pwdExpiredDate = DateUtil.convertUserTimeToUTC(this.pwdExpiredDate);
	}
}