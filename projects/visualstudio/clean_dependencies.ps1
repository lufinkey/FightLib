#!C:/WINDOWS/System32/WindowsPowerShell/v1.0/powershell -file
param([string]$FightLibPath="")

$msbuild_path=(Resolve-Path HKLM:\SOFTWARE\MICROSOFT\MSBuild\ToolsVersions\14.0 | Get-ItemProperty -Name MSBuildToolsPath).MSBuildToolsPath
$msbuild_path="$msbuild_path\msbuild.exe"

echo "cleaning Box2D"
$b2d_path="$FightLibPath\external\Box2D\Build\vs2017"
& "$msbuild_path" "$b2d_path\Box2D.sln" /t:Clean

echo "Windows is a garbage OS" | Out-File "$FightLibPath\projects\visualstudio\dependency_clean.log"
