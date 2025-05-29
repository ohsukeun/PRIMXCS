# deploy-war.ps1
# 목적: 빌드된 WAR 파일을 Tomcat 컨테이너로 복사하고 자동 배포

# 설정
$warFile = "D:\eGovFrameDev-4.2.0-64bit\workspace\PRIMX\target\egovframework-all-in-one.war"
$containerName = "egov-tomcat"
$webappsPath = "/usr/local/tomcat/webapps"

# 0. Maven 빌드 실행
Write-Host "Building WAR with Maven..."
Push-Location "D:\eGovFrameDev-4.2.0-64bit\workspace\PRIMX"
mvn clean install
if ($LASTEXITCODE -ne 0) {
    Write-Host "Maven build failed."
    Pop-Location
    exit 1
}
Pop-Location

# 1. WAR 파일 존재 확인
if (!(Test-Path $warFile)) {
    Write-Host "WAR file not found: $warFile"
    exit 1
}

# 2. Tomcat 컨테이너 실행 여부 확인
$containerRunning = docker ps --format "{{.Names}}" | Where-Object { $_ -eq $containerName }
if (-not $containerRunning) {
    Write-Host "Tomcat container '$containerName' is not running."
    exit 1
}

# 3. 기존 WAR 및 디렉토리 제거
Write-Host "Removing old deployment..."
docker exec $containerName rm -rf "$webappsPath/ROOT"
docker exec $containerName rm -f "$webappsPath/ROOT.war"

# 4. WAR 파일 복사 및 배포
Write-Host "Deploying WAR as ROOT.war..."
$targetPath = $containerName + ":" + $webappsPath + "/ROOT.war"
docker cp $warFile $targetPath

# 5. Tomcat 자동 배포 대기
Start-Sleep -Seconds 10

Write-Host "WAR deployment complete. Access at http://localhost:50010/"

# 6. 실시간 로그 확인 (또는 tail로 변경 가능)
Write-Host "`n--- Tomcat Logs ---"
docker logs -f $containerName