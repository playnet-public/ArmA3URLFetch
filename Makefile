
CPPFLAGS=-Wall -fPIC -pthread -std=c++11
INCLUDES=-Iinclude/windows/x64/
INCLUDES_x32=-Iinclude/windows/x86/
OBJS=src/common.o src/requests.o src/clients.o src/output.o src/handler.o src/main.o
LIBS=""
LIBS_x64=lib/linux/libcurl_x64.a lib/linux/libssl_x64.a lib/linux/libcrypto_x64.a
LIBS_x32=lib/linux/libcurl.a lib/linux/libssl.a lib/linux/libcrypto.a
LDFLAGS=-shared -pthread -fPIC
OUTPUT=""

linux64: prepare clean build_obj_linux_x64 link
linux32: prepare clean build_obj_linux_x32 link

prepare:
	@mkdir -p .build/

build_obj_linux_x64: $(OBJS)
	$(eval LIBS=$(LIBS_x64))
	$(eval OUTPUT=.build/arma3urlfetch_x64.so)

build_prep_linux_x32:
	$(eval CPPFLAGS+= -m32)
	$(eval LDFLAGS+= -m32)
	$(eval INCLUDES=$(INCLUDES_x32))

build_obj_linux_x32: build_prep_linux_x32 $(OBJS)
	$(eval LIBS=$(LIBS_x32))
	$(eval OUTPUT=.build/arma3urlfetch.so)

%.o: %.cpp
	@echo " CXX $@"
	@$(CXX) $(INCLUDES) $(CPPFLAGS) -ldl -c $< -o $@

link:
	@echo " LD $(OUTPUT)"
	@$(CXX) $(LDFLAGS) $(OBJ) $(LIBS) -ldl -o $(OUTPUT)

.PHONY: linux32 linux64

clean:
	@echo " Cleaning up objects"
	@rm -f src/*.o

test: testLinux64

testLinux32: cleanTest
	@echo " TEST Linux x32"
	@$(CXX) -m32 -pthread -fPIC -Iinclude/windows/x86/ \
		-Isrc/ \
		-std=c++11 \
		src/common.cpp \
		src/requests.cpp \
		src/clients.cpp \
		src/output.cpp \
		src/handler.cpp \
		test/main_x32.cpp \
		/usr/lib/x86_64-linux/libcurl.a \
		/usr/lib/x86_64-linux/libssl.a \
		/usr/lib/x86_64-linux/libcrypto.a \
		-ldl \
		-o .build/test.a
	@echo " TEST test.a x32"
	@.build/test.a

testLinux64: cleanTest
	@echo " TEST Linux x64"
	@$(CXX) -pthread -fPIC -Iinclude/windows/x86/ \
		-Isrc/ \
		-std=c++11 \
		src/common.cpp \
		src/requests.cpp \
		src/clients.cpp \
		src/output.cpp \
		src/handler.cpp \
		test/main_x32.cpp \
		/usr/lib/x86_64-linux/libcurl_x64.a \
		/usr/lib/x86_64-linux/libssl_x64.a \
		/usr/lib/x86_64-linux/libcrypto_x64.a \
		-ldl \
		-o .build/test.a
	@echo " TEST test.a x64"
	@.build/test.a

cleanTest:
	@rm -f .build/test.a
