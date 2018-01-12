############################## Global String ##############################
.data
_str0 :  .asciiz "variable and const defination passed!"
_str1 :  .asciiz "c is "
_str2 :  .asciiz "fuck the world"
_str3 :  .asciiz " !#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
_str4 :  .asciiz "please input the mode before test(1~3)"
_str5 :  .asciiz "Easy"
_str6 :  .asciiz "Middle"
_str7 :  .asciiz "Hard"
_str8 :  .asciiz "fuck the world"
_str9 :  .asciiz "The character a in str_test is encountered "
_str10 :  .asciiz "The sum of input numbers is "
_str11 :  .asciiz "The cov of input number is "
_str12 :  .asciiz "quick sort failed!"
_str13 :  .asciiz "quick sort success!"
##############################  Text Segment ##############################
.text
# initialize global constant
	addu $gp,$sp,$0 # let $gp=$sp to get more space
	subi $gp,$gp,484 # make space for global variabel and constant
# initialize global constant
	li $t0,5 # a=5
	sw $t0,0($gp)
	li $t0,51 # m=51
	sw $t0,4($gp)
	li $t0,6 # z=6
	sw $t0,8($gp)
	li $t0,10 # b=10
	sw $t0,12($gp)
	li $t0,115 # q=115
	sw $t0,16($gp)
	li $t0,116 # p=116
	sw $t0,20($gp)
	addu $sp,$gp,$0 # update $sp
	j Func_12 # jump to main function
Func_0: # function defi
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addi $fp,$sp,8 # $fp = old $sp
	subi $sp,$sp,512 # make space for locals and temps
	li $t0,100 # const D1=100
	sw $t0,-12($fp)
	li $t0,43 # const G1=43
	sw $t0,-24($fp)
	li $t0,5
	sll $t1,$t0,2
	subi $t2,$fp,48
	subu $t2,$t2,$t1
	lw $t3,0($fp) # load $t3 = value
	sw $t3,0($t2) # d[5] = value
	li $v0,4 # print string
	la $a0,_str0
	syscall 
exit_Func_0: # exit function defi
	addi $sp,$sp,512 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addi $sp,$sp,12
	jr $ra
Func_1: # function defi_test
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addi $fp,$sp,4 # $fp = old $sp
	li $t0,66
	subi $sp,$sp,4
	sw $t0,0($sp)
	jal Func_0
exit_Func_1: # exit function defi_test
	lw $fp,4($sp)
	lw $ra,0($sp)
	addi $sp,$sp,8
	jr $ra
Func_2: # function max
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addi $fp,$sp,12 # $fp = old $sp
	subi $sp,$sp,12 # make space for locals and temps
	lw $t0,0($fp) # load $t0 = a
	lw $t1,-4($fp) # load $t1 = b
	ble $t0,$t1,Label_0 # a<=b
	lw $v1,0($fp) # @RET = a
	j exit_Func_2
	j Label_1
Label_0:
Label_1:
	lw $t0,-4($fp) # load $t0 = b
	lw $t1,0($fp) # load $t1 = a
	ble $t0,$t1,Label_2 # b<=a
	lw $v1,-4($fp) # @RET = b
	j exit_Func_2
	j Label_3
Label_2:
Label_3:
	li $t0,0
	lw $t1,-4($fp) # load $t1 = b
	subu $t2,$t0,$t1 # #reg13 = 0-b
	lw $t3,0($fp) # load $t3 = a
	subu $t4,$t3,$t2 # #reg362 = a-#reg13
	li $t5,2
	div $t6,$t4,$t5 # #reg12 = #reg362/2
	addu $v1,$0,$t6 # @RET = #reg12
	j exit_Func_2
exit_Func_2: # exit function max
	addi $sp,$sp,12 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addi $sp,$sp,16
	jr $ra
Func_3: # function fun_def_test
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addi $fp,$sp,4 # $fp = old $sp
	subi $sp,$sp,4 # make space for locals and temps
	li $t0,5
	subi $sp,$sp,4
	sw $t0,0($sp)
	subi $sp,$sp,4
	sw $t0,0($sp)
	jal Func_2
	addu $t0,$0,$v1
	subi $sp,$sp,4
	sw $t0,0($sp)
	li $t1,5
	subi $sp,$sp,4
	sw $t1,0($sp)
	jal Func_2
	addu $t0,$0,$v1
	subi $sp,$sp,4
	sw $t0,0($sp)
	li $t1,5
	subi $sp,$sp,4
	sw $t1,0($sp)
	jal Func_2
	li $v0,4 # print string
	la $a0,_str1
	syscall 
	li $v0,1# print int : @RET
	addu $a0,$0,$v1
	syscall 
exit_Func_3: # exit function fun_def_test
	addi $sp,$sp,4 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addi $sp,$sp,8
	jr $ra
Func_4: # function calcu
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addi $fp,$sp,8 # $fp = old $sp
	subi $sp,$sp,20 # make space for locals and temps
	li $s2,0 # $s2=0
	addu $s1,$0,$s2 # $s1=i
	addu $s0,$0,$s1 # $s0=sum
Label_4:
	addu $t0,$0,$s2 # $t0=i
	li $t1,1
	addu $s0,$s0,$t1 # toy = toy+1
	addu $s2,$t0,$t1 # i = #reg363+1
	sll $t2,$t0,2
	addiu $t3,$gp,228
	addu $t3,$t3,$t2
	lw $t4,0($t3) # #reg26=number[#reg363]
	addu $s1,$s1,$t4 # sum = sum+#reg26
	lw $t5,0($fp) # load $t5 = size
	blt $s2,$t5,Label_4 # i<size
	addu $v1,$0,$s1 # @RET = sum
	j exit_Func_4
exit_Func_4: # exit function calcu
	addi $sp,$sp,20 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addi $sp,$sp,12
	jr $ra
Func_5: # function calcuCov
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addi $fp,$sp,12 # $fp = old $sp
	subi $sp,$sp,28 # make space for locals and temps
	li $s1,0 # $s1=0
	addu $s2,$0,$s1 # $s2=i
	addu $s0,$0,$s2 # $s0=cov
Label_5:
	addu $t0,$0,$s1 # $t0=i
	li $t1,1
	addu $s0,$s0,$t1 # toy = toy+1
	addu $s1,$t0,$t1 # i = #reg364+1
	sll $t2,$t0,2
	addiu $t3,$gp,228
	addu $t3,$t3,$t2
	lw $t4,0($t3) # #reg365=number[#reg364]
	lw $t5,-4($fp) # load $t5 = aver
	subu $t6,$t4,$t5 # #reg35 = #reg365-aver
	mul $t7,$t6,$t6 # temp = #reg35*#reg35
	addu $s2,$s2,$t7 # cov = cov+temp
	lw $t8,0($fp) # load $t8 = size
	blt $s1,$t8,Label_5 # i<size
	addu $v1,$0,$s2 # @RET = cov
	j exit_Func_5
exit_Func_5: # exit function calcuCov
	addi $sp,$sp,28 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addi $sp,$sp,16
	jr $ra
Func_6: # function partion
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addi $fp,$sp,12 # $fp = old $sp
	subi $sp,$sp,40 # make space for locals and temps
	lw $t0,0($fp) # load $t0 = sta
	li $t1,1
	subu $s4,$t0,$t1 # i = sta-1
	lw $t2,-4($fp) # load $t2 = end
	sll $t3,$t2,2
	addiu $t4,$gp,228
	addu $t4,$t4,$t3
	lw $s0,0($t4) # x=number[end]
	addu $s3,$0,$t0 # $s3=sta
	li $s1,0 # $s1=0
Label_6:
	sll $t0,$s3,2
	addiu $t1,$gp,228
	addu $t1,$t1,$t0
	lw $t2,0($t1) # #reg49=number[j]
	bgt $t2,$s0,Label_7 # #reg49>x
	sll $t0,$s3,2
	addiu $t1,$gp,228
	addu $t1,$t1,$t0
	lw $t2,0($t1) # #reg56=number[j]
	li $t3,1
	addu $s4,$s4,$t3 # i = i+1
	sll $t4,$s4,2
	addiu $t5,$gp,228
	addu $t5,$t5,$t4
	lw $s2,0($t5) # temp=number[i]
	sll $t6,$s4,2
	addiu $t7,$gp,228
	addu $t7,$t7,$t6
	sw $t2,0($t7) # number[i] = #reg56
	sll $t0,$s3,2
	addiu $t1,$gp,228
	addu $t1,$t1,$t0
	sw $s2,0($t1) # number[j] = temp
	j Label_8
Label_7:
Label_8:
	li $t0,1
	addu $s1,$s1,$t0 # toy = toy+1
	addu $s3,$s3,$t0 # j = j+1
	lw $t1,-4($fp) # load $t1 = end
	blt $s3,$t1,Label_6 # j<end
	lw $t0,-4($fp) # load $t0 = end
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	lw $t3,0($t2) # #reg66=number[end]
	li $t4,1
	addu $t5,$s4,$t4 # #reg62 = i+1
	sll $t6,$t5,2
	addiu $t7,$gp,228
	addu $t7,$t7,$t6
	lw $s2,0($t7) # temp=number[#reg62]
	sll $t8,$t5,2
	addiu $t9,$gp,228
	addu $t9,$t9,$t8
	sw $t3,0($t9) # number[#reg62] = #reg66
	lw $t0,-4($fp) # load $t0 = end
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	sw $s2,0($t2) # number[end] = temp
	li $t0,1
	addu $t1,$s4,$t0 # #reg69 = i+1
	addu $v1,$0,$t1 # @RET = #reg69
	j exit_Func_6
exit_Func_6: # exit function partion
	addi $sp,$sp,40 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addi $sp,$sp,16
	jr $ra
Func_7: # function quick_sort
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addi $fp,$sp,12 # $fp = old $sp
	subi $sp,$sp,12 # make space for locals and temps
	lw $t0,0($fp) # load $t0 = sta
	lw $t1,-4($fp) # load $t1 = end
	bge $t0,$t1,Label_9 # sta>=end
	lw $t0,0($fp) # load $t0 = sta
	subi $sp,$sp,4
	sw $t0,0($sp)
	lw $t1,-4($fp) # load $t1 = end
	subi $sp,$sp,4
	sw $t1,0($sp)
	jal Func_6
	lw $t0,0($fp) # load $t0 = sta
	subi $sp,$sp,4
	sw $t0,0($sp)
	addu $t1,$0,$v1
	li $t2,1
	subu $t3,$t1,$t2 # #reg76 = @RET-1
	addu $t4,$0,$v1 # $t4=@RET
	subi $sp,$sp,4
	sw $t3,0($sp)
	sw $t4,-16($fp) # spill q
	jal Func_7
	lw $t0,-16($fp) # load $t0 = q
	li $t1,1
	addu $t2,$t0,$t1 # #reg77 = q+1
	subi $sp,$sp,4
	sw $t2,0($sp)
	lw $t3,-4($fp) # load $t3 = end
	subi $sp,$sp,4
	sw $t3,0($sp)
	jal Func_7
	j Label_10
Label_9:
Label_10:
exit_Func_7: # exit function quick_sort
	addi $sp,$sp,12 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addi $sp,$sp,16
	jr $ra
Func_8: # function reverse
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addi $fp,$sp,8 # $fp = old $sp
	subi $sp,$sp,32 # make space for locals and temps
	li $s2,0 # $s2=0
	addu $s0,$0,$s2 # $s0=i
Label_11:
	lw $t0,0($fp) # load $t0 = len
	subu $t1,$t0,$s2 # #reg84 = len-i
	sll $t2,$t1,2
	addiu $t3,$gp,28
	addu $t3,$t3,$t2
	lw $t4,0($t3) # #reg85=str_test[#reg84]
	sll $t5,$s2,2
	addiu $t6,$gp,28
	addu $t6,$t6,$t5
	lw $s1,0($t6) # temp=str_test[i]
	sll $t7,$s2,2
	addiu $t8,$gp,28
	addu $t8,$t8,$t7
	sw $t4,0($t8) # str_test[i] = #reg85
	lw $t0,0($fp) # load $t0 = len
	subu $t1,$t0,$s2 # #reg86 = len-i
	sll $t2,$t1,2
	addiu $t3,$gp,28
	addu $t3,$t3,$t2
	sw $s1,0($t3) # str_test[#reg86] = temp
	lw $t0,0($fp) # load $t0 = len
	li $t1,2
	div $t2,$t0,$t1 # #reg90 = len/2
	li $t3,1
	addu $s0,$s0,$t3 # toy = toy+1
	li $t4,0
	addu $t5,$t4,$t3 # #reg88 = 0+1
	addu $s2,$s2,$t5 # i = i+#reg88
	blt $s2,$t2,Label_11 # i<#reg90
exit_Func_8: # exit function reverse
	addi $sp,$sp,32 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addi $sp,$sp,12
	jr $ra
Func_9: # function charCount
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addi $fp,$sp,4 # $fp = old $sp
	subi $sp,$sp,24 # make space for locals and temps
	li $s2,0 # $s2=0
	addu $s3,$0,$s2 # $s3=sum
	addu $s0,$0,$s3 # $s0=i
Label_12:
	sll $t0,$s3,2
	addiu $t1,$gp,28
	addu $t1,$t1,$t0
	lw $s1,0($t1) # #reg95=str_test[i]
Label_14:
	li $t0,'S'
	bne $s1,$t0,Label_15 # #reg95!='S'
	li $s1,0 # $s1=0
Label_16:
	sll $t0,$s3,2
	addiu $t1,$gp,28
	addu $t1,$t1,$t0
	lw $t2,0($t1) # #reg98=str_test[i]
	li $t3,'a'
	bne $t2,$t3,Label_17 # #reg98!='a'
	li $t0,1
	addu $s2,$s2,$t0 # sum = sum+1
	j Label_18
Label_17:
Label_18:
	li $t0,1
	addu $s1,$s1,$t0 # t = t+1
	addu $s3,$s3,$t0 # i = i+1
	li $t1,10
	ble $s3,$t1,Label_16 # i<=10
	j Label_13
Label_15:
	li $t0,'E'
	bne $s1,$t0,Label_19 # #reg95!='E'
	addu $v1,$0,$s2 # @RET = sum
	j exit_Func_9
	j Label_13
Label_19:
	li $v0,4 # print string
	la $a0,_str2
	syscall 
	j Label_13
Label_13:
	li $t0,1
	addu $s0,$s0,$t0 # toy = toy+1
	addu $s3,$s3,$t0 # i = i+1
	li $t1,0
	bne $t0,$t1,Label_12 # 1!=0
exit_Func_9: # exit function charCount
	addi $sp,$sp,24 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addi $sp,$sp,8
	jr $ra
Func_10: # function other_test1
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addi $fp,$sp,4 # $fp = old $sp
	subi $sp,$sp,8 # make space for locals and temps
	li $v0,4 # print string
	la $a0,_str3
	syscall 
	li $v0,4 # print string
	la $a0,_str4
	syscall 
	li $v0,12 # read char : score
	syscall 
	sw $v0,-8($fp)
	lw $t0,-8($fp) # load $t0 = score
	addu $s0,$0,$t0 # $s0=score
Label_21:
	li $t0,'1'
	bne $s0,$t0,Label_22 # #reg107!='1'
	li $v0,4 # print string
	la $a0,_str5
	syscall 
	li $t0,0
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,5
	sw $t3,0($t2) # number[0] = 5
	li $t0,1
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,7
	sw $t3,0($t2) # number[1] = 7
	li $t0,2
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,9
	sw $t3,0($t2) # number[2] = 9
	li $t0,3
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,4
	sw $t3,0($t2) # number[3] = 4
	li $t0,4
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,1
	sw $t3,0($t2) # number[4] = 1
	li $t0,5
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,3
	sw $t3,0($t2) # number[5] = 3
	li $t0,6
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,8
	sw $t3,0($t2) # number[6] = 8
	li $t0,7
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,10
	sw $t3,0($t2) # number[7] = 10
	li $t0,8
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,2
	sw $t3,0($t2) # number[8] = 2
	li $t0,9
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,6
	sw $t3,0($t2) # number[9] = 6
	li $t0,0
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,1
	sw $t3,0($t2) # ans[0] = 1
	li $t0,1
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,2
	sw $t3,0($t2) # ans[1] = 2
	li $t0,2
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,3
	sw $t3,0($t2) # ans[2] = 3
	li $t0,3
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,4
	sw $t3,0($t2) # ans[3] = 4
	li $t0,4
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,5
	sw $t3,0($t2) # ans[4] = 5
	li $t0,5
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,6
	sw $t3,0($t2) # ans[5] = 6
	li $t0,6
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,7
	sw $t3,0($t2) # ans[6] = 7
	li $t0,7
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,8
	sw $t3,0($t2) # ans[7] = 8
	li $t0,8
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,9
	sw $t3,0($t2) # ans[8] = 9
	li $t0,9
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,10
	sw $t3,0($t2) # ans[9] = 10
	j Label_20
Label_22:
	li $t0,'2'
	bne $s0,$t0,Label_23 # #reg107!='2'
	li $v0,4 # print string
	la $a0,_str6
	syscall 
	lw $t0,468($gp) # load $t0 = ra
	li $t1,5
	addu $t0,$t0,$t1 # ra = ra+5
	li $t2,0
	sll $t3,$t2,2
	addiu $t4,$gp,228
	addu $t4,$t4,$t3
	li $t5,45
	sw $t5,0($t4) # number[0] = 45
	sw $t0,468($gp) # spill ra
	li $t0,1
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,19
	sw $t3,0($t2) # number[1] = 19
	li $t0,2
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,23
	sw $t3,0($t2) # number[2] = 23
	li $t0,3
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,78
	sw $t3,0($t2) # number[3] = 78
	li $t0,4
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,83
	sw $t3,0($t2) # number[4] = 83
	li $t0,5
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,55
	sw $t3,0($t2) # number[5] = 55
	li $t0,6
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,69
	sw $t3,0($t2) # number[6] = 69
	li $t0,7
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,37
	sw $t3,0($t2) # number[7] = 37
	li $t0,8
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,95
	sw $t3,0($t2) # number[8] = 95
	li $t0,9
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,77
	sw $t3,0($t2) # number[9] = 77
	li $t0,10
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,145
	sw $t3,0($t2) # number[10] = 145
	li $t0,11
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,189
	sw $t3,0($t2) # number[11] = 189
	li $t0,12
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,156
	sw $t3,0($t2) # number[12] = 156
	li $t0,13
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,123
	sw $t3,0($t2) # number[13] = 123
	li $t0,14
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,100
	sw $t3,0($t2) # number[14] = 100
	li $t0,0
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,19
	sw $t3,0($t2) # ans[0] = 19
	li $t0,1
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,23
	sw $t3,0($t2) # ans[1] = 23
	li $t0,2
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,37
	sw $t3,0($t2) # ans[2] = 37
	li $t0,3
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,45
	sw $t3,0($t2) # ans[3] = 45
	li $t0,4
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,55
	sw $t3,0($t2) # ans[4] = 55
	li $t0,5
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,69
	sw $t3,0($t2) # ans[5] = 69
	li $t0,6
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,77
	sw $t3,0($t2) # ans[6] = 77
	li $t0,7
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,78
	sw $t3,0($t2) # ans[7] = 78
	li $t0,8
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,83
	sw $t3,0($t2) # ans[8] = 83
	li $t0,9
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,95
	sw $t3,0($t2) # ans[9] = 95
	li $t0,10
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,100
	sw $t3,0($t2) # ans[10] = 100
	li $t0,11
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,123
	sw $t3,0($t2) # ans[11] = 123
	li $t0,12
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,145
	sw $t3,0($t2) # ans[12] = 145
	li $t0,13
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,156
	sw $t3,0($t2) # ans[13] = 156
	li $t0,14
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,189
	sw $t3,0($t2) # ans[14] = 189
	j Label_20
Label_23:
	li $t0,'3'
	bne $s0,$t0,Label_24 # #reg107!='3'
	li $v0,4 # print string
	la $a0,_str7
	syscall 
	lw $t0,468($gp) # load $t0 = ra
	li $t1,10
	addu $t0,$t0,$t1 # ra = ra+10
	li $t2,0
	sll $t3,$t2,2
	addiu $t4,$gp,228
	addu $t4,$t4,$t3
	li $t5,120
	sw $t5,0($t4) # number[0] = 120
	sw $t0,468($gp) # spill ra
	li $t0,1
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,854
	sw $t3,0($t2) # number[1] = 854
	li $t0,2
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,165
	sw $t3,0($t2) # number[2] = 165
	li $t0,3
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,785
	sw $t3,0($t2) # number[3] = 785
	li $t0,4
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,666
	sw $t3,0($t2) # number[4] = 666
	li $t0,5
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,475
	sw $t3,0($t2) # number[5] = 475
	li $t0,6
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,324
	sw $t3,0($t2) # number[6] = 324
	li $t0,7
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,218
	sw $t3,0($t2) # number[7] = 218
	li $t0,8
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,165
	sw $t3,0($t2) # number[8] = 165
	li $t0,9
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,994
	sw $t3,0($t2) # number[9] = 994
	li $t0,10
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,1230
	sw $t3,0($t2) # number[10] = 1230
	li $t0,11
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,1542
	sw $t3,0($t2) # number[11] = 1542
	li $t0,12
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,1462
	sw $t3,0($t2) # number[12] = 1462
	li $t0,13
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,1864
	sw $t3,0($t2) # number[13] = 1864
	li $t0,14
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,1023
	sw $t3,0($t2) # number[14] = 1023
	li $t0,15
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,1000
	sw $t3,0($t2) # number[15] = 1000
	li $t0,16
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,1765
	sw $t3,0($t2) # number[16] = 1765
	li $t0,17
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,1952
	sw $t3,0($t2) # number[17] = 1952
	li $t0,18
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,1949
	sw $t3,0($t2) # number[18] = 1949
	li $t0,19
	sll $t1,$t0,2
	addiu $t2,$gp,228
	addu $t2,$t2,$t1
	li $t3,1764
	sw $t3,0($t2) # number[19] = 1764
	li $t0,0
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,120
	sw $t3,0($t2) # ans[0] = 120
	li $t0,1
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,165
	sw $t3,0($t2) # ans[1] = 165
	li $t0,2
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,165
	sw $t3,0($t2) # ans[2] = 165
	li $t0,3
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,218
	sw $t3,0($t2) # ans[3] = 218
	li $t0,4
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,324
	sw $t3,0($t2) # ans[4] = 324
	li $t0,5
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,475
	sw $t3,0($t2) # ans[5] = 475
	li $t0,6
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,666
	sw $t3,0($t2) # ans[6] = 666
	li $t0,7
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,785
	sw $t3,0($t2) # ans[7] = 785
	li $t0,8
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,854
	sw $t3,0($t2) # ans[8] = 854
	li $t0,9
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,994
	sw $t3,0($t2) # ans[9] = 994
	li $t0,10
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,1000
	sw $t3,0($t2) # ans[10] = 1000
	li $t0,11
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,1023
	sw $t3,0($t2) # ans[11] = 1023
	li $t0,12
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,1230
	sw $t3,0($t2) # ans[12] = 1230
	li $t0,13
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,1462
	sw $t3,0($t2) # ans[13] = 1462
	li $t0,14
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,1542
	sw $t3,0($t2) # ans[14] = 1542
	li $t0,15
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,1764
	sw $t3,0($t2) # ans[15] = 1764
	li $t0,16
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,1765
	sw $t3,0($t2) # ans[16] = 1765
	li $t0,17
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,1864
	sw $t3,0($t2) # ans[17] = 1864
	li $t0,18
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,1949
	sw $t3,0($t2) # ans[18] = 1949
	li $t0,19
	sll $t1,$t0,2
	addiu $t2,$gp,348
	addu $t2,$t2,$t1
	li $t3,1952
	sw $t3,0($t2) # ans[19] = 1952
	j Label_20
Label_24:
	li $v0,4 # print string
	la $a0,_str8
	syscall 
	j Label_20
Label_20:
exit_Func_10: # exit function other_test1
	addi $sp,$sp,8 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addi $sp,$sp,8
	jr $ra
Func_11: # function other_test2
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addi $fp,$sp,4 # $fp = old $sp
	subi $sp,$sp,76 # make space for locals and temps
	li $t0,0
	sll $t1,$t0,2
	addiu $t2,$gp,28
	addu $t2,$t2,$t1
	li $t3,'S'
	sw $t3,0($t2) # str_test[0] = 'S'
	li $t0,1
	sll $t1,$t0,2
	addiu $t2,$gp,28
	addu $t2,$t2,$t1
	li $t3,'a'
	sw $t3,0($t2) # str_test[1] = 'a'
	li $t0,2
	sll $t1,$t0,2
	addiu $t2,$gp,28
	addu $t2,$t2,$t1
	li $t3,'a'
	sw $t3,0($t2) # str_test[2] = 'a'
	li $t0,3
	sll $t1,$t0,2
	addiu $t2,$gp,28
	addu $t2,$t2,$t1
	li $t3,'a'
	sw $t3,0($t2) # str_test[3] = 'a'
	li $t0,4
	sll $t1,$t0,2
	addiu $t2,$gp,28
	addu $t2,$t2,$t1
	li $t3,'a'
	sw $t3,0($t2) # str_test[4] = 'a'
	li $t0,5
	sll $t1,$t0,2
	addiu $t2,$gp,28
	addu $t2,$t2,$t1
	li $t3,'a'
	sw $t3,0($t2) # str_test[5] = 'a'
	li $t0,6
	sll $t1,$t0,2
	addiu $t2,$gp,28
	addu $t2,$t2,$t1
	li $t3,'b'
	sw $t3,0($t2) # str_test[6] = 'b'
	li $t0,7
	sll $t1,$t0,2
	addiu $t2,$gp,28
	addu $t2,$t2,$t1
	li $t3,'b'
	sw $t3,0($t2) # str_test[7] = 'b'
	li $t0,8
	sll $t1,$t0,2
	addiu $t2,$gp,28
	addu $t2,$t2,$t1
	li $t3,'a'
	sw $t3,0($t2) # str_test[8] = 'a'
	li $t0,9
	sll $t1,$t0,2
	addiu $t2,$gp,28
	addu $t2,$t2,$t1
	li $t3,'b'
	sw $t3,0($t2) # str_test[9] = 'b'
	li $t0,10
	sll $t1,$t0,2
	addiu $t2,$gp,28
	addu $t2,$t2,$t1
	li $t3,'b'
	sw $t3,0($t2) # str_test[10] = 'b'
	li $t0,11
	sll $t1,$t0,2
	addiu $t2,$gp,28
	addu $t2,$t2,$t1
	li $t3,'E'
	sw $t3,0($t2) # str_test[11] = 'E'
	li $t0,12
	sll $t1,$t0,2
	addiu $t2,$gp,28
	addu $t2,$t2,$t1
	li $t3,'E'
	sw $t3,0($t2) # str_test[12] = 'E'
	li $t0,12
	subi $sp,$sp,4
	sw $t0,0($sp)
	jal Func_8
	jal Func_9
	li $v0,4 # print string
	la $a0,_str9
	syscall 
	li $v0,1# print int : @RET
	addu $a0,$0,$v1
	syscall 
	li $t0,0
	lw $t1,0($gp) # load $t1 = a
	subu $t2,$t0,$t1 # #reg366 = 0-a
	addu $t3,$t1,$t2 # #reg323 = a+#reg366
	li $t4,10 # $t4=10
	li $t5,10 # $t5=10
	li $t6,10 # $t6=10
	li $t7,10 # $t7=10
	sll $t8,$t0,2
	addiu $t9,$gp,24
	addu $t9,$t9,$t8
	sw $t3,0($t9) # ss[0] = #reg323
	sw $t4,468($gp) # spill ra
	sw $t5,472($gp) # spill rb
	sw $t6,476($gp) # spill rc
	sw $t7,480($gp) # spill rd
	lw $t0,468($gp) # load $t0 = ra
	lw $t1,480($gp) # load $t1 = rd
	addu $t2,$t0,$t1 # #reg326 = ra+rd
	lw $t3,472($gp) # load $t3 = rb
	lw $t4,476($gp) # load $t4 = rc
	mul $t5,$t3,$t4 # #reg367 = rb*rc
	addu $t6,$t5,$t2 # #reg368 = #reg367+#reg326
	li $t7,88
	addu $t8,$t6,$t7 # #reg325 = #reg368+88
	addu $t9,$t0,$t5 # #reg369 = ra+#reg367
	addu $t0,$t9,$t1 # #reg370 = #reg369+rd
	div $s0,$t0,$t8 # #reg324 = #reg370/#reg325
	sw $t0,-48($fp)
	lw $t0,468($gp) # load $t0 = ra
	subi $sp,$sp,4
	sw $t0,0($sp)
	subi $sp,$sp,4
	sw $t3,0($sp)
	sw $s0,-52($fp) # save #reg324(global reg)
	jal Func_2
	lw $s0,-52($fp) # save #reg324(global reg)
	li $t0,0
	li $t1,70
	subu $t2,$t0,$t1 # #reg334 = 0-70
	sll $t3,$t0,2
	addiu $t4,$gp,24
	addu $t4,$t4,$t3
	lw $t5,0($t4) # #reg332=ss[0]
	sll $t6,$t5,2
	addiu $t7,$gp,28
	addu $t7,$t7,$t6
	lw $t8,0($t7) # #reg333=str_test[#reg332]
	addu $t9,$0,$v1
	addu $t0,$s0,$t9 # #reg371 = #reg324+@RET
	subu $t1,$t0,$t8 # #reg372 = #reg371-#reg333
	sw $t2,-56($fp)
	li $t2,'H'
	addu $s0,$t1,$t2 # #reg324 = #reg372+'H'
	lw $t2,-56($fp) # load $t2 = #reg334
	addu $t3,$s0,$t2 # ra = #reg324+#reg334
	li $s0,0 # $s0=0
	sw $t3,468($gp) # spill ra
Label_25:
	li $t0,1
	addu $s0,$s0,$t0 # t = t+1
	li $t1,0
	li $t2,10
	subu $t3,$t1,$t2 # #reg336 = 0-10
	lw $t4,468($gp) # load $t4 = ra
	addu $t4,$t4,$t3 # ra = ra+#reg336
	li $t5,20
	sw $t4,468($gp) # spill ra
	bge $t4,$t5,Label_25 # ra>=20
	lw $t0,468($gp) # load $t0 = ra
	li $t1,20
	bge $t0,$t1,Label_26 # ra>=20
	li $t0,0
	li $t1,10
	addu $t2,$t0,$t1 # #reg341 = 0+10
	addu $t3,$t2,$t0 # ra = #reg341+0
	sw $t3,468($gp) # spill ra
	j Label_27
Label_26:
Label_27:
exit_Func_11: # exit function other_test2
	addi $sp,$sp,76 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addi $sp,$sp,8
	jr $ra
Func_12: # function main
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addi $fp,$sp,4 # $fp = old $sp
	subi $sp,$sp,32 # make space for locals and temps
	jal Func_11
	jal Func_10
	lw $t0,468($gp) # load $t0 = ra
	subi $sp,$sp,4
	sw $t0,0($sp)
	jal Func_4
	addu $t0,$0,$v1
	lw $t1,468($gp) # load $t1 = ra
	div $t2,$t0,$t1 # aver = @RET/ra
	addu $s0,$0,$v1 # $s0=@RET
	subi $sp,$sp,4
	sw $t1,0($sp)
	subi $sp,$sp,4
	sw $t2,0($sp)
	sw $s0,-16($fp) # save sum(global reg)
	jal Func_5
	lw $s0,-16($fp) # save sum(global reg)
	lw $t0,468($gp) # load $t0 = ra
	li $t1,1
	subu $t2,$t0,$t1 # #reg352 = ra-1
	addu $t3,$0,$v1
	div $t4,$t3,$t0 # cov = @RET/ra
	li $v0,4 # print string
	la $a0,_str10
	syscall 
	li $v0,1# print int : sum
	addu $a0,$0,$s0
	syscall 
	li $v0,4 # print string
	la $a0,_str11
	syscall 
	li $v0,1# print int : cov
	addu $a0,$0,$t4
	syscall 
	li $t5,0
	subi $sp,$sp,4
	sw $t5,0($sp)
	subi $sp,$sp,4
	sw $t2,0($sp)
	jal Func_7
	li $s1,0 # $s1=0
	addu $s0,$0,$s1 # $s0=i
Label_28:
	sll $t0,$s1,2
	addiu $t1,$gp,348
	addu $t1,$t1,$t0
	lw $t2,0($t1) # #reg358=ans[i]
	sll $t3,$s1,2
	addiu $t4,$gp,228
	addu $t4,$t4,$t3
	lw $t5,0($t4) # #reg356=number[i]
	beq $t5,$t2,Label_29 # #reg356==#reg358
	li $v0,4 # print string
	la $a0,_str12
	syscall 
	j exit_Func_12
	j Label_30
Label_29:
Label_30:
	li $t0,1
	addu $s0,$s0,$t0 # t = t+1
	addu $s1,$s1,$t0 # i = i+1
	lw $t1,468($gp) # load $t1 = ra
	blt $s1,$t1,Label_28 # i<ra
	li $v0,4 # print string
	la $a0,_str13
	syscall 
	jal Func_3
	jal Func_1
	j exit_Func_12
exit_Func_12: # exit function main
	addi $sp,$sp,32 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addi $sp,$sp,8
	addi $sp,$sp,484
	li $gp,0x10008000 # restore $gp
