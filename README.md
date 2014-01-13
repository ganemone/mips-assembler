MIPS Assembler
==============

This repository contains an assembler for MIPS assembly language code. The project takes a file as input with MIPS assembly code, and outputs the binary assembled result.

Example
=======

An example input file would be as follows


main:        lw $a0, 0($t0)                       
begin:       addi $t0, $zero, 0                
             addi $t1, $zero, 1
loop:        slt $t2, $a0, $t1                
             bne $t2, $zero, finish
             add $t0, $t0, $t1
             addi $t1, $t1, 2
             j loop                                
finish:      add $v0, $t0, $zero


The corresponding output for this file would be as follows.

10001101000001000000000000000000
00100000000010000000000000000000
00100000000010010000000000000001
00000000100010010101000000101010
00010101010000000000000000001000
00000001000010010100000000100000
00100001001010010000000000000010
00001000000000000000000000000011
00000001000000000001000000100000
