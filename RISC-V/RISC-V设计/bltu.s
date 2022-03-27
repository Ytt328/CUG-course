      addi s0,x0,5
      addi s1,x0,2
loop:
      addi s0,s0,-1
      bltu s1,s0,loop
