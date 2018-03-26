## Three-Level Cost Model 

![Alt text](../image//flowchart.png)

### Brief description

* Traditional cost model evaluate the **total instruction number**, we regard this as a **second level cost**.
* Since we have multiple options for a certain instruction, there will be many option transformation, we call it **transformation cost**, transformation might introduce many memory operations, which is much "heavier" than register operation cost, we regard transformation cost as **first level cost**.
* The **CPU circle cost** is the **third level cost**, which isn't the total CPU circles needed by all the instructions, since basic instruction can run simultaneously.

These three level cost have the same tendency, if there're more transformations, there might be more instructions, which will lead to more CPU circles when execute these instructions. 


### Three-Level Down To Two-Level

LLVM SelectDAG legalization phase is an iterative process, first widen the vector to a power of two, and after that we split, promote, scalarize, ... 
The legalization phase contains both **Type Legalization** and **Instruction Legalization**, the transformation cost which is the second level cost is mainly introduced by **Instruction Legalization**, because there're memory operations in **Instruction Legalization**.
But we don't use iterative implementation, all the legalization and SWAR part are down one time! So we increase the second level 

### Next to do

Register number is another important parameter !

If we only consider the total instruction number, we will always give up the option of SWAR, because SWAR will introduce more instructions, but it definitely save the register.

Now we're facing a problem of how to quatify the register number within the three-level cost model. T_T
