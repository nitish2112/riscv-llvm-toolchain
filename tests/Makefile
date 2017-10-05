FILE=matrix-mul-xloops#vvadd-xloops-int
DEBUG=--debug-pass=Structure
DEBUG_CLANG=#-debug-pass=Structure 
TRACE_LLVMIRS=#-print-after-all
DISABLE_LSR=-disable-lsr
CFLAGS=-Wall -g -O3
OPTFLAGS=-O3
LLVM_FLAGS=$(DISABLE_LSR)
CLANG_FLAGS=-mllvm $(DISABLE_LSR) #-no-integrated-as -static
TARGET_FLAGS=-MMD -MP #-target riscv64 #-mriscv=RV64IM

compile:
	riscv64-unknown-elf-clang $(CFLAGS) $(DEBUG_CLANG) $(CLANG_FLAGS) $(TARGET_FLAGS) $(FILE).c -o $(FILE) 2>&1 | tee run.log

asm-compile:
	 riscv64-unknown-elf-clang $(CFLAGS) $(DEBUG_CLANG) $(CLANG_FLAGS) $(TARGET_FLAGS) -S $(FILE).c 2>&1 | tee run.log	

gdb-compile:
	gdb -args clang $(CFLAGS) $(DEBUG_CLANG) $(CLANG_FLAGS) $(TARGET_FLAGS) -S $(FILE).c 2>&1 | tee run.log	

opt-O3:
	opt $(OPTFLAGS) $(LLVM_FLAGS) $(FILE).ll -o $(FILE)-opt-O3.bc 2>&1 | tee run.log
	llvm-dis $(FILE)-opt-O3.bc > $(FILE)-opt-O3.ll

clang:
	clang -g -emit-llvm -c  $(FILE).c
	llvm-dis $(FILE).bc > $(FILE).ll

rotate-opt:
	opt -O3 -loop-rotate $(TRACE_LLVMIRS) $(DEBUG) $(FILE).ll -o $(FILE)-loop-rotate.bc 2>&1 | tee rotate-tree.log
	llvm-dis $(FILE)-loop-rotate.bc > $(FILE)-loop-rotate.ll

gdb-rotate-opt:
	gdb -args opt -O1 -loop-rotate $(DEBUG) $(FILE).ll -o $(FILE)-loop-rotate.ll

strength-reduce:
	opt -O3 -loop-rotate -loop-reduce $(TRACE_LLVMIRS) $(DEBUG) $(FILE).bc -o $(FILE)-loop-rotate-reduce.bc 2>&1 | tee rotate-reduce.log
	llvm-dis $(FILE)-loop-rotate-reduce.bc > $(FILE)-loop-rotate-reduce.ll


gdb-strength-reduce:
	opt -O1 -loop-rotate $(DEBUG) $(FILE).ll -o $(FILE)-loop-rotate.ll
	gdb -args opt -O1 -loop-reduce $(DEBUG) $(FILE)-loop-rotate.ll -o $(FILE)-loop-rotate-reduce.ll

asm:
	llc -march=riscv $(FILE)-loop-rotate-reduce.bc -o $(FILE)-loop-rotate-reduce.S

asm-opt-O3:
	llc -march=riscv $(LLVM_FLAGS) $(FILE)-opt-O3.ll -o $(FILE)-opt-O3.S | tee run.log	

asm-rotate:
	llc -march=riscv $(FILE)-loop-rotate.ll -o $(FILE)-loop-rotate.S | tee run.log	

asm-unopt:
	llc -march=riscv $(FILE).ll -o $(FILE).S | tee run.log	

gdb-asm-unopt:
	gdb -args llc -march=riscv $(FILE).ll -o $(FILE).S | tee run.log	

bin:
	clang -target riscv64 -mriscv=RV64IAMFD -S $(FILE).c -o $(FILE).S

clean:
	rm *.ll *.bc *.S *.s *.d *.log *.dump 

.PHONY: clang opt clean asm bin
