package egovframework.com.primx.fun.sup.service;

import lombok.Data;

@Data
public class PasswordSettingVO {
    private int id;
    private String userRole;
    private int passExpireDays;
    private int passMinLength;
    private int passNotifyBefore;
    private int passCombUppercase;
    private int passCombDigit;
    private int passCombSymbol;
}
