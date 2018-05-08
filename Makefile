
CPPFLAGS=-Wall -fPIC -pthread -std=c++11
INCLUDES=-I.build/usr/lib/curl/include
INCLUDES_x32=-I.build/usr/lib/curl/i386/include
OBJS=src/common.o src/requests.o src/clients.o src/output.o src/handler.o src/main.o
LIBS=.build/usr/lib/curl/lib/libcurl.a /usr/lib/x86_64-linux-gnu/libssl.a /usr/lib/x86_64-linux-gnu/libcrypto.a
LIBS_x32=.build/usr/lib/curl/i386/lib/libcurl.a /usr/lib/i386-linux-gnu/libssl.a /usr/lib/i386-linux-gnu/libcrypto.a
LDFLAGS=-shared -pthread -fPIC
OUTPUT=""
CURLSRC=https://github.com/curl/curl/releases/download/curl-7_59_0/curl-7.59.0.zip

linux64: prepare clean build_obj_linux_x64 link
linux32: prepare clean build_obj_linux_x32 link

clean_curl:
	@rm -fR .build/curl/

get_curl: clean_curl
	@wget -P .build/ $(CURLSRC)
	@unzip .build/curl-7.59.0.zip -d .build/
	@mv .build/curl-7.59.0 .build/curl/
	@rm .build/curl-7.59.0.zip

build_i386_curl:
	@cd .build/curl/ && \
		CFLAGS=-m32 ./configure --prefix=$(shell pwd)/.build/usr/lib/curl/i386/ --host=i686-pc-linux-gnu --disable-ftp --disable-file --disable-ldap --disable-ldaps --disable-rtsp --disable-dict --disable-telnet --disable-tftp --disable-pop3 --disable-imap --disable-smb --disable-smtp --disable-gopher --disable-manual --enable-ipv6 --disable-pthreads --enable-crypto-auth --enable-cookies --without-zlib --disable-threaded-resolver --without-brotli --with-ssl=/usr/lib/i386-linux-gnu && \
		make && \
		make install

build_curl:
	@cd .build/curl/ && \
		./configure --prefix=$(shell pwd)/.build/usr/lib/curl/ --disable-ftp --disable-file --disable-ldap --disable-ldaps --disable-rtsp --disable-dict --disable-telnet --disable-tftp --disable-pop3 --disable-imap --disable-smb --disable-smtp --disable-gopher --disable-manual --enable-ipv6 --disable-pthreads --enable-crypto-auth --enable-cookies --without-zlib --disable-threaded-resolver --without-brotli --with-ssl=/usr/lib/x86_64-linux-gnu && \
		make && \
		make install

curl: get_curl build_curl build_i386_curl

prepare:
	@mkdir -p .build/

build_obj_linux_x64: $(OBJS)
	$(eval OUTPUT=.build/arma3urlfetch_x64.so)

build_prep_linux_x32:
	$(eval CPPFLAGS+= -m32)
	$(eval LDFLAGS+= -m32)
	$(eval INCLUDES=$(INCLUDES_x32))

build_obj_linux_x32: build_prep_linux_x32 $(OBJS)
	$(eval LIBS=$(LIBS_x32))
	$(eval OUTPUT=.build/arma3urlfetch.so)

%.o: %.cpp
	@echo "\tCXX\t\t$@"
	@$(CXX) $(INCLUDES) $(CPPFLAGS) -ldl -c $< -o $@

link:
	@echo "\tLD\t\t$(OUTPUT)"
	@$(CXX) $(LDFLAGS) $(OBJ) $(LIBS) -ldl -o $(OUTPUT)

clean:
	@echo "\tCLEANUP"
	@rm -f src/*.o

test: testLinux64 testLinux32

testLinux32: cleanTest
	@echo "\tTEST\t\tLinux x32"
	@$(CXX) -m32 -pthread -fPIC -I.build/usr/lib/curl/i386/include/ \
		-Isrc/ \
		-std=c++11 \
		src/common.cpp \
		src/requests.cpp \
		src/clients.cpp \
		src/output.cpp \
		src/handler.cpp \
		test/main_x32.cpp \
		$(LIBS_x32) \
		-ldl \
		-o .build/test.a
	@echo "\tTEST\t\ttest.a x32"
	@.build/test.a

testLinux64: cleanTest
	@echo "\tTEST\t\tLinux x64"
	@$(CXX) -Wall -pthread -fPIC -I.build/usr/lib/curl/include/ \
		-Isrc/ \
		-std=c++11 \
		src/common.cpp \
		src/requests.cpp \
		src/clients.cpp \
		src/output.cpp \
		src/handler.cpp \
		test/main_x64.cpp \
		$(LIBS) \
		-ldl \
		-o .build/test.a
	@echo "\tTEST\t\ttest.a x64"
	@.build/test.a

cleanTest:
	@rm -f .build/test.a
