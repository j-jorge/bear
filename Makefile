CFLAGS=-Wall -Wno-sign-compare -fdiagnostics-color=always
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
else
  CMAKE_BUILD_TYPE=debug
  BUILD_DIR=build/debug
endif

ifeq ($(VERBOSE),)
  VERBOSE=0
endif

CMAKE_ARGS=-DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE)

all: test

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
