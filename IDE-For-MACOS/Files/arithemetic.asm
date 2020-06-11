.text
li $v0, 5 
syscall   
move $t0, $v0, 
sw $t0,  aL
lw $t1, aL 
li $t2, 3 
add $t3, $t2, $t1
sw $t3, t1L 
lw $t1, t1L 
li $t2, 2 
mul $t3, $t2, $t1
sw $t3, t2L 
lw $t1, t2L 
sw $t1,  bL
lw $a0,  bL
li $v0, 1 
syscall   
li $v0, 10 
syscall   
.data
aL: .space	4
bL: .space	4
t1L: .space	4
t2L: .space	4
