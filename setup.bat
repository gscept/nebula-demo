@ECHO OFF
setlocal
pushd "%~dp0"
echo Checking required software
where /q python
IF ERRORLEVEL 1 ( 
    ECHO No Python executable found in path
    pause
    EXIT /B 1
)
echo Getting initial dependencies
call fips set config win64-vstudio-debug
call fips fetch
echo Compiling external components
call fips nebula set work %~dp0
call fips nebula set toolkit %~dp0\..\nebula
call fips physx build vc17 debug
call fips anyfx setup
call fips gpulang setup
echo Generating solution
call fips gen
setlocal
:PROMPT
SET /P OPENVS=Do want to open Visual Studio for the generated project (Y/[N])?
IF /I "%OPENVS%" NEQ "Y" GOTO END
call fips open
:END
endlocal
popd



