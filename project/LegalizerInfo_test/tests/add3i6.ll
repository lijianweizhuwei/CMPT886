define <3 x i6> @add25i5(<3 x i6> %a, <3 x i6> %b)  {
entry:
  %0 = add <3 x i6> %a, %b
  ret <3 x i6> %0
}