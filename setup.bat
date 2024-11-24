@ECHO OFF
setlocal
pushd "%~dp0"
echo Getting initial dependencies
call fips set config win64-vstudio-debug
call fips fetch
echo Compiling external components
call fips nebula set work "%~dp0"
call fips nebula set toolkit "%~dp0"..\nebula"
call fips physx build vc17 debug
call fips anyfx setup
call pip install py7zr
call fips ultralight
echo Generating solution
call fips gen
call fips open
