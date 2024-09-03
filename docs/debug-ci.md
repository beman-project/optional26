# Debug CI

<!--
SPDX-License-Identifier: 2.0 license with LLVM exceptions
-->

## CI flows

CI flows are found in [.github/workflows/](.github/workflows/). Usually, they are run when generating a Git Action (e.g. at `push`).

WIP to add flows for tests, linting etc.

## Run CI Flows on Local Setup

We can use [nektos/act](https://github.com/nektos/act) to simulate and test Git Actions on local setup.
Setup and tutorial on [nektosact.com/](https://nektosact.com/).

```shell
# Check available jobs
$ sudo act --container-architecture linux/amd64 --list
Stage  Job ID  Job name                   Workflow name  Workflow file  Events
0      build   ${{ matrix.config.name }}  CI Tests       ci.yml         pull_request,push

# Run all CI jobs
Optional26 $ sudo act -P ubuntu-24.04=ghcr.io/catthehacker/ubuntu:full-24.04 --container-architecture linux/amd64 push
[CI Tests/Ubuntu Clang 18-2] 🚀  Start image=ghcr.io/catthehacker/ubuntu:full-24.04
[CI Tests/Ubuntu Clang 19-3] 🚀  Start image=ghcr.io/catthehacker/ubuntu:full-24.04
[CI Tests/Ubuntu Clang 17-1] 🚀  Start image=ghcr.io/catthehacker/ubuntu:full-24.04
[CI Tests/Ubuntu GCC 14-5  ] 🚀  Start image=ghcr.io/catthehacker/ubuntu:full-24.04
[CI Tests/Ubuntu GCC 13-4  ] 🚀  Start image=ghcr.io/catthehacker/ubuntu:full-24.04
...

```
