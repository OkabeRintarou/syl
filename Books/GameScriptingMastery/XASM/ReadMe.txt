; XVM指令集
; 1.内存
;	Mov Destination,Source
; 2. 算数运算
;	Add Destination,Source
;	Sub Destination,Source
;	Mul Destination,Source
;	Div Destination,Source
;	Mod Destination,Source
;	Exp Destination,Power
;	Neg Destination
;	Inc Destination
;	Dec Destination
; 3.位运算符
;	And Destination,Source
;	Or  Destination,Source
;	Xor Destination,Source
;	Not Destination
;	ShL Destination,ShiftCount
;	ShR Destination,ShiftCount
; 4.字符串操作
;	Concat String0,String1
;	GetChar Destination,Source,Index
;	SetChar Index,Destination,Source
; 5.条件分支
;	Jmp Label
;	JE	Op0,Op1,Label1
;	JNE Op0,Op1,Label1
;	JG  Op0,Op1,Label1
;	JL	Op0,Op1,Label1
;	JGE Op0,Op1,Label1
;	JLE Op0,Op1,Label1
; 6.堆栈接口
;	Push Source
;	Pop	 Destination	
; 7.函数接口
;	Call FunctionName
;	Ret
;	CallHost FunctionName
; 8.其他
;	Pause Duration
;	Exit  Code
;