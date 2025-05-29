/*
	file name     : primxdb_create.sql
	file revision : 0.9.018
	comment       : 이 파일은 반드시 MariaDB의 root 권한으로 수행되어야 함.
	created time  : 2023-06-07
	last update   : 2023-12-28
	history       : 
		2023-06-07	created
		2023-06-08	T_EX_FILE_AUTH_INFO table add
					CONSRV_ZEDFILE table add 
					CONSRV_LOG table add
					CONSRV_INTERFACE table add
		2023-06-14  T_AGENT_PRODUCTS_LOG table add
					T_DEPT_INFO.C_PARENT_ID column add
					T_TEMP_DEPT_INFO.C_PARENT_ID column add
					T_USER_MASTER.C_USE_YN column add
					T_SYSTEM_MASTER.C_USE_YN column add
					T_SYSTEM_MASTER.C_UNINSTALL_YN column add 
					T_SYSTEM_CONFIG records define
		2023-06-15  3 database accounts(primx_ctrl, primx_update, primx_auth) add
		2023-06-23  T_ACCOUNT_INFO delete
					T_USER_MASTER.C_COMPANY_ID, C_INSERTED_BY_HR_YN add
					T_USER_MASTER.C_COMPANY_ID add on Unique Index & Index 
					T_DEPT_INFO -> T_DEPT_MASTER name changed
					T_DEPT_MASTER.C_COMPANY_ID add
					T_DEPT_MASTERC_COMPANY_ID add on Unique Index & Index
					T_USER_SYSTEM_MASTER -> T_USER_SYSTEM_MATCH name changed
					T_USER_SYSTEM_MATCH.C_COMPAN_ID add
					T_USER_SYSTEM_MATCH.C_COMPANY_ID add on Unique Index & Index
					T_TEMP_USER_MASTER.C_COMPANY_ID add
					T_TEMP_USER_MASTER.C_COMPANY_ID add onUnique Index & Index
					T_TEMP_DEPT_MASTER -> T_TEMP_DEPT_MASTER name changed
					T_TEMP_DEPT_MASTER.C_COMPANY_ID add
					T_TEMP_DEPT_MASTER.C_COMPANY_ID add onUnique Index & Index
					T_SYSTEM_MASTER.C_COMMON_SYSTEM_YN add
					T_SYSTEM_MASTER.C_AUTHORITY_WEB_CONSOLE, C_AUTHORITY_USER_MOD and C_AUTHORITY_EXPORT_FILE delete
					T_AGENT_POLICY_MASTER add
					T_ENCRYPTION_PATH_INFO add
					T_SERVER_MODULE_STATUS add
		2023-06-25  T_AGENT_POLICY_MASTER.C_DEFAULT_POLICY_YN add
					default record insert into T_AGENT_POLICY_MASTER 
					T_USER_MASTER.C_USER_TYPE add
					default record insert into T_USER_MASTER 
					T_ENCRYPTION_PATH_INFO -> T_NOT_ENCRYPTION_PATH_INFO naem changed
					T_NOT_ENCRYPTION_PATH_INFO.C_ENCRYPTION_PATH -> T_NOT_ENCRYPTION_PATH_INFO.C_NOT_ENCRYPT_PATH name changed
		2023-07-02  T_SYSTEM_MASTER.C_BUILD_NUMBER type changed. INT(11) -> VARCHAR(50)
		2023-07-03  HRImportType, HRImportRunTime, SystemInfoAutoDeleteRunTime records inserted into T_SERVER_CONFIG
					T_AGENT_POLICY_SYSTEM_MATCH table add
		2023-07-04	T_PRIMX_SETUP_INFO unique index modified. UNIQUE KEY (`C_AGENT_ID`) -> UNIQUE KEY (`C_AGENT_ID`, `C_PRODUCT_TYPE`)
		2023-07-05	T_SYSTEM_PROC_ERROR table add
		2023-07-06	T_NIC_INFO.C_NAME add
		2023-07-12	Web Module related tables add
		2023-07-13	T_AGENT_POLICY_MASTER.C_PRODUCT_MENU_ENABLE, C_WEB_MANAGER_CONSOLE_MENU_ENABLE, C_EXPORT_FILE_MENU_ENABLE columns add
					GUEST user record insert into T_USER_MASTER table
		2023-07-15	FUNC_COMPARE_VERSION, PROC_CHECK_INSERT_USER_SYSTEM_MATCH, PROC_GET_PRODUCT_UPDATE_CHECK, PROC_GET_SERVER_CONFIGS_FOR_KEEPALIVE, PROC_GET_KEEPALIVE_RESPONSE add
		2023-07-20	Table name modified.
					T_DEPT_MASTER.C_INSERTED_BY_HR_YN add
		2023-07-22	PROC_GET_PRODUCT_UPDATE_INFO add
					PROC_GET_PRODUCT_UPDATE_CHECK -> PROC_CHECK_PRODUCT_UPDATE rename
		2023-07-25	T_PRIMX_SETUP_INFO.C_USE_EX_DOWNLOAD_SERVER_YN delete
					T_EX_FILE_AUTH_INFO.C_ORG_FILE_NAME/C_ORG_FILE_SIZE/C_ORG_FILE_PATH delete
					T_EX_ARCHIVED_FILE_INFO table add
		2023-08-09	T_AGENT_PRODUCTS_LOG.C_LOG_CODE size enlarged to 20 
					T_EX_SERVER_INFO unique index modified. UNIQUE KEY (`C_PRODUCT_TYPE`) -> UNIQUE KEY (`C_PRODUCT_TYPE`, `C_SERVER_URL`)
		2023-08-10	PROC_GET_PRODUCT_UPDATE_INFO procedure modified
		2023-08-16	T_USER_MASTER.C_USER_PWD, C_PWD_CHANGED default value and attribute modified
					T_USER_MASTERC_USER_PWD size 50 to 200
					T_TEMP_USER_MASTERC_USER_PWD size 50 to 200
					T_MANAGE_LOG.C_DISP_CLASS add
		2023-08-18	T_AGENT_POLICY_MASTER.C_INSERT_BY, C_AGENT_POLICY_CODE add
		2023-08-23	T_UPDATE_INFO.C_PROCESSOR_ARCHITECTURE add
		2023-08-24	T_ENV_VERIFICATION base records defined
					T_ENV_GENERAL table add
		2023-08-25	T_VERSION_INFO table delete
					PROC_SYNC_HR_WITH_RETURN, PROC_SYNC_HR_NO_RETURN procedure created
		2023-08-31	PROC_SYNC_HR_WITH_RETURN, PROC_SYNC_HR_NO_RETURN procedure modified
		2023-09-01	PROC_STAT_T_MANAGE_LOG created
					SCH_INSERT_T_MANAGE_LOG_STAT created
		2023-09-04	T_MANAGER_MASTER, T_MANAGER_ALLOWED_IP table add
		2023-09-05	T_SERVER_INTERNAL_KV add
		2023-09-08	PROC_GET_KEEPALIVE_RESPONSE modified
					FUNC_GET_SERVER_CONFIG add
		2023-09-09	PROC_FOR_SCH_CHECK_AND_RUN_HRSYNC add
					SCH_CHECK_AND_RUN_HRSYNC add
					PROC_FOR_SCH_CHECK_AND_DELETE_OFFLINE_SYSTEM_INFO add
					SCH_CHECK_AND_RUN_UPDATE_OFFLINE_SYSTEM_INFO add
					PROC_FOR_SCH_CHECK_AND_DELETE_OLD_LOG add
					SCH_CHECK_AND_RUN_DELETE_OLD_LOG add
		2023-09-21	T_EX_FILE_AUTH_INFO.C_FILE_SIZE, C_FILE_NAME add
					PROC_INSERT_T_EX_FILE_AUTH_INFO modified
					PROC_APPLY_EX_FILE_AUTH_INFO modified
		2023-09-22	T_EX_FILE_AUTH_LOG add
					PROC_FOR_SCH_CHECK_AND_DELETE_OLD_LOG modified
		2023-09-25	T_LOG_RESOURCE delete
					T_RESOURCE_PRODUCT_LOG, T_RESOURCE_MANAGE_LOG, T_RESOURCE_LOG_TYPE, T_RESOURCE_LOG_RESULT, T_EX_FILE_AUTH_LOG_STAT add
					T_EX_FILE_AUTH_LOG.C_REQUEST_IP, T_EX_FILE_AUTH_LOG.C_REQUEST_MAC add
					PROC_APPLY_EX_FILE_AUTH_INFO modified
					PROC_STAT_T_EX_FILE_AUTH_LOG, SCH_INSERT_T_EX_FILE_AUTH_LOG_STAT add
		2023-10-04	T_EX_SERVER_INFO.C_PRODUCT_TYPE delete, C_SERVER_NAME add
					T_UPDATE_INFO.C_EX_SERVER_USE delete
					FUNC_GET_FILE_NAME_FROM_PATH add
					PROC_GET_PRODUCT_UPDATE_INFO procedure modified
		2023-10-10	T_MANAGER_MASTER.C_EXPIRED_DATE, C_PWD_EXPIRED_DATE, C_PWD_CHANGED, C_BEGIN_DATE add
		2023-10-15	FUNC_COMPARE_VERSION, FUNC_GET_SERVER_CONFIG, PROC_SYNC_HR_USER modified
		2023-10-16	PROC_GET_PRODUCT_UPDATE_INFO modified
		2023-10-18	PROC_CHECK_INSERT_USER_SYSTEM_MATCH, PROC_SYNC_HR_USER, PROC_GET_KEEPALIVE_RESPONSE modified. (PUBLIC_PC deleted)
		2023-10-26	T_MANAGER_MASTER.C_USE_IP_YN, C_IS_PERIOD, C_IS_ACTIVE add
		2023-10-27	T_AGENT_PRODUCTS_LOG.C_WORKER_NAME add
					T_DEPT_MASTER.C_ORDER_INDEX add
					T_MANAGE_LOG.C_WORKER_NAME add
		2023-11-02	T_EX_FILE_AUTH_LOG.C_DECRYPT_CNT, C_EXPIRE_DATE add
					PROC_APPLY_EX_FILE_AUTH_INFO modified
		2023-11-03	T_EMPTY_DAYS table add
		2023-11-06	PROC_FOR_SCH_CHECK_AND_RUN_HRSYNC modified
		2023-11-08	PROC_SYNC_HR_DEPT, PROC_SYNC_HR_USER modified
		2023-11-10	PROC_FOR_SCH_CHECK_AND_DELETE_OFFLINE_SYSTEM_INFO, PROC_FOR_SCH_CHECK_AND_UPDATE_OFFLINE_SYSTEM_INFO modified
					T_USER_MASTER table unique index add
					
		*Rimahr modify list			
		2023-12-01  T_MANAGER_MASTER C_MANAGER_PWD modified (Encrypted pwd error)
		2023-12-04  default record insert into T_ENV_REQUEST_APPROVAL
		2023-12-05  default record insert into T_AGENT_POLICY_MASTER
		            default record insert into T_COMMON_CODE
		            default record insert into T_ENV_POLICY
		            T_ENV_POLICY file_id column default modified (NOT NULL > DEFAULT NULL)
					T_ENV_GENERAL company_id column add
					T_ENV_GENERAL column comment add
		2023-12-05  default record update into T_PRODUCTS (PRIMX Zonecentral, Orizon, Cryhod > ZONECENTRAL, ORIZON, CRYHOD)
					default record insert into T_UPDATE_INFO (Agent 32bit, Agent 64Bit)
					T_POLICY_SYSTEM_MAP C_AGENT_ID UNIQUE KEY delete (Name:uidx_policy_system_map)
		2023-12-12  T_POLICY table unique index add	(policy_code)
					T_POLICY_SYSTEM_MAP policy_id column name modified (policy_id > policy_code)
					T_POLICY_SYSTEM_MAP key modified > KEY `idx_policy_system_map` (`policy_code`,`C_AGENT_ID`)
					T_POLICY_SYSTEM_MAP key modified > KEY `policy_system_map_policy_id_fk` (`policy_code`)
					T_POLICY_SYSTEM_MAP constraint modified > CONSTRAINT `policy_system_map_policy_id_fk` FOREIGN KEY (`policy_code`) REFERENCES `T_POLICY` (`policy_code`)
					T_REQUEST request_cd column name modified (request_cd > request_code)
					T_REQUEST table unique index add	(request_code)
					T_REQUEST_APPROVERS request_cd column name modified (request_cd > request_code)
					T_REQUEST_APPROVERS key delete > KEY `request_approvers_request_request_id_fk` (`request_id`)
					T_REQUEST_APPROVERS constraint modified > CONSTRAINT `request_approvers_request_request_id_fk` FOREIGN KEY (`request_code`) REFERENCES `T_REQUEST` (`request_code`)					
		2023-12-14  default record modified T_AGENT_POLICY_MASTER C_AGENT_POLICY_CODE (NULL > AGN-2020010100001)
		2023-12-19  T_AGENT_POLICY_MASTER key delete > UNIQUE KEY uidx_t_agent_policy_master (`C_POLICY_NAME`)
		            T_AGENT_POLICY_SYSTEM_MATCH column name modified (C_POLICY_ID > C_AGENT_POLICY_CODE)
        2023-12-20  default record insert into T_EMPTY_DAYS ( C_DAY_INDEX : 1~3650 (10 YEARS))
					T_AGENT_POLICY_SYSTEM_MATCH key modified > KEY idx_t_agent_policy_system_match (`C_AGENT_POLICY_CODE`, `C_AGENT_ID`) 
					T_NOT_ENCRYPTION_PATH_INFO column name&type modified (C_POLICY_ID > C_AGENT_POLICY_CODE)
					T_NOT_ENCRYPTION_PATH_INFO key modified > KEY idx_t_not_encryption_path_info (`C_AGENT_POLICY_CODE`) 					
					PROC_GET_KEEPALIVE_RESPONSE procedure modified ( C_AGENT_POLICY_CODE column )
		2023-12-28	PROC_OPERATE_T_POLICY_SYSTEM_MAP procedure add
		2024-02-28	T_REQUEST column modifed_by type modify (INT(11)->VARCHAR(50))
		2024-03-06	default record insert into T_NOT_ENCRYPTION_PATH_INFO 
*/

/* ----------------------------------------------------------------------------
	PRE-PROCESSING PROCEDURE(S)
---------------------------------------------------------------------------- */ 
-- check and set database permission procedure
DROP PROCEDURE IF EXISTS `mysql`.`proc_primx_check_and_set_db_permission`;
DELIMITER |
CREATE PROCEDURE `mysql`.`proc_primx_check_and_set_db_permission`(
    IN STR_HOST     VARCHAR(50),
    IN STR_DBNAME   VARCHAR(50),
    IN STR_USER     VARCHAR(50),
    IN STR_PASSWORD VARCHAR(50),
    IN BOOL_SSL     INT
)
BEGIN
    DECLARE INT_EXISTANCE INT;

    SELECT COUNT(*) INTO INT_EXISTANCE FROM `mysql`.`db` WHERE `Host` = STR_HOST AND `Db` = STR_DBNAME AND `User` = STR_USER;
    IF (0 = INT_EXISTANCE) THEN
        IF (1 = BOOL_SSL) THEN
            SET @QUERY = CONCAT_WS('','GRANT ALL PRIVILEGES ON ', STR_DBNAME, '.* TO ', STR_USER, '@', CHAR(39), STR_HOST, CHAR(39), ' IDENTIFIED BY ', CHAR(39), STR_PASSWORD, CHAR(39), ' require SSL; ');
            PREPARE stmt1 FROM @QUERY;
            EXECUTE stmt1;
            FLUSH PRIVILEGES;
        ELSE
            SET @QUERY = CONCAT_WS('','GRANT ALL PRIVILEGES ON ', STR_DBNAME, '.* TO ', STR_USER, '@', CHAR(39), STR_HOST, CHAR(39), ' IDENTIFIED BY ', CHAR(39), STR_PASSWORD, CHAR(39), '; ');
            PREPARE stmt1 FROM @QUERY;
            EXECUTE stmt1;
			FLUSH PRIVILEGES;
        END IF;
    END IF;
END |
DELIMITER ;

/* ----------------------------------------------------------------------------
	CREATE DATABASE & USERS
---------------------------------------------------------------------------- */ 
-- database creation
CREATE DATABASE IF NOT EXISTS PRIMXDB;

-- check user
CREATE USER IF NOT EXISTS 'primx_user'@'localhost' IDENTIFIED BY 'primxuser1234~!';
CREATE USER IF NOT EXISTS 'primx_user'@'%' IDENTIFIED BY 'primxuser1234~!';

CREATE USER IF NOT EXISTS 'primx_user2'@'localhost' IDENTIFIED BY 'primxuser1234~!';
CREATE USER IF NOT EXISTS 'primx_user2'@'%' IDENTIFIED BY 'primxuser1234~!';

CREATE USER IF NOT EXISTS 'primx_web'@'localhost' IDENTIFIED BY 'primxuser1234~!';
CREATE USER IF NOT EXISTS 'primx_web'@'%' IDENTIFIED BY 'primxuser1234~!';

CREATE USER IF NOT EXISTS 'primx_ctrl'@'localhost' IDENTIFIED BY 'primxuser1234~!';
CREATE USER IF NOT EXISTS 'primx_ctrl'@'%' IDENTIFIED BY 'primxuser1234~!';

CREATE USER IF NOT EXISTS 'primx_auth'@'localhost' IDENTIFIED BY 'primxuser1234~!';
CREATE USER IF NOT EXISTS 'primx_auth'@'%' IDENTIFIED BY 'primxuser1234~!';

-- check database permission
call `mysql`.`proc_primx_check_and_set_db_permission`('localhost', 'PRIMXDB', 'primx_user', 'primxuser1234~!', 1);
call `mysql`.`proc_primx_check_and_set_db_permission`('%',         'PRIMXDB', 'primx_user', 'primxuser1234~!', 1);

call `mysql`.`proc_primx_check_and_set_db_permission`('localhost', 'PRIMXDB', 'primx_user2', 'primxuser1234~!', 0);
call `mysql`.`proc_primx_check_and_set_db_permission`('%',         'PRIMXDB', 'primx_user2', 'primxuser1234~!', 0);

call `mysql`.`proc_primx_check_and_set_db_permission`('localhost', 'PRIMXDB', 'primx_web', 'primxuser1234~!', 1);
call `mysql`.`proc_primx_check_and_set_db_permission`('%',         'PRIMXDB', 'primx_web', 'primxuser1234~!', 1);

call `mysql`.`proc_primx_check_and_set_db_permission`('localhost', 'PRIMXDB', 'primx_ctrl', 'primxuser1234~!', 1);
call `mysql`.`proc_primx_check_and_set_db_permission`('%',         'PRIMXDB', 'primx_ctrl', 'primxuser1234~!', 1);

call `mysql`.`proc_primx_check_and_set_db_permission`('localhost', 'PRIMXDB', 'primx_auth', 'primxuser1234~!', 1);
call `mysql`.`proc_primx_check_and_set_db_permission`('%',         'PRIMXDB', 'primx_auth', 'primxuser1234~!', 1);

-- change database
USE PRIMXDB;

/* ----------------------------------------------------------------------------
	CREATE TABLES - PRIMX_CS
---------------------------------------------------------------------------- */ 
-- Agent 정책 테이블
CREATE TABLE IF NOT EXISTS `T_AGENT_POLICY_MASTER` (
  `C_POLICY_ID` int(11) NOT NULL AUTO_INCREMENT COMMENT '정책 ID',
  `C_POLICY_NAME` varchar(255) NOT NULL COMMENT '정책 이름',
  `C_INSERT_BY` varchar(50) DEFAULT NULL,
  `C_AGENT_POLICY_CODE` varchar(50) NOT NULL COMMENT 'Agent 정책 Code',
  `C_DEFAULT_POLICY_YN` char(1) DEFAULT 'N' COMMENT '기본 정책 여부 (‘Y’: 기본정책, ‘N’: 기본정책 아님(Default))',
  `C_EXPORT_FILE_AUTH` int(11) DEFAULT 0 COMMENT '반출 파일 생성 권한. 0: 권한 없음, 1: 권한 있음',
  `C_WEB_MANAGER_AUTH` int(11) DEFAULT 0 COMMENT '반출 신청 결재 권한. 0: 권한 없음, 1: 권한 있음',
  `C_NOT_ENCRYPTION_PATH_AUTH` int(11) DEFAULT 0 COMMENT '비암호화 영역 설정 권한. 0: 권한 없음, 1: 권한 있음',
  `C_PRODUCT_MENU_ENABLE` int(11) DEFAULT 0 COMMENT 'PRIMX 제품의 빠른 메뉴 표시 여부. 0: 권한 없음, 1: 권한 있음',
  `C_WEB_MANAGER_CONSOLE_MENU_ENABLE` int(11) DEFAULT 0 COMMENT '사용자 콘솔 메뉴 표시 여부. 0: 권한 없음, 1: 권한 있음',
  `C_EXPORT_FILE_MENU_ENABLE` int(11) DEFAULT 0 COMMENT '이동파일 생성 메뉴 표시 여부. 0: 권한 없음, 1: 권한 있음',
  `C_ZTRY_MENU_ENABLE` int(11) DEFAULT 0 COMMENT 'Display ZONECENTRAL menu. 0: No permission, 1: Has permission',
  `C_ZOPEN_AREA` int(11) DEFAULT 0 COMMENT 'Display ZONECENTRAL open area. 0: No permission, 1: Has permission',
  `C_ZPERSONAL_ACCESS` int(11) DEFAULT 0 COMMENT 'Display ZONECENTRAL personal access. 0: No permission, 1: Has permission',
  `C_ZOPTION_DISPLAY` int(11) DEFAULT 0 COMMENT 'Display ZONECENTRAL options. 0: No permission, 1: Has permission',
  `C_ZSAFETY_ZONE` int(11) DEFAULT 0 COMMENT 'Display ZONECENTRAL SafetyZone. 0: No permission, 1: Has permission',
  `C_ZAREA_CLOSE` int(11) DEFAULT 0 COMMENT 'Display ZONECENTRAL close all areas. 0: No permission, 1: Has permission',
  `C_ZENC` int(11) DEFAULT 0 COMMENT 'Display ZONECENTRAL encryption status. 0: No permission, 1: Has permission',
  `C_EXPORT_FILE_SEND_MENU_ENABLE` int(11) DEFAULT 0 COMMENT 'EX{PRT_FILE_SEND_MENU_ENABLE. 0: No permission, 1: Has permission',
  `C_INSERT_DATE` datetime DEFAULT NULL COMMENT '생성 일자',
  `C_UPDATE_DATE` datetime DEFAULT NULL COMMENT '수정 일자',
  PRIMARY KEY (`C_POLICY_ID`)
) ENGINE=InnoDB AUTO_INCREMENT=27 DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci COMMENT='Agent 정책 테이블';

INSERT IGNORE INTO T_AGENT_POLICY_MASTER VALUES (1,'기본 정책',NULL,'AGN-2020010100001','Y',0,0,0,0,0,0,0,0,0,0,0,0,0,0,'2023-12-04 08:24:23',NULL);

-- Agent 정책 시스템 매칭 테이블
CREATE TABLE IF NOT EXISTS T_AGENT_POLICY_SYSTEM_MATCH ( 
  `C_SEQ_NO` BIGINT(20) NOT NULL AUTO_INCREMENT COMMENT '번호', 
  `C_AGENT_POLICY_CODE` varchar(50) NOT NULL COMMENT 'Agent 정책 Code',
  `C_AGENT_ID` VARCHAR(50) NOT NULL COMMENT 'Agent ID', 
  `C_POLICY_TIMESTAMP` DATETIME DEFAULT NULL COMMENT '정책 적용 시각', 
  `C_INSERT_DATE` DATETIME DEFAULT NULL COMMENT '생성 일자', 
  `C_UPDATE_DATE` DATETIME DEFAULT NULL COMMENT '수정 일자', 
  PRIMARY KEY (`C_SEQ_NO`), 
  UNIQUE KEY uidx_t_agent_policy_system_match (`C_AGENT_ID`), 
  KEY idx_t_agent_policy_system_match (`C_AGENT_POLICY_CODE`, `C_AGENT_ID`) 
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Agent 정책 시스템 매칭 테이블';

-- Agent에 설치된 제품 로그 테이블
CREATE TABLE IF NOT EXISTS T_AGENT_PRODUCTS_LOG ( 
  `C_SEQ_NO` BIGINT(20) NOT NULL AUTO_INCREMENT COMMENT '번호', 
  `C_LOG_TIME` DATETIME NOT NULL COMMENT '로그 발생 시각', 
  `C_AGENT_ID` VARCHAR(50) NOT NULL COMMENT 'Agent ID', 
  `C_PRODUCT_TYPE` TINYINT(4) NOT NULL DEFAULT 0 COMMENT '제품 종류 (0: 알 수 없음, 1: PRIMX ZONECENTRAL, 2: PRIMX ORIZON, 3: PRIMX CRYHOD)', 
  `C_LOG_TYPE` VARCHAR(20) DEFAULT NULL COMMENT '로그 종류 (MASTER.EXE, AFSCMD.EXE, AFSCMD, ZCACMD, ZBOARD, ZCAPPLY, BLANK, ORIZON, MASTER, OZU, OZCMD)',
  `C_LOG_CODE` VARCHAR(20) DEFAULT NULL COMMENT '로그 코드', 
  `C_LOG_LEVEL` VARCHAR(10) DEFAULT NULL COMMENT '로그 레벨 (INFO, WARNING, ERROR)', 
  `C_LOG_TEXT` VARCHAR(1024) DEFAULT NULL COMMENT '로그 내용', 
  `C_WORKER_NAME` VARCHAR(50) DEFAULT NULL COMMENT '작업자명', 
  PRIMARY KEY (`C_SEQ_NO`), 
  KEY idx_t_agent_products_log (`C_AGENT_ID`,`C_PRODUCT_TYPE`,`C_LOG_TYPE`,`C_LOG_TIME`) 
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='에이전트에 설치된 제품 로그 테이블';

-- 알림 매핑 테이블
CREATE TABLE IF NOT EXISTS `T_ALARM_MAP` (
  `C_CODE` varchar(10) NOT NULL COMMENT '알림종류코드',
  `C_TYPE_DESC` varchar(50) NOT NULL COMMENT '알림종류 설명',
  `C_USED_YN` char(1) NOT NULL DEFAULT 'Y' COMMENT '사용유무 (Y:사용, N:미사용)',
  PRIMARY KEY (`C_CODE`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

INSERT IGNORE INTO T_ALARM_MAP (C_CODE, C_TYPE_DESC, C_USED_YN) VALUES('A', '사용자 등록 결재 요청', 'Y');
INSERT IGNORE INTO T_ALARM_MAP (C_CODE, C_TYPE_DESC, C_USED_YN) VALUES('C', '계정 연장 결재 요청', 'Y');
INSERT IGNORE INTO T_ALARM_MAP (C_CODE, C_TYPE_DESC, C_USED_YN) VALUES('D', '미접속 시스템 삭제 알림', 'Y');
INSERT IGNORE INTO T_ALARM_MAP (C_CODE, C_TYPE_DESC, C_USED_YN) VALUES('E', '계정 만료 알림', 'Y');
INSERT IGNORE INTO T_ALARM_MAP (C_CODE, C_TYPE_DESC, C_USED_YN) VALUES('H', '인사 연동 알림', 'Y');
INSERT IGNORE INTO T_ALARM_MAP (C_CODE, C_TYPE_DESC, C_USED_YN) VALUES('M', '파일 반출 결재 요청', 'Y');


-- 알림 테이블
CREATE TABLE IF NOT EXISTS `T_ALARM` (
  `C_SEQ_NO` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '번호',
  `C_USER_ID` varchar(50) NOT NULL COMMENT '사용자',
  `C_PATH` varchar(255) NOT NULL COMMENT '연결페이지',
  `C_TYPE` varchar(10) NOT NULL COMMENT '알림종류(A0001:파일반출결재, A0002:사용자등록결재, A0003:계정연장결재, E0001:계정만료, H0001:인사연동, D0001:미접속시스템삭제)',
  `C_USER_ROLE` varchar(50) NOT NULL COMMENT '대상자권한',
  `C_INSERT_DATE` datetime NOT NULL DEFAULT current_timestamp() COMMENT '알림생성일자',
  `C_READ` tinyint(4) NOT NULL DEFAULT 0 COMMENT '읽음여부 (0:안읽음, 1:읽음)',
  `C_TARGET_ID` varchar(50) NOT NULL COMMENT '알림대상자',
  PRIMARY KEY (`C_SEQ_NO`),
  KEY `FK_ALARM_TYPE` (`C_TYPE`),
  CONSTRAINT `FK_ALARM_TYPE` FOREIGN KEY (`C_TYPE`) REFERENCES `T_ALARM_MAP` (`C_CODE`),
  CONSTRAINT `FK_T_ALARM_C_TYPE` FOREIGN KEY (`C_TYPE`) REFERENCES `T_ALARM_MAP` (`C_CODE`)
) ENGINE=InnoDB AUTO_INCREMENT=73 DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

-- 코드 생성하여 정렬
CREATE TABLE IF NOT EXISTS `T_CODE_SEQ` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '자동 생성', 
  `code_date` DATE NOT NULL COMMENT '코드 생성 날짜', 
  `order_number` INT(11) NOT NULL COMMENT '코드 생성된 순번', 
  `code_key` VARCHAR(10) NOT NULL COMMENT '코드 키. code type(R,D, POL)', 
  PRIMARY KEY (`id`) 
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT '코드 생성하여 정렬';

-- 상태 저장 및 필터링
CREATE TABLE IF NOT EXISTS `T_COMMON_CODE` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '자동 생성', 
  `code_group` VARCHAR(255) DEFAULT NULL COMMENT '코드 유형. Code group. request(DR,PE,CO)Result(AP, REj)U', 
  `code_group_nm` VARCHAR(255) DEFAULT NULL COMMENT '코드 그룹명. Code group. request(DR,PE,CO)Result(AP, REj)U', 
  `code` VARCHAR(255) DEFAULT NULL COMMENT '코드. DR,PE,CO,AP,RE,A,M,U', 
  `code_eng_nm` VARCHAR(255) DEFAULT NULL COMMENT '코드 영문명', 
  `use_yn` VARCHAR(2) DEFAULT NULL COMMENT '사용 여부. Y :Yes, N:No', 
  `code_description` TEXT DEFAULT NULL COMMENT '코드 설명', 
  `created_by` INT(11) DEFAULT NULL COMMENT '등록자. 생성자 ID', 
  `created_date` DATETIME DEFAULT NULL COMMENT '생성된 날짜', 
  `modified_by` INT(11) DEFAULT NULL COMMENT '수정자 ID', 
  `modified_date` DATETIME DEFAULT NULL COMMENT '수정된 날짜', 
  `is_deleted` VARCHAR(2) NOT NULL DEFAULT 'N' COMMENT '삭제 여부. Y :Yes, N:No', 
  `is_active` VARCHAR(2) NOT NULL DEFAULT 'Y' COMMENT '활성 여부. Y :Yes, N:No', 
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT '상태 저장 및 필터링';

INSERT IGNORE INTO T_COMMON_CODE VALUES
(1, 'REQUEST_STATUS', 'REQUEST_STATUS', 'DR', 'DRAFT', 'Y', 'label_draft', 1, '2023-01-18 15:39:11.000', NULL, NULL, 'N', 'Y'),
(2, 'REQUEST_STATUS', 'REQUEST_STATUS', 'PE', 'PENDING', 'Y', 'label_pending', 1, '2023-01-18 15:39:11.000', NULL, NULL, 'N', 'Y'),
(3, 'REQUEST_STATUS', 'REQUEST_STATUS', 'CO', 'COMPLETED', 'Y', 'label_completed', 1, '2023-01-18 15:39:11.000', NULL, NULL, 'N', 'Y'),
(4, 'REQUEST_RESULT', 'REQUEST_RESULT', 'AP', 'APPROVED', 'Y', 'label_approved', 1, '2023-01-18 15:39:11.000', 7, '2023-01-26 13:14:32.000', 'N', 'Y'),
(5, 'REQUEST_RESULT', 'REQUEST_RESULT', 'RE', 'REJECTED', 'Y', 'label_rejected', 1, '2023-01-18 15:39:11.000', 7, '2023-01-19 17:42:19.000', 'N', 'Y'),
(6, 'USER_ROLE', 'USER_ROLE', 'A', 'ADMIN', 'Y', 'label_admin', 1, '2023-01-18 15:39:11.000', NULL, NULL, 'N', 'Y'),
(7, 'USER_ROLE', 'USER_ROLE', 'M', 'MANAGER', 'Y', 'label_manager', 1, '2023-01-18 15:39:11.000', NULL, NULL, 'N', 'Y'),
(8, 'USER_ROLE', 'USER_ROLE', 'U', 'USER', 'Y', 'label_user', 1, '2023-01-18 15:39:11.000', NULL, NULL, 'N', 'Y');

-- Container Server-가 컨테이너 파일(ZED File)을 만들도록 요청할 수 있는 interface table 
CREATE TABLE IF NOT EXISTS `T_CONSRV_INTERFACE` (
  `nSeq` INT(11) NOT NULL AUTO_INCREMENT COMMENT '자동 생성', 
  `LOG_DT` DATETIME DEFAULT NULL COMMENT '요청 시각', 
  `MANAGEMENTID` VARCHAR(100) DEFAULT NULL COMMENT '관리 ID로 "신청 번호"등을 사용할 수 있음', 
  `REQUESTID` VARCHAR(100) DEFAULT NULL COMMENT '요청자 ID로 3가지 ID를 등록할 수 있음', 
  `REQUESTIDTYPE` INT(11) DEFAULT NULL COMMENT '요청자 ID type. 0:system, 1:user, 2:Manager', 
  `FILE_SOURCE` VARCHAR(512) DEFAULT NULL COMMENT '원본파일로 path가 포함 되어 있음', 
  `DESTINSOURCEFILE` INT(11) DEFAULT NULL COMMENT '관리 ID 기준 원본 파일 수', 
  `APPLYFLAG` INT(11) DEFAULT NULL COMMENT '적용 FLAG. Container Server 적용 여부. 0:미적용, 1:적용', 
  PRIMARY KEY (`nSeq`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT 'Container Server-가 컨테이너 파일(ZED File)을 만들도록 요청할 수 있는 interface table';

-- Container server가 원본파일을 암호화 파일로 만든 로그 테이블
CREATE TABLE IF NOT EXISTS `T_CONSRV_LOG` (
  `nSeq` INT(11) NOT NULL AUTO_INCREMENT COMMENT '자동 생성', 
  `LOG_DT` DATETIME NOT NULL COMMENT '로그 저장 시각', 
  `MANAGEMENTID` VARCHAR(100) DEFAULT NULL COMMENT '관리 ID로 "신청번호"등을 사용할 수 있음', 
  `REQUESTID` VARCHAR(1050) DEFAULT NULL COMMENT '요청자 ID로 3가지 ID를 등록할 수 있음', 
  `REQUESTIDTYPE` INT(11) DEFAULT NULL COMMENT '요청자 ID type. 0:System, 1:User, 2:Manager', 
  `ZCHOSTNAME` VARCHAR(400) DEFAULT NULL COMMENT 'ZoneCentral host name. 암호화를 수행한 시스템의  host name', 
  `FILE_SOURCE` VARCHAR(512) DEFAULT NULL COMMENT '원본 파일. 원본파일 path가 포함 되어 있음', 
  `FILE_DESTINATION` VARCHAR(512) DEFAULT NULL COMMENT '암호 파일로 path가 포함 되어 있음', 
  `DESTINSOURCEFILE` INT(11) DEFAULT NULL COMMENT '관리 ID 기준 원본 파일 수', 
  `ORGFILETYPE` INT(11) DEFAULT NULL COMMENT '원본 파일을 Container Server가 처리한 방법. 0:delete, 1:backup', 
  PRIMARY KEY (`nSeq`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT 'Container server가 원본파일을 암호화 파일로 만든 로그 테이블';

-- Container Server가 만든 컨테이너 파일(Zed file) 테이블
CREATE TABLE IF NOT EXISTS `T_CONSRV_ZEDFILE` (
  `nSeq` INT(11) NOT NULL AUTO_INCREMENT COMMENT '자동 생성', 
  `CREATE_DT` DATETIME NOT NULL COMMENT 'Zed 파일 생성 시각', 
  `MANAGEMENTID` VARCHAR(100) DEFAULT NULL COMMENT '관리 ID로 "신청번호"등을 사용할 수 있음', 
  `ZEDFILE` VARCHAR(512) DEFAULT NULL COMMENT 'Zed file name (Path 없음)', 
  `ZEDFILEPATH` VARCHAR(512) DEFAULT NULL COMMENT 'Zed file name (path 있음)', 
  `ACCESSID` VARCHAR(100) DEFAULT NULL COMMENT '요청자 ID. Zed file Access ID (랜덤 생성)', 
  `ACCESSPW` VARCHAR(100) DEFAULT NULL COMMENT '요청자 id type. Zed file Access password(암호화, 랜덤 생성)', 
  `ZCHOSTNAME` VARCHAR(400) DEFAULT NULL COMMENT 'ZoneCentral host name. 암호화를 수행한 시스템의 host name', 
  `ZEDINSOURCEFILE` INT(11) DEFAULT NULL COMMENT '원본 파일 수. Container 파일의 암호화 파일 수', 
  PRIMARY KEY (`nSeq`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT 'Container Server가 만든 컨테이너 파일(Zed file) 테이블';

-- 부서 정보 테이블
CREATE TABLE IF NOT EXISTS T_DEPT_MASTER ( 
  `C_SEQ_NO` BIGINT(20) NOT NULL AUTO_INCREMENT COMMENT '번호', 
  `C_COMPANY_ID` VARCHAR(50) NOT NULL COMMENT '회사 ID', 
  `C_DEPT_ID` VARCHAR(50) NOT NULL COMMENT '부서 ID', 
  `C_PARENT_ID` VARCHAR(50) DEFAULT NULL COMMENT '부모 부서 ID', 
  `C_DEPT_NAME` VARCHAR(50) DEFAULT NULL COMMENT '부서 이름', 
  `C_INSERTED_BY_HR_YN` char(1) DEFAULT 'N' COMMENT '인사연동으로 생성됨 여부 (‘Y’: 인사연동됨, ‘N’: 관리자가 생성(Default))',
  `C_RESERVED1` VARCHAR(260) DEFAULT NULL COMMENT '예약 컬럼 1', 
  `C_RESERVED2` VARCHAR(260) DEFAULT NULL COMMENT '예약 컬럼 2', 
  `C_RESERVED3` VARCHAR(260) DEFAULT NULL COMMENT '예약 컬럼 3', 
  `C_RESERVED4` VARCHAR(260) DEFAULT NULL COMMENT '예약 컬럼 4', 
  `C_RESERVED5` VARCHAR(260) DEFAULT NULL COMMENT '예약 컬럼 5', 
  `C_INSERT_DATE` DATETIME DEFAULT NULL COMMENT '생성 일자', 
  `C_UPDATE_DATE` DATETIME DEFAULT NULL COMMENT '수정 일자', 
  `C_ORDER_INDEX` INT(11) DEFAULT NULL COMMENT '부서 위치 이동. 부서 위치를 위/아래로 이동', 
  PRIMARY KEY (`C_SEQ_NO`), 
  UNIQUE KEY uidx_t_dept_master (`C_COMPANY_ID`, `C_DEPT_ID`), 
  KEY idx_t_dept_master (`C_COMPANY_ID`, `C_DEPT_ID`) 
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='부서 정보 테이블';

INSERT IGNORE INTO T_DEPT_MASTER (C_COMPANY_ID, C_DEPT_ID, C_PARENT_ID, C_DEPT_NAME, C_INSERTED_BY_HR_YN, C_INSERT_DATE) VALUES ('00000001', 'NO_DEPT', NULL, '부서미지정', 'Y', UTC_TIMESTAMP());

-- 디스크 드라이브 테이블
CREATE TABLE IF NOT EXISTS T_DISK_INFO ( 
  `C_SEQ_NO` BIGINT(20) NOT NULL AUTO_INCREMENT COMMENT '번호', 
  `C_AGENT_ID` VARCHAR(50) NOT NULL COMMENT 'Agent ID', 
  `C_DISK_NAME` VARCHAR(50) NOT NULL COMMENT '디스크 이름', 
  `C_DISK_TYPE` TINYINT(4) DEFAULT 0 COMMENT '디스크 종류 (0: 알 수 없음, 2: 이동식 디스크, 3: 고정 디스크, 5: CD-ROM, 6:RAM 디스크)', 
  `C_VOLUMN_NAME` VARCHAR(50) DEFAULT NULL COMMENT '볼륨명', 
  `C_FILE_SYSTEM_NAME` VARCHAR(50) DEFAULT NULL COMMENT '파일시스템', 
  `C_TOTAL_SIZE` BIGINT(20) DEFAULT 0 COMMENT '전체 사이즈', 
  `C_FREE_SIZE` BIGINT(20) DEFAULT 0 COMMENT '저장 가능 사이즈', 
  `C_INSERT_DATE` DATETIME DEFAULT NULL COMMENT '생성 일자', 
  `C_UPDATE_DATE` DATETIME DEFAULT NULL COMMENT '수정 일자', 
  PRIMARY KEY (`C_SEQ_NO`), 
  UNIQUE KEY uidx_t_disk_info (`C_AGENT_ID`, `C_DISK_NAME`), 
  KEY idx_t_disk_info (`C_AGENT_ID`, `C_DISK_NAME`) 
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='디스크 드라이브 테이블';

CREATE TABLE IF NOT EXISTS `T_EMPTY_DAYS` (
  `C_DAY_INDEX` INT(11) DEFAULT NULL COMMENT '365날(한 해)',
  `C_DAY_DATE` DATETIME DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='대쉬보드 이동파일 그래프에서 데이터 없는 날짜를 표시';

-- 10 YEARS DAY : 3650 DAYS
INSERT INTO T_EMPTY_DAYS (C_DAY_INDEX) SELECT (t4.n * 1000 + t3.n * 100 + t2.n * 10 + t1.n + 1) AS n
FROM (
    SELECT 0 AS n UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9
) AS t1
CROSS JOIN (
    SELECT 0 AS n UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9
) AS t2
CROSS JOIN (
    SELECT 0 AS n UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9
) AS t3
CROSS JOIN (
    SELECT 0 AS n UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9
) AS t4
WHERE (t4.n * 1000 + t3.n * 100 + t2.n * 10 + t1.n + 1) <= 3650;

CREATE TABLE IF NOT EXISTS `T_ENV_GENERAL` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '자동 생성',
  `company_name` VARCHAR(255) DEFAULT NULL COMMENT '회사 이름',
  `company_id` VARCHAR(50) DEFAULT NULL COMMENT '회사 ID',
  `admin_console_version` VARCHAR(255) DEFAULT NULL COMMENT 'Web Manager version',
  `user_console_version` VARCHAR(255) DEFAULT NULL COMMENT 'Web User version',
  `database_version` VARCHAR(255) DEFAULT NULL COMMENT 'DB version',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- 결재 환경 설정
CREATE TABLE IF NOT EXISTS `T_ENV_REQUEST_APPROVAL` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '자동 생성', 
  `approve_mode` VARCHAR(255) NOT NULL COMMENT '결재자 선택. URL, Admin,User', 
  `external_url` VARCHAR(255) DEFAULT NULL COMMENT '외부 결재 연동URL. link', 
  `reason` TEXT DEFAULT NULL COMMENT '결재 사유', 
  `require_reason_yn` VARCHAR(2) NOT NULL DEFAULT 'N' COMMENT '결재 사유 필수 입력 체크 여부. Y:Yes, N:No', 
  `hint` TEXT DEFAULT NULL COMMENT '힌트. 설명', 
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT '결재 환경 설정';


-- 라이선스 업로드시 시스템의 mac 주소 등록
CREATE TABLE IF NOT EXISTS `T_ENV_SYSTEM_SETTINGS` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '자동 생성', 
  `mac_address` VARCHAR(255) DEFAULT NULL COMMENT 'Mac address 등록', 
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT '라이선스 업로드시 시스템의 mac 주소 등록';

-- 패스워드(인증)
CREATE TABLE IF NOT EXISTS `T_ENV_VERIFICATION` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '자동 생성',
  `user_role` VARCHAR(10) NOT NULL COMMENT '사용자 등급. admin,user',
  `pass_expire_days` INT(11) NOT NULL COMMENT '패스워드 유효 기간. date',
  `pass_min_length` INT(11) NOT NULL COMMENT '패스워드 최소 길이',
  `pass_notify_before` INT(11) NOT NULL COMMENT '패스워드 만료 전 알림',
  `pass_comb_uppercase` INT(11) NOT NULL COMMENT '패스워드 대문자 수. Uppercase',
  `pass_comb_digit` INT(11) NOT NULL COMMENT '패스워드 숫자 수. Number',
  `pass_comb_symbol` INT(11) DEFAULT NULL COMMENT '패스워드 특수문자 수. character',
  PRIMARY KEY (`id`), 
  UNIQUE KEY `uidx_t_env_verfication`(`user_role`) 
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='패스워드(인증)';

INSERT IGNORE INTO T_ENV_VERIFICATION (user_role, pass_expire_days, pass_min_length, pass_notify_before, pass_comb_uppercase, pass_comb_digit, pass_comb_symbol)
VALUES 
('ADMIN', 28, 8, 5, 2, 6, 1), 
('USER', 20, 11, 5, 1, 2, 2);

-- 반출 원본 파일 정보 테이블 (묶여있는 파일들 정보)
CREATE TABLE IF NOT EXISTS T_EX_ARCHIVED_FILE_INFO ( 
  `C_SEQ_NO` BIGINT(20) NOT NULL AUTO_INCREMENT COMMENT '번호', 
  `C_FILE_KEY` VARCHAR(50) NOT NULL COMMENT '파일 고유키. T_EX_FILE_AUTH_INFO.C_FILE_KEY', 
  `C_FILE_NAME` VARCHAR(200) NOT NULL COMMENT '파일 이름', 
  `C_FILE_SIZE` BIGINT DEFAULT 0 COMMENT '파일 크기. Byte 단위', 
  `C_INSERT_DATE` DATETIME DEFAULT NULL COMMENT '생성 일자', 
  `C_UPDATE_DATE` DATETIME DEFAULT NULL COMMENT '수정 일자', 
  PRIMARY KEY (`C_SEQ_NO`), 
  UNIQUE KEY uidx_t_ex_archived_file_info (`C_FILE_KEY`, `C_FILE_NAME`), 
  KEY idx_t_ex_archived_file_info (`C_FILE_KEY`, `C_FILE_NAME`) 
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='반출 원본 파일 정보 테이블 (묶여있는 파일들 정보)';

-- 반출파일 인증 정보 테이블
CREATE TABLE IF NOT EXISTS T_EX_FILE_AUTH_INFO ( 
  `C_SEQ_NO` BIGINT(20) NOT NULL AUTO_INCREMENT COMMENT '번호', 
  `C_FILE_KEY` VARCHAR(50) NOT NULL COMMENT '파일 고유키', 
  `C_COMPANY_ID` VARCHAR(50) DEFAULT NULL COMMENT '회사 ID', 
  `C_AGENT_ID` VARCHAR(50) NOT NULL COMMENT 'Agent ID', 
  `C_USER_ID` VARCHAR(50) NOT NULL COMMENT '사용자 ID', 
  `C_FILE_HASH` VARCHAR(200) DEFAULT NULL COMMENT '파일 해시', 
  `C_DECRYPT_CNT` TINYINT(4) DEFAULT 0 COMMENT '복호화 남은 횟수', 
  `C_EXPIRE_DATE` DATETIME DEFAULT NULL COMMENT '복호화 만료일', 
  `C_FILE_NAME` VARCHAR(200) DEFAULT NULL COMMENT '이동파일 이름', 
  `C_FILE_SIZE` BIGINT(20) DEFAULT 0 COMMENT '이동파이 크기', 
  `C_INSERT_DATE` DATETIME DEFAULT NULL COMMENT '생성 일자', 
  `C_UPDATE_DATE` DATETIME DEFAULT NULL COMMENT '수정 일자', 
  PRIMARY KEY (`C_SEQ_NO`), 
  UNIQUE KEY uidx_t_ex_file_auth_info (`C_FILE_KEY`), 
  KEY idx_t_ex_file_auth_info (`C_USER_ID`,`C_AGENT_ID`,`C_FILE_KEY`) 
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='반출파일 인증 정보 테이블';

-- 이동파일 인증 로그
CREATE TABLE IF NOT EXISTS T_EX_FILE_AUTH_LOG ( 
  `C_SEQ_NO` BIGINT(20) NOT NULL AUTO_INCREMENT COMMENT '번호', 
  `C_LOG_TIME` DATETIME NOT NULL COMMENT '로그 발생 시각', 
  `C_FILE_KEY` VARCHAR(50) NOT NULL COMMENT '파일 고유키',
  `C_RESULT_CODE` VARCHAR(10) DEFAULT NULL COMMENT '결과 코드 (성공(SUCCESS)/실패. 실패코드는 별도 정의)', 
  `C_USER_ID` VARCHAR(50) DEFAULT NULL COMMENT '이동파일 생성 사용자 ID', 
  `C_USER_NAME` VARCHAR(50) DEFAULT NULL COMMENT '이동파일 생성 사용자 이름', 
  `C_AGENT_ID` VARCHAR(50) DEFAULT NULL COMMENT 'Agent ID', 
  `C_REQUEST_IP` VARCHAR(15) DEFAULT NULL COMMENT '이동파일 인증 요청 PC IP 주소', 
  `C_REQUEST_MAC` VARCHAR(30) DEFAULT NULL COMMENT '이동파일 인증 요청 PC MAC 주소', 
  `C_FILE_NAME` VARCHAR(200) DEFAULT NULL COMMENT '이동파일 이름',
  `C_FILE_SIZE` BIGINT(20) DEFAULT 0 COMMENT '이동파일 크기',
  `C_FILE_HASH` VARCHAR(200) DEFAULT NULL COMMENT '이동파일 해시',
  `C_DECRYPT_CNT` TINYINT(4) DEFAULT 0 COMMENT '복호화 남은 횟수', 
  `C_EXPIRE_DATE` DATETIME DEFAULT NULL COMMENT '복호화 만료일', 
  `C_COMMENT` VARCHAR(2048) DEFAULT NULL COMMENT '상세설명', 
  PRIMARY KEY (`C_SEQ_NO`), 
  KEY idx_t_ex_file_auth_log (`C_FILE_KEY`,`C_RESULT_CODE`,`C_USER_ID`,`C_AGENT_ID`,`C_LOG_TIME`)  
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='이동파일 인증 로그';

-- 로그 중간 통계 테이블 (T_EX_FILE_AUTH_LOG 테이블 레코드에 대한 일별/파일별/인증결과별 합계)
CREATE TABLE IF NOT EXISTS T_EX_FILE_AUTH_LOG_STAT ( 
  `C_SEQ_NO` BIGINT(20) NOT NULL AUTO_INCREMENT COMMENT '번호', 
  `C_STAT_DATE` DATE NOT NULL COMMENT '통계 생성 날짜', 
  `C_FILE_KEY` VARCHAR(50) NOT NULL COMMENT '파일 고유키',
  `C_RESULT` BOOLEAN DEFAULT NULL COMMENT '인증결과 구분. NULL:인증 결과(C_RESULT_CODE)가 NULL인 경우 / true:인증성공 / false:인증실패', 
  `C_SUM` INT(11) DEFAULT 0 COMMENT '일별/파일별/인증결과별 발생 로그개수', 
  PRIMARY KEY (`C_SEQ_NO`), 
  UNIQUE KEY uidx_t_ex_file_auth_log_stat (`C_STAT_DATE`,`C_FILE_KEY`,`C_RESULT`), 
  KEY idx_t_ex_file_auth_log_stat (`C_STAT_DATE`,`C_FILE_KEY`)  
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='로그 중간 통계 테이블 (T_EX_FILE_AUTH_LOG 테이블 레코드에 대한 일별/파일별/인증결과별 합계)';

-- 외부 다운로드 서버 정보 테이블
CREATE TABLE IF NOT EXISTS T_EX_SERVER_INFO ( 
  `C_SEQ_NO` BIGINT(20) NOT NULL AUTO_INCREMENT COMMENT '번호', 
  `C_SERVER_NAME` VARCHAR(1024) DEFAULT NULL COMMENT '외부서버 이름', 
  `C_SERVER_URL` VARCHAR(1024) DEFAULT NULL COMMENT '업데이트 파일 다운로드 URL', 
  `C_INSERT_DATE` DATETIME DEFAULT NULL COMMENT '생성 일자', 
  `C_UPDATE_DATE` DATETIME DEFAULT NULL COMMENT '수정 일자', 
  PRIMARY KEY (`C_SEQ_NO`), 
  UNIQUE KEY uidx_t_ex_server_info (`C_SERVER_NAME`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='외부 다운로드 서버 정보 테이블';

-- 업로드 되는 모든 파일 기록
CREATE TABLE IF NOT EXISTS `T_FILE` (
  `file_id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '자동 생성', 
  `file_grp_id` INT(11) DEFAULT NULL COMMENT '사용자가 등록한 여러개의 파일을 묶어 하나의 그룹으로 만듬', 
  `file_nm` VARCHAR(255) DEFAULT NULL COMMENT '파일명', 
  `file_ext` VARCHAR(255) DEFAULT NULL COMMENT '파일 확장자', 
  `file_size` INT(11) DEFAULT NULL COMMENT '파일 크기', 
  `down_cnt` INT(11) DEFAULT NULL COMMENT '다운로드 카운트 수', 
  `stored_nm` VARCHAR(255) DEFAULT NULL COMMENT '저장 이름', 
  `stored_path` VARCHAR(255) DEFAULT NULL COMMENT '저장 경로', 
  `url` tinytext DEFAULT NULL COMMENT 'url', 
  `download_url` VARCHAR(255) DEFAULT NULL COMMENT '다운로드 url', 
  `delete_url` VARCHAR(255) DEFAULT NULL COMMENT '삭제 url', 
  `is_deleted` VARCHAR(2) NOT NULL DEFAULT 'N' COMMENT '삭제 여부', 
  `created_date` DATETIME DEFAULT NULL COMMENT '생성 날짜', 
  `file_original_nm` VARCHAR(255) DEFAULT NULL COMMENT '파일 원본명', 
  PRIMARY KEY (`file_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT '업로드 되는 모든 파일 기록';

-- 파일 그룹
CREATE TABLE IF NOT EXISTS `T_FILE_GROUP` (
  `file_grp_id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '자동 생성', 
  `created_date` DATETIME DEFAULT NULL COMMENT '생성 날짜', 
  PRIMARY KEY (`file_grp_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT '파일 그룹';

-- 파일 존
CREATE TABLE IF NOT EXISTS `T_FILE_ZONE` (
  `C_ZONE_ID` int(11) NOT NULL AUTO_INCREMENT COMMENT 'ZONE ID',
  `C_ZONE_NM` varchar(255) NOT NULL COMMENT 'ZONE명',
  `C_PROTO` enum('http','https','ftp','sftp') NOT NULL COMMENT '프로토콜',
  `C_ZONE_DATA` varchar(2000) NOT NULL COMMENT 'Zone 관련 데이터(JSON)',
  PRIMARY KEY (`C_ZONE_ID`)
) ENGINE=InnoDB AUTO_INCREMENT=25 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci COMMENT='파일 존 테이블';


-- 라이선스 등록
CREATE TABLE IF NOT EXISTS `T_LICENSE_DETAIL` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '자동 생성', 
  `status_cd` VARCHAR(255) DEFAULT NULL COMMENT '상태 코드. USING. 사용 여부', 
  `product_name` VARCHAR(255) DEFAULT NULL COMMENT '제품명. 정책 제품. zonezentral,orizon,cryhod', 
  `quantity` INT(11) DEFAULT NULL COMMENT '라이선스 개수', 
  `start_date` VARCHAR(255) DEFAULT NULL COMMENT '라이선스 시작일', 
  `end_date` VARCHAR(255) DEFAULT NULL COMMENT '라이선스 만료일', 
  `general_id` INT(11) NOT NULL COMMENT '라이선스 general id', 
  `product_key` VARCHAR(255) DEFAULT NULL COMMENT '라이선스 제품 키', 
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT '라이선스 등록';

-- 라이선스
CREATE TABLE IF NOT EXISTS `T_LICENSE_GENERAL` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '자동 생성', 
  `company` VARCHAR(255) NOT NULL COMMENT '회사', 
  `mac_address` VARCHAR(255) NOT NULL COMMENT 'mac address', 
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT '라이선스';

-- 관리자 접속 허용 IP 리스트
CREATE TABLE IF NOT EXISTS `T_MANAGER_ALLOWED_IP` (
  `C_SEQ_NO` BIGINT(20) NOT NULL AUTO_INCREMENT COMMENT '번호', 
  `C_MANAGER_ID` VARCHAR(50) NOT NULL COMMENT '관리자 ID',
  `C_IP` VARCHAR(15) DEFAULT NULL COMMENT '허용 IP 주소', 
  `C_INSERT_DATE` DATETIME DEFAULT NULL COMMENT '생성 일자', 
  `C_UPDATE_DATE` DATETIME DEFAULT NULL COMMENT '수정 일자', 
  PRIMARY KEY (`C_SEQ_NO`),
  UNIQUE KEY `uidx_t_manager_allowed_ip` (`C_MANAGER_ID`, `C_IP`),
  KEY `idx_t_manager_allowed_ip` (`C_MANAGER_ID`, `C_IP`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='관리자 접속 허용 IP 리스트'; 

-- 관리자 마스터 테이블
CREATE TABLE IF NOT EXISTS `T_MANAGER_MASTER` (
  `C_SEQ_NO` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '번호',
  `C_MANAGER_ID` varchar(50) NOT NULL COMMENT '관리자 ID',
  `C_MANAGER_PWD` varchar(200) NOT NULL DEFAULT '$2a$10$Y0LyC8NurbWUWoOEVn5iIucnZevtA46/cA/ScqILwPVW2tm9wkF22' COMMENT '암호. 기본 패스워드: Password123*',
  `C_MANAGER_NAME` varchar(50) DEFAULT NULL COMMENT '관리자 이름',
  `C_MANAGER_LEVEL` tinyint(11) DEFAULT 2 COMMENT '관리자 권한. 1: admin / 2: 일반관리자',
  `C_EMAIL_ADDR` varchar(50) DEFAULT NULL COMMENT 'E-Mail',
  `C_USE_IP_YN` char(1) NOT NULL DEFAULT 'N' COMMENT '관리자 접속 IP 적용 여부',
  `C_RESERVED1` varchar(260) DEFAULT NULL COMMENT '예약 컬럼 1',
  `C_RESERVED2` varchar(260) DEFAULT NULL COMMENT '예약 컬럼 2',
  `C_RESERVED3` varchar(260) DEFAULT NULL COMMENT '예약 컬럼 3',
  `C_RESERVED4` varchar(260) DEFAULT NULL COMMENT '예약 컬럼 4',
  `C_RESERVED5` varchar(260) DEFAULT NULL COMMENT '예약 컬럼 5',
  `C_INSERT_DATE` datetime DEFAULT NULL COMMENT '생성 일자',
  `C_UPDATE_DATE` datetime DEFAULT NULL COMMENT '수정 일자',
  `C_EXPIRED_DATE` datetime DEFAULT NULL COMMENT '관리자 사용 기간 만료 기간',
  `C_PWD_EXPIRED_DATE` datetime DEFAULT NULL COMMENT '관리자 패스워드 기간',
  `C_PWD_CHANGED` char(1) DEFAULT 'N' COMMENT '관리자 패스워드 초기화',
  `C_BEGIN_DATE` datetime DEFAULT NULL COMMENT '관리자 사용 기간 시작일',
  `C_IS_PERIOD` char(1) DEFAULT 'N' COMMENT '사용 기간을 지정했는지 여부',
  `C_IS_ACTIVE` char(1) NOT NULL DEFAULT 'Y' COMMENT '관리자 계정 활성 여부',
  PRIMARY KEY (`C_SEQ_NO`),
  UNIQUE KEY `uidx_t_manager_master` (`C_MANAGER_ID`),
  KEY `idx_t_manager_master` (`C_MANAGER_ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8 COMMENT='관리자 마스터 테이블';


-- 관리/운용/설정 관련 로그
CREATE TABLE IF NOT EXISTS T_MANAGE_LOG ( 
  `C_SEQ_NO` BIGINT(20) NOT NULL AUTO_INCREMENT COMMENT '번호', 
  `C_LOG_TIME` DATETIME NOT NULL COMMENT '로그 발생 시각', 
  `C_LOG_CODE` VARCHAR(10) NOT NULL COMMENT '로그 코드', 
  `C_LOG_TYPE` TINYINT(4) DEFAULT 0 COMMENT '로그 발생 주체. 0: 웹매니저(Default), 1:서버', 
  `C_USER_ID` VARCHAR(50) DEFAULT NULL COMMENT '사용자 ID', 
  `C_USER_NAME` VARCHAR(50) DEFAULT NULL COMMENT '사용자 이름', 
  `C_USER_IP` VARCHAR(15) DEFAULT NULL COMMENT '웹매니저에 접속한 IP', 
  `C_AGENT_ID` VARCHAR(50) DEFAULT NULL COMMENT 'Agent ID', 
  `C_AGENT_IP` VARCHAR(15) DEFAULT NULL COMMENT 'Agent의 IP', 
  `C_COMPUTER_NAME` VARCHAR(260) DEFAULT NULL COMMENT '컴퓨터 이름',
  `C_PRODUCT_POLICY_ID` INT(11) DEFAULT NULL COMMENT 'PRIMX 제품 정책 ID. policy.id', 
  `C_PRODUCT_POLICY_NAME` VARCHAR(255) DEFAULT NULL COMMENT 'PRIMX 제폼 정책 이름. polixy.polixy_name', 
  `C_AGENT_POLICY_ID` INT(11) DEFAULT NULL COMMENT 'Agent 정책 ID. T_AGENT_POLICY_MASTER.C_POLICY_ID', 
  `C_AGENT_POLICY_NAME` VARCHAR(255) DEFAULT NULL COMMENT 'Agent 정책 이름. T_AGENT_POLICY_MASTER.C_POLICY_NAME', 
  `C_RESULT_CODE` VARCHAR(10) DEFAULT NULL COMMENT '결과 코드 (성공/실패. 실패코드는 별도 정의)', 
  `C_RESERVED1` VARCHAR(100) DEFAULT NULL COMMENT '예약컬럼 1',
  `C_RESERVED2` VARCHAR(100) DEFAULT NULL COMMENT '예약컬럼 2',
  `C_RESERVED3` VARCHAR(100) DEFAULT NULL COMMENT '예약컬럼 3',
  `C_RESERVED4` VARCHAR(100) DEFAULT NULL COMMENT '예약컬럼 4',
  `C_RESERVED5` VARCHAR(100) DEFAULT NULL COMMENT '예약컬럼 5',
  `C_RESERVED6` VARCHAR(100) DEFAULT NULL COMMENT '예약컬럼 6',
  `C_COMMENT` VARCHAR(2048) DEFAULT NULL COMMENT '상세설명', 
  `C_WORKER_NAME` VARCHAR(50) DEFAULT NULL COMMENT '작업자명', 
  PRIMARY KEY (`C_SEQ_NO`), 
  KEY idx_t_mamage_log (`C_LOG_CODE`,`C_LOG_TYPE`,`C_LOG_TIME`)  
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='관리/운용/설정 관련 로그';

-- 로그 중간 통계 테이블 (T_MANAGE_LOG 테이블 레코드에 대한 일별/사용자별/로그코드별/수행결과별 합계)
CREATE TABLE IF NOT EXISTS T_MANAGE_LOG_STAT ( 
  `C_SEQ_NO` BIGINT(20) NOT NULL AUTO_INCREMENT COMMENT '번호', 
  `C_STAT_DATE` DATE NOT NULL COMMENT '통계 생성 날짜', 
  `C_USER_ID` VARCHAR(50) NOT NULL COMMENT '사용자 ID', 
  `C_USER_NAME` VARCHAR(50) DEFAULT NULL COMMENT '사용자 이름', 
  `C_LOG_CODE` VARCHAR(10) NOT NULL COMMENT '로그 코드', 
  `C_RESULT` BOOLEAN DEFAULT NULL COMMENT '수행결과 구분. NULL:수행 결과(C_RESULT_CODE)가 NULL인 경우 / true:수행성공 / false:수행실패', 
  `C_SUM` INT(11) DEFAULT 0 COMMENT '일별/사용자별/로그코드별/수행결과별 발생 로그개수', 
  PRIMARY KEY (`C_SEQ_NO`), 
  UNIQUE KEY uidx_t_manage_log_stat (`C_STAT_DATE`,`C_USER_ID`,`C_LOG_CODE`,`C_RESULT`), 
  KEY idx_t_manage_log_stat (`C_STAT_DATE`,`C_USER_ID`,`C_LOG_CODE`)  
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='로그 중간 통계 테이블 (T_MANAGE_LOG 테이블 레코드에 대한 일별/사용자별/로그코드별/수행결과별 합계)';

-- Agent NIC 정보 테이블
CREATE TABLE IF NOT EXISTS T_NIC_INFO ( 
  `C_SEQ_NO` BIGINT(20) NOT NULL AUTO_INCREMENT COMMENT '번호', 
  `C_AGENT_ID` VARCHAR(50) NOT NULL COMMENT 'Agent ID', 
  `C_IP` VARCHAR(15) DEFAULT NULL COMMENT 'IP 주소', 
  `C_MAC` VARCHAR(30) NOT NULL COMMENT 'MAC 주소', 
  `C_NAME` VARCHAR(200) DEFAULT NULL COMMENT '어뎁터 이름', 
  `C_GATEWAY` VARCHAR(15) DEFAULT NULL COMMENT '게이트웨이', 
  `C_DNS` VARCHAR(15) DEFAULT NULL COMMENT 'DNS 서버주소', 
  `C_DHCP_ENABLE` TINYINT(4) DEFAULT 0 COMMENT 'DHCP 사용 여부 (0: Static IP, 1: DHCP IP)', 
  `C_DHCP_SERVER` VARCHAR(15) DEFAULT NULL COMMENT 'DHCP 서버주소', 
  `C_INSERT_DATE` DATETIME DEFAULT NULL COMMENT '생성 일자', 
  `C_UPDATE_DATE` DATETIME DEFAULT NULL COMMENT '수정 일자', 
  PRIMARY KEY (`C_SEQ_NO`), 
  UNIQUE KEY uidx_t_nic_info (`C_AGENT_ID`, `C_MAC`), 
  KEY idx_t_nic_info (`C_AGENT_ID`, `C_MAC`) 
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Agent NIC 정보 테이블';

-- 신청 결재 설정 테이블
CREATE TABLE `T_NOTICE_SETTING` (
  `NOTICE_SEQ` int(11) NOT NULL AUTO_INCREMENT COMMENT '공지 ID. 자동 생성',
  `REQUEST_USER_ROLE` varchar(255) DEFAULT NULL COMMENT 'ADMIN(관리자), USER(사용자)',
  `REQUEST_APPROVAL_CODE` char(1) DEFAULT 'C' COMMENT '이동파일 생성 신청 : M, 사용자 등록 신청 : A, 계정연장신청 : C',
  `APPROV_TARGET` char(2) DEFAULT NULL COMMENT '결재대상 U:사용자, A:관리자, AP : 결재자',
  `DELEGATED_YN` char(1) NOT NULL DEFAULT 'N' COMMENT '전결 설정 여부 (Y/N)',
  `NOTICE_CONTENT` varchar(255) DEFAULT NULL COMMENT '공지 내용',
  `NOTICE_REASON` varchar(255) DEFAULT NULL COMMENT '사유',
  `USE_YN` char(1) DEFAULT NULL COMMENT '사용 유무 Y=사용, N=미사용',
  `CREATED_DATE` datetime DEFAULT NULL COMMENT '생성일',
  `MODIFIED_DATE` datetime DEFAULT NULL COMMENT '수정일',
  `REASON_USED_YN` char(1) DEFAULT 'Y' COMMENT '사용 유무 Y=사용, N=미사용',
  PRIMARY KEY (`NOTICE_SEQ`)
) ENGINE=InnoDB AUTO_INCREMENT=25 DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci COMMENT='공지 등록 테이블';

INSERT IGNORE INTO PRIMXDB.T_NOTICE_SETTING
(NOTICE_SEQ, REQUEST_USER_ROLE, REQUEST_APPROVAL_CODE, APPROV_TARGET, DELEGATED_YN, NOTICE_CONTENT, NOTICE_REASON, USE_YN, CREATED_DATE, MODIFIED_DATE, REASON_USED_YN)
VALUES(19, 'USER', 'M', 'U', 'N', '◉ 결재선 : 작성자 → 부서팀장 → 부서임원 → 사장 → 대표이사 (이동파일 사용자)', '사유를 입력해주세요.', 'Y', '2025-01-25 16:21:00.000', '2025-03-28 15:44:52.000', 'Y');
INSERT IGNORE INTO PRIMXDB.T_NOTICE_SETTING
(NOTICE_SEQ, REQUEST_USER_ROLE, REQUEST_APPROVAL_CODE, APPROV_TARGET, DELEGATED_YN, NOTICE_CONTENT, NOTICE_REASON, USE_YN, CREATED_DATE, MODIFIED_DATE, REASON_USED_YN)
VALUES(20, 'ADMIN', 'M', 'U', 'N', '◉ 결재선 : 작성자 → 부서팀장 → 부서임원 → 사장 → 대표이사 (이동파일 관리자)', '사유는 [] 입니다.', 'Y', '2025-01-25 16:21:00.000', '2025-03-28 15:44:57.000', 'Y');
INSERT IGNORE INTO PRIMXDB.T_NOTICE_SETTING
(NOTICE_SEQ, REQUEST_USER_ROLE, REQUEST_APPROVAL_CODE, APPROV_TARGET, DELEGATED_YN, NOTICE_CONTENT, NOTICE_REASON, USE_YN, CREATED_DATE, MODIFIED_DATE, REASON_USED_YN)
VALUES(21, 'USER', 'C', 'A', 'N', '◉ 결재선 : 작성자 → 부서팀장 → 부서임원 → 사장 → 대표이사 (계정연장 사용자)', '사유를 입력해주세요.', 'Y', '2025-01-25 16:21:00.000', '2025-05-09 07:30:06.000', 'Y');
INSERT IGNORE INTO PRIMXDB.T_NOTICE_SETTING
(NOTICE_SEQ, REQUEST_USER_ROLE, REQUEST_APPROVAL_CODE, APPROV_TARGET, DELEGATED_YN, NOTICE_CONTENT, NOTICE_REASON, USE_YN, CREATED_DATE, MODIFIED_DATE, REASON_USED_YN)
VALUES(22, 'ADMIN', 'C', 'A', 'N', '◉ 결재선 : 작성자 → 부서팀장 → 부서임원 → 사장 → 대표이사 (계정연장 관리자)', '사유를 입력해주세요.', 'Y', '2025-01-25 16:21:00.000', '2025-05-09 07:30:11.000', 'Y');
INSERT IGNORE INTO PRIMXDB.T_NOTICE_SETTING
(NOTICE_SEQ, REQUEST_USER_ROLE, REQUEST_APPROVAL_CODE, APPROV_TARGET, DELEGATED_YN, NOTICE_CONTENT, NOTICE_REASON, USE_YN, CREATED_DATE, MODIFIED_DATE, REASON_USED_YN)
VALUES(23, 'USER', 'A', 'A', 'N', '◉ 결재선을 지정하는 방법
', '사유를 입력해주세요.', 'Y', '2025-01-25 16:21:00.000', '2025-04-03 11:29:22.000', 'Y');
INSERT IGNORE INTO PRIMXDB.T_NOTICE_SETTING
(NOTICE_SEQ, REQUEST_USER_ROLE, REQUEST_APPROVAL_CODE, APPROV_TARGET, DELEGATED_YN, NOTICE_CONTENT, NOTICE_REASON, USE_YN, CREATED_DATE, MODIFIED_DATE, REASON_USED_YN)
VALUES(24, 'ADMIN', 'A', 'A', 'N', '◉ 결재선 : 작성자 → 부서팀장 → 부서임원 → 사장 → 대표이사 (사용자등록 관리자)', '사유를 입력해주세요.. 
', 'Y', '2025-01-25 16:21:00.000', '2025-04-15 14:32:51.000', 'Y');



-- 시스템의 비암호화 영역 정보 테이블
CREATE TABLE IF NOT EXISTS  T_NOT_ENCRYPTION_PATH_INFO ( 
  `C_SEQ_NO` BIGINT(20) NOT NULL AUTO_INCREMENT COMMENT '번호', 
  `C_AGENT_POLICY_CODE` varchar(50) NOT NULL COMMENT 'Agent 정책 Code',
  `C_NOT_ENCRYPT_PATH` VARCHAR(600) DEFAULT NULL COMMENT '비암호화 영역 경로', 
  `C_INSERT_DATE` DATETIME DEFAULT NULL COMMENT '생성 일자', 
  `C_UPDATE_DATE` DATETIME DEFAULT NULL COMMENT '수정 일자', 
  PRIMARY KEY (`C_SEQ_NO`), 
  KEY idx_t_not_encryption_path_info (`C_AGENT_POLICY_CODE`) 
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='시스템의 비암호화 영역 정보 테이블';

INSERT IGNORE INTO T_NOT_ENCRYPTION_PATH_INFO (C_SEQ_NO, C_AGENT_POLICY_CODE, C_NOT_ENCRYPT_PATH, C_INSERT_DATE) VALUES
(1, "", "C:\Program Files", UTC_TIMESTAMP()),
(2, "", "C:\Program Files (x86)", UTC_TIMESTAMP()),
(3, "", "C:\Windows", UTC_TIMESTAMP()),
(4, "AGN-2020010100001", "C:\Program Files", UTC_TIMESTAMP()),
(5, "AGN-2020010100001", "C:\Program Files (x86)", UTC_TIMESTAMP()),
(6, "AGN-2020010100001", "C:\Windows", UTC_TIMESTAMP());

-- PRIMX 제품 설치 정보 테이블
CREATE TABLE IF NOT EXISTS T_PRIMX_SETUP_INFO ( 
  `C_SEQ_NO` BIGINT(20) NOT NULL AUTO_INCREMENT COMMENT '번호', 
  `C_AGENT_ID` VARCHAR(50) NOT NULL COMMENT 'Agent ID', 
  `C_PRODUCT_TYPE` TINYINT(4) DEFAULT 0 COMMENT '제품 종류 (0: 알 수 없음, 1: PRIMX ZONECENTRAL, 2: PRIMX ORIZON, 3: PRIMX CRYHOD, 4: PRIMX Agent)', 
  `C_PRODUCT_VERSION` VARCHAR(50) DEFAULT NULL COMMENT '제품 버전', 
  `C_LAST_POLICY_DATE` DATETIME DEFAULT NULL COMMENT '마지막 정책 반영 일자', 
  `C_INSERT_DATE` DATETIME DEFAULT NULL COMMENT '생성 일자', 
  `C_UPDATE_DATE` DATETIME DEFAULT NULL COMMENT '수정 일자', 
  PRIMARY KEY (`C_SEQ_NO`), 
  UNIQUE KEY uidx_t_primx_setup_info (`C_AGENT_ID`, `C_PRODUCT_TYPE`), 
  KEY idx_t_primx_setup_info (`C_AGENT_ID`) 
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='PRIMX 제품 설치 정보 테이블';

-- 제품
CREATE TABLE IF NOT EXISTS `T_PRODUCTS` (
  `id` INT(11) NOT NULL COMMENT '제폼 ID',
  `product_name` VARCHAR(255) NOT NULL COMMENT '제품명. Zonecentral, Orizon, Cryhod',
  PRIMARY KEY (`id`), 
  UNIQUE KEY uidx_products (`product_name`) 
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT '제품';

INSERT IGNORE INTO `T_PRODUCTS` (`id`, `product_name`) VALUES (0, 'unknown'), (1, 'ZONECENTRAL'), (2, 'ORIZON'), (3, 'CRYHOD'), (4, 'Agent');

-- 정책등록 (수정)
CREATE TABLE IF NOT EXISTS `T_POLICY` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '자동 생성',
  `policy_code` VARCHAR(255) NOT NULL COMMENT '정책 코드. POL-date-ordernumber. POL-2023050500001식으로 생성 됨',
  `policy_name` VARCHAR(255) DEFAULT NULL COMMENT '정책명',
  `policy_json` TEXT DEFAULT NULL COMMENT '정책 json. Json 확장자로 저장이 됨',
  `policy_xml` BLOB DEFAULT NULL COMMENT '정책 xml. Xml tag으로 저장 및 Xml 파일로 저장',
  `product_id` INT(11) DEFAULT NULL COMMENT '정책 id. 해당 제품 ID. 1:ZoneCentral, 2:Orizon, 3:CRYHOD',
  `created_date` DATETIME DEFAULT NULL COMMENT '등록 날짜',
  `created_by` INT(11) DEFAULT NULL COMMENT '등록자',
  `modified_date` DATETIME DEFAULT NULL COMMENT '수정일',
  `modified_by` INT(11) DEFAULT NULL COMMENT '수정자',
  `is_deleted` VARCHAR(2) NOT NULL DEFAULT 'N' COMMENT '삭제 여부. Y:Yes, N:No',
  `use_yn` VARCHAR(2) DEFAULT 'Y' COMMENT '사용 여부. Y:Yes, N:No',
  `is_default` VARCHAR(2) NOT NULL DEFAULT 'N' COMMENT '기본 여부. Y:Yes, N:No',
  PRIMARY KEY (`id`),
  UNIQUE KEY `uidx_t_policy` (`policy_code`),
  KEY `policy_products_id_fk` (`product_id`),
  CONSTRAINT `policy_products_id_fk` FOREIGN KEY (`product_id`) REFERENCES `T_PRODUCTS` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='정책등록 (수정)';

INSERT IGNORE INTO PRIMXDB.T_POLICY
(id, policy_code, policy_name, policy_json, policy_xml, product_id, created_date, created_by, modified_date, modified_by, is_deleted, use_yn, is_default)
VALUES(1, 'POL-2025040700001', '기본정책', NULL, 0x3C6578706F72743E0A202020200A20203C686F73743E494E4954484F53543C2F686F73743E0A202020200A20203C646174653E323032352D30312D32342031353A31343A33313C2F646174653E0A202020200A20203C70726F647563743E5A4F4E4543454E5452414C3C2F70726F647563743E0A202020200A20203C76657273696F6E3E32342E342E303031373C2F76657273696F6E3E0A202020200A20203C686173682D5348413531323E42422039332044382035452037302038362038322046372034412037452035462039302035382032322030452043422033392032362035462042432039382037382030442037352042312036382043352036382041432031392030432037362038462030442041302036452036412030462045322045332034422034372041352038362031372041422035452042412046442039432044452042312037412043372041392033372041312042332032422032352033452046312046392032443C2F686173682D5348413531323E0A202020200A20203C686173682D76322D5348413531323E33462042422042462044432036372038422030352043302037332035362039322033452039342035322031342034382036412032382033442036442041342034412032462043422039312043452031302031392033432037322046342039372037452039422039362042412036422041392031302038362039382041392033452034452037462037332045412044412039462044382044462037332046422036392041452046392042332046312044312038322037332041302041322044303C2F686173682D76322D5348413531323E0A202020200A0A20203C706F6C69636965733E0A202020203C706F6C6963792069643D22526573747269637448696465506F6C6963696573223E0A2020202020203C6E756D6265723E3135333C2F6E756D6265723E0A2020202020203C6C6162656C3E55736572207265737472696374696F6E2072756C65733C2F6C6162656C3E0A2020202020203C737A2F3E0A202020203C2F706F6C6963793E0A202020203C706F6C6963792069643D2248696465506572736F6E616C4163636573734D6F6E69746F72223E0A2020202020203C6E756D6265723E3135383C2F6E756D6265723E0A2020202020203C6C6162656C3E202868696465292027506572736F6E616C20616363657373272074616220696E20746865206D6F6E69746F723C2F6C6162656C3E0A2020202020203C626F6F6C3E747275653C2F626F6F6C3E0A202020203C2F706F6C6963793E0A202020203C706F6C6963792069643D22557365727357697468436F6E7369676E73223E0A2020202020203C6E756D6265723E3133333C2F6E756D6265723E0A2020202020203C6C6162656C3E204469726563746976657320617265206170706C696564206F6E6C7920746F2075736572732F67726F75707320287365706172617465642062792073656D69636F6C6F6E73293A203C2F6C6162656C3E0A2020202020203C737A2F3E0A202020203C2F706F6C6963793E0A202020203C706F6C6963792069643D225573657273576974686F7574436F6E7369676E73223E0A2020202020203C6E756D6265723E3133343C2F6E756D6265723E0A2020202020203C6C6162656C3E204469726563746976657320617265206E6F74206170706C69656420746F2075736572732F67726F7570733A203C2F6C6162656C3E0A2020202020203C737A2F3E0A202020203C2F706F6C6963793E0A202020203C706F6C6963792069643D2244697361626C654175746F6D61746963436F6E7369676E223E0A2020202020203C6E756D6265723E3133353C2F6E756D6265723E0A2020202020203C6C6162656C3E204170706C69636174696F6E206F7074696F6E7320666F72206C6F67696E20696E737472756374696F6E733C2F6C6162656C3E0A2020202020203C696E743E323C2F696E743E0A202020203C2F706F6C6963793E0A202020203C706F6C6963792069643D22436F6E7369676E73223E0A2020202020203C6E756D6265723E3133323C2F6E756D6265723E0A2020202020203C6C6162656C3E456E6372797074696F6E20646972656374697665733C2F6C6162656C3E0A2020202020203C6C6973743E0A20202020202020203C6E616D653E443A5C7361666574797A6F6E653C2F6E616D653E0A20202020202020203C76616C3E63727970743D313C2F76616C3E0A2020202020203C2F6C6973743E0A202020203C2F706F6C6963793E0A202020203C706F6C6963792069643D22417070466F6C6465727350726976696C65676573223E0A2020202020203C6E756D6265723E3335343C2F6E756D6265723E0A2020202020203C6C6162656C3E204170706C69636174696F6E7320746861742073686F756C64206265206772616E746564207370656369616C2070726976696C65676573206F6E20737065636966696320666F6C646572733C2F6C6162656C3E0A2020202020203C6C6973743E0A20202020202020203C6E616D653E2A3C2F6E616D653E0A20202020202020203C76616C3E2A3D7B64656E7972656164656E637279707465643D317D3C2F76616C3E0A2020202020203C2F6C6973743E0A2020202020203C6C6973743E0A20202020202020203C6E616D653E6578706C6F7265722E6578653C2F6E616D653E0A20202020202020203C76616C3E2A3D7B64656E7972656164656E637279707465643D317D3C2F76616C3E0A2020202020203C2F6C6973743E0A2020202020203C6C6973743E0A20202020202020203C6E616D653E5052494D584167656E742E6578653C2F6E616D653E0A20202020202020203C76616C3E2A3D7B64656E7972656164656E637279707465643D307D3C2F76616C3E0A2020202020203C2F6C6973743E0A2020202020203C6C6973743E0A20202020202020203C6E616D653E5052494D584167656E7443532E6578653C2F6E616D653E0A20202020202020203C76616C3E2A3D7B64656E7972656164656E637279707465643D307D3C2F76616C3E0A2020202020203C2F6C6973743E0A2020202020203C6C6973743E0A20202020202020203C6E616D653E77696E776F72642E6578653C2F6E616D653E0A20202020202020203C76616C3E2A3D7B64656E7972656164656E637279707465643D303B64656E797772697465636C6561723D317D3B255553455250524F46494C45255C417070446174613D7B64656E797772697465636C656172203D307D3C2F76616C3E0A2020202020203C2F6C6973743E0A202020203C2F706F6C6963793E0A202020203C706F6C6963792069643D22506572736F6E616C41636365737346696C654C6F636174696F6E223E0A2020202020203C6E756D6265723E3132303C2F6E756D6265723E0A2020202020203C6C6162656C3E22506572736F6E616C20616363657373206C697374206C6F636174696F6E223C2F6C6162656C3E0A2020202020203C657870616E64737A3E25435349444C5F41505044415441255C5A4F4E4543454E5452414C3C2F657870616E64737A3E0A202020203C2F706F6C6963793E0A202020203C706F6C6963792069643D2241636365737346696C65734C6F636174696F6E223E0A2020202020203C6E756D6265723E3132313C2F6E756D6265723E0A2020202020203C6C6162656C3E22416363657373206C69737473206C6F636174696F6E223C2F6C6162656C3E0A2020202020203C657870616E64737A3E25435349444C5F434F4D4D4F4E5F41505044415441255C5A4F4E4543454E5452414C5C41636365737346696C65733C2F657870616E64737A3E0A202020203C2F706F6C6963793E0A202020203C706F6C6963792069643D2241636365737346696C657343616368654C6F636174696F6E223E0A2020202020203C6E756D6265723E3132323C2F6E756D6265723E0A2020202020203C6C6162656C3E22416363657373206C69737473206361636865206C6F636174696F6E223C2F6C6162656C3E0A2020202020203C657870616E64737A3E25435349444C5F434F4D4D4F4E5F41505044415441255C5A4F4E4543454E5452414C5C43616368653C2F657870616E64737A3E0A202020203C2F706F6C6963793E0A202020203C706F6C6963792069643D22506572736F6E616C4261636B75704C6F636174696F6E223E0A2020202020203C6E756D6265723E3132373C2F6E756D6265723E0A2020202020203C6C6162656C3E22506572736F6E616C206261636B7570206C6F636174696F6E223C2F6C6162656C3E0A2020202020203C657870616E64737A3E25435349444C5F4C4F43414C5F41505044415441255C5A4F4E4543454E5452414C5C4261636B75703C2F657870616E64737A3E0A202020203C2F706F6C6963793E0A202020203C706F6C6963792069643D22436F6D6D616E644C6F6746696C65223E0A2020202020203C6E756D6265723E3330393C2F6E756D6265723E0A2020202020203C6C6162656C3E224C6F672066696C6573223C2F6C6162656C3E0A2020202020203C657870616E64737A3E25435349444C5F434F4D4D4F4E5F41505044415441255C5A4F4E4543454E5452414C5C4C6F67733C2F657870616E64737A3E0A202020203C2F706F6C6963793E0A202020203C706F6C6963792069643D225A6F6E654D61705075626C6973684C6F636174696F6E73223E0A2020202020203C6E756D6265723E3334333C2F6E756D6265723E0A2020202020203C6C6162656C3E224578706F72742F7361766520746865207A6F6E65206D6170223C2F6C6162656C3E0A2020202020203C657870616E64737A3E25435349444C5F434F4D4D4F4E5F41505044415441255C5A4F4E4543454E5452414C5C5A6F6E654D61703D7B6D6178686973746F72793D31307D3C2F657870616E64737A3E0A202020203C2F706F6C6963793E0A202020203C706F6C6963792069643D224B65794D61705075626C6973684C6F636174696F6E73223E0A2020202020203C6E756D6265723E3334353C2F6E756D6265723E0A2020202020203C6C6162656C3E224578706F72742F7361766520746865206B6579206D6170223C2F6C6162656C3E0A2020202020203C657870616E64737A3E25435349444C5F434F4D4D4F4E5F41505044415441255C5A4F4E4543454E5452414C5C4B65794D61703C2F657870616E64737A3E0A202020203C2F706F6C6963793E0A202020203C706F6C6963792069643D224D616E6461746F72794D656D62657273223E0A2020202020203C6E756D6265723E3133313C2F6E756D6265723E0A2020202020203C6C6162656C3E4D616E6461746F7279206D656D626572733C2F6C6162656C3E0A2020202020203C6C6973743E0A20202020202020203C6E616D653E4D616E6461746F72792E7A61663C2F6E616D653E0A20202020202020203C76616C3E7368613235363D37462037382034412035382030342044462042312042302031422046382035372038382032322041322038312030392038332038312030362030362030332030372046302046362044432039302046432038422038352034312039352036343C2F76616C3E0A2020202020203C2F6C6973743E0A2020202020203C6C6973743E0A20202020202020203C6E616D653E48656C704465736B2E7A61663C2F6E616D653E0A20202020202020203C76616C3E7368613235363D35312036392041362044452043462046392032452046452041432039362038302033302033462041362038422045322039322036362038412046332033332034332039352038362033312035382037332044302037452031372035412039383B68656C706465736B3D313C2F76616C3E0A2020202020203C2F6C6973743E0A202020203C2F706F6C6963793E0A202020203C706F6C6963792069643D225265736572766564223E0A2020202020203C6E756D6265723E3439393C2F6E756D6265723E0A2020202020203C6C6162656C3E225265736572766564223C2F6C6162656C3E0A2020202020203C6C6973743E0A20202020202020203C6E616D653E4170706C794B696C6C50726F633C2F6E616D653E0A20202020202020203C76616C3E6D736564676577656276696577322E6578653D7B73746172746F726465723D313B436C6F7365496E51756965744D6F64653D313B4E6576657261736B3D317D3B6D737465616D732E6578653D7B73746172746F726465723D313B436C6F7365496E51756965744D6F64653D313B4E6576657261736B3D317D3B53746172744D656E75457870657269656E6365486F73742E6578653D7B73746172746F726465723D313B436C6F7365496E51756965744D6F64653D313B4E6576657261736B3D317D3B646C6C686F73742E6578653D7B73746172746F726465723D313B436C6F7365496E51756965744D6F64653D313B4E6576657261736B3D317D3C2F76616C3E0A2020202020203C2F6C6973743E0A202020203C2F706F6C6963793E0A202020203C706F6C6963792069643D2244697361626C6550726976617465506F6C6963696573223E0A2020202020203C6E756D6265723E323C2F6E756D6265723E0A2020202020203C6C6162656C3E2244697361626C65205072697661746520506F6C6963696573223C2F6C6162656C3E0A2020202020203C626F6F6C3E547275653C2F626F6F6C3E0A202020203C2F706F6C6963793E0A202020203C706F6C6963792069643D2244697361626C6553797374726179223E0A2020202020203C6E756D6265723E3139373C2F6E756D6265723E0A2020202020203C6C6162656C3E224869646520537973747261792069636F6E223C2F6C6162656C3E0A2020202020203C626F6F6C3E747275653C2F626F6F6C3E0A202020203C2F706F6C6963793E0A202020203C706F6C6963792069643D2244697361626C65415049223E0A2020202020203C6E756D6265723E3339323C2F6E756D6265723E0A2020202020203C6C6162656C3E2244697361626C6520415049223C2F6C6162656C3E0A2020202020203C626F6F6C3E46616C73653C2F626F6F6C3E0A202020203C2F706F6C6963793E0A202020203C706F6C6963792069643D2244697361626C654B657946696C65416363657373223E0A2020202020203C6E756D6265723E3130333C2F6E756D6265723E0A2020202020203C6C6162656C3E22446973616C6C6F77206B65792066696C65206163636573736573223C2F6C6162656C3E0A2020202020203C626F6F6C3E547275653C2F626F6F6C3E0A202020203C2F706F6C6963793E0A202020203C706F6C6963792069643D2244697361626C65437370416363657373223E0A2020202020203C6E756D6265723E3130353C2F6E756D6265723E0A2020202020203C6C6162656C3E22446973616C6C6F7720435350206163636573736573223C2F6C6162656C3E0A2020202020203C626F6F6C3E547275653C2F626F6F6C3E0A202020203C2F706F6C6963793E0A202020203C706F6C6963792069643D2244697361626C65506572736F6E616C41636365737346696C65223E0A2020202020203C6E756D6265723E3132333C2F6E756D6265723E0A2020202020203C6C6162656C3E2244697361626C6520506572736F6E616C20616363657373206C69737473223C2F6C6162656C3E0A2020202020203C626F6F6C3E46616C73653C2F626F6F6C3E0A202020203C2F706F6C6963793E0A202020203C706F6C6963792069643D22446973616C6C6F77506572736F6E616C41636365737346696C654372656174696F6E223E0A2020202020203C6E756D6265723E3132343C2F6E756D6265723E0A2020202020203C6C6162656C3E22446973616C6C6F7720506572736F6E616C20616363657373206C69737473206372656174696F6E223C2F6C6162656C3E0A2020202020203C626F6F6C3E46616C73653C2F626F6F6C3E0A202020203C2F706F6C6963793E0A202020203C706F6C6963792069643D2253796E6368726F6E697A65506572736F6E616C41636365737346696C65223E0A2020202020203C6E756D6265723E3132353C2F6E756D6265723E0A2020202020203C6C6162656C3E2253796E6368726F6E697A6520506572736F6E616C20616363657373206C697374223C2F6C6162656C3E0A2020202020203C626F6F6C3E547275653C2F626F6F6C3E0A202020203C2F706F6C6963793E0A202020203C706F6C6963792069643D22506572736F6E616C41636365737346696C654E616D65223E0A2020202020203C6E756D6265723E3132363C2F6E756D6265723E0A2020202020203C6C6162656C3E22506572736F6E616C20416363657373206C697374206E616D65223C2F6C6162656C3E0A2020202020203C657870616E64737A3E2555534552444F4D41494E252025555345524E414D45253C2F657870616E64737A3E0A202020203C2F706F6C6963793E0A202020203C706F6C6963792069643D22506572736F6E616C4163636573734372656174696F6E4F6E4C6F67696E223E0A2020202020203C6E756D6265723E353132303C2F6E756D6265723E0A2020202020203C6C6162656C3E224372656174696F6E206F662074686520706572736F6E616C20616363657373206C69737420617420746865206F70656E696E67206F66207468652073657373696F6E223C2F6C6162656C3E0A2020202020203C626F6F6C3E547275653C2F626F6F6C3E0A202020203C2F706F6C6963793E0A202020203C706F6C6963792069643D224C6F675A6F6E654F70656E436C6F7365223E0A2020202020203C6E756D6265723E3330323C2F6E756D6265723E0A2020202020203C6C6162656C3E205475726E206F6E206C6F6767696E6720666F72207A6F6E65206F70656E20616E6420636C6F7365206576656E74733C2F6C6162656C3E0A2020202020203C626F6F6C3E547275653C2F626F6F6C3E0A202020203C2F706F6C6963793E0A202020203C706F6C6963792069643D224C6F67456E6372797074656446696C65416363657373223E0A2020202020203C6E756D6265723E3330363C2F6E756D6265723E0A2020202020203C6C6162656C3E204C6F6720616363657373657320746F20656E637279707465642066696C65733C2F6C6162656C3E0A2020202020203C626F6F6C3E547275653C2F626F6F6C3E0A202020203C2F706F6C6963793E0A202020203C706F6C6963792069643D224C6F67456E6372797074656446696C6541636365737352656A656374223E0A2020202020203C6E756D6265723E3330373C2F6E756D6265723E0A2020202020203C6C6162656C3E224C6F672072656A656374656420616363657373657320746F20656E632E2046696C6573223C2F6C6162656C3E0A2020202020203C626F6F6C3E547275653C2F626F6F6C3E0A202020203C2F706F6C6963793E0A202020203C706F6C6963792069643D224C6F67437265617465436C65617246696C6552656A656374223E0A2020202020203C6E756D6265723E3330383C2F6E756D6265723E0A2020202020203C6C6162656C3E224C6F672072656A656374656420756E656E637279707465642066696C65206372656174696F6E223C2F6C6162656C3E0A2020202020203C626F6F6C3E547275653C2F626F6F6C3E0A202020203C2F706F6C6963793E0A20203C2F706F6C69636965733E0A3C2F6578706F72743E0A, 
1, '2025-04-07 15:28:28.000', 1081, '2025-05-12 07:11:11.000', 1156, 'N', 'Y', 'Y');


-- 정책 디테일
CREATE TABLE IF NOT EXISTS `T_POLICY_DETAIL_INFO` (
  `pol_seq` int(11) NOT NULL AUTO_INCREMENT COMMENT '자동 생성',
  `pol_code` varchar(255) NOT NULL COMMENT '정책 코드 POL-date-ordernumber. POL-2023050500001식으로 생성 됨',
  `pol_id` int(11) DEFAULT NULL COMMENT '정책 id',
  `pol_data` varchar(1024) DEFAULT NULL,
  `created_date` datetime DEFAULT current_timestamp() COMMENT '등록 날짜',
  `updated_date` datetime DEFAULT NULL ON UPDATE current_timestamp() COMMENT '수정일',
  PRIMARY KEY (`pol_seq`),
  KEY `policy_code_fk` (`pol_code`),
  CONSTRAINT `policy_code_fk` FOREIGN KEY (`pol_code`) REFERENCES `T_POLICY` (`policy_code`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=6857 DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci COMMENT='정책 세부 정보 테이블';

INSERT IGNORE INTO PRIMXDB.T_POLICY_DETAIL_INFO
(pol_seq, pol_code, pol_id, pol_data, created_date, updated_date)
VALUES(6888, 'POL-2025040700001', 153, '<policy id="RestrictHidePolicies"><number>153</number><label>User restriction rules</label><sz></sz></policy>', NULL, '2025-05-12 07:11:11.000');
INSERT IGNORE INTO PRIMXDB.T_POLICY_DETAIL_INFO
(pol_seq, pol_code, pol_id, pol_data, created_date, updated_date)
VALUES(6889, 'POL-2025040700001', 158, ' <policy id="HidePersonalAccessMonitor"><number>158</number><label> (hide) ''Personal access'' tab in the monitor</label><bool>true</bool></policy>', NULL, '2025-05-12 07:11:11.000');
INSERT IGNORE INTO PRIMXDB.T_POLICY_DETAIL_INFO
(pol_seq, pol_code, pol_id, pol_data, created_date, updated_date)
VALUES(6890, 'POL-2025040700001', 133, '<policy id="UsersWithConsigns"><number>133</number><label> Directives are applied only to users/groups (separated by semicolons): </label><sz/></policy>', NULL, '2025-05-12 07:11:11.000');
INSERT IGNORE INTO PRIMXDB.T_POLICY_DETAIL_INFO
(pol_seq, pol_code, pol_id, pol_data, created_date, updated_date)
VALUES(6891, 'POL-2025040700001', 134, '<policy id="UsersWithoutConsigns"><number>134</number><label> Directives are not applied to users/groups: </label><sz/></policy>', NULL, '2025-05-12 07:11:11.000');
INSERT IGNORE INTO PRIMXDB.T_POLICY_DETAIL_INFO
(pol_seq, pol_code, pol_id, pol_data, created_date, updated_date)
VALUES(6892, 'POL-2025040700001', 135, '<policy id="DisableAutomaticConsign"><number>135</number><label> Application options for login instructions</label><int>2</int></policy>', NULL, '2025-05-12 07:11:11.000');
INSERT IGNORE INTO PRIMXDB.T_POLICY_DETAIL_INFO
(pol_seq, pol_code, pol_id, pol_data, created_date, updated_date)
VALUES(6893, 'POL-2025040700001', 132, '<policy id="Consigns"><number>132</number><label>Encryption directives</label><list><name>D:\\safetyzone</name><val>crypt=1</val></list></policy>', NULL, '2025-05-12 07:11:11.000');
INSERT IGNORE INTO PRIMXDB.T_POLICY_DETAIL_INFO
(pol_seq, pol_code, pol_id, pol_data, created_date, updated_date)
VALUES(6894, 'POL-2025040700001', 354, '<policy id="AppFoldersPrivileges"><number>354</number><label> Applications that should be granted special privileges on specific folders</label><list><name>*</name><val>*={denyreadencrypted=1}</val></list><list><name>explorer.exe</name><val>*={denyreadencrypted=1}</val></list><list><name>PRIMXAgent.exe</name><val>*={denyreadencrypted=0}</val></list><list><name>PRIMXAgentCS.exe</name><val>*={denyreadencrypted=0}</val></list><list><name>winword.exe</name><val>*={denyreadencrypted=0;denywriteclear=1};%USERPROFILE%\\AppData={denywriteclear =0}</val></list></policy>', NULL, '2025-05-12 07:11:11.000');
INSERT IGNORE INTO PRIMXDB.T_POLICY_DETAIL_INFO
(pol_seq, pol_code, pol_id, pol_data, created_date, updated_date)
VALUES(6895, 'POL-2025040700001', 120, '<policy id="PersonalAccessFileLocation"><number>120</number><label>"Personal access list location"</label><expandsz>%CSIDL_APPDATA%\\ZONECENTRAL</expandsz></policy>', NULL, '2025-05-12 07:11:11.000');
INSERT IGNORE INTO PRIMXDB.T_POLICY_DETAIL_INFO
(pol_seq, pol_code, pol_id, pol_data, created_date, updated_date)
VALUES(6896, 'POL-2025040700001', 121, '<policy id="AccessFilesLocation"><number>121</number><label>"Access lists location"</label><expandsz>%CSIDL_COMMON_APPDATA%\\ZONECENTRAL\\AccessFiles</expandsz></policy>', NULL, '2025-05-12 07:11:11.000');
INSERT IGNORE INTO PRIMXDB.T_POLICY_DETAIL_INFO
(pol_seq, pol_code, pol_id, pol_data, created_date, updated_date)
VALUES(6897, 'POL-2025040700001', 122, '<policy id="AccessFilesCacheLocation"><number>122</number><label>"Access lists cache location"</label><expandsz>%CSIDL_COMMON_APPDATA%\\ZONECENTRAL\\Cache</expandsz></policy>', NULL, '2025-05-12 07:11:11.000');
INSERT IGNORE INTO PRIMXDB.T_POLICY_DETAIL_INFO
(pol_seq, pol_code, pol_id, pol_data, created_date, updated_date)
VALUES(6898, 'POL-2025040700001', 127, '<policy id="PersonalBackupLocation"><number>127</number><label>"Personal backup location"</label><expandsz>%CSIDL_LOCAL_APPDATA%\\ZONECENTRAL\\Backup</expandsz></policy>', NULL, '2025-05-12 07:11:11.000');
INSERT IGNORE INTO PRIMXDB.T_POLICY_DETAIL_INFO
(pol_seq, pol_code, pol_id, pol_data, created_date, updated_date)
VALUES(6899, 'POL-2025040700001', 309, '<policy id="CommandLogFile"><number>309</number><label>"Log files"</label><expandsz>%CSIDL_COMMON_APPDATA%\\ZONECENTRAL\\Logs</expandsz></policy>', NULL, '2025-05-12 07:11:11.000');
INSERT IGNORE INTO PRIMXDB.T_POLICY_DETAIL_INFO
(pol_seq, pol_code, pol_id, pol_data, created_date, updated_date)
VALUES(6900, 'POL-2025040700001', 343, '<policy id="ZoneMapPublishLocations"><number>343</number><label>"Export/save the zone map"</label><expandsz>%CSIDL_COMMON_APPDATA%\\ZONECENTRAL\\ZoneMap={maxhistory=10}</expandsz></policy>', NULL, '2025-05-12 07:11:11.000');
INSERT IGNORE INTO PRIMXDB.T_POLICY_DETAIL_INFO
(pol_seq, pol_code, pol_id, pol_data, created_date, updated_date)
VALUES(6901, 'POL-2025040700001', 345, '<policy id="KeyMapPublishLocations"><number>345</number><label>"Export/save the key map"</label><expandsz>%CSIDL_COMMON_APPDATA%\\ZONECENTRAL\\KeyMap</expandsz></policy>', NULL, '2025-05-12 07:11:11.000');
INSERT IGNORE INTO PRIMXDB.T_POLICY_DETAIL_INFO
(pol_seq, pol_code, pol_id, pol_data, created_date, updated_date)
VALUES(6902, 'POL-2025040700001', 131, '<policy id="MandatoryMembers"><number>131</number><label>Mandatory members</label><list><name>Mandatory.zaf</name><val>sha256=7F 78 4A 58 04 DF B1 B0 1B F8 57 88 22 A2 81 09 83 81 06 06 03 07 F0 F6 DC 90 FC 8B 85 41 95 64</val></list><list><name>HelpDesk.zaf</name><val>sha256=51 69 A6 DE CF F9 2E FE AC 96 80 30 3F A6 8B E2 92 66 8A F3 33 43 95 86 31 58 73 D0 7E 17 5A 98;helpdesk=1</val></list></policy>', NULL, '2025-05-12 07:11:11.000');
INSERT IGNORE INTO PRIMXDB.T_POLICY_DETAIL_INFO
(pol_seq, pol_code, pol_id, pol_data, created_date, updated_date)
VALUES(6903, 'POL-2025040700001', 499, '<policy id="Reserved"><number>499</number><label>"Reserved"</label><list><name>ApplyKillProc</name><val>msedgewebview2.exe={startorder=1;CloseInQuietMode=1;Neverask=1};msteams.exe={startorder=1;CloseInQuietMode=1;Neverask=1};StartMenuExperienceHost.exe={startorder=1;CloseInQuietMode=1;Neverask=1};dllhost.exe={startorder=1;CloseInQuietMode=1;Neverask=1}</val></list></policy>', NULL, '2025-05-12 07:11:11.000');
INSERT IGNORE INTO PRIMXDB.T_POLICY_DETAIL_INFO
(pol_seq, pol_code, pol_id, pol_data, created_date, updated_date)
VALUES(6904, 'POL-2025040700001', 2, '<policy id="DisablePrivatePolicies"><number>2</number><label>"Disable Private Policies"</label><bool>True</bool></policy>', NULL, '2025-05-12 07:11:11.000');
INSERT IGNORE INTO PRIMXDB.T_POLICY_DETAIL_INFO
(pol_seq, pol_code, pol_id, pol_data, created_date, updated_date)
VALUES(6905, 'POL-2025040700001', 197, '<policy id="DisableSystray"><number>197</number><label>"Hide Systray icon"</label><bool>true</bool></policy>', NULL, '2025-05-12 07:11:11.000');
INSERT IGNORE INTO PRIMXDB.T_POLICY_DETAIL_INFO
(pol_seq, pol_code, pol_id, pol_data, created_date, updated_date)
VALUES(6906, 'POL-2025040700001', 392, ' <policy id="DisableAPI"><number>392</number><label>"Disable API"</label><bool>False</bool></policy>', NULL, '2025-05-12 07:11:11.000');
INSERT IGNORE INTO PRIMXDB.T_POLICY_DETAIL_INFO
(pol_seq, pol_code, pol_id, pol_data, created_date, updated_date)
VALUES(6907, 'POL-2025040700001', 103, '<policy id="DisableKeyFileAccess"><number>103</number><label>"Disallow key file accesses"</label><bool>True</bool></policy>', NULL, '2025-05-12 07:11:11.000');
INSERT IGNORE INTO PRIMXDB.T_POLICY_DETAIL_INFO
(pol_seq, pol_code, pol_id, pol_data, created_date, updated_date)
VALUES(6908, 'POL-2025040700001', 105, '<policy id="DisableCspAccess"><number>105</number><label>"Disallow CSP accesses"</label><bool>True</bool></policy>', NULL, '2025-05-12 07:11:11.000');
INSERT IGNORE INTO PRIMXDB.T_POLICY_DETAIL_INFO
(pol_seq, pol_code, pol_id, pol_data, created_date, updated_date)
VALUES(6909, 'POL-2025040700001', 123, '<policy id="DisablePersonalAccessFile"><number>123</number><label>"Disable Personal access lists"</label><bool>False</bool></policy>', NULL, '2025-05-12 07:11:11.000');
INSERT IGNORE INTO PRIMXDB.T_POLICY_DETAIL_INFO
(pol_seq, pol_code, pol_id, pol_data, created_date, updated_date)
VALUES(6910, 'POL-2025040700001', 124, '<policy id="DisallowPersonalAccessFileCreation"><number>124</number><label>"Disallow Personal access lists creation"</label><bool>False</bool></policy>', NULL, '2025-05-12 07:11:11.000');
INSERT IGNORE INTO PRIMXDB.T_POLICY_DETAIL_INFO
(pol_seq, pol_code, pol_id, pol_data, created_date, updated_date)
VALUES(6911, 'POL-2025040700001', 125, '<policy id="SynchronizePersonalAccessFile"><number>125</number><label>"Synchronize Personal access list"</label><bool>True</bool></policy>', NULL, '2025-05-12 07:11:11.000');
INSERT IGNORE INTO PRIMXDB.T_POLICY_DETAIL_INFO
(pol_seq, pol_code, pol_id, pol_data, created_date, updated_date)
VALUES(6912, 'POL-2025040700001', 126, '<policy id="PersonalAccessFileName"><number>126</number><label>"Personal Access list name"</label><expandsz>%USERDOMAIN% %USERNAME%</expandsz></policy>', NULL, '2025-05-12 07:11:11.000');
INSERT IGNORE INTO PRIMXDB.T_POLICY_DETAIL_INFO
(pol_seq, pol_code, pol_id, pol_data, created_date, updated_date)
VALUES(6913, 'POL-2025040700001', 5120, ' <policy id="PersonalAccessCreationOnLogin"><number>5120</number><label>"Creation of the personal access list at the opening of the session"</label><bool>True</bool></policy>', NULL, '2025-05-12 07:11:11.000');
INSERT IGNORE INTO PRIMXDB.T_POLICY_DETAIL_INFO
(pol_seq, pol_code, pol_id, pol_data, created_date, updated_date)
VALUES(6914, 'POL-2025040700001', 302, '<policy id="LogZoneOpenClose"><number>302</number><label> Turn on logging for zone open and close events</label><bool>True</bool></policy>', NULL, '2025-05-12 07:11:11.000');
INSERT IGNORE INTO PRIMXDB.T_POLICY_DETAIL_INFO
(pol_seq, pol_code, pol_id, pol_data, created_date, updated_date)
VALUES(6915, 'POL-2025040700001', 306, '<policy id="LogEncryptedFileAccess"><number>306</number><label> Log accesses to encrypted files</label><bool>True</bool></policy>', NULL, '2025-05-12 07:11:11.000');
INSERT IGNORE INTO PRIMXDB.T_POLICY_DETAIL_INFO
(pol_seq, pol_code, pol_id, pol_data, created_date, updated_date)
VALUES(6916, 'POL-2025040700001', 307, '<policy id="LogEncryptedFileAccessReject"><number>307</number><label>"Log rejected accesses to enc. Files"</label><bool>True</bool></policy>', NULL, '2025-05-12 07:11:11.000');
INSERT IGNORE INTO PRIMXDB.T_POLICY_DETAIL_INFO
(pol_seq, pol_code, pol_id, pol_data, created_date, updated_date)
VALUES(6917, 'POL-2025040700001', 308, '<policy id="LogCreateClearFileReject"><number>308</number><label>"Log rejected unencrypted file creation"</label><bool>True</bool></policy>', NULL, '2025-05-12 07:11:11.000');

-- 정책 파일
CREATE TABLE IF NOT EXISTS `T_POLICY_FILE` (
  `C_FILE_ID` int(11) NOT NULL AUTO_INCREMENT COMMENT '자동 순번',
  `C_FILE_TYPE` char(1) NOT NULL DEFAULT 'M' COMMENT '파일종류 (이동파일 = M, 일반파일 = C)',
  `C_EXPIRED_DATE` int(11) NOT NULL DEFAULT 10 COMMENT '유효기간',
  `C_DEC_COUNT` int(11) NOT NULL DEFAULT 10 COMMENT '복호화 횟수',
  `C_CERT_URL` varchar(255) DEFAULT NULL COMMENT '인증서버주소',
  PRIMARY KEY (`C_FILE_ID`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

-- 정책 그릇
CREATE TABLE IF NOT EXISTS `T_POLICY_MANDATORY_INFO` (
  `pol_seq` int(11) NOT NULL AUTO_INCREMENT COMMENT '자동 생성',
  `pol_code` varchar(255) NOT NULL COMMENT '정책 코드 POL-date-ordernumber. POL-2023050500001식으로 생성 됨',
  `pol_id` int(11) DEFAULT NULL COMMENT '정책 id',
  `pol_data` varchar(511) NOT NULL COMMENT '정책 세부 내용',
  `pol_use_yn` varchar(2) DEFAULT 'Y' COMMENT '사용 여부. Y:Yes, N:No',
  `created_date` datetime DEFAULT current_timestamp() COMMENT '등록 날짜',
  `updated_date` datetime DEFAULT NULL ON UPDATE current_timestamp() COMMENT '수정일',
  PRIMARY KEY (`pol_seq`),
  KEY `policy_code_mandatory_fk` (`pol_code`)
) ENGINE=InnoDB AUTO_INCREMENT=2130 DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci COMMENT='정책 기본 구성 정보 테이블';

INSERT IGNORE INTO PRIMXDB.T_POLICY_MANDATORY_INFO (pol_seq, pol_code,pol_id,pol_data,pol_use_yn,created_date,updated_date) VALUES
	 (0, 'POL-0000000000001',162,' <policy id="HideInPlaceEncrypt"><number>162</number><label> (hide) Encrypt operation</label><bool>False</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',163,' <policy id="HideInPlaceDecrypt"><number>163</number><label> (hide) Decrypt operation</label><bool>False</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',164,' <policy id="HideInPlaceVerify"><number>164</number><label> (hide) Verify operation</label><bool>False</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',165,' <policy id="HideInPlaceTranscrypt"><number>165</number><label> (hide) Transcrypt operation</label><bool>False</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',166,'<policy id="HideInPlaceAccesses"><number>166</number><label> (hide) Accesses</label><bool>False</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',167,' <policy id="HideInPlaceAccessesOps"><number>167</number><label> (hide) Operations on accesses</label><bool>False</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',168,' <policy id="HideAddPassword"><number>168</number><label> (hide) Add Password access</label><bool>False</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',218,' <policy id="HideAddPasswordForZed"><number>218 </number><label> (hide) Add Password access for encrypted containers</label><bool>False</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',169,' <policy id="HideAddCertFile"><number>169</number><label> (hide) Add certificate access (from a file)</label><bool>False</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',219,' <policy id="HideAddCertFileForZed"><number>219 </number><label> (hide) Add certificate access (from a file) for encrypted containers</label><bool>False</bool></policy>','Y',current_timestamp(), NULL);
INSERT IGNORE INTO PRIMXDB.T_POLICY_MANDATORY_INFO (pol_seq, pol_code,pol_id,pol_data,pol_use_yn,created_date,updated_date) VALUES
	 (0, 'POL-0000000000001',170,' <policy id="HideAddCertStore"><label> (hide) Add certificate access (from a Windows certstore)</label><bool>False</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',220,' <policy id="HideAddCertStoreForZed"><number>220 </number><label> (hide) Add certificate access (from a Windows certstore) for encrypted containers</label><bool>False</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',221,' <policy id="HideAddLDAPForZed"><number>221 </number><label> (hide) Add certificate access (from a LDAP directory) for encrypted containers</label><bool>False</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',173,' <policy id="HideInPlaceGeneralPage"><number>173</number><label> (hide) Zone general characteristics</label><bool>False</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',174,' <policy id="DisallowZoneLabelChange"><number>174</number><label> (hide) Zone properties change</label><bool>False</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',175,'<policy id="HideInPlaceCreateClearZone"><number>175</number><label> (hide) Clear zone creation</label><bool>False</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',176,'<policy id="HideInPlaceGroup"><number>176</number><label> (hide) Group operation</label><bool>False</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',177,' <policy id="HideInPlaceSeparate"><number>177</number><label> (hide) Ungroup operation</label><bool>False</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',178,'<policy id="HideInPlaceExceptions"><number>178</number><label> (hide) Zone exceptions patterns</label><bool>False</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',179,'<policy id="HideInPlaceExceptionsOps"><number>179</number><label> (hide) Operations on exceptions patterns</label><bool>False</bool></policy>','Y',current_timestamp(), NULL);
INSERT IGNORE INTO PRIMXDB.T_POLICY_MANDATORY_INFO (pol_seq, pol_code,pol_id,pol_data,pol_use_yn,created_date,updated_date) VALUES
	 (0, 'POL-0000000000001',180,'<policy id="HideInPlaceSubzones"><number>180</number><label> (hide) Sub</label><bool>False</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',181,' <policy id="HideShowSos"><number>181</number><label> (hide) Show SOS access</label><bool>False</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',182,' <policy id="HideCheckerDetails"><number>182</number><label> (hide) Display details of a check</label><bool>False</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',183,' <policy id="HideComputerWideOperations"><number>183</number><label> (hide) Workstation operations</label><bool>False</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',184,'<policy id="HideAddAccessFile"><number>184</number><label> (hide) Add access lists</label><bool>False</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',222,' <policy id="HideAddAccessFileForZed"><number>222 </number><label> (hide) Add access lists for encrypted containers</label><bool>False</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',185,' <policy id="HideInPlaceRecover"><number>185</number><label> (hide) Repair operation</label><bool>False</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',133,'<policy id="UsersWithConsigns"><number>133</number><label> Directives are applied only to users/groups (separated by semicolons): </label><sz>C:UsersSonSeJinDesktopSejinLL</sz></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',134,'<policy id="UsersWithoutConsigns"><number>134</number><label> Directives are not applied to users/groups: </label><sz>clear=1</sz></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',135,'<policy id="DisableAutomaticConsign"><number>135</number><label> Application options for login instructions</label><int>0</int></policy>','Y',current_timestamp(), NULL);
INSERT IGNORE INTO PRIMXDB.T_POLICY_MANDATORY_INFO (pol_seq, pol_code,pol_id,pol_data,pol_use_yn,created_date,updated_date) VALUES
	 (0, 'POL-0000000000001',354,'<policy id="AppFoldersPrivileges"><number>354</number><label> Applications that should be granted special privileges on specific folders</label><list><name>*</name><val>*={denyreadencrypted=1}</val></list><list><name>explorer.exe</name><val>*={denyreadencrypted=1}</val></list><list><name>winword.exe</name><val>*={denyreadencrypted=0;denywriteclear=1};%USERPROFILE%AppData={denywriteclear=0}</val></list></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',70,'<policy id="NetPolicies"><number>70</number><label>"Alternative policies configuration"</label><sz>file=http://192.168.0.49/PRIMX/ZONECENTRAL/zcindex.xml;noxsd=1;refresh=10</sz></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',120,'<policy id="PersonalAccessFileLocation"><number>120</number><label>"Personal access list location"</label><expandsz>%CSIDL_APPDATA%ZONECENTRAL</expandsz></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',121,'<policy id="AccessFilesLocation"><number>121</number><label>"Access lists location"</label><expandsz>%CSIDL_COMMON_APPDATA%ZONECENTRALAccessFiles</expandsz></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',122,'<policy id="AccessFilesCacheLocation"><number>122</number><label>"Access lists cache location"</label><expandsz>%CSIDL_COMMON_APPDATA%ZONECENTRALCache</expandsz></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',127,'<policy id="PersonalBackupLocation"><number>127</number><label>"Personal backup location"</label><expandsz>%CSIDL_LOCAL_APPDATA%ZONECENTRALBackup</expandsz></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',309,'<policy id="CommandLogFile"><number>309</number><label>"Log files"</label><expandsz>%CSIDL_COMMON_APPDATA%ZONECENTRALLogs</expandsz></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',343,'<policy id="ZoneMapPublishLocations"><number>343</number><label>"Export/save the zone map"</label><expandsz>%CSIDL_COMMON_APPDATA%ZONECENTRALoneMap={maxhistory=10}</expandsz></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',345,'<policy id="KeyMapPublishLocations"><number>345</number><label>"Export/save the key map"</label><expandsz>%CSIDL_COMMON_APPDATA%ZONECENTRALKeyMap</expandsz></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',131,'<policy id="MandatoryMembers"><number>131</number><label>Mandatory members</label><list><name>Mandatory.zaf</name><val>sha256=7F 78 4A 58 04 DF B1 B0 1B F8 57 88 22 A2 81 09 83 81 06 06 03 07 F0 F6 DC 90 FC 8B 85 41 95 64</val></list><list><name>HelpDesk.zaf</name><val>sha256=51 69 A6 DE CF F9 2E FE AC 96 80 30 3F A6 8B E2 92 66 8A F3 33 43 95 86 31 58 73 D0 7E 17 5A 98;helpdesk=1</val></list></policy>','Y',current_timestamp(), NULL);
INSERT IGNORE INTO PRIMXDB.T_POLICY_MANDATORY_INFO (pol_seq, pol_code,pol_id,pol_data,pol_use_yn,created_date,updated_date) VALUES
	 (0, 'POL-0000000000001',499,'<policy id="Reserved"><number>499</number><label>"Reserved"</label><list><name>ApplyKillProc</name><val>msedgewebview2.exe={startorder=1;CloseInQuietMode=1;Neverask=1};msteams.exe={startorder=1;CloseInQuietMode=1;Neverask=1};StartMenuExperienceHost.exe={startorder=1;CloseInQuietMode=1;Neverask=1};dllhost.exe={startorder=1;CloseInQuietMode=1;Neverask=1}</val></list></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',2,'<policy id="DisablePrivatePolicies"><number>2</number><label>"Disable Private Policies"</label><bool>True</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',153,'<policy id="RestrictHidePolicies"><number>153</number><label>User restriction rules</label><sz></sz></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',132,'<policy id="Consigns"><number>132</number><label>Encryption directives</label><list><name>1111</name><val>11111</val></list></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',159,' <policy id="HidePersonalKeyMonitor"><number>159</number><label> (hide) ''Personal access key'' tab in the monitor</label><bool>False</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',161,' <policy id="HideUnauthorizedInPlaceOps"><number>161</number><label> (hide) Operations not granted by the ''Admin rights'' policies (P231+)</label><bool>False</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',158,' <policy id="HidePersonalAccessMonitor"><number>158</number><label> (hide) ''Personal access'' tab in the monitor</label><bool>False</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',160,'<policy id="HideFolderContextMenu"><number>160</number><label> (hide) Folder context menus</label><bool>False</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',171,'<policy id="HideAddLDAP"><number>171</number><label> (hide) Add certificate access (from a LDAP directory)</label><bool>False</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',172,'<policy id="HideInPlaceDetails"><number>172</number><label> (hide) Zone advanced</label><bool>False</bool></policy>','Y',current_timestamp(), NULL);
INSERT IGNORE INTO PRIMXDB.T_POLICY_MANDATORY_INFO (pol_seq, pol_code,pol_id,pol_data,pol_use_yn,created_date,updated_date) VALUES
	 (0, 'POL-0000000000001',197,'  <policy id="DisableSystray"><number>197</number><label>"Hide Systray icon"</label><bool>True</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',392,' <policy id="DisableAPI"><number>392</number><label>"Disable API"</label><bool>False</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',103,'<policy id="DisableKeyFileAccess"><number>103</number><label>"Disallow key file accesses"</label><bool>True</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',105,'<policy id="DisableCspAccess"><number>105</number><label>"Disallow CSP accesses"</label><bool>True</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',123,'<policy id="DisablePersonalAccessFile"><number>123</number><label>"Disable Personal access lists"</label><bool>False</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',124,'<policy id="DisallowPersonalAccessFileCreation"><number>124</number><label>"Disallow Personal access lists creation"</label><bool>False</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',125,'<policy id="SynchronizePersonalAccessFile"><number>125</number><label>"Synchronize Personal access list"</label><bool>True</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',126,'<policy id="PersonalAccessFileName"><number>126</number><label>"Personal Access list name"</label><expandsz>%USERDOMAIN% %USERNAME%</expandsz></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',5120,' <policy id="PersonalAccessCreationOnLogin"><number>5120</number><label>"Creation of the personal access list at the opening of the session"</label><bool>True</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',302,'<policy id="LogZoneOpenClose"><number>302</number><label> Turn on logging for zone open and close events</label><bool>True</bool></policy>','Y',current_timestamp(), NULL);
INSERT IGNORE INTO PRIMXDB.T_POLICY_MANDATORY_INFO (pol_seq, pol_code,pol_id,pol_data,pol_use_yn,created_date,updated_date) VALUES
	 (0, 'POL-0000000000001',306,'<policy id="LogEncryptedFileAccess"><number>306</number><label> Log accesses to encrypted files</label><bool>True</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',307,'<policy id="LogEncryptedFileAccessReject"><number>307</number><label>"Log rejected accesses to enc. Files"</label><bool>True</bool></policy>','Y',current_timestamp(), NULL),
	 (0, 'POL-0000000000001',308,'<policy id="LogCreateClearFileReject"><number>308</number><label>"Log rejected unencrypted file creation"</label><bool>True</bool></policy>','Y',current_timestamp(), NULL);




-- 시스템에 정책 적용 테이블
CREATE TABLE IF NOT EXISTS `T_POLICY_SYSTEM_MAP` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '자동 생성',
  `policy_code` varchar(50) NOT NULL COMMENT '시스템에 적용된 정책 ID',
  `C_AGENT_ID` VARCHAR(50) NOT NULL COMMENT '정책을 적용한 시스템 ID',
  `modified_date` DATETIME DEFAULT NULL COMMENT '수정일',
  PRIMARY KEY (`id`),
  KEY `idx_policy_system_map` (`policy_code`,`C_AGENT_ID`),
  KEY `policy_system_map_policy_id_fk` (`policy_code`),
  CONSTRAINT `policy_system_map_policy_id_fk` FOREIGN KEY (`policy_code`) REFERENCES `T_POLICY` (`policy_code`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='시스템에 정책 적용 테이블';

-- 배포 정책
CREATE TABLE IF NOT EXISTS `T_RELEASE_POLICY` (
  `C_REG_ID` int(11) NOT NULL AUTO_INCREMENT COMMENT '자동 생성',
  `C_DEPT_ID` varchar(50) NOT NULL COMMENT '부서 정보',
  `C_DEPT_PATH` varchar(255) NOT NULL COMMENT '부서 경로',
  `C_BEGIN_DATE` datetime DEFAULT NULL COMMENT '시작일',
  `C_END_DATE` datetime DEFAULT NULL COMMENT '종료일',
  `C_RELEASE_YN` char(1) NOT NULL DEFAULT 'N' COMMENT '배포 여부 (Y: 배포, N: 중단)',
  PRIMARY KEY (`C_REG_ID`),
  KEY `idx_dept_id` (`C_DEPT_ID`)
) ENGINE=InnoDB AUTO_INCREMENT=70 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci COMMENT='배포 정책 테이블';

-- 정책 환경 설정
CREATE TABLE IF NOT EXISTS `T_ENV_POLICY` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '자동 생성',
  `product_id` INT(11) NOT NULL COMMENT '제품 ID. 1:Zonecentral, 2:Orizon, 3:Cryhod',
  `script_name` VARCHAR(255) NOT NULL COMMENT '스크립트명. ADML,ADMX,XML',
  `file_id` INT(11) DEFAULT NULL COMMENT '파일 등록 ID',
  PRIMARY KEY (`id`),
  KEY `env_policy_file_file_id_fk` (`file_id`),
  KEY `env_policy_products_id_fk` (`product_id`),
  CONSTRAINT `env_policy_file_file_id_fk` FOREIGN KEY (`file_id`) REFERENCES `T_FILE` (`file_id`),
  CONSTRAINT `env_policy_products_id_fk` FOREIGN KEY (`product_id`) REFERENCES `T_PRODUCTS` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='정책 환경 설정';

INSERT IGNORE INTO T_ENV_POLICY VALUES 
(1,1,' Policy From Sript (ADML) ',NULL),
(2,1,' Policy From Sript (ADMX) ',NULL);


-- 신청
CREATE TABLE IF NOT EXISTS `T_REQUEST` (
  `request_id` int(11) NOT NULL AUTO_INCREMENT COMMENT '신청 ID. 자동 생성',
  `REQUEST_APPROVAL_CODE` char(1) DEFAULT 'M' COMMENT '신청 분류 코드 (A: 신규 사용자 등록, C:사용자 만료 연장, M:이동파일)',
  `status_cd` varchar(255) DEFAULT NULL COMMENT '상태 코드. 신청 상태. PE:pending, CO:complete,DR:draft',
  `result_cd` varchar(255) DEFAULT NULL COMMENT '결과 코드. 결과 상태. AP:Approved, RE:Rejected',
  `title` tinytext DEFAULT NULL COMMENT '신청 제목',
  `reason` varchar(255) DEFAULT NULL COMMENT '결재 사유',
  `description` text DEFAULT NULL COMMENT '설명',
  `file_group_id` int(11) DEFAULT NULL COMMENT '파일 그룹 ID',
  `request_code` varchar(255) NOT NULL COMMENT '신청 코드. R-date-ordernumber, R202305050001식으로 저장 됨',
  `C_USER_ID` varchar(50) DEFAULT NULL COMMENT '등록자',
  `created_date` datetime DEFAULT NULL COMMENT '등록날짜',
  `created_ip` varchar(255) DEFAULT NULL COMMENT '등록 ip',
  `modified_by` varchar(50) DEFAULT NULL COMMENT '수정자',
  `modified_date` datetime DEFAULT NULL COMMENT '수정일',
  `modified_ip` varchar(255) DEFAULT NULL COMMENT '수정 IP',
  PRIMARY KEY (`request_id`),
  UNIQUE KEY `uidx_t_request` (`request_code`)
) ENGINE=InnoDB AUTO_INCREMENT=334 DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci COMMENT='신청';

-- 신청 결재
CREATE TABLE IF NOT EXISTS `T_REQUEST_APPROVERS` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '자동 생성',
  `request_code` varchar(50) NOT NULL COMMENT '신청 ID',
  `C_USER_ID` varchar(50) NOT NULL COMMENT '결재자 ID',
  `order_number` int(11) DEFAULT NULL COMMENT '결재 순서 번호',
  `result_cd` varchar(20) DEFAULT NULL COMMENT '결과 상태 코드. AP:Approved, RE: Rejected',
  `description` text DEFAULT NULL COMMENT '설명',
  `result_date` datetime DEFAULT NULL COMMENT '결과 날짜',
  `is_auto_rejected` varchar(2) NOT NULL DEFAULT 'N' COMMENT '자동으로 반려 여부. 첫번째 결재자가 반려 하면 다음 결재가 자동으로 반려 됨. Y:Yes, N:No',
  `approval_status` int(11) NOT NULL DEFAULT 0 COMMENT '0: 대기, 1: 승인, 2: 거부',
  PRIMARY KEY (`id`),
  KEY `request_approvers_request_request_id_fk` (`request_code`),
  CONSTRAINT `request_approvers_request_request_id_fk` FOREIGN KEY (`request_code`) REFERENCES `T_REQUEST` (`request_code`)
) ENGINE=InnoDB AUTO_INCREMENT=601 DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci COMMENT='신청 결재';

-- 신청 결재 (유저)
CREATE TABLE IF NOT EXISTS `T_REQUEST_USER_APPROVAL` (
  `REQ_SEQ` bigint(20) NOT NULL AUTO_INCREMENT COMMENT 'INDEX 순번',
  `EMPLYR_ID` varchar(50) NOT NULL COMMENT '업무사용자ID',
  `COMPANY_ID` varchar(50) NOT NULL DEFAULT '00000001' COMMENT '회사 ID',
  `PSTINST_CODE` char(8) DEFAULT NULL COMMENT '소속기관코드',
  `GROUP_ID` char(20) DEFAULT NULL COMMENT '그룹ID',
  `ORGNZT_ID` char(20) DEFAULT NULL COMMENT '조직ID',
  `USER_NM` varchar(60) NOT NULL COMMENT '사용자명',
  `PASSWORD` varchar(200) NOT NULL COMMENT '비밀번호',
  `EMAIL_ADRES` varchar(50) DEFAULT NULL COMMENT '이메일주소',
  `REQUEST_RESULT_YN` char(1) DEFAULT 'N' COMMENT '신청 등록 결과 여부 (Y: 신청완료, N:신청요청)',
  `USER_TYPE` tinyint(4) DEFAULT 0 COMMENT '사용자 타입. (0: 일반사용자(Default), 1: 게스트 사용자, 2:관리자)',
  `USER_ROLE` varchar(255) DEFAULT NULL COMMENT '권한. ADMIN/User',
  `ESNTL_ID` char(20) DEFAULT NULL COMMENT '고유ID',
  `INSERT_DATE` datetime DEFAULT current_timestamp() COMMENT '생성 일자',
  `EXPIRED_DATE` datetime DEFAULT NULL COMMENT '유효 기간',
  `REQUEST_APPROVAL_CODE` char(1) DEFAULT 'C' COMMENT '신청 분류 코드 (A: 신규 사용자 등록, C:사용자 만료 연장)',
  `request_code` varchar(255) NOT NULL COMMENT '신청 코드. R-date-ordernumber, R202305050001식으로 저장 됨',
  PRIMARY KEY (`REQ_SEQ`,`EMPLYR_ID`),
  UNIQUE KEY `COMTNEMPLYRINFO_PK` (`REQ_SEQ`,`EMPLYR_ID`),
  UNIQUE KEY `uidx_t_user_master` (`COMPANY_ID`,`EMPLYR_ID`),
  UNIQUE KEY `uidx_c_email_addr` (`EMAIL_ADRES`)
) ENGINE=InnoDB AUTO_INCREMENT=1187 DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci COMMENT='신청 및 결재 || 계정 만료 연장 이력 / 신규 사용자 등록 정보';

-- 결과 코드 리소스 테이블
CREATE TABLE IF NOT EXISTS T_RESOURCE_LOG_RESULT ( 
  `C_RESULT_CODE` VARCHAR(10) NOT NULL COMMENT '결과 코드', 
  `C_LANGUAGE` VARCHAR(10) NOT NULL COMMENT '언어 구분. (세자리 언어코드(KOR, ENG ...) ISO 3166-1 alpha-3 codes 규약)',  
  `C_RESULT_NAME` VARCHAR(200) DEFAULT NULL COMMENT '결과 이름', 
  `C_DESCRIPTION` VARCHAR(200) DEFAULT NULL COMMENT '결과 설명', 
  PRIMARY KEY (`C_RESULT_CODE`,`C_LANGUAGE`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='결과 코드 리소스 테이블';

INSERT IGNORE INTO T_RESOURCE_LOG_RESULT VALUES 
('SUCCESS', 'KOR', '성공', '작업에 성공하였습니다.'), 	
('E10001', 'KOR', '데이터베이스 연결 실패', '시스템 데이터베이스 연결에 실패하였습니다.'), 
('E10002', 'KOR', '트랜잭션 쿼리 실패', '데이터 변경에 실패하였습니다.'), 
('E10003', 'KOR', '조회 쿼리 실패', '데이터 조회에 실패하였습니다.'), 
('E10004', 'KOR', 'Procedure 실행 실패', '데이터베이스 프로시저 실행에 실패하였습니다.'), 
('E20001', 'KOR', 'User ID를 찾을 수 없음', '사용자 ID를 찾을 수 없습니다.'), 
('E20002', 'KOR', 'Agent ID가 공백', 'Agent ID가 공백입니다.'), 
('E20003', 'KOR', 'User ID가 공백', '사용자 ID가 공백입니다.'), 
('E20004', 'KOR', 'XML 파싱 실패', 'XML 자료 추출에 실패하였습니다.'), 
('E20005', 'KOR', '파일 키가 공백', '파일 키가 공백입니다.'), 
('E20006', 'KOR', '회사 ID가 공백', '회사 ID가 공백입니다.'), 
('E20007', 'KOR', '파일 해시코드가 공백', '파일 해시코드가 공백입니다.'), 
('E30001', 'KOR', '파일 쓰기 실패', '파일 쓰기에 실패하였습니다.'), 
('E30002', 'KOR', '파일이 없음', '해당 파일이 없습니다.'), 
('E30003', 'KOR', '파일 크기가 0', '파일 크기가 0입니다.'), 
('E40001', 'KOR', '업데이트 불필요', '업데이트가 없거나 불필요한 상태입니다.'), 
('E40002', 'KOR', '파일 키를 찾을 수 없음', '파일 키를 찾을 수 없습니다.'), 
('E40003', 'KOR', '파일 해시코드 불일치', '파일 해시코드가 불일치합니다.'), 
('E40004', 'KOR', '복호화 기간 만료', '복호화 기간이 만료되었습니다.'), 
('E40005', 'KOR', '복호화 회수 초과', '복호화 회수를 초과하였습니다.'), 
('E40006', 'KOR', 'User ID 불일치', '사용자 ID가 일치하지 않습니다.'), 
('E40007', 'KOR', 'Agent ID 불일치', 'Agent ID가 일치하지 않습니다.'), 
('E40008', 'KOR', '동일한 파일 키가 이미 존재', '동일한 파일 키가 이미 존재합니다.');

-- 로그 운영 타입 리소스 테이블
CREATE TABLE IF NOT EXISTS T_RESOURCE_LOG_TYPE ( 
  `C_LOG_TYPE_CODE` VARCHAR(10) NOT NULL COMMENT '로그 타입 코드', 
  `C_LANGUAGE` VARCHAR(10) NOT NULL COMMENT '언어 구분. (세자리 언어코드(KOR, ENG ...) ISO 3166-1 alpha-3 codes 규약)',  
  `C_LOG_TYPE_NAME` VARCHAR(200) DEFAULT NULL COMMENT '로그 타입 이름', 
  `C_DESCRIPTION` VARCHAR(200) DEFAULT NULL COMMENT '로그 타입 설명', 
  PRIMARY KEY (`C_LOG_TYPE_CODE`,`C_LANGUAGE`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='로그 운영 타입 리소스 테이블';

INSERT IGNORE INTO T_RESOURCE_LOG_TYPE VALUES 
('T001', 'KOR', '접속', '접속 관련 로그'), 
('T002', 'KOR', '정책', '정책 관련 로그'), 
('T003', 'KOR', '시스템', '시스템 관련 로그'), 
('T004', 'KOR', '관리자', '관리자 계정 관련 로그'), 
('T005', 'KOR', '사용자', '사용자 계정 관련 로그'), 
('T006', 'KOR', '업데이트', '프로그램 업데이트 관련 로그'), 
('T007', 'KOR', '연동', '솔루션 연동 로그');

-- 운영 로그의 리소스 테이블
CREATE TABLE IF NOT EXISTS T_RESOURCE_MANAGE_LOG ( 
  `C_LOG_CODE` VARCHAR(10) NOT NULL COMMENT '로그 코드', 
  `C_LANGUAGE` VARCHAR(10) NOT NULL COMMENT '언어 구분. (세자리 언어코드(KOR, ENG ...) ISO 3166-1 alpha-3 codes 규약)',  
  `C_LOG_TYPE_CODE` VARCHAR(10) NOT NULL COMMENT '로그 타입 코드', 
  `C_LOG_NAME` VARCHAR(200) DEFAULT NULL COMMENT '로그 이름', 
  `C_DESCRIPTION` VARCHAR(200) DEFAULT NULL COMMENT '로그 설명', 
  `C_MESSAGE` VARCHAR(1024) DEFAULT NULL COMMENT '로그 메시지. __VALUE[X]__ 문자열을 각 경우에 맞는 문자열로 대체', 
  `C_COLUMN_GUIDE` VARCHAR(500) DEFAULT NULL COMMENT '컬럼 가이드. __VALUE[X]__에 들어가는 T_MANAGE_LOG의 컬럼을 순서대로 명시',
  PRIMARY KEY (`C_LOG_CODE`,`C_LANGUAGE`), 
  KEY idx_t_resource_manage_log (`C_LOG_CODE`,`C_LANGUAGE`,`C_LOG_TYPE_CODE`) 
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='운영 로그의 리소스 테이블';

INSERT IGNORE INTO T_RESOURCE_MANAGE_LOG VALUES 
('W10001', 'KOR', 'T001', '관리자 로그인', '관리자 로그인됨', '__VALUE1__로 로그인', 'C_USER_ID'),
('W10002', 'KOR', 'T001', '사용자 로그인', '사용자 로그인됨', '__VALUE1__로 로그인', 'C_USER_ID'),
('W20001', 'KOR', 'T002', '제품 정책 변경', '제품 정책이 변경됨','__VALUE1__정책을 __VALUE2__ 관리자가 변경', 'C_PRODUCT_POLICY_NAME|C_RESERVED1'),
('W20002', 'KOR', 'T002', '제품 정책 적용', '제품 정책이 적용됨', '__VALUE1__시스템에 __VALUE2__정책을 __VALUE3__ 관리자가 적용', 'C_AGENT_ID|C_PRODUCT_POLICY_NAME|C_USER_ID'),
('W20003', 'KOR', 'T003', '시스템 종류 변경', '시스템 종류가 변경됨', '__VALUE1__시스템을 __VALUE2__로 __VALUE3__ 관리자가 변경', 'C_AGENT_ID|C_RESERVED1|C_USER_ID'),
('W20004', 'KOR', 'T002', 'PRIMX Agent 정책 적용', 'Agent 정책이 변경됨', '__VALUE1__정책을 __VALUE2__ 관리자가 변경', 'C_AGENT_POLICY_NAME|C_USER_ID'),
('W20005', 'KOR', 'T002', 'PRIMX Agent 정책 적용', 'Agent 정책이 적용됨', '__VALUE1__ 시스템에 __VALUE2__ 정책을 __VALUE3__ 관리자가 적용', 'C_AGENT_ID|C_AGENT_POLICY_NAME|C_USER_ID'),
('W20006', 'KOR', 'T004', '관리자 추가', '관리자가 추가됨', '__VALUE1__를 __VALUE2__ 관리자가 추가', 'C_USER_ID|C_RESERVED1'),
('W20007', 'KOR', 'T004', '관리자 삭제', '관리자가 삭제됨', '__VALUE1__를 __VALUE2__ 관리자가 삭제', 'C_USER_ID|C_RESERVED1'),
('W20008', 'KOR', 'T005', '수동 사용자 추가', '수동 사용자가 추가됨', '__VALUE1__를 __VALUE2__ 관리자가 수동으로 추가', 'C_USER_ID|C_RESERVED1'),
('W20009', 'KOR', 'T005', '수동 사용자 삭제', '수동 사용자가 삭제됨', '__VALUE1__를 __VALUE2__ 관리자가 수동으로 삭제', 'C_USER_ID|C_RESERVED1'),
('W20010', 'KOR', 'T006', 'PRIMX Agent 업데이트', 'Agent 프로그램이 업데이트됨', 'PRIMX Agent 버전 __VALUE1__을 __VALUE2__ 관리자가 업데이트', 'C_RESERVED1|C_RESERVED2'),
('W20011', 'KOR', 'T006', '제품 업데이트', '제품 프로그램이 업데이트됨', '__VALUE1__ 프로그램 버전 __VALUE2__을 __VALUE3__ 관리자가 업데이트', 'C_RESERVED1|C_RESERVED2'),
('S10001', 'KOR', 'T007', '인사 연동', '인사 연동이 실행됨', '인사 연동 실행 __VALUE1__', 'C_RESERVED1'),
('S10002', 'KOR', 'T003', '미접속 시스템 삭제', '미접속 시스템이 삭제됨', '__VALUE1__ 미접속 시스템 삭제', 'C_AGENT_ID'),
('S20001', 'KOR', 'T003', '시스템 사용자 변경', '시스템에 사용자가 변경됨', '__VALUE1__ 시스템의 사용자를 __VALUE2__ 사용자로 변경', 'C_AGENT_ID|C_USER_ID'),
('S20003', 'KOR', 'T003', '에이전트 삭제', '에이전트가 삭제됨', '__VALUE1__ 삭제명령 전송됨', 'C_AGENT_ID');

-- 제품 로그의 리소스 테이블
CREATE TABLE IF NOT EXISTS T_RESOURCE_PRODUCT_LOG ( 
  `C_LOG_CODE` VARCHAR(10) NOT NULL COMMENT '로그 코드', 
  `C_LANGUAGE` VARCHAR(10) NOT NULL COMMENT '언어 구분. (세자리 언어코드(KOR, ENG ...) ISO 3166-1 alpha-3 codes 규약)',  
  `C_LOG_NAME` VARCHAR(200) DEFAULT NULL COMMENT '로그 이름', 
  `C_DESCRIPTION` VARCHAR(200) DEFAULT NULL COMMENT '로그 설명', 
  PRIMARY KEY (`C_LOG_CODE`,`C_LANGUAGE`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='제품 로그의 리소스 테이블';

INSERT IGNORE INTO T_RESOURCE_PRODUCT_LOG VALUES 
('E00001', 'KOR', '실행 오류', '실행 중 오류 발생함'), 
('W00001', 'KOR', '실행 경고', '실행 중 경고 발생함'), 
('P01001', 'KOR', '명령 실행', '제품의 명령을 실행함'), 
('P01002', 'KOR', '엑세스 리스트 색인 생성', '엑세스 리스트 색인 생성'), 
('P01003', 'KOR', '엑세스 리스트 폴더 생성', '지정된 위치에 엑세스 리스트 폴더 생성'), 
('P02001', 'KOR', '파일 암복호화', '파일을 암복호화함'), 
('P02002', 'KOR', '폴더 암복호화', '폴더를 암복호화함'), 
('P02003', 'KOR', '암호 영역 검사', '암호화 영역을 검사함'), 
('P02004', 'KOR', '암호 영역 접근', '선택한 암호화 영역 접근'), 
('P02005', 'KOR', '영역 암호화', '영역이 암호화됨'), 
('P02006', 'KOR', '암호 파일 접근', '암호 파일을 접근함'), 
('P02007', 'KOR', '공유 폴더 갱신', '공유 폴더 정보가 갱신됨'), 
('P02008', 'KOR', '시스템 동작', '시스템에서 동작하는 결과'), 
('P02009', 'KOR', '파일 변경', '파일에 대한 생성/제거/암호화 행위 등이 발생'), 
('P02010', 'KOR', '영역 업데이트', '영역에 대해 변화 또는 업데이트 발생'), 
('P02011', 'KOR', '접근 목록 파일 업데이트', '접근 목록 파일에 대해 변화 또는 업데이트 발생'), 
('P02012', 'KOR', '접근 목록 파일 접근', '접근 목록 파일에 대한 접근 또는 불러오기'), 
('P02013', 'KOR', '세션 업데이트', '세션의 상태 변경'), 
('P02014', 'KOR', '내보내기', '백업 내보내기'), 
('P02015', 'KOR', '인증서 파일 접근', '사용자 인증서 파일에 대한 접근'), 
('P02016', 'KOR', 'SSO 동작', 'SSO에 대한 설정'), 
('P02017', 'KOR', '접근 키 업데이트', '접근 키에 대한 변화 또는 업데이트 발생'), 
('P02018', 'KOR', '접근키 접근', '접근 키에 대한 접근 또는 불러오기'), 
('P02019', 'KOR', '복구 동작', '패스워드 또는 영역에 대한 복구 발생 '), 
('P02020', 'KOR', '그룹 정책 업데이트', '그룹 정책에 대한 변화 또는 업데이트 발생'), 
('P02021', 'KOR', '컨테이너', '컨테이너 변경');

-- 시스템 환경설정 정보 테이블
CREATE TABLE IF NOT EXISTS T_SERVER_CONFIG ( 
  `C_SEQ_NO` BIGINT(20) NOT NULL AUTO_INCREMENT COMMENT '번호', 
  `C_KEY_NAME` VARCHAR(50) NOT NULL COMMENT '키 이름', 
  `C_VALUE_DATA` VARCHAR(200) DEFAULT NULL COMMENT '설정 값', 
  `C_INSERT_DATE` DATETIME DEFAULT NULL COMMENT '생성 일자', 
  `C_UPDATE_DATE` DATETIME DEFAULT NULL COMMENT '수정 일자', 
  PRIMARY KEY (`C_SEQ_NO`), 
  UNIQUE KEY uidx_t_server_config (`C_KEY_NAME`), 
  KEY idx_t_server_config (`C_KEY_NAME`) 
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='시스템 환경설정 정보 테이블';

-- 회사 ID. 임시 설정 정보
INSERT IGNORE INTO T_SERVER_CONFIG (C_KEY_NAME, C_VALUE_DATA, C_INSERT_DATE) VALUES ('CompanyID', '00000001', UTC_TIMESTAMP());
-- 반출 파일 유효 기간
INSERT IGNORE INTO T_SERVER_CONFIG (C_KEY_NAME, C_VALUE_DATA, C_INSERT_DATE) VALUES ('FileExpireDayLimit', '3', UTC_TIMESTAMP());
-- 반출 파일 복호화 회수
INSERT IGNORE INTO T_SERVER_CONFIG (C_KEY_NAME, C_VALUE_DATA, C_INSERT_DATE) VALUES ('FileDecryptCountLimit', '10', UTC_TIMESTAMP());
-- 반출 파일 인증 서버 정보
INSERT IGNORE INTO T_SERVER_CONFIG (C_KEY_NAME, C_VALUE_DATA, C_INSERT_DATE) VALUES ('AuthServerAddress', '', UTC_TIMESTAMP());
-- Keep Alive 주기
INSERT IGNORE INTO T_SERVER_CONFIG (C_KEY_NAME, C_VALUE_DATA, C_INSERT_DATE) VALUES ('AgentKeepAliveMinuteInterval', '120', UTC_TIMESTAMP());
-- 로그 전송 주기
INSERT IGNORE INTO T_SERVER_CONFIG (C_KEY_NAME, C_VALUE_DATA, C_INSERT_DATE) VALUES ('AgentProductLogSendMinuteInterval', '120', UTC_TIMESTAMP());
-- 인사 DB 연동 여부
INSERT IGNORE INTO T_SERVER_CONFIG (C_KEY_NAME, C_VALUE_DATA, C_INSERT_DATE) VALUES ('HRImportYN', 'N', UTC_TIMESTAMP());
-- 인사 DB 연동 타입
INSERT IGNORE INTO T_SERVER_CONFIG (C_KEY_NAME, C_VALUE_DATA, C_INSERT_DATE) VALUES ('HRImportType', '', UTC_TIMESTAMP());
-- 인사 DB 연동 실행 계획
INSERT IGNORE INTO T_SERVER_CONFIG (C_KEY_NAME, C_VALUE_DATA, C_INSERT_DATE) VALUES ('HRImportRunTime', '', UTC_TIMESTAMP());
-- 시스템 정보 회수 기간 설정
INSERT IGNORE INTO T_SERVER_CONFIG (C_KEY_NAME, C_VALUE_DATA, C_INSERT_DATE) VALUES ('SystemInfoAutoDeleteDayLimit', '0', UTC_TIMESTAMP());
-- 시스템 정보 회수 시각 설정
INSERT IGNORE INTO T_SERVER_CONFIG (C_KEY_NAME, C_VALUE_DATA, C_INSERT_DATE) VALUES ('SystemInfoAutoDeleteRunTime', '00:00', UTC_TIMESTAMP());
-- Keep Alive 미전송 시 PRIMX Agent를 Off Line으로 상태 변경 시간
INSERT IGNORE INTO T_SERVER_CONFIG (C_KEY_NAME, C_VALUE_DATA, C_INSERT_DATE) VALUES ('AgentOfflineMinuteLimit', '720', UTC_TIMESTAMP());
-- Web Manager Console의 신청 및 결재 페이지 URL 설정
INSERT IGNORE INTO T_SERVER_CONFIG (C_KEY_NAME, C_VALUE_DATA, C_INSERT_DATE) VALUES ('AgentRegisterPageURL', '', UTC_TIMESTAMP());
-- 신청 및 결재 사용 유무
INSERT IGNORE INTO T_SERVER_CONFIG (C_KEY_NAME, C_VALUE_DATA, C_INSERT_DATE) VALUES ('AgentRegisterFunctionUseYN', 'N', UTC_TIMESTAMP());
-- 사용자 변경 사용 유무
INSERT IGNORE INTO T_SERVER_CONFIG (C_KEY_NAME, C_VALUE_DATA, C_INSERT_DATE) VALUES ('UserEditFunctionUseYN', 'N', UTC_TIMESTAMP());
-- 반출 파일 생성 사용 유무
INSERT IGNORE INTO T_SERVER_CONFIG (C_KEY_NAME, C_VALUE_DATA, C_INSERT_DATE) VALUES ('CreateExportFileFunctionUseYN', 'N', UTC_TIMESTAMP());
-- 과거 로그 삭제 주기 설정 기능
INSERT IGNORE INTO T_SERVER_CONFIG (C_KEY_NAME, C_VALUE_DATA, C_INSERT_DATE) VALUES ('OldLogDeleteMonthLimit', '0', UTC_TIMESTAMP());
INSERT IGNORE INTO T_SERVER_CONFIG (C_KEY_NAME, C_VALUE_DATA, C_INSERT_DATE) VALUES ('NotConnSystemUseYN', 'Y', UTC_TIMESTAMP());
INSERT IGNORE INTO T_SERVER_CONFIG (C_KEY_NAME, C_VALUE_DATA, C_INSERT_DATE) VALUES ('LogDataDeletePolicyUseYN', 'Y', UTC_TIMESTAMP());

-- 서버 시스템 내부 동작 관련 설정 
CREATE TABLE IF NOT EXISTS T_SERVER_INTERNAL_KV ( 
  `C_SEQ_NO` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '번호',
  `C_KEY_NAME` varchar(50) NOT NULL COMMENT '키 이름',
  `C_VALUE_DATA` varchar(200) DEFAULT NULL COMMENT '설정 값',
  `C_INSERT_DATE` datetime DEFAULT NULL COMMENT '생성 일자',
  `C_UPDATE_DATE` datetime DEFAULT NULL COMMENT '수정 일자',
  PRIMARY KEY (`C_SEQ_NO`),
  UNIQUE KEY `uidx_t_server_internal_kv` (`C_KEY_NAME`),
  KEY `idx_t_server_internal_kv` (`C_KEY_NAME`)
) ENGINE=InnoDB AUTO_INCREMENT=3798 DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci COMMENT='서버 시스템 내부 동작 관련 설정';

-- 서버 모듈 별 시스템 상태 체크 테이블
CREATE TABLE IF NOT EXISTS T_SERVER_MODULE_STATUS ( 
  `C_MODULE_NAME` varchar(100) NOT NULL COMMENT '서버 내부 모듈 이름',
  `C_CHECK_TIME` datetime DEFAULT NULL COMMENT '상태 체크 시각',
  PRIMARY KEY (`C_MODULE_NAME`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci COMMENT='서버 모듈 별 시스템 상태 체크 테이블';

-- 스마트 컨트렉트
CREATE TABLE IF NOT EXISTS `T_SMART_CONTRACT_SETTING` (
  `SC_SEQ_NO` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '번호',
  `SC_FILE_NAME` varchar(255) NOT NULL COMMENT '파일명',
  `SC_AGENT_ID` varchar(50) NOT NULL COMMENT '에이전트 ID',
  `SC_EMPLYR_ID` varchar(50) NOT NULL COMMENT '사용자 ID',
  `SC_CREATED_DATE` datetime DEFAULT current_timestamp() COMMENT '등록일시',
  PRIMARY KEY (`SC_SEQ_NO`),
  UNIQUE KEY `UK_SMART_CONTRACT` (`SC_FILE_NAME`,`SC_AGENT_ID`,`SC_EMPLYR_ID`)
) ENGINE=InnoDB AUTO_INCREMENT=14 DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

-- Agent가 설처된 시스템 정보 테이블
CREATE TABLE IF NOT EXISTS T_SYSTEM_MASTER ( 
  `C_SEQ_NO` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '번호',
  `C_AGENT_ID` varchar(50) NOT NULL COMMENT 'Agent ID',
  `C_WIN_VERSION_STRING` varchar(50) DEFAULT NULL COMMENT 'Windows 버전',
  `C_UUID` varchar(50) DEFAULT NULL COMMENT 'Windows UUID',
  `C_MAJOR_VERSION` int(11) DEFAULT NULL COMMENT 'OS Major 버전',
  `C_MINOR_VERSION` int(11) DEFAULT NULL COMMENT 'OS Minor 버전',
  `C_BUILD_NUMBER` varchar(50) DEFAULT NULL COMMENT 'OS Build 번호',
  `C_SERVICEPACK_MAJOR` int(11) DEFAULT NULL COMMENT 'OS 서비스팩 Major 버전',
  `C_SERVICEPACK_MINOR` int(11) DEFAULT NULL COMMENT 'OS 서비스팩 Minor 버전',
  `C_PROCESSOR_ARCHITECTURE` int(11) DEFAULT NULL COMMENT 'OS 프로세서 아키텍처',
  `C_RELEASE_ID` varchar(10) DEFAULT NULL COMMENT 'OS Releae ID',
  `C_COMPUTER_NAME` varchar(260) DEFAULT NULL COMMENT '컴퓨터 이름',
  `C_WORKGROUP_NAME` varchar(260) DEFAULT NULL COMMENT '작업그룹 이름',
  `C_ENCRYPTION_KEY` varchar(255) DEFAULT NULL COMMENT '암호 키',
  `C_AGENT_VERSION` varchar(15) DEFAULT NULL COMMENT 'Agent 버전',
  `C_AGENT_STATE` tinyint(4) DEFAULT 0 COMMENT 'Agent 상태 (0: 알 수 없음, 1: Online, 2: Offline)',
  `C_LAST_KA_DATE` datetime DEFAULT NULL COMMENT '마지막 Keep Alive 시간',
  `C_COMMON_SYSTEM_YN` char(1) DEFAULT 'N' COMMENT '공용 PC 여부 (Y: 공용 PC, N:사용자 PC(Default))',
  `C_USE_YN` char(1) DEFAULT 'Y' COMMENT '정보 사용 여부 (Y: 사용, N:미사용)',
  `C_UNINSTALL_YN` char(1) DEFAULT 'N' COMMENT 'Agent 삭제 여부 (Y: 삭제대상, N:삭제안함)',
  `C_MODIFIED_BY` varchar(50) DEFAULT NULL COMMENT '수정자 User ID',
  `C_MODIFIED_IP` varchar(15) DEFAULT NULL COMMENT '수정자 IP',
  `C_INSERT_DATE` datetime DEFAULT NULL COMMENT '생성 일자',
  `C_UPDATE_DATE` datetime DEFAULT NULL COMMENT '수정 일자',
  PRIMARY KEY (`C_SEQ_NO`),
  UNIQUE KEY `uidx_t_system_master` (`C_AGENT_ID`),
  KEY `idx_t_system_master` (`C_AGENT_ID`)
) ENGINE=InnoDB AUTO_INCREMENT=258 DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci COMMENT='Agent가 설처된 시스템 정보 테이블';

-- 프로시저 애러 테이블 (내부 처리용)
CREATE TABLE IF NOT EXISTS T_SYSTEM_PROC_ERROR (
  `C_ERROR_TIME` datetime NOT NULL COMMENT '프로시저 애러 발생 시각',
  `C_PROC_NAME` varchar(256) NOT NULL COMMENT '프로시저 이름',
  `C_SQL_STATE` varchar(20) DEFAULT NULL COMMENT 'SQL State 문자열',
  `C_ERROR_NO` int(11) DEFAULT 0 COMMENT 'SQL Error 번호',
  `C_COMMENT` varchar(2048) DEFAULT NULL COMMENT '비고',
  `C_ERROR_MSG` text DEFAULT NULL COMMENT '상세 애러 매시지',
  PRIMARY KEY (`C_ERROR_TIME`,`C_PROC_NAME`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci COMMENT='프로시저 애러 테이블 (내부 처리용)';

-- 임시 부서 정보 테이블
CREATE TABLE IF NOT EXISTS T_TEMP_DEPT_MASTER ( 
  `C_SEQ_NO` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '번호',
  `C_COMPANY_ID` varchar(50) NOT NULL COMMENT '회사 ID',
  `C_DEPT_ID` varchar(50) NOT NULL COMMENT '부서 ID',
  `C_PARENT_ID` varchar(50) DEFAULT NULL COMMENT '부모 부서 ID',
  `C_DEPT_NAME` varchar(50) DEFAULT NULL COMMENT '부서 이름',
  `C_RESERVED1` varchar(260) DEFAULT NULL COMMENT '예약 컬럼 1',
  `C_RESERVED2` varchar(260) DEFAULT NULL COMMENT '예약 컬럼 2',
  `C_RESERVED3` varchar(260) DEFAULT NULL COMMENT '예약 컬럼 3',
  `C_RESERVED4` varchar(260) DEFAULT NULL COMMENT '예약 컬럼 4',
  `C_RESERVED5` varchar(260) DEFAULT NULL COMMENT '예약 컬럼 5',
  `C_INSERT_DATE` datetime DEFAULT NULL COMMENT '생성 일자',
  `C_UPDATE_DATE` datetime DEFAULT NULL COMMENT '수정 일자',
  PRIMARY KEY (`C_SEQ_NO`),
  UNIQUE KEY `uidx_t_temp_dept_master` (`C_COMPANY_ID`,`C_DEPT_ID`),
  KEY `idx_t_temp_dept_master` (`C_COMPANY_ID`,`C_DEPT_ID`)
) ENGINE=InnoDB AUTO_INCREMENT=65 DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci COMMENT='임시 부서 정보 테이블';

-- 임시 사용자 정보 테이블
CREATE TABLE IF NOT EXISTS T_TEMP_USER_MASTER ( 
  `C_SEQ_NO` BIGINT(20) NOT NULL AUTO_INCREMENT COMMENT '번호', 
  `C_COMPANY_ID` VARCHAR(50) NOT NULL COMMENT '회사 ID', 
  `C_USER_ID` VARCHAR(50) NOT NULL COMMENT '사용자 ID', 
  `C_USER_PWD` VARCHAR(200) DEFAULT NULL COMMENT '암호', 
  `C_USER_NAME` VARCHAR(50) DEFAULT NULL COMMENT '사용자 이름', 
  `C_DEPT_ID` VARCHAR(50) DEFAULT NULL COMMENT '부서 ID', 
  `C_GRADE_NAME` VARCHAR(50) DEFAULT NULL COMMENT '직급', 
  `C_CELL_PHONE_NO` VARCHAR(30) DEFAULT NULL COMMENT '이동전화번호', 
  `C_TEL_NO` VARCHAR(30) DEFAULT NULL COMMENT '연락처', 
  `C_EMAIL_ADDR` VARCHAR(50) DEFAULT NULL COMMENT '이메일', 
  `C_EXT_NO` VARCHAR(30) DEFAULT NULL COMMENT '내선번호', 
  `C_RESERVED1` VARCHAR(260) DEFAULT NULL COMMENT '예약 컬럼 1', 
  `C_RESERVED2` VARCHAR(260) DEFAULT NULL COMMENT '예약 컬럼 2', 
  `C_RESERVED3` VARCHAR(260) DEFAULT NULL COMMENT '예약 컬럼 3', 
  `C_RESERVED4` VARCHAR(260) DEFAULT NULL COMMENT '예약 컬럼 4', 
  `C_RESERVED5` VARCHAR(260) DEFAULT NULL COMMENT '예약 컬럼 5', 
  `C_RESERVED6` VARCHAR(260) DEFAULT NULL COMMENT '예약 컬럼 6', 
  `C_RESERVED7` VARCHAR(260) DEFAULT NULL COMMENT '예약 컬럼 7', 
  `C_RESERVED8` VARCHAR(260) DEFAULT NULL COMMENT '예약 컬럼 8', 
  `C_RESERVED9` VARCHAR(260) DEFAULT NULL COMMENT '예약 컬럼 9', 
  `C_RESERVED10` VARCHAR(260) DEFAULT NULL COMMENT '예약 컬럼 10', 
  `C_INSERT_DATE` DATETIME DEFAULT NULL COMMENT '생성 일자', 
  `C_UPDATE_DATE` DATETIME DEFAULT NULL COMMENT '수정 일자', 
  PRIMARY KEY (`C_SEQ_NO`), 
  UNIQUE KEY uidx_t_temp_user_master (`C_COMPANY_ID`, `C_USER_ID`), 
  KEY idx_t_temp_user_master (`C_COMPANY_ID`, `C_USER_ID`) 
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='임시 사용자 정보 테이블';

-- PRIMX 제품 업데이트 정보 테이블
CREATE TABLE IF NOT EXISTS T_UPDATE_INFO ( 
  `C_SEQ_NO` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '번호',
  `C_PRODUCT_TYPE` tinyint(4) NOT NULL COMMENT '제품 종류 (0: 알 수 없음, 1: PRIMX ZONECENTRAL, 2: PRIMX ORIZON, 3: PRIMX CRYHOD, 4: PRIMX Agent)',
  `C_FILE_NAME` varchar(255) DEFAULT NULL COMMENT '파일명',
  `C_PROCESSOR_ARCHITECTURE` int(11) DEFAULT NULL COMMENT 'OS 프로세서 아키텍처 (0: 32BIT, 9: 64BIT)',
  `C_PRODUCT_VERSION` varchar(20) DEFAULT NULL COMMENT '업데이트 제품 버전',
  `C_UPDATE_FILE_HASH` varchar(200) DEFAULT NULL COMMENT '업데이트 파일 해시',
  `C_UPDATE_FILE_PATH` varchar(600) DEFAULT NULL COMMENT '업데이트 파일 경로',
  `C_INSERT_DATE` datetime DEFAULT NULL COMMENT '생성 일자',
  `C_UPDATE_DATE` datetime DEFAULT NULL COMMENT '수정 일자',
  PRIMARY KEY (`C_SEQ_NO`),
  UNIQUE KEY `uidx_t_update_info` (`C_PRODUCT_TYPE`,`C_PROCESSOR_ARCHITECTURE`),
  KEY `idx_t_update_info` (`C_PRODUCT_TYPE`,`C_PROCESSOR_ARCHITECTURE`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci COMMENT='PRIMX 제품 업데이트 정보 테이블';

INSERT IGNORE INTO T_UPDATE_INFO (C_SEQ_NO, C_PRODUCT_TYPE, C_PROCESSOR_ARCHITECTURE) VALUES (1, 4, 0), (2, 4, 9);

-- 업데이트 설정 서버 
CREATE TABLE IF NOT EXISTS `T_UPDATE_SERVER` (
  `C_SEQ_NO` int(11) NOT NULL AUTO_INCREMENT,
  `C_PROGRAM` varchar(255) NOT NULL,
  `C_VERSION` varchar(50) NOT NULL,
  `C_INSTALL_DATE` datetime NOT NULL DEFAULT current_timestamp(),
  PRIMARY KEY (`C_SEQ_NO`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

INSERT IGNORE INTO PRIMXDB.T_UPDATE_SERVER (C_SEQ_NO, C_PROGRAM, C_VERSION, C_INSTALL_DATE) VALUES(0, 'Admin Web Console', '1.0', current_timestamp());
INSERT IGNORE INTO PRIMXDB.T_UPDATE_SERVER (C_SEQ_NO, C_PROGRAM, C_VERSION, C_INSTALL_DATE) VALUES(0, 'User Web Console', '1.0', current_timestamp());
INSERT IGNORE INTO PRIMXDB.T_UPDATE_SERVER (C_SEQ_NO, C_PROGRAM, C_VERSION, C_INSTALL_DATE) VALUES(0, 'Support Web Console', '1.0', current_timestamp());
INSERT IGNORE INTO PRIMXDB.T_UPDATE_SERVER (C_SEQ_NO, C_PROGRAM, C_VERSION, C_INSTALL_DATE) VALUES(0, 'APPLICATION SYSTEM', '1.0', current_timestamp());
INSERT IGNORE INTO PRIMXDB.T_UPDATE_SERVER (C_SEQ_NO, C_PROGRAM, C_VERSION, C_INSTALL_DATE) VALUES(0, 'PRIMX DB', '1.0', current_timestamp());


-- 사용자 정보 테이블
CREATE TABLE IF NOT EXISTS T_USER_MASTER ( 
  `C_SEQ_NO` BIGINT(20) NOT NULL AUTO_INCREMENT COMMENT '번호',
  `C_COMPANY_ID` VARCHAR(50) NOT NULL DEFAULT '00000001' COMMENT '회사 ID',
  `C_USER_ID` VARCHAR(50) NOT NULL COMMENT '사용자 ID',
  `C_USER_PWD` VARCHAR(200) NOT NULL DEFAULT '$2a$10$Dzx.LU5e4X78479pI5flf.e3xzO8j6xySwpSLvU0m6fK756zyBVWu' COMMENT '암호',
  `C_USER_NAME` VARCHAR(50) DEFAULT NULL COMMENT '사용자 이름',
  `C_DEPT_ID` VARCHAR(50) DEFAULT 'NO_DEPT' COMMENT '부서 ID',
  `C_GRADE_NAME` VARCHAR(50) DEFAULT NULL COMMENT '직급',
  `C_CELL_PHONE_NO` VARCHAR(30) DEFAULT NULL COMMENT '이동전화번호',
  `C_TEL_NO` VARCHAR(30) DEFAULT NULL COMMENT '연락처',
  `C_EMAIL_ADDR` VARCHAR(50) DEFAULT NULL COMMENT '이메일',
  `C_EXT_NO` VARCHAR(30) DEFAULT NULL COMMENT '내선번호',
  `C_INSERTED_BY_HR_YN` CHAR(1) DEFAULT 'N' COMMENT '인사연동으로 생성됨 여부 (‘Y’: 인사연동됨, ‘N’: 관리자가 생성(Default))',
  `C_USE_YN` CHAR(1) DEFAULT 'Y' COMMENT '정보 사용 여부 (Y: 사용, N:미사용)',
  `C_USER_TYPE` tinyint(4) DEFAULT 0 COMMENT '사용자 타입. (0: 일반사용자(Default), 1: 게스트 사용자)',
  `C_USER_ROLE` VARCHAR(255) DEFAULT NULL COMMENT '권한. ADMIN/User',
  `C_FULL_NAME` VARCHAR(255) DEFAULT NULL COMMENT '성명',
  `C_TITLE_NAME` VARCHAR(255) DEFAULT NULL COMMENT '제목명',
  `C_INSERTED_BY` VARCHAR(50) DEFAULT NULL COMMENT '등록자',
  `C_INSERTED_IP` VARCHAR(15) DEFAULT NULL COMMENT '등록 IP',
  `C_MODIFIED_BY` VARCHAR(50) DEFAULT NULL COMMENT '수정자',
  `C_MODIFIED_IP` VARCHAR(15) DEFAULT NULL COMMENT '수정 IP',
  `C_ACTIVIE_YN` CHAR(1) DEFAULT 'Y' COMMENT '활성 여부. (‘Y’: 활성(Default), ‘N’: 비활성)',
  `C_IS_PERIOD` CHAR(1) DEFAULT 'N' COMMENT '사용 기간을 지정했는지 여부. (‘Y’: 지정, ‘N’: 미지정(Default))',
  `C_PWD_CHANGED` CHAR(1) NOT NULL DEFAULT 'Y' COMMENT '패스워드 변경 여부. (‘Y’: 변경, ‘N’: 미변경(Default))',
  `C_BEGIN_DATE` DATETIME DEFAULT NULL COMMENT '시작일',
  `C_EXPIRED_DATE` DATETIME DEFAULT NULL COMMENT '유효 기간',
  `C_PWD_EXPIRED_DATE` DATETIME DEFAULT NULL COMMENT '패스워드 만료일',
  `C_RESERVED1` VARCHAR(260) DEFAULT NULL COMMENT '예약 컬럼 1',
  `C_RESERVED2` VARCHAR(260) DEFAULT NULL COMMENT '예약 컬럼 2',
  `C_RESERVED3` VARCHAR(260) DEFAULT NULL COMMENT '예약 컬럼 3',
  `C_RESERVED4` VARCHAR(260) DEFAULT NULL COMMENT '예약 컬럼 4',
  `C_RESERVED5` VARCHAR(260) DEFAULT NULL COMMENT '예약 컬럼 5',
  `C_RESERVED6` VARCHAR(260) DEFAULT NULL COMMENT '예약 컬럼 6',
  `C_RESERVED7` VARCHAR(260) DEFAULT NULL COMMENT '예약 컬럼 7',
  `C_RESERVED8` VARCHAR(260) DEFAULT NULL COMMENT '예약 컬럼 8',
  `C_RESERVED9` VARCHAR(260) DEFAULT NULL COMMENT '예약 컬럼 9',
  `C_RESERVED10` VARCHAR(260) DEFAULT NULL COMMENT '예약 컬럼 10',
  `C_INSERT_DATE` DATETIME DEFAULT NULL COMMENT '생성 일자',
  `C_UPDATE_DATE` DATETIME DEFAULT NULL COMMENT '수정 일자',  
  PRIMARY KEY (`C_SEQ_NO`), 
  UNIQUE KEY uidx_t_user_master (`C_COMPANY_ID`, `C_USER_ID`), 
  UNIQUE KEY uidx_c_email_addr (`C_EMAIL_ADDR`),
  KEY idx_t_user_master (`C_COMPANY_ID`, `C_USER_ID`) 
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='사용자 정보 테이블';


-- 사용자 시스템 정보 매칭 테이블
CREATE TABLE IF NOT EXISTS T_USER_SYSTEM_MATCH ( 
  `C_SEQ_NO` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '번호',
  `C_COMPANY_ID` varchar(50) NOT NULL COMMENT '회사 ID',
  `C_USER_ID` varchar(50) NOT NULL COMMENT '사용자 ID',
  `C_AGENT_ID` varchar(50) NOT NULL COMMENT 'Agent ID',
  `C_USER_REG_DATE` datetime DEFAULT NULL COMMENT '사용자 등록 일자',
  `C_MODIFIED_BY` varchar(50) DEFAULT NULL COMMENT '수정자',
  `C_MODIFIED_IP` varchar(15) DEFAULT NULL COMMENT '수정 IP',
  `C_INSERT_DATE` datetime DEFAULT NULL COMMENT '생성 일자',
  `C_UPDATE_DATE` datetime DEFAULT NULL COMMENT '수정 일자',
  PRIMARY KEY (`C_SEQ_NO`),
  UNIQUE KEY `uidx_t_user_system_match` (`C_COMPANY_ID`,`C_USER_ID`,`C_AGENT_ID`),
  KEY `idx_t_user_system_match` (`C_COMPANY_ID`,`C_USER_ID`,`C_AGENT_ID`)
) ENGINE=InnoDB AUTO_INCREMENT=202 DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci COMMENT='사용자 시스템 정보 매칭 테이블';

-- 파일전송 존 매핑 테이블
CREATE TABLE IF NOT EXISTS `T_ZONE_MAP` (
  `C_APID` int(11) NOT NULL COMMENT 'T_AGENT_POLICY_MASTER의 C_POLICY_ID',
  `C_ZONEID` int(11) NOT NULL COMMENT 'T_FILE_ZONE의 C_ZONE_ID',
  PRIMARY KEY (`C_APID`,`C_ZONEID`),
  KEY `FK_ZONE_MAP_ZONEID` (`C_ZONEID`),
  CONSTRAINT `FK_ZONE_MAP_APID` FOREIGN KEY (`C_APID`) REFERENCES `T_AGENT_POLICY_MASTER` (`C_POLICY_ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_ZONE_MAP_ZONEID` FOREIGN KEY (`C_ZONEID`) REFERENCES `T_FILE_ZONE` (`C_ZONE_ID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci COMMENT='ZONE 매핑 테이블';

/* ----------------------------------------------------------------------------
	FUNCTIONS
---------------------------------------------------------------------------- */ 
-- 버전 문자열 비교 함수
DELIMITER |
DROP FUNCTION IF EXISTS `PRIMXDB`.`FUNC_COMPARE_VERSION` |
CREATE FUNCTION `PRIMXDB`.`FUNC_COMPARE_VERSION` 
(
	IN_VERSION_FST VARCHAR(50),
	IN_VERSION_SND VARCHAR(50)
) RETURNS TINYINT(4)
BEGIN
	/*
	COMMENT: 버전 문자열 비교 함수
	PARAMETERS:
		IN_VERSION_FST - 첫 번쨰 버전 문자열
		IN_VERSION_SND - 두 번째 버전 문자열
	RETURN: 
		 1 - 첫 번째 버전이 두 번째 버전 보다 크다
		 0 - 두 버전이 일치한다.
		-1 - 첫 번째 버전이 두 번째 버전 보다 작다
	HISTORY:
		2023-07-07 생성
		2023-10-15 비전 비교 루틴 block. 버전이 다르면 1을 리턴하도록 수정
	*/
	/*
	DECLARE V_RET             TINYINT(4)  DEFAULT 0; -- 리턴값
	DECLARE V_BUF_FST         VARCHAR(50) DEFAULT '';
	DECLARE V_BUF_SND         VARCHAR(50) DEFAULT '';
	DECLARE V_CNT_DOT_FST     TINYINT(4)  DEFAULT 0;
	DECLARE V_CNT_DOT_SND     TINYINT(4)  DEFAULT 0;
	DECLARE V_CNT_DOT_MIN     TINYINT(4)  DEFAULT 0;
	DECLARE V_CNT_LOOP        TINYINT(4)  DEFAULT -1;
	DECLARE V_CHR_CUR_POS_FST TINYINT(4)  DEFAULT 0;
	DECLARE V_CHR_CUR_POS_SND TINYINT(4)  DEFAULT 0;
	DECLARE V_CHR_PRV_POS_FST TINYINT(4)  DEFAULT 0;
	DECLARE V_CHR_PRV_POS_SND TINYINT(4)  DEFAULT 0;
	DECLARE V_STR_CUR_FST     VARCHAR(30) DEFAULT '';
	DECLARE V_STR_CUR_SND     VARCHAR(30) DEFAULT '';
	DECLARE V_VER_CUR_FST     INT         DEFAULT 0;
	DECLARE V_VER_CUR_SND     INT         DEFAULT 0;
	
	SET V_BUF_FST = REGEXP_REPLACE(IN_VERSION_FST, '[a-z]|[\]\[\, ]', '');
	SET V_BUF_SND = REGEXP_REPLACE(IN_VERSION_SND, '[a-z]|[\]\[\, ]', '');
	SET V_CNT_DOT_FST = LENGTH(V_BUF_FST) - LENGTH(REPLACE(V_BUF_FST, '.', ''));
	SET V_CNT_DOT_SND = LENGTH(V_BUF_SND) - LENGTH(REPLACE(V_BUF_SND, '.', ''));
	SET V_CNT_DOT_MIN = LEAST(V_CNT_DOT_FST, V_CNT_DOT_SND);
	
	VER_CHK_LOOP : WHILE (V_CNT_LOOP < V_CNT_DOT_MIN) DO
		SET V_CNT_LOOP = V_CNT_LOOP + 1;
		
		IF V_CNT_LOOP = V_CNT_DOT_MIN THEN
			IF V_CNT_DOT_MIN = V_CNT_DOT_FST THEN
				SET V_STR_CUR_FST = SUBSTRING(V_BUF_FST, (V_CHR_PRV_POS_FST + 1), (LENGTH(V_BUF_FST) - V_CHR_PRV_POS_FST));
			ELSE
				SET V_CHR_CUR_POS_FST = LOCATE('.', V_BUF_FST, (V_CHR_PRV_POS_FST + 1));
				SET V_STR_CUR_FST = SUBSTRING(V_BUF_FST, (V_CHR_PRV_POS_FST + 1), (V_CHR_CUR_POS_FST - (V_CHR_PRV_POS_FST + 1)));
				SET V_CHR_PRV_POS_FST = V_CHR_CUR_POS_FST;			
			END IF;
			
			IF V_CNT_DOT_MIN = V_CNT_DOT_SND THEN
				SET V_STR_CUR_SND = SUBSTRING(V_BUF_SND, (V_CHR_PRV_POS_SND + 1), (LENGTH(V_BUF_SND) - V_CHR_PRV_POS_SND));
			ELSE
				SET V_CHR_CUR_POS_SND = LOCATE('.', V_BUF_SND, (V_CHR_PRV_POS_SND + 1));
				SET V_STR_CUR_SND = SUBSTRING(V_BUF_SND, (V_CHR_PRV_POS_SND + 1), (V_CHR_CUR_POS_SND - (V_CHR_PRV_POS_SND + 1)));
				SET V_CHR_PRV_POS_SND = V_CHR_CUR_POS_SND;
			END IF;
		ELSE
			SET V_CHR_CUR_POS_FST = LOCATE('.', V_BUF_FST, (V_CHR_PRV_POS_FST + 1));
			SET V_STR_CUR_FST = SUBSTRING(V_BUF_FST, (V_CHR_PRV_POS_FST + 1), (V_CHR_CUR_POS_FST - (V_CHR_PRV_POS_FST + 1)));
			SET V_CHR_PRV_POS_FST = V_CHR_CUR_POS_FST;
			
			SET V_CHR_CUR_POS_SND = LOCATE('.', V_BUF_SND, (V_CHR_PRV_POS_SND + 1));
			SET V_STR_CUR_SND = SUBSTRING(V_BUF_SND, (V_CHR_PRV_POS_SND + 1), (V_CHR_CUR_POS_SND - (V_CHR_PRV_POS_SND + 1)));
			SET V_CHR_PRV_POS_SND = V_CHR_CUR_POS_SND;
		END IF;
		
		SET V_VER_CUR_FST = CONVERT(V_STR_CUR_FST, INT);
		SET V_VER_CUR_SND = CONVERT(V_STR_CUR_SND, INT);
		
		IF V_VER_CUR_FST > V_VER_CUR_SND THEN 
			SET V_RET = 1;
			LEAVE VER_CHK_LOOP;
		ELSEIF V_VER_CUR_FST < V_VER_CUR_SND THEN 
			SET V_RET = -1;
			LEAVE VER_CHK_LOOP;
		END IF;		
	END WHILE;
	
	IF V_RET = 0 AND V_CNT_DOT_FST != V_CNT_DOT_SND THEN
		IF     V_CNT_DOT_FST > V_CNT_DOT_SND THEN SET V_RET = 1;
		ELSEIF V_CNT_DOT_FST < V_CNT_DOT_SND THEN SET V_RET = -1;
		END IF;
	END IF;
	*/
	DECLARE V_RET     TINYINT(4)  DEFAULT 0; -- 리턴값
	DECLARE V_BUF_FST VARCHAR(50) DEFAULT '';
	DECLARE V_BUF_SND VARCHAR(50) DEFAULT '';
	
	SET V_BUF_FST = TRIM(IN_VERSION_FST);
	SET V_BUF_SND = TRIM(IN_VERSION_SND);
	
	IF V_BUF_FST = V_BUF_SND THEN 
		SET V_RET = 0;
	ELSE
		SET V_RET = 1;
	END IF;
  
	RETURN V_RET;
END |
DELIMITER ; 

-- T_SERVER_CONFIG 에서 입력된 키에 해당하는 값을 리턴. 각 키 값에 따라 별도 체크 루틴 존재 
DELIMITER |
DROP FUNCTION IF EXISTS `PRIMXDB`.`FUNC_GET_SERVER_CONFIG` |
CREATE FUNCTION `PRIMXDB`.`FUNC_GET_SERVER_CONFIG`
(
	IN_KEY_NAME VARCHAR(50)
) RETURNS VARCHAR(200) NOT DETERMINISTIC 
BEGIN
	/* T_SERVER_CONFIG 에서 입력된 키에 해당하는 값을 리턴. 각 키 값에 따라 별도 체크 루틴 존재 
	<키 별 규칙>
	(1) HRImportYN (인사 DB 연동 여부)
		Y / N 두 가지 외엔 애러
	(2) HRImportType (인사 DB 연동 타입)
		INTERVAL / DAILY / WEEKLY / MONTHLY 외엔 애러
	(3) HRImportRunTime (인사 DB 연동 실행 계획)
		HH:MM 형식 외엔 애러
	(4) SystemInfoAutoDeleteDayLimit (시스템 정보 회수 기간 설정)
		숫자 외엔 애러
	(5) SystemInfoAutoDeleteRunTime (시스템 정보 회수 시각 설정)
		HH:MM 형식 외엔 애러
	(6) AgentOfflineMinuteLimit (Keep Alive 미전송 시 PRIMX Agent를 Off Line으로 상태 변경 시간)
		숫자 이외에는 애러
	(7) OldLogDeleteMonthLimit (과거 로그 삭제 주기 설정 기능)
		숫자 이외에는 애러
	<애러 리턴>
	(1) INVALID_VALUE : 잘못된 설정값
	(2) NOT_FOUND_KEY : 키가 없음
	*/    
	DECLARE V_RET        VARCHAR(200) DEFAULT ''; -- 값이 없거나 형식에 맞지 않으면 'INVALID_VALUE' 리턴
	DECLARE V_CNT_KEY    INT(11)      DEFAULT 0;
	DECLARE V_VALUE      VARCHAR(200) DEFAULT '';
	DECLARE V_CHECK      TINYINT(1)   DEFAULT FALSE;
	
	SELECT COUNT(*), C_VALUE_DATA INTO V_CNT_KEY, V_VALUE FROM T_SERVER_CONFIG WHERE C_KEY_NAME = IN_KEY_NAME;
	
	IF V_CNT_KEY = 0 THEN
		SET V_RET = 'NOT_FOUND_KEY';
	ELSE
		IF IN_KEY_NAME = 'FileExpireDayLimit' THEN 
			SELECT V_VALUE REGEXP('^[0-9]+$') INTO V_CHECK;
			IF V_CHECK = FALSE THEN 
				SET V_RET = 'INVALID_VALUE';
			END IF;
		ELSEIF IN_KEY_NAME = 'FileDecryptCountLimit' THEN 
			SELECT V_VALUE REGEXP('^[0-9]+$') INTO V_CHECK;
			IF V_CHECK = FALSE THEN 
				SET V_RET = 'INVALID_VALUE';
			END IF;
		ELSEIF IN_KEY_NAME = 'AgentKeepAliveMinuteInterval' THEN 
			SELECT V_VALUE REGEXP('^[0-9]+$') INTO V_CHECK;
			IF V_CHECK = FALSE THEN 
				SET V_RET = 'INVALID_VALUE';
			END IF;
		ELSEIF IN_KEY_NAME = 'AgentProductLogSendMinuteInterval' THEN 
			SELECT V_VALUE REGEXP('^[0-9]+$') INTO V_CHECK;
			IF V_CHECK = FALSE THEN 
				SET V_RET = 'INVALID_VALUE';
			END IF;
		ELSEIF IN_KEY_NAME = 'HRImportYN' THEN 
			IF V_VALUE != 'Y' AND V_VALUE != 'N' THEN 
				SET V_RET = 'INVALID_VALUE';
			END IF;
		ELSEIF IN_KEY_NAME = 'HRImportType' THEN 
			IF V_VALUE != 'DAILY' AND V_VALUE != 'WEEKLY' AND V_VALUE != 'MONTHLY' THEN 
				SET V_RET = 'INVALID_VALUE';
			END IF;
		ELSEIF IN_KEY_NAME = 'HRImportRunTime' THEN 
			IF LENGTH(V_VALUE) = 0 THEN 
				SET V_RET = 'INVALID_VALUE';
			END IF;
		ELSEIF IN_KEY_NAME = 'SystemInfoAutoDeleteDayLimit' THEN 
			SELECT V_VALUE REGEXP('^[0-9]+$') INTO V_CHECK;
			IF V_CHECK = FALSE THEN 
				SET V_RET = 'INVALID_VALUE';
			END IF;
		ELSEIF IN_KEY_NAME = 'SystemInfoAutoDeleteRunTime' THEN 
			SELECT V_VALUE REGEXP('^[0-9]+$') INTO V_CHECK;
			IF V_CHECK = FALSE THEN SET V_RET = 'INVALID_VALUE'; END IF;
			IF CAST(V_VALUE AS INTEGER) > 23 THEN SET V_RET = 'INVALID_VALUE'; END IF;
		ELSEIF IN_KEY_NAME = 'AgentOfflineMinuteLimit' THEN 
			SELECT V_VALUE REGEXP('^[0-9]+$') INTO V_CHECK;
			IF V_CHECK = FALSE THEN 
				SET V_RET = 'INVALID_VALUE';
			END IF;
		ELSEIF IN_KEY_NAME = 'OldLogDeleteMonthLimit' THEN 
			SELECT V_VALUE REGEXP('^[0-9]+$') INTO V_CHECK;
			IF V_CHECK = FALSE THEN 
				SET V_RET = 'INVALID_VALUE';
			END IF;
		END IF;
	END IF;
	
	IF V_RET != 'INVALID_VALUE' AND V_RET != 'NOT_FOUND_KEY' THEN 
		SET V_RET = V_VALUE;
	END IF; 
  
	RETURN V_RET;
END |
DELIMITER ; 

-- 파일 경로에서 파일 이름 추출
DELIMITER |
DROP FUNCTION IF EXISTS `PRIMXDB`.`FUNC_GET_FILE_NAME_FROM_PATH` |
CREATE FUNCTION `PRIMXDB`.`FUNC_GET_FILE_NAME_FROM_PATH`
(
	IN_FILE_PATH VARCHAR(1024)
) RETURNS VARCHAR(200) NOT DETERMINISTIC 
BEGIN
	/*
	COMMENT: 파일 경로에서 파일 이름 추출
	PARAMETERS:
		IN_FILE_PATH - 파일 전체 경로
	RETURN: 
		파일 이름
	HISTORY:
		2023-10-05 생성
	*/
	DECLARE V_RET VARCHAR(200) DEFAULT ''; 
	DECLARE V_POS_LAST_SLASH INT(11) DEFAULT 0;
	
	SET V_POS_LAST_SLASH = INSTR(REVERSE(IN_FILE_PATH), '/');
	
	IF V_POS_LAST_SLASH > 0 THEN
		SET V_RET = RIGHT(IN_FILE_PATH, (V_POS_LAST_SLASH - 1));
	END IF;
	
	RETURN V_RET;	
END |
DELIMITER ; 

/* ----------------------------------------------------------------------------
	PROCEDURES - SERVER SYSTEM 
---------------------------------------------------------------------------- */ 
-- T_USER_SYSTEM_MATCH 테이블에 레코드 삽입 프로시저
DELIMITER |
DROP PROCEDURE IF EXISTS `PRIMXDB`.`PROC_CHECK_INSERT_USER_SYSTEM_MATCH` |
CREATE PROCEDURE `PRIMXDB`.`PROC_CHECK_INSERT_USER_SYSTEM_MATCH`
(
	IN IN_AGENT_ID   VARCHAR(50), -- Agent ID
	IN IN_USER_ID    VARCHAR(50), -- User ID
	IN IN_COMPANY_ID VARCHAR(50), -- Company ID
	IN IN_SHARE_PC   VARCHAR(10), -- 공용 PC 여부. YES/NO
	OUT OUT_RESULT   INT(11)      -- return value (1:성공 / -1:IN_AGENT_ID가 없음 / -2:USER_ID가 T_USER_MASTER에 없음 / -3:프로시저 수행오류)
)
BEGIN
	/*
	COMMENT: T_USER_SYSTEM_MATCH 테이블에 레코드 삽입 프로시저
	PARAMETERS:
		IN_AGENT_ID    - Agent ID
		IN_USER_ID     - User ID
		IN_COMPANY_ID  - 회사 ID
		IN_SHARE_PC    - 공용 PC 여부. YES/NO
		OUT OUT_RESULT - return value (1:성공 / -1:IN_AGENT_ID가 없음 / -2:USER_ID가 T_USER_MASTER에 없음 / -3:프로시저 수행오류)
	HISTORY:
		2023-07-13 생성
	*/

	DECLARE V_CNT_USER_SYSTEM_MATCH INT(11) DEFAULT 0; -- AGENT_ID에 일차하는 T_USER_SYSTEM_MATCH 레코드 존재 여부
	DECLARE V_CNT_SYSTEM_MASTER     INT(11) DEFAULT 0; -- AGENT_ID에 일치하는 T_SYSTEM_MASTER 레코드 존재 여부
	DECLARE V_CNT_USER_MASTER       INT(11) DEFAULT 0; -- USER_ID에 일치하는 T_USER_MASTER 레코드 존재 여부
	DECLARE V_PREV_USER_ID_MATCHED  VARCHAR(50) DEFAULT '';
	DECLARE V_PREV_USER_ID          VARCHAR(50) DEFAULT '';
	DECLARE V_COMPANY_ID            VARCHAR(50) DEFAULT '';
	DECLARE V_PREV_COMMON_SYSTEM_YN CHAR(1) DEFAULT 'N'; -- PC 이전 상태. N:일반 PC / Y:공용 PC
	DECLARE V_PREV_USER_TYPE        INT(11) DEFAULT 0;   -- 사용자 이전 상태. 0:일반 사용자 / 1:게스트 사용자
	DECLARE V_LOG_COMMENT           VARCHAR(2048) DEFAULT '';
	DECLARE V_APPLIED_USER_ID       VARCHAR(50) DEFAULT '';
	DECLARE V_COMPUTER_NAME         VARCHAR(260) DEFAULT '';
	
	-- 예외처리 변수 선언
	DECLARE V_PROC_NAME      VARCHAR(256) DEFAULT 'PROC_CHECK_INSERT_USER_SYSTEM_MATCH';
	DECLARE V_SQL_STATE      VARCHAR(20) DEFAULT '';
	DECLARE V_INT_ERROR_NO   INT(11) DEFAULT 0;
	DECLARE V_PROC_COMMENT   VARCHAR(2048) DEFAULT '';
	DECLARE V_TEXT_ERROR_MSG TEXT;
	
	-- 예외처리
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
	BEGIN
		ROLLBACK;
		GET DIAGNOSTICS CONDITION 1 V_SQL_STATE = RETURNED_SQLSTATE
		, V_INT_ERROR_NO = MYSQL_ERRNO
		, V_TEXT_ERROR_MSG = MESSAGE_TEXT;
		SET V_PROC_COMMENT = CONCAT('AGENT_ID:', IN_AGENT_ID, '/USER_ID:', IN_USER_ID, '/COMPANY_ID:', IN_COMPANY_ID, '/SHARE_PC:', IN_SHARE_PC);
		INSERT T_SYSTEM_PROC_ERROR (C_ERROR_TIME, C_PROC_NAME, C_SQL_STATE, C_ERROR_NO, C_COMMENT, C_ERROR_MSG)
		VALUES (NOW(), V_PROC_NAME, V_SQL_STATE, V_INT_ERROR_NO, V_PROC_COMMENT, V_TEXT_ERROR_MSG);
		SET OUT_RESULT = -4;
	END;
	
	START TRANSACTION;
	
	SET OUT_RESULT = 1;
	
	IF LENGTH(IN_AGENT_ID) = 0 THEN 
		-- error Case 
		SET V_LOG_COMMENT = CONCAT('AGENT_ID:', IN_AGENT_ID, '/USER_ID:', IN_USER_ID, '/COMPANY_ID:', IN_COMPANY_ID, '/SHARE_PCP:', IN_SHARE_PC, '/COMMENT:AGENT_ID is empty');
		SET OUT_RESULT = -1;
	ELSE 
		-- 이전 정보 추출
		SELECT 
			A.CNT, IFNULL(A.C_USER_ID, ''), 
			B.CNT, IFNULL(B.C_COMMON_SYSTEM_YN, ''), IFNULL(B.C_COMPUTER_NAME, ''), 
			C.CNT, IFNULL(C.EMPLYR_ID, ''), IFNULL(C.COMPANY_ID, '') 
		INTO 
			V_CNT_USER_SYSTEM_MATCH, V_PREV_USER_ID_MATCHED,  			
			V_CNT_SYSTEM_MASTER, V_PREV_COMMON_SYSTEM_YN, V_COMPUTER_NAME, 
			V_CNT_USER_MASTER, V_PREV_USER_ID, V_COMPANY_ID 
		FROM
		(SELECT COUNT(*) AS CNT, C_USER_ID FROM T_USER_SYSTEM_MATCH WHERE C_AGENT_ID = IN_AGENT_ID) AS A,
		(SELECT COUNT(*) AS CNT, C_COMMON_SYSTEM_YN, C_COMPUTER_NAME FROM T_SYSTEM_MASTER WHERE C_AGENT_ID = IN_AGENT_ID) AS B,
		(SELECT COUNT(*) AS CNT, EMPLYR_ID, COMPANY_ID FROM COMTNEMPLYRINFO WHERE EMPLYR_ID = IN_USER_ID) AS C;		

		-- Company ID가 공백인 경우, 기본  Company ID로 대체
		IF LENGTH(V_COMPANY_ID) = 0 THEN
			SET V_COMPANY_ID = FUNC_GET_SERVER_CONFIG('CompanyID');
		END IF;

		-- Agent ID로 다른 UserID와 매칭된 경우 매칭 레코드 삭제 
		IF V_PREV_USER_ID_MATCHED != IN_USER_ID THEN
			DELETE FROM T_USER_SYSTEM_MATCH WHERE C_AGENT_ID = IN_AGENT_ID;
		END IF;
		
		-- 공용 PC 일 때 공용 PC로 등록
		IF IN_SHARE_PC = 'YES' THEN
			DELETE FROM T_USER_SYSTEM_MATCH WHERE C_AGENT_ID = IN_AGENT_ID;
			/* 2023-10-18 blocked
			DELETE FROM T_USER_SYSTEM_MATCH WHERE C_AGENT_ID = IN_AGENT_ID AND C_USER_ID <> 'PUBLIC_PC';
			INSERT IGNORE INTO T_USER_SYSTEM_MATCH 
			(C_COMPANY_ID, C_USER_ID, C_AGENT_ID, C_USER_REG_DATE, C_MODIFIED_BY, C_MODIFIED_IP, C_INSERT_DATE)
			VALUES (V_COMPANY_ID, 'PUBLIC_PC', IN_AGENT_ID, UTC_TIMESTAMP(), 'SYSTEM', '127.0.0.1', UTC_TIMESTAMP());
			SET V_APPLIED_USER_ID = 'PUBLIC_PC';
			*/
		-- 일반 PC 일 때
		
		ELSE
			-- 공용 PC가 아니면서 USER_ID가 없는 경우 error
			IF LENGTH(IN_USER_ID) = 0 THEN
				SET OUT_RESULT = -3;
			-- USER_ID가 T_USER_MASTER에 없을 경우 error
			ELSEIF V_CNT_USER_MASTER = 0 THEN
				SET OUT_RESULT = -2;
			ELSE
				INSERT IGNORE INTO T_USER_SYSTEM_MATCH 
				(C_COMPANY_ID, C_USER_ID, C_AGENT_ID, C_USER_REG_DATE, C_MODIFIED_BY, C_MODIFIED_IP, C_INSERT_DATE)
				VALUES (V_COMPANY_ID, IN_USER_ID, IN_AGENT_ID, UTC_TIMESTAMP(), 'SYSTEM', '127.0.0.1', UTC_TIMESTAMP());
				SET V_APPLIED_USER_ID = IN_USER_ID;
			END IF;
		END IF;
	END IF;	
	
	-- 로그 기록 
	IF OUT_RESULT = 1 THEN
		INSERT INTO T_MANAGE_LOG (C_LOG_TIME, C_LOG_CODE, C_LOG_TYPE, C_USER_ID, C_AGENT_ID, C_COMPUTER_NAME, C_COMMENT)
		VALUES 
		(UTC_TIMESTAMP(), 'S20001', 1, V_APPLIED_USER_ID, IN_AGENT_ID, V_COMPUTER_NAME, V_LOG_COMMENT);
	END IF;
	
	IF OUT_RESULT = 1 THEN COMMIT; 
	ELSE                   ROLLBACK;
	END IF;
END |
DELIMITER ;

-- Agent의 제품별 (Agent 포함) 업데이트 여부 필요 판정 프로시저
DELIMITER |
DROP PROCEDURE IF EXISTS `PRIMXDB`.`PROC_CHECK_PRODUCT_UPDATE` |
CREATE PROCEDURE `PRIMXDB`.`PROC_CHECK_PRODUCT_UPDATE`
(
	IN IN_AGENT_ID             VARCHAR(50), -- Agent ID
	OUT OUT_AGENT_UPDATE       VARCHAR(10), -- PRIMX Agent 업데이트 여부 (YES/NO)
	OUT OUT_ZONECENTRAL_UPDATE VARCHAR(10), -- PRIMX ZONECENTRAL 업데이트 여부 (YES/NO)
	OUT OUT_ORIZON_UPDATE      VARCHAR(10), -- PRIMX ORIZON 업데이트 여부 (YES/NO)
	OUT OUT_CRYHOD_UPDATE      VARCHAR(10)  -- PRIMX CRYHOD 업데이트 여부 (YES/NO)
)
BEGIN
	/*
	COMMENT: Agent의 제품별 (Agent 포함) 업데이트 여부 필요 판정 프로시저
	PARAMETERS:
		IN_AGENT_ID            - Agent ID
		OUT_AGENT_UPDATE       - PRIMX Agent 업데이트 여부 (YES/NO)
		OUT_ZONECENTRAL_UPDATE - PRIMX ZONECENTRAL 업데이트 여부 (YES/NO)
		OUT_ORIZON_UPDATE      - PRIMX ORIZON 업데이트 여부 (YES/NO)
		OUT OUT_CRYHOD_UPDATE  - PRIMX CRYHOD 업데이트 여부 (YES/NO)
	HISTORY:
		2023-07-15 생성
	*/

	DECLARE V_CNT_SYSTEM        INT(11)     DEFAULT 0;
	DECLARE V_PROCESSOR_ARCH    INT(11)     DEFAULT 9;
	DECLARE V_CNT_INSTALLED_ZCR INT(11)     DEFAULT 0;
	DECLARE V_CNT_INSTALLED_OZN INT(11)     DEFAULT 0;
	DECLARE V_CNT_INSTALLED_CHD INT(11)     DEFAULT 0;
	DECLARE V_CNT_INSTALLED_AGT INT(11)     DEFAULT 0;
	DECLARE V_CNT_UPDATE_ZCR    INT(11)     DEFAULT 0;
	DECLARE V_CNT_UPDATE_OZN    INT(11)     DEFAULT 0;
	DECLARE V_CNT_UPDATE_CHD    INT(11)     DEFAULT 0;
	DECLARE V_CNT_UPDATE_AGT    INT(11)     DEFAULT 0;
	DECLARE V_VER_INSTALLED_ZCR VARCHAR(50) DEFAULT '';
	DECLARE V_VER_INSTALLED_OZN VARCHAR(50) DEFAULT '';
	DECLARE V_VER_INSTALLED_CHD VARCHAR(50) DEFAULT '';
	DECLARE V_VER_INSTALLED_AGT VARCHAR(50) DEFAULT '';
	DECLARE V_VER_UPDATE_ZCR    VARCHAR(50) DEFAULT '';
	DECLARE V_VER_UPDATE_OZN    VARCHAR(50) DEFAULT '';
	DECLARE V_VER_UPDATE_CHD    VARCHAR(50) DEFAULT '';
	DECLARE V_VER_UPDATE_AGT    VARCHAR(50) DEFAULT '';
	DECLARE V_COMP_ZCR          INT(11)     DEFAULT 0;
	DECLARE V_COMP_OZN          INT(11)     DEFAULT 0;
	DECLARE V_COMP_CHD          INT(11)     DEFAULT 0;
	DECLARE V_COMP_AGT          INT(11)     DEFAULT 0;
	
	-- 예외처리 변수 선언
	DECLARE V_PROC_NAME      VARCHAR(256) DEFAULT 'PROC_CHECK_PRODUCT_UPDATE';
	DECLARE V_SQL_STATE      VARCHAR(20) DEFAULT '';
	DECLARE V_INT_ERROR_NO   INT(11) DEFAULT 0;
	DECLARE V_PROC_COMMENT   VARCHAR(2048) DEFAULT '';
	DECLARE V_TEXT_ERROR_MSG TEXT;
	
	-- 예외처리
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
	BEGIN
		GET DIAGNOSTICS CONDITION 1 V_SQL_STATE = RETURNED_SQLSTATE
		, V_INT_ERROR_NO = MYSQL_ERRNO
		, V_TEXT_ERROR_MSG = MESSAGE_TEXT;
		SET V_PROC_COMMENT = CONCAT('AGENT_ID:', IN_AGENT_ID);
		INSERT T_SYSTEM_PROC_ERROR (C_ERROR_TIME, C_PROC_NAME, C_SQL_STATE, C_ERROR_NO, C_COMMENT, C_ERROR_MSG)
		VALUES (NOW(), V_PROC_NAME, V_SQL_STATE, V_INT_ERROR_NO, V_PROC_COMMENT, V_TEXT_ERROR_MSG);
		-- RESIGNAL;
	END;

	-- 외부변수 초기화
	SET OUT_ZONECENTRAL_UPDATE = 'NO';
	SET OUT_ORIZON_UPDATE      = 'NO';
	SET OUT_CRYHOD_UPDATE      = 'NO';
	SET OUT_AGENT_UPDATE       = 'NO';

	-- 정보 추출
	SELECT COUNT(*), IFNULL(C_PROCESSOR_ARCHITECTURE, 9) 
	INTO V_CNT_SYSTEM, V_PROCESSOR_ARCH 
	FROM T_SYSTEM_MASTER WHERE C_AGENT_ID = IN_AGENT_ID;
	
	SELECT 
		ZCR.CNT, ZCR.VER, 
		OZN.CNT, OZN.VER, 
		CHD.CNT, CHD.VER, 
		AGT.CNT, AGT.VER
	INTO 
		V_CNT_INSTALLED_ZCR, V_VER_INSTALLED_ZCR, 
		V_CNT_INSTALLED_OZN, V_VER_INSTALLED_OZN, 
		V_CNT_INSTALLED_CHD, V_VER_INSTALLED_CHD,
		V_CNT_INSTALLED_AGT, V_VER_INSTALLED_AGT
	FROM 
		(SELECT COUNT(*) AS CNT, C_PRODUCT_VERSION AS VER FROM T_PRIMX_SETUP_INFO WHERE C_AGENT_ID = IN_AGENT_ID AND C_PRODUCT_TYPE = 1) AS ZCR, 
		(SELECT COUNT(*) AS CNT, C_PRODUCT_VERSION AS VER FROM T_PRIMX_SETUP_INFO WHERE C_AGENT_ID = IN_AGENT_ID AND C_PRODUCT_TYPE = 2) AS OZN,
		(SELECT COUNT(*) AS CNT, C_PRODUCT_VERSION AS VER FROM T_PRIMX_SETUP_INFO WHERE C_AGENT_ID = IN_AGENT_ID AND C_PRODUCT_TYPE = 3) AS CHD,
		(SELECT COUNT(*) AS CNT, C_AGENT_VERSION   AS VER FROM T_SYSTEM_MASTER    WHERE C_AGENT_ID = IN_AGENT_ID)                        AS AGT;
	
	SELECT 
		ZCR.CNT, ZCR.VER, 
		OZN.CNT, OZN.VER, 
		CHD.CNT, CHD.VER, 
		AGT.CNT, AGT.VER
	INTO
		V_CNT_UPDATE_ZCR, V_VER_UPDATE_ZCR, 
		V_CNT_UPDATE_OZN, V_VER_UPDATE_OZN, 
		V_CNT_UPDATE_CHD, V_VER_UPDATE_CHD, 
		V_CNT_UPDATE_AGT, V_VER_UPDATE_AGT
	FROM 
		(SELECT COUNT(*) AS CNT, C_PRODUCT_VERSION AS VER FROM T_UPDATE_INFO WHERE C_PRODUCT_TYPE = 1 AND C_PROCESSOR_ARCHITECTURE = V_PROCESSOR_ARCH) AS ZCR, 
		(SELECT COUNT(*) AS CNT, C_PRODUCT_VERSION AS VER FROM T_UPDATE_INFO WHERE C_PRODUCT_TYPE = 2 AND C_PROCESSOR_ARCHITECTURE = V_PROCESSOR_ARCH) AS OZN,
		(SELECT COUNT(*) AS CNT, C_PRODUCT_VERSION AS VER FROM T_UPDATE_INFO WHERE C_PRODUCT_TYPE = 3 AND C_PROCESSOR_ARCHITECTURE = V_PROCESSOR_ARCH) AS CHD,
		(SELECT COUNT(*) AS CNT, C_PRODUCT_VERSION AS VER FROM T_UPDATE_INFO WHERE C_PRODUCT_TYPE = 4 AND C_PROCESSOR_ARCHITECTURE = V_PROCESSOR_ARCH) AS AGT;

	IF V_CNT_UPDATE_ZCR > 0 THEN
		IF V_CNT_INSTALLED_ZCR = 0 THEN
			SET OUT_ZONECENTRAL_UPDATE = 'YES';
		ELSE
			IF LENGTH(V_VER_UPDATE_ZCR) > 0 AND LENGTH(V_VER_INSTALLED_ZCR) > 0 THEN
				SET V_COMP_ZCR = `PRIMXDB`.`FUNC_COMPARE_VERSION`(V_VER_UPDATE_ZCR, V_VER_INSTALLED_ZCR);
				IF V_COMP_ZCR = 1 THEN 
					SET OUT_ZONECENTRAL_UPDATE = 'YES';
				END IF;
			END IF;
		END IF;
	END IF;
	
	IF V_CNT_UPDATE_OZN > 0 THEN
		IF V_CNT_INSTALLED_OZN = 0 THEN
			SET OUT_ORIZON_UPDATE = 'YES';
		ELSE
			IF LENGTH(V_VER_UPDATE_OZN) > 0 AND LENGTH(V_VER_INSTALLED_OZN) > 0 THEN
				SET V_COMP_OZN = `PRIMXDB`.`FUNC_COMPARE_VERSION`(V_VER_UPDATE_OZN, V_VER_INSTALLED_OZN);
				IF V_COMP_OZN = 1 THEN 
					SET OUT_ORIZON_UPDATE = 'YES';
				END IF;
			END IF;
		END IF;
	END IF;
	
	IF V_CNT_UPDATE_CHD > 0 THEN
		IF V_CNT_INSTALLED_CHD = 0 THEN
			SET OUT_CRYHOD_UPDATE = 'YES';
		ELSE
			IF LENGTH(V_VER_UPDATE_CHD) > 0 AND LENGTH(V_VER_INSTALLED_CHD) > 0 THEN
				SET V_COMP_CHD = `PRIMXDB`.`FUNC_COMPARE_VERSION`(V_VER_UPDATE_CHD, V_VER_INSTALLED_CHD);
				IF V_COMP_CHD = 1 THEN 
					SET OUT_CRYHOD_UPDATE = 'YES';
				END IF;
			END IF;
		END IF;
	END IF;
	
	IF V_CNT_UPDATE_AGT > 0 THEN
		IF V_CNT_INSTALLED_AGT = 0 THEN
			SET OUT_AGENT_UPDATE = 'YES';
		ELSE
			IF LENGTH(V_VER_UPDATE_AGT) > 0 AND LENGTH(V_VER_INSTALLED_AGT) > 0 THEN
				SET V_COMP_AGT = `PRIMXDB`.`FUNC_COMPARE_VERSION`(V_VER_UPDATE_AGT, V_VER_INSTALLED_AGT);
				IF V_COMP_AGT = 1 THEN 
					SET OUT_AGENT_UPDATE = 'YES';
				END IF;
			END IF;
		END IF;
	END IF;
END |
DELIMITER ;

-- Keep Alive Response 시 필요한 환경변수 추출 프로시저
DELIMITER |
DROP PROCEDURE IF EXISTS `PRIMXDB`.`PROC_GET_SERVER_CONFIGS_FOR_KEEPALIVE` |
CREATE PROCEDURE `PRIMXDB`.`PROC_GET_SERVER_CONFIGS_FOR_KEEPALIVE`
(
	OUT OUT_KEEPALIVE_CYCLE        INT(11),     -- KeepAlive 주기 (AgentKeepAliveMinuteInterval. Default 120분. -> 초단위 환산 숫자로 에이전트에 전달)
	OUT OUT_LOG_CYCLE              INT(11),     -- 로그 전송 주기 (AgentProductLogSendMinuteInterval. Default 120분 -> 초단위 환산 숫자로 에이전트에 전달)
	OUT OUT_EXPORT_SHELF_LIFE      INT(11),     -- 반출 파일 실행 가능 일 수 (FileExpireDayLimit. Default 3일. -> 일단위 그대로 에이전트에 전달)
	OUT OUT_AUTH_SERVER_ADDR       VARCHAR(50), -- 반출 파일 인증 서버 주소 (AuthServerAddress)
	OUT OUT_AUTH_SERVER_PORT       INT(11),     -- 반출 파일 인증 서버 포트 (AuthServerAddress)
	OUT OUT_WEBMANAGER_CONSOLE_URL VARCHAR(200) -- Web Manager Console URL 
)
BEGIN
	/*
	COMMENT: Keep Alive Response 시 필요한 환경변수 추출 프로시저
	PARAMETERS:
		OUT_KEEPALIVE_CYCLE      - KeepAlive 주기 (AgentKeepAliveMinuteInterval. Default 120분. -> 초단위 환산 숫자로 에이전트에 전달)
		OUT_LOG_CYCLE            - 로그 전송 주기 (AgentProductLogSendMinuteInterval. Default 120분 -> 초단위 환산 숫자로 에이전트에 전달)
		OUT_EXPORT_SHELF_LIFE    - 반출 파일 실행 가능 일 수 (FileExpireDayLimit. Default 3일. -> 일단위 그대로 에이전트에 전달)
		OUT_AUTH_SERVER_ADDR     - 반출 파일 인증 서버 주소 (AuthServerAddress)
		OUT OUT_AUTH_SERVER_PORT - 반출 파일 인증 서버 포트 (AuthServerAddress)
	HISTORY:
		2023-07-15 생성
	*/

	-- 환경 변수 추출을 위한 커서 관련 변수 선언
	DECLARE V_CUR_KEY       VARCHAR(50)  DEFAULT '';
	DECLARE V_CUR_VALUE     VARCHAR(50)  DEFAULT '';
	DECLARE V_LOOP_END      BOOLEAN      DEFAULT FALSE;
	DECLARE V_BUF_AUTH_ADDR VARCHAR(100) DEFAULT '';
	DECLARE V_POS_COLON     INT(11)      DEFAULT 0;
	DECLARE V_LEN_ADDR      INT(11)      DEFAULT 0;
	DECLARE V_PORT_STR      VARCHAR(10)  DEFAULT '50103';

	-- 예외처리 변수 선언
	DECLARE V_PROC_NAME      VARCHAR(256) DEFAULT 'PROC_GET_SERVER_CONFIGS_FOR_KEEPALIVE';
	DECLARE V_SQL_STATE      VARCHAR(20) DEFAULT '';
	DECLARE V_INT_ERROR_NO   INT(11) DEFAULT 0;
	DECLARE V_PROC_COMMENT   VARCHAR(2048) DEFAULT '';
	DECLARE V_TEXT_ERROR_MSG TEXT;
	
	-- 환경 변수 추출을 위한 커서 선언
	DECLARE CURSOR_FOR_CONFIG CURSOR FOR 
	SELECT C_KEY_NAME, C_VALUE_DATA FROM T_SERVER_CONFIG;
	DECLARE CONTINUE HANDLER FOR NOT FOUND SET V_LOOP_END = TRUE; 
	
	-- 예외처리
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
	BEGIN
		GET DIAGNOSTICS CONDITION 1 V_SQL_STATE = RETURNED_SQLSTATE
		, V_INT_ERROR_NO = MYSQL_ERRNO
		, V_TEXT_ERROR_MSG = MESSAGE_TEXT;
		SET V_PROC_COMMENT = '';
		INSERT T_SYSTEM_PROC_ERROR (C_ERROR_TIME, C_PROC_NAME, C_SQL_STATE, C_ERROR_NO, C_COMMENT, C_ERROR_MSG)
		VALUES (NOW(), V_PROC_NAME, V_SQL_STATE, V_INT_ERROR_NO, V_PROC_COMMENT, V_TEXT_ERROR_MSG);
		-- RESIGNAL;
	END;
	
	-- 기본값 설정
	SET OUT_KEEPALIVE_CYCLE = 120 * 60;
	SET OUT_LOG_CYCLE = 120 * 60;
	SET OUT_EXPORT_SHELF_LIFE = 30;
	SET OUT_AUTH_SERVER_ADDR = '';
	SET OUT_AUTH_SERVER_PORT = 0;
	
	-- 환경변수 추출
	OPEN CURSOR_FOR_CONFIG;
	LOOP_READ: LOOP
		FETCH CURSOR_FOR_CONFIG INTO V_CUR_KEY, V_CUR_VALUE;
		IF V_LOOP_END THEN
			LEAVE LOOP_READ;
		END IF;
		CASE 			
			WHEN V_CUR_KEY = 'AgentKeepAliveMinuteInterval'      THEN 
				SET V_CUR_VALUE = REGEXP_REPLACE(IFNULL(V_CUR_VALUE, '120'), '[a-z]|[\]\[\, ]', '');
				IF LENGTH(V_CUR_VALUE) = 0 THEN SET V_CUR_VALUE = '120'; END IF;
				SET OUT_KEEPALIVE_CYCLE = (CONVERT(V_CUR_VALUE, DECIMAL) * 60);
			WHEN V_CUR_KEY = 'AgentProductLogSendMinuteInterval' THEN 
				SET V_CUR_VALUE = REGEXP_REPLACE(IFNULL(V_CUR_VALUE, '120'), '[a-z]|[\]\[\, ]', '');
				IF LENGTH(V_CUR_VALUE) = 0 THEN SET V_CUR_VALUE = '120'; END IF;
				SET OUT_LOG_CYCLE = (CONVERT(V_CUR_VALUE, DECIMAL) * 60);
			WHEN V_CUR_KEY = 'FileExpireDayLimit'                THEN 
				SET V_CUR_VALUE = REGEXP_REPLACE(IFNULL(V_CUR_VALUE, '30'), '[a-z]|[\]\[\, ]', '');
				IF LENGTH(V_CUR_VALUE) = 0 THEN SET V_CUR_VALUE = '30'; END IF;
				SET OUT_EXPORT_SHELF_LIFE = CONVERT(V_CUR_VALUE, DECIMAL);
			WHEN V_CUR_KEY = 'AgentRegisterPageURL'              THEN 
				SET OUT_WEBMANAGER_CONSOLE_URL = IFNULL(V_CUR_VALUE, '');
			WHEN V_CUR_KEY = 'AuthServerAddress'                 THEN 
			-- 인증서버 주소 / 포트 추출
				SET V_BUF_AUTH_ADDR = IFNULL(V_CUR_VALUE, '');
				SET V_POS_COLON = INSTR(V_BUF_AUTH_ADDR, ':');
				SET V_LEN_ADDR = LENGTH(V_BUF_AUTH_ADDR);
				IF V_POS_COLON > 0 AND V_LEN_ADDR > 0 THEN
					SET OUT_AUTH_SERVER_ADDR = SUBSTRING(V_BUF_AUTH_ADDR, 1, (V_POS_COLON - 1));
					SET V_PORT_STR = SUBSTRING(V_BUF_AUTH_ADDR, (V_POS_COLON + 1), (V_LEN_ADDR - V_POS_COLON));
					SET V_PORT_STR = REGEXP_REPLACE(V_PORT_STR, '[a-z]|[\]\[\, ]', '');
					IF LENGTH(V_PORT_STR) = 0 THEN SET V_PORT_STR = '0'; END IF;
					SET OUT_AUTH_SERVER_PORT = CONVERT(V_PORT_STR, DECIMAL);
				ELSE 
				    SET OUT_AUTH_SERVER_ADDR = '106.247.226.210';
					SET OUT_AUTH_SERVER_PORT = '50103';
				END IF;
			ELSE 
				ITERATE LOOP_READ;
		END CASE;		
	END LOOP LOOP_READ;
	CLOSE CURSOR_FOR_CONFIG;
END |
DELIMITER ; 

-- Keep Alive Response 시 응답 내용 추출 프로시저
DELIMITER |
DROP PROCEDURE IF EXISTS `PRIMXDB`.`PROC_GET_KEEPALIVE_RESPONSE` |
CREATE PROCEDURE `PRIMXDB`.`PROC_GET_KEEPALIVE_RESPONSE`
(
	IN IN_AGENT_ID                         VARCHAR(50),  -- 1. Agent ID
	IN IN_USER_ID                          VARCHAR(50),  -- 2. User ID
	IN IN_COMPANY_ID                       VARCHAR(50),  -- 3. 회사 ID
	IN IN_AGENT_POLICY_STAMP               VARCHAR(50),  -- 4. Agent에 저정된 정책 시각 
	IN IN_SHARE_PC                         VARCHAR(10),  -- 5. 현재 공용 PC 여부 상태 (YES/NO)
	IN IN_LAST_POLICY_DATE                 VARCHAR(50),  -- NEW. CLIENT POLICY 최종 반영 시간
	IN IN_LAST_POLICY_ID                   VARCHAR(50),  -- NEW. POL-2023050500001 
	
	OUT OUT_EXPAND_COMMAND                 VARCHAR(10),  -- 6. 확장명령 (NONE:없음/EX001:등록된 Agent ID가 없는 경우 시스템 정보를 등록하도록 명령/EX002:등록된 사용자ID가 존재하지 않는 경우 사용자ID 재등록 명령/EX003:PRIMX Agent 삭제 명령/EX004:사용자PC로 변경/EX005:공용PC로 변경)
	OUT OUT_KEEPALIVE_CYCLE                INT(11),      -- 7. KeepAlive 주기 (AgentKeepAliveMinuteInterval. Default 120분. -> 초단위 환산 숫자로 에이전트에 전달)
	OUT OUT_LOG_CYCLE                      INT(11),      -- 8. 로그 전송 주기 (AgentProductLogSendMinuteInterval. Default 120분 -> 초단위 환산 숫자로 에이전트에 전달)
	OUT OUT_AGENT_UPDATE                   VARCHAR(10),  -- 9. PRIMX Agent 업데이트 여부 (YES/NO)
	OUT OUT_ZONECENTRAL_UPDATE             VARCHAR(10),  -- 10. PRIMX ZONECENTRAL 업데이트 여부 (YES/NO)
	OUT OUT_ORIZON_UPDATE                  VARCHAR(10),  -- 11. PRIMX ORIZON 업데이트 여부 (YES/NO)
	OUT OUT_CRYHOD_UPDATE                  VARCHAR(10),  -- 12. PRIMX CRYHOD 업데이트 여부 (YES/NO)
	OUT OUT_SERVER_TIME                    VARCHAR(50),  -- 13. 현재 서버 시각 (YYYY-mm-dd HH:MM:SS)
	OUT OUT_EXPORT_SHELF_LIFE              INT(11),      -- 14. 반출 파일 실행 가능 일 수 (FileExpireDayLimit. Default 3일. -> 일단위 그대로 에이전트에 전달)
	OUT OUT_COMPANY_ID                     VARCHAR(50),  -- 15. 회사 ID
	OUT OUT_AUTH_SERVER_ADDR               VARCHAR(50),  -- 16. 반출 파일 인증 서버 주소
	OUT OUT_AUTH_SERVER_PORT               INT(11),      -- 17. 반출 파일 인증 서버 포트
	OUT OUT_PRODUCT_MENT_ENABLE            VARCHAR(10),  -- 18. PRIMX 제품의 빠른 메뉴 표시 여부 (YES/NO)
	OUT OUT_WEBMANAGER_CONSOLE_MENU_ENABLE VARCHAR(10),  -- 19. 사용자 콘솔 메뉴 표시 여부 (YES/NO)
	OUT OUT_WEBMANAGER_CONSOLE_URL         VARCHAR(200), -- 20. Web Manager Console URL
	OUT OUT_EXPORT_FILE_MENU_ENABLE        VARCHAR(10),  -- 21. 이동파일 생성 메뉴 표시 여부 (YES/NO) 
	OUT OUT_AUTH_WEB_MANAGER               VARCHAR(10),  -- 22. 신청 및 결재 권한 여부 (YES/NO) 
	OUT OUT_AUTH_EXPORT_FILE               VARCHAR(10),  -- 23. 반출 파일 생성 권한 여부 (YES/NO) 
	OUT OUT_AUTH_NOT_ENCRYPTION_PATH       VARCHAR(10),  -- 24. 비암호화 영역 경로 사용 여부 (YES/NO)
	OUT OUT_AUTH_NOT_ENCRYPT_PATH_LIST     TEXT,          -- 25. 비암호화 영역 경로 목록 (경로 배열을 문자열로 붙여서 리턴. 구분자는 '|')
	OUT OUT_POLICY_UPDATE 				   VARCHAR(10),
	OUT OUT_POLICY_FILE_NAME               VARCHAR(100),
	OUT OUT_LAST_POLICY_DATE               VARCHAR(50),
	OUT OUT_LAST_POLICY_ID                 VARCHAR(50),
	OUT OUT_ZTRY_MENU_ENABLE			   VARCHAR(10), 
	OUT OUT_ZOPEN_AREA					   VARCHAR(10),
	OUT	OUT_ZPERSONAL_ACCESS			   VARCHAR(10),
	OUT OUT_ZOPTION_DISPLAY				   VARCHAR(10),
	OUT OUT_ZSAFETY_ZONE			       VARCHAR(10),
	OUT OUT_ZAREA_CLOSE					   VARCHAR(10), 
	OUT OUT_ZENC						   VARCHAR(10)
)
BEGIN
	/*
	COMMENT: Keep Alive Response 시 응답 내용 추출 프로시저
	PARAMETERS:
		IN_AGENT_ID                        - Agent ID
		IN_USER_ID                         - User ID
		IN_COMPANY_ID                      - 회사 ID
		IN_AGENT_POLICY_STAMP              - Agent에 저정된 정책 시각 
		IN_SHARE_PC                        - 현재 공용 PC 여부 상태 (YES/NO)
		OUT_EXPAND_COMMAND                 - 확장명령 (NONE:없음/EX001:등록된 Agent ID가 없는 경우 시스템 정보를 등록하도록 명령/EX002:등록된 사용자ID가 존재하지 않는 경우 사용자ID 재등록 명령/EX003:PRIMX Agent 삭제 명령/EX004:사용자PC로 변경/EX005:공용PC로 변경)
		OUT_KEEPALIVE_CYCLE                - KeepAlive 주기 (AgentKeepAliveMinuteInterval. Default 120분. -> 초단위 환산 숫자로 에이전트에 전달)
		OUT_LOG_CYCLE                      - 로그 전송 주기 (AgentProductLogSendMinuteInterval. Default 120분 -> 초단위 환산 숫자로 에이전트에 전달)
		OUT_AGENT_UPDATE                   - PRIMX Agent 업데이트 여부 (YES/NO)
		OUT_ZONECENTRAL_UPDATE             - PRIMX ZONECENTRAL 업데이트 여부 (YES/NO)
		OUT_ORIZON_UPDATE                  - PRIMX ORIZON 업데이트 여부 (YES/NO)
		OUT_CRYHOD_UPDATE                  - PRIMX CRYHOD 업데이트 여부 (YES/NO)
		OUT_SERVER_TIME                    - 현재 서버 시각 (YYYY-mm-dd HH:MM:SS)
		OUT_EXPORT_SHELF_LIFE              - 반출 파일 실행 가능 일 수 (FileExpireDayLimit. Default 3일. -> 일단위 그대로 에이전트에 전달)
		OUT_COMPANY_ID                     - 회사 ID
		OUT_AUTH_SERVER_ADDR               - 반출 파일 인증 서버 주소
		OUT_AUTH_SERVER_PORT               - 반출 파일 인증 서버 포트
		OUT_PRODUCT_MENT_ENABLE            - PRIMX 제품의 빠른 메뉴 표시 여부 (YES/NO)
		OUT_WEBMANAGER_CONSOLE_MENU_ENABLE - 사용자 콘솔 메뉴 표시 여부 (YES/NO)
		OUT_WEBMANAGER_CONSOLE_URL         - Web Manager Console URL
		OUT_EXPORT_FILE_MENU_ENABLE        - 이동파일 생성 메뉴 표시 여부 (YES/NO) 
		OUT_AUTH_WEB_MANAGER               - 신청 및 결재 권한 여부 (YES/NO) 
		OUT_AUTH_EXPORT_FILE               - 반출 파일 생성 권한 여부 (YES/NO) 
		OUT_AUTH_NOT_ENCRYPTION_PATH       - 비암호화 영역 경로 사용 여부 (YES/NO)
		OUT_AUTH_NOT_ENCRYPT_PATH_LIST     - 비암호화 영역 경로 목록 (경로 배열을 문자열로 붙여서 리턴. 구분자는 '|')
	HISTORY:
		2023-07-15 생성
	*/

	-- 기본 변수 선언
	DECLARE V_CNT_USERID                      INT(11) DEFAULT 0;
	DECLARE V_CNT_SYSTEM_AGENTID              INT(11) DEFAULT 0;
	DECLARE V_CNT_POLICY                      INT(11) DEFAULT 0;
	DECLARE V_UNINSTALL_YN                    CHAR(1) DEFAULT 'N';
	DECLARE V_COMMON_SYSTEM_YN                CHAR(1) DEFAULT 'N';
	DECLARE V_SYSTEM_AGENT_VERSION            VARCHAR(50) DEFAULT '';
	DECLARE V_AUTH_EXPORT_FILE                INT(11) DEFAULT 0;
	DECLARE V_AUTH_WEB_MANAGER                INT(11) DEFAULT 0; 
	DECLARE V_AUTH_NOT_ENCRYPTION_PATH        INT(11) DEFAULT 0;
	DECLARE V_PRODUCT_MENU_ENABLE             INT(11) DEFAULT 0;
	DECLARE V_WEB_MANAGER_CONSOLE_MENU_ENABLE INT(11) DEFAULT 0;
	DECLARE V_EXPORT_FILE_MENU_ENABLE         INT(11) DEFAULT 0;
	DECLARE V_AGENT_POLICY_CODE               VARCHAR(50) DEFAULT '';
	DECLARE V_COMPANY_ID                      VARCHAR(50) DEFAULT '00000001';
	DECLARE V_COMPUTER_NAME                   VARCHAR(260) DEFAULT '';
	DECLARE V_CURRENT_POLICY_CODE    		  VARCHAR(50) DEFAULT '';
	DECLARE V_DB_LAST_POLICY_DATE    		  VARCHAR(50) DEFAULT '';
	DECLARE V_DB_LAST_POLICY_ID      		  VARCHAR(50) DEFAULT '';
	DECLARE V_POLICY_MODIFIED_DATE   		  VARCHAR(50) DEFAULT '';
    DECLARE V_ZTRY_MENU_ENABLE		 		  INT DEFAULT 0;
   	DECLARE V_ZOPEN_AREA      		 		  INT DEFAULT 0;
  	DECLARE V_ZPERSONAL_ACCESS		 		  INT DEFAULT 0;
 	DECLARE V_ZOPTION_DISPLAY		 		  INT DEFAULT 0;
	DECLARE V_ZSAFETY_ZONE   		 		  INT DEFAULT 0;
	DECLARE V_ZAREA_CLOSE			 		  INT DEFAULT 0;
	DECLARE V_ZENC					 		  INT DEFAULT 0;

	-- 예외처리 변수 선언
	DECLARE V_PROC_NAME      VARCHAR(256) DEFAULT 'PROC_GET_KEEPALIVE_RESPONSE';
	DECLARE V_SQL_STATE      VARCHAR(20) DEFAULT '';
	DECLARE V_INT_ERROR_NO   INT(11) DEFAULT 0;
	DECLARE V_PROC_COMMENT   VARCHAR(2048) DEFAULT '';
	DECLARE V_TEXT_ERROR_MSG TEXT;
	
	-- 예외처리
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
	BEGIN
		GET DIAGNOSTICS CONDITION 1 V_SQL_STATE = RETURNED_SQLSTATE
		, V_INT_ERROR_NO = MYSQL_ERRNO
		, V_TEXT_ERROR_MSG = MESSAGE_TEXT;
		SET V_PROC_COMMENT = CONCAT('AGENT_ID:', IN_AGENT_ID, '/USER_ID:', IN_USER_ID, '/COMPANY_ID:', IN_COMPANY_ID, '/AGENT_POLICY_STAMP:', IN_AGENT_POLICY_STAMP);
		INSERT T_SYSTEM_PROC_ERROR (C_ERROR_TIME, C_PROC_NAME, C_SQL_STATE, C_ERROR_NO, C_COMMENT, C_ERROR_MSG)
		VALUES (NOW(), V_PROC_NAME, V_SQL_STATE, V_INT_ERROR_NO, V_PROC_COMMENT, V_TEXT_ERROR_MSG);
		-- RESIGNAL;
	END;
  
	-- 기본값 설정
	SET OUT_EXPAND_COMMAND                 = 'NONE';
	SET OUT_KEEPALIVE_CYCLE                = (120 * 60);
	SET OUT_LOG_CYCLE                      = (120 * 60);
	SET OUT_AGENT_UPDATE                   = 'NO';
	SET OUT_ZONECENTRAL_UPDATE             = 'NO';
	SET OUT_ORIZON_UPDATE                  = 'NO';
	SET OUT_CRYHOD_UPDATE                  = 'NO';
	SET OUT_SERVER_TIME                    = DATE_FORMAT(UTC_TIMESTAMP(), '%Y-%m-%d %H:%i:%S');
	SET OUT_EXPORT_SHELF_LIFE              = 3;
	SET OUT_COMPANY_ID                     = '00000001';
	SET OUT_AUTH_SERVER_ADDR               = '';
	SET OUT_AUTH_SERVER_PORT               = 50103;
	SET OUT_PRODUCT_MENT_ENABLE            = 'NO';
	SET OUT_WEBMANAGER_CONSOLE_MENU_ENABLE = 'NO';
	SET OUT_WEBMANAGER_CONSOLE_URL         = '';
	SET OUT_EXPORT_FILE_MENU_ENABLE        = 'NO';
	SET OUT_AUTH_WEB_MANAGER               = 'NO';
	SET OUT_AUTH_EXPORT_FILE               = 'NO';
	SET OUT_AUTH_NOT_ENCRYPTION_PATH       = 'NO';
	SET OUT_AUTH_NOT_ENCRYPT_PATH_LIST     = '';
    SET OUT_POLICY_UPDATE = 'NO';
	SET OUT_POLICY_FILE_NAME = '';
	SET OUT_LAST_POLICY_DATE = '';
	SET OUT_LAST_POLICY_ID = '';
    SET OUT_ZTRY_MENU_ENABLE = 'NO';
    SET OUT_ZOPEN_AREA = 'NO';
    SET OUT_ZPERSONAL_ACCESS = 'NO';
    SET OUT_ZOPTION_DISPLAY = 'NO';
    SET OUT_ZSAFETY_ZONE = 'NO';
    SET OUT_ZAREA_CLOSE = 'NO';
    SET OUT_ZENC = 'NO';
	
	-- 시스템 및 사용자 정보 추출
	SELECT COUNT(*), IFNULL(C_AGENT_VERSION, ''), IFNULL(C_UNINSTALL_YN, 'N'), IFNULL(C_COMMON_SYSTEM_YN, 'X'), IFNULL(C_COMPUTER_NAME, '') 
	INTO V_CNT_SYSTEM_AGENTID, V_SYSTEM_AGENT_VERSION, V_UNINSTALL_YN, V_COMMON_SYSTEM_YN, V_COMPUTER_NAME 
	FROM T_SYSTEM_MASTER
	WHERE C_AGENT_ID = IN_AGENT_ID;
	SELECT COUNT(*), C_COMPANY_ID INTO V_CNT_USERID, V_COMPANY_ID FROM T_USER_SYSTEM_MATCH WHERE C_AGENT_ID = IN_AGENT_ID;
	
	-- 확장 명령 처리
	IF     V_CNT_SYSTEM_AGENTID  = 0   THEN SET OUT_EXPAND_COMMAND = 'EX001';
	ELSEIF V_UNINSTALL_YN        = 'Y' THEN SET OUT_EXPAND_COMMAND = 'EX003';
	ELSEIF V_CNT_USERID          = 0 AND V_COMMON_SYSTEM_YN = 'N'  THEN SET OUT_EXPAND_COMMAND = 'EX002';
	ELSE
		IF     IN_SHARE_PC = 'YES' AND V_COMMON_SYSTEM_YN = 'N' THEN SET OUT_EXPAND_COMMAND = 'EX004';
		ELSEIF IN_SHARE_PC = 'NO'  AND V_COMMON_SYSTEM_YN = 'Y' THEN SET OUT_EXPAND_COMMAND = 'EX005';
		END IF;
	END IF;
	
	-- 환경변수 추출
	CALL `PRIMXDB`.`PROC_GET_SERVER_CONFIGS_FOR_KEEPALIVE`(OUT_KEEPALIVE_CYCLE, OUT_LOG_CYCLE, OUT_EXPORT_SHELF_LIFE, OUT_AUTH_SERVER_ADDR, OUT_AUTH_SERVER_PORT, OUT_WEBMANAGER_CONSOLE_URL);
	
	-- Company ID가 공백인 경우, 기본 Company ID로 대체
	IF LENGTH(V_COMPANY_ID) = 0 THEN
		SET V_COMPANY_ID = FUNC_GET_SERVER_CONFIG('CompanyID');
		IF LENGTH(V_COMPANY_ID) = 0 THEN
			SET V_COMPANY_ID = '00000001';
		END IF; 
	END IF;
	
	IF OUT_EXPAND_COMMAND = 'NONE' THEN
		SET OUT_COMPANY_ID = V_COMPANY_ID;
	
		-- 제품별 버전정보 추출 및 업데이트 여부 확인
		CALL `PRIMXDB`.`PROC_CHECK_PRODUCT_UPDATE`(IN_AGENT_ID, OUT_AGENT_UPDATE, OUT_ZONECENTRAL_UPDATE, OUT_ORIZON_UPDATE, OUT_CRYHOD_UPDATE);
		
		-- Agent에 적용된 정책 추출
		SELECT COUNT(*), 
		       IFNULL(A.C_AGENT_POLICY_CODE, ''), 
		       IFNULL(A.C_EXPORT_FILE_AUTH, 0), 
		       IFNULL(A.C_WEB_MANAGER_AUTH, 0), 
		       IFNULL(A.C_NOT_ENCRYPTION_PATH_AUTH, 0), 
		       IFNULL(A.C_PRODUCT_MENU_ENABLE, 0), 
		       IFNULL(A.C_WEB_MANAGER_CONSOLE_MENU_ENABLE, 0), 
		       IFNULL(A.C_EXPORT_FILE_MENU_ENABLE, 0),
		       IFNULL(A.C_ZTRY_MENU_ENABLE, 0),
		       IFNULL(A.C_ZOPEN_AREA, 0),
		       IFNULL(A.C_ZPERSONAL_ACCESS, 0),
		       IFNULL(A.C_ZOPTION_DISPLAY, 0),
		       IFNULL(A.C_ZSAFETY_ZONE, 0),
		       IFNULL(A.C_ZAREA_CLOSE, 0),
		       IFNULL(A.C_ZENC, 0)
		INTO 
		     V_CNT_POLICY, V_AGENT_POLICY_CODE,
		     V_AUTH_EXPORT_FILE, V_AUTH_WEB_MANAGER,
		     V_AUTH_NOT_ENCRYPTION_PATH, V_PRODUCT_MENU_ENABLE,
		     V_WEB_MANAGER_CONSOLE_MENU_ENABLE, V_EXPORT_FILE_MENU_ENABLE,
		     V_ZTRY_MENU_ENABLE, V_ZOPEN_AREA, V_ZPERSONAL_ACCESS,
		     V_ZOPTION_DISPLAY, V_ZSAFETY_ZONE, V_ZAREA_CLOSE, V_ZENC
		FROM T_AGENT_POLICY_MASTER A
		JOIN T_AGENT_POLICY_SYSTEM_MATCH B 
		  ON A.C_AGENT_POLICY_CODE = B.C_AGENT_POLICY_CODE
		WHERE B.C_AGENT_ID = IN_AGENT_ID;
		
		-- 권한값 매핑
		IF V_CNT_POLICY > 0 THEN
		    IF V_AUTH_EXPORT_FILE = 1 THEN SET OUT_AUTH_EXPORT_FILE = 'YES'; END IF;
		    IF V_AUTH_WEB_MANAGER = 1 THEN SET OUT_AUTH_WEB_MANAGER = 'YES'; END IF;
		    IF V_AUTH_NOT_ENCRYPTION_PATH = 1 THEN SET OUT_AUTH_NOT_ENCRYPTION_PATH = 'YES'; END IF;
		    IF V_PRODUCT_MENU_ENABLE = 1 THEN SET OUT_PRODUCT_MENT_ENABLE = 'YES'; END IF;
		    IF V_WEB_MANAGER_CONSOLE_MENU_ENABLE = 1 THEN SET OUT_WEBMANAGER_CONSOLE_MENU_ENABLE = 'YES'; END IF;
		    IF V_EXPORT_FILE_MENU_ENABLE = 1 THEN SET OUT_EXPORT_FILE_MENU_ENABLE = 'YES'; END IF;
		
		    IF V_ZTRY_MENU_ENABLE = 1 THEN SET OUT_ZTRY_MENU_ENABLE = 'YES'; END IF;
		    IF V_ZOPEN_AREA = 1 THEN SET OUT_ZOPEN_AREA = 'YES'; END IF;
		    IF V_ZPERSONAL_ACCESS = 1 THEN SET OUT_ZPERSONAL_ACCESS = 'YES'; END IF;
		    IF V_ZOPTION_DISPLAY = 1 THEN SET OUT_ZOPTION_DISPLAY = 'YES'; END IF;
		    IF V_ZSAFETY_ZONE = 1 THEN SET OUT_ZSAFETY_ZONE = 'YES'; END IF;
		    IF V_ZAREA_CLOSE = 1 THEN SET OUT_ZAREA_CLOSE = 'YES'; END IF;
		    IF V_ZENC = 1 THEN SET OUT_ZENC = 'YES'; END IF;
		END IF;
		
		-- 비암호화 경로
		 IF V_AUTH_NOT_ENCRYPTION_PATH = 1 THEN
			SELECT GROUP_CONCAT(C_NOT_ENCRYPT_PATH SEPARATOR '|') 
			INTO OUT_AUTH_NOT_ENCRYPT_PATH_LIST
			FROM T_NOT_ENCRYPTION_PATH_INFO
			WHERE C_AGENT_POLICY_CODE = V_AGENT_POLICY_CODE;
		 END IF;

		-- 정책 정보 조회 및 비교
		SELECT 
			IFNULL(C_LAST_POLICY_DATE, '') AS LAST_POLICY_DATE
		INTO 
			V_DB_LAST_POLICY_DATE
		FROM T_PRIMX_SETUP_INFO
		WHERE C_AGENT_ID = IN_AGENT_ID AND C_PRODUCT_TYPE = 1;

		SELECT policy_code 
		INTO V_CURRENT_POLICY_CODE
		FROM T_POLICY_SYSTEM_MAP
		WHERE C_AGENT_ID = IN_AGENT_ID
		ORDER BY modified_date ASC
		LIMIT 1;

		-- 정책 파일 이름
		IF LENGTH(V_CURRENT_POLICY_CODE) > 0 THEN
			SET OUT_POLICY_FILE_NAME = CONCAT(V_CURRENT_POLICY_CODE, '.xml');
		 	SET OUT_LAST_POLICY_ID   = V_CURRENT_POLICY_CODE;
		END IF;

		-- 변경 여부 비교 및 응답
		IF LENGTH(IN_LAST_POLICY_DATE) = 0 OR IN_LAST_POLICY_DATE != V_DB_LAST_POLICY_DATE THEN
			SET OUT_LAST_POLICY_DATE = V_DB_LAST_POLICY_DATE;
		END IF;
		
		SELECT DATE_FORMAT(P.modified_date, '%Y-%m-%d %H:%i:%s')
		INTO V_POLICY_MODIFIED_DATE
		FROM T_POLICY P
		WHERE P.policy_code = V_CURRENT_POLICY_CODE
		LIMIT 1;
		
	
		-- 정책 날짜가 없을 경우 Fallback
		IF OUT_LAST_POLICY_DATE IS NULL OR LENGTH(TRIM(OUT_LAST_POLICY_DATE)) = 0 THEN
	SELECT DATE_FORMAT(P.modified_date, '%Y-%m-%d %H:%i:%s')
		INTO OUT_LAST_POLICY_DATE
		FROM T_POLICY_SYSTEM_MAP M
		JOIN T_POLICY P ON M.policy_code = P.policy_code
		WHERE M.C_AGENT_ID = IN_AGENT_ID
		ORDER BY P.modified_date DESC
		LIMIT 1;
	END IF;
	END IF;

		-- 정책 변경 여부 판단
	IF IN_LAST_POLICY_DATE IS NULL OR IN_LAST_POLICY_DATE = '' THEN
	    SET OUT_POLICY_UPDATE = 'YES';
	ELSEIF IN_LAST_POLICY_DATE != V_POLICY_MODIFIED_DATE THEN
	    SET OUT_POLICY_UPDATE = 'YES';
	ELSE
	    SET OUT_POLICY_UPDATE = 'NO';
	END IF;

	-- 삭제 복구 처리
	IF OUT_EXPAND_COMMAND = 'EX003' THEN
		INSERT INTO T_MANAGE_LOG (C_LOG_TIME, C_LOG_CODE, C_LOG_TYPE, C_USER_ID, C_AGENT_ID, C_COMPUTER_NAME, C_RESULT_CODE)
		VALUES (UTC_TIMESTAMP(), 'S20003', 1, 'SYSTEM', IN_AGENT_ID, V_COMPUTER_NAME, 'SUCCESS');

		UPDATE T_SYSTEM_MASTER SET C_UNINSTALL_YN = 'N' WHERE C_AGENT_ID = IN_AGENT_ID;
	END IF;
END |
DELIMITER ; 

-- PRIMX 제품 업데이트 정보 추출
DELIMITER |
DROP PROCEDURE IF EXISTS `PRIMXDB`.`PROC_GET_PRODUCT_UPDATE_INFO` |
CREATE PROCEDURE `PRIMXDB`.`PROC_GET_PRODUCT_UPDATE_INFO`
(
	IN IN_AGENT_ID           VARCHAR(50),   -- Agent ID
	IN IN_PRODUCT_TYPE       INT(11),       -- 제품 종료 (1: PRIMX ZONECENTRAL, 2: PRIMX ORIZON, 3: PRIMX CRYHOD, 4: PRIMX Agent)
	IN IN_INSTALLED_VERSION  VARCHAR(50),   -- 에이전트에 설치된 제품 버전
	OUT OUT_NEED_PATCH_YN    VARCHAR(10),   -- 패치 필요 여부 (YES/NO)
	OUT OUT_PRODUCT_VERSION  VARCHAR(50),   -- 설치되어야 할 제품 버전
	OUT OUT_EX_SERVER_URL    VARCHAR(4096), -- 외부 서버 다운로드 URL (다수개일 경우 '|'로 구분)
	OUT OUT_FILE_PATH        VARCHAR(600),  -- PRIMX-CS 다운로드 서버 사용 시 파일 경로
	OUT OUT_FILE_HASH        VARCHAR(200)   -- 업데이트 파일 해시
)
BEGIN
	/*
	COMMENT: PRIMX 제품 업데이트 정보 추출
	PARAMETERS:
		IN IN_AGENT_ID           - Agent ID
		IN IN_PRODUCT_TYPE       - 제품 종료 (ZONECENTRAL/ORIZON/CRYHOD/AGENT)
		IN IN_INSTALLED_VERSION  - 에이전트에 설치된 제품 버전
		OUT OUT_NEED_PATCH_YN    - 패치 필요 여부 (YES/NO)
		OUT OUT_PRODUCT_VERSION  - 설치되어야 할 제품 버전
		OUT OUT_USE_EX_SERVER_YN - 외부 다운로드 서버 사용 여부 (YES/NO)
		OUT OUT_EX_SERVER_URL    - 외부 서버 다운로드 URL (다수개일 경우 '|'로 구분)
		OUT OUT_FILE_PATH        - PRIMX-CS 다운로드 서버 사용 시 파일 경로
		OUT OUT_FILE_HASH        - 업데이트 파일 해시
	HISTORY:
		2023-07-22 생성
	*/

	-- 기본 변수 선언
	DECLARE V_CNT_UPDATE_INFO        INT(11) DEFAULT 0;
	DECLARE V_PRODUCT_VERSION        VARCHAR(20) DEFAULT '';
	DECLARE V_UPADTE_FILE_HASH       VARCHAR(200) DEFAULT '';
	DECLARE V_UPDATE_FILE_PATH       VARCHAR(600) DEFAULT '';
	DECLARE V_UPDATE_FILE_NAME       VARCHAR(200) DEFAULT '';
	DECLARE V_EX_SERVER_URL          VARCHAR(4096) DEFAULT '';
	DECLARE V_VERSION_COMPARE_RESULT INT(11) DEFAULT 0;
	DECLARE V_CNT_LICENSE            INT(11) DEFAULT 0;
	DECLARE V_PRODUCT_NAME           VARCHAR(255) DEFAULT 'NONE';
	DECLARE V_END_DATE_STR           VARCHAR(255) DEFAULT '';
	DECLARE V_END_DATE               DATETIME;
	DECLARE V_IS_VALID_LICENSE       TINYINT(1) DEFAULT 0;
	DECLARE V_CNT_SYSTEM_MASTER      INT(11) DEFAULT 0;
	DECLARE V_AGENT_PROCESSOR_ARCH   INT(11) DEFAULT 0;
	
	-- 예외처리 변수 선언
	DECLARE V_PROC_NAME      VARCHAR(256) DEFAULT 'PROC_GET_PRODUCT_UPDATE_INFO';
	DECLARE V_SQL_STATE      VARCHAR(20) DEFAULT '';
	DECLARE V_INT_ERROR_NO   INT(11) DEFAULT 0;
	DECLARE V_PROC_COMMENT   VARCHAR(2048) DEFAULT '';
	DECLARE V_TEXT_ERROR_MSG TEXT;
	
	-- 예외처리
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
	BEGIN
		GET DIAGNOSTICS CONDITION 1 V_SQL_STATE = RETURNED_SQLSTATE
		, V_INT_ERROR_NO = MYSQL_ERRNO
		, V_TEXT_ERROR_MSG = MESSAGE_TEXT;
		SET V_PROC_COMMENT = CONCAT('AGENT_ID:', IN_AGENT_ID, '/PRODUCT_TYPE:', IN_PRODUCT_TYPE, '/INSTALLED_VERSION:', IN_INSTALLED_VERSION);
		INSERT T_SYSTEM_PROC_ERROR (C_ERROR_TIME, C_PROC_NAME, C_SQL_STATE, C_ERROR_NO, C_COMMENT, C_ERROR_MSG)
		VALUES (NOW(), V_PROC_NAME, V_SQL_STATE, V_INT_ERROR_NO, V_PROC_COMMENT, V_TEXT_ERROR_MSG);
		-- RESIGNAL;
	END;
	
	-- 기본값 설정
	SET OUT_NEED_PATCH_YN    = 'NO';
	SET OUT_PRODUCT_VERSION  = '';
	SET OUT_EX_SERVER_URL    = '';
	SET OUT_FILE_PATH        = '';
	SET OUT_FILE_HASH        = '';
	SET V_PRODUCT_NAME       = (CASE IN_PRODUCT_TYPE 
								WHEN 1 THEN 'ZONECENTRAL' 
								WHEN 2 THEN 'ORIZON'
								WHEN 3 THEN 'CRYHOD'
								WHEN 4 THEN 'AGENT'
								ELSE 'NONE'
								END);
	
	-- 정보 추출
	SELECT COUNT(*), IFNULL(C_PROCESSOR_ARCHITECTURE, 9)  
	INTO V_CNT_SYSTEM_MASTER, V_AGENT_PROCESSOR_ARCH 
	FROM T_SYSTEM_MASTER 
	WHERE C_AGENT_ID = IN_AGENT_ID; 
	
	SELECT COUNT(*), UPPER(IFNULL(END_DATE, '')) 
	INTO V_CNT_LICENSE, V_END_DATE_STR 
	FROM T_LICENSE_DETAIL 
	WHERE UPPER(PRODUCT_NAME) LIKE CONCAT('%', V_PRODUCT_NAME, '%') AND UPPER(STATUS_CD) = 'USING' LIMIT 1; 

	SELECT COUNT(*), IFNULL(C_PRODUCT_VERSION, ''), IFNULL(C_UPDATE_FILE_HASH, ''), IFNULL(C_UPDATE_FILE_PATH, '') 
	INTO V_CNT_UPDATE_INFO, V_PRODUCT_VERSION, V_UPADTE_FILE_HASH, V_UPDATE_FILE_PATH 
	FROM T_UPDATE_INFO 
	WHERE C_PRODUCT_TYPE = IN_PRODUCT_TYPE AND C_PROCESSOR_ARCHITECTURE = V_AGENT_PROCESSOR_ARCH;
	
	SET V_UPDATE_FILE_NAME = FUNC_GET_FILE_NAME_FROM_PATH(V_UPDATE_FILE_PATH);
	
	SELECT GROUP_CONCAT(CONCAT(C_SERVER_URL, '/', V_UPDATE_FILE_NAME) SEPARATOR '|') 
	INTO V_EX_SERVER_URL 
	FROM T_EX_SERVER_INFO
	WHERE LENGTH(IFNULL(C_SERVER_URL, '')) > 0;
	
	-- AGENT 인 경우에는 라이센스 항상 유효, 기간은 무제한
	IF IN_PRODUCT_TYPE = 4 THEN
		SET V_CNT_LICENSE = 1;
		SET V_END_DATE_STR = 'UNLIMITED';
	END IF;
	
	-- 라이센스 체크
	IF V_CNT_LICENSE > 0 THEN 
		IF V_END_DATE_STR = 'UNLIMITED' THEN 
			SET V_IS_VALID_LICENSE = 1;
		ELSEIF DATE(STR_TO_DATE(V_END_DATE_STR, '%Y-%m-%d')) IS NOT NULL THEN
			SET V_END_DATE = STR_TO_DATE(V_END_DATE_STR, '%Y-%m-%d');
			IF UTC_TIMESTAMP() <= V_END_DATE THEN
				SET V_IS_VALID_LICENSE = 1;
			END IF;
		END IF;
	END IF;
	
	-- 라이센스가 있는 경우
	IF V_IS_VALID_LICENSE = 1 THEN
		-- 업데이트 정보가 있는 경우
		IF V_CNT_UPDATE_INFO > 0 THEN
			-- 현재 버전이 없는 경우 (설치되지 않은 경우)
			IF LENGTH(IN_INSTALLED_VERSION) = 0 THEN
				-- 무조건 설치
				SET V_VERSION_COMPARE_RESULT = -1;
			-- 현재 버전이 있는 경우 (설치된 경우)
			ELSE
				-- 현재 설치된 버전과 이전 버전 비교 후 설치 여부 판단
				SET V_VERSION_COMPARE_RESULT = `PRIMXDB`.`FUNC_COMPARE_VERSION`(IN_INSTALLED_VERSION, V_PRODUCT_VERSION);
			END IF;
			
			-- 업데이트가 필요한 경우
			IF V_VERSION_COMPARE_RESULT != 0 THEN 
				SET OUT_NEED_PATCH_YN = 'YES';
				SET OUT_PRODUCT_VERSION = V_PRODUCT_VERSION;
				SET OUT_FILE_HASH = V_UPADTE_FILE_HASH;
				SET OUT_FILE_PATH = V_UPDATE_FILE_PATH;
				SET OUT_EX_SERVER_URL = V_EX_SERVER_URL;
			END IF;
		END IF;
	END IF;
END |
DELIMITER ; 

-- 반출파일 인증 결과 추출
DELIMITER |
DROP PROCEDURE IF EXISTS `PRIMXDB`.`PROC_GET_EX_FILE_AUTH_INFO` |
CREATE PROCEDURE `PRIMXDB`.`PROC_GET_EX_FILE_AUTH_INFO`
(
	IN IN_USER_ID       VARCHAR(50),  -- User ID
	IN IN_AGENT_ID      VARCHAR(50),  -- Agent ID
	IN IN_FILE_KEY      VARCHAR(50),  -- 파일 고유키
	IN IN_COMPANY_ID    VARCHAR(50),  -- 회사 ID
	IN IN_FILE_HASH     VARCHAR(200), -- 파일 해시 코드
	OUT OUT_RESULT_CODE VARCHAR(20)   -- 인증 결과 코드 
)
BEGIN
	/*
	COMMENT: 반출파일 인증 결과 추출
	PARAMETERS:
		IN IN_USER_ID       - User ID
		IN IN_AGENT_ID      - Agent ID
		IN IN_FILE_KEY      - 파일 고유키
		IN IN_COMPANY_ID    - 회사 ID
		IN IN_FILE_HASH     - 파일 해시 코드
		OUT OUT_RESULT_CODE - 인증 결과 코드
			SUCCESS: 인증성공
			E20001 : User ID를 찾을 수 없음
			E20002 : Agent ID가 공백
			E20003 : User ID가 공백
			E20005 : 파일 키가 공백
			E20006 : 회사 ID가 공백
			E20007 : 파일 해시코드가 공백
			E40002 : 파일 키를 찾을 수 없음
			E40003 : 파일 해시 불일치
			E40004 : 복호화 기간 만료
			E40005 : 복호화 회수 초과
			E40006 : User ID 불일치
			E40007 : Agent ID 불일치
	HISTORY:
		2023-07-22 생성
	*/
	-- 기본 변수 선언
	DECLARE V_CNT_AUTH_INFO INT(11) DEFAULT 0;
	DECLARE V_COMPANY_ID    VARCHAR(50) DEFAULT '';
	DECLARE V_AGENT_ID      VARCHAR(50) DEFAULT '';
	DECLARE V_USER_ID       VARCHAR(50) DEFAULT '';
	DECLARE V_FILE_HASH     VARCHAR(200) DEFAULT '';
	DECLARE V_DECRYPT_CNT   INT(11) DEFAULT 0;
	DECLARE V_EXPIRE_DATE   DATETIME;
	
	-- 예외처리 변수 선언
	DECLARE V_PROC_NAME      VARCHAR(256) DEFAULT 'PROC_GET_EX_FILE_AUTH_INFO';
	DECLARE V_SQL_STATE      VARCHAR(20) DEFAULT '';
	DECLARE V_INT_ERROR_NO   INT(11) DEFAULT 0;
	DECLARE V_PROC_COMMENT   VARCHAR(2048) DEFAULT '';
	DECLARE V_TEXT_ERROR_MSG TEXT;
	
	-- 예외처리
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
	BEGIN
		GET DIAGNOSTICS CONDITION 1 V_SQL_STATE = RETURNED_SQLSTATE
		, V_INT_ERROR_NO = MYSQL_ERRNO
		, V_TEXT_ERROR_MSG = MESSAGE_TEXT;
		SET V_PROC_COMMENT = CONCAT('USER_ID:', IN_USER_ID, '/AGENT_ID:', IN_AGENT_ID, '/FILE_KEY:', IN_FILE_KEY, '/IN_COMPANY_ID:', IN_COMPANY_ID, '/FILE_HASH:', IN_FILE_HASH);
		INSERT T_SYSTEM_PROC_ERROR (C_ERROR_TIME, C_PROC_NAME, C_SQL_STATE, C_ERROR_NO, C_COMMENT, C_ERROR_MSG)
		VALUES (NOW(), V_PROC_NAME, V_SQL_STATE, V_INT_ERROR_NO, V_PROC_COMMENT, V_TEXT_ERROR_MSG);
		-- RESIGNAL;
	END;
	
	-- 기본값 설정
	SET OUT_RESULT_CODE = 'SUCCESS';
	
	IF     LENGTH(IN_AGENT_ID)   = 0 THEN SET OUT_RESULT_CODE = 'E20002';
	ELSEIF LENGTH(IN_USER_ID)    = 0 THEN SET OUT_RESULT_CODE = 'E20003';
	ELSEIF LENGTH(IN_FILE_KEY)   = 0 THEN SET OUT_RESULT_CODE = 'E20005';
	ELSEIF LENGTH(IN_COMPANY_ID) = 0 THEN SET OUT_RESULT_CODE = 'E20006';
	ELSEIF LENGTH(IN_FILE_HASH)  = 0 THEN SET OUT_RESULT_CODE = 'E20007';
	ELSE	
		SELECT COUNT(*), IFNULL(C_COMPANY_ID, ''), IFNULL(C_AGENT_ID, ''), IFNULL(C_USER_ID, ''), IFNULL(C_FILE_HASH, ''), IFNULL(C_DECRYPT_CNT, 0), IFNULL(C_EXPIRE_DATE, UTC_TIMESTAMP())  
		INTO V_CNT_AUTH_INFO, V_COMPANY_ID, V_AGENT_ID, V_USER_ID, V_FILE_HASH, V_DECRYPT_CNT, V_EXPIRE_DATE 
		FROM T_EX_FILE_AUTH_INFO WHERE C_FILE_KEY = IN_FILE_KEY;
		
		IF     V_USER_ID != IN_USER_ID          THEN SET OUT_RESULT_CODE = 'E40006';
		ELSEIF V_AGENT_ID != IN_AGENT_ID        THEN SET OUT_RESULT_CODE = 'E40007';
		ELSEIF V_CNT_AUTH_INFO = 0              THEN SET OUT_RESULT_CODE = 'E40002';
		ELSEIF V_FILE_HASH != IN_FILE_HASH      THEN SET OUT_RESULT_CODE = 'E40003';
		ELSEIF V_EXPIRE_DATE <= UTC_TIMESTAMP() THEN SET OUT_RESULT_CODE = 'E40004';
		ELSEIF V_DECRYPT_CNT <= 0               THEN SET OUT_RESULT_CODE = 'E40005';
		END IF;
	END IF;	
END |
DELIMITER ; 

-- 반출파일 인증 결과 반영 및 로그기록
DELIMITER |
DROP PROCEDURE IF EXISTS `PRIMXDB`.`PROC_APPLY_EX_FILE_AUTH_INFO` |
CREATE PROCEDURE `PRIMXDB`.`PROC_APPLY_EX_FILE_AUTH_INFO` 
(
	IN IN_USER_ID     VARCHAR(50),  -- User ID
	IN IN_AGENT_ID    VARCHAR(50),  -- Agent ID
	IN IN_COMPANY_ID  VARCHAR(50),  -- 회사 ID
	IN IN_FILE_KEY    VARCHAR(50),  -- 파일 고유키
	IN IN_REQUEST_IP  VARCHAR(15),  -- 이동파일 인증 요청 PC IP 주소
	IN IN_REQUEST_MAC VARCHAR(30),  -- 이동파일 인증 요청 PC MAC 주소
	IN IN_RESULT_CODE VARCHAR(20),  -- 결과 코드 
	IN IN_COMMENT     VARCHAR(2048) -- 추가 설명
)
BEGIN
	/*
	COMMENT: 반출파일 인증 결과 반영 및 로그기록
	PARAMETERS:
		IN IN_USER_ID     - User ID
		IN IN_AGENT_ID    - Agent ID
		IN IN_COMPANY_ID  - 회사 ID
		IN IN_FILE_KEY    - 파일 고유키
		IN IN_REQUEST_IP  - 이동파일 인증 요청 PC IP 주소
		IN IN_REQUEST_MAC - 이동파일 인증 요청 PC MAC 주소
		IN IN_RESULT_CODE - 결과 코드
		IN IN_COMMENT     - 추가 설명
	HISTORY:
		2023-07-22 생성
		2023-09-22 로그 기록 테이블 변경. T_MANAGE_LOG -> T_EX_FILE_AUTH_LOG
		2023-09-25 이동파일 인증 요청 PC IP/MAC 추가
	*/
	-- 기본 변수 선언
	DECLARE V_CNT_SYSTEM_MASTER INT(11) DEFAULT 0;
	DECLARE V_USER_ID           VARCHAR(50) DEFAULT '';
	DECLARE V_USER_NAME         VARCHAR(50) DEFAULT '';
	DECLARE V_CNT_FILE          INT(11) DEFAULT 0;
	DECLARE V_FILE_NAME         VARCHAR(200) DEFAULT '';
	DECLARE V_FILE_SIZE         BIGINT(20) DEFAULT 0;
	DECLARE V_FILE_HASH         VARCHAR(200) DEFAULT '';
	DECLARE V_DECRYPT_CNT       INT(11) DEFAULT 0;
	DECLARE V_EXPIRE_DATE       DATETIME DEFAULT NULL;
	
	-- 예외처리 변수 선언
	DECLARE V_PROC_NAME      VARCHAR(256) DEFAULT 'PROC_APPLY_EX_FILE_AUTH_INFO';
	DECLARE V_SQL_STATE      VARCHAR(20) DEFAULT '';
	DECLARE V_INT_ERROR_NO   INT(11) DEFAULT 0;
	DECLARE V_PROC_COMMENT   VARCHAR(2048) DEFAULT '';
	DECLARE V_TEXT_ERROR_MSG TEXT;
	
	-- 예외처리
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
	BEGIN
		ROLLBACK;
		GET DIAGNOSTICS CONDITION 1 V_SQL_STATE = RETURNED_SQLSTATE
		, V_INT_ERROR_NO = MYSQL_ERRNO
		, V_TEXT_ERROR_MSG = MESSAGE_TEXT;
		SET V_PROC_COMMENT = CONCAT('USER_ID:', IN_USER_ID, '/AGENT_ID:', IN_AGENT_ID, '/COMPANY_ID:', IN_COMPANY_ID, '/FILE_KEY:', IN_FILE_KEY, '/REQUEST_IP:', IN_REQUEST_IP, '/REQUEST_MAC:', IN_REQUEST_MAC, '/RESULT_CODE:', IN_RESULT_CODE, '/COMMENT:', IN_COMMENT);
		INSERT T_SYSTEM_PROC_ERROR (C_ERROR_TIME, C_PROC_NAME, C_SQL_STATE, C_ERROR_NO, C_COMMENT, C_ERROR_MSG)
		VALUES (NOW(), V_PROC_NAME, V_SQL_STATE, V_INT_ERROR_NO, V_PROC_COMMENT, V_TEXT_ERROR_MSG);
		-- RESIGNAL;
	END;
	
	START TRANSACTION;

	SELECT COUNT(SM.C_AGENT_ID), IFNULL(USM.C_USER_ID, ''), IFNULL(UM.USER_NM, '') 
	INTO V_CNT_SYSTEM_MASTER, V_USER_ID, V_USER_NAME 
	FROM 
		T_SYSTEM_MASTER AS SM 
		LEFT OUTER JOIN T_USER_SYSTEM_MATCH AS USM ON SM.C_AGENT_ID = USM.C_AGENT_ID 
		LEFT OUTER JOIN COMTNEMPLYRINFO AS UM ON UM.EMPLYR_ID = USM.C_USER_ID
	WHERE SM.C_AGENT_ID = IN_AGENT_ID;
		
	SELECT COUNT(C_FILE_KEY), IFNULL(C_FILE_NAME, ''), IFNULL(C_FILE_SIZE, 0), IFNULL(C_FILE_HASH, ''), IFNULL(C_DECRYPT_CNT, 0), C_EXPIRE_DATE  
	INTO V_CNT_FILE, V_FILE_NAME, V_FILE_SIZE, V_FILE_HASH, V_DECRYPT_CNT, V_EXPIRE_DATE 
	FROM T_EX_FILE_AUTH_INFO WHERE C_FILE_KEY = IN_FILE_KEY;

	IF IN_RESULT_CODE = 'SUCCESS' THEN
		UPDATE T_EX_FILE_AUTH_INFO SET 
		C_DECRYPT_CNT = (CASE V_DECRYPT_CNT WHEN 0 THEN 0 ELSE V_DECRYPT_CNT - 1 END)
		WHERE C_FILE_KEY = IN_FILE_KEY;

		INSERT INTO T_EX_FILE_AUTH_LOG (C_LOG_TIME, C_FILE_KEY, C_RESULT_CODE, C_USER_ID, C_USER_NAME, C_AGENT_ID, C_REQUEST_IP, C_REQUEST_MAC, C_FILE_NAME, C_FILE_SIZE, C_FILE_HASH, C_DECRYPT_CNT, C_EXPIRE_DATE) 
		VALUES (UTC_TIMESTAMP(), IN_FILE_KEY, 'SUCCESS', IN_USER_ID, V_USER_NAME, IN_AGENT_ID, IN_REQUEST_IP, IN_REQUEST_MAC, V_FILE_NAME, V_FILE_SIZE, V_FILE_HASH, (CASE V_DECRYPT_CNT WHEN 0 THEN 0 ELSE V_DECRYPT_CNT - 1 END), V_EXPIRE_DATE);
	ELSE
		INSERT INTO T_EX_FILE_AUTH_LOG (C_LOG_TIME, C_FILE_KEY, C_RESULT_CODE, C_USER_ID, C_USER_NAME, C_AGENT_ID, C_REQUEST_IP, C_REQUEST_MAC, C_FILE_NAME, C_FILE_SIZE, C_FILE_HASH, C_DECRYPT_CNT, C_EXPIRE_DATE, C_COMMENT) 
		VALUES (UTC_TIMESTAMP(), IN_FILE_KEY, IN_RESULT_CODE, IN_USER_ID, V_USER_NAME, IN_AGENT_ID, IN_REQUEST_IP, IN_REQUEST_MAC, V_FILE_NAME, V_FILE_SIZE, V_FILE_HASH, (CASE V_DECRYPT_CNT WHEN 0 THEN 0 ELSE V_DECRYPT_CNT - 1 END), V_EXPIRE_DATE, IN_COMMENT);
	END IF;
	
	COMMIT;
END |
DELIMITER ; 

-- T_EX_FILE_AUTH_INFO 테이블에 새로운 반출파일 정보 등록
DELIMITER |
DROP PROCEDURE IF EXISTS `PRIMXDB`.`PROC_INSERT_T_EX_FILE_AUTH_INFO` |
CREATE PROCEDURE `PRIMXDB`.`PROC_INSERT_T_EX_FILE_AUTH_INFO`
(
	IN IN_USER_ID    VARCHAR(50),   -- User ID
	IN IN_AGENT_ID   VARCHAR(50),   -- Agent ID
	IN IN_FILE_KEY   VARCHAR(50),   -- 파일 고유키
	IN IN_COMPANY_ID VARCHAR(50),   -- 회사 ID
	IN IN_FILE_HASH  VARCHAR(200),  -- 파알 해시 코드
	IN IN_FILE_NAME  VARCHAR(200),  -- 파일 이름
	IN IN_FILE_SIZE  BIGINT(20),    -- 파일 크기 
	OUT OUT_RESULT_CODE VARCHAR(20) -- 결과 코드
)
BEGIN
	/*
	COMMENT: T_EX_FILE_AUTH_INFO 테이블에 새로운 반출파일 정보 등록
	PARAMETERS:
		IN IN_USER_ID       - User ID
		IN IN_AGENT_ID      - Agent ID
		IN IN_FILE_KEY      - 파일 고유키
		IN IN_COMPANY_ID    - 회사 ID
		IN IN_FILE_HASH     - 파일 해시 코드
		IN IN_FILE_NAME     - 파일 이름
		IN IN_FILE_SIZE     - 파일 크기 
		OUT OUT_RESULT_CODE - 결과 코드
			SUCCESS: 인증성공
			E10002 : 트렌잭션 쿼리 실패
			E40008 : 동일한 파일 키가 이미 존재
	HISTORY:
		2023-07-22 생성
		2023-09-21 파일 이름, 파일 크기 추가
	*/
	-- 기본 변수 선언
	DECLARE V_CNT_EX_FILE_INFO          INT(11) DEFAULT 0; 
	DECLARE V_CNT_EXPIRE_DAY_LIMIT      INT(11) DEFAULT 0; -- FileExpireDayLimit
	DECLARE V_EXPIRE_DAY_LIMIT_VALUE    VARCHAR(200) DEFAULT '3';
	DECLARE V_EXPIRE_DAY_LIMIT          INT(11) DEFAULT 3;
	DECLARE V_CNT_DECRYPT_COUNT_LIMIT   INT(11) DEFAULT 0; -- FileDecryptCountLimit
	DECLARE V_DECRYPT_COUNT_LIMIT_VALUE VARCHAR(200) DEFAULT '10';
	DECLARE V_DECRYPT_COUNT_LIMIT       INT(11) DEFAULT 10;
	DECLARE V_EXPIRE_DATE               DATETIME;
	
	-- 예외처리 변수 선언
	DECLARE V_PROC_NAME      VARCHAR(256) DEFAULT 'PROC_INSERT_T_EX_FILE_AUTH_INFO';
	DECLARE V_SQL_STATE      VARCHAR(20) DEFAULT '';
	DECLARE V_INT_ERROR_NO   INT(11) DEFAULT 0;
	DECLARE V_PROC_COMMENT   VARCHAR(2048) DEFAULT '';
	DECLARE V_TEXT_ERROR_MSG TEXT;
	
	-- 예외처리
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
	BEGIN
		SET OUT_RESULT_CODE = 'E10002';
		ROLLBACK;
		GET DIAGNOSTICS CONDITION 1 V_SQL_STATE = RETURNED_SQLSTATE
		, V_INT_ERROR_NO = MYSQL_ERRNO
		, V_TEXT_ERROR_MSG = MESSAGE_TEXT;
		SET V_PROC_COMMENT = CONCAT('USER_ID:', IN_USER_ID, '/AGENT_ID:', IN_AGENT_ID, '/FILE_KEY:', IN_FILE_KEY, '/IN_COMPANY_ID:', IN_COMPANY_ID, '/FILE_NAME:', IN_FILE_NAME, '/FILE_SIZE:', IN_FILE_SIZE);
		INSERT T_SYSTEM_PROC_ERROR (C_ERROR_TIME, C_PROC_NAME, C_SQL_STATE, C_ERROR_NO, C_COMMENT, C_ERROR_MSG)
		VALUES (NOW(), V_PROC_NAME, V_SQL_STATE, V_INT_ERROR_NO, V_PROC_COMMENT, V_TEXT_ERROR_MSG);
		-- RESIGNAL;
	END;
	
	-- 기본값 설정
	SET OUT_RESULT_CODE = 'SUCCESS';
	
	-- 설정값 추출 및 연산
	SELECT COUNT(*) INTO V_CNT_EX_FILE_INFO FROM T_EX_FILE_AUTH_INFO WHERE C_FILE_KEY = IN_FILE_KEY;
	
	SELECT COUNT(*), IFNULL(C_VALUE_DATA, '3') 
	INTO V_CNT_EXPIRE_DAY_LIMIT, V_EXPIRE_DAY_LIMIT_VALUE 
	FROM T_SERVER_CONFIG WHERE C_KEY_NAME = 'FileExpireDayLimit' LIMIT 1;
	
	SET V_EXPIRE_DAY_LIMIT_VALUE = REGEXP_REPLACE(V_EXPIRE_DAY_LIMIT_VALUE, '[a-z]|[\]\[\, ]', '');
	IF LENGTH(V_EXPIRE_DAY_LIMIT_VALUE) = 0 THEN SET V_EXPIRE_DAY_LIMIT_VALUE = '3'; END IF;	
	SET V_EXPIRE_DAY_LIMIT = CONVERT(V_EXPIRE_DAY_LIMIT_VALUE, DECIMAL);
	
	SELECT COUNT(*), IFNULL(C_VALUE_DATA, '10') 
	INTO V_CNT_DECRYPT_COUNT_LIMIT, V_DECRYPT_COUNT_LIMIT_VALUE 
	FROM T_SERVER_CONFIG WHERE C_KEY_NAME = 'FileDecryptCountLimit' LIMIT 1;

	SET V_DECRYPT_COUNT_LIMIT_VALUE = REGEXP_REPLACE(V_DECRYPT_COUNT_LIMIT_VALUE, '[a-z]|[\]\[\, ]', '');
	IF LENGTH(V_DECRYPT_COUNT_LIMIT_VALUE) = 0 THEN SET V_DECRYPT_COUNT_LIMIT_VALUE = '10'; END IF;	
	SET V_DECRYPT_COUNT_LIMIT = CONVERT(V_DECRYPT_COUNT_LIMIT_VALUE, DECIMAL);
	
	-- 동일한 파일 키가 이미 존재하는 지 확인
	IF V_CNT_EX_FILE_INFO > 0 THEN 
		SET OUT_RESULT_CODE = 'E40008';
	-- 동일한 파일 키가 없으면 삽입
	ELSE
		SET V_EXPIRE_DATE = (UTC_TIMESTAMP() + INTERVAL V_EXPIRE_DAY_LIMIT DAY);
	
		START TRANSACTION;
		INSERT INTO T_EX_FILE_AUTH_INFO (C_FILE_KEY, C_COMPANY_ID, C_AGENT_ID, C_USER_ID, C_FILE_HASH, C_FILE_NAME, C_FILE_SIZE, C_DECRYPT_CNT, C_EXPIRE_DATE, C_INSERT_DATE)
		VALUES 
		(IN_FILE_KEY, IN_COMPANY_ID, IN_AGENT_ID, IN_USER_ID, IN_FILE_HASH, IN_FILE_NAME, IN_FILE_SIZE, V_DECRYPT_COUNT_LIMIT, V_EXPIRE_DATE, UTC_TIMESTAMP());
		COMMIT;
	END IF;
END |
DELIMITER ; 

-- T_POLICY_SYSTEM_MAP 테이블에 제품 별 기본 정책 삽입 또는 삭제
DELIMITER |
DROP PROCEDURE IF EXISTS `PRIMXDB`.`PROC_OPERATE_T_POLICY_SYSTEM_MAP` |
CREATE PROCEDURE `PRIMXDB`.`PROC_OPERATE_T_POLICY_SYSTEM_MAP`
(
	IN IN_AGENT_ID   VARCHAR(50), -- Agent ID
	IN IN_PRODUCT_ID INT(11),     -- 제품 ID
	IN IN_ACTION     VARCHAR(10)  -- 동작 옵션. 'ADD': 제품 설치 시 동작 / 'DELETE': 제품 삭제 시 동작
)
BEGIN
	/*
	COMMENT: T_POLICY_SYSTEM_MAP 테이블에 제품 별 기본 정책 삽입 또는 삭제
	PARAMETERS:
		IN IN_AGENT_ID   - Agent ID
		IN IN_PRODUCT_ID - 제품 ID
		IN IN_ACTION     - 동작 옵션. 'ADD': 제품 설치 시 동작 / 'DELETE': 제품 삭제 시 동작
	HISTORY:
		2023-12-28 최초 생성
	*/
	-- 기본 변수 선언
	DECLARE V_CNT_POLICY     INT(11) DEFAULT 0; 
	DECLARE V_POLICY_CODE    VARCHAR(255) DEFAULT '3';
	DECLARE V_CNT_POLICY_MAP INT(11) DEFAULT 0;
	
	-- 예외처리 변수 선언
	DECLARE V_PROC_NAME      VARCHAR(256) DEFAULT 'PROC_OPERATE_T_POLICY_SYSTEM_MAP';
	DECLARE V_SQL_STATE      VARCHAR(20) DEFAULT '';
	DECLARE V_INT_ERROR_NO   INT(11) DEFAULT 0;
	DECLARE V_PROC_COMMENT   VARCHAR(2048) DEFAULT '';
	DECLARE V_TEXT_ERROR_MSG TEXT;
	
	-- 예외처리
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
	BEGIN
		GET DIAGNOSTICS CONDITION 1 V_SQL_STATE = RETURNED_SQLSTATE
		, V_INT_ERROR_NO = MYSQL_ERRNO
		, V_TEXT_ERROR_MSG = MESSAGE_TEXT;
		SET V_PROC_COMMENT = CONCAT('/AGENT_ID:', IN_AGENT_ID, '/PRODUCT_ID:', IN_PRODUCT_ID, '/ACTION:', IN_ACTION);
		INSERT T_SYSTEM_PROC_ERROR (C_ERROR_TIME, C_PROC_NAME, C_SQL_STATE, C_ERROR_NO, C_COMMENT, C_ERROR_MSG)
		VALUES (NOW(), V_PROC_NAME, V_SQL_STATE, V_INT_ERROR_NO, V_PROC_COMMENT, V_TEXT_ERROR_MSG);
		-- RESIGNAL;
	END;

	-- 기본 정책 추출
	SELECT COUNT(*), IFNULL(policy_code, '') 
	INTO V_CNT_POLICY, V_POLICY_CODE 
	FROM T_POLICY 
	WHERE product_id = IN_PRODUCT_ID AND is_default = 'Y' AND is_deleted = 'N' AND use_yn = 'Y';

	-- 제품 설치 경우
	IF IN_ACTION = 'ADD' THEN 
		IF V_CNT_POLICY > 0 THEN
			SELECT COUNT(*) INTO V_CNT_POLICY_MAP FROM T_POLICY_SYSTEM_MAP 
			WHERE policy_code = V_POLICY_CODE AND C_AGENT_ID = IN_AGENT_ID;
		
			IF V_CNT_POLICY_MAP = 0 THEN 
				INSERT IGNORE INTO T_POLICY_SYSTEM_MAP (policy_code, C_AGENT_ID, modified_date) 
				VALUES (V_POLICY_CODE, IN_AGENT_ID, UTC_TIMESTAMP());
			END IF;
		END IF;
	-- 제폼 삭제 경우
	ELSEIF IN_ACTION = 'DELETE' THEN 
		DELETE FROM T_POLICY_SYSTEM_MAP 
		WHERE C_AGENT_ID = IN_AGENT_ID 
		AND policy_code IN (SELECT DISTINCT policy_code FROM T_POLICY WHERE product_id = IN_PRODUCT_ID);
	END IF;
END |
DELIMITER ; 

/* ----------------------------------------------------------------------------
	PROCEDURES - HR SYNC.
---------------------------------------------------------------------------- */ 
-- 조직 정보 동기화 (커스터마이징이 필요할 수 있는 프로시저여서, 기본 프로시저가 없을 경우에만 삽입하고, 있을 경우에는 갱신 안함.)
DELIMITER |
-- DROP PROCEDURE IF EXISTS `PRIMXDB`.`PROC_SYNC_HR_DEPT`|
CREATE PROCEDURE IF NOT EXISTS `PRIMXDB`.`PROC_SYNC_HR_DEPT`
(
	OUT OUT_DEPT_RESULT TINYINT(1) -- 결과 코드. (0: 실패 / 1: 성공)
)
BEGIN
	/*
	COMMENT: 조직 정보 동기화
	PARAMETERS:
		OUT OUT_DEPT_RESULT - 결과 코드. (0: 실패 / 1: 성공)
	HISTORY:
		2023-08-14 생성
	*/
	-- 기본 변수 선언
	DECLARE V_NO_DEPT_ID VARCHAR(50) DEFAULT 'NO_DEPT';
	DECLARE V_COMPANY_ID VARCHAR(50) DEFAULT '00000001';
	
	-- 예외처리 변수 선언
	DECLARE V_PROC_NAME      VARCHAR(256) DEFAULT 'PROC_SYNC_HR_DEPT';
	DECLARE V_SQL_STATE      VARCHAR(20) DEFAULT '';
	DECLARE V_INT_ERROR_NO   INT(11) DEFAULT 0;
	DECLARE V_PROC_COMMENT   VARCHAR(2048) DEFAULT '';
	DECLARE V_TEXT_ERROR_MSG TEXT;
	
	-- 예외처리
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
	BEGIN
		GET DIAGNOSTICS CONDITION 1 V_SQL_STATE = RETURNED_SQLSTATE
		, V_INT_ERROR_NO = MYSQL_ERRNO
		, V_TEXT_ERROR_MSG = MESSAGE_TEXT;
		SET V_PROC_COMMENT = '';
		INSERT T_SYSTEM_PROC_ERROR (C_ERROR_TIME, C_PROC_NAME, C_SQL_STATE, C_ERROR_NO, C_COMMENT, C_ERROR_MSG)
		VALUES (NOW(), V_PROC_NAME, V_SQL_STATE, V_INT_ERROR_NO, V_PROC_COMMENT, V_TEXT_ERROR_MSG);
		SET OUT_DEPT_RESULT = 0;
	END;
	
	-- 리턴값 초기화
	SET OUT_DEPT_RESULT = 1;
	-- 현재 Company ID 추출
	SELECT DISTINCT C_VALUE_DATA INTO V_COMPANY_ID FROM T_SERVER_CONFIG WHERE C_KEY_NAME = 'CompanyID';
	
	-- 1st: 조직 삭제 -------------------------------------------------------------
	-- (1) T_DEPT_MASTER에 존재하면서 T_TEMP_DEPT_MASTER에 없는 조직(삭제대상)에 소속된 사용자는 소속 부서가 없는 것으로 업데이트.
	-- 단, 대상은 C_INSERTED_BY_HR_YN가 'Y'인 조직
	UPDATE T_USER_MASTER 
		SET C_DEPT_ID = V_NO_DEPT_ID 
	WHERE 
		C_COMPANY_ID = V_COMPANY_ID 
		AND C_DEPT_ID IN 
		( 
			SELECT 
				A.C_DEPT_ID 
			FROM 
				T_DEPT_MASTER AS A 
				LEFT OUTER JOIN T_TEMP_DEPT_MASTER AS B ON A.C_DEPT_ID = B.C_DEPT_ID 
			WHERE 
				A.C_COMPANY_ID = V_COMPANY_ID 
				AND B.C_DEPT_ID IS NULL 
				AND A.C_INSERTED_BY_HR_YN = 'Y' 
		);
	
	-- (2) 삭제 대상 조직을 T_DEPT_MASTER에서 삭제 
	-- 단, 대상은 C_INSERTED_BY_HR_YN가 'Y'인 조직
	DELETE A FROM 
		T_DEPT_MASTER AS A
		LEFT OUTER JOIN T_TEMP_DEPT_MASTER AS B ON A.C_DEPT_ID = B.C_DEPT_ID
	WHERE
		A.C_COMPANY_ID = V_COMPANY_ID 
		AND B.C_DEPT_ID IS NULL 
		AND A.C_INSERTED_BY_HR_YN = 'Y';
	
	-- 2nd: 조직 업데이트 -----------------------------------------------------------
	-- (1) T_DEPT_MASTER와 T_TEMP_DEPT_MASTER에 동시에 존재하면서 T_TEMP_DEPT_MASTER.C_UPDATE_DATE가 
	-- T_DEPT_MASTER.C_INSERT_DATE 또는 C_UPDATE_DATE 보다 앞서는 업데이트
	-- 단, 대상은 C_INSERTED_BY_HR_YN가 'Y'인 조직
	UPDATE 
		T_DEPT_MASTER AS A
		LEFT OUTER JOIN T_TEMP_DEPT_MASTER AS B ON A.C_DEPT_ID = B.C_DEPT_ID 
		SET A.C_DEPT_NAME = B.C_DEPT_NAME, A.C_PARENT_ID = B.C_PARENT_ID, A.C_UPDATE_DATE = B.C_UPDATE_DATE 
	WHERE
		A.C_COMPANY_ID = V_COMPANY_ID 
		AND A.C_INSERTED_BY_HR_YN = 'Y' 
		AND B.C_UPDATE_DATE IS NOT NULL 
		AND B.C_UPDATE_DATE > (CASE WHEN A.C_UPDATE_DATE IS NULL THEN A.C_INSERT_DATE ELSE A.C_UPDATE_DATE END);
	
	-- 3rd: 조직 삽입 -------------------------------------------------------------
	-- (1) T_DEPT_MASTER에 존재하지 않으면서 T_TEMP_DEPT_MASTER에 있는 조직 삽입
	INSERT IGNORE INTO T_DEPT_MASTER 
	(C_COMPANY_ID, C_DEPT_ID, C_PARENT_ID, C_DEPT_NAME, C_INSERTED_BY_HR_YN, C_INSERT_DATE, C_UPDATE_DATE) 
	SELECT 
		V_COMPANY_ID, B.C_DEPT_ID, B.C_PARENT_ID, B.C_DEPT_NAME, 'Y', B.C_INSERT_DATE, B.C_UPDATE_DATE 
	FROM 
		T_TEMP_DEPT_MASTER AS B 
		LEFT OUTER JOIN T_DEPT_MASTER AS A ON B.C_DEPT_ID = A.C_DEPT_ID 
	WHERE 
		A.C_DEPT_ID IS NULL; 

	-- 4th: 예외처리
	-- (1) '부서미지정' 조직 삽입 (삭제된 경우 대비)
	INSERT IGNORE INTO T_DEPT_MASTER (C_COMPANY_ID, C_DEPT_ID, C_PARENT_ID, C_DEPT_NAME, C_INSERTED_BY_HR_YN, C_INSERT_DATE) VALUES (V_COMPANY_ID, 'NO_DEPT', NULL, '부서미지정', 'Y', UTC_TIMESTAMP());
	
END |
DELIMITER ; 

-- 사원 정보 동기화 (커스터마이징이 필요할 수 있는 프로시저여서, 기본 프로시저가 없을 경우에만 삽입하고, 있을 경우에는 갱신 안함.)
DELIMITER |
-- DROP PROCEDURE IF EXISTS `PRIMXDB`.`PROC_SYNC_HR_USER` |
CREATE PROCEDURE IF NOT EXISTS `PRIMXDB`.`PROC_SYNC_HR_USER`
(
	OUT OUT_USER_RESULT TINYINT(1) -- 결과 코드. (0: 실패 / 1: 성공)
)
BEGIN
	/*
	COMMENT: 사원 정보 동기화
	PARAMETERS:
		OUT OUT_USER_RESULT - 결과 코드. (0: 실패 / 1: 성공)
	HISTORY:
		2023-08-14 생성
	*/
	-- 기본 변수 선언
	DECLARE V_COMPANY_ID VARCHAR(50) DEFAULT '00000001';
	
	-- 예외처리 변수 선언
	DECLARE V_PROC_NAME      VARCHAR(256) DEFAULT 'PROC_SYNC_HR_USER';
	DECLARE V_SQL_STATE      VARCHAR(20) DEFAULT '';
	DECLARE V_INT_ERROR_NO   INT(11) DEFAULT 0;
	DECLARE V_PROC_COMMENT   VARCHAR(2048) DEFAULT '';
	DECLARE V_TEXT_ERROR_MSG TEXT;
	
	-- 예외처리
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
	BEGIN
		GET DIAGNOSTICS CONDITION 1 V_SQL_STATE = RETURNED_SQLSTATE
		, V_INT_ERROR_NO = MYSQL_ERRNO
		, V_TEXT_ERROR_MSG = MESSAGE_TEXT;
		SET V_PROC_COMMENT = '';
		INSERT T_SYSTEM_PROC_ERROR (C_ERROR_TIME, C_PROC_NAME, C_SQL_STATE, C_ERROR_NO, C_COMMENT, C_ERROR_MSG)
		VALUES (NOW(), V_PROC_NAME, V_SQL_STATE, V_INT_ERROR_NO, V_PROC_COMMENT, V_TEXT_ERROR_MSG);
		SET OUT_USER_RESULT = 0;
	END;
	
	-- 리턴값 초기화
	SET OUT_USER_RESULT = 1;
	-- 현재 Company ID 추출
	SELECT DISTINCT C_VALUE_DATA INTO V_COMPANY_ID FROM T_SERVER_CONFIG WHERE C_KEY_NAME = 'CompanyID';
	
	-- 1st: 사원 삭제 -------------------------------------------------------------
	-- (1) T_USER_MASTER에 존재하면서 T_TEMP_USER_MASTER에 없는 사원에 대해 C_ACTIVIE_YN을 'N'으로 변경
	-- 단, 대상은 C_INSERTED_BY_HR_YN가 'Y'면서 C_USER_TYPE이 0(일반사용자)인 것
	UPDATE 	
		T_USER_MASTER AS A 
		LEFT OUTER JOIN T_TEMP_USER_MASTER AS B ON A.C_USER_ID = B.C_USER_ID 
		SET A.C_USE_YN = 'N', A.C_ACTIVIE_YN = 'N'
	WHERE
		A.C_COMPANY_ID = V_COMPANY_ID 
		AND B.C_USER_ID IS NULL 
		AND A.C_ACTIVIE_YN = 'Y' 
		AND A.C_INSERTED_BY_HR_YN = 'Y' 
		AND A.C_USER_TYPE = 0;
	
	-- 2nd: 사원 업데이트 -----------------------------------------------------------
	-- (1) T_USER_MASTER에 T_TEMP_USER_MASTER에 동시에 존재하면서 T_TEMP_USER_MASTER에.C_UPDATE_DATE가 
	-- T_DEPT_MASTER.C_UPDATE_DATE 보다 앞서는 경우에 PK 제외한 정보 업데이트 (C_UPDATE_DATE 비교 업데이트)
	-- 단, 대상은 C_INSERTED_BY_HR_YN가 'Y'면서 C_USER_TYPE이 0(일반사용자)인 것
	UPDATE 
		T_USER_MASTER AS A 
		LEFT OUTER JOIN T_TEMP_USER_MASTER AS B ON A.C_USER_ID = B.C_USER_ID 
		SET A.C_USER_NAME = B.C_USER_NAME, A.C_USER_PWD = IFNULL(B.C_USER_PWD, '$2a$10$Dzx.LU5e4X78479pI5flf.e3xzO8j6xySwpSLvU0m6fK756zyBVWu'), A.C_DEPT_ID = B.C_DEPT_ID, 
		A.C_GRADE_NAME = B.C_GRADE_NAME, A.C_CELL_PHONE_NO = B.C_CELL_PHONE_NO, A.C_TEL_NO = B.C_TEL_NO, 
		A.C_EMAIL_ADDR = B.C_EMAIL_ADDR, A.C_EXT_NO = B.C_EXT_NO, A.C_USE_YN = 'Y', A.C_ACTIVIE_YN = 'Y', 
		A.C_INSERTED_BY_HR_YN = 'Y', A.C_UPDATE_DATE = B.C_UPDATE_DATE 
	WHERE 
		A.C_COMPANY_ID = V_COMPANY_ID 
		AND A.C_INSERTED_BY_HR_YN = 'Y' 
		AND A.C_USER_TYPE = 0 
		AND B.C_UPDATE_DATE IS NOT NULL 
		AND B.C_UPDATE_DATE > (CASE WHEN A.C_UPDATE_DATE IS NULL THEN A.C_INSERT_DATE ELSE A.C_UPDATE_DATE END);
	
	-- 3rd: 사원 삽입 -------------------------------------------------------------
	-- (1) T_USER_MASTER에 존재하지 않으면서 T_TEMP_USER_MASTER에 있는 사원 삽입
	INSERT INTO T_USER_MASTER 
	(C_COMPANY_ID, C_USER_ID, C_USER_NAME, C_USER_PWD, C_DEPT_ID, 
	C_GRADE_NAME, C_CELL_PHONE_NO, C_TEL_NO, C_EMAIL_ADDR, C_EXT_NO, 
	C_USE_YN, C_ACTIVIE_YN, C_INSERTED_BY_HR_YN, C_USER_TYPE, C_USER_ROLE, C_INSERT_DATE) 
	SELECT 
		V_COMPANY_ID, B.C_USER_ID, B.C_USER_NAME, IFNULL(B.C_USER_PWD, '$2a$10$Dzx.LU5e4X78479pI5flf.e3xzO8j6xySwpSLvU0m6fK756zyBVWu'), B.C_DEPT_ID, 
		B.C_GRADE_NAME, B.C_CELL_PHONE_NO, B.C_TEL_NO, B.C_EMAIL_ADDR, B.C_EXT_NO, 
		'Y', 'Y', 'Y', 0, 'User', B.C_INSERT_DATE 
	FROM 
		T_TEMP_USER_MASTER AS B 
		LEFT OUTER JOIN T_USER_MASTER AS A ON B.C_USER_ID = A.C_USER_ID 
	WHERE 
		A.C_USER_ID IS NULL 
	ON DUPLICATE KEY UPDATE 
		C_USER_NAME = B.C_USER_NAME, C_USER_PWD = IFNULL(B.C_USER_PWD, '$2a$10$Dzx.LU5e4X78479pI5flf.e3xzO8j6xySwpSLvU0m6fK756zyBVWu'), C_DEPT_ID = B.C_DEPT_ID, 
		C_GRADE_NAME = B.C_GRADE_NAME, C_CELL_PHONE_NO = B.C_CELL_PHONE_NO, C_TEL_NO = B.C_TEL_NO, C_EMAIL_ADDR = B.C_EMAIL_ADDR, C_EXT_NO = B.C_EXT_NO, 
		C_USE_YN = 'Y', C_ACTIVIE_YN = 'Y', C_INSERTED_BY_HR_YN = 'Y', C_UPDATE_DATE = B.C_UPDATE_DATE ; 
		
	-- 4th: 예외처리. 
	-- (1) '공용 PC', 'Guest PC' 레코드 삽입. (삭제된 경우 대비)
	-- INSERT IGNORE INTO T_USER_MASTER (C_COMPANY_ID, C_USER_ID, C_USER_NAME, C_USER_TYPE, C_DEPT_ID, C_INSERT_DATE) VALUES ('00000001', 'PUBLIC_PC', '공용 PC', 1, 'NO_DEPT', UTC_TIMESTAMP());
	INSERT IGNORE INTO T_USER_MASTER (C_COMPANY_ID, C_USER_ID, C_USER_NAME, C_USER_TYPE, C_DEPT_ID, C_INSERT_DATE) VALUES (V_COMPANY_ID, 'GUEST_PC', 'Guest PC', 1, 'NO_DEPT', UTC_TIMESTAMP());

END |
DELIMITER ; 

-- 조직, 사원 정보 동기화 (사용자 ID 입력, 리턴값 존재) (커스터마이징이 필요할 수 있는 프로시저여서, 기본 프로시저가 없을 경우에만 삽입하고, 있을 경우에는 갱신 안함.)
DELIMITER |
-- DROP PROCEDURE IF EXISTS `PRIMXDB`.`PROC_SYNC_HR_WITH_RETURN` |
CREATE PROCEDURE IF NOT EXISTS `PRIMXDB`.`PROC_SYNC_HR_WITH_RETURN`
(
	IN IN_USER_ID VARCHAR(50), -- 프로시저를 실행한 User ID
	OUT OUT_RESULT TINYINT(1)  -- 결과 코드. (-1: 다른 프로세스가 이 프로시저 실행 중 / 0: 실패 / 1: 성공)
)
BEGIN
	/*
	COMMENT: 조직, 사원 정보 동기화 (사용자 ID 입력, 리턴값 존재)
	PARAMETERS:
		IN IN_USER_ID  - 프로시저를 실행한 User ID
		OUT OUT_RESULT - 결과 코드. (-1: 다른 프로세스가 이 프로시저 실행 중 / 0: 실패 / 1: 성공)
	HISTORY:
		2023-08-14 생성
	*/
	-- 기본 변수 선언
	DECLARE V_OUT_DEPT_RESULT  TINYINT(1) DEFAULT 0;
	DECLARE V_OUT_USER_RESULT  TINYINT(1) DEFAULT 0;
	DECLARE V_CNT_USING_FLAG   INT(11) DEFAULT 0;
	DECLARE V_KEY_USING_FLAG   VARCHAR(50) DEFAULT 'UsingHRProc';
	DECLARE V_VALUE_USING_FLAG VARCHAR(200) DEFAULT 'NO';
	DECLARE V_RESERVED1        VARCHAR(260) DEFAULT 'SUCCESS';
	DECLARE V_LOG_TYPE         TINYINT(4) DEFAULT 1;	
	DECLARE V_RESULT_CODE      VARCHAR(10) DEFAULT 'SUCCESS';
	
	-- 예외처리 변수 선언
	DECLARE V_PROC_NAME      VARCHAR(256) DEFAULT 'PROC_SYNC_HR_WITH_RETURN';
	DECLARE V_SQL_STATE      VARCHAR(20) DEFAULT '';
	DECLARE V_INT_ERROR_NO   INT(11) DEFAULT 0;
	DECLARE V_PROC_COMMENT   VARCHAR(2048) DEFAULT '';
	DECLARE V_TEXT_ERROR_MSG TEXT;
	
	-- 예외처리
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
	BEGIN
		ROLLBACK;
		GET DIAGNOSTICS CONDITION 1 V_SQL_STATE = RETURNED_SQLSTATE
		, V_INT_ERROR_NO = MYSQL_ERRNO
		, V_TEXT_ERROR_MSG = MESSAGE_TEXT;
		SET V_PROC_COMMENT = CONCAT('USER_ID:', IN_USER_ID);
		INSERT T_SYSTEM_PROC_ERROR (C_ERROR_TIME, C_PROC_NAME, C_SQL_STATE, C_ERROR_NO, C_COMMENT, C_ERROR_MSG)
		VALUES (NOW(), V_PROC_NAME, V_SQL_STATE, V_INT_ERROR_NO, V_PROC_COMMENT, V_TEXT_ERROR_MSG);
		
		INSERT INTO T_MANAGE_LOG (C_LOG_TIME, C_LOG_CODE, C_LOG_TYPE, C_USER_ID, C_RESULT_CODE, C_RESERVED1, C_COMMENT) 
		VALUES (UTC_TIMESTAMP(), 'S10001', 1, IN_USER_ID, 'E10002', 'FAIL', 'sql exception occured');
		
		UPDATE T_SERVER_INTERNAL_KV SET C_VALUE_DATA = 'NO', C_UPDATE_DATE = NOW() WHERE C_KEY_NAME = V_KEY_USING_FLAG;
		
		SET OUT_RESULT = 0;
	END;
	
	SET OUT_RESULT = 1;
	SELECT COUNT(*), C_VALUE_DATA INTO V_CNT_USING_FLAG, V_VALUE_USING_FLAG FROM T_SERVER_INTERNAL_KV WHERE C_KEY_NAME = V_KEY_USING_FLAG;
	
	IF V_CNT_USING_FLAG = 0 OR V_VALUE_USING_FLAG = 'NO' 
	THEN
		INSERT INTO T_SERVER_INTERNAL_KV (C_KEY_NAME, C_VALUE_DATA, C_INSERT_DATE) VALUES (V_KEY_USING_FLAG, 'YES', NOW()) 
		ON DUPLICATE KEY UPDATE C_VALUE_DATA = 'YES', C_UPDATE_DATE = UTC_TIMESTAMP();
	
		IF LENGTH(IN_USER_ID) = 0 THEN SET IN_USER_ID = 'SYSTEM'; END IF;
		IF IN_USER_ID != 'SYSTEM' THEN SET V_LOG_TYPE = 0; END IF;
		
		START TRANSACTION;
		CALL `PRIMXDB`.`PROC_SYNC_HR_DEPT`(V_OUT_DEPT_RESULT);
		
		IF V_OUT_DEPT_RESULT = 1 THEN
			CALL `PRIMXDB`.`PROC_SYNC_HR_USER`(V_OUT_USER_RESULT);
		END IF;
		
		IF V_OUT_DEPT_RESULT = 0 OR V_OUT_USER_RESULT = 0 THEN
			SET V_RESULT_CODE = 'E10002';
			SET V_RESERVED1 = 'FAIL';
			SET OUT_RESULT = 0;
			ROLLBACK;
		ELSE 
			COMMIT;
		END IF;
		
		INSERT INTO T_MANAGE_LOG (C_LOG_TIME, C_LOG_CODE, C_LOG_TYPE, C_USER_ID, C_RESULT_CODE, C_RESERVED1) 
		VALUES (UTC_TIMESTAMP(), 'S10001', V_LOG_TYPE, IN_USER_ID, V_RESULT_CODE, V_RESERVED1);
		
		UPDATE T_SERVER_INTERNAL_KV SET C_VALUE_DATA = 'NO', C_UPDATE_DATE = NOW() WHERE C_KEY_NAME = V_KEY_USING_FLAG;
	ELSE 
		SET OUT_RESULT = -1;
	END IF; 
END |
DELIMITER ; 

-- 조직, 사원 정보 동기화 (리턴값 없음) (커스터마이징이 필요할 수 있는 프로시저여서, 기본 프로시저가 없을 경우에만 삽입하고, 있을 경우에는 갱신 안함.)
DELIMITER |
-- DROP PROCEDURE IF EXISTS `PRIMXDB`.`PROC_SYNC_HR_NO_RETURN` |
CREATE PROCEDURE IF NOT EXISTS `PRIMXDB`.`PROC_SYNC_HR_NO_RETURN`()
BEGIN
	/*
	COMMENT: 조직, 사원 정보 동기화 (리턴값 없음)
	HISTORY:
		2023-08-14 생성
	*/
	-- 기본 변수 선언
	DECLARE IN_USER_ID VARCHAR(50) DEFAULT ''; -- 'SYSTEM'
	DECLARE OUT_RESULT TINYINT(1) DEFAULT 0;
	
	CALL PROC_SYNC_HR_WITH_RETURN(IN_USER_ID, OUT_RESULT);
END |
DELIMITER ; 

/* ----------------------------------------------------------------------------
	PROCEDURES - STATISTICS
---------------------------------------------------------------------------- */ 
-- T_MANAGE_LOG 테이블의 중간 통계 (T_MANAGE_LOG_STAT 테이블에 삽입)
DELIMITER |
DROP PROCEDURE IF EXISTS `PRIMXDB`.`PROC_STAT_T_MANAGE_LOG` |
CREATE PROCEDURE `PRIMXDB`.`PROC_STAT_T_MANAGE_LOG`
(
	IN IN_DATE VARCHAR(20) -- 통계일자. 'YYYYMMDD' 또는 'YESTERDAY' 
)
BEGIN
	/*
	COMMENT: T_MANAGE_LOG 테이블의 중간 통계 (T_MANAGE_LOG_STAT 테이블에 삽입)
	PARAMETERS:
		IN_DATE: 통계일자. 'YYYYMMDD' 또는 'YESTERDAY' 
	HISTORY:
		2023-09-01 생성
	*/
	
	-- 기본 변수 선언
	DECLARE V_HMS_STR_START VARCHAR(20) DEFAULT '000000';
	DECLARE V_HMS_STR_END   VARCHAR(20) DEFAULT '235959';
	DECLARE V_YMD_STR       VARCHAR(20) DEFAULT '';
	DECLARE V_CUR_DATE      DATETIME;
	
	-- 예외처리 변수 선언
	DECLARE V_PROC_NAME      VARCHAR(256) DEFAULT 'PROC_STAT_T_MANAGE_LOG';
	DECLARE V_SQL_STATE      VARCHAR(20) DEFAULT '';
	DECLARE V_INT_ERROR_NO   INT(11) DEFAULT 0;
	DECLARE V_PROC_COMMENT   VARCHAR(2048) DEFAULT '';
	DECLARE V_TEXT_ERROR_MSG TEXT;
	
	-- 예외처리
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
	BEGIN
		ROLLBACK;
	
		GET DIAGNOSTICS CONDITION 1 V_SQL_STATE = RETURNED_SQLSTATE
		, V_INT_ERROR_NO = MYSQL_ERRNO
		, V_TEXT_ERROR_MSG = MESSAGE_TEXT;
		SET V_PROC_COMMENT = CONCAT('DATE:', IN_DATE);		
		
		INSERT T_SYSTEM_PROC_ERROR (C_ERROR_TIME, C_PROC_NAME, C_SQL_STATE, C_ERROR_NO, C_COMMENT, C_ERROR_MSG)
		VALUES (NOW(), V_PROC_NAME, V_SQL_STATE, V_INT_ERROR_NO, V_PROC_COMMENT, V_TEXT_ERROR_MSG);
	END;
	
	IF IN_DATE = 'YESTERDAY' THEN 
		SET V_YMD_STR = DATE_FORMAT(DATE_ADD(NOW(), INTERVAL -1 DAY), '%Y%m%d');
	ELSE 
		SET V_YMD_STR = IN_DATE;
	END IF;
	
	SET V_CUR_DATE = STR_TO_DATE(CONCAT(V_YMD_STR, V_HMS_STR_START), '%Y%m%d%H%i%s');
	
	START TRANSACTION;
	INSERT INTO T_MANAGE_LOG_STAT 
	(C_STAT_DATE, C_USER_ID, C_USER_NAME, C_LOG_CODE, C_RESULT, C_SUM) 
	SELECT 
		STAT.CUR_DATE, 
		STAT.USER_ID, 
		STAT.USER_NAME, 
		STAT.LOG_CODE, 
		STAT.RESULT, 
		STAT.SUMMARY 
	FROM 
	(
		SELECT 
			V_CUR_DATE AS CUR_DATE, 
			C_USER_ID AS USER_ID, 
			C_USER_NAME AS USER_NAME, 
			C_LOG_CODE AS LOG_CODE, 
			(CASE 
			WHEN C_RESULT_CODE IS NULL THEN NULL 
			WHEN C_RESULT_CODE = 'SUCCESS' THEN TRUE 
			ELSE FALSE 
			END) AS RESULT, 
			COUNT(C_SEQ_NO) AS SUMMARY
		FROM 
			T_MANAGE_LOG
		WHERE 
			C_LOG_TIME BETWEEN STR_TO_DATE(CONCAT(V_YMD_STR, V_HMS_STR_START), '%Y%m%d%H%i%s') AND STR_TO_DATE(CONCAT(V_YMD_STR, V_HMS_STR_END), '%Y%m%d%H%i%s')
			AND LENGTH(IFNULL(C_USER_ID, '')) > 0 
		GROUP BY C_USER_ID, C_USER_NAME, C_LOG_CODE, C_RESULT_CODE 
		ORDER BY C_USER_ID, C_USER_NAME, C_LOG_CODE, C_RESULT_CODE
	) AS STAT
	ON DUPLICATE KEY UPDATE 
	C_SUM = STAT.SUMMARY;
	COMMIT;
END |
DELIMITER ; 

-- T_EX_FILE_AUTH_LOG 테이블의 중간 통계 (T_EX_FILE_AUTH_LOG_STAT 테이블에 삽입)
DELIMITER |
DROP PROCEDURE IF EXISTS `PRIMXDB`.`PROC_STAT_T_EX_FILE_AUTH_LOG` |
CREATE PROCEDURE `PRIMXDB`.`PROC_STAT_T_EX_FILE_AUTH_LOG`
(
	IN IN_DATE VARCHAR(20) -- 통계일자. 'YYYYMMDD' 또는 'YESTERDAY' 
)
BEGIN
	/*
	COMMENT: T_EX_FILE_AUTH_LOG 테이블의 중간 통계 (T_EX_FILE_AUTH_LOG_STAT 테이블에 삽입)
	PARAMETERS:
		IN_DATE: 통계일자. 'YYYYMMDD' 또는 'YESTERDAY' 
	HISTORY:
		2023-09-01 생성
	*/
	
	-- 기본 변수 선언
	DECLARE V_HMS_STR_START VARCHAR(20) DEFAULT '000000';
	DECLARE V_HMS_STR_END   VARCHAR(20) DEFAULT '235959';
	DECLARE V_YMD_STR       VARCHAR(20) DEFAULT '';
	DECLARE V_CUR_DATE      DATETIME;
	
	-- 예외처리 변수 선언
	DECLARE V_PROC_NAME      VARCHAR(256) DEFAULT 'PROC_STAT_T_EX_FILE_AUTH_LOG';
	DECLARE V_SQL_STATE      VARCHAR(20) DEFAULT '';
	DECLARE V_INT_ERROR_NO   INT(11) DEFAULT 0;
	DECLARE V_PROC_COMMENT   VARCHAR(2048) DEFAULT '';
	DECLARE V_TEXT_ERROR_MSG TEXT;
	
	-- 예외처리
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
	BEGIN
		ROLLBACK;
	
		GET DIAGNOSTICS CONDITION 1 V_SQL_STATE = RETURNED_SQLSTATE
		, V_INT_ERROR_NO = MYSQL_ERRNO
		, V_TEXT_ERROR_MSG = MESSAGE_TEXT;
		SET V_PROC_COMMENT = CONCAT('DATE:', IN_DATE);		
		
		INSERT T_SYSTEM_PROC_ERROR (C_ERROR_TIME, C_PROC_NAME, C_SQL_STATE, C_ERROR_NO, C_COMMENT, C_ERROR_MSG)
		VALUES (NOW(), V_PROC_NAME, V_SQL_STATE, V_INT_ERROR_NO, V_PROC_COMMENT, V_TEXT_ERROR_MSG);
	END;
	
	IF IN_DATE = 'YESTERDAY' THEN 
		SET V_YMD_STR = DATE_FORMAT(DATE_ADD(NOW(), INTERVAL -1 DAY), '%Y%m%d');
	ELSE 
		SET V_YMD_STR = IN_DATE;
	END IF;
	
	SET V_CUR_DATE = STR_TO_DATE(CONCAT(V_YMD_STR, V_HMS_STR_START), '%Y%m%d%H%i%s');
	
	START TRANSACTION;
	INSERT INTO T_EX_FILE_AUTH_LOG_STAT 
	(C_STAT_DATE, C_FILE_KEY, C_RESULT, C_SUM) 
	SELECT 
		STAT.CUR_DATE, 
		STAT.C_FILE_KEY, 
		STAT.RESULT, 
		STAT.SUMMARY 
	FROM 
	(
		SELECT 
			V_CUR_DATE AS CUR_DATE, 
			C_FILE_KEY, 
			(CASE 
			WHEN C_RESULT_CODE IS NULL THEN NULL 
			WHEN C_RESULT_CODE = 'SUCCESS' THEN TRUE 
			ELSE FALSE 
			END) AS RESULT, 
			COUNT(C_SEQ_NO) AS SUMMARY
		FROM 
			T_EX_FILE_AUTH_LOG
		WHERE
			C_LOG_TIME BETWEEN STR_TO_DATE(CONCAT(V_YMD_STR, V_HMS_STR_START), '%Y%m%d%H%i%s') AND STR_TO_DATE(CONCAT(V_YMD_STR, V_HMS_STR_END), '%Y%m%d%H%i%s')
			AND LENGTH(IFNULL(C_FILE_KEY, '')) > 0
		GROUP BY C_FILE_KEY, RESULT
		ORDER BY C_FILE_KEY ASC
	) AS STAT
	ON DUPLICATE KEY UPDATE 
	C_SUM = STAT.SUMMARY;
	COMMIT;
END |
DELIMITER ; 

/* ----------------------------------------------------------------------------
	PROCEDURES - FOR SCHEDULER
---------------------------------------------------------------------------- */ 
-- 인사 연동 스케줄러 구동용 프로시저
DELIMITER | 
DROP PROCEDURE IF EXISTS `PRIMXDB`.`PROC_FOR_SCH_CHECK_AND_RUN_HRSYNC` | 
CREATE PROCEDURE `PRIMXDB`.`PROC_FOR_SCH_CHECK_AND_RUN_HRSYNC` 
( 
) 
BEGIN 
	/* 
	COMMENT: 인사 연동 스케줄러 구동용 프로시저
	HISTORY:
		2023-09-05 생성
	*/
	-- 기본 변수 선언
	DECLARE V_HRIMPORTYN       VARCHAR(200) DEFAULT 'N';
	DECLARE V_HRIMPORTTYPE     VARCHAR(200) DEFAULT '';
	DECLARE V_HRIMPORTRUNTIME  VARCHAR(200) DEFAULT '';
	DECLARE V_IS_CHECKED       TINYINT(1)   DEFAULT TRUE;
	DECLARE V_IS_RUN           TINYINT(1)   DEFAULT FALSE;
	DECLARE V_CNT_LAST_RUNTIME INT(11)      DEFAULT 0;
	DECLARE V_LAST_RUN_TIME    VARCHAR(200) DEFAULT '';
	DECLARE V_CURRENT_TIME     DATETIME;
	DECLARE V_VALUE_RUN_HOUR   INT(11)      DEFAULT 0;
	DECLARE V_CHECK_RUNTIME    TINYINT(1)   DEFAULT FALSE;
	DECLARE V_VALUE_RUN_DOW    INT(11)      DEFAULT 0;
	DECLARE V_COMMENT          VARCHAR(200) DEFAULT '';
	
	-- 예외처리 변수 선언
	DECLARE V_PROC_NAME      VARCHAR(256) DEFAULT 'PROC_FOR_SCH_CHECK_AND_RUN_HRSYNC';
	DECLARE V_SQL_STATE      VARCHAR(20) DEFAULT '';
	DECLARE V_INT_ERROR_NO   INT(11) DEFAULT 0;
	DECLARE V_PROC_COMMENT   VARCHAR(2048) DEFAULT '';
	DECLARE V_TEXT_ERROR_MSG TEXT;
	
	-- 예외처리
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
	BEGIN
		GET DIAGNOSTICS CONDITION 1 V_SQL_STATE = RETURNED_SQLSTATE
		, V_INT_ERROR_NO = MYSQL_ERRNO
		, V_TEXT_ERROR_MSG = MESSAGE_TEXT;
		SET V_PROC_COMMENT = '';
		INSERT T_SYSTEM_PROC_ERROR (C_ERROR_TIME, C_PROC_NAME, C_SQL_STATE, C_ERROR_NO, C_COMMENT, C_ERROR_MSG)
		VALUES (NOW(), V_PROC_NAME, V_SQL_STATE, V_INT_ERROR_NO, V_PROC_COMMENT, V_TEXT_ERROR_MSG);
	END;
	
	--  설정값 추출
	SELECT FUNC_GET_SERVER_CONFIG('HRImportYN'), FUNC_GET_SERVER_CONFIG('HRImportType'), FUNC_GET_SERVER_CONFIG('HRImportRunTime')
	INTO V_HRIMPORTYN, V_HRIMPORTTYPE, V_HRIMPORTRUNTIME;
	
	IF  V_HRIMPORTYN      = 'INVALID_VALUE' OR V_HRIMPORTYN      = 'NOT_FOUND_KEY' OR 
		V_HRIMPORTTYPE    = 'INVALID_VALUE' OR V_HRIMPORTTYPE    = 'NOT_FOUND_KEY' OR 
		V_HRIMPORTRUNTIME = 'INVALID_VALUE' OR V_HRIMPORTRUNTIME = 'NOT_FOUND_KEY' 
	THEN
		SET V_IS_CHECKED = FALSE;
	END IF;
	
	IF V_HRIMPORTYN != 'Y' THEN SET V_IS_CHECKED = FALSE; END IF;
	
	-- 현재 시각 추출
	SET V_CURRENT_TIME = NOW();
	
	IF V_IS_CHECKED = TRUE THEN
		-- 이전 마지막 실행시각 추출
		SELECT COUNT(*), IFNULL(C_VALUE_DATA, '') 
		INTO V_CNT_LAST_RUNTIME, V_LAST_RUN_TIME 
		FROM T_SERVER_INTERNAL_KV 
		WHERE C_KEY_NAME = 'LastRunTime_HRImport';

		-- 매일 특정시각 
		IF V_HRIMPORTTYPE = 'DAILY' THEN 
			SELECT V_HRIMPORTRUNTIME REGEXP('^[0-9]+$') INTO V_CHECK_RUNTIME;
			IF V_CHECK_RUNTIME = FALSE THEN SET V_IS_CHECKED = FALSE; END IF;
			IF CAST(V_HRIMPORTRUNTIME AS INTEGER) > 23 THEN SET V_IS_CHECKED = FALSE; END IF;
			
			-- 이전 구동시각과 날짜가 같으면 Skip
			IF ( YEAR(V_CURRENT_TIME) = YEAR(V_LAST_RUN_TIME) AND MONTH(V_CURRENT_TIME) = MONTH(V_LAST_RUN_TIME) AND DAY(V_CURRENT_TIME) = DAY(V_LAST_RUN_TIME) ) THEN SET V_IS_CHECKED = FALSE; END IF;
			
			IF V_IS_CHECKED = TRUE THEN				
				SET V_VALUE_RUN_HOUR = CAST(V_HRIMPORTRUNTIME AS INTEGER);				
				IF V_CNT_LAST_RUNTIME = 0 THEN 
					IF HOUR(V_CURRENT_TIME) = V_VALUE_RUN_HOUR THEN 
						SET V_IS_RUN = TRUE;
						SET V_COMMENT = CONCAT('[DAILY] FIRST RUN. RUN TIME : ', DATE_FORMAT(V_CURRENT_TIME, '%Y-%m-%d %H:%i:%s'));						
					END IF;
				ELSE				
					-- 설정된 시각과 현재 시각이 같은 경우 실행
					IF HOUR(V_CURRENT_TIME) = V_VALUE_RUN_HOUR THEN 
						SET V_IS_RUN = TRUE;				
						SET V_COMMENT = CONCAT('[DAILY] NEXT RUN. RUN TIME : ', DATE_FORMAT(V_CURRENT_TIME, '%Y-%m-%d %H:%i:%s'));							
					END IF;
				END IF;
			END IF;
		-- 매주 특정요일 특정시각
		ELSEIF V_HRIMPORTTYPE = 'WEEKLY' THEN 
			SELECT V_HRIMPORTRUNTIME REGEXP('^MON|TUE|WED|THU|FRI|SAT|SUN/[0-9]+$') INTO V_CHECK_RUNTIME;
			IF V_CHECK_RUNTIME = FALSE THEN SET V_IS_CHECKED = FALSE; END IF;
			IF CAST(SUBSTRING(V_HRIMPORTRUNTIME, 5, 2) AS INTEGER) > 23 THEN SET V_IS_CHECKED = FALSE; END IF;
			SET V_VALUE_RUN_DOW = (CASE SUBSTRING(V_HRIMPORTRUNTIME, 1, 3) 
									WHEN 'SUN' THEN 1 
									WHEN 'MON' THEN 2 
									WHEN 'TUE' THEN 3
									WHEN 'WED' THEN 4 
									WHEN 'THU' THEN 5
									WHEN 'FRI' THEN 6
									WHEN 'SAT' THEN 7
									ELSE 0
									END);
			IF V_VALUE_RUN_DOW = 0 THEN SET V_IS_CHECKED = FALSE; END IF;
			
			-- 이전 구동시각과 날짜가 같으면 Skip
			IF ( YEAR(V_CURRENT_TIME) = YEAR(V_LAST_RUN_TIME) AND MONTH(V_CURRENT_TIME) = MONTH(V_LAST_RUN_TIME) AND DAY(V_CURRENT_TIME) = DAY(V_LAST_RUN_TIME) ) THEN SET V_IS_CHECKED = FALSE; END IF;
			
			IF V_IS_CHECKED = TRUE THEN	
				IF DAYOFWEEK(V_CURRENT_TIME) = V_VALUE_RUN_DOW THEN
					SET V_VALUE_RUN_HOUR = CAST(SUBSTRING(V_HRIMPORTRUNTIME, 5, 2) AS INTEGER);
					IF V_CNT_LAST_RUNTIME = 0 THEN 
						IF HOUR(V_CURRENT_TIME) = V_VALUE_RUN_HOUR THEN 
							SET V_IS_RUN = TRUE;				
							SET V_COMMENT = CONCAT('[WEEKLY] FIRST RUN. RUN TIME : ', DATE_FORMAT(V_CURRENT_TIME, '%Y-%m-%d %H:%i:%s'));
						END IF;
					ELSE
						-- 설정된 시각과 현재 시각이 같은 경우 실행
						IF HOUR(V_CURRENT_TIME) = V_VALUE_RUN_HOUR THEN 
							SET V_IS_RUN = TRUE;				
							SET V_COMMENT = CONCAT('[WEEKLY] NEXT RUN. RUN TIME : ', DATE_FORMAT(V_CURRENT_TIME, '%Y-%m-%d %H:%i:%s'));								
						END IF;
					END IF;
				END IF;
			END IF;
		-- 매월 특정일 특정시각
		ELSEIF V_HRIMPORTTYPE = 'MONTHLY' THEN
			IF INSTR(V_HRIMPORTRUNTIME, '/') = 0 THEN 
				SET V_IS_CHECKED = FALSE; 
			ELSEIF CAST(LEFT(V_HRIMPORTRUNTIME, INSTR(V_HRIMPORTRUNTIME, '/') - 1) AS INTEGER) > 28 THEN 
				SET V_IS_CHECKED = FALSE; 
			ELSEIF CAST(LEFT(V_HRIMPORTRUNTIME, INSTR(V_HRIMPORTRUNTIME, '/') - 1) AS INTEGER) = 0 THEN 
				SET V_IS_CHECKED = FALSE; 
			ELSEIF CAST(RIGHT(V_HRIMPORTRUNTIME, LENGTH(V_HRIMPORTRUNTIME) - INSTR(V_HRIMPORTRUNTIME, '/')) AS INTEGER) > 23 THEN 
				SET V_IS_CHECKED = FALSE; 
			END IF;
			
			-- 이전 구동시각과 날짜가 같으면 Skip
			IF ( YEAR(V_CURRENT_TIME) = YEAR(V_LAST_RUN_TIME) AND MONTH(V_CURRENT_TIME) = MONTH(V_LAST_RUN_TIME) AND DAY(V_CURRENT_TIME) = DAY(V_LAST_RUN_TIME) ) THEN SET V_IS_CHECKED = FALSE; END IF;
			
			IF V_IS_CHECKED = TRUE THEN	
				IF DAY(V_CURRENT_TIME) = CAST(LEFT(V_HRIMPORTRUNTIME, INSTR(V_HRIMPORTRUNTIME, '/') - 1) AS INTEGER) THEN
					SET V_VALUE_RUN_HOUR = CAST(RIGHT(V_HRIMPORTRUNTIME, LENGTH(V_HRIMPORTRUNTIME) - INSTR(V_HRIMPORTRUNTIME, '/')) AS INTEGER);
					IF V_CNT_LAST_RUNTIME = 0 THEN 
						IF HOUR(V_CURRENT_TIME) = V_VALUE_RUN_HOUR THEN 
							SET V_IS_RUN = TRUE;					
							SET V_COMMENT = CONCAT('[MONTHLY] FIRST RUN. RUN TIME : ', DATE_FORMAT(V_CURRENT_TIME, '%Y-%m-%d %H:%i:%s'));
						END IF;
					ELSE
						-- 설정된 시각과 현재 시각이 같은 경우 실행
						IF HOUR(V_CURRENT_TIME) = V_VALUE_RUN_HOUR THEN 
							SET V_IS_RUN = TRUE;				
							SET V_COMMENT = CONCAT('[MONTHLY] NEXT RUN. RUN TIME : ', DATE_FORMAT(V_CURRENT_TIME, '%Y-%m-%d %H:%i:%s'));
						END IF;
					END IF;
				END IF;
			END IF;
		END IF;
	END IF;
	
	IF V_IS_RUN = TRUE THEN 
		INSERT INTO T_SERVER_INTERNAL_KV (C_KEY_NAME, C_VALUE_DATA, C_INSERT_DATE) VALUE ('LastRunTime_HRImport', V_CURRENT_TIME, V_CURRENT_TIME)
		ON DUPLICATE KEY UPDATE C_VALUE_DATA = V_CURRENT_TIME, C_UPDATE_DATE = V_CURRENT_TIME;
		
		CALL PROC_SYNC_HR_NO_RETURN();
		-- TODO: for debugging
		-- INSERT INTO T_MANAGE_LOG (C_LOG_TIME, C_LOG_CODE, C_LOG_TYPE, C_USER_ID, C_RESULT_CODE, C_COMMENT) 
		-- VALUES (UTC_TIMESTAMP(), 'S10001', 1, 'SYSTEM', 'SUCCESS', V_COMMENT);
	END IF;
END | 
DELIMITER ; 

-- 일정 시간 이상 Keep Alive가 없는 시스템 정보 삭제 스케줄러용 프로시저
DELIMITER | 
DROP PROCEDURE IF EXISTS `PRIMXDB`.`PROC_FOR_SCH_CHECK_AND_DELETE_OFFLINE_SYSTEM_INFO` | 
CREATE PROCEDURE `PRIMXDB`.`PROC_FOR_SCH_CHECK_AND_DELETE_OFFLINE_SYSTEM_INFO` 
( 
) 
BEGIN 
	/* 
	COMMENT: 일정 시간 이상 Keep Alive가 없는 시스템 정보 삭제 스케줄러용 프로시저 
	HISTORY:
		2023-09-09 생성
	*/
	-- 기본 변수 선언
	DECLARE V_SYSTEMINFO_AUTO_DELETE_DAY_LIMIT VARCHAR(200) DEFAULT '';
	DECLARE V_SYSTEMINFO_AUTO_DELETE_RUN_TIME  VARCHAR(200) DEFAULT '';
	DECLARE V_CURRENT_TIME                     DATETIME;
	DECLARE V_CNT_LAST_RUNTIME                 INT(11)      DEFAULT 0;
	DECLARE V_LAST_RUN_TIME                    VARCHAR(200) DEFAULT '';
	DECLARE V_DAY_LIMIT                        INT(11)      DEFAULT 0;
	DECLARE V_VALUE_RUN_HOUR                   INT(11)      DEFAULT 0;
	DECLARE V_IS_CHECKED                       TINYINT(1)   DEFAULT TRUE;
	DECLARE V_IS_RUN                           TINYINT(1)   DEFAULT FALSE;
	
	-- 예외처리 변수 선언
	DECLARE V_PROC_NAME      VARCHAR(256) DEFAULT 'PROC_FOR_SCH_CHECK_AND_DELETE_OFFLINE_SYSTEM_INFO';
	DECLARE V_SQL_STATE      VARCHAR(20) DEFAULT '';
	DECLARE V_INT_ERROR_NO   INT(11) DEFAULT 0;
	DECLARE V_PROC_COMMENT   VARCHAR(2048) DEFAULT '';
	DECLARE V_TEXT_ERROR_MSG TEXT;
	
	-- 예외처리
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
	BEGIN
		GET DIAGNOSTICS CONDITION 1 V_SQL_STATE = RETURNED_SQLSTATE
		, V_INT_ERROR_NO = MYSQL_ERRNO
		, V_TEXT_ERROR_MSG = MESSAGE_TEXT;
		SET V_PROC_COMMENT = '';
		INSERT T_SYSTEM_PROC_ERROR (C_ERROR_TIME, C_PROC_NAME, C_SQL_STATE, C_ERROR_NO, C_COMMENT, C_ERROR_MSG)
		VALUES (NOW(), V_PROC_NAME, V_SQL_STATE, V_INT_ERROR_NO, V_PROC_COMMENT, V_TEXT_ERROR_MSG);
	END;
	
	--  설정값 추출
	SELECT FUNC_GET_SERVER_CONFIG('SystemInfoAutoDeleteDayLimit'), FUNC_GET_SERVER_CONFIG('SystemInfoAutoDeleteRunTime') 
	INTO V_SYSTEMINFO_AUTO_DELETE_DAY_LIMIT, V_SYSTEMINFO_AUTO_DELETE_RUN_TIME;
	
	IF  V_SYSTEMINFO_AUTO_DELETE_DAY_LIMIT = 'INVALID_VALUE' OR V_SYSTEMINFO_AUTO_DELETE_DAY_LIMIT = 'NOT_FOUND_KEY' OR 
		V_SYSTEMINFO_AUTO_DELETE_RUN_TIME  = 'INVALID_VALUE' OR V_SYSTEMINFO_AUTO_DELETE_RUN_TIME  = 'NOT_FOUND_KEY'
	THEN
		SET V_IS_CHECKED = FALSE;
	END IF;

	IF V_IS_CHECKED = TRUE 
	THEN 
		SET V_DAY_LIMIT      = CAST(V_SYSTEMINFO_AUTO_DELETE_DAY_LIMIT AS INTEGER); -- 회수기간이 0인 경우, 삭제안함. 
		SET V_VALUE_RUN_HOUR = CAST(V_SYSTEMINFO_AUTO_DELETE_RUN_TIME AS INTEGER);
				
		IF V_DAY_LIMIT > 0 AND V_VALUE_RUN_HOUR < 24 
		THEN
			SET V_CURRENT_TIME = NOW();

			-- 이전 마지막 실행시각 추출
			SELECT COUNT(*), IFNULL(C_VALUE_DATA, '') 
			INTO V_CNT_LAST_RUNTIME, V_LAST_RUN_TIME 
			FROM T_SERVER_INTERNAL_KV 
			WHERE C_KEY_NAME = 'LastRunTime_SystemInfoDelete';
			
			IF V_CNT_LAST_RUNTIME = 0 THEN
				IF HOUR(V_CURRENT_TIME) = V_VALUE_RUN_HOUR THEN 
					SET V_IS_RUN = TRUE;
				END IF;
			ELSE
				-- 이전 구동시각과 날짜가 같으면 Skip
				IF !( YEAR(V_CURRENT_TIME) = YEAR(V_LAST_RUN_TIME) AND MONTH(V_CURRENT_TIME) = MONTH(V_LAST_RUN_TIME) AND DAY(V_CURRENT_TIME) = DAY(V_LAST_RUN_TIME) ) THEN 
					-- 설정된 시각과 현재 시각이 같은 경우 실행
					IF HOUR(V_CURRENT_TIME) = V_VALUE_RUN_HOUR THEN 
						SET V_IS_RUN = TRUE;						
					END IF;
				END IF;
			END IF;
			
			IF V_IS_RUN = TRUE THEN 
				INSERT INTO T_SERVER_INTERNAL_KV (C_KEY_NAME, C_VALUE_DATA, C_INSERT_DATE) VALUE ('LastRunTime_SystemInfoDelete', V_CURRENT_TIME, V_CURRENT_TIME)
				ON DUPLICATE KEY UPDATE C_VALUE_DATA = V_CURRENT_TIME, C_UPDATE_DATE = V_CURRENT_TIME;
				
				-- SystemInfoAutoDeleteDayLimit(일단위)를 경과한 에이전트 삭제 처리
				UPDATE T_SYSTEM_MASTER SET C_USE_YN = 'N', C_MODIFIED_BY = 'SYSTEM', C_MODIFIED_IP = '127.0.0.1', C_UPDATE_DATE = UTC_TIMESTAMP() 
				WHERE C_USE_YN = 'Y' AND C_LAST_KA_DATE < (UTC_TIMESTAMP() - INTERVAL V_DAY_LIMIT DAY);
				
				-- SystemInfoAutoDeleteDayLimit 설정이 변경된 경우를 대비해, 기존 삭제 처리된 에이전트 가운데 정상상태로 변경되어야 할 경우에 예외처리
				UPDATE T_SYSTEM_MASTER SET C_USE_YN = 'Y', C_MODIFIED_BY = 'SYSTEM', C_MODIFIED_IP = '127.0.0.1', C_UPDATE_DATE = UTC_TIMESTAMP() 
				WHERE C_USE_YN = 'N' AND C_LAST_KA_DATE > (UTC_TIMESTAMP() - INTERVAL V_DAY_LIMIT DAY);
			END IF;
		END IF;
	END IF;
END | 
DELIMITER ; 

-- 일정 시간 이상 Keep Alive가 없는 시스템을 Offline으로 변경하는 스케줄러용 프로시저 
DELIMITER | 
DROP PROCEDURE IF EXISTS `PRIMXDB`.`PROC_FOR_SCH_CHECK_AND_UPDATE_OFFLINE_SYSTEM_INFO` | 
CREATE PROCEDURE `PRIMXDB`.`PROC_FOR_SCH_CHECK_AND_UPDATE_OFFLINE_SYSTEM_INFO` 
( 
) 
BEGIN 
	/* 
	COMMENT: 일정 시간 이상 Keep Alive가 없는 시스템을 Offline으로 변경하는 스케줄러용 프로시저 
	HISTORY:
		2023-09-09 생성
	*/
	-- 기본 변수 선언
	DECLARE V_AGENT_OFFLINE_MINUTE_LIMIT VARCHAR(200) DEFAULT '';
	DECLARE V_MINUTE_LIMIT               INT(11)      DEFAULT 0;
	DECLARE V_IS_CHECKED                 TINYINT(1)   DEFAULT TRUE;
	DECLARE V_IS_RUN                     TINYINT(1)   DEFAULT FALSE;
	DECLARE V_CURRENT_TIME               DATETIME;
	
	-- 예외처리 변수 선언
	DECLARE V_PROC_NAME      VARCHAR(256) DEFAULT 'PROC_FOR_SCH_CHECK_AND_UPDATE_OFFLINE_SYSTEM_INFO';
	DECLARE V_SQL_STATE      VARCHAR(20) DEFAULT '';
	DECLARE V_INT_ERROR_NO   INT(11) DEFAULT 0;
	DECLARE V_PROC_COMMENT   VARCHAR(2048) DEFAULT '';
	DECLARE V_TEXT_ERROR_MSG TEXT;
	
	-- 예외처리
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
	BEGIN
		GET DIAGNOSTICS CONDITION 1 V_SQL_STATE = RETURNED_SQLSTATE
		, V_INT_ERROR_NO = MYSQL_ERRNO
		, V_TEXT_ERROR_MSG = MESSAGE_TEXT;
		SET V_PROC_COMMENT = '';
		INSERT T_SYSTEM_PROC_ERROR (C_ERROR_TIME, C_PROC_NAME, C_SQL_STATE, C_ERROR_NO, C_COMMENT, C_ERROR_MSG)
		VALUES (NOW(), V_PROC_NAME, V_SQL_STATE, V_INT_ERROR_NO, V_PROC_COMMENT, V_TEXT_ERROR_MSG);
	END;
	
	--  설정값 추출
	SELECT FUNC_GET_SERVER_CONFIG('AgentOfflineMinuteLimit') INTO V_AGENT_OFFLINE_MINUTE_LIMIT;
	IF  V_AGENT_OFFLINE_MINUTE_LIMIT = 'INVALID_VALUE' OR V_AGENT_OFFLINE_MINUTE_LIMIT = 'NOT_FOUND_KEY'
	THEN
		SET V_IS_CHECKED = FALSE;
	END IF;
	
	IF V_IS_CHECKED = TRUE 
	THEN
		SET V_MINUTE_LIMIT = CAST(V_AGENT_OFFLINE_MINUTE_LIMIT AS INTEGER); -- 0인 경우, 동작안함. 
		IF V_MINUTE_LIMIT > 0
		THEN
			SET V_CURRENT_TIME = NOW();
		
			INSERT INTO T_SERVER_INTERNAL_KV (C_KEY_NAME, C_VALUE_DATA, C_INSERT_DATE) VALUE ('LastRunTime_UpdateSystemOffline', V_CURRENT_TIME, V_CURRENT_TIME)
			ON DUPLICATE KEY UPDATE C_VALUE_DATA = V_CURRENT_TIME, C_UPDATE_DATE = V_CURRENT_TIME;
		
			-- AgentOfflineMinuteLimit(분단위)를 경과한 에이전트 OFFLINE 처리
			UPDATE T_SYSTEM_MASTER SET C_AGENT_STATE = 2, C_MODIFIED_BY = 'SYSTEM', C_MODIFIED_IP = '127.0.0.1', C_UPDATE_DATE = UTC_TIMESTAMP() 
			WHERE C_AGENT_STATE = 1 AND C_LAST_KA_DATE < (UTC_TIMESTAMP() - INTERVAL V_MINUTE_LIMIT MINUTE);
			
			-- AgentOfflineMinuteLimit 설정이 변경된 경우를 대비해, 기존 OFFLINE 처리된 에이전트 가운데 ONLINE으로 변경되어야 할 경우에 예외처리
			UPDATE T_SYSTEM_MASTER SET C_AGENT_STATE = 1, C_MODIFIED_BY = 'SYSTEM', C_MODIFIED_IP = '127.0.0.1', C_UPDATE_DATE = UTC_TIMESTAMP() 
			WHERE C_AGENT_STATE = 2 AND C_LAST_KA_DATE > (UTC_TIMESTAMP() - INTERVAL V_MINUTE_LIMIT MINUTE);
		END IF;
	END IF;	
END | 
DELIMITER ; 

-- 일정 기간 경과된 로그를 삭제하는 스케줄러용 프로시저 
DELIMITER | 
DROP PROCEDURE IF EXISTS `PRIMXDB`.`PROC_FOR_SCH_CHECK_AND_DELETE_OLD_LOG` | 
CREATE PROCEDURE `PRIMXDB`.`PROC_FOR_SCH_CHECK_AND_DELETE_OLD_LOG` 
( 
) 
BEGIN 
	/* 
	COMMENT: 일정 기간 경과된 로그를 삭제하는 스케줄러용 프로시저 
	HISTORY:
		2023-09-09 생성
	*/
	-- 기본 변수 선언
	DECLARE V_OLD_LOG_DELETE_MONTH_LIMIT VARCHAR(200) DEFAULT '';
	DECLARE V_MONTH_LIMIT                INT(11)      DEFAULT 0;
	DECLARE V_IS_CHECKED                 TINYINT(1)   DEFAULT TRUE;
	DECLARE V_IS_RUN                     TINYINT(1)   DEFAULT FALSE;
	DECLARE V_CURRENT_TIME               DATETIME;
	
	-- 예외처리 변수 선언
	DECLARE V_PROC_NAME      VARCHAR(256) DEFAULT 'PROC_FOR_SCH_CHECK_AND_DELETE_OLD_LOG';
	DECLARE V_SQL_STATE      VARCHAR(20) DEFAULT '';
	DECLARE V_INT_ERROR_NO   INT(11) DEFAULT 0;
	DECLARE V_PROC_COMMENT   VARCHAR(2048) DEFAULT '';
	DECLARE V_TEXT_ERROR_MSG TEXT;
	
	-- 예외처리
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
	BEGIN
		GET DIAGNOSTICS CONDITION 1 V_SQL_STATE = RETURNED_SQLSTATE
		, V_INT_ERROR_NO = MYSQL_ERRNO
		, V_TEXT_ERROR_MSG = MESSAGE_TEXT;
		SET V_PROC_COMMENT = '';
		INSERT T_SYSTEM_PROC_ERROR (C_ERROR_TIME, C_PROC_NAME, C_SQL_STATE, C_ERROR_NO, C_COMMENT, C_ERROR_MSG)
		VALUES (NOW(), V_PROC_NAME, V_SQL_STATE, V_INT_ERROR_NO, V_PROC_COMMENT, V_TEXT_ERROR_MSG);
	END;
	
	--  설정값 추출
	SELECT FUNC_GET_SERVER_CONFIG('OldLogDeleteMonthLimit') INTO V_OLD_LOG_DELETE_MONTH_LIMIT;
	IF  V_OLD_LOG_DELETE_MONTH_LIMIT = 'INVALID_VALUE' OR V_OLD_LOG_DELETE_MONTH_LIMIT = 'NOT_FOUND_KEY'
	THEN
		SET V_IS_CHECKED = FALSE;
	END IF;
	
	IF V_IS_CHECKED = TRUE 
	THEN
		SET V_MONTH_LIMIT = CAST(V_OLD_LOG_DELETE_MONTH_LIMIT AS INTEGER); -- 0인 경우, 동작안함. 
		IF V_MONTH_LIMIT > 0
		THEN
			SET V_CURRENT_TIME = NOW();
		
			INSERT INTO T_SERVER_INTERNAL_KV (C_KEY_NAME, C_VALUE_DATA, C_INSERT_DATE) VALUE ('LastRunTime_DeleteOlgLog', V_CURRENT_TIME, V_CURRENT_TIME)
			ON DUPLICATE KEY UPDATE C_VALUE_DATA = V_CURRENT_TIME, C_UPDATE_DATE = V_CURRENT_TIME;			
		
			DELETE FROM T_AGENT_PRODUCTS_LOG WHERE C_LOG_TIME < (UTC_TIMESTAMP() - INTERVAL V_MONTH_LIMIT MONTH);
			DELETE FROM T_CONSRV_LOG WHERE LOG_DT < (UTC_TIMESTAMP() - INTERVAL V_MONTH_LIMIT MONTH);
			DELETE FROM T_MANAGE_LOG WHERE C_LOG_TIME < (UTC_TIMESTAMP() - INTERVAL V_MONTH_LIMIT MONTH);
			DELETE FROM T_MANAGE_LOG_STAT WHERE C_STAT_DATE < (UTC_TIMESTAMP() - INTERVAL V_MONTH_LIMIT MONTH);	
			DELETE FROM T_EX_FILE_AUTH_LOG WHERE C_LOG_TIME < (UTC_TIMESTAMP() - INTERVAL V_MONTH_LIMIT MONTH);			
		END IF;
	END IF;	
END | 
DELIMITER ; 

/* ----------------------------------------------------------------------------
	SCHEDULERS
---------------------------------------------------------------------------- */ 
-- 10분 주기로 T_SYSTEM_PROC_ERROR 테이블에서 최근 1000개의 레코드만 남기고 자동 삭제하는 스케줄러
DROP EVENT IF EXISTS SCH_DELETE_T_SYSTEM_PROC_ERROR_OLD_RECORD;
CREATE EVENT IF NOT EXISTS SCH_DELETE_T_SYSTEM_PROC_ERROR_OLD_RECORD 
    ON SCHEDULE EVERY 10 MINUTE
    DO 
    DELETE `PRIMXDB`.`T_SYSTEM_PROC_ERROR` 
	FROM `PRIMXDB`.`T_SYSTEM_PROC_ERROR` 
	INNER JOIN 
	(
		SELECT 
			C_ERROR_TIME, 
			@ROWNUM := @ROWNUM + 1 AS ROWNUM
		FROM 
			`PRIMXDB`.`T_SYSTEM_PROC_ERROR`
			CROSS JOIN (SELECT @ROWNUM := 0) AS VAR 
		ORDER BY C_ERROR_TIME DESC
	)
	AS RN ON `PRIMXDB`.`T_SYSTEM_PROC_ERROR`.C_ERROR_TIME = RN.C_ERROR_TIME AND RN.ROWNUM > 1000;

-- 매일 새벽 1시에 전날 T_MANAGE_LOG 테이블을 통계연산하여 T_MANAGE_LOG_STAT 테이블로 삽입하는 스케줄러
DROP EVENT IF EXISTS SCH_INSERT_T_MANAGE_LOG_STAT;
CREATE EVENT IF NOT EXISTS SCH_INSERT_T_MANAGE_LOG_STAT 
    ON SCHEDULE EVERY 1 DAY STARTS STR_TO_DATE(CONCAT(DATE_FORMAT(DATE_ADD(NOW(), INTERVAL 1 DAY), '%Y-%m-%d'), ' 01:00:00'), '%Y-%m-%d %H:%i:%s')
    DO 
    CALL `PRIMXDB`.`PROC_STAT_T_MANAGE_LOG`('YESTERDAY');
	
-- 매일 새벽 2시에 전날 T_EX_FILE_AUTH_LOG 테이블을 통계연산하여 T_EX_FILE_AUTH_LOG_STAT 테이블로 삽입하는 스케줄러
DROP EVENT IF EXISTS SCH_INSERT_T_EX_FILE_AUTH_LOG_STAT;
CREATE EVENT IF NOT EXISTS SCH_INSERT_T_EX_FILE_AUTH_LOG_STAT 
    ON SCHEDULE EVERY 1 DAY STARTS STR_TO_DATE(CONCAT(DATE_FORMAT(DATE_ADD(NOW(), INTERVAL 1 DAY), '%Y-%m-%d'), ' 02:00:00'), '%Y-%m-%d %H:%i:%s')
    DO 
    CALL `PRIMXDB`.`PROC_STAT_T_EX_FILE_AUTH_LOG`('YESTERDAY');

-- 1시간 주기로 인사연동 동작 여부 확인 및 구동하는 스케줄러
DROP EVENT IF EXISTS SCH_CHECK_AND_RUN_HRSYNC;
CREATE EVENT IF NOT EXISTS SCH_CHECK_AND_RUN_HRSYNC 
    ON SCHEDULE EVERY 1 HOUR
    DO 
	CALL `PRIMXDB`.`PROC_FOR_SCH_CHECK_AND_RUN_HRSYNC`();

-- 1시간 주기로 일정 시간 이상 Keep Alive가 없는 시스템 정보 삭제하는 스케줄러
DROP EVENT IF EXISTS SCH_CHECK_AND_RUN_DELETE_OFFLINE_SYSTEM_INFO;
CREATE EVENT IF NOT EXISTS SCH_CHECK_AND_RUN_DELETE_OFFLINE_SYSTEM_INFO 
    ON SCHEDULE EVERY 1 HOUR
    DO 
	CALL `PRIMXDB`.`PROC_FOR_SCH_CHECK_AND_DELETE_OFFLINE_SYSTEM_INFO`();
	
-- 1시간 주기로 일정 시간 이상 Keep Alive가 없는 시스템을 Offline으로 변경하는 스케줄러
DROP EVENT IF EXISTS SCH_CHECK_AND_RUN_UPDATE_OFFLINE_SYSTEM_INFO;
CREATE EVENT IF NOT EXISTS SCH_CHECK_AND_RUN_UPDATE_OFFLINE_SYSTEM_INFO 
    ON SCHEDULE EVERY 1 HOUR
    DO 
	CALL `PRIMXDB`.`PROC_FOR_SCH_CHECK_AND_UPDATE_OFFLINE_SYSTEM_INFO`();
	
-- 1일 주기로 일정 기간 경과된 로그를 삭제하는 스케줄러
DROP EVENT IF EXISTS SCH_CHECK_AND_RUN_DELETE_OLD_LOG;
CREATE EVENT IF NOT EXISTS SCH_CHECK_AND_RUN_DELETE_OLD_LOG 
    ON SCHEDULE EVERY 1 DAY
    DO 
	CALL `PRIMXDB`.`PROC_FOR_SCH_CHECK_AND_DELETE_OLD_LOG`();