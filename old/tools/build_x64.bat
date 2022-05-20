
call "C:\Program Files\Microsoft SDKs\Windows\v7.1\Bin\SetEnv.cmd" /x64
call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" amd64
cl /MD /LD /Fe:arma3urlfetch_x64.dll^
    /EHsc^
    /I include\jsoncpp^
    include\jsoncpp.cpp^
    src\common.cpp^
    src\arguments.cpp^
    src\requests.cpp^
    src\clients.cpp^
    src\output.cpp^
    src\handler.cpp^
    src\main.cpp^
    /DCURL_STATICLIB^
    /I include\windows\x64^
    lib\windows\libcurl_a_x64.lib
rm *.obj
rm *.lib
rm *.exp
