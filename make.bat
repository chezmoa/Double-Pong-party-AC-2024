REM delete previous files
DEL *.gb

REM compile .c files into .o files
C:\gbdk\bin\lcc -c -o main.o main.c sprites.c

REM Compile a .gb file from the compiled .o files
C:\gbdk\bin\lcc -o ac2024v3.gb main.o sprites.o

REM delete intermediate files created for the conmpilation process
DEL *.asm
DEL *.lst
DEL *.ihx
DEL *.sym
DEL *.o
