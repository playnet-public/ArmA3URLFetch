
CPPFLAGS=-m32 -Wall -fPIC -pthread -std=c++11
INCLUDES_x32=-I.build/usr/lib/curl/i386/include -Iinclude/jsoncpp
OBJS=include/jsoncpp.o src/common.o src/arguments.o src/requests.o src/clients.o src/output.o src/handler.o src/main.o
LIBS_x32=.build/usr/lib/curl/i386/lib/libcurl.a .build/usr/lib/openssl/lib/libssl.a .build/usr/lib/openssl/lib/libcrypto.a
LDFLAGS=-m32 -shared -fPIC -pthread
OUTPUT=""
OPENSSLSRC=https://www.openssl.org/source/openssl-1.1.0f.tar.gz
CURLSRC=https://github.com/curl/curl/releases/download/curl-7_59_0/curl-7.59.0.zip
ARMAKE=$(abspath tools/bin/armake)
TAG=$(shell git describe --tag | sed "s/-.*-/-/")
OUTPUTPATH=".build/@ArmA3URLFetch/"

all: linux32 build_mod deploy_mod

linux32: prepare clean build_obj_linux_x32 link

openssl_clean:
	@echo "\tCLEAN    openssl"
	@rm -fR .build/openssl .build/usr/lib/openssl .build/openssl-1.1.0f.tar.gz

openssl: openssl_clean
	@echo "\tGET    openssl"
	@mkdir -p .build/openssl
	@wget -P .build/ $(OPENSSLSRC)
	@cd .build/ && tar xzvf openssl-1.1.0f.tar.gz
	@mv .build/openssl-1.1.0f/* .build/openssl
	@rm -R .build/openssl-1.1.0f/
	@cd .build/openssl && setarch i386 ./config -m32 --prefix=$(abspath .build/usr/lib/openssl) no-ui no-ssl3 shared
	@$(MAKE) -C .build/openssl
	@$(MAKE) -C .build/openssl install

curl_clean:
	@echo "\tCLEAN    curl"
	@rm -fR .build/curl/ .build/usr/lib/curl

curl: curl_clean
	@wget -P .build/ $(CURLSRC)
	@unzip .build/curl-7.59.0.zip -d .build/
	@mv .build/curl-7.59.0 .build/curl/
	@rm .build/curl-7.59.0.zip
	@cd .build/curl/ CFLAGS=-m32 ./configure --prefix=$(shell pwd)/.build/usr/lib/curl/i386/ --without-librtmp --host=i686-pc-linux-gnu --disable-ftp --disable-file --disable-ldap --disable-ldaps --disable-rtsp --disable-dict --disable-telnet --disable-tftp --disable-pop3 --disable-imap --disable-smb --disable-smtp --disable-gopher --disable-manual --enable-ipv6 --disable-pthreads --enable-crypto-auth --enable-cookies --without-zlib --disable-threaded-resolver --without-brotli --with-ssl=/usr/lib/i386-linux-gnu
	@$(MAKE) -C .build/curl
	@$(MAKE) -C .build/curl install

prepare:
	@mkdir -p .build/
	@mkdir -p .build/@ArmA3URLFetch/addons
	@mkdir -p .build/@ArmA3URLFetch/keys
	@mkdir -p .build/keys
	@mkdir -p .build/bin

build_obj_linux_x32: $(OBJS)
	$(eval LIBS=$(LIBS_x32))
	$(eval OUTPUT=$(OUTPUTPATH)/arma3urlfetch.so)

%.o: %.cpp
	@echo "\tCXX\t\t$@"
	@$(CXX) $(INCLUDES_x32) $(CPPFLAGS) -ldl -c $< -o $@

link:
	@echo "\tLD\t\t$(OUTPUT)"
	@$(CXX) $(OBJS) $(LIBS) $(LDFLAGS) -ldl -o $(OUTPUT)

clean:
	@echo "\tCLEANUP"
	@rm -f src/*.o
	@rm -f include/*.o

test: testLinux32

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
