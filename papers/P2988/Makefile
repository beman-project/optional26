PYEXECPATH ?= $(shell which python3.12 || which python3.11 || which python3.10)
PYTHON ?= $(shell basename $(PYEXECPATH))
VENV := .venv/
SOURCE_VENV := . $(VENV)/bin/activate;
PYEXEC := $(SOURCE_VENV) $(PYTHON)
PIP_SYNC := $(PYEXEC) -m piptools sync
REQS_MARKER := $(VENV)/bin/.pip-sync
PIP := $(PYEXEC) -m pip
DEPS_DIR := .deps

override DEPS := $(VENV) wg21.bib

default: papers

$(VENV):
	$(PYTHON) -m venv --system-site-packages $(VENV)
	$(PIP) install setuptools pip pip-tools wheel
	$(PIP_SYNC) requirements.txt

.PHONY: create-venv
create-venv:
	rm -rf $(VENV)
	make $(VENV)

$(DEPS_DIR):
	mkdir -p $(DEPS_DIR)

.PHONY: papers
papers: optional_ref_wording.pdf ## Compile papers

optional_ref_wording.pdf : optional_ref_wording.tex wg21.bib $(DEPS_DIR) | $(VENV)
	$(SOURCE_VENV) latexmk -f -shell-escape -pdflua -use-make -deps -deps-out=$(DEPS_DIR)/$@.d -MP optional_ref_wording.tex

base.pdf : base.tex wg21.bib $(DEPS_DIR) | $(VENV)
	$(SOURCE_VENV) latexmk -f -shell-escape -pdflua -use-make -deps -deps-out=$(DEPS_DIR)/$@.d -MP base.tex

new.pdf : new.tex wg21.bib $(DEPS_DIR) | $(VENV)
	$(SOURCE_VENV) latexmk -f -shell-escape -pdflua -use-make -deps -deps-out=$(DEPS_DIR)/$@.d -MP new.tex


define curl_cmd =
	curl https://wg21.link/index.bib > wg21.bib
endef

wg21.bib:
	$(curl_cmd)

.PHONY: curl-bib
curl-bib:  ## Download wg21.bib from wg21.link
	$(curl_cmd)

.PHONY: clean
clean:
	latexmk -c
	@$(MAKE) -f wg21/Makefile clean
	-rm *.pdf

# Include dependencies
# $(foreach file,$(TARGET),$(eval -include $(DEPS_DIR)/$(file).d))
-include $(DEPS_DIR)/*.d

help: ## Show this help.
	@awk 'BEGIN {FS = ":.*?## "} /^[a-zA-Z_-]+:.*?## / {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'  $(MAKEFILE_LIST)  | sort
