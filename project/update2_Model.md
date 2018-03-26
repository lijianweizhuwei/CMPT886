# We Rebuild the Cost Model Again !

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
But we don't use iterative implementation, all the legalization and SWAR part are down one time! We increase the second level but we avoid most of the first level cost. There're still transformation cost but now they are also basic instruction set, so the first level cost now is down to second level cost.

### Third Level Cost Implementation
The **third level cost** is the **CPU circle cost**. We introduce the **instruction allocation**, the basic idea is to allocate the execution of basic instrcutions which have longer latency as early as possible, of course this should be dynamic, achieved by additional logic. This implementation spilt the third level cost out of the three-level model.

So after these rebuilding, we transform the Three-Level Cost Model into two One Level Cost Models which can be optimized seperately.
