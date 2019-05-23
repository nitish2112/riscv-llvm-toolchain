==========================================================================
  Installing Andes RISCV-LLVM toolchain
==========================================================================

0.) Prerequisites:
  - gcc-5.x
  - cmake-3.x
  - autoconf-2.69 and above
  - *do not* source any of setup-brg.sh or other scripts
  - makeinfo (apparently I couldn't find makeinfo on brg-vip)

2.) Follow the step by step procedure to compile the tool-chain:

  % mkdir RISCV-LLVM-TOOLCHAIN && cd RISCV-LLVM-TOOLCHAIN
  % git clone --recursive https://github.com/andestech/riscv-llvm-toolchain.git
  % cd riscv-llvm-toolchain
  % mkdir build && cd build
  % ../configure --prefix=/path/to/RISCV-LLVM-TOOLCHAIN --with-arch=rv64ima
    --with-build-type=Release
  % make -j64

  NOTE: 

  a) `--with-build-type=Release` reduces the size of the build. The build 
  script by default builds llvm with RelWithDebInfo and static linking, so the 
  resulting llvm executables are quite large. I took around 60GB of space for 
  me when built without this option, and around 10 GB with this option. You
  can additionally add `-DBUILD_SHARED_LIBS=On` to the CMAKE parameters to 
  shave a size a little bit more. 
  
  b) Before typing "make", make sure that your CPATH **does not** contain the
  the riscv includes. This is a very common error which I end up in, as before
  running the benchmarks you want the includes to be the ones that are provided
  by this build, and while compiling the compiler itself you want the the 
  headers to be from the ones installed on your system along with gcc.

3.) Add the riscv compilers to the path
  - Make sure that you have not cluttered your environment by various versions
    of riscv64-unknown-elf-*. 
  - Both gcc and clang must be used from andes toolchain
  
  % export PATH=/path/to/RISCV-LLVM-TOOLCHAIN/bin:$PATH
 
4.) Add the include directories to the CPATH

  % export CPATH="/path/to/RISCV-LLVM-TOOLCHAIN/riscv64-unknown-elf/include/c++/7.1.1/"
  % export CPATH=$CPATH:"/path/to/RISCV-LLVM-TOOLCHAIN/riscv64-unknown-elf/include/c++/7.1.1/riscv64-unknown-elf/
  % export CPATH=$CPATH:"/path/to/RISCV-LLVM-TOOLCHAIN/riscv64-unknown-elf/include" 

5.) Compiling the ISA simulator
  - Andes toolchain only provides riscv compilers and does not provide the 
    ISA simulator etc. Follow the following steps to build spike, proxy kernel
    etc
  
  - Prerequisistes:
      You need to have device-tree-compiler installed and added to your path

    % export TOP=$PWD
    % mkdir RISCV-ISA-SIM && cd RISCV-ISA-SIM
    % git clone https://github.com/riscv/riscv-fesvr.git
    % git clone https://github.com/riscv/riscv-isa-sim.git
    % git clone https://github.com/riscv/riscv-pk.git
    % cd riscv-fesvr 
    % mkdir build && cd build
    % ../configure --prefix=/path/to/RISCV-ISA-SIM 
    % make install
    % cd $(TOP)/riscv-isa-sim
    % mkdir build && cd build
    % ../configure --prefix=/path/to/RISCV-ISA-SIM --with-fesvr=/path/to/RISCV-ISA-SIM
    % make install
    % cd $(TOP)/riscv-pk
    % mkdir build && cd build
    % ../configure --prefix=/path/to/RISCV-ISA-SIM 
    % make install

  - Add ISA simulator to your path
    % export PATH=/path/to/RISCV-ISA-SIM/bin:$PATH

NOTE: The commit point that I worked on was 131978c for riscv-llvm-toolchain. 
  This included:

  - 0189d2d for riscv-clang
  - 2409b3d for riscv-compiler-rt
  - dc3df86 for riscv-gnu-toolchain
  - 3a24817 for riscv-llvm
  - 8ddded3 for extern-gcc-testsuite   

==============================================================================
Forking Andes Compiler Repo
==============================================================================

1) First of all fork the main repo
   -- Go to https://github.com/andestech/riscv-llvm-toolchain and click on fork
      Note that it will fork riscv-gnu-toolchain for you automatically.

2) Fork all the submodules that have the relative path:
   -- Go to https://github.com/andestech/extern-gcc-testsuite and click on fork
   -- Go to https://github.com/andestech/riscv-clang and click on fork
   -- Go to https://github.com/andestech/riscv-compiler-rt and click on fork
   -- Go to https://github.com/andestech/riscv-llvm and click on fork.

3) Clone the riscv-llvm-toolchain from your own fork:

   % git clone --recursive https://github.com/nitish2112/riscv-llvm-toolchain

   Note that when you will go through the submodules and type git branch,
   you will see that it is in the detatched head state. This should be the case
   as the detatched HEAD is actually the commit point of this submodule
   that the main repo is tracking. 

   The concept of submodules is quite simple. A submodule is just a pointer to
   a certain commit (SHA-1) in another git repo (htto://github.com/blah/blah).
   As the SHA-1 for different commits and branches are different, just having
   the right SHA-1 entry for the submodule can track the exact branch and 
   commit you were working on. 

   If you create and checkout a branch and may be just commit something in a 
   submodule, the SHA-1 entry changes and hence the main repo will show that 
   there are changes in the submodule when you type:

   % git submodule status --recursive
   
   It is advisable to commit the changes in all the submodules before you 
   commit in the main repo. As then the main repo will always keep track of the
   right commit that you worked on. 


4) Set the upstream for all the forks:

   % cd riscv-llvm-toochain
   % git remote add upstream https://github.com/andestech/riscv-llvm-toolchain.git
   % cd extern-gcc-testsuite
   % git remote add upstream https://github.com/andestech/extern-gcc-testsuite.git
   % cd riscv-clang
   % git remote add upstream https://github.com/andestech/riscv-clang
   % cd riscv-compiler-rt
   % git remote add upstream https://github.com/andestech/riscv-compiler-rt
   % cd riscv-llvm
   % git remote add upstream https://github.com/andestech/riscv-llvm

5) Create your own branch in each of the repository, at the specific commit 
   point of the submodules which is being tracked by the main repo. If you just 
   create a branch without specifying the commit tag, it will create a branch
   from the latest commit point. Note that when you cloned the repo, each of the
   submodule was in a detatched head state -- which is not something we want. 
   Commits on a detatched head will get lost.
  
   % cd riscv-llvm-toolchain
   % git checkout -b parallel-micro-engine
   % cd riscv-clang
   % git checkout -b parallel-micro-engine 0189d2d
   % cd riscv-compiler-rt
   % git checkout -b parallel-micro-engine 2409b3d
   % riscv-llvm
   % git checkout -b parallel-micro-engine 3a24817
   
   I did not create any branch for the extern-gcc-testsuite as I am not going to
   edit that. Though it is safe to create one for future.

6) Create a separate fork for riscv-gnu-toolchain. As we the original repo just
   point to the riscv-gnu-toolchain and we want to edit it as well so that
   we can hack the assembler, do the following

   Make sure the riscv-gnu-toolchain is in sync with riscv/riscv-gnu-toolchain
   
   % cd riscv-gnu-toolchain
   % git checkout master
   % git pull

   As origin was still riscv/riscv-gnu-toolchain so we were pulling from that.
   Now set the origin to you forked repo, and add riscv repo as the upstream.

   % git remote set-url origin https://github.com/nitish2112/riscv-gnu-toolchain.git
   % git remote add upstream https://github.com/riscv/riscv-gnu-toolchain.git

   Create a branch at commit point that the riscv-llvm-toolchain was pointing to

   % git cehckout -b parallel-micro-engine dc3df86
  
   Now go to https://github.com/riscv/riscv-binutils-gdb and fork this repository,
   as this is the repository we want to work on.

   Add the your own fork as the origin and riscv version as upstream, and also
   create a branch at the commit point which was pointed by the commit of 
   riscv-gnu-toolchain to which the riscv-llvm-toolchain was pointing to.

   % cd riscv-binutils-gdb
   % git remote add upstream https://github.com/riscv/riscv-binutils-gdb.git
   % git remote set-url origin https://github.com/nitish2112/riscv-binutils-gdb.git
   % git checkout -b parallel-micro-engine 5852e73

   Now, we need to tell our local riscv-gnu-toolchain repo that the remote 
   for riscv-gnu-binutils is the one which we forked.
  
   Go to .gitmodules in riscv-gnu-toolchain and edit the path for binutils-gdb to

   [submodule "riscv-binutils-gdb"]
        path = riscv-binutils-gdb
        url = https://github.com/nitish2112/riscv-binutils-gdb.git

   Now run:
   
   % git submodule sync

   Now note that the other submodules (gcc, glibc, dejagnu, newlib) in 
   ricv-gnu-toolchain have relative paths instead of absolute path, so 
   now when we clone next time git will look for 
   https://github.com/nitish2112/riscv-gcc.git which does not exist. So
   convert the relative paths to absoulte paths like this:

   [submodule "riscv-binutils-gdb"]
           path = riscv-binutils-gdb
           url = https://github.com/nitish2112/riscv-binutils-gdb.git
   [submodule "riscv-gcc"]
           path = riscv-gcc
           url = https://github.com/riscv/riscv-gcc.git
   [submodule "riscv-glibc"]
           path = riscv-glibc
           url = https://github.com/riscv/riscv-glibc.git
   [submodule "riscv-dejagnu"]
           path = riscv-dejagnu
           url = https://github.com/riscv/riscv-dejagnu.git
   [submodule "riscv-newlib"]
           path = riscv-newlib
           url = https://github.com/riscv/riscv-newlib.git
   [submodule "riscv-qemu"]
           path = riscv-qemu
           url = git://github.com/riscv/riscv-qemu.git

   And run the sync again

   % git submodule sync

   Now go to riscv-llvm-toolchain and change .gitmodules so that the url for
   riscv-gnu-toolchain is set to the local forked repository.

   [submodule "riscv-gnu-toolchain"]
        path = riscv-gnu-toolchain
        url = https://github.com/nitish2112/riscv-gnu-toolchain.git

   Now run a sync in the riscv-llvm-toolchain folder.
  
   % git submodule sync


7) If you were working on some previous repo, go to each repo and do

   % git diff >  ~/git.patch

   And apply it to new repo. for eg.
   
   % cd riscv-clang
   % git apply ~/git.patch

8) Once you are done with all the changes in multiple submdoules that you
   were working on, go the main repo and type:

   % git submodule status --recursive
  
  You should see a "+" sign before any submodule in which either you checked
  a commit point or create a new branch (i.e. basically if old SHA-1 for the
  submodule is different than the current SHA-1). Note that of you have just
  made changes in a submodule but haven't commited anything in that submodule,
  the SHA-1 for it will still be the same, and "git submodule status --recursive"
  will not show any "+" sign. If you commit in the main repo at this point,
  you have created a mess for yourself, as now when you clone this repo and 
  do "git submodule update --init --recursive", you are not going to see the
  changes in that particular submodule.

  ** Always commit the children (submodules) before you commit the parent **

  For e.g.:
  
  % cd riscv-clang
  % git commit -a -m "xyz" && git push && cd ..
  % cd riscv-llvm
  % git commit -a -m "abc" && git push && cd ..
  % git submodule status --recursive
  % git commit -a -m "commiting main repo" && git push 

9) Cloning and building the repo:

   It is quite simple as we just want to clone from our branch
 
   % git clone -b parallel-micro-engine --recursive \
        https://github.com/nitish2112/riscv-llvm-toolchain.git

   Or you can do:

   % git clone https://github.com/nitish2112/riscv-llvm-toolchain.git
   % git checkout parallel-micro-engine
   % git submodule update --init --recursive

   After this point follow steps mentioned in andes-riscv-llvm-compiler-notes.txt 
==============================================================================
Original README RISC-V LLVM Toolchain
==============================================================================

This is the RISC-V LLVM C and C++ cross-compiler. It supports ELF/Newlib toolchain only in this moment.

###  Getting the sources

This repository uses submodules. You need the --recursive option to fetch the submodules automatically

    $ git clone --recursive https://github.com/andestech/riscv-llvm-toolchain.git

### Installation

To build the Newlib cross-compiler, pick an install path.  If you choose,
say, `/opt/riscv`, then add `/opt/riscv/bin` to your `PATH` now.  Then, simply
run the following command:

    ./configure --prefix=/opt/riscv
    make

You should now be able to use riscv-llvm and its cousins.

The build defaults to targetting RV64IMC (64-bit), even on a 32-bit build
environment.  To build the 32-bit RV32IMC toolchain, use:

    ./configure --prefix=/opt/riscv --with-arch=rv32imc
    make

Supported architectures are rv32i or rv64i plus standard extensions (a)tomics,
(m)ultiplication and division.

### Usage

You can found a LLVM toolchain in your install path, we recommand you use `riscv32-elf-unknown-clang` (or `riscv64-elf-unknown-clang` for RV64) to compile program just like `riscv32-elf-unknown-gcc` instead of use clang directly.

### Advanced Options

There are a number of additional options that may be passed to
configure.  See './configure --help' for more details.
