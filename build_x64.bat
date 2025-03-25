
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
