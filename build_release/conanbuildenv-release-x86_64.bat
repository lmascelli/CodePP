@echo off
chcp 65001 > nul
setlocal
echo @echo off > "%~dp0/deactivate_conanbuildenv-release-x86_64.bat"
echo echo Restoring environment >> "%~dp0/deactivate_conanbuildenv-release-x86_64.bat"
for %%v in (PATH) do (
    set foundenvvar=
    for /f "delims== tokens=1,2" %%a in ('set') do (
        if /I "%%a" == "%%v" (
            echo set "%%a=%%b">> "%~dp0/deactivate_conanbuildenv-release-x86_64.bat"
            set foundenvvar=1
        )
    )
    if not defined foundenvvar (
        echo set %%v=>> "%~dp0/deactivate_conanbuildenv-release-x86_64.bat"
    )
)
endlocal


set "PATH=E:\unige\CodePP\devel\conan\p\ninjae2ad385cd85df\p\bin;E:\unige\CodePP\devel\conan\p\cmake615d6e26ece18\p\bin;%PATH%"