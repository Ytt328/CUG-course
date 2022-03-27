      lui s0,0x0000010f
      addi s1,x0,0xffffffff
      sw s0,2044(sp)
      sw s1,2040(sp)
      jalr ra,x0,24
      slt t0,s0,s1
      beq t0,x0,lable1
      bne t0,x0,lable2
      sub s0,s0,s1
lable1:
      lb t0,2045(sp)
      lh t1,2046(sp)
      lw t2,2040(sp)
      sw t0,2032(sp)
      blt t1,t2,lable2
lable2:
      mul t0,s0,s1
      mulh t1,s0,s1
      mulhu t2,s0,s0
      sw t0,2028(sp)
      jal ra,lable3
      add t0,t1,t2
lable3:
      srl s0,s0,t0
      sll s1,s1,t1
      xor a0,s0,s1
      andi t0,t0,0x00000000
      ori t1,t1,0xffffffff
      bltu t0,t1,end
      slli t0,t0,2
      srli t1,t1,2
      srai t2,t2,4
      addi s1,x0,0x00000004
      divu t0,s0,s1
      remu t1,s0,s1
      xori t0,t0,0
      sw t0,2040(sp)
end:
      and x0,x0,x0
