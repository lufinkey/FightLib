#!C:/WINDOWS/System32/WindowsPowerShell/v1.0/powershell -file
param([string]$FightLibPath="", [string]$Config = "Debug", [string]$Platform = "Win32", [string]$PlatformToolset = "v140", [string]$ToolsVersion = "14.0")

$exitvalue=0

$msbuild_path=(Resolve-Path HKLM:\SOFTWARE\MICROSOFT\MSBuild\ToolsVersions\14.0 | Get-ItemProperty -Name MSBuildToolsPath).MSBuildToolsPath
$msbuild_path="$msbuild_path\msbuild.exe"

$lib_path="$FightLibPath\projects\visualstudio\lib\$Platform\$Config"
mkdir -p "$lib_path"

#Compile Box2D
$b2d_path="$FightLibPath\external\Box2D\Box2D\Build\vs2013"
(Get-Content "$b2d_path\Box2D.vcxproj").replace(
	'<TreatWarningAsError>true</TreatWarningAsError>', '<TreatWarningAsError>false</TreatWarningAsError>') |
	Set-Content "$b2d_path\Box2D.vcxproj"
& "$msbuild_path" /t:Box2D "$b2d_path\Box2D.sln" "/p:PlatformToolset=$PlatformToolset" "/p:Configuration=$Config" "/p:Platform=$Platform" "/ToolsVersion:$ToolsVersion"
$b2d_platform="$Platform"
if($Platform -eq "Win32" -Or $Platform -eq "x86")
{
	$b2d_platform="x32"
}
$b2d_output_path="$b2d_path\bin\$b2d_platform\$Config"
if(Test-Path "$b2d_output_path\Box2D.lib")
{
	copy -Path "$b2d_output_path\Box2D.lib" -Destination "$lib_path\Box2D.lib" -Force
}
else
{
	echo "Could not find Box2D.lib"
	$exitvalue=1
}

exit $exitvalue
