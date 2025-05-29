# setup-env.ps1
# 전체 환경 초기화: OpenSSL + 환경변수 + 인증서 + Tomcat + MariaDB + 네트워크 + 확인 쿼리 + SQL 초기화

# 0. 기본 변수 설정
$sslDir = "$PSScriptRoot\mariadb-ssl"
$opensslPath = "C:\Program Files\OpenSSL-Win64\bin"
$mycnfPath = "$sslDir\my.cnf"
$policyFile = "$PSScriptRoot\PRIMXDB_policy.sql"
$ddlFile = "$PSScriptRoot\primxdb_create 4.sql"
$ddl2File = "$PSScriptRoot\com_DDL_maria.sql"
$dmlFile = "$PSScriptRoot\com_DML_maria.sql"
$copyTargetDir = "D:\eGovFrameDev-4.2.0-64bit\workspace\PRIMX\src\main\resources\ssl\docker"

# 1. 디렉토리 준비
New-Item -ItemType Directory -Path $sslDir -Force | Out-Null
Set-Location $sslDir

# 2. OpenSSL 설치 여부 확인
if (-not (Get-Command openssl -ErrorAction SilentlyContinue)) {
    Write-Host "OpenSSL not found. Install from https://slproweb.com/products/Win32OpenSSL.html"
    return
}

# 3. 환경변수 등록
$currentPath = [Environment]::GetEnvironmentVariable("Path", "User")
if ($currentPath -notlike "*$opensslPath*") {
    Write-Host "Registering OpenSSL to user PATH..."
    $newPath = "$currentPath;$opensslPath"
    [Environment]::SetEnvironmentVariable("Path", $newPath, "User")
}

# 4. 기존 파일 정리
Remove-Item "$sslDir\*" -Force -ErrorAction SilentlyContinue

# 5. 인증서 생성
Write-Host "Generating SSL certificates..."
Set-Location $sslDir

openssl genrsa -out ca-key.pem 2048
openssl req -new -x509 -nodes -days 3650 -key ca-key.pem -out ca.pem -subj "/CN=MySQL_SSL_CA"

openssl req -newkey rsa:2048 -days 3650 -nodes -keyout server-key.pem -out server-req.pem -subj "/CN=MySQL_Server"
openssl x509 -req -in server-req.pem -days 3650 -CA ca.pem -CAkey ca-key.pem -set_serial 01 -out server-cert.pem

openssl req -newkey rsa:2048 -days 3650 -nodes -keyout client-key.pem -out client-req.pem -subj "/CN=MySQL_Client"
openssl x509 -req -in client-req.pem -days 3650 -CA ca.pem -CAkey ca-key.pem -set_serial 01 -out client-cert.pem

# 5-1. 인증서 파일 존재 확인
if (!(Test-Path "$sslDir\ca.pem") -or !(Test-Path "$sslDir\server-cert.pem") -or !(Test-Path "$sslDir\server-key.pem")) {
    Write-Host "Certificate creation failed: missing ca.pem or server cert/key. Check OpenSSL execution."
    exit
}

# 5-2. 인증서 복사
if (!(Test-Path $copyTargetDir)) {
    New-Item -ItemType Directory -Path $copyTargetDir -Force | Out-Null
}
Copy-Item "$sslDir\ca.pem" -Destination "$copyTargetDir\ca.pem" -Force
Copy-Item "$sslDir\server-cert.pem" -Destination "$copyTargetDir\server-cert.pem" -Force
Copy-Item "$sslDir\server-key.pem" -Destination "$copyTargetDir\server-key.pem" -Force
Copy-Item "$sslDir\client-cert.pem" -Destination "$copyTargetDir\client-cert.pem" -Force
Copy-Item "$sslDir\client-key.pem" -Destination "$copyTargetDir\client-key.pem" -Force

# 6. my.cnf 생성
if (Test-Path $mycnfPath) {
    Remove-Item $mycnfPath -Force
}

[System.IO.File]::WriteAllLines($mycnfPath, @(
    "[mysqld]",
    "ssl-ca=/etc/mysql/ssl/ca.pem",
    "ssl-cert=/etc/mysql/ssl/server-cert.pem",
    "ssl-key=/etc/mysql/ssl/server-key.pem"
))

# 7. 도커 네트워크 생성
docker network create egov-net 2>$null

# 8. 기존 컨테이너 제거
docker rm -f egov-mysql 2>$null
docker rm -f egov-tomcat 2>$null

# 9. MariaDB 컨테이너 실행
Write-Host "Starting MariaDB with SSL..."
docker run -d --name egov-mysql `
  --network egov-net `
  -e MYSQL_ROOT_PASSWORD=1234 `
  -e MYSQL_DATABASE=PRIMXDB `
  -p 53306:3306 `
  mariadb:10.5.27

Start-Sleep -Seconds 10

# 9-1. SSL 인증서 복사
docker exec egov-mysql mkdir -p /etc/mysql/ssl

docker cp "$sslDir\ca.pem" egov-mysql:/etc/mysql/ssl/ca.pem
if ($LASTEXITCODE -ne 0) { Write-Host "Failed to copy ca.pem"; exit }

docker cp "$sslDir\server-cert.pem" egov-mysql:/etc/mysql/ssl/server-cert.pem
if ($LASTEXITCODE -ne 0) { Write-Host "Failed to copy server-cert.pem"; exit }

docker cp "$sslDir\server-key.pem" egov-mysql:/etc/mysql/ssl/server-key.pem
if ($LASTEXITCODE -ne 0) { Write-Host "Failed to copy server-key.pem"; exit }

# 9-2. 설정 복사 및 권한
if (Test-Path $mycnfPath) {
    docker cp "$mycnfPath" egov-mysql:/etc/mysql/conf.d/ssl.cnf
    docker exec egov-mysql chmod 644 /etc/mysql/conf.d/ssl.cnf
}

# 9-3. MariaDB 재시작
Write-Host "Restarting MariaDB..."
docker restart egov-mysql
Start-Sleep -Seconds 10

# 10. Tomcat 컨테이너 실행
Write-Host "Starting Tomcat..."
docker run -d --name egov-tomcat `
  --network egov-net `
  -p 50010:8080 `
  tomcat:9-jdk11

# 11. MariaDB SSL 상태 확인
Write-Host "Checking MariaDB SSL status..."
Start-Sleep -Seconds 10
$sslStatus = docker exec egov-mysql mysql -uroot -p1234 -e "SHOW VARIABLES LIKE 'have_ssl';" 2>&1
Write-Host "--- SSL Status ---"
Write-Host $sslStatus

# 12. SQL 파일 실행
Write-Host "Running schema and policy scripts..."

if (Test-Path $policyFile) {
    docker cp "$policyFile" egov-mysql:/policy.sql
    docker exec egov-mysql sh -c "mysql -uroot -p1234 PRIMXDB < /policy.sql"
} else {
    Write-Host "Policy script not found."
}

if (Test-Path $ddl2File) {
    docker cp "$ddl2File" egov-mysql:/ddl2.sql
    docker exec egov-mysql sh -c "mysql -uroot -p1234 PRIMXDB < /ddl2.sql"
} else {
    Write-Host "Second DDL script not found."
}

if (Test-Path $ddlFile) {
    docker cp "$ddlFile" egov-mysql:/ddl.sql
    docker exec egov-mysql sh -c "mysql -uroot -p1234 PRIMXDB < /ddl.sql"
} else {
    Write-Host "DDL script not found."
}

if (Test-Path $dmlFile) {
    docker cp "$dmlFile" egov-mysql:/dml.sql
    docker exec egov-mysql sh -c "mysql -uroot -p1234 PRIMXDB < /dml.sql"
} else {
    Write-Host "DML script not found."
}

Write-Host "Environment setup completed."
