BIN=@ArmA3URLFetch
PREFIX=a3uf
BUILD_PATH=.build
BUILDS_PATH=.builds
GIT_HASH=$(shell git describe --tag | sed "s/-.*-/-/")
ARMAKE_FLAGS=-w unquoted-string -w redefinition-wo-undef
CPPFLAGS=-m32 -Wall -fPIC -pthread -std=c++11
INCLUDES_x32=-I/usr/local/include -Iinclude/jsoncpp
OBJS=include/jsoncpp.o src/common.o src/arguments.o src/requests.o src/clients.o src/output.o src/handler.o src/main.o
LIBS_x32=/usr/local/lib/libcurl.a /usr/local/lib/libssl.a /usr/local/lib/libcrypto.a
LDFLAGS=-m32 -shared -fPIC -pthread
OUTPUT=""
OPENSSLSRC=https://www.openssl.org/source/openssl-1.1.0f.tar.gz
CURLSRC=https://github.com/curl/curl/releases/download/curl-7_59_0/curl-7.59.0.zip
TAG=$(shell git describe --tag | sed "s/-.*-/-/")
OUTPUTPATH=".build/@ArmA3URLFetch/"

ifeq ($(OS), Windows_NT)
	ARMAKE = ./tools/armake_w64.exe
else
	ARMAKE = ./tools/armake
endif

all: linux32 build_mod deploy_mod

linux32: prepare clean build_obj_linux_x32 link

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

test: prepare testLinux32

testLinux32: cleanTest
	@echo "\tTEST\t\tLinux (x86/x32)"
	@$(CXX) -m32 -pthread -fPIC -I.build/usr/local/include/ \
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
















build: $(patsubst addons/%, $(BUILD_PATH)/$(BIN)/addons/$(PREFIX)_%.pbo, $(wildcard addons/*))



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
