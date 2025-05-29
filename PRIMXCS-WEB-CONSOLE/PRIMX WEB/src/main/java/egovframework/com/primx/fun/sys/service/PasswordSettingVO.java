package egovframework.com.primx.fun.sys.service;

import lombok.Data;


/*
 * 테이블 정보
 * 
CREATE TABLE `T_ENV_VERIFICATION` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '자동 생성',
  `user_role` varchar(10) NOT NULL COMMENT '사용자 등급. admin,user',
  `pass_expire_days` int(11) NOT NULL COMMENT '패스워드 유효 기간. date',
  `pass_min_length` int(11) NOT NULL COMMENT '패스워드 최소 길이',
  `pass_notify_before` int(11) NOT NULL COMMENT '패스워드 만료 전 알림',
  `pass_comb_uppercase` int(11) NOT NULL COMMENT '패스워드 대문자 수. Uppercase',
  `pass_comb_digit` int(11) NOT NULL COMMENT '패스워드 숫자 수. Number',
  `pass_comb_symbol` int(11) DEFAULT NULL COMMENT '패스워드 특수문자 수. character',
  PRIMARY KEY (`id`),
  UNIQUE KEY `uidx_t_env_verfication` (`user_role`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci COMMENT='패스워드(인증)';
 * 
 */
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
