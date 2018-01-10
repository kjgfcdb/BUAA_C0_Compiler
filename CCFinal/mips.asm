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
subu $gp,$gp,20512 # make space for global variabel and constant
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
	subu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addu $fp,$sp,4 # fp = new base
	subu $sp,$sp,40 # make space for locals and temps
	li $t0,1 # a=1
	sw $t0,-8($fp)
	li $t0,0 # b=0
	sw $t0,-12($fp)
	li $t0,-1 # c=-1
	sw $t0,-16($fp)
	li $t0,120 # cst23=120
	sw $t0,-20($fp)
	li $t0,122 # cst24=122
	sw $t0,-24($fp)
	li $t0,65 # constA0=65
	sw $t0,-28($fp)
	li $t0,49 # cstA1=49
	sw $t0,-32($fp)
	li $t0,47 # cst_div=47
	sw $t0,-36($fp)
	lw $t0,-8($fp) # #reg0=a
	sw $t0,-40($fp)
	li $v0,1# print int #reg0
	lw $a0,-40($fp)
	syscall 
	lw $t0,-36($fp) # #reg1=cst_div
	sw $t0,-44($fp)
	li $v0,11 # print char : #reg1
	lw $a0,-44($fp)
	syscall 
exit_Func_0: # exit function const_define
	addu $sp,$sp,40 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addu $sp,$sp,8
	jr $ra

Func_1: # function const_define_test
	subu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addu $fp,$sp,4 # fp = new base
	subu $sp,$sp,0 # make space for locals and temps
	jal Func_0
	addu $v1,$0,$v0 # save the return value
exit_Func_1: # exit function const_define_test
	addu $sp,$sp,0 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addu $sp,$sp,8
	jr $ra

Func_2: # function var_define
	subu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addu $fp,$sp,4 # fp = new base
	subu $sp,$sp,80 # make space for locals and temps
exit_Func_2: # exit function var_define
	addu $sp,$sp,80 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addu $sp,$sp,8
	jr $ra

Func_3: # function var_define_test
	subu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addu $fp,$sp,4 # fp = new base
	subu $sp,$sp,0 # make space for locals and temps
	jal Func_2
	addu $v1,$0,$v0 # save the return value
exit_Func_3: # exit function var_define_test
	addu $sp,$sp,0 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addu $sp,$sp,8
	jr $ra

Func_4: # function define
	subu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addu $fp,$sp,4 # fp = new base
	subu $sp,$sp,24 # make space for locals and temps
	li $t0,1 # int1=1
	sw $t0,-8($fp)
	li $t0,122 # cst24=122
	sw $t0,-12($fp)
	li $t0,65 # constA0=65
	sw $t0,-16($fp)
exit_Func_4: # exit function define
	addu $sp,$sp,24 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addu $sp,$sp,8
	jr $ra

Func_5: # function define_test
	subu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addu $fp,$sp,4 # fp = new base
	subu $sp,$sp,0 # make space for locals and temps
	jal Func_3
	addu $v1,$0,$v0 # save the return value
	jal Func_1
	addu $v1,$0,$v0 # save the return value
	jal Func_4
	addu $v1,$0,$v0 # save the return value
	li $v0,4 # print string
	la $a0,_str0
	syscall 
exit_Func_5: # exit function define_test
	addu $sp,$sp,0 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addu $sp,$sp,8
	jr $ra

Func_6: # function operator
	subu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addu $fp,$sp,4 # fp = new base
	subu $sp,$sp,88 # make space for locals and temps
	li $t0,1 # #reg2=1
	sw $t0,-32($fp)
	lw $t0,-32($fp) # a=#reg2
	sw $t0,-8($fp)
	li $t0,1
	li $t1,2
	addu $t2,$t0,$t1
	sw $t2,-36($fp)
	lw $t0,-36($fp) # b=#reg3
	sw $t0,-12($fp)
	li $t0,3
	li $t1,1
	mul $t2,$t0,$t1
	sw $t2,-40($fp)
	lw $t0,-40($fp) # c=#reg4
	sw $t0,-16($fp)
	li $t0,0
	li $t1,4
	subu $t2,$t0,$t1
	sw $t2,-44($fp)
	lw $t0,-44($fp) # d=#reg5
	sw $t0,-20($fp)
	li $t0,5 # #reg6=5
	sw $t0,-48($fp)
	lw $t0,-48($fp) # e=#reg6
	sw $t0,-24($fp)
	li $t0,6 # #reg7=6
	sw $t0,-52($fp)
	lw $t0,-52($fp) # f=#reg7
	sw $t0,-28($fp)
	lw $t0,-12($fp)
	li $t1,3
	mul $t2,$t0,$t1
	sw $t2,-56($fp)
	lw $t0,-56($fp)
	lw $t1,-16($fp)
	addu $t2,$t0,$t1
	sw $t2,-56($fp)
	lw $t0,-20($fp)
	li $t1,4
	div $t2,$t0,$t1
	sw $t2,-60($fp)
	lw $t0,-56($fp)
	lw $t1,-60($fp)
	subu $t2,$t0,$t1
	sw $t2,-56($fp)
	lw $t0,-8($fp)
	lw $t1,-8($fp)
	div $t2,$t0,$t1
	sw $t2,-64($fp)
	lw $t0,-56($fp)
	lw $t1,-64($fp)
	addu $t2,$t0,$t1
	sw $t2,-56($fp)
	lw $t0,-56($fp) # a=#reg8
	sw $t0,-8($fp)
	li $t0,0
	lw $t1,-8($fp)
	addu $t2,$t0,$t1
	sw $t2,-68($fp)
	lw $t0,-8($fp)
	li $t1,3
	mul $t2,$t0,$t1
	sw $t2,-72($fp)
	lw $t0,-68($fp)
	lw $t1,-72($fp)
	addu $t2,$t0,$t1
	sw $t2,-68($fp)
	lw $t0,-68($fp)
	lw $t1,-16($fp)
	addu $t2,$t0,$t1
	sw $t2,-68($fp)
	lw $t0,-20($fp)
	li $t1,4
	div $t2,$t0,$t1
	sw $t2,-76($fp)
	lw $t0,-68($fp)
	lw $t1,-76($fp)
	subu $t2,$t0,$t1
	sw $t2,-68($fp)
	lw $t0,-24($fp)
	lw $t1,-28($fp)
	mul $t2,$t0,$t1
	sw $t2,-80($fp)
	lw $t0,-68($fp)
	lw $t1,-80($fp)
	addu $t2,$t0,$t1
	sw $t2,-68($fp)
	lw $t0,-68($fp) # b=#reg11
	sw $t0,-12($fp)
	lw $t0,-8($fp) # #reg15=a
	sw $t0,-84($fp)
	li $t0,0
	lw $t1,-84($fp)
	subu $t2,$t0,$t1
	sw $t2,-84($fp)
	lw $t0,-12($fp)
	li $t1,0
	mul $t2,$t0,$t1
	sw $t2,-88($fp)
	lw $t0,-84($fp)
	lw $t1,-88($fp)
	addu $t2,$t0,$t1
	sw $t2,-84($fp)
	li $t0,1
	li $t1,0
	subu $t2,$t0,$t1
	sw $t2,-92($fp)
	lw $t0,-84($fp)
	lw $t1,-92($fp)
	subu $t2,$t0,$t1
	sw $t2,-84($fp)
	lw $t0,-84($fp) # e=#reg15
	sw $t0,-24($fp)
exit_Func_6: # exit function operator
	addu $sp,$sp,88 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addu $sp,$sp,8
	jr $ra

Func_7: # function operator_test
	subu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addu $fp,$sp,4 # fp = new base
	subu $sp,$sp,0 # make space for locals and temps
	jal Func_6
	addu $v1,$0,$v0 # save the return value
	li $v0,4 # print string
	la $a0,_str1
	syscall 
exit_Func_7: # exit function operator_test
	addu $sp,$sp,0 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addu $sp,$sp,8
	jr $ra

Func_8: # function string
	subu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addu $fp,$sp,4 # fp = new base
	subu $sp,$sp,0 # make space for locals and temps
	li $v0,4 # print string
	la $a0,_str2
	syscall 
	li $v0,4 # print string
	la $a0,_str3
	syscall 
exit_Func_8: # exit function string
	addu $sp,$sp,0 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addu $sp,$sp,8
	jr $ra

Func_9: # function string_test
	subu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addu $fp,$sp,4 # fp = new base
	subu $sp,$sp,0 # make space for locals and temps
	jal Func_8
	addu $v1,$0,$v0 # save the return value
exit_Func_9: # exit function string_test
	addu $sp,$sp,0 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addu $sp,$sp,8
	jr $ra

Func_10: # function dowhile
	subu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addu $fp,$sp,4 # fp = new base
	subu $sp,$sp,60 # make space for locals and temps
	li $t0,100 # #reg18=100
	sw $t0,-16($fp)
	lw $t0,-16($fp) # i=#reg18
	sw $t0,-8($fp)
	li $t0,0 # #reg19=0
	sw $t0,-20($fp)
	lw $t0,-20($fp) # toy=#reg19
	sw $t0,-12($fp)
Label_0:
	lw $t0,-8($fp) # #reg20=i
	sw $t0,-24($fp)
	li $t0,50 # #reg21=50
	sw $t0,-28($fp)
	lw $t0,-24($fp)
	lw $t1,-28($fp)
	blt $t0,$t1,Label_1
	lw $t0,-8($fp)
	li $t1,2
	div $t2,$t0,$t1
	sw $t2,-32($fp)
	lw $t0,-32($fp) # i=#reg22
	sw $t0,-8($fp)
	j Label_2
Label_1:
Label_2:
	lw $t0,-8($fp) # #reg23=i
	sw $t0,-36($fp)
	li $t0,50 # #reg24=50
	sw $t0,-40($fp)
	lw $t0,-36($fp)
	lw $t1,-40($fp)
	bge $t0,$t1,Label_3
	lw $t0,-8($fp)
	li $t1,10
	addu $t2,$t0,$t1
	sw $t2,-44($fp)
	lw $t0,-44($fp)
	li $t1,10
	div $t2,$t0,$t1
	sw $t2,-44($fp)
	lw $t0,-44($fp) # i=#reg25
	sw $t0,-8($fp)
	j Label_4
Label_3:
Label_4:
	lw $t0,-12($fp)
	li $t1,1
	addu $t2,$t0,$t1
	sw $t2,-12($fp)
	lw $t0,-8($fp) # #reg26=i
	sw $t0,-48($fp)
	li $t0,10 # #reg27=10
	sw $t0,-52($fp)
	lw $t0,-48($fp)
	lw $t1,-52($fp)
	bge $t0,$t1,Label_0
	li $t0,0 # #reg28=0
	sw $t0,-56($fp)
	lw $t0,-56($fp) # toy=#reg28
	sw $t0,-12($fp)
Label_5:
	lw $t0,-8($fp)
	li $t1,1
	subu $t2,$t0,$t1
	sw $t2,-60($fp)
	lw $t0,-60($fp) # i=#reg29
	sw $t0,-8($fp)
	li $v0,4 # print string
	la $a0,_str4
	syscall 
	lw $t0,-12($fp)
	li $t1,1
	addu $t2,$t0,$t1
	sw $t2,-12($fp)
	li $t0,0 # #reg30=0
	sw $t0,-64($fp)
	lw $t0,-64($fp)
	li $t1,0
	bne $t0,$t1,Label_5
exit_Func_10: # exit function dowhile
	addu $sp,$sp,60 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addu $sp,$sp,8
	jr $ra

Func_11: # function dowhile_test
	subu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addu $fp,$sp,4 # fp = new base
	subu $sp,$sp,0 # make space for locals and temps
	jal Func_10
	addu $v1,$0,$v0 # save the return value
exit_Func_11: # exit function dowhile_test
	addu $sp,$sp,0 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addu $sp,$sp,8
	jr $ra

Func_12: # function ifcondition
	subu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addu $fp,$sp,4 # fp = new base
	subu $sp,$sp,116 # make space for locals and temps
	li $t0,1 # #reg31=1
	sw $t0,-16($fp)
	lw $t0,-16($fp) # a=#reg31
	sw $t0,-8($fp)
	li $t0,0 # #reg32=0
	sw $t0,-20($fp)
	lw $t0,-20($fp) # rightnumber=#reg32
	sw $t0,-12($fp)
	lw $t0,-8($fp) # #reg33=a
	sw $t0,-24($fp)
	li $t0,1 # #reg34=1
	sw $t0,-28($fp)
	lw $t0,-24($fp)
	lw $t1,-28($fp)
	bne $t0,$t1,Label_6
	lw $t0,-12($fp)
	li $t1,1
	addu $t2,$t0,$t1
	sw $t2,-32($fp)
	lw $t0,-32($fp) # rightnumber=#reg35
	sw $t0,-12($fp)
	j Label_7
Label_6:
Label_7:
	li $t0,2 # #reg36=2
	sw $t0,-36($fp)
	lw $t0,-36($fp) # a=#reg36
	sw $t0,-8($fp)
	lw $t0,-8($fp) # #reg37=a
	sw $t0,-40($fp)
	li $t0,1 # #reg38=1
	sw $t0,-44($fp)
	lw $t0,-40($fp)
	lw $t1,-44($fp)
	ble $t0,$t1,Label_8
	lw $t0,-12($fp)
	li $t1,1
	addu $t2,$t0,$t1
	sw $t2,-48($fp)
	lw $t0,-48($fp) # rightnumber=#reg39
	sw $t0,-12($fp)
	j Label_9
Label_8:
Label_9:
	li $t0,0 # #reg40=0
	sw $t0,-52($fp)
	lw $t0,-52($fp) # a=#reg40
	sw $t0,-8($fp)
	lw $t0,-8($fp) # #reg41=a
	sw $t0,-56($fp)
	li $t0,1 # #reg42=1
	sw $t0,-60($fp)
	lw $t0,-56($fp)
	lw $t1,-60($fp)
	bge $t0,$t1,Label_10
	lw $t0,-12($fp)
	li $t1,1
	addu $t2,$t0,$t1
	sw $t2,-64($fp)
	lw $t0,-64($fp) # rightnumber=#reg43
	sw $t0,-12($fp)
	j Label_11
Label_10:
Label_11:
	lw $t0,-8($fp) # #reg44=a
	sw $t0,-68($fp)
	li $t0,1 # #reg45=1
	sw $t0,-72($fp)
	lw $t0,-68($fp)
	lw $t1,-72($fp)
	beq $t0,$t1,Label_12
	lw $t0,-12($fp)
	li $t1,1
	addu $t2,$t0,$t1
	sw $t2,-76($fp)
	lw $t0,-76($fp) # rightnumber=#reg46
	sw $t0,-12($fp)
	j Label_13
Label_12:
Label_13:
	lw $t0,-8($fp) # #reg47=a
	sw $t0,-80($fp)
	li $t0,0 # #reg48=0
	sw $t0,-84($fp)
	lw $t0,-80($fp)
	lw $t1,-84($fp)
	blt $t0,$t1,Label_14
	lw $t0,-12($fp)
	li $t1,1
	addu $t2,$t0,$t1
	sw $t2,-88($fp)
	lw $t0,-88($fp) # rightnumber=#reg49
	sw $t0,-12($fp)
	j Label_15
Label_14:
Label_15:
	lw $t0,-8($fp) # #reg50=a
	sw $t0,-92($fp)
	li $t0,0 # #reg51=0
	sw $t0,-96($fp)
	lw $t0,-92($fp)
	lw $t1,-96($fp)
	bgt $t0,$t1,Label_16
	lw $t0,-12($fp)
	li $t1,1
	addu $t2,$t0,$t1
	sw $t2,-100($fp)
	lw $t0,-100($fp) # rightnumber=#reg52
	sw $t0,-12($fp)
	j Label_17
Label_16:
Label_17:
	lw $t0,-12($fp) # #reg53=rightnumber
	sw $t0,-104($fp)
	li $t0,1
	li $t1,1
	addu $t2,$t0,$t1
	sw $t2,-108($fp)
	lw $t0,-108($fp)
	li $t1,1
	addu $t2,$t0,$t1
	sw $t2,-108($fp)
	li $t0,1
	li $t1,3
	mul $t2,$t0,$t1
	sw $t2,-112($fp)
	lw $t0,-108($fp)
	lw $t1,-112($fp)
	addu $t2,$t0,$t1
	sw $t2,-108($fp)
	lw $t0,-104($fp)
	lw $t1,-108($fp)
	beq $t0,$t1,Label_18
	li $v0,4 # print string
	la $a0,_str5
	syscall 
	j Label_19
Label_18:
Label_19:
	lw $t0,-12($fp) # #reg56=rightnumber
	sw $t0,-116($fp)
	lw $t0,-12($fp) # #reg57=rightnumber
	sw $t0,-120($fp)
	lw $t0,-116($fp)
	lw $t1,-120($fp)
	beq $t0,$t1,Label_20
	li $v0,4 # print string
	la $a0,_str6
	syscall 
	j Label_21
Label_20:
Label_21:
exit_Func_12: # exit function ifcondition
	addu $sp,$sp,116 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addu $sp,$sp,8
	jr $ra

Func_13: # function switchcodintionans
	subu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addu $fp,$sp,8 # fp = new base
	subu $sp,$sp,20 # make space for locals and temps
	lw $t0,0($fp) # #reg58=x
	sw $t0,-12($fp)
Label_23:
	lw $t0,-12($fp)
	li $t1,1
	bne $t0,$t1,Label_24
	li $t0,2 # #reg59=2
	sw $t0,-16($fp)
	lw $v0,-16($fp)
	j exit_Func_13
	j Label_22
Label_24:
	lw $t0,-12($fp)
	li $t1,2
	bne $t0,$t1,Label_25
	li $t0,3 # #reg60=3
	sw $t0,-20($fp)
	lw $v0,-20($fp)
	j exit_Func_13
	j Label_22
Label_25:
	lw $t0,-12($fp)
	li $t1,3
	bne $t0,$t1,Label_26
	li $t0,4 # #reg61=4
	sw $t0,-24($fp)
	lw $v0,-24($fp)
	j exit_Func_13
	j Label_22
Label_26:
	j Label_22
Label_22:
	li $t0,5 # #reg62=5
	sw $t0,-28($fp)
	lw $v0,-28($fp)
	j exit_Func_13
exit_Func_13: # exit function switchcodintionans
	addu $sp,$sp,20 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addu $sp,$sp,12
	jr $ra

Func_14: # function switchcodintion
	subu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addu $fp,$sp,4 # fp = new base
	subu $sp,$sp,68 # make space for locals and temps
	li $t0,1 # #reg63=1
	sw $t0,-20($fp)
	lw $t0,-20($fp) # i=#reg63
	sw $t0,-8($fp)
	li $t0,0 # #reg64=0
	sw $t0,-24($fp)
	lw $t0,-24($fp) # rightnumber=#reg64
	sw $t0,-12($fp)
	li $t0,0 # #reg65=0
	sw $t0,-28($fp)
	lw $t0,-28($fp) # toy=#reg65
	sw $t0,-16($fp)
Label_27:
	lw $t0,-8($fp)
	li $t1,1
	addu $t2,$t0,$t1
	sw $t2,-32($fp)
	lw $t0,-8($fp) # #reg67=i
	sw $t0,-36($fp)
	lw $t0,-36($fp) # push #reg67
	subu $sp,$sp,4
	sw $t0,0($sp)
	jal Func_13
	addu $v1,$0,$v0 # save the return value
	sw $v1,-40($fp) # #reg68=@RET
	lw $t0,-32($fp)
	lw $t1,-40($fp)
	bne $t0,$t1,Label_28
	lw $t0,-12($fp)
	li $t1,1
	addu $t2,$t0,$t1
	sw $t2,-44($fp)
	lw $t0,-44($fp) # rightnumber=#reg69
	sw $t0,-12($fp)
	j Label_29
Label_28:
Label_29:
	lw $t0,-8($fp)
	li $t1,1
	addu $t2,$t0,$t1
	sw $t2,-48($fp)
	lw $t0,-48($fp) # i=#reg70
	sw $t0,-8($fp)
	lw $t0,-16($fp)
	li $t1,1
	addu $t2,$t0,$t1
	sw $t2,-16($fp)
	lw $t0,-8($fp) # #reg71=i
	sw $t0,-52($fp)
	li $t0,5 # #reg72=5
	sw $t0,-56($fp)
	lw $t0,-52($fp)
	lw $t1,-56($fp)
	blt $t0,$t1,Label_27
	lw $t0,-12($fp) # #reg73=rightnumber
	sw $t0,-60($fp)
	li $t0,4 # #reg74=4
	sw $t0,-64($fp)
	lw $t0,-60($fp)
	lw $t1,-64($fp)
	beq $t0,$t1,Label_30
	li $v0,4 # print string
	la $a0,_str7
	syscall 
	j Label_31
Label_30:
Label_31:
	lw $t0,-12($fp) # #reg75=rightnumber
	sw $t0,-68($fp)
	lw $t0,-68($fp)
	li $t1,0
	beq $t0,$t1,Label_32
	lw $t0,-8($fp) # #reg76=i
	sw $t0,-72($fp)
	lw $v0,-72($fp)
	j exit_Func_14
	j Label_33
Label_32:
Label_33:
	li $v0,4 # print string
	la $a0,_str8
	syscall 
exit_Func_14: # exit function switchcodintion
	addu $sp,$sp,68 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addu $sp,$sp,8
	jr $ra

Func_15: # function condition
	subu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addu $fp,$sp,4 # fp = new base
	subu $sp,$sp,60 # make space for locals and temps
	li $t0,5 # intermax=5
	sw $t0,-8($fp)
	li $t0,0 # #reg77=0
	sw $t0,-20($fp)
	lw $t0,-20($fp) # iter=#reg77
	sw $t0,-12($fp)
	li $t0,0 # #reg78=0
	sw $t0,-24($fp)
	lw $t0,-24($fp) # toy=#reg78
	sw $t0,-16($fp)
Label_34:
	lw $t0,-12($fp) # #reg79=iter
	sw $t0,-28($fp)
Label_36:
	lw $t0,-28($fp)
	li $t1,0
	bne $t0,$t1,Label_37
	lw $t0,-12($fp) # #reg80=iter
	sw $t0,-32($fp)
	li $t0,0 # #reg81=0
	sw $t0,-36($fp)
	lw $t0,-32($fp)
	lw $t1,-36($fp)
	beq $t0,$t1,Label_38
	li $v0,4 # print string
	la $a0,_str9
	syscall 
	j Label_39
Label_38:
Label_39:
	j Label_35
Label_37:
	lw $t0,-28($fp)
	li $t1,1
	bne $t0,$t1,Label_40
	lw $t0,-12($fp) # #reg82=iter
	sw $t0,-40($fp)
	li $t0,1 # #reg83=1
	sw $t0,-44($fp)
	lw $t0,-40($fp)
	lw $t1,-44($fp)
	beq $t0,$t1,Label_41
	li $v0,4 # print string
	la $a0,_str10
	syscall 
	j Label_42
Label_41:
Label_42:
	j Label_35
Label_40:
	lw $t0,-28($fp)
	li $t1,2
	bne $t0,$t1,Label_43
	lw $t0,-12($fp) # #reg84=iter
	sw $t0,-48($fp)
	li $t0,2 # #reg85=2
	sw $t0,-52($fp)
	lw $t0,-48($fp)
	lw $t1,-52($fp)
	beq $t0,$t1,Label_44
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
	lw $t0,-12($fp)
	li $t1,1
	addu $t2,$t0,$t1
	sw $t2,-56($fp)
	lw $t0,-56($fp) # iter=#reg86
	sw $t0,-12($fp)
	lw $t0,-16($fp)
	li $t1,1
	addu $t2,$t0,$t1
	sw $t2,-16($fp)
	lw $t0,-12($fp) # #reg87=iter
	sw $t0,-60($fp)
	lw $t0,-8($fp) # #reg88=intermax
	sw $t0,-64($fp)
	lw $t0,-60($fp)
	lw $t1,-64($fp)
	blt $t0,$t1,Label_34
exit_Func_15: # exit function condition
	addu $sp,$sp,60 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addu $sp,$sp,8
	jr $ra

Func_16: # function condition_test
	subu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addu $fp,$sp,4 # fp = new base
	subu $sp,$sp,0 # make space for locals and temps
	jal Func_12
	addu $v1,$0,$v0 # save the return value
	jal Func_14
	addu $v1,$0,$v0 # save the return value
	jal Func_15
	addu $v1,$0,$v0 # save the return value
	li $v0,4 # print string
	la $a0,_str12
	syscall 
exit_Func_16: # exit function condition_test
	addu $sp,$sp,0 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addu $sp,$sp,8
	jr $ra

Func_17: # function miscellaneous
	subu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addu $fp,$sp,4 # fp = new base
	subu $sp,$sp,1260 # make space for locals and temps
	li $t0,0 # #reg89=0
	sw $t0,-1148($fp)
	lw $t0,-1148($fp) # i=#reg89
	sw $t0,-108($fp)
	li $t0,1 # #reg90=1
	sw $t0,-1152($fp)
	lw $t0,-1152($fp) # _=#reg90
	sw $t0,-8($fp)
	li $t0,2 # #reg91=2
	sw $t0,-1156($fp)
	lw $t0,-1156($fp) # a=#reg91
	sw $t0,-12($fp)
	li $t0,0 # #reg92=0
	sw $t0,-1160($fp)
	lw $t0,-1160($fp) # toy=#reg92
	sw $t0,-116($fp)
Label_46:
	lw $t0,-108($fp) # #reg93=i
	sw $t0,-1164($fp)
	lw $t0,-108($fp) # #reg94=i
	sw $t0,-1168($fp)
	lw $t0,-1164($fp)
	sll $t0,$t0,2
	subu $t1,$fp,16
	subu $t1,$t1,$t0
	lw $t0,-1168($fp)
	sw $t0,0($t1)
	lw $t0,-108($fp)
	li $t1,1
	addu $t2,$t0,$t1
	sw $t2,-1172($fp)
	lw $t0,-1172($fp) # i=#reg95
	sw $t0,-108($fp)
	lw $t0,-116($fp)
	li $t1,1
	addu $t2,$t0,$t1
	sw $t2,-116($fp)
	lw $t0,-108($fp) # #reg96=i
	sw $t0,-1176($fp)
	li $t0,23 # #reg97=23
	sw $t0,-1180($fp)
	lw $t0,-1176($fp)
	lw $t1,-1180($fp)
	blt $t0,$t1,Label_46
	li $t0,'a' # #reg98='a'
	sw $t0,-1184($fp)
	lw $t0,-1184($fp) # ch1=#reg98
	sw $t0,-120($fp)
	li $t0,0 # #reg99=0
	sw $t0,-1188($fp)
	lw $t0,-1188($fp) # i=#reg99
	sw $t0,-108($fp)
	li $t0,0 # #reg100=0
	sw $t0,-1192($fp)
	lw $t0,-1192($fp) # toy=#reg100
	sw $t0,-116($fp)
Label_47:
	lw $t0,-108($fp) # #reg101=i
	sw $t0,-1196($fp)
	lw $t0,-120($fp) # #reg102=ch1
	sw $t0,-1200($fp)
	lw $t0,-1196($fp)
	sll $t0,$t0,2
	subu $t1,$fp,124
	subu $t1,$t1,$t0
	lw $t0,-1200($fp)
	sw $t0,0($t1)
	lw $t0,-120($fp)
	li $t1,1
	addu $t2,$t0,$t1
	sw $t2,-1204($fp)
	lw $t0,-1204($fp) # ch1=#reg103
	sw $t0,-120($fp)
	lw $t0,-108($fp)
	li $t1,1
	addu $t2,$t0,$t1
	sw $t2,-1208($fp)
	lw $t0,-1208($fp) # i=#reg104
	sw $t0,-108($fp)
	lw $t0,-116($fp)
	li $t1,1
	addu $t2,$t0,$t1
	sw $t2,-116($fp)
	lw $t0,-120($fp) # #reg105=ch1
	sw $t0,-1212($fp)
	li $t0,127 # #reg106=127
	sw $t0,-1216($fp)
	lw $t0,-1212($fp)
	lw $t1,-1216($fp)
	blt $t0,$t1,Label_47
	lw $t0,-108($fp) # #reg107=i
	sw $t0,-1220($fp)
	lw $t0,-1220($fp) # endid=#reg107
	sw $t0,-112($fp)
	li $t0,0 # #reg108=0
	sw $t0,-1224($fp)
	lw $t0,-1224($fp) # _=#reg108
	sw $t0,-8($fp)
	li $t0,0 # #reg109=0
	sw $t0,-1228($fp)
	lw $t0,-1228($fp) # toy=#reg109
	sw $t0,-116($fp)
Label_48:
	lw $t0,-8($fp)
	li $t1,1
	mul $t2,$t0,$t1
	sw $t2,-1232($fp)
	lw $t0,-1232($fp)
	lw $t1,-8($fp)
	subu $t2,$t0,$t1
	sw $t2,-1232($fp)
	lw $t0,-1232($fp)
	li $t1,1
	subu $t2,$t0,$t1
	sw $t2,-1232($fp)
	li $t0,2
	li $t1,1
	div $t2,$t0,$t1
	sw $t2,-1236($fp)
	lw $t0,-1232($fp)
	lw $t1,-1236($fp)
	addu $t2,$t0,$t1
	sw $t2,-1232($fp)
	lw $t0,-1232($fp)
	li $t1,1
	subu $t2,$t0,$t1
	sw $t2,-1232($fp)
	lw $t0,-1232($fp)
	li $t1,2
	div $t2,$t0,$t1
	sw $t2,-1232($fp)
	li $t0,'a' # #reg112='a'
	sw $t0,-1240($fp)
	lw $t0,-1232($fp)
	sll $t0,$t0,2
	subu $t1,$fp,124
	subu $t1,$t1,$t0
	lw $t0,-1240($fp)
	sw $t0,0($t1)
	lw $t0,-8($fp)
	li $t1,1
	addu $t2,$t0,$t1
	sw $t2,-1244($fp)
	lw $t0,-1244($fp) # _=#reg113
	sw $t0,-8($fp)
	lw $t0,-116($fp)
	li $t1,1
	addu $t2,$t0,$t1
	sw $t2,-116($fp)
	lw $t0,-8($fp) # #reg114=_
	sw $t0,-1248($fp)
	lw $t0,-112($fp) # #reg115=endid
	sw $t0,-1252($fp)
	lw $t0,-1248($fp)
	lw $t1,-1252($fp)
	blt $t0,$t1,Label_48
	li $t0,0 # #reg116=0
	sw $t0,-1256($fp)
	lw $t0,-1256($fp)
	sll $t0,$t0,2
	subu $t1,$fp,124
	subu $t1,$t1,$t0
	lw $t0,0($t1)
	sw $t0,-1260($fp)
	li $t0,'a' # #reg118='a'
	sw $t0,-1264($fp)
	lw $t0,-1260($fp)
	lw $t1,-1264($fp)
	bne $t0,$t1,Label_49
	li $v0,4 # print string
	la $a0,_str13
	syscall 
	j Label_50
Label_49:
Label_50:
exit_Func_17: # exit function miscellaneous
	addu $sp,$sp,1260 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addu $sp,$sp,8
	jr $ra

Func_18: # function miscellaneous_test
	subu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addu $fp,$sp,4 # fp = new base
	subu $sp,$sp,0 # make space for locals and temps
	jal Func_17
	addu $v1,$0,$v0 # save the return value
exit_Func_18: # exit function miscellaneous_test
	addu $sp,$sp,0 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addu $sp,$sp,8
	jr $ra

Func_19: # function fibonaci
	subu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addu $fp,$sp,8 # fp = new base
	subu $sp,$sp,32 # make space for locals and temps
	lw $t0,0($fp) # #reg119=n
	sw $t0,-12($fp)
Label_52:
	lw $t0,-12($fp)
	li $t1,0
	bne $t0,$t1,Label_53
	li $t0,0 # #reg120=0
	sw $t0,-16($fp)
	lw $v0,-16($fp)
	j exit_Func_19
	j Label_51
Label_53:
	lw $t0,-12($fp)
	li $t1,1
	bne $t0,$t1,Label_54
	li $t0,1 # #reg121=1
	sw $t0,-20($fp)
	lw $v0,-20($fp)
	j exit_Func_19
	j Label_51
Label_54:
	lw $t0,-12($fp)
	li $t1,2
	bne $t0,$t1,Label_55
	li $t0,1 # #reg122=1
	sw $t0,-24($fp)
	lw $v0,-24($fp)
	j exit_Func_19
	j Label_51
Label_55:
	j Label_51
Label_51:
	lw $t0,0($fp)
	li $t1,1
	subu $t2,$t0,$t1
	sw $t2,-28($fp)
	lw $t0,-28($fp) # push #reg123
	subu $sp,$sp,4
	sw $t0,0($sp)
	jal Func_19
	addu $v1,$0,$v0 # save the return value
	sw $v1,-32($fp) # #reg124=@RET
	lw $t0,0($fp)
	li $t1,2
	subu $t2,$t0,$t1
	sw $t2,-36($fp)
	lw $t0,-36($fp) # push #reg125
	subu $sp,$sp,4
	sw $t0,0($sp)
	jal Func_19
	addu $v1,$0,$v0 # save the return value
	sw $v1,-40($fp) # #reg126=@RET
	lw $t0,-32($fp)
	lw $t1,-40($fp)
	addu $t2,$t0,$t1
	sw $t2,-32($fp)
	lw $v0,-32($fp)
	j exit_Func_19
exit_Func_19: # exit function fibonaci
	addu $sp,$sp,32 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addu $sp,$sp,12
	jr $ra

Func_20: # function fibonaci_test
	subu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addu $fp,$sp,4 # fp = new base
	subu $sp,$sp,44 # make space for locals and temps
	li $v0,4 # print string
	la $a0,_str14
	syscall 
	li $v0,5# read int
	syscall 
	sw $v0,-8($fp) # scanfChr n
	li $t0,0 # #reg127=0
	sw $t0,-16($fp)
	lw $t0,-16($fp) # toy=#reg127
	sw $t0,-12($fp)
Label_56:
	lw $t0,-8($fp) # #reg128=n
	sw $t0,-20($fp)
	li $t0,0 # #reg129=0
	sw $t0,-24($fp)
	lw $t0,-20($fp)
	lw $t1,-24($fp)
	bge $t0,$t1,Label_57
	li $v0,4 # print string
	la $a0,_str15
	syscall 
	li $v0,5# read int
	syscall 
	sw $v0,-8($fp) # scanfChr n
	j Label_58
Label_57:
Label_58:
	lw $t0,-8($fp) # #reg130=n
	sw $t0,-28($fp)
	li $t0,0 # #reg131=0
	sw $t0,-32($fp)
	lw $t0,-28($fp)
	lw $t1,-32($fp)
	blt $t0,$t1,Label_59
	lw $t0,-8($fp) # #reg132=n
	sw $t0,-36($fp)
	lw $t0,-36($fp) # push #reg132
	subu $sp,$sp,4
	sw $t0,0($sp)
	jal Func_19
	addu $v1,$0,$v0 # save the return value
	sw $v1,-40($fp) # #reg133=@RET
	li $v0,4 # print string
	la $a0,_str16
	syscall 
	li $v0,1# print int #reg133
	lw $a0,-40($fp)
	syscall 
	j Label_60
Label_59:
Label_60:
	lw $t0,-12($fp)
	li $t1,1
	addu $t2,$t0,$t1
	sw $t2,-12($fp)
	lw $t0,-8($fp) # #reg134=n
	sw $t0,-44($fp)
	li $t0,0 # #reg135=0
	sw $t0,-48($fp)
	lw $t0,-44($fp)
	lw $t1,-48($fp)
	blt $t0,$t1,Label_56
exit_Func_20: # exit function fibonaci_test
	addu $sp,$sp,44 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addu $sp,$sp,8
	jr $ra

Func_21: # function mod
	subu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addu $fp,$sp,12 # fp = new base
	subu $sp,$sp,4 # make space for locals and temps
	lw $t0,0($fp)
	lw $t1,-4($fp)
	div $t2,$t0,$t1
	sw $t2,-16($fp)
	lw $t0,-16($fp)
	lw $t1,-4($fp)
	mul $t2,$t0,$t1
	sw $t2,-16($fp)
	lw $t0,0($fp)
	lw $t1,-16($fp)
	subu $t2,$t0,$t1
	sw $t2,-16($fp)
	lw $v0,-16($fp)
	j exit_Func_21
exit_Func_21: # exit function mod
	addu $sp,$sp,4 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addu $sp,$sp,16
	jr $ra

Func_22: # function gcd
	subu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addu $fp,$sp,12 # fp = new base
	subu $sp,$sp,52 # make space for locals and temps
	li $t0,0 # #reg137=0
	sw $t0,-20($fp)
	lw $t0,-20($fp) # rem=#reg137
	sw $t0,-16($fp)
	lw $t0,0($fp) # #reg138=n
	sw $t0,-24($fp)
	li $t0,0 # #reg139=0
	sw $t0,-28($fp)
	lw $t0,-24($fp)
	lw $t1,-28($fp)
	bne $t0,$t1,Label_61
	lw $t0,-4($fp) # #reg140=m
	sw $t0,-32($fp)
	lw $v0,-32($fp)
	j exit_Func_22
	j Label_62
Label_61:
Label_62:
	lw $t0,-4($fp) # #reg141=m
	sw $t0,-36($fp)
	li $t0,0 # #reg142=0
	sw $t0,-40($fp)
	lw $t0,-36($fp)
	lw $t1,-40($fp)
	bne $t0,$t1,Label_63
	lw $t0,0($fp) # #reg143=n
	sw $t0,-44($fp)
	lw $v0,-44($fp)
	j exit_Func_22
	j Label_64
Label_63:
Label_64:
	lw $t0,-4($fp) # #reg144=m
	sw $t0,-48($fp)
	lw $t0,-48($fp) # push #reg144
	subu $sp,$sp,4
	sw $t0,0($sp)
	lw $t0,0($fp) # #reg145=n
	sw $t0,-52($fp)
	lw $t0,-52($fp) # push #reg145
	subu $sp,$sp,4
	sw $t0,0($sp)
	lw $t0,-4($fp) # #reg146=m
	sw $t0,-56($fp)
	lw $t0,-56($fp) # push #reg146
	subu $sp,$sp,4
	sw $t0,0($sp)
	jal Func_21
	addu $v1,$0,$v0 # save the return value
	sw $v1,-60($fp) # #reg147=@RET
	lw $t0,-60($fp) # push #reg147
	subu $sp,$sp,4
	sw $t0,0($sp)
	jal Func_22
	addu $v1,$0,$v0 # save the return value
	sw $v1,-64($fp) # #reg148=@RET
	lw $v0,-64($fp)
	j exit_Func_22
exit_Func_22: # exit function gcd
	addu $sp,$sp,52 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addu $sp,$sp,16
	jr $ra

Func_23: # function isWrongGcd
	subu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addu $fp,$sp,12 # fp = new base
	subu $sp,$sp,28 # make space for locals and temps
	lw $t0,0($fp) # #reg149=n
	sw $t0,-16($fp)
	li $t0,0 # #reg150=0
	sw $t0,-20($fp)
	lw $t0,-16($fp)
	lw $t1,-20($fp)
	bgt $t0,$t1,Label_65
	li $t0,1 # #reg151=1
	sw $t0,-24($fp)
	lw $v0,-24($fp)
	j exit_Func_23
	j Label_66
Label_65:
Label_66:
	lw $t0,-4($fp) # #reg152=m
	sw $t0,-28($fp)
	li $t0,0 # #reg153=0
	sw $t0,-32($fp)
	lw $t0,-28($fp)
	lw $t1,-32($fp)
	bgt $t0,$t1,Label_67
	li $t0,1 # #reg154=1
	sw $t0,-36($fp)
	lw $v0,-36($fp)
	j exit_Func_23
	j Label_68
Label_67:
Label_68:
	li $t0,0 # #reg155=0
	sw $t0,-40($fp)
	lw $v0,-40($fp)
	j exit_Func_23
exit_Func_23: # exit function isWrongGcd
	addu $sp,$sp,28 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addu $sp,$sp,16
	jr $ra

Func_24: # function gcd_test
	subu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addu $fp,$sp,4 # fp = new base
	subu $sp,$sp,52 # make space for locals and temps
	li $v0,4 # print string
	la $a0,_str17
	syscall 
	li $v0,5# read int
	syscall 
	sw $v0,-8($fp) # scanfChr n
	li $v0,5# read int
	syscall 
	sw $v0,-12($fp) # scanfChr m
	lw $t0,-8($fp) # #reg156=n
	sw $t0,-20($fp)
	lw $t0,-20($fp) # push #reg156
	subu $sp,$sp,4
	sw $t0,0($sp)
	lw $t0,-12($fp) # #reg157=m
	sw $t0,-24($fp)
	lw $t0,-24($fp) # push #reg157
	subu $sp,$sp,4
	sw $t0,0($sp)
	jal Func_23
	addu $v1,$0,$v0 # save the return value
	sw $v1,-28($fp) # #reg158=@RET
	lw $t0,-28($fp)
	li $t1,0
	beq $t0,$t1,Label_69
	li $t0,0 # #reg159=0
	sw $t0,-32($fp)
	lw $t0,-32($fp) # toy=#reg159
	sw $t0,-16($fp)
Label_70:
	li $v0,4 # print string
	la $a0,_str18
	syscall 
	li $v0,5# read int
	syscall 
	sw $v0,-8($fp) # scanfChr n
	li $v0,5# read int
	syscall 
	sw $v0,-12($fp) # scanfChr m
	lw $t0,-16($fp)
	li $t1,1
	addu $t2,$t0,$t1
	sw $t2,-16($fp)
	lw $t0,-8($fp) # #reg160=n
	sw $t0,-36($fp)
	lw $t0,-36($fp) # push #reg160
	subu $sp,$sp,4
	sw $t0,0($sp)
	lw $t0,-12($fp) # #reg161=m
	sw $t0,-40($fp)
	lw $t0,-40($fp) # push #reg161
	subu $sp,$sp,4
	sw $t0,0($sp)
	jal Func_23
	addu $v1,$0,$v0 # save the return value
	sw $v1,-44($fp) # #reg162=@RET
	lw $t0,-44($fp)
	li $t1,0
	bne $t0,$t1,Label_70
	j Label_71
Label_69:
Label_71:
	lw $t0,-8($fp) # #reg163=n
	sw $t0,-48($fp)
	lw $t0,-48($fp) # push #reg163
	subu $sp,$sp,4
	sw $t0,0($sp)
	lw $t0,-12($fp) # #reg164=m
	sw $t0,-52($fp)
	lw $t0,-52($fp) # push #reg164
	subu $sp,$sp,4
	sw $t0,0($sp)
	jal Func_22
	addu $v1,$0,$v0 # save the return value
	sw $v1,-56($fp) # #reg165=@RET
	li $v0,4 # print string
	la $a0,_str19
	syscall 
	li $v0,1# print int #reg165
	lw $a0,-56($fp)
	syscall 
exit_Func_24: # exit function gcd_test
	addu $sp,$sp,52 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addu $sp,$sp,8
	jr $ra

Func_25: # function is_prime
	subu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addu $fp,$sp,8 # fp = new base
	subu $sp,$sp,64 # make space for locals and temps
	li $t0,2 # #reg166=2
	sw $t0,-20($fp)
	lw $t0,-20($fp) # i=#reg166
	sw $t0,-12($fp)
	lw $t0,0($fp) # #reg167=x
	sw $t0,-24($fp)
	li $t0,2 # #reg168=2
	sw $t0,-28($fp)
	lw $t0,-24($fp)
	lw $t1,-28($fp)
	bne $t0,$t1,Label_72
	li $t0,1 # #reg169=1
	sw $t0,-32($fp)
	lw $v0,-32($fp)
	j exit_Func_25
	j Label_73
Label_72:
Label_73:
	li $t0,0 # #reg170=0
	sw $t0,-36($fp)
	lw $t0,-36($fp) # toy=#reg170
	sw $t0,-16($fp)
Label_74:
	lw $t0,0($fp) # #reg171=x
	sw $t0,-40($fp)
	lw $t0,-40($fp) # push #reg171
	subu $sp,$sp,4
	sw $t0,0($sp)
	lw $t0,-12($fp) # #reg172=i
	sw $t0,-44($fp)
	lw $t0,-44($fp) # push #reg172
	subu $sp,$sp,4
	sw $t0,0($sp)
	jal Func_21
	addu $v1,$0,$v0 # save the return value
	sw $v1,-48($fp) # #reg173=@RET
	li $t0,0 # #reg174=0
	sw $t0,-52($fp)
	lw $t0,-48($fp)
	lw $t1,-52($fp)
	bne $t0,$t1,Label_75
	li $t0,0 # #reg175=0
	sw $t0,-56($fp)
	lw $v0,-56($fp)
	j exit_Func_25
	j Label_76
Label_75:
Label_76:
	lw $t0,-12($fp)
	li $t1,1
	addu $t2,$t0,$t1
	sw $t2,-60($fp)
	lw $t0,-60($fp) # i=#reg176
	sw $t0,-12($fp)
	lw $t0,-16($fp)
	li $t1,1
	addu $t2,$t0,$t1
	sw $t2,-16($fp)
	lw $t0,-12($fp)
	lw $t1,-12($fp)
	mul $t2,$t0,$t1
	sw $t2,-64($fp)
	lw $t0,0($fp) # #reg178=x
	sw $t0,-68($fp)
	lw $t0,-64($fp)
	lw $t1,-68($fp)
	blt $t0,$t1,Label_74
	li $t0,1 # #reg179=1
	sw $t0,-72($fp)
	lw $v0,-72($fp)
	j exit_Func_25
exit_Func_25: # exit function is_prime
	addu $sp,$sp,64 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addu $sp,$sp,12
	jr $ra

Func_26: # function prime_factorization
	subu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addu $fp,$sp,8 # fp = new base
	subu $sp,$sp,136 # make space for locals and temps
	li $t0,2 # #reg180=2
	sw $t0,-28($fp)
	lw $t0,-28($fp) # i=#reg180
	sw $t0,-12($fp)
	li $t0,0 # #reg181=0
	sw $t0,-32($fp)
	lw $t0,-32($fp) # first=#reg181
	sw $t0,-16($fp)
	lw $t0,0($fp) # #reg182=n
	sw $t0,-36($fp)
	li $t0,1 # #reg183=1
	sw $t0,-40($fp)
	lw $t0,-36($fp)
	lw $t1,-40($fp)
	bne $t0,$t1,Label_77
	li $v0,4 # print string
	la $a0,_str20
	syscall 
	j exit_Func_26
	j Label_78
Label_77:
Label_78:
	li $t0,0 # #reg184=0
	sw $t0,-44($fp)
	lw $t0,-44($fp) # fuck=#reg184
	sw $t0,-24($fp)
Label_79:
	lw $t0,-12($fp) # #reg185=i
	sw $t0,-48($fp)
	lw $t0,-48($fp) # push #reg185
	subu $sp,$sp,4
	sw $t0,0($sp)
	jal Func_25
	addu $v1,$0,$v0 # save the return value
	sw $v1,-52($fp) # #reg186=@RET
	li $t0,1 # #reg187=1
	sw $t0,-56($fp)
	lw $t0,-52($fp)
	lw $t1,-56($fp)
	bne $t0,$t1,Label_80
	lw $t0,0($fp) # #reg188=n
	sw $t0,-60($fp)
	lw $t0,-60($fp) # push #reg188
	subu $sp,$sp,4
	sw $t0,0($sp)
	lw $t0,-12($fp) # #reg189=i
	sw $t0,-64($fp)
	lw $t0,-64($fp) # push #reg189
	subu $sp,$sp,4
	sw $t0,0($sp)
	jal Func_21
	addu $v1,$0,$v0 # save the return value
	sw $v1,-68($fp) # #reg190=@RET
	li $t0,0 # #reg191=0
	sw $t0,-72($fp)
	lw $t0,-68($fp)
	lw $t1,-72($fp)
	bne $t0,$t1,Label_81
	li $t0,0 # #reg192=0
	sw $t0,-76($fp)
	lw $t0,-76($fp) # toy=#reg192
	sw $t0,-20($fp)
Label_82:
	lw $t0,0($fp)
	lw $t1,-12($fp)
	div $t2,$t0,$t1
	sw $t2,-80($fp)
	lw $t0,-80($fp) # n=#reg193
	sw $t0,0($fp)
	lw $t0,-16($fp) # #reg194=first
	sw $t0,-84($fp)
	li $t0,1 # #reg195=1
	sw $t0,-88($fp)
	lw $t0,-84($fp)
	lw $t1,-88($fp)
	bne $t0,$t1,Label_83
	li $v0,4 # print string
	la $a0,_str21
	syscall 
	j Label_84
Label_83:
Label_84:
	li $t0,1 # #reg196=1
	sw $t0,-92($fp)
	lw $t0,-92($fp) # first=#reg196
	sw $t0,-16($fp)
	lw $t0,-12($fp) # #reg197=i
	sw $t0,-96($fp)
	li $v0,1# print int #reg197
	lw $a0,-96($fp)
	syscall 
	lw $t0,-20($fp)
	li $t1,1
	addu $t2,$t0,$t1
	sw $t2,-20($fp)
	lw $t0,0($fp) # #reg198=n
	sw $t0,-100($fp)
	lw $t0,-100($fp) # push #reg198
	subu $sp,$sp,4
	sw $t0,0($sp)
	lw $t0,-12($fp) # #reg199=i
	sw $t0,-104($fp)
	lw $t0,-104($fp) # push #reg199
	subu $sp,$sp,4
	sw $t0,0($sp)
	jal Func_21
	addu $v1,$0,$v0 # save the return value
	sw $v1,-108($fp) # #reg200=@RET
	li $t0,0 # #reg201=0
	sw $t0,-112($fp)
	lw $t0,-108($fp)
	lw $t1,-112($fp)
	beq $t0,$t1,Label_82
	j Label_85
Label_81:
Label_85:
	j Label_86
Label_80:
Label_86:
	lw $t0,-12($fp)
	li $t1,1
	addu $t2,$t0,$t1
	sw $t2,-116($fp)
	lw $t0,-116($fp) # i=#reg202
	sw $t0,-12($fp)
	lw $t0,-24($fp)
	li $t1,1
	addu $t2,$t0,$t1
	sw $t2,-24($fp)
	lw $t0,-12($fp)
	lw $t1,-12($fp)
	mul $t2,$t0,$t1
	sw $t2,-120($fp)
	lw $t0,0($fp) # #reg204=n
	sw $t0,-124($fp)
	lw $t0,-120($fp)
	lw $t1,-124($fp)
	blt $t0,$t1,Label_79
	lw $t0,0($fp) # #reg205=n
	sw $t0,-128($fp)
	li $t0,1 # #reg206=1
	sw $t0,-132($fp)
	lw $t0,-128($fp)
	lw $t1,-132($fp)
	beq $t0,$t1,Label_87
	lw $t0,-16($fp) # #reg207=first
	sw $t0,-136($fp)
	li $t0,1 # #reg208=1
	sw $t0,-140($fp)
	lw $t0,-136($fp)
	lw $t1,-140($fp)
	bne $t0,$t1,Label_88
	li $v0,4 # print string
	la $a0,_str22
	syscall 
	j Label_89
Label_88:
Label_89:
	lw $t0,0($fp) # #reg209=n
	sw $t0,-144($fp)
	li $v0,1# print int #reg209
	lw $a0,-144($fp)
	syscall 
	j Label_90
Label_87:
Label_90:
exit_Func_26: # exit function prime_factorization
	addu $sp,$sp,136 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addu $sp,$sp,12
	jr $ra

Func_27: # function prime_factorization_test
	subu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addu $fp,$sp,4 # fp = new base
	subu $sp,$sp,32 # make space for locals and temps
	li $v0,4 # print string
	la $a0,_str23
	syscall 
	li $v0,5# read int
	syscall 
	sw $v0,-8($fp) # scanfChr n
	li $t0,0 # #reg210=0
	sw $t0,-16($fp)
	lw $t0,-16($fp) # toy=#reg210
	sw $t0,-12($fp)
Label_91:
	lw $t0,-8($fp) # #reg211=n
	sw $t0,-20($fp)
	li $t0,0 # #reg212=0
	sw $t0,-24($fp)
	lw $t0,-20($fp)
	lw $t1,-24($fp)
	bge $t0,$t1,Label_92
	li $v0,4 # print string
	la $a0,_str24
	syscall 
	li $v0,5# read int
	syscall 
	sw $v0,-8($fp) # scanfChr n
	j Label_93
Label_92:
Label_93:
	lw $t0,-12($fp)
	li $t1,1
	addu $t2,$t0,$t1
	sw $t2,-12($fp)
	lw $t0,-8($fp) # #reg213=n
	sw $t0,-28($fp)
	li $t0,0 # #reg214=0
	sw $t0,-32($fp)
	lw $t0,-28($fp)
	lw $t1,-32($fp)
	blt $t0,$t1,Label_91
	li $v0,4 # print string
	la $a0,_str25
	syscall 
	lw $t0,-8($fp) # #reg215=n
	sw $t0,-36($fp)
	lw $t0,-36($fp) # push #reg215
	subu $sp,$sp,4
	sw $t0,0($sp)
	jal Func_26
	addu $v1,$0,$v0 # save the return value
exit_Func_27: # exit function prime_factorization_test
	addu $sp,$sp,32 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addu $sp,$sp,8
	jr $ra

Func_28: # function iswrong
	subu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addu $fp,$sp,8 # fp = new base
	subu $sp,$sp,40 # make space for locals and temps
	li $t0,'E' # #reg216='E'
	sw $t0,-12($fp)
	lw $t0,0($fp) # #reg217=A
	sw $t0,-16($fp)
	lw $t0,-12($fp)
	lw $t1,-16($fp)
	bne $t0,$t1,Label_94
	li $t0,0 # #reg218=0
	sw $t0,-20($fp)
	lw $v0,-20($fp)
	j exit_Func_28
	j Label_95
Label_94:
Label_95:
	li $t0,'M' # #reg219='M'
	sw $t0,-24($fp)
	lw $t0,0($fp) # #reg220=A
	sw $t0,-28($fp)
	lw $t0,-24($fp)
	lw $t1,-28($fp)
	bne $t0,$t1,Label_96
	li $t0,0 # #reg221=0
	sw $t0,-32($fp)
	lw $v0,-32($fp)
	j exit_Func_28
	j Label_97
Label_96:
Label_97:
	li $t0,'H' # #reg222='H'
	sw $t0,-36($fp)
	lw $t0,0($fp) # #reg223=A
	sw $t0,-40($fp)
	lw $t0,-36($fp)
	lw $t1,-40($fp)
	bne $t0,$t1,Label_98
	li $t0,0 # #reg224=0
	sw $t0,-44($fp)
	lw $v0,-44($fp)
	j exit_Func_28
	j Label_99
Label_98:
Label_99:
	li $t0,1 # #reg225=1
	sw $t0,-48($fp)
	lw $v0,-48($fp)
	j exit_Func_28
exit_Func_28: # exit function iswrong
	addu $sp,$sp,40 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addu $sp,$sp,12
	jr $ra

Func_29: # function main
	subu $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addu $fp,$sp,4 # fp = new base
	subu $sp,$sp,32 # make space for locals and temps
	li $v0,4 # print string
	la $a0,_str26
	syscall 
	li $v0,12# read char
	syscall 
	sw $v0,-8($fp) # scanfChr kind
	lw $t0,-8($fp) # #reg226=kind
	sw $t0,-16($fp)
	lw $t0,-16($fp) # push #reg226
	subu $sp,$sp,4
	sw $t0,0($sp)
	jal Func_28
	addu $v1,$0,$v0 # save the return value
	sw $v1,-20($fp) # #reg227=@RET
	lw $t0,-20($fp)
	li $t1,0
	beq $t0,$t1,Label_100
	li $t0,0 # #reg228=0
	sw $t0,-24($fp)
	lw $t0,-24($fp) # toy=#reg228
	sw $t0,-12($fp)
Label_101:
	li $v0,4 # print string
	la $a0,_str27
	syscall 
	li $v0,12# read char
	syscall 
	sw $v0,-8($fp) # scanfChr kind
	lw $t0,-12($fp)
	li $t1,1
	addu $t2,$t0,$t1
	sw $t2,-12($fp)
	lw $t0,-8($fp) # #reg229=kind
	sw $t0,-28($fp)
	lw $t0,-28($fp) # push #reg229
	subu $sp,$sp,4
	sw $t0,0($sp)
	jal Func_28
	addu $v1,$0,$v0 # save the return value
	sw $v1,-32($fp) # #reg230=@RET
	lw $t0,-32($fp)
	li $t1,0
	bne $t0,$t1,Label_101
	j Label_102
Label_100:
Label_102:
	lw $t0,-8($fp) # #reg231=kind
	sw $t0,-36($fp)
Label_104:
	lw $t0,-36($fp)
	li $t1,'E'
	bne $t0,$t1,Label_105
	jal Func_5
	addu $v1,$0,$v0 # save the return value
	jal Func_7
	addu $v1,$0,$v0 # save the return value
	jal Func_20
	addu $v1,$0,$v0 # save the return value
	j Label_103
Label_105:
	lw $t0,-36($fp)
	li $t1,'M'
	bne $t0,$t1,Label_106
	jal Func_11
	addu $v1,$0,$v0 # save the return value
	jal Func_16
	addu $v1,$0,$v0 # save the return value
	jal Func_9
	addu $v1,$0,$v0 # save the return value
	jal Func_24
	addu $v1,$0,$v0 # save the return value
	j Label_103
Label_106:
	lw $t0,-36($fp)
	li $t1,'H'
	bne $t0,$t1,Label_107
	jal Func_5
	addu $v1,$0,$v0 # save the return value
	jal Func_7
	addu $v1,$0,$v0 # save the return value
	jal Func_11
	addu $v1,$0,$v0 # save the return value
	jal Func_16
	addu $v1,$0,$v0 # save the return value
	jal Func_9
	addu $v1,$0,$v0 # save the return value
	jal Func_18
	addu $v1,$0,$v0 # save the return value
	jal Func_27
	addu $v1,$0,$v0 # save the return value
	j Label_103
Label_107:
	li $v0,4 # print string
	la $a0,_str28
	syscall 
	j Label_103
Label_103:
	j exit_Func_29
exit_Func_29: # exit function main
	addu $sp,$sp,32 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addu $sp,$sp,8
	addu $sp,$sp,20512
	li $gp,0x10008000 # restore $gp
