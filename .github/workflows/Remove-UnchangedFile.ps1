$allChangedFiles = $env:ALL_CHANGED_FILES
$allChangedFiles = $allChangedFiles -replace '\\', ''
$allChangedFiles = $allChangedFiles -split ';'

$absolutePaths = @()
foreach ($file in $allChangedFiles) {
    $absolutePath = Convert-Path $file
    $absolutePaths += $absolutePath
}

$cppFiles = Get-ChildItem -Path $env:src_path -Recurse -File -Include *.cpp, *.cxx, *.cc
foreach ($cppFile in $cppFiles) {
    if ($absolutePaths -NotContains $cppFile.FullName) {
        Remove-Item $cppFile.FullName
    }
}