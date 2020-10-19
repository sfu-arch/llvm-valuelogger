# LLVM Value Logger


This program instruments a function and dump args, initial and final state of memory. This essentially creates a closure and enables the function to be blackboxed and run independent of the program (e.g., on an FPGA).

Limitiation (Feature list)
=========================
Only instruments a single function. 
Function to be instrumented runs only once.



Building with CMake
==============================================
1. Clone the demo repository.

        git clone git@csil-git1.cs.surrey.sfu.ca:Dandelion/ValueLogger.git

2. Create a new directory for building.

        mkdir build

3. Change into the new directory.

        cd build

4. Run CMake with the path to the LLVM source.

        cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=True \
            -DLLVM_DIR=</path/to/LLVM/build>/lib/cmake/llvm/ ../




This produces a valuelogger and supporting
libraries in lib/.

Note, building with a tool like ninja can be done by adding `-G Ninja` to
the cmake invocation and running ninja instead of make.

Running
==============================================

First suppose that you have a program compiled to bitcode:

    clang -O1 -g -c -emit-llvm ../ValueLogger/tests/matmul/matmul.cpp -o matmul.bc

Running the instrumenter:


        ./build/bin/logger -fn-name=_Z3fooiPiS_S_ ./tests/matmul/matmul.bc -o matmul

Running the logger

        ./matmul
 
Ouput Format   
==============================================
Output is written to 3 files. 

| File      | Description                                                                               |
|-----------|-------------------------------------------------------------------------------------------|
| Args.mem  | Argument and return value for each invocation. Arg[0-n]: 0xVAL Ret:                       |
| Init.mem  | Initial value of memory touched by functon. Logs value for first-touch of memory location |
| Final.mem | Final value of memory touched by function. Logs last touch of memory location.            |


If invoked multiple times; Args.mem will contain Arg list/Ret for each invocation.


Example 1
=========================
### Args.mem
```
Arg0:0002
Arg1:00009a402c60
Arg2:00009a402d60
Arg3:00009a402e60
Ret:9d9d9d9c
```
### Init.mem (Bitmap display. All bytes in a read page are displayed (even if only one of the bytes in a page is read). 0x0 for a byte could either mean value read is 0 (OR) value was not read at all. )
```
0x9a402000    [Page Address]
0x0           [Byte 0 value]
0x0
0x0
...
...
0xff          [Byte 4095 value]
..
```

### Final.mem (run length encoded. Only written bytes are displayed. 0x0 for byte means value written was 0)
```
0x41c02c60
0xa
0x0
0x0
0x0
0x41c02c68
0xa
0x0
0x0
0x0
0x41c02c70
0xa
0x0
0x0
0x0
0x41c02c78
```

Example 2 (see vector.cpp)
=========================
```C++
for (int i = 0; i < k; i++)
    *sum = *sum + a[i];

for (int i = 0; i < k; i = i + 2)
    a[i] = 10;
```

a[0] is both read first during the sum calculation and written in the next loop.

Note that there could be overlap between Init.mem and Final.mem. Read-followed-by-Write to the same address in the function, in which case the Final.mem indicates the latest value. In the example below,
Byte 0x27c02c60-27c02c63 overlap between Init and Final.

### Final.mem
```
0x27c02c60
0xa
0x0
0x0
0x0
...
```

### Init.mem
```
0x27c02000
0x0
..
..
0x0 (Byte: 0xc60 line in txt file: 3170. )
0x0
0x0
0x0
0x1
0x0
0x0
0x0
```
### Final.mem
```
0x27c02c60
0xa
0x0
0x0
0x0
```
