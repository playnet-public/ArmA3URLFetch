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

$(BUILD_PATH)/$(BIN)/addons/$(PREFIX)_%.pbo: addons/%
	@mkdir -p $(BUILD_PATH)/$(BIN)/addons
	@echo "    PBO   $@"
	@${ARMAKE} build $(FLAGS) -p -f -e "version=$(GIT_HASH)" $< $@

$(BUILD_PATH)/keys/%.biprivatekey:
	@mkdir -p $(BUILD_PATH)/keys
	@echo "    KEY   $@"
	@${ARMAKE} keygen -f $(patsubst $(BUILD_PATH)/keys/%.biprivatekey, $(BUILD_PATH)/keys/%, $@)

$(BUILD_PATH)/$(BIN)/addons/$(PREFIX)_%.pbo.lil_$(GIT_HASH).bisign: $(BUILD_PATH)/$(BIN)/addons/$(PREFIX)_%.pbo $(BUILD_PATH)/keys/lil_$(GIT_HASH).biprivatekey
	@echo "    SIG   $@"
	@${ARMAKE} sign -f -s $@ $(BUILD_PATH)/keys/lil_$(GIT_HASH).biprivatekey $<

signatures: $(patsubst addons/%, $(BUILD_PATH)/$(BIN)/addons/$(PREFIX)_%.pbo.lil_$(GIT_HASH).bisign, $(wildcard addons/*))

mod_clean:
	rm -Rf $(BUILD_PATH)/$(BIN) $(BUILD_PATH)/keys

release: mod_clean
	@"$(MAKE)" $(MAKEFLAGS) signatures
	@mkdir -p $(BUILDS_PATH)/$(GIT_HASH)
	@echo "    CP   $(BUILD_PATH)/* to $(BUILDS_PATH)/$(GIT_HASH)"
	@cp -Rf $(BUILD_PATH)/$(BIN) $(BUILD_PATH)/keys $(BUILDS_PATH)/$(GIT_HASH)/
