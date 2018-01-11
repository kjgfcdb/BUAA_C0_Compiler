############################## Global String ##############################
.data
_str0 :  .asciiz "OVERFLOW! "
_str1 :  .asciiz "complete number: "
_str2 :  .asciiz "  "
_str3 :  .asciiz "  "
_str4 :  .asciiz "---------------------------------------------------------------\\n"
_str5 :  .asciiz " "
_str6 :  .asciiz "  "
_str7 :  .asciiz "The total is "
##############################  Text Segment ##############################
.text
# initialize global constant
	addu $gp,$sp,$0 # let $gp=$sp to get more space
	subi $gp,$gp,4 # make space for global variabel and constant
# initialize global constant
	li $t0,1024 # MAX_NUM=1024
	sw $t0,0($gp)
	addu $sp,$gp,$0 # update $sp
	j Func_1 # jump to main function
Func_0: # function complete_num
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addi $fp,$sp,4 # $fp = old $sp
	subi $sp,$sp,4156 # make space for locals and temps
	li $s3,2 # $s3=2
Label_0:
	li $t0,0
	li $t1,1
	subu $s1,$t0,$t1 # n = 0-1
	li $s4,1 # $s4=1
	addu $s0,$0,$s3 # $s0=j
Label_1:
	div $t0,$s3,$s4 # #reg4 = j/i
	mul $t1,$t0,$s4 # x1 = #reg4*i
	sw $t1,-4120($fp) # spill x1
	bne $t1,$s3,Label_2 # x1!=j
	subu $s0,$s0,$s4 # s = s-i
	li $t0,1
	addu $s1,$s1,$t0 # n = n+1
	li $t1,1024
	blt $s1,$t1,Label_3 # n<1024
	li $v0,4 # print string
	la $a0,_str0
	syscall 
	j Label_4
Label_3:
	sll $t0,$s1,2
	subi $t1,$fp,8
	subu $t1,$t1,$t0
	sw $s4,0($t1) # k[n] = i
Label_4:
	j Label_5
Label_2:
Label_5:
	li $t0,1
	addu $s4,$s4,$t0 # i = i+1
	blt $s4,$s3,Label_1 # i<j
	li $t0,0
	bne $s0,$t0,Label_6 # s!=0
	li $v0,4 # print string
	la $a0,_str1
	syscall 
	li $v0,1# print int : j
	addu $a0,$0,$s3
	syscall 
	li $s4,0 # $s4=0
Label_7:
	addu $t0,$0,$s4 # $t0=i
	li $t1,1
	addu $s4,$t0,$t1 # i = #reg47+1
	sll $t2,$t0,2
	subi $t3,$fp,8
	subu $t3,$t3,$t2
	lw $t4,0($t3) # #reg20=k[#reg47]
	li $v0,4 # print string
	la $a0,_str2
	syscall 
	li $v0,1# print int : #reg20
	addu $a0,$0,$t4
	syscall 
	ble $s4,$s1,Label_7 # i<=n
	li $v0,4 # print string
	la $a0,_str3
	syscall 
	j Label_8
Label_6:
Label_8:
	li $t0,1
	addu $s3,$s3,$t0 # j = j+1
	lw $t1,0($gp) # load $t1 = MAX_NUM
	blt $s3,$t1,Label_0 # j<MAX_NUM
	li $v0,4 # print string
	la $a0,_str4
	syscall 
	li $s3,0 # $s3=0
	li $s1,1 # $s1=1
	li $s0,2 # $s0=2
Label_9:
	li $t0,1
	subu $s2,$s0,$t0 # k2 = m-1
	li $s4,2 # $s4=2
Label_10:
	div $t0,$s0,$s4 # #reg30 = m/i
	mul $t1,$t0,$s4 # x2 = #reg30*i
	sw $t1,-4140($fp) # spill x2
	bne $t1,$s0,Label_11 # x2!=m
	li $s1,0 # $s1=0
	j Label_12
Label_11:
Label_12:
	li $t0,1
	addu $s4,$s4,$t0 # i = i+1
	ble $s4,$s2,Label_10 # i<=k2
	li $t0,1
	bne $s1,$t0,Label_13 # leap!=1
	li $v0,4 # print string
	la $a0,_str5
	syscall 
	li $v0,1# print int : m
	addu $a0,$0,$s0
	syscall 
	li $t0,1
	addu $s3,$s3,$t0 # h = h+1
	li $t1,10
	div $t2,$s3,$t1 # #reg40 = h/10
	mul $t3,$t2,$t1 # x2 = #reg40*10
	sw $t3,-4140($fp) # spill x2
	bne $t3,$s3,Label_14 # x2!=h
	li $v0,4 # print string
	la $a0,_str6
	syscall 
	j Label_15
Label_14:
Label_15:
	j Label_16
Label_13:
Label_16:
	li $t0,1
	addu $s0,$s0,$t0 # m = m+1
	li $s1,1 # $s1=1
	lw $t1,0($gp) # load $t1 = MAX_NUM
	ble $s0,$t1,Label_9 # m<=MAX_NUM
	li $v0,4 # print string
	la $a0,_str7
	syscall 
	li $v0,1# print int : h
	addu $a0,$0,$s3
	syscall 
exit_Func_0: # exit function complete_num
	addi $sp,$sp,4156 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addi $sp,$sp,8
	jr $ra
Func_1: # function main
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addi $fp,$sp,4 # $fp = old $sp
	jal Func_0
exit_Func_1: # exit function main
	lw $fp,4($sp)
	lw $ra,0($sp)
	addi $sp,$sp,8
	addi $sp,$sp,4
	li $gp,0x10008000 # restore $gp
