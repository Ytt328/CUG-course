      addi s0,x0,3
      addi s1,x0,-3
loop:
      addi s0,s0,-1
      blt s1,s0,loop
