[CmdletBinding()]
param (
    [Parameter(Mandatory=$true)]
    [string]$Execution,
    [Parameter(Mandatory=$true)]
    [string]$OutputName,
    [string]$ExecInput,
    [Parameter(Mandatory=$true)]
    [string]$CorrectExecOutput
)

$OutputFile = "$OutputName.txt"
if (Test-Path $ExecInput) {
Start-Process -FilePath $Execution -Wait -RedirectStandardOutput $OutputFile -RedirectStandardInput $ExecInput
} else {
Start-Process -FilePath $Execution -Wait -RedirectStandardOutput $OutputFile
}

if (-not (Test-Path -Path $OutputFile -PathType leaf)) {
    Write-Output "output file not found"
    exit 1
}

try {
    $comparison = Compare-Object -ReferenceObject (Get-Content $CorrectExecOutput) -DifferenceObject (Get-Content $OutputFile)
    if ($comparison.Count -eq 0) {
        Write-Output "check result: same"
    } else {
        Write-Output "check result: diff"
        Write-Host "Current Output:"
        Get-Content -Path $OutputFile
        Write-Host "Correct Output:"
        Get-Content -Path $CorrectExecOutput
        exit 1
    }
}
catch {
    Write-Output "Error: $_"
    exit 1
}