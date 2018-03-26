define <25 x i5> @add25i5(<25 x i5> %a, <25 x i5> %b)  {
entry:
  %0 = add <25 x i5> %a, %b
  ret <25 x i5> %0
}