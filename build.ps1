$ErrorActionPreference = "Stop"

$buildDir = "build"
$output = Join-Path $buildDir "arkanoid.exe"

if (-not (Test-Path $buildDir)) {
    New-Item -ItemType Directory -Path $buildDir | Out-Null
}

$sources = Get-ChildItem -Path "src" -Filter "*.cpp" | ForEach-Object { $_.FullName }

g++ -std=c++17 -Wall -Wextra -O2 $sources -o $output -lraylib -lopengl32 -lgdi32 -lwinmm

if ($LASTEXITCODE -ne 0) {
    throw "Build failed. Please confirm raylib is installed for MSYS2 UCRT64."
}

Write-Host "Build complete: $output"
