### CCFinal
---
> CCFinal(C0 Compiler final version) is the final version of BUAA Compiler Design project which is able to compile extended C0 grammar source code to Mips assembly language.

#### Grammar
```html
<plusMinus>     ::= +|-
<multDiv>       ::= *|/
<relOp>         ::= < | <= | > |>= | != | == 
<letter>        ::= _|a|...|z|A|...|Z
<num>           ::= 0|1|...|9
<notZeroNum>    ::= 1|...|9
<char>          ::= '<plusMinus>'|'<multDiv>'|'<letter>'|'<num>'
<string>        ::= "{十进制编码为32,33,35-126的ASCII字符}"

<program>       ::= [<constDec>][<varDec>]{<retFuncDef>|<noRetFuncDef>}<mainFunc>
<constDec>      ::= const<constDef>;{ const<constDef>;}
<constDef>      ::=  int<ident>＝<integer>
				{,<ident>＝<integer>
				|char<ident>＝<char>
				{,<ident>＝<char>
<unsignedNum>   ::= <notZeroNum>{<num>}
<integer>       ::= [+|-]<unsignedNum>|0
<ident>         ::= <letter>{<letter>|<num>}
<decHead>       ::= int<ident> |char<ident>
<varDec>        ::= <varDef>;{<varDef>;}
<varDef>        ::= <type>(<ident>|<ident>‘[’<unsignedNum>‘]’){,(<ident>|<ident>‘[’<unsignedNum>‘]’) }
<constant>      ::= <integer>|<char>
<type>          ::= int | char
<retFuncDef>    ::= <decHead>‘(’<paramTabel>‘)’ ‘{’<compStmt>‘}’
<noRetFuncDef>  ::= void<ident>‘(’<paramTabel>‘)’‘{’<compStmt>‘}’
<compStmt>      ::= [<constDec>][<varDec>]<stmtList>
<paramTabel>    ::= <type><ident>{,<type><ident>}|<NULL>
<mainFunc>      ::= void main‘(’‘)’ ‘{’<compStmt>‘}’

<expr>          ::= [+|-]<term>{<plusMinus><term>}
<term>          ::= <factor>{<multDiv><factor>}
<factor>        ::= <ident>|<ident>‘[’<expr>‘]’|<integer>|<char>|<retFuncCall>|‘(’<expr>‘)’         

<stmt>          ::= <ifStmt>|<forStmt>| ‘{’<stmtList>‘}’|<retFuncCall>; 
				| <noRetFuncCall>;|<assignStmt>;|<readStmt>;|<writeStmt>;|<NULL>;|<switchStmt>|<returnStmt>;
<ifStmt>        ::= if ‘(’<condition>‘)’<stmt>[else<stmt>]
<forStmt>       ::= for‘(’<ident>＝<expr>;<condition>;<ident>＝<ident>(+|-)<unsignedNum>‘)’<stmt>
<condition>     ::= <expr><relOp><expr>|<expr> <stmtList>      ::= {<stmt>}
<retFuncCall>   ::= <ident>‘(’<valueParam>‘)’
<noRetFuncCall> ::= <ident>‘(’<valueParam>‘)’
<valueParam>    ::= <expr>{,<expr>}|<NULL>
<assignStmt>    ::= <ident>＝<expr>|<ident>‘[’<expr>‘]’ = <expr>
<readStmt>      ::= scanf ‘(’<ident>{,<ident>}‘)’
<writeStmt>     ::= printf ‘(’ <string>,<expr> ‘)’
					| printf ‘(’<string> ‘)’
					| printf ‘(’<expr>‘)’
<switchStmt>    ::= switch ‘(’<expr>‘)’ ‘{’<caseTable><default>‘}’
<caseTable>     ::= <caseStmt>{<caseStmt>}
<caseStmt>      ::= case<constant>：<stmt>
<default>       ::= default : <stmt>
<returnStmt>    ::=  return[‘(’<expr>‘)’]  

附加说明：
（1）char类型的<expr>，用字符的ASCII码对应的整数参加运算，在写语句中输出字符
（2）<ident>区分大小写字母
（3）<writeStmt>的字符串原样输出
（4）<switchStmt>中，switch后面的表达式和case后面的常量只允许出现int和char类型；每个情况子语句执行完毕后，不继续执行后面的情况子语句
（5）数组的下标从0开始
（6）for语句先执行一次循环体中的语句再进行循环变量是否越界的测试
```

#### Example
> Source Code
```c
int fib(int n){
	if (n==0){
		return (0);
	} else if (n==1){
		return (1);
	} else {
		return (fib(n-1)+fib(n-2));
	}
}
void main() {
	const int n = 10;
	int fib_result;
	printf("Hello world!");
	fib_result = fib(n);
	printf("result of fib(10) is : ",fib_result);
}
```
> Mips assembly
```mips
############################## Global String ##############################
.data
_str0 :  .asciiz "Hello world!"
_str1 :  .asciiz "result of fib(10) is : "
##############################  Text Segment ##############################
.text
# initialize global constant
	addu $gp,$sp,$0 # let $gp=$sp to get more space
	subi $gp,$gp,0 # make space for global variabel and constant
# initialize global constant
	addu $sp,$gp,$0 # update $sp
	j Func_1 # jump to main function
Func_0: # function fib
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addi $fp,$sp,8 # $fp = old $sp
	subi $sp,$sp,12 # make space for locals and temps
	lw $t0,0($fp) # load $t0 = n
	li $t1,0
	bne $t0,$t1,Label_0 # n!=0
	li $v1,0 # @RET = 0
	j exit_Func_0
	j Label_1
Label_0:
	lw $t0,0($fp) # load $t0 = n
	li $t1,1
	bne $t0,$t1,Label_2 # n!=1
	li $v1,1 # @RET = 1
	j exit_Func_0
	j Label_3
Label_2:
	lw $t0,0($fp) # load $t0 = n
	li $t1,1
	subu $t2,$t0,$t1 # #reg6 = n-1
	subi $sp,$sp,4
	sw $t2,0($sp)
	jal Func_0
	lw $t0,0($fp) # load $t0 = n
	li $t1,2
	subu $t2,$t0,$t1 # #reg8 = n-2
	addu $s0,$0,$v1 # $s0=@RET
	subi $sp,$sp,4
	sw $t2,0($sp)
	sw $s0,-20($fp) # save #reg7(global reg)
	jal Func_0
	lw $s0,-20($fp) # save #reg7(global reg)
	addu $t0,$0,$v1
	addu $s0,$s0,$t0 # #reg7 = #reg7+@RET
	addu $v1,$0,$s0 # @RET = #reg7
	j exit_Func_0
Label_3:
Label_1:
exit_Func_0: # exit function fib
	addi $sp,$sp,12 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addi $sp,$sp,12
	jr $ra
Func_1: # function main
	subi $sp,$sp,8 # make space for $fp and $ra
	sw $fp,4($sp) # save $fp
	sw $ra,0($sp) # save $ra
	addi $fp,$sp,4 # $fp = old $sp
	subi $sp,$sp,8 # make space for locals and temps
	li $t0,10 # const n=10
	sw $t0,-8($fp)
	li $v0,4 # print string
	la $a0,_str0
	syscall 
	lw $t0,-8($fp) # load $t0 = n
	subi $sp,$sp,4
	sw $t0,0($sp)
	jal Func_0
	li $v0,4 # print string
	la $a0,_str1
	syscall 
	li $v0,1# print int : @RET
	addu $a0,$0,$v1
	syscall 
exit_Func_1: # exit function main
	addi $sp,$sp,8 # pop locals and temps
	lw $fp,4($sp)
	lw $ra,0($sp)
	addi $sp,$sp,8
	addi $sp,$sp,0
	li $gp,0x10008000 # restore $gp

```