-- 1. 사용자 생성
CREATE USER 'primx_user'@'%' IDENTIFIED BY 'primxuser1234~!';

-- 2. 권한 부여 (PRIMXDB 전체 권한)
GRANT ALL PRIVILEGES ON PRIMXDB.* TO 'primx_user'@'%';

-- 3. 권한 적용
FLUSH PRIVILEGES;

