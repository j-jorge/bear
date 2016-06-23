CFLAGS=-Wall -Wno-sign-compare -fdiagnostics-color=always -fext-numeric-literals
CXXFLAGS= $(CFLAGS) -std=c++11
LDFLAGS=

CMAKE_BUILD_TYPE=

ifeq ($(BUILD),release)
  CMAKE_BUILD_TYPE=release
  BUILD_DIR=build/release
else ifeq ($(BUILD),profile)
  CMAKE_BUILD_TYPE=release
  CXXFLAGS += -pg
  BUILD_DIR=build/profile
  CMAKE_ARGS=-DBEAR_ENGINE_CORE_LINK_TYPE=STATIC
else
  CMAKE_BUILD_TYPE=debug
  BUILD_DIR=build/debug
endif

CMAKE_ARGS += -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE) \
        -DCMAKE_INSTALL_PREFIX=/usr

all: test

install: test
	cd $(BUILD_DIR) && $(MAKE) install

uninstall:
	[ ! -d $(BUILD_DIR) ] || cd $(BUILD_DIR) && $(MAKE) uninstall

test: target
	cd $(BUILD_DIR) && CTEST_OUTPUT_ON_FAILURE=1 ctest

target: .cmake
	cd $(BUILD_DIR) && $(MAKE) VERBOSE=$(VERBOSE)

.cmake: $(BUILD_DIR)
	cd $(BUILD_DIR) \
	  && CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)" LDFLAGS="$(LDFLAGS)" \
	     cmake ../.. $(CMAKE_ARGS)

$(BUILD_DIR):
	[ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR)

clean:
	rm -fr build
