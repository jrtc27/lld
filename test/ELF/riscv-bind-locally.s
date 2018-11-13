# REQUIRES: riscv
# RUN: llvm-mc -filetype=obj -triple=riscv32-unknown-linux --position-independent %s -o %t.o
# RUN: llvm-mc -filetype=obj -triple=riscv32-unknown-linux --position-independent %S/Inputs/riscv-bind-locally2.s -o %t2.o
# RUN: ld.lld -shared %t.o %t2.o -o %t.so
# RUN: llvm-objdump -d %t.so | FileCheck %s

# CHECK-LABEL: foo:
# CHECK-NEXT:     1000:       ef 00 80 01     jal     24
# CHECK-NEXT:     1004:       97 00 00 00     auipc   ra, 0
# CHECK-NEXT:     1008:       e7 80 40 01     jalr    ra, ra, 20
# CHECK-NEXT:     100c:       63 06 00 00     beqz    zero, 12

# CHECK-LABEL: .Ltmp0:
# CHECK-NEXT:     1010:       17 05 00 00     auipc   a0, 0
# CHECK-NEXT:     1014:       e7 00 85 00     jalr    ra, a0, 8

# CHECK-LABEL: preemptible:
# CHECK-NEXT:     1018:       67 80 00 00     ret

    .text
    .global preemptible

foo:
    jal preemptible

    call preemptible

    beqz zero, preemptible

1:  auipc a0, %pcrel_hi(preemptible)
    jalr ra, a0, %pcrel_lo(1b)
