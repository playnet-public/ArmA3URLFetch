
cd D:\Users\agphe\Documents\GitHub\public\ArmA3URLFetch\src
cl /MD /LD /Fe:arma3urlfetch_x64.dll fetchURL.cpp main.cpp /DCURL_STATICLIB /I D:\Compiler\libcurl_win\builds\libcurl-vc14-x64-release-static-ipv6-sspi-winssl\include D:\Compiler\libcurl_win\builds\libcurl-vc14-x64-release-static-ipv6-sspi-winssl\lib\libcurl_a.lib
mv arma3urlfetch_x64.dll ..\
rm *.obj
rm *.lib
rm *.exp
