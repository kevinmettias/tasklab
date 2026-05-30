param(
    [Parameter(ValueFromRemainingArguments = $true)]
    [string[]] $Command
)

$ErrorActionPreference = "Stop"

$msys_root = "C:\msys64"
$ucrt_bin = Join-Path $msys_root "ucrt64\bin"
$usr_bin = Join-Path $msys_root "usr\bin"
$vcpkg_root = if ($env:VCPKG_ROOT) { $env:VCPKG_ROOT } else { "C:\dev\vcpkg" }

$env:VCPKG_ROOT = $vcpkg_root
$env:Path = "$ucrt_bin;$usr_bin;$env:Path"

if ($Command.Count -eq 0)
{
    Write-Host "tasklab local development environment"
    gcc --version
    cmake --version
    make --version
    if (Get-Command pkg-config -ErrorAction SilentlyContinue)
    {
        pkg-config --modversion cmocka
    }
    exit $LASTEXITCODE
}

$program = $Command[0]
$arguments = @()
if ($Command.Count -gt 1)
{
    $arguments = $Command[1..($Command.Count - 1)]
}

& $program @arguments
exit $LASTEXITCODE

