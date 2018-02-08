## SWAR

@(CS)

#### Direction

(下面有斜体加粗的是指有Detail的)

Transform one instruction on irregular size of vectors to a bunch of instructions on regular size of vectors which are supported by CPUs.

With notion of SWAR.

If for Parabix, we can focus on several specific type of size
* 128-bit SIMD <32 x i4>, <64 x i2>, <128 x i1>
* 256-bit SIMD <64 x i4>, <128 x i2>, <256 x i1>

##### 1. Concept of SWAR
The basic concept of SWAR, SIMD Within A Register, is that operations on word-length registers can be used to speed-up computations by performing SIMD parallel operations on n k/n-bit field values.
我的理解是 计算一个不常规大小的vector的加法或者减法或者其他运算，比如<6 x i3>， 依然在一个register里面计算（大小放得下的register比如i32）:

<i3 x, i3 y, i3 z, i3 a, i3 b, i3 c>放在i32的后18位里，其余清零，

而不是把它变成<6 x i8>或者<6 x i4>再进行计算。

##### 2. [Types of Operations](http://www.phys.aoyama.ac.jp/~w3-furu/aoyama+/Tech_notes/adaptor_doc/Users_Guide.pdf)
* Polymorphic Operations (data type independent) e.g.: **AND**, **OR**, **NOT** (不因为vector大小而结果不同的运算)
* Partitioned Operations (not independent) e.g.: + - x / (需要对vector进行校正才能得到正确结果的运算，比如加减乘除，因为加、乘会溢出，如果直接将vector相加，一旦某个vector进位就会对其他vector的结果造成影响，所以下面的运算也基本实在解决这个问题)
* Communication & Type Conversion Operations (下面这几个不知道是啥，感觉对这个问题没啥用。。。。)
* Recurrence Operations (Reductions, Scans, etc.)
* Masking Operations

##### 3. [Ways of Tranformation](https://www.tldp.org/HOWTO/Parallel-Processing-HOWTO-4.html)
1. Partitioned Instructions (hardware support 让硬件支持特殊的vector大小)
2. ***Unpartitioned Operations With Correction Code***
3. ***Controlling Field Values***

#### Details
##### 1. Unpartitioned Operations With Correction Code
###### 1.1 Add
* Challenge: Overflow
* Solution:
	* Mask out high bit of each area (去掉每个field的最高位，所以相加时有进位也没关系)
	* Perform addition as **result1** (相加)
	* XOR (Add high bits) (将要相加的vector异或，不知道为毛这个操作可以获得是否进位过的信息，反正就是可以知道)
	* Mask high bits **result2** (获取每个field的最高位)
	* Add two results with XOR (将两个结果异或得到最终结果。并不知道为毛)
* [Example](https://coursys.sfu.ca/2016sp-cmpt-886-g2/pages/SWAR-Example1/view) (直接看第二页的那个例子)

所以其他的运算都要我们自己设计。或者找到别人设计过的。。

##### 2. Controlling Field Values
###### 2.1 Add
* Challenge: Overflow
* Solution: Add one bit in front of each vector for the result (这个的方法就是在每个vector前面多留了一位给它进位，然后获取结果的时候用mask忽略进位)

e.g.: <4 x i7> x + <4 x i7> y
((x + y) & 0x7f7f7f7f)
![Alt text](./1518093676456.png)
