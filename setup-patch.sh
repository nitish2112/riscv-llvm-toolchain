#!/bin/bash

cd riscv-gnu-toolchain/riscv-binutils-gdb
git apply ../../patches/riscv-binutils-gdb.patch
cd ../../
