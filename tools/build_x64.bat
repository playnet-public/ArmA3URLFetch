
call "C:\Program Files (x86)\Microsoft Visual Studio\2013\Community\VC\Auxiliary\Build\vcvars64.bat"
cl /MD /LD /Fe:arma3urlfetch_x64.dll^
    src\common.cpp^
    src\arguments.cpp^
    src\requests.cpp^
    src\clients.cpp^
    src\output.cpp^
    src\handler.cpp^
    src\main.cpp^
    /DCURL_STATICLIB^
    /I include\windows\x64^
    lib\windows\libcurl_a_x64.lib^
    /std:c++17
rm *.obj
rm *.lib
rm *.exp
