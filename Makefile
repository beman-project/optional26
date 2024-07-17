#! /usr/bin/make -f
# cmake-format: off
# /Makefile -*-makefile-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
# cmake-format: on

# This Makefile is a thin wrapper around CMake. It is intended to be used.

# Default target.
PROJECT?=$(shell basename $(CURDIR))

# Default install prefix.
INSTALL_PREFIX?=.install/
# Default build directory.
BUILD_DIR?=.build
# Default destination directory.
DEST?=$(INSTALL_PREFIX)
# Default toolchain: missing => c++ sytem tool.
TOOLCHAIN?=
# Default configuration: missing => Asan.
BUILD_CONFIG?=Asan
# Default CMake flags.
CMAKE_FLAGS?=

# Default targets.
TARGETS := test clean all ctest

export

.update-submodules:
	git submodule update --init --recursive
	touch .update-submodules

.gitmodules: .update-submodules


export

ifeq ($(strip $(TOOLCHAIN)),)
	# Default build configuration.
	_build_name?=toolchain/c++-system
	_build_dir?=$(BUILD_DIR)
	_configuration_types?="RelWithDebInfo;Debug;Tsan;Asan"
	_cmake_args=-DCMAKE_TOOLCHAIN_FILE=$(CURDIR)/etc/toolchain.cmake
else
	# Custom toolchain.
	_build_name?=toolchain/$(TOOLCHAIN)
	_build_dir?=$(BUILD_DIR)
	_configuration_types?="RelWithDebInfo;Debug;Tsan;Asan"
	_cmake_args=-DCMAKE_TOOLCHAIN_FILE=$(CURDIR)/etc/$(TOOLCHAIN)-toolchain.cmake
endif

# Build path.
_build_path?=$(_build_dir)/$(_build_name)

# CMake command wrapper.
define run_cmake =
	cmake \
	-G "Ninja Multi-Config" \
	-DCMAKE_CONFIGURATION_TYPES=$(_configuration_types) \
	-DCMAKE_INSTALL_PREFIX=$(abspath $(INSTALL_PREFIX)) \
	-DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
	$(_cmake_args) \
	$(CMAKE_FLAGS) \
	$(CURDIR)
endef

# Default target.
default: build test

$(_build_path):
	mkdir -p $(_build_path)

$(_build_path)/CMakeCache.txt: | $(_build_path) .gitmodules
	cd $(_build_path) && $(run_cmake)
	-rm compile_commands.json
	ln -s $(_build_path)/compile_commands.json

build: $(_build_path)/CMakeCache.txt ## Compile the project.
	cmake $(CMAKE_FLAGS) -DCMAKE_TOOLCHAIN_FILE=etc/$(TOOLCHAIN)-toolchain.cmake  -B $(_build_path) -S .
	cmake --build $(_build_path)  --config $(BUILD_CONFIG) --target all -- -k 0

install: $(_build_path)/CMakeCache.txt ## Install the project.
	DESTDIR=$(abspath $(DEST)) ninja -C $(_build_path) -k 0  install

ctest: $(_build_path)/CMakeCache.txt ## Run CTest on current build.
	ctest --build-config $(BUILD_CONFIG) --output-on-failure --test-dir $(_build_path)

ctest_ : build
	ctest --build-config $(BUILD_CONFIG) --output-on-failure --test-dir $(_build_path)

test: ctest_ ## Rebuild and run tests.

cmake: |  $(_build_path)
	cd $(_build_path) && ${run_cmake}

clean: $(_build_path)/CMakeCache.txt ## Clean the build artifacts, but don't delete the build directory.
	cmake --build $(_build_path)  --config $(BUILD_CONFIG) --target clean

realclean: ## Delete the entire build directory.
	rm -rf $(_build_path)

env:
	$(foreach v, $(.VARIABLES), $(info $(v) = $($(v))))

.PHONY : build install ctest ctest_ test cmake clean realclean env

.PHONY: papers
papers:
	$(MAKE) -C papers papers

# Help target
.PHONY: help
help: ## Show this help.
	@awk 'BEGIN {FS = ":.*?## "} /^[a-zA-Z_-]+:.*?## / {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'  $(MAKEFILE_LIST) | sort
