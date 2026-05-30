$ErrorActionPreference = "Stop"
$repo_root = Split-Path -Parent $PSScriptRoot
$dev_env = Join-Path $PSScriptRoot "dev-env.ps1"
function Invoke-Checked {
    param(
        [Parameter(Mandatory = $true)]
        [string[]] $Command
    )
    & $dev_env @Command
    if ($LASTEXITCODE -ne 0)
    {
        throw "Command failed with exit code ${LASTEXITCODE}: $($Command -join ' ')"
    }
}
$preset = if ($IsWindows) { "debug-vcpkg" } else { "debug" }
Push-Location $repo_root
try {
    Invoke-Checked -Command @("cmake", "--preset", $preset)
    Invoke-Checked -Command @("cmake", "--build", "--preset", $preset)
    Invoke-Checked -Command @("ctest", "--preset", $preset, "--output-on-failure")
} finally {
    Pop-Location
}