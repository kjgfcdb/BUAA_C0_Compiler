############################## Global String ##############################
.data
_str0 :  .asciiz "define test passed"
_str1 :  .asciiz "operator test passed"
_str2 :  .asciiz "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz"
_str3 :  .asciiz "!#$%&'()*+,-./:;<=>?@[]^_`{|}~"
_str4 :  .asciiz "dowhile test passed"
_str5 :  .asciiz "ifcondition clause error!"
_str6 :  .asciiz "ifcondition clause error"
_str7 :  .asciiz "switchcodintion clause  error!"
_str8 :  .asciiz "switchcodintion clause  error!"
_str9 :  .asciiz "error 0"
_str10 :  .asciiz "error 1"
_str11 :  .asciiz "error 2"
_str12 :  .asciiz "condition test passed"
_str13 :  .asciiz "miscellaneous test passed"
_str14 :  .asciiz "Fibonaci: Please input a unsigned positive integer ?"
_str15 :  .asciiz "Input error, try again:"
_str16 :  .asciiz "The fibonaci's answer is "
_str17 :  .asciiz "GCD: Please input two unsigned positive integer ?"
_str18 :  .asciiz "Input error, try Again: "
_str19 :  .asciiz "The gcd's answer is "
_str20 :  .asciiz "1"
_str21 :  .asciiz "*"
_str22 :  .asciiz "*"
_str23 :  .asciiz "prime_factorization: Please input a unsigned positive integer(<2^31-1) ?"
_str24 :  .asciiz "Input error, try again:"
_str25 :  .asciiz "The factor of n is "
_str26 :  .asciiz "Please input Test Type:E(Easy), (M)Middle, H(Hard): "
_str27 :  .asciiz "Try Again: "
_str28 :  .asciiz "default"
##############################  Text Segment ##############################
.text
# initialize global constant
addu $gp,$sp,$0 # let $gp=$sp to get more space
subiu $gp,$gp,20512 # make space for global variabel and constant
# initialize global constant
li $t0,1 # cst1=1
sw $t0,0($gp)
li $t0,2 # cst2=2
sw $t0,4($gp)
li $t0,3 # cst3=3
sw $t0,8($gp)
li $t0,97 # csta=97
sw $t0,12($gp)
li $t0,98 # cstb=98
sw $t0,16($gp)
li $t0,99 # cstc=99
sw $t0,20($gp)
addu $sp,$gp,$0 # update $sp
j Func_29 # jump to main function

Func_0: # function const_define
	subiu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	subiu $sp,$sp,32 # make space for locals and temps
	li $t0,1 # const a=1
	sw $t0,-8($fp)
	li $t0,0 # const b=0
	sw $t0,-12($fp)
	li $t0,-1 # const c=-1
	sw $t0,-16($fp)
	li $t0,120 # const cst23=120
	sw $t0,-20($fp)
	li $t0,122 # const cst24=122
	sw $t0,-24($fp)
	li $t0,65 # const constA0=65
	sw $t0,-28($fp)
	li $t0,49 # const cstA1=49
	sw $t0,-32($fp)
	li $t0,47 # const cst_div=47
	sw $t0,-36($fp)
	li $v0,1# print int : a
	lw $a0,-8($fp)
	syscall 
	li $v0,11 # print char : cst_div
	lw $a0,-36($fp)
	syscall 
exit_Func_0: # exit function const_define
	addiu $sp,$sp,32 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,8
	jr $ra

Func_1: # function const_define_test
	subiu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	jal Func_0
exit_Func_1: # exit function const_define_test
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,8
	jr $ra

Func_2: # function var_define
	subiu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	subiu $sp,$sp,80 # make space for locals and temps
exit_Func_2: # exit function var_define
	addiu $sp,$sp,80 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,8
	jr $ra

Func_3: # function var_define_test
	subiu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	jal Func_2
exit_Func_3: # exit function var_define_test
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,8
	jr $ra

Func_4: # function define
	subiu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	subiu $sp,$sp,24 # make space for locals and temps
	li $t0,1 # const int1=1
	sw $t0,-8($fp)
	li $t0,122 # const cst24=122
	sw $t0,-12($fp)
	li $t0,65 # const constA0=65
	sw $t0,-16($fp)
exit_Func_4: # exit function define
	addiu $sp,$sp,24 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,8
	jr $ra

Func_5: # function define_test
	subiu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	jal Func_3
	jal Func_1
	jal Func_4
	li $v0,4 # print string
	la $a0,_str0
	syscall 
exit_Func_5: # exit function define_test
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,8
	jr $ra

Func_6: # function operator
	subiu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	subiu $sp,$sp,108 # make space for locals and temps
	li $t0,1
	li $t1,0
	subu $t2,$t0,$t1 # #reg17 = 1-0
	li $t3,5
	li $t4,6
	mul $t5,$t3,$t4 # #reg14 = 5*6
	li $t6,1
	li $t7,1
	div $t8,$t6,$t7 # #reg10 = 1/1
	li $t9,0
	li $t0,4
	subu $t1,$t9,$t0 # #reg5 = 0-4
	sw $t2,-32($fp)
	li $t2,4
	div $t3,$t1,$t2 # #reg9 = #reg5/4
	sw $t3,-48($fp)
	li $t3,1
	li $t4,2
	sw $t5,-36($fp)
	addu $t5,$t3,$t4 # #reg3 = 1+2
	li $t6,3
	mul $t7,$t5,$t6 # #reg232 = #reg3*3
	sw $t7,-56($fp)
	li $t7,3
	sw $t8,-40($fp)
	li $t8,1
	mul $t9,$t7,$t8 # #reg4 = 3*1
	sw $t9,-60($fp)
	lw $t9,-56($fp) # load $t9 = #reg232
	lw $t0,-60($fp) # load $t0 = #reg4
	sw $t1,-44($fp)
	addu $t1,$t9,$t0 # #reg233 = #reg232+#reg4
	lw $t2,-48($fp) # load $t2 = #reg9
	subu $t3,$t1,$t2 # #reg234 = #reg233-#reg9
	lw $t4,-40($fp) # load $t4 = #reg10
	sw $t5,-52($fp)
	addu $t5,$t3,$t4 # #reg8 = #reg234+#reg10
	li $t6,3
	mul $t7,$t5,$t6 # #reg12 = #reg8*3
	sw $t7,-76($fp)
	li $t7,0
	addu $t8,$t7,$t5 # #reg235 = 0+#reg8
	lw $t9,-76($fp) # load $t9 = #reg12
	addu $t0,$t8,$t9 # #reg236 = #reg235+#reg12
	sw $t1,-64($fp)
	lw $t1,-60($fp) # load $t1 = #reg4
	addu $t2,$t0,$t1 # #reg237 = #reg236+#reg4
	sw $t3,-68($fp)
	lw $t3,-48($fp) # load $t3 = #reg9
	subu $t4,$t2,$t3 # #reg238 = #reg237-#reg9
	sw $t5,-72($fp)
	lw $t5,-36($fp) # load $t5 = #reg14
	addu $t6,$t4,$t5 # #reg11 = #reg238+#reg14
	li $t7,0
	sw $t8,-80($fp)
	mul $t8,$t6,$t7 # #reg16 = #reg11*0
	sw $t8,-100($fp)
	li $t8,0
	lw $t9,-72($fp) # load $t9 = #reg8
	sw $t0,-84($fp)
	subu $t0,$t8,$t9 # #reg239 = 0-#reg8
	lw $t1,-100($fp) # load $t1 = #reg16
	sw $t2,-88($fp)
	addu $t2,$t0,$t1 # #reg240 = #reg239+#reg16
	lw $t3,-32($fp) # load $t3 = #reg17
	sw $t4,-92($fp)
	subu $t4,$t2,$t3 # #reg15 = #reg240-#reg17
exit_Func_6: # exit function operator
	addiu $sp,$sp,108 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,8
	jr $ra

Func_7: # function operator_test
	subiu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	jal Func_6
	li $v0,4 # print string
	la $a0,_str1
	syscall 
exit_Func_7: # exit function operator_test
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,8
	jr $ra

Func_8: # function string
	subiu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	li $v0,4 # print string
	la $a0,_str2
	syscall 
	li $v0,4 # print string
	la $a0,_str3
	syscall 
exit_Func_8: # exit function string
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,8
	jr $ra

Func_9: # function string_test
	subiu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	jal Func_8
exit_Func_9: # exit function string_test
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,8
	jr $ra

Func_10: # function dowhile
	subiu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	subiu $sp,$sp,24 # make space for locals and temps
	li $s1,100 # $s1=100
	li $s0,0 # $s0=0
	sw $s0,-12($fp) # spill toy
	sw $s1,-8($fp) # spill i
Label_0:
	lw $s1,-8($fp) # load ($s1) = i
	li $t0,50
	blt $s1,$t0,Label_1 # i<50
	lw $s1,-8($fp) # load ($s1) = i
	li $t0,2
	div $t1,$s1,$t0 # #reg22 = i/2
	addu $s1,$0,$t1 # $s1=#reg22
	sw $s1,-8($fp) # spill i
	j Label_2
Label_1:
Label_2:
	lw $s1,-8($fp) # load ($s1) = i
	li $t0,50
	bge $s1,$t0,Label_3 # i>=50
	lw $s1,-8($fp) # load ($s1) = i
	li $t0,10
	addu $t1,$s1,$t0 # #reg241 = i+10
	li $t2,10
	div $t3,$t1,$t2 # #reg25 = #reg241/10
	addu $s1,$0,$t3 # $s1=#reg25
	sw $s1,-8($fp) # spill i
	j Label_4
Label_3:
Label_4:
	lw $s0,-12($fp) # load ($s0) = toy
	li $t0,1
	addu $s0,$s0,$t0 # toy = toy+1
	lw $s1,-8($fp) # load ($s1) = i
	li $t1,10
	sw $s0,-12($fp) # spill toy
	bge $s1,$t1,Label_0 # i>=10
	li $s0,0 # $s0=0
	sw $s0,-12($fp) # spill toy
Label_5:
	lw $s0,-12($fp) # load ($s0) = toy
	li $t0,1
	addu $s0,$s0,$t0 # toy = toy+1
	lw $s1,-8($fp) # load ($s1) = i
	li $t1,1
	subu $t2,$s1,$t1 # #reg29 = i-1
	addu $s1,$0,$t2 # $s1=#reg29
	li $v0,4 # print string
	la $a0,_str4
	syscall 
	li $t3,0
	li $t4,0
	sw $s0,-12($fp) # spill toy
	sw $s1,-8($fp) # spill i
	bne $t3,$t4,Label_5 # 0!=0
exit_Func_10: # exit function dowhile
	addiu $sp,$sp,24 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,8
	jr $ra

Func_11: # function dowhile_test
	subiu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	jal Func_10
exit_Func_11: # exit function dowhile_test
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,8
	jr $ra

Func_12: # function ifcondition
	subiu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	subiu $sp,$sp,48 # make space for locals and temps
	li $s0,0 # $s0=0
	li $t0,1
	li $t1,1
	sw $s0,-12($fp) # spill rightnumber
	bne $t0,$t1,Label_6 # 1!=1
	lw $s0,-12($fp) # load ($s0) = rightnumber
	li $t0,1
	addu $t1,$s0,$t0 # #reg35 = rightnumber+1
	addu $s0,$0,$t1 # $s0=#reg35
	sw $s0,-12($fp) # spill rightnumber
	j Label_7
Label_6:
Label_7:
	li $t0,2
	li $t1,1
	ble $t0,$t1,Label_8 # 2<=1
	lw $s0,-12($fp) # load ($s0) = rightnumber
	li $t0,1
	addu $t1,$s0,$t0 # #reg39 = rightnumber+1
	addu $s0,$0,$t1 # $s0=#reg39
	sw $s0,-12($fp) # spill rightnumber
	j Label_9
Label_8:
Label_9:
	li $s1,0 # $s1=0
	li $t0,0
	li $t1,1
	sw $s1,-8($fp) # spill a
	bge $t0,$t1,Label_10 # 0>=1
	lw $s0,-12($fp) # load ($s0) = rightnumber
	li $t0,1
	addu $t1,$s0,$t0 # #reg43 = rightnumber+1
	addu $s0,$0,$t1 # $s0=#reg43
	sw $s0,-12($fp) # spill rightnumber
	j Label_11
Label_10:
Label_11:
	lw $s1,-8($fp) # load ($s1) = a
	li $t0,1
	beq $s1,$t0,Label_12 # a==1
	lw $s0,-12($fp) # load ($s0) = rightnumber
	li $t0,1
	addu $t1,$s0,$t0 # #reg46 = rightnumber+1
	addu $s0,$0,$t1 # $s0=#reg46
	sw $s0,-12($fp) # spill rightnumber
	j Label_13
Label_12:
Label_13:
	lw $s1,-8($fp) # load ($s1) = a
	li $t0,0
	blt $s1,$t0,Label_14 # a<0
	lw $s0,-12($fp) # load ($s0) = rightnumber
	li $t0,1
	addu $t1,$s0,$t0 # #reg49 = rightnumber+1
	addu $s0,$0,$t1 # $s0=#reg49
	sw $s0,-12($fp) # spill rightnumber
	j Label_15
Label_14:
Label_15:
	lw $s1,-8($fp) # load ($s1) = a
	li $t0,0
	bgt $s1,$t0,Label_16 # a>0
	lw $s0,-12($fp) # load ($s0) = rightnumber
	li $t0,1
	addu $t1,$s0,$t0 # #reg52 = rightnumber+1
	addu $s0,$0,$t1 # $s0=#reg52
	sw $s0,-12($fp) # spill rightnumber
	j Label_17
Label_16:
Label_17:
	li $t0,1
	li $t1,3
	mul $t2,$t0,$t1 # #reg55 = 1*3
	li $t3,1
	li $t4,1
	addu $t5,$t3,$t4 # #reg242 = 1+1
	li $t6,1
	addu $t7,$t5,$t6 # #reg243 = #reg242+1
	addu $t8,$t7,$t2 # #reg54 = #reg243+#reg55
	lw $s0,-12($fp) # load ($s0) = rightnumber
	beq $s0,$t8,Label_18 # rightnumber==#reg54
	li $v0,4 # print string
	la $a0,_str5
	syscall 
	j Label_19
Label_18:
Label_19:
	lw $s0,-12($fp) # load ($s0) = rightnumber
	beq $s0,$s0,Label_20 # rightnumber==rightnumber
	li $v0,4 # print string
	la $a0,_str6
	syscall 
	j Label_21
Label_20:
Label_21:
exit_Func_12: # exit function ifcondition
	addiu $sp,$sp,48 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,8
	jr $ra

Func_13: # function switchcodintionans
	subiu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,8 # $fp = old $sp
	subiu $sp,$sp,4 # make space for locals and temps
	lw $t0,0($fp) # load $t0 = x
	addu $s0,$0,$t0 # $s0=x
	sw $s0,-12($fp) # spill #reg58
Label_23:
	lw $s0,-12($fp) # load ($s0) = #reg58
	li $t0,1
	bne $s0,$t0,Label_24 # #reg58!=1
	li $v1,2 # @RET = 2
	j exit_Func_13
	j Label_22
Label_24:
	lw $s0,-12($fp) # load ($s0) = #reg58
	li $t0,2
	bne $s0,$t0,Label_25 # #reg58!=2
	li $v1,3 # @RET = 3
	j exit_Func_13
	j Label_22
Label_25:
	lw $s0,-12($fp) # load ($s0) = #reg58
	li $t0,3
	bne $s0,$t0,Label_26 # #reg58!=3
	li $v1,4 # @RET = 4
	j exit_Func_13
	j Label_22
Label_26:
	j Label_22
Label_22:
	li $v1,5 # @RET = 5
	j exit_Func_13
exit_Func_13: # exit function switchcodintionans
	addiu $sp,$sp,4 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,12
	jr $ra

Func_14: # function switchcodintion
	subiu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	subiu $sp,$sp,24 # make space for locals and temps
	li $s2,1 # $s2=1
	li $s1,0 # $s1=0
	li $s0,0 # $s0=0
	sw $s0,-16($fp) # spill toy
	sw $s1,-12($fp) # spill rightnumber
	sw $s2,-8($fp) # spill i
Label_27:
	lw $s2,-8($fp) # load ($s2) = i
	li $t0,1
	addu $s3,$s2,$t0 # #reg66 = i+1
	subiu $sp,$sp,4
	sw $s2,0($sp)
	sw $s2,-8($fp) # spill i
	sw $s3,-20($fp) # spill #reg66
	jal Func_13
	lw $s3,-20($fp) # load ($s3) = #reg66
	addu $t0,$0,$v1
	bne $s3,$t0,Label_28 # #reg66!=@RET
	lw $s1,-12($fp) # load ($s1) = rightnumber
	li $t0,1
	addu $t1,$s1,$t0 # #reg69 = rightnumber+1
	addu $s1,$0,$t1 # $s1=#reg69
	sw $s1,-12($fp) # spill rightnumber
	j Label_29
Label_28:
Label_29:
	lw $s0,-16($fp) # load ($s0) = toy
	li $t0,1
	addu $s0,$s0,$t0 # toy = toy+1
	lw $s2,-8($fp) # load ($s2) = i
	li $t1,1
	addu $t2,$s2,$t1 # #reg70 = i+1
	addu $s2,$0,$t2 # $s2=#reg70
	li $t3,5
	sw $s0,-16($fp) # spill toy
	sw $s2,-8($fp) # spill i
	blt $t2,$t3,Label_27 # #reg70<5
	lw $s1,-12($fp) # load ($s1) = rightnumber
	li $t0,4
	beq $s1,$t0,Label_30 # rightnumber==4
	li $v0,4 # print string
	la $a0,_str7
	syscall 
	j Label_31
Label_30:
Label_31:
	lw $s1,-12($fp) # load ($s1) = rightnumber
	li $t0,0
	beq $s1,$t0,Label_32 # rightnumber==0
	lw $v1,-8($fp) # @RET = i
	j exit_Func_14
	j Label_33
Label_32:
Label_33:
	li $v0,4 # print string
	la $a0,_str8
	syscall 
exit_Func_14: # exit function switchcodintion
	addiu $sp,$sp,24 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,8
	jr $ra

Func_15: # function condition
	subiu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	subiu $sp,$sp,20 # make space for locals and temps
	li $t0,5 # const intermax=5
	sw $t0,-8($fp)
	li $s1,0 # $s1=0
	li $s0,0 # $s0=0
	sw $s0,-16($fp) # spill toy
	sw $s1,-12($fp) # spill iter
Label_34:
	lw $s1,-12($fp) # load ($s1) = iter
	addu $s3,$0,$s1 # $s3=iter
	sw $s3,-20($fp) # spill #reg79
Label_36:
	lw $s3,-20($fp) # load ($s3) = #reg79
	li $t0,0
	bne $s3,$t0,Label_37 # #reg79!=0
	lw $s1,-12($fp) # load ($s1) = iter
	li $t0,0
	beq $s1,$t0,Label_38 # iter==0
	li $v0,4 # print string
	la $a0,_str9
	syscall 
	j Label_39
Label_38:
Label_39:
	j Label_35
Label_37:
	lw $s3,-20($fp) # load ($s3) = #reg79
	li $t0,1
	bne $s3,$t0,Label_40 # #reg79!=1
	lw $s1,-12($fp) # load ($s1) = iter
	li $t0,1
	beq $s1,$t0,Label_41 # iter==1
	li $v0,4 # print string
	la $a0,_str10
	syscall 
	j Label_42
Label_41:
Label_42:
	j Label_35
Label_40:
	lw $s3,-20($fp) # load ($s3) = #reg79
	li $t0,2
	bne $s3,$t0,Label_43 # #reg79!=2
	lw $s1,-12($fp) # load ($s1) = iter
	li $t0,2
	beq $s1,$t0,Label_44 # iter==2
	li $v0,4 # print string
	la $a0,_str11
	syscall 
	j Label_45
Label_44:
Label_45:
	j Label_35
Label_43:
	j Label_35
Label_35:
	lw $s0,-16($fp) # load ($s0) = toy
	li $t0,1
	addu $s0,$s0,$t0 # toy = toy+1
	lw $s1,-12($fp) # load ($s1) = iter
	li $t1,1
	addu $t2,$s1,$t1 # #reg86 = iter+1
	addu $s1,$0,$t2 # $s1=#reg86
	lw $s2,-8($fp) # load ($s2) = intermax
	sw $s0,-16($fp) # spill toy
	sw $s1,-12($fp) # spill iter
	blt $t2,$s2,Label_34 # #reg86<intermax
exit_Func_15: # exit function condition
	addiu $sp,$sp,20 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,8
	jr $ra

Func_16: # function condition_test
	subiu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	jal Func_12
	jal Func_14
	jal Func_15
	li $v0,4 # print string
	la $a0,_str12
	syscall 
exit_Func_16: # exit function condition_test
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,8
	jr $ra

Func_17: # function miscellaneous
	subiu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	subiu $sp,$sp,1188 # make space for locals and temps
	li $s1,0 # $s1=0
	li $s0,0 # $s0=0
	sw $s0,-116($fp) # spill toy
	sw $s1,-108($fp) # spill i
Label_46:
	lw $s1,-108($fp) # load ($s1) = i
	sll $t0,$s1,2
	subiu $t1,$fp,16
	subu $t1,$t1,$t0
	sw $s1,0($t1) # array[i] = i
	lw $s0,-116($fp) # load ($s0) = toy
	li $t0,1
	addu $s0,$s0,$t0 # toy = toy+1
	lw $s1,-108($fp) # load ($s1) = i
	li $t1,1
	addu $t2,$s1,$t1 # #reg95 = i+1
	addu $s1,$0,$t2 # $s1=#reg95
	li $t3,23
	sw $s0,-116($fp) # spill toy
	sw $s1,-108($fp) # spill i
	blt $t2,$t3,Label_46 # #reg95<23
	li $s3,'a' # $s3='a'
	li $s1,0 # $s1=0
	li $s0,0 # $s0=0
	sw $s0,-116($fp) # spill toy
	sw $s1,-108($fp) # spill i
	sw $s3,-120($fp) # spill ch1
Label_47:
	lw $s1,-108($fp) # load ($s1) = i
	sll $t0,$s1,2
	subiu $t1,$fp,124
	subu $t1,$t1,$t0
	lw $s3,-120($fp) # load ($s3) = ch1
	sw $s3,0($t1) # buf[i] = ch1
	lw $s0,-116($fp) # load ($s0) = toy
	li $t0,1
	addu $s0,$s0,$t0 # toy = toy+1
	lw $s1,-108($fp) # load ($s1) = i
	li $t1,1
	addu $t2,$s1,$t1 # #reg104 = i+1
	lw $s3,-120($fp) # load ($s3) = ch1
	li $t3,1
	addu $t4,$s3,$t3 # #reg103 = ch1+1
	addu $s3,$0,$t4 # $s3=#reg103
	addu $s1,$0,$t2 # $s1=#reg104
	li $t5,127
	sw $s0,-116($fp) # spill toy
	sw $s1,-108($fp) # spill i
	sw $s3,-120($fp) # spill ch1
	blt $t4,$t5,Label_47 # #reg103<127
	lw $s1,-108($fp) # load ($s1) = i
	addu $s2,$0,$s1 # $s2=i
	li $s5,0 # $s5=0
	li $s0,0 # $s0=0
	sw $s0,-116($fp) # spill toy
	sw $s2,-112($fp) # spill endid
	sw $s5,-8($fp) # spill _
Label_48:
	li $t0,2
	li $t1,1
	div $t2,$t0,$t1 # #reg111 = 2/1
	lw $s5,-8($fp) # load ($s5) = _
	li $t3,1
	mul $t4,$s5,$t3 # #reg244 = _*1
	subu $t5,$t4,$s5 # #reg245 = #reg244-_
	li $t6,1
	subu $t7,$t5,$t6 # #reg246 = #reg245-1
	addu $t8,$t7,$t2 # #reg247 = #reg246+#reg111
	li $t9,1
	sw $t4,-1164($fp)
	subu $t4,$t8,$t9 # #reg248 = #reg247-1
	sw $t5,-1168($fp)
	li $t5,2
	div $t6,$t4,$t5 # #reg110 = #reg248/2
	sw $t7,-1172($fp)
	sll $t7,$t6,2
	sw $t8,-1176($fp)
	subiu $t8,$fp,124
	subu $t8,$t8,$t7
	li $t9,'a'
	sw $t9,0($t8) # buf[#reg110] = 'a'
	lw $s0,-116($fp) # load ($s0) = toy
	li $t0,1
	addu $s0,$s0,$t0 # toy = toy+1
	lw $s5,-8($fp) # load ($s5) = _
	li $t1,1
	addu $t2,$s5,$t1 # #reg113 = _+1
	addu $s5,$0,$t2 # $s5=#reg113
	lw $s2,-112($fp) # load ($s2) = endid
	sw $s0,-116($fp) # spill toy
	sw $s5,-8($fp) # spill _
	blt $t2,$s2,Label_48 # #reg113<endid
	li $t0,0
	sll $t1,$t0,2
	subiu $t2,$fp,124
	subu $t2,$t2,$t1
	lw $t3,0($t2) # #reg117=buf[0]
	li $t4,'a'
	bne $t3,$t4,Label_49 # #reg117!='a'
	li $v0,4 # print string
	la $a0,_str13
	syscall 
	j Label_50
Label_49:
Label_50:
exit_Func_17: # exit function miscellaneous
	addiu $sp,$sp,1188 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,8
	jr $ra

Func_18: # function miscellaneous_test
	subiu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	jal Func_17
exit_Func_18: # exit function miscellaneous_test
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,8
	jr $ra

Func_19: # function fibonaci
	subiu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,8 # $fp = old $sp
	subiu $sp,$sp,16 # make space for locals and temps
	lw $s0,0($fp) # load ($s0) = n
	addu $s2,$0,$s0 # $s2=n
	sw $s2,-12($fp) # spill #reg119
Label_52:
	lw $s2,-12($fp) # load ($s2) = #reg119
	li $t0,0
	bne $s2,$t0,Label_53 # #reg119!=0
	li $v1,0 # @RET = 0
	j exit_Func_19
	j Label_51
Label_53:
	lw $s2,-12($fp) # load ($s2) = #reg119
	li $t0,1
	bne $s2,$t0,Label_54 # #reg119!=1
	li $v1,1 # @RET = 1
	j exit_Func_19
	j Label_51
Label_54:
	lw $s2,-12($fp) # load ($s2) = #reg119
	li $t0,2
	bne $s2,$t0,Label_55 # #reg119!=2
	li $v1,1 # @RET = 1
	j exit_Func_19
	j Label_51
Label_55:
	j Label_51
Label_51:
	lw $s0,0($fp) # load ($s0) = n
	li $t0,1
	subu $t1,$s0,$t0 # #reg123 = n-1
	subiu $sp,$sp,4
	sw $t1,0($sp)
	sw $s0,0($fp) # spill n
	jal Func_19
	lw $s0,0($fp) # load ($s0) = n
	li $t0,2
	subu $t1,$s0,$t0 # #reg125 = n-2
	addu $s1,$0,$v1 # $s1=@RET
	subiu $sp,$sp,4
	sw $t1,0($sp)
	sw $s0,0($fp) # spill n
	sw $s1,-24($fp) # spill #reg124
	jal Func_19
	lw $s1,-24($fp) # load ($s1) = #reg124
	addu $t0,$0,$v1
	addu $s1,$s1,$t0 # #reg124 = #reg124+@RET
	addu $v1,$0,$s1 # @RET = #reg124
	sw $s1,-24($fp) # spill #reg124
	j exit_Func_19
exit_Func_19: # exit function fibonaci
	addiu $sp,$sp,16 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,12
	jr $ra

Func_20: # function fibonaci_test
	subiu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	subiu $sp,$sp,8 # make space for locals and temps
	li $v0,4 # print string
	la $a0,_str14
	syscall 
	li $v0,5# read int : n
	syscall 
	sw $v0,-8($fp)
	li $s0,0 # $s0=0
	sw $s0,-12($fp) # spill toy
Label_56:
	lw $s1,-8($fp) # load ($s1) = n
	li $t0,0
	bge $s1,$t0,Label_57 # n>=0
	li $v0,4 # print string
	la $a0,_str15
	syscall 
	li $v0,5# read int : n
	syscall 
	sw $v0,-8($fp)
	j Label_58
Label_57:
Label_58:
	lw $s1,-8($fp) # load ($s1) = n
	li $t0,0
	blt $s1,$t0,Label_59 # n<0
	lw $s1,-8($fp) # load ($s1) = n
	subiu $sp,$sp,4
	sw $s1,0($sp)
	sw $s1,-8($fp) # spill n
	jal Func_19
	li $v0,4 # print string
	la $a0,_str16
	syscall 
	li $v0,1# print int : @RET
	addu $a0,$0,$v1
	syscall 
	j Label_60
Label_59:
Label_60:
	lw $s0,-12($fp) # load ($s0) = toy
	li $t0,1
	addu $s0,$s0,$t0 # toy = toy+1
	lw $s1,-8($fp) # load ($s1) = n
	li $t1,0
	sw $s0,-12($fp) # spill toy
	blt $s1,$t1,Label_56 # n<0
exit_Func_20: # exit function fibonaci_test
	addiu $sp,$sp,8 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,8
	jr $ra

Func_21: # function mod
	subiu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,12 # $fp = old $sp
	subiu $sp,$sp,12 # make space for locals and temps
	lw $t0,0($fp) # load $t0 = n
	lw $t1,-4($fp) # load $t1 = m
	div $t2,$t0,$t1 # #reg249 = n/m
	mul $t3,$t2,$t1 # #reg250 = #reg249*m
	subu $t4,$t0,$t3 # #reg136 = n-#reg250
	addu $v1,$0,$t4 # @RET = #reg136
	j exit_Func_21
exit_Func_21: # exit function mod
	addiu $sp,$sp,12 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,16
	jr $ra

Func_22: # function gcd
	subiu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,12 # $fp = old $sp
	subiu $sp,$sp,4 # make space for locals and temps
	lw $s0,0($fp) # load ($s0) = n
	li $t0,0
	bne $s0,$t0,Label_61 # n!=0
	lw $v1,-4($fp) # @RET = m
	j exit_Func_22
	j Label_62
Label_61:
Label_62:
	lw $s1,-4($fp) # load ($s1) = m
	li $t0,0
	bne $s1,$t0,Label_63 # m!=0
	lw $v1,0($fp) # @RET = n
	j exit_Func_22
	j Label_64
Label_63:
Label_64:
	lw $s1,-4($fp) # load ($s1) = m
	subiu $sp,$sp,4
	sw $s1,0($sp)
	lw $s0,0($fp) # load ($s0) = n
	subiu $sp,$sp,4
	sw $s0,0($sp)
	subiu $sp,$sp,4
	sw $s1,0($sp)
	sw $s0,0($fp) # spill n
	sw $s1,-4($fp) # spill m
	jal Func_21
	addu $t0,$0,$v1
	subiu $sp,$sp,4
	sw $t0,0($sp)
	jal Func_22
	j exit_Func_22
exit_Func_22: # exit function gcd
	addiu $sp,$sp,4 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,16
	jr $ra

Func_23: # function isWrongGcd
	subiu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,12 # $fp = old $sp
	lw $t0,0($fp) # load $t0 = n
	li $t1,0
	bgt $t0,$t1,Label_65 # n>0
	li $v1,1 # @RET = 1
	j exit_Func_23
	j Label_66
Label_65:
Label_66:
	lw $s0,-4($fp) # load ($s0) = m
	li $t0,0
	bgt $s0,$t0,Label_67 # m>0
	li $v1,1 # @RET = 1
	j exit_Func_23
	j Label_68
Label_67:
Label_68:
	li $v1,0 # @RET = 0
	j exit_Func_23
exit_Func_23: # exit function isWrongGcd
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,16
	jr $ra

Func_24: # function gcd_test
	subiu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	subiu $sp,$sp,12 # make space for locals and temps
	li $v0,4 # print string
	la $a0,_str17
	syscall 
	li $v0,5# read int : n
	syscall 
	sw $v0,-8($fp)
	li $v0,5# read int : m
	syscall 
	sw $v0,-12($fp)
	lw $s1,-8($fp) # load ($s1) = n
	subiu $sp,$sp,4
	sw $s1,0($sp)
	lw $s2,-12($fp) # load ($s2) = m
	subiu $sp,$sp,4
	sw $s2,0($sp)
	sw $s1,-8($fp) # spill n
	sw $s2,-12($fp) # spill m
	jal Func_23
	addu $t0,$0,$v1
	li $t1,0
	beq $t0,$t1,Label_69 # @RET==0
	li $s0,0 # $s0=0
	sw $s0,-16($fp) # spill toy
Label_70:
	li $v0,4 # print string
	la $a0,_str18
	syscall 
	li $v0,5# read int : n
	syscall 
	sw $v0,-8($fp)
	li $v0,5# read int : m
	syscall 
	sw $v0,-12($fp)
	lw $s0,-16($fp) # load ($s0) = toy
	li $t0,1
	addu $s0,$s0,$t0 # toy = toy+1
	lw $s1,-8($fp) # load ($s1) = n
	subiu $sp,$sp,4
	sw $s1,0($sp)
	lw $s2,-12($fp) # load ($s2) = m
	subiu $sp,$sp,4
	sw $s2,0($sp)
	sw $s0,-16($fp) # spill toy
	sw $s1,-8($fp) # spill n
	sw $s2,-12($fp) # spill m
	jal Func_23
	addu $t0,$0,$v1
	li $t1,0
	bne $t0,$t1,Label_70 # @RET!=0
	j Label_71
Label_69:
Label_71:
	lw $s1,-8($fp) # load ($s1) = n
	subiu $sp,$sp,4
	sw $s1,0($sp)
	lw $s2,-12($fp) # load ($s2) = m
	subiu $sp,$sp,4
	sw $s2,0($sp)
	sw $s1,-8($fp) # spill n
	sw $s2,-12($fp) # spill m
	jal Func_22
	li $v0,4 # print string
	la $a0,_str19
	syscall 
	li $v0,1# print int : @RET
	addu $a0,$0,$v1
	syscall 
exit_Func_24: # exit function gcd_test
	addiu $sp,$sp,12 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,8
	jr $ra

Func_25: # function is_prime
	subiu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,8 # $fp = old $sp
	subiu $sp,$sp,16 # make space for locals and temps
	li $s2,2 # $s2=2
	lw $s0,0($fp) # load ($s0) = x
	li $t0,2
	sw $s2,-12($fp) # spill i
	bne $s0,$t0,Label_72 # x!=2
	li $v1,1 # @RET = 1
	j exit_Func_25
	j Label_73
Label_72:
Label_73:
	li $s1,0 # $s1=0
	sw $s1,-16($fp) # spill toy
Label_74:
	lw $s0,0($fp) # load ($s0) = x
	subiu $sp,$sp,4
	sw $s0,0($sp)
	lw $s2,-12($fp) # load ($s2) = i
	subiu $sp,$sp,4
	sw $s2,0($sp)
	sw $s0,0($fp) # spill x
	sw $s2,-12($fp) # spill i
	jal Func_21
	addu $t0,$0,$v1
	li $t1,0
	bne $t0,$t1,Label_75 # @RET!=0
	li $v1,0 # @RET = 0
	j exit_Func_25
	j Label_76
Label_75:
Label_76:
	lw $s2,-12($fp) # load ($s2) = i
	li $t0,1
	addu $t1,$s2,$t0 # #reg176 = i+1
	mul $t2,$t1,$t1 # #reg177 = #reg176*#reg176
	lw $s1,-16($fp) # load ($s1) = toy
	li $t3,1
	addu $s1,$s1,$t3 # toy = toy+1
	addu $s2,$0,$t1 # $s2=#reg176
	lw $s0,0($fp) # load ($s0) = x
	sw $s1,-16($fp) # spill toy
	sw $s2,-12($fp) # spill i
	blt $t2,$s0,Label_74 # #reg177<x
	li $v1,1 # @RET = 1
	j exit_Func_25
exit_Func_25: # exit function is_prime
	addiu $sp,$sp,16 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,12
	jr $ra

Func_26: # function prime_factorization
	subiu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,8 # $fp = old $sp
	subiu $sp,$sp,28 # make space for locals and temps
	li $s2,2 # $s2=2
	li $s4,0 # $s4=0
	lw $s1,0($fp) # load ($s1) = n
	li $t0,1
	sw $s2,-12($fp) # spill i
	sw $s4,-16($fp) # spill first
	bne $s1,$t0,Label_77 # n!=1
	li $v0,4 # print string
	la $a0,_str20
	syscall 
	j exit_Func_26
	j Label_78
Label_77:
Label_78:
	li $s3,0 # $s3=0
	sw $s3,-24($fp) # spill fuck
Label_79:
	lw $s2,-12($fp) # load ($s2) = i
	subiu $sp,$sp,4
	sw $s2,0($sp)
	sw $s2,-12($fp) # spill i
	jal Func_25
	addu $t0,$0,$v1
	li $t1,1
	bne $t0,$t1,Label_80 # @RET!=1
	lw $s1,0($fp) # load ($s1) = n
	subiu $sp,$sp,4
	sw $s1,0($sp)
	lw $s2,-12($fp) # load ($s2) = i
	subiu $sp,$sp,4
	sw $s2,0($sp)
	sw $s1,0($fp) # spill n
	sw $s2,-12($fp) # spill i
	jal Func_21
	addu $t0,$0,$v1
	li $t1,0
	bne $t0,$t1,Label_81 # @RET!=0
	li $s0,0 # $s0=0
	sw $s0,-20($fp) # spill toy
Label_82:
	lw $s1,0($fp) # load ($s1) = n
	lw $s2,-12($fp) # load ($s2) = i
	div $t0,$s1,$s2 # #reg193 = n/i
	addu $s1,$0,$t0 # $s1=#reg193
	lw $s4,-16($fp) # load ($s4) = first
	li $t1,1
	sw $s1,0($fp) # spill n
	bne $s4,$t1,Label_83 # first!=1
	li $v0,4 # print string
	la $a0,_str21
	syscall 
	j Label_84
Label_83:
Label_84:
	li $v0,1# print int : i
	lw $a0,-12($fp)
	syscall 
	lw $s0,-20($fp) # load ($s0) = toy
	li $t0,1
	addu $s0,$s0,$t0 # toy = toy+1
	li $s4,1 # $s4=1
	lw $s1,0($fp) # load ($s1) = n
	subiu $sp,$sp,4
	sw $s1,0($sp)
	lw $s2,-12($fp) # load ($s2) = i
	subiu $sp,$sp,4
	sw $s2,0($sp)
	sw $s0,-20($fp) # spill toy
	sw $s1,0($fp) # spill n
	sw $s2,-12($fp) # spill i
	sw $s4,-16($fp) # spill first
	jal Func_21
	addu $t0,$0,$v1
	li $t1,0
	beq $t0,$t1,Label_82 # @RET==0
	j Label_85
Label_81:
Label_85:
	j Label_86
Label_80:
Label_86:
	lw $s2,-12($fp) # load ($s2) = i
	li $t0,1
	addu $t1,$s2,$t0 # #reg202 = i+1
	mul $t2,$t1,$t1 # #reg203 = #reg202*#reg202
	lw $s3,-24($fp) # load ($s3) = fuck
	li $t3,1
	addu $s3,$s3,$t3 # fuck = fuck+1
	addu $s2,$0,$t1 # $s2=#reg202
	lw $s1,0($fp) # load ($s1) = n
	sw $s2,-12($fp) # spill i
	sw $s3,-24($fp) # spill fuck
	blt $t2,$s1,Label_79 # #reg203<n
	lw $s1,0($fp) # load ($s1) = n
	li $t0,1
	beq $s1,$t0,Label_87 # n==1
	lw $s4,-16($fp) # load ($s4) = first
	li $t0,1
	bne $s4,$t0,Label_88 # first!=1
	li $v0,4 # print string
	la $a0,_str22
	syscall 
	j Label_89
Label_88:
Label_89:
	li $v0,1# print int : n
	lw $a0,0($fp)
	syscall 
	j Label_90
Label_87:
Label_90:
exit_Func_26: # exit function prime_factorization
	addiu $sp,$sp,28 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,12
	jr $ra

Func_27: # function prime_factorization_test
	subiu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	subiu $sp,$sp,8 # make space for locals and temps
	li $v0,4 # print string
	la $a0,_str23
	syscall 
	li $v0,5# read int : n
	syscall 
	sw $v0,-8($fp)
	li $s0,0 # $s0=0
	sw $s0,-12($fp) # spill toy
Label_91:
	lw $s1,-8($fp) # load ($s1) = n
	li $t0,0
	bge $s1,$t0,Label_92 # n>=0
	li $v0,4 # print string
	la $a0,_str24
	syscall 
	li $v0,5# read int : n
	syscall 
	sw $v0,-8($fp)
	j Label_93
Label_92:
Label_93:
	lw $s0,-12($fp) # load ($s0) = toy
	li $t0,1
	addu $s0,$s0,$t0 # toy = toy+1
	lw $s1,-8($fp) # load ($s1) = n
	li $t1,0
	sw $s0,-12($fp) # spill toy
	blt $s1,$t1,Label_91 # n<0
	li $v0,4 # print string
	la $a0,_str25
	syscall 
	lw $s1,-8($fp) # load ($s1) = n
	subiu $sp,$sp,4
	sw $s1,0($sp)
	sw $s1,-8($fp) # spill n
	jal Func_26
exit_Func_27: # exit function prime_factorization_test
	addiu $sp,$sp,8 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,8
	jr $ra

Func_28: # function iswrong
	subiu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,8 # $fp = old $sp
	li $t0,'E'
	lw $s0,0($fp) # load ($s0) = A
	bne $t0,$s0,Label_94 # 'E'!=A
	li $v1,0 # @RET = 0
	j exit_Func_28
	j Label_95
Label_94:
Label_95:
	li $t0,'M'
	lw $s0,0($fp) # load ($s0) = A
	bne $t0,$s0,Label_96 # 'M'!=A
	li $v1,0 # @RET = 0
	j exit_Func_28
	j Label_97
Label_96:
Label_97:
	li $t0,'H'
	lw $s0,0($fp) # load ($s0) = A
	bne $t0,$s0,Label_98 # 'H'!=A
	li $v1,0 # @RET = 0
	j exit_Func_28
	j Label_99
Label_98:
Label_99:
	li $v1,1 # @RET = 1
	j exit_Func_28
exit_Func_28: # exit function iswrong
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,12
	jr $ra

Func_29: # function main
	subiu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	subiu $sp,$sp,12 # make space for locals and temps
	li $v0,4 # print string
	la $a0,_str26
	syscall 
	li $v0,12# read char : kind
	syscall 
	sw $v0,-8($fp)
	lw $s1,-8($fp) # load ($s1) = kind
	subiu $sp,$sp,4
	sw $s1,0($sp)
	sw $s1,-8($fp) # spill kind
	jal Func_28
	addu $t0,$0,$v1
	li $t1,0
	beq $t0,$t1,Label_100 # @RET==0
	li $s0,0 # $s0=0
	sw $s0,-12($fp) # spill toy
Label_101:
	li $v0,4 # print string
	la $a0,_str27
	syscall 
	li $v0,12# read char : kind
	syscall 
	sw $v0,-8($fp)
	lw $s0,-12($fp) # load ($s0) = toy
	li $t0,1
	addu $s0,$s0,$t0 # toy = toy+1
	lw $s1,-8($fp) # load ($s1) = kind
	subiu $sp,$sp,4
	sw $s1,0($sp)
	sw $s0,-12($fp) # spill toy
	sw $s1,-8($fp) # spill kind
	jal Func_28
	addu $t0,$0,$v1
	li $t1,0
	bne $t0,$t1,Label_101 # @RET!=0
	j Label_102
Label_100:
Label_102:
	lw $s1,-8($fp) # load ($s1) = kind
	addu $s2,$0,$s1 # $s2=kind
	sw $s2,-16($fp) # spill #reg231
Label_104:
	lw $s2,-16($fp) # load ($s2) = #reg231
	li $t0,'E'
	bne $s2,$t0,Label_105 # #reg231!='E'
	jal Func_5
	jal Func_7
	jal Func_20
	j Label_103
Label_105:
	lw $s2,-16($fp) # load ($s2) = #reg231
	li $t0,'M'
	bne $s2,$t0,Label_106 # #reg231!='M'
	jal Func_11
	jal Func_16
	jal Func_9
	jal Func_24
	j Label_103
Label_106:
	lw $s2,-16($fp) # load ($s2) = #reg231
	li $t0,'H'
	bne $s2,$t0,Label_107 # #reg231!='H'
	jal Func_5
	jal Func_7
	jal Func_11
	jal Func_16
	jal Func_9
	jal Func_18
	jal Func_27
	j Label_103
Label_107:
	li $v0,4 # print string
	la $a0,_str28
	syscall 
	j Label_103
Label_103:
	j exit_Func_29
exit_Func_29: # exit function main
	addiu $sp,$sp,12 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,8
	addiu $sp,$sp,20512
	li $gp,0x10008000 # restore $gp
