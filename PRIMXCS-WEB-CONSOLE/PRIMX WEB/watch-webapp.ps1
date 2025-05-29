# watch-webapp.ps1

$src = "D:\eGovFrameDev-4.2.0-64bit\workspace\PRIMX\src\main\webapp"
$dest = "D:\eGovFrameDev-4.2.0-64bit\workspace\PRIMX\target\ROOT"

$watcher = New-Object System.IO.FileSystemWatcher $src, "*.*"
$watcher.IncludeSubdirectories = $true
$watcher.EnableRaisingEvents = $true

# 공통 처리 함수
function Handle-Change($path) {
    $ext = [System.IO.Path]::GetExtension($path).ToLower()
    if ($ext -in @(".js", ".jsp", ".css", ".html", "json")) {
        $relative = $path.Substring($src.Length).TrimStart('\')
        $target = Join-Path $dest $relative
        $targetDir = Split-Path $target

        Write-Host "`n[Detected] $path"
        Write-Host "[Target   ] $target"

        if (!(Test-Path $targetDir)) {
            New-Item -ItemType Directory -Path $targetDir -Force | Out-Null
        }

        try {
            Copy-Item $path $target -Force
            Write-Host "[Copied   ] $relative"
        } catch {
            Write-Host "[Error    ] $($_.Exception.Message)"
        }
    }
}

# 이벤트 등록
Register-ObjectEvent $watcher Changed -SourceIdentifier WatchChange
Register-ObjectEvent $watcher Created -SourceIdentifier WatchCreate
Register-ObjectEvent $watcher Renamed -SourceIdentifier WatchRename

Write-Host "Watching for changes in: $src"
Write-Host "Press Ctrl+C to stop."

# 이벤트 루프 실행
while ($true) {
    $e = Wait-Event -Timeout 1
    if ($e) {
        Handle-Change $e.SourceEventArgs.FullPath
        Remove-Event -SourceIdentifier $e.SourceIdentifier
    }
}
