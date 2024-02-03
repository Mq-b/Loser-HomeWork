$allChangedFiles = $env:ALL_CHANGED_FILES
$allChangedFiles = $allChangedFiles -split ';'

$absolutePaths = @()
foreach ($file in $allChangedFiles) {
    if ($file -ne "") {
        $absolutePath = Convert-Path $file
        $absolutePaths += $absolutePath
    }
}

$cppFiles = Get-ChildItem -Path $env:TARGET_PATH -Recurse -File -Include *.cpp, *.cxx, *.cc
foreach ($cppFile in $cppFiles) {
    if ($absolutePaths -NotContains $cppFile.FullName) {
        Remove-Item $cppFile.FullName
    }
}