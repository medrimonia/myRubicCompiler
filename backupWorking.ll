define i32 @main(){
	%test = alloca i32, align 4
  %tmp1 = add i32 3, 0
  %tmp2 = add i32 5, 0
  %tmp3 = add i32 %tmp1, %tmp2
	store i32 %tmp3, i32 * %test
  %tmp5 = add i32 3, 0
  %tmp6 = add i32 5, 0
  %tmp7 = add i32 %tmp5, %tmp6
	store i32 %tmp7, i32 * %test
	
	ret i32 0
}