# A Simple JIT Compiler for  BrainF***

## Compile

```shell
make
```

## Performance Comparasion

Compile the source with -O3 option, run with the following command and compare the performance between interpreter and JIT.

```shell
./bf bf_programs/mendelbrot.bf
./bf --jit bf_programs/mendelbrot.bf
```



| Tag                           | Interpreter | JIT   |
| ----------------------------- | ----------- | ----- |
| Runtime Compile Time(seconds) | --          | 3.17s |
| Runtime (seconds)             |  35.09s     |3.16s  |

