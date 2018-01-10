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
	subi $gp,$gp,20512 # make space for global variabel and constant
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
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	subi $sp,$sp,32 # make space for locals and temps
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
	subi $sp,$sp,8 # make space for $fp and $ra
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
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	subi $sp,$sp,80 # make space for locals and temps
exit_Func_2: # exit function var_define
	addiu $sp,$sp,80 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,8
	jr $ra
Func_3: # function var_define_test
	subi $sp,$sp,8 # make space for $fp and $ra
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
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	subi $sp,$sp,24 # make space for locals and temps
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
	subi $sp,$sp,8 # make space for $fp and $ra
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
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	subi $sp,$sp,108 # make space for locals and temps
	li $t0,1
	li $t1,0
	subu $t2,$t0,$t1 # #reg17 = 1-0
	li $t3,5
	li $t4,6
	mul $t5,$t3,$t4 # #reg14 = 5*6
	div $t6,$t0,$t0 # #reg10 = 1/1
	li $t7,4
	subu $t8,$t1,$t7 # #reg5 = 0-4
	div $t9,$t8,$t7 # #reg9 = #reg5/4
	li $t1,2
	sw $t2,-32($fp)
	addu $t2,$t0,$t1 # #reg3 = 1+2
	li $t3,3
	mul $t4,$t2,$t3 # #reg232 = #reg3*3
	sw $t4,-56($fp)
	mul $t4,$t3,$t0 # #reg4 = 3*1
	sw $t4,-60($fp)
	lw $t4,-56($fp) # load $t4 = #reg232
	sw $t5,-36($fp)
	lw $t5,-60($fp) # load $t5 = #reg4
	sw $t6,-40($fp)
	addu $t6,$t4,$t5 # #reg233 = #reg232+#reg4
	subu $t7,$t6,$t9 # #reg234 = #reg233-#reg9
	sw $t8,-44($fp)
	lw $t8,-40($fp) # load $t8 = #reg10
	sw $t9,-48($fp)
	addu $t9,$t7,$t8 # #reg8 = #reg234+#reg10
	mul $t0,$t9,$t3 # #reg12 = #reg8*3
	sw $t0,-76($fp)
	li $t0,0
	addu $t1,$t0,$t9 # #reg235 = 0+#reg8
	sw $t2,-52($fp)
	lw $t2,-76($fp) # load $t2 = #reg12
	addu $t3,$t1,$t2 # #reg236 = #reg235+#reg12
	addu $t4,$t3,$t5 # #reg237 = #reg236+#reg4
	lw $t5,-48($fp) # load $t5 = #reg9
	sw $t6,-64($fp)
	subu $t6,$t4,$t5 # #reg238 = #reg237-#reg9
	sw $t7,-68($fp)
	lw $t7,-36($fp) # load $t7 = #reg14
	addu $t8,$t6,$t7 # #reg11 = #reg238+#reg14
	sw $t9,-72($fp)
	mul $t9,$t8,$t0 # #reg16 = #reg11*0
	sw $t9,-100($fp)
	lw $t9,-72($fp) # load $t9 = #reg8
	sw $t1,-80($fp)
	subu $t1,$t0,$t9 # #reg239 = 0-#reg8
	lw $t2,-100($fp) # load $t2 = #reg16
	sw $t3,-84($fp)
	addu $t3,$t1,$t2 # #reg240 = #reg239+#reg16
	sw $t4,-88($fp)
	lw $t4,-32($fp) # load $t4 = #reg17
	subu $t5,$t3,$t4 # #reg15 = #reg240-#reg17
exit_Func_6: # exit function operator
	addiu $sp,$sp,108 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,8
	jr $ra
Func_7: # function operator_test
	subi $sp,$sp,8 # make space for $fp and $ra
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
	subi $sp,$sp,8 # make space for $fp and $ra
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
	subi $sp,$sp,8 # make space for $fp and $ra
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
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	subi $sp,$sp,24 # make space for locals and temps
	li $s1,100 # $s1=100
	li $s0,0 # $s0=0
Label_0:
	li $t0,50
	blt $s1,$t0,Label_1 # i<50
	li $t0,2
	div $t1,$s1,$t0 # #reg22 = i/2
	addu $s1,$0,$t1 # $s1=#reg22
	j Label_2
Label_1:
Label_2:
	li $t0,50
	bge $s1,$t0,Label_3 # i>=50
	li $t0,10
	addu $t1,$s1,$t0 # #reg241 = i+10
	div $t2,$t1,$t0 # #reg25 = #reg241/10
	addu $s1,$0,$t2 # $s1=#reg25
	j Label_4
Label_3:
Label_4:
	li $t0,1
	addu $s0,$s0,$t0 # toy = toy+1
	li $t1,10
	bge $s1,$t1,Label_0 # i>=10
	li $s0,0 # $s0=0
Label_5:
	li $t0,1
	addu $s0,$s0,$t0 # toy = toy+1
	subu $t1,$s1,$t0 # #reg29 = i-1
	addu $s1,$0,$t1 # $s1=#reg29
	li $v0,4 # print string
	la $a0,_str4
	syscall 
	li $t2,0
	bne $t2,$t2,Label_5 # 0!=0
exit_Func_10: # exit function dowhile
	addiu $sp,$sp,24 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,8
	jr $ra
Func_11: # function dowhile_test
	subi $sp,$sp,8 # make space for $fp and $ra
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
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	subi $sp,$sp,48 # make space for locals and temps
	li $s0,0 # $s0=0
	li $t0,1
	bne $t0,$t0,Label_6 # 1!=1
	li $t0,1
	addu $t1,$s0,$t0 # #reg35 = rightnumber+1
	addu $s0,$0,$t1 # $s0=#reg35
	j Label_7
Label_6:
Label_7:
	li $t0,2
	li $t1,1
	ble $t0,$t1,Label_8 # 2<=1
	li $t0,1
	addu $t1,$s0,$t0 # #reg39 = rightnumber+1
	addu $s0,$0,$t1 # $s0=#reg39
	j Label_9
Label_8:
Label_9:
	li $s1,0 # $s1=0
	li $t0,0
	li $t1,1
	bge $t0,$t1,Label_10 # 0>=1
	li $t0,1
	addu $t1,$s0,$t0 # #reg43 = rightnumber+1
	addu $s0,$0,$t1 # $s0=#reg43
	j Label_11
Label_10:
Label_11:
	li $t0,1
	beq $s1,$t0,Label_12 # a==1
	li $t0,1
	addu $t1,$s0,$t0 # #reg46 = rightnumber+1
	addu $s0,$0,$t1 # $s0=#reg46
	j Label_13
Label_12:
Label_13:
	li $t0,0
	blt $s1,$t0,Label_14 # a<0
	li $t0,1
	addu $t1,$s0,$t0 # #reg49 = rightnumber+1
	addu $s0,$0,$t1 # $s0=#reg49
	j Label_15
Label_14:
Label_15:
	li $t0,0
	bgt $s1,$t0,Label_16 # a>0
	li $t0,1
	addu $t1,$s0,$t0 # #reg52 = rightnumber+1
	addu $s0,$0,$t1 # $s0=#reg52
	j Label_17
Label_16:
Label_17:
	li $t0,1
	li $t1,3
	mul $t2,$t0,$t1 # #reg55 = 1*3
	addu $t3,$t0,$t0 # #reg242 = 1+1
	addu $t4,$t3,$t0 # #reg243 = #reg242+1
	addu $t5,$t4,$t2 # #reg54 = #reg243+#reg55
	beq $s0,$t5,Label_18 # rightnumber==#reg54
	li $v0,4 # print string
	la $a0,_str5
	syscall 
	j Label_19
Label_18:
Label_19:
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
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,8 # $fp = old $sp
	subi $sp,$sp,4 # make space for locals and temps
	lw $t0,0($fp) # load $t0 = x
	addu $s0,$0,$t0 # $s0=x
Label_23:
	li $t0,1
	bne $s0,$t0,Label_24 # #reg58!=1
	li $v1,2 # @RET = 2
	j exit_Func_13
	j Label_22
Label_24:
	li $t0,2
	bne $s0,$t0,Label_25 # #reg58!=2
	li $v1,3 # @RET = 3
	j exit_Func_13
	j Label_22
Label_25:
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
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	subi $sp,$sp,24 # make space for locals and temps
	li $s2,1 # $s2=1
	li $s1,0 # $s1=0
	li $s0,0 # $s0=0
Label_27:
	li $t0,1
	addu $s3,$s2,$t0 # #reg66 = i+1
	subi $sp,$sp,4
	sw $s2,0($sp)
	sw $s0,-16($fp) # save toy(global reg)
	sw $s1,-12($fp) # save rightnumber(global reg)
	sw $s2,-8($fp) # save i(global reg)
	sw $s3,-20($fp) # save #reg66(global reg)
	jal Func_13
	lw $s0,-16($fp) # save toy(global reg)
	lw $s1,-12($fp) # save rightnumber(global reg)
	lw $s2,-8($fp) # save i(global reg)
	lw $s3,-20($fp) # save #reg66(global reg)
	addu $t0,$0,$v1
	bne $s3,$t0,Label_28 # #reg66!=@RET
	li $t0,1
	addu $t1,$s1,$t0 # #reg69 = rightnumber+1
	addu $s1,$0,$t1 # $s1=#reg69
	j Label_29
Label_28:
Label_29:
	li $t0,1
	addu $s0,$s0,$t0 # toy = toy+1
	addu $t1,$s2,$t0 # #reg70 = i+1
	addu $s2,$0,$t1 # $s2=#reg70
	li $t2,5
	blt $t1,$t2,Label_27 # #reg70<5
	li $t0,4
	beq $s1,$t0,Label_30 # rightnumber==4
	li $v0,4 # print string
	la $a0,_str7
	syscall 
	j Label_31
Label_30:
Label_31:
	li $t0,0
	beq $s1,$t0,Label_32 # rightnumber==0
	addu $v1,$0,$s2 # @RET = i
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
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	subi $sp,$sp,20 # make space for locals and temps
	li $t0,5 # const intermax=5
	sw $t0,-8($fp)
	li $s1,0 # $s1=0
	li $s0,0 # $s0=0
Label_34:
	addu $s2,$0,$s1 # $s2=iter
Label_36:
	li $t0,0
	bne $s2,$t0,Label_37 # #reg79!=0
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
	li $t0,1
	bne $s2,$t0,Label_40 # #reg79!=1
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
	li $t0,2
	bne $s2,$t0,Label_43 # #reg79!=2
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
	li $t0,1
	addu $s0,$s0,$t0 # toy = toy+1
	addu $t1,$s1,$t0 # #reg86 = iter+1
	addu $s1,$0,$t1 # $s1=#reg86
	lw $t2,-8($fp) # load $t2 = intermax
	blt $t1,$t2,Label_34 # #reg86<intermax
exit_Func_15: # exit function condition
	addiu $sp,$sp,20 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,8
	jr $ra
Func_16: # function condition_test
	subi $sp,$sp,8 # make space for $fp and $ra
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
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	subi $sp,$sp,1188 # make space for locals and temps
	li $s1,0 # $s1=0
	li $s0,0 # $s0=0
Label_46:
	sll $t0,$s1,2
	subi $t1,$fp,16
	subu $t1,$t1,$t0
	sw $s1,0($t1) # array[i] = i
	li $t0,1
	addu $s0,$s0,$t0 # toy = toy+1
	addu $t1,$s1,$t0 # #reg95 = i+1
	addu $s1,$0,$t1 # $s1=#reg95
	li $t2,23
	blt $t1,$t2,Label_46 # #reg95<23
	li $s2,'a' # $s2='a'
	li $s1,0 # $s1=0
	li $s0,0 # $s0=0
Label_47:
	sll $t0,$s1,2
	subi $t1,$fp,124
	subu $t1,$t1,$t0
	sw $s2,0($t1) # buf[i] = ch1
	li $t0,1
	addu $s0,$s0,$t0 # toy = toy+1
	addu $t1,$s1,$t0 # #reg104 = i+1
	addu $t2,$s2,$t0 # #reg103 = ch1+1
	addu $s2,$0,$t2 # $s2=#reg103
	addu $s1,$0,$t1 # $s1=#reg104
	li $t3,127
	blt $t2,$t3,Label_47 # #reg103<127
	addu $s1,$0,$s1 # $s1=i
	li $s2,0 # $s2=0
	li $s0,0 # $s0=0
Label_48:
	li $t0,2
	li $t1,1
	div $t2,$t0,$t1 # #reg111 = 2/1
	mul $t3,$s2,$t1 # #reg244 = _*1
	subu $t4,$t3,$s2 # #reg245 = #reg244-_
	subu $t5,$t4,$t1 # #reg246 = #reg245-1
	addu $t6,$t5,$t2 # #reg247 = #reg246+#reg111
	subu $t7,$t6,$t1 # #reg248 = #reg247-1
	div $t8,$t7,$t0 # #reg110 = #reg248/2
	sll $t9,$t8,2
	sw $t5,-1172($fp)
	subi $t5,$fp,124
	subu $t5,$t5,$t9
	sw $t6,-1176($fp)
	li $t6,'a'
	sw $t6,0($t5) # buf[#reg110] = 'a'
	li $t0,1
	addu $s0,$s0,$t0 # toy = toy+1
	addu $t1,$s2,$t0 # #reg113 = _+1
	addu $s2,$0,$t1 # $s2=#reg113
	blt $t1,$s1,Label_48 # #reg113<endid
	li $t0,0
	sll $t1,$t0,2
	subi $t2,$fp,124
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
	subi $sp,$sp,8 # make space for $fp and $ra
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
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,8 # $fp = old $sp
	subi $sp,$sp,16 # make space for locals and temps
	lw $t0,0($fp) # load $t0 = n
	addu $s0,$0,$t0 # $s0=n
Label_52:
	li $t0,0
	bne $s0,$t0,Label_53 # #reg119!=0
	li $v1,0 # @RET = 0
	j exit_Func_19
	j Label_51
Label_53:
	li $t0,1
	bne $s0,$t0,Label_54 # #reg119!=1
	li $v1,1 # @RET = 1
	j exit_Func_19
	j Label_51
Label_54:
	li $t0,2
	bne $s0,$t0,Label_55 # #reg119!=2
	li $v1,1 # @RET = 1
	j exit_Func_19
	j Label_51
Label_55:
	j Label_51
Label_51:
	lw $t0,0($fp) # load $t0 = n
	li $t1,1
	subu $t2,$t0,$t1 # #reg123 = n-1
	subi $sp,$sp,4
	sw $t2,0($sp)
	jal Func_19
	lw $t0,0($fp) # load $t0 = n
	li $t1,2
	subu $t2,$t0,$t1 # #reg125 = n-2
	addu $s0,$0,$v1 # $s0=@RET
	subi $sp,$sp,4
	sw $t2,0($sp)
	sw $s0,-24($fp) # save #reg124(global reg)
	jal Func_19
	lw $s0,-24($fp) # save #reg124(global reg)
	addu $t0,$0,$v1
	addu $s0,$s0,$t0 # #reg124 = #reg124+@RET
	addu $v1,$0,$s0 # @RET = #reg124
	j exit_Func_19
exit_Func_19: # exit function fibonaci
	addiu $sp,$sp,16 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,12
	jr $ra
Func_20: # function fibonaci_test
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	subi $sp,$sp,8 # make space for locals and temps
	li $v0,4 # print string
	la $a0,_str14
	syscall 
	li $v0,5 # read int : n
	syscall 
	addu $s1,$0,$v0
	li $s0,0 # $s0=0
Label_56:
	li $t0,0
	bge $s1,$t0,Label_57 # n>=0
	li $v0,4 # print string
	la $a0,_str15
	syscall 
	li $v0,5 # read int : n
	syscall 
	addu $s1,$0,$v0
	j Label_58
Label_57:
Label_58:
	li $t0,0
	blt $s1,$t0,Label_59 # n<0
	subi $sp,$sp,4
	sw $s1,0($sp)
	sw $s0,-12($fp) # save toy(global reg)
	sw $s1,-8($fp) # save n(global reg)
	jal Func_19
	lw $s0,-12($fp) # save toy(global reg)
	lw $s1,-8($fp) # save n(global reg)
	li $v0,4 # print string
	la $a0,_str16
	syscall 
	li $v0,1# print int : @RET
	addu $a0,$0,$v1
	syscall 
	j Label_60
Label_59:
Label_60:
	li $t0,1
	addu $s0,$s0,$t0 # toy = toy+1
	li $t1,0
	blt $s1,$t1,Label_56 # n<0
exit_Func_20: # exit function fibonaci_test
	addiu $sp,$sp,8 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,8
	jr $ra
Func_21: # function mod
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,12 # $fp = old $sp
	subi $sp,$sp,12 # make space for locals and temps
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
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,12 # $fp = old $sp
	subi $sp,$sp,4 # make space for locals and temps
	lw $t0,0($fp) # load $t0 = n
	li $t1,0
	bne $t0,$t1,Label_61 # n!=0
	lw $v1,-4($fp) # @RET = m
	j exit_Func_22
	j Label_62
Label_61:
Label_62:
	lw $t0,-4($fp) # load $t0 = m
	li $t1,0
	bne $t0,$t1,Label_63 # m!=0
	lw $v1,0($fp) # @RET = n
	j exit_Func_22
	j Label_64
Label_63:
Label_64:
	lw $t0,-4($fp) # load $t0 = m
	subi $sp,$sp,4
	sw $t0,0($sp)
	lw $t1,0($fp) # load $t1 = n
	subi $sp,$sp,4
	sw $t1,0($sp)
	subi $sp,$sp,4
	sw $t0,0($sp)
	jal Func_21
	addu $t0,$0,$v1
	subi $sp,$sp,4
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
	subi $sp,$sp,8 # make space for $fp and $ra
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
	lw $t0,-4($fp) # load $t0 = m
	li $t1,0
	bgt $t0,$t1,Label_67 # m>0
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
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	subi $sp,$sp,12 # make space for locals and temps
	li $v0,4 # print string
	la $a0,_str17
	syscall 
	li $v0,5 # read int : n
	syscall 
	addu $s1,$0,$v0
	li $v0,5 # read int : m
	syscall 
	addu $s2,$0,$v0
	subi $sp,$sp,4
	sw $s1,0($sp)
	subi $sp,$sp,4
	sw $s2,0($sp)
	sw $s1,-8($fp) # save n(global reg)
	sw $s2,-12($fp) # save m(global reg)
	jal Func_23
	lw $s1,-8($fp) # save n(global reg)
	lw $s2,-12($fp) # save m(global reg)
	addu $t0,$0,$v1
	li $t1,0
	beq $t0,$t1,Label_69 # @RET==0
	li $s0,0 # $s0=0
Label_70:
	li $v0,4 # print string
	la $a0,_str18
	syscall 
	li $v0,5 # read int : n
	syscall 
	addu $s1,$0,$v0
	li $v0,5 # read int : m
	syscall 
	addu $s2,$0,$v0
	li $t0,1
	addu $s0,$s0,$t0 # toy = toy+1
	subi $sp,$sp,4
	sw $s1,0($sp)
	subi $sp,$sp,4
	sw $s2,0($sp)
	sw $s0,-16($fp) # save toy(global reg)
	sw $s1,-8($fp) # save n(global reg)
	sw $s2,-12($fp) # save m(global reg)
	jal Func_23
	lw $s0,-16($fp) # save toy(global reg)
	lw $s1,-8($fp) # save n(global reg)
	lw $s2,-12($fp) # save m(global reg)
	addu $t0,$0,$v1
	li $t1,0
	bne $t0,$t1,Label_70 # @RET!=0
	j Label_71
Label_69:
Label_71:
	subi $sp,$sp,4
	sw $s1,0($sp)
	subi $sp,$sp,4
	sw $s2,0($sp)
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
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,8 # $fp = old $sp
	subi $sp,$sp,16 # make space for locals and temps
	li $s1,2 # $s1=2
	lw $t0,0($fp) # load $t0 = x
	li $t1,2
	bne $t0,$t1,Label_72 # x!=2
	li $v1,1 # @RET = 1
	j exit_Func_25
	j Label_73
Label_72:
Label_73:
	li $s0,0 # $s0=0
Label_74:
	lw $t0,0($fp) # load $t0 = x
	subi $sp,$sp,4
	sw $t0,0($sp)
	subi $sp,$sp,4
	sw $s1,0($sp)
	sw $s0,-16($fp) # save toy(global reg)
	sw $s1,-12($fp) # save i(global reg)
	jal Func_21
	lw $s0,-16($fp) # save toy(global reg)
	lw $s1,-12($fp) # save i(global reg)
	addu $t0,$0,$v1
	li $t1,0
	bne $t0,$t1,Label_75 # @RET!=0
	li $v1,0 # @RET = 0
	j exit_Func_25
	j Label_76
Label_75:
Label_76:
	li $t0,1
	addu $t1,$s1,$t0 # #reg176 = i+1
	mul $t2,$t1,$t1 # #reg177 = #reg176*#reg176
	addu $s0,$s0,$t0 # toy = toy+1
	addu $s1,$0,$t1 # $s1=#reg176
	lw $t3,0($fp) # load $t3 = x
	blt $t2,$t3,Label_74 # #reg177<x
	li $v1,1 # @RET = 1
	j exit_Func_25
exit_Func_25: # exit function is_prime
	addiu $sp,$sp,16 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,12
	jr $ra
Func_26: # function prime_factorization
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,8 # $fp = old $sp
	subi $sp,$sp,28 # make space for locals and temps
	li $s1,2 # $s1=2
	li $s3,0 # $s3=0
	lw $t0,0($fp) # load $t0 = n
	li $t1,1
	bne $t0,$t1,Label_77 # n!=1
	li $v0,4 # print string
	la $a0,_str20
	syscall 
	j exit_Func_26
	j Label_78
Label_77:
Label_78:
	li $s2,0 # $s2=0
Label_79:
	subi $sp,$sp,4
	sw $s1,0($sp)
	sw $s1,-12($fp) # save i(global reg)
	sw $s2,-24($fp) # save fuck(global reg)
	sw $s3,-16($fp) # save first(global reg)
	jal Func_25
	lw $s1,-12($fp) # save i(global reg)
	lw $s2,-24($fp) # save fuck(global reg)
	lw $s3,-16($fp) # save first(global reg)
	addu $t0,$0,$v1
	li $t1,1
	bne $t0,$t1,Label_80 # @RET!=1
	lw $t0,0($fp) # load $t0 = n
	subi $sp,$sp,4
	sw $t0,0($sp)
	subi $sp,$sp,4
	sw $s1,0($sp)
	sw $s1,-12($fp) # save i(global reg)
	sw $s2,-24($fp) # save fuck(global reg)
	sw $s3,-16($fp) # save first(global reg)
	jal Func_21
	lw $s1,-12($fp) # save i(global reg)
	lw $s2,-24($fp) # save fuck(global reg)
	lw $s3,-16($fp) # save first(global reg)
	addu $t0,$0,$v1
	li $t1,0
	bne $t0,$t1,Label_81 # @RET!=0
	li $s0,0 # $s0=0
Label_82:
	lw $t0,0($fp) # load $t0 = n
	div $t1,$t0,$s1 # #reg193 = n/i
	addu $t0,$0,$t1 # $t0=#reg193
	li $t2,1
	sw $t0,0($fp) # spill n
	bne $s3,$t2,Label_83 # first!=1
	li $v0,4 # print string
	la $a0,_str21
	syscall 
	j Label_84
Label_83:
Label_84:
	li $v0,1# print int : i
	addu $a0,$0,$s1
	syscall 
	li $t0,1
	addu $s0,$s0,$t0 # toy = toy+1
	li $s3,1 # $s3=1
	lw $t1,0($fp) # load $t1 = n
	subi $sp,$sp,4
	sw $t1,0($sp)
	subi $sp,$sp,4
	sw $s1,0($sp)
	sw $s0,-20($fp) # save toy(global reg)
	sw $s1,-12($fp) # save i(global reg)
	sw $s2,-24($fp) # save fuck(global reg)
	sw $s3,-16($fp) # save first(global reg)
	jal Func_21
	lw $s0,-20($fp) # save toy(global reg)
	lw $s1,-12($fp) # save i(global reg)
	lw $s2,-24($fp) # save fuck(global reg)
	lw $s3,-16($fp) # save first(global reg)
	addu $t0,$0,$v1
	li $t1,0
	beq $t0,$t1,Label_82 # @RET==0
	j Label_85
Label_81:
Label_85:
	j Label_86
Label_80:
Label_86:
	li $t0,1
	addu $t1,$s1,$t0 # #reg202 = i+1
	mul $t2,$t1,$t1 # #reg203 = #reg202*#reg202
	addu $s2,$s2,$t0 # fuck = fuck+1
	addu $s1,$0,$t1 # $s1=#reg202
	lw $t3,0($fp) # load $t3 = n
	blt $t2,$t3,Label_79 # #reg203<n
	lw $t0,0($fp) # load $t0 = n
	li $t1,1
	beq $t0,$t1,Label_87 # n==1
	li $t0,1
	bne $s3,$t0,Label_88 # first!=1
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
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	subi $sp,$sp,8 # make space for locals and temps
	li $v0,4 # print string
	la $a0,_str23
	syscall 
	li $v0,5 # read int : n
	syscall 
	addu $s1,$0,$v0
	li $s0,0 # $s0=0
Label_91:
	li $t0,0
	bge $s1,$t0,Label_92 # n>=0
	li $v0,4 # print string
	la $a0,_str24
	syscall 
	li $v0,5 # read int : n
	syscall 
	addu $s1,$0,$v0
	j Label_93
Label_92:
Label_93:
	li $t0,1
	addu $s0,$s0,$t0 # toy = toy+1
	li $t1,0
	blt $s1,$t1,Label_91 # n<0
	li $v0,4 # print string
	la $a0,_str25
	syscall 
	subi $sp,$sp,4
	sw $s1,0($sp)
	jal Func_26
exit_Func_27: # exit function prime_factorization_test
	addiu $sp,$sp,8 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,8
	jr $ra
Func_28: # function iswrong
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,8 # $fp = old $sp
	li $t0,'E'
	lw $t1,0($fp) # load $t1 = A
	bne $t0,$t1,Label_94 # 'E'!=A
	li $v1,0 # @RET = 0
	j exit_Func_28
	j Label_95
Label_94:
Label_95:
	li $t0,'M'
	lw $t1,0($fp) # load $t1 = A
	bne $t0,$t1,Label_96 # 'M'!=A
	li $v1,0 # @RET = 0
	j exit_Func_28
	j Label_97
Label_96:
Label_97:
	li $t0,'H'
	lw $t1,0($fp) # load $t1 = A
	bne $t0,$t1,Label_98 # 'H'!=A
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
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addiu $fp,$sp,4 # $fp = old $sp
	subi $sp,$sp,12 # make space for locals and temps
	li $v0,4 # print string
	la $a0,_str26
	syscall 
	li $v0,12 # read char : kind
	syscall 
	addu $s1,$0,$v0
	subi $sp,$sp,4
	sw $s1,0($sp)
	sw $s1,-8($fp) # save kind(global reg)
	jal Func_28
	lw $s1,-8($fp) # save kind(global reg)
	addu $t0,$0,$v1
	li $t1,0
	beq $t0,$t1,Label_100 # @RET==0
	li $s0,0 # $s0=0
Label_101:
	li $v0,4 # print string
	la $a0,_str27
	syscall 
	li $v0,12 # read char : kind
	syscall 
	addu $s1,$0,$v0
	li $t0,1
	addu $s0,$s0,$t0 # toy = toy+1
	subi $sp,$sp,4
	sw $s1,0($sp)
	sw $s0,-12($fp) # save toy(global reg)
	sw $s1,-8($fp) # save kind(global reg)
	jal Func_28
	lw $s0,-12($fp) # save toy(global reg)
	lw $s1,-8($fp) # save kind(global reg)
	addu $t0,$0,$v1
	li $t1,0
	bne $t0,$t1,Label_101 # @RET!=0
	j Label_102
Label_100:
Label_102:
	addu $s0,$0,$s1 # $s0=kind
Label_104:
	li $t0,'E'
	bne $s0,$t0,Label_105 # #reg231!='E'
	jal Func_5
	jal Func_7
	jal Func_20
	j Label_103
Label_105:
	li $t0,'M'
	bne $s0,$t0,Label_106 # #reg231!='M'
	jal Func_11
	jal Func_16
	jal Func_9
	jal Func_24
	j Label_103
Label_106:
	li $t0,'H'
	bne $s0,$t0,Label_107 # #reg231!='H'
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
