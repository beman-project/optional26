#! /usr/bin/make -f
# -*-makefile-*-

INSTALL_PREFIX?=/home/sdowney/install
PROJECT?=$(shell basename $(CURDIR))
BUILD_DIR?=../cmake.bld/${PROJECT}
CONFIGURATION_TYPES?=RelWithDebInfo;Debug;Tsan;Asan
DEST?=../install
CMAKE_FLAGS?=
#CONFIG?=RelWithDebInfo
USE_DOCKER_FILE:=.use-docker
DOCKER_CMD := docker volume create cmake.bld; docker-compose run --rm dev
LOCAL_MAKE_CMD := make -e -f targets.mk
MAKE_CMD := $(LOCAL_MAKE_CMD)

TARGETS := test clean all ctest

# These targets are only run locally
LOCAL_ONLY_TARGETS :=

# If .lcldev/use-docker exists, then set `USE_DOCKER` to True
ifneq ("$(wildcard $(USE_DOCKER_FILE))","")
	USE_DOCKER := True
endif

ifdef USE_DOCKER
	MAKE_CMD := $(DOCKER_CMD) $(MAKE_CMD)
endif

export

.DEFAULT:
	$(MAKE_CMD) $@

# These targets are specified separately to enable bash autocomplete.
$(TARGETS):
	$(MAKE_CMD) $@

# These targets that should only be run locally.
$(LOCAL_ONLY_TARGETS):
	$(LOCAL_MAKE_CMD) $@

.update-submodules:
	git submodule update --init --recursive
	touch .update-submodules

.gitmodules: .update-submodules

.PHONY: use-docker
use-docker: ## Create docker switch file so that subsequent `make` commands run inside docker container.
	touch $(USE_DOCKER_FILE)

.PHONY: remove-docker
remove-docker: ## Remove docker switch file so that subsequent `make` commands run locally.
	$(RM) $(USE_DOCKER_FILE)

.PHONY: docker-rebuild
docker-rebuild: ## Rebuilds the docker file using the latest base image.
	docker-compose build

.PHONY: docker-clean
docker-clean: ## Clean up the docker volumes and rebuilds the image from scratch.
	docker-compose down -v
	docker-compose build

.PHONY: docker-shell
docker-shell: ## Shell in container
	docker-compose run --rm dev


# Help target
.PHONY: help
help: ## Show this help.
	@awk 'BEGIN {FS = ":.*?## "} /^[a-zA-Z_-]+:.*?## / {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'  $(MAKEFILE_LIST) targets.mk | sort

.PHONY: help
