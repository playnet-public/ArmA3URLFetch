
cl /MD /LD /EHsc /Fe:arma3urlfetch.dll^
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
    /I include\windows\x86^
    lib\windows\libcurl_a.lib
rm *.obj
rm *.lib
rm *.exp