# P2988

## Building P2988

Compiling the paper requires a working LaTeX installation. See instructions for configuring your system at [C++ Standard Draft Sources](https://github.com/cplusplus/draft/blob/main/README.rst)

The papers/ subdirectory has the LaTeX souces for P2988 and the supporting macro definitions. To build, run
```shell
make papers
```

A working recent Python 3 is required to format the sources for the paper. A virtual env will be created in the papers subdirectory which the `minted` LaTeX package will use.
