CMAKE := cmake
CTEST := ctest

PRESET := debug
ifeq ($(OS),Windows_NT)
EXE_EXT := .exe
ifdef VCPKG_ROOT
PRESET := debug-vcpkg
endif
else
EXE_EXT :=
endif

BUILD_DIR := cmake-build-$(PRESET)

.PHONY: all test bench clean

all:
	$(CMAKE) --build --preset $(PRESET)

test:
	$(CMAKE) --build --preset $(PRESET)
	$(CTEST) --preset $(PRESET) --output-on-failure

bench:
	$(CMAKE) --build --preset $(PRESET) --target bench_runner
	$(BUILD_DIR)/bench_runner$(EXE_EXT)

clean:
	$(CMAKE) -E rm -rf cmake-build-debug cmake-build-debug-vcpkg cmake-build-release cmake-build-coverage cmake-build-sanitize build
