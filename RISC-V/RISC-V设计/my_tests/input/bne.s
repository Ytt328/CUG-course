      addi s0,x0,-1
      addi s1,x0,4
loop:
      addi s1,s1,-1
      bne s0,s1,loop
