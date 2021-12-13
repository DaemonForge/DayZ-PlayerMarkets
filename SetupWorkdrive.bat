@echo off

cd /D "%~dp0"

IF exist "P:\PlayerMarkets\" (
	echo Removing existing link P:\PlayerMarkets
	rmdir "P:\PlayerMarkets\"
)

echo Creating link P:\PlayerMarkets
mklink /J "P:\PlayerMarkets\" "%cd%\PlayerMarkets\"

echo Done