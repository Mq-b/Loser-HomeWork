# 获取环境变量并处理空值情况
$allChangedFiles = @($env:ALL_CHANGED_FILES -split ';' | Where-Object { $_ -ne "" })

# 使用 HashSet 提高查找性能
$changedFileSet = [System.Collections.Generic.HashSet[string]]::new([StringComparer]::OrdinalIgnoreCase)

foreach ($file in $allChangedFiles) {
    try {
        $absolutePath = Convert-Path $file -ErrorAction Stop
        [void]$changedFileSet.Add($absolutePath)
    }
    catch {
        Write-Warning "无法找到文件: $file"
    }
}

# 查找目标路径下的所有 C++ 源文件
$cppFiles = Get-ChildItem -Path $env:TARGET_PATH -Recurse -File -Include '*.cpp', '*.cxx', '*.cc'

foreach ($cppFile in $cppFiles) {
    if (-not $changedFileSet.Contains($cppFile.FullName)) {
        try {
            Remove-Item $cppFile.FullName -ErrorAction Stop
            Write-Host "已删除文件: $($cppFile.FullName)"
        }
        catch {
            Write-Warning "无法删除文件: $($cppFile.FullName) - $_"
        }
    }
}