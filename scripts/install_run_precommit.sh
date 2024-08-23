#!/usr/bin/env bash

python3 -m venv .venv

. .venv/bin/activate && python3 -m pip install --upgrade pip setuptools wheel
. .venv/bin/activate && python3 -m pip install pip-tools
. .venv/bin/activate && python3 -m piptools sync requirements.txt
. .venv/bin/activate && python3 -m piptools sync requirements-dev.txt
. .venv/bin/activate && pre-commit run -a
