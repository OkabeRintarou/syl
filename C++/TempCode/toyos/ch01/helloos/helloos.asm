;
org 0x7c00  ;告诉编译器将程序加载到内存0x7c00处

mov ax,cs
mov ds,ax
mov es,ax
call Show_Hello_Os
jmp $			;无限循环

Show_Hello_Os:
	mov ax,Hello_Os_Message
	mov bp,ax    					;ES:BP  = 字符串地址
	mov cx,8						;   CS  = 字符串长度
	mov ax,0x1301					;   AH  = 13, AL = 01
	mov bx,0x000E                   ;   BH  = 页码,BL = 黄色
	mov dx,0x0800					;   DH  = 行 ， DL = 列
	int 0x10                        ;   BIOS 10h中断进行字符串写入

	mov ax,Hello_Os_Author
	mov bp,ax
	mov cx,17
	mov ax,0x1301
	mov bx,0x000C					;黑底红字
	mov dx,0x0900
	int 0x10

	;10h中断
	;功能                 功能代码            参数           		返回
	;写字符串			  AH = 13H		AL = 写模式
	;							 		BH = 页码,BL = 颜色
	;									CH = 字符串长度
	;									DH = 行，DL = 列
	;									ES:BP = 字符串偏移量		

Hello_Os_Message:	db "hello os"
Hello_Os_Author:    db "author : sylcoder"

times 510 - ( $ - $$) db 0 
dw 0xaa55

