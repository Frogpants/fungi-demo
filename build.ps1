# ---------------- CONFIG ----------------

$wslPath  = "/home/spenc/mystuff/fungi-demo"
$winPath  = "C:\Users\spenc\projects\fungi-demo"
$makeExe  = "C:\msys64\mingw64\bin\mingw32-make.exe"

# ----------------------------------------

Write-Host "---------------------------------"
Write-Host "Syncing WSL -> Windows..."
Write-Host "---------------------------------"

wsl rsync -av --delete $wslPath/ /mnt/c/Users/spenc/projects/fungi-demo/

if ($LASTEXITCODE -ne 0) {
    Write-Host "Sync failed."
    exit $LASTEXITCODE
}

Write-Host "---------------------------------"
Write-Host "Building..."
Write-Host "---------------------------------"

& $makeExe -C $winPath clean
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

& $makeExe -C $winPath
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

Write-Host "---------------------------------"
Write-Host "Running..."
Write-Host "---------------------------------"

& $makeExe -C $winPath run
