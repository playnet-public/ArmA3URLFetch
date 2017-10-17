
cd ..\
cl /MD /LD /Fe:arma3urlfetch_x64.dll^
    src\fetchURL.cpp^
    src\main.cpp^
    src\common.cpp^
    /DCURL_STATICLIB^
    /I src\lib\windows\libcurl\x64\include^
    src\lib\windows\libcurl\x64\lib\libcurl_a.lib
rm *.obj
rm *.lib
rm *.exp
