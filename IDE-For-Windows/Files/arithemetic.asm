.text
la $a0,  cL
li $v0, 8 
syscall   
li $t1, 0 
sw $t1,  xL
li $t1, 0 
sw $t1,  yL
L2:   
lw $t0,  yL
la $t2,  cL
add $t1, $t0, $t2
lb $t3, $t1 
la $t4,  t1L
sb $t3, $t4 
li $t1, 0 
lw $t2, t1L 
bne $t1, $t2, L3
b   L1
L3:   
lw $t1, yL 
li $t2, 1 
add $t3, $t2, $t1
sw $t3, t2L 
lw $t1, t2L 
sw $t1,  yL
b   L2
L1:   
lw $t1, yL 
li $t2, 1 
sub $t3, $t1, $t2
sw $t3, t3L 
lw $t1, t3L 
sw $t1,  yL
L6:   
lw $t0,  xL
la $t2,  cL
add $t1, $t0, $t2
lb $t3, $t1 
la $t4,  t4L
sb $t3, $t4 
lw $t0,  yL
la $t2,  cL
add $t1, $t0, $t2
lb $t3, $t1 
la $t4,  t5L
sb $t3, $t4 
lw $t1, t4L 
lw $t2, t5L 
beq $t1, $t2, L9
b   L5
L9:   
lw $t1, yL 
lw $t2, xL 
bgt $t1, $t2, L7
b   L5
L7:   
lw $t1, yL 
li $t2, 1 
sub $t3, $t1, $t2
sw $t3, t6L 
lw $t1, t6L 
sw $t1,  yL
lw $t1, xL 
li $t2, 1 
add $t3, $t2, $t1
sw $t3, t7L 
lw $t1, t7L 
sw $t1,  xL
b   L6
L5:   
lw $t1, xL 
lw $t2, yL 
bge $t1, $t2, L10
b   L11
L10:   
la $a0  Message1
li $v0, 4 
syscall   
b   L12
L11:   
la $a0  Message2
li $v0, 4 
syscall   
L12:   
li $v0, 10 
syscall   
.data
cL: .byte	400
aL: .byte	1
bL: .byte	1
xL: .space	4
yL: .space	4
t1L: .space	4
t2L: .space	4
t3L: .space	4
t4L: .space	4
t5L: .space	4
t6L: .space	4
t7L: .space	4
Message1: .asciiz	"Is palindrome"
Message2: .asciiz	"Is not palindrome"
