package egovframework.com.primx.fun.cmm.service;
// 회원 정보 수정 DTO 
import lombok.Data;

@Data
public class MyPageUpdateDTO {
    private String userId;
    private String userName;
	private String phoneNumber;
	private String currentPassword;  // 기존 패스워드
	private String password;         // 새 패스워드
	private String confirmPassword;  // 새 패스워드 확인

    public String getUserId() {
        return userId;
    }

    public String getUserName() {   
		return userName;
	}

	public String getPhoneNumber() {
		return phoneNumber;
	}

	public String getCurrentPassword() {
		return currentPassword;
	}

	public String getPassword() {
		return password;
	}

	public String getConfirmPassword() {
		return confirmPassword;
	}
}