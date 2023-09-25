VENV := .venv/
SOURCE_VENV := . $(VENV)/bin/activate;
PYEXEC := $(SOURCE_VENV) python3.11
PIP_SYNC := $(PYEXEC) -m piptools sync
PYEXECPATH ?= $(shell which python3.11 || which python3.10)
PYTHON ?= $(shell basename $(PYEXECPATH))
REQS_MARKER := $(VENV)/bin/.pip-sync
PIP := $(PYEXEC) -m pip
DEPS_DIR := .deps

override DEPS := $(VENV) wg21.bib


$(VENV):
	$(PYTHON) -m venv --system-site-packages $(VENV)
	$(PIP) install setuptools pip pip-tools wheel
	$(PIP_SYNC) requirements.txt

.PHONY: create-venv
create-venv:
	rm -rf $(VENV)
	make $(VENV)

wording : optional_ref_wording.tex wg21.bib | $(VENV)
	mkdir -p $(DEPS_DIR)
	$(SOURCE_VENV) latexmk -shell-escape -pdflua -use-make -deps -deps-out=$(DEPS_DIR)/$@.d -MP optional_ref_wording.tex

wg21.bib:
	curl https://wg21.link/index.bib > wg21.bib

.PHONY: clean
clean:
	latexmk -c
	@$(MAKE) -f wg21/Makefile clean

# Include dependencies
$(foreach file,$(TARGET),$(eval -include $(DEPS_DIR)/$(file).d))

%: force
	@$(MAKE) -f wg21/Makefile $@
force: ;
