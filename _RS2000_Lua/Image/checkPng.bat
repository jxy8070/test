@echo off
::修复libpng warning: iCCP: known incorrect sRGB profile
:: 下载ImageMagick-7.1.0-portable-Q16-x64,并解压到指定位置，并在下面设置路径
echo ImageMagick fix libpng warning: iCCP: Not recognizing known sRGB profile ......
echo Search PNG in subdirs and process ...
set fn=C:\Program Files\ImageMagick-7.1.0-Q16\convert.exe
for /f "tokens=*" %%i in ('dir/s/b *.png') do "%fn%" "%%i" -strip "%%i"
pause