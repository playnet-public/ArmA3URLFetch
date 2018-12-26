
CPPFLAGS=-Wall -fPIC -pthread -std=c++11
INCLUDES=-I.build/usr/lib/curl/include -Iinclude/jsoncpp
INCLUDES_x32=-I.build/usr/lib/curl/i386/include -Iinclude/jsoncpp
OBJS=include/jsoncpp.o src/common.o src/arguments.o src/requests.o src/clients.o src/output.o src/handler.o src/main.o
LIBS=.build/usr/lib/curl/lib/libcurl.a /usr/lib/x86_64-linux-gnu/libssl.a /usr/lib/x86_64-linux-gnu/libcrypto.a
LIBS_x32=.build/usr/lib/curl/i386/lib/libcurl.a /usr/lib/i386-linux-gnu/libssl.a /usr/lib/i386-linux-gnu/libcrypto.a
LDFLAGS=-shared -fPIC -pthread
OUTPUT=""
CURLSRC=https://github.com/curl/curl/releases/download/curl-7_59_0/curl-7.59.0.zip
ARMAKE=$(abspath tools/bin/armake)
TAG=$(shell git describe --tag | sed "s/-.*-/-/")
OUTPUTPATH=".build/@ArmA3URLFetch/"


all: linux64 linux32 build_mod deploy_mod

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
		CFLAGS=-m32 ./configure --prefix=$(shell pwd)/.build/usr/lib/curl/i386/ --without-librtmp --host=i686-pc-linux-gnu --disable-ftp --disable-file --disable-ldap --disable-ldaps --disable-rtsp --disable-dict --disable-telnet --disable-tftp --disable-pop3 --disable-imap --disable-smb --disable-smtp --disable-gopher --disable-manual --enable-ipv6 --disable-pthreads --enable-crypto-auth --enable-cookies --without-zlib --disable-threaded-resolver --without-brotli --with-ssl=/usr/lib/i386-linux-gnu && \
		make && \
		make install

build_curl:
	@cd .build/curl/ && \
		./configure --prefix=$(shell pwd)/.build/usr/lib/curl/ --without-librtmp --disable-ftp --disable-file --disable-ldap --disable-ldaps --disable-rtsp --disable-dict --disable-telnet --disable-tftp --disable-pop3 --disable-imap --disable-smb --disable-smtp --disable-gopher --disable-manual --enable-ipv6 --disable-pthreads --enable-crypto-auth --enable-cookies --without-zlib --disable-threaded-resolver --without-brotli --with-ssl=/usr/lib/x86_64-linux-gnu && \
		make && \
		make install

curl: get_curl build_curl build_i386_curl

prepare:
	@mkdir -p .build/
	@mkdir -p .build/@ArmA3URLFetch/addons
	@mkdir -p .build/@ArmA3URLFetch/keys
	@mkdir -p .build/keys
	@mkdir -p .build/bin

build_obj_linux_x64: $(OBJS)
	$(eval OUTPUT=$(OUTPUTPATH)/arma3urlfetch_x64.so)

build_prep_linux_x32:
	$(eval CPPFLAGS+= -m32)
	$(eval LDFLAGS+= -m32)
	$(eval INCLUDES=$(INCLUDES_x32))

build_obj_linux_x32: build_prep_linux_x32 $(OBJS)
	$(eval LIBS=$(LIBS_x32))
	$(eval OUTPUT=$(OUTPUTPATH)/arma3urlfetch.so)

%.o: %.cpp
	@echo "\tCXX\t\t$@"
	@$(CXX) $(INCLUDES) $(CPPFLAGS) -ldl -c $< -o $@

link:
	@echo "\tLD\t\t$(OUTPUT)"
	@$(CXX) $(LIBS) $(OBJS) $(LDFLAGS) -ldl -o $(OUTPUT)

clean:
	@echo "\tCLEANUP"
	@rm -f src/*.o

test: testLinux64 testLinux32

testLinux32: cleanTest
	@echo "\tTEST\t\tLinux (x86/x32)"
	@$(CXX) -m32 -pthread -fPIC -I.build/usr/lib/curl/i386/include/ \
		-Isrc/ \
		-Iinclude/jsoncpp \
		-std=c++11 \
		include/jsoncpp.cpp \
		src/common.cpp \
		src/arguments.cpp \
		src/requests.cpp \
		src/clients.cpp \
		src/output.cpp \
		src/handler.cpp \
		test/main.cpp \
		$(LIBS_x32) \
		-ldl \
		-o .build/test.a
	@echo "\tTEST\t\ttest.a (x86/x32)"
	@.build/test.a

testLinux64: cleanTest
	@echo "\tTEST\t\tLinux (x64)"
	@$(CXX) -Wall -pthread -fPIC -I.build/usr/lib/curl/include/ \
		-Isrc/ \
		-Iinclude/jsoncpp \
		-std=c++11 \
		include/jsoncpp.cpp \
		src/common.cpp \
		src/arguments.cpp \
		src/requests.cpp \
		src/clients.cpp \
		src/output.cpp \
		src/handler.cpp \
		test/main.cpp \
		$(LIBS) \
		-ldl \
		-o .build/test.a
	@echo "\tTEST\t\ttest.a (x64)"
	@.build/test.a

cleanTest:
	@echo "\tRM\t\tCleaning up tests"
	@rm -f .build/test.a

createKey: prepare
ifndef PRVKEYFILE
	rm -f .build/keys/*
	cd .build/keys/ && $(ARMAKE) keygen -f a3uf_$(TAG)
	$(eval KEY := a3uf_$(TAG))
	$(eval PRVKEYFILE := .build/keys/$(KEY).biprivatekey)
	cp -f .build/keys/*.bikey .build/@ArmA3URLFetch/keys
endif

build_mod: createKey a3uf_common a3uf_json

prepare_deploy:
	@rm -fR .builds/$(TAG)
	@mkdir -p .builds/$(TAG)

deploy_mod: prepare_deploy
	@mv .build/@ArmA3URLFetch .builds/$(TAG)
	@mv .build/keys .builds/$(TAG)

a3uf_common: prepare
	$(ARMAKE) build -p --force -k $(PRVKEYFILE) -e prefix=x\\a3uf\\addons\\common @ArmA3URLFetch/addons/common .build/@ArmA3URLFetch/addons/$@.pbo

a3uf_json: prepare
	$(ARMAKE) build -p --force -k $(PRVKEYFILE) -e prefix=x\\a3uf\\addons\\json @ArmA3URLFetch/addons/json .build/@ArmA3URLFetch/addons/$@.pbo
