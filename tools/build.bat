
dir "C:\Program Files (x86)\"
call "C:\Program Files (x86)\Microsoft Visual Studio\2013\Community\VC\Auxiliary\Build\vcvars32.bat"
cl /MD /LD /EHsc /Fe:arma3urlfetch.dll^
    src\common.cpp^
    src\arguments.cpp^
    src\requests.cpp^
    src\clients.cpp^
    src\output.cpp^
    src\handler.cpp^
    src\main.cpp^
    /DCURL_STATICLIB^
    /I include\windows\x86^
    lib\windows\libcurl_a.lib^
    /std:c++17
rm *.obj
rm *.lib
rm *.exp