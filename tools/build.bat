
cd ..\
cl /MD /LD /Fe:arma3urlfetch.dll^
    src\fetchURL.cpp^
    src\main.cpp^
    src\common.cpp^
    /DCURL_STATICLIB^
    /I src\lib\windows\libcurl\x86\include^
    src\lib\windows\libcurl\x86\lib\libcurl_a.lib
rm *.obj
rm *.lib
rm *.exp
exit