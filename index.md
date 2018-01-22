# Error Bit Stream

Suppose we have the regular expression `<[a-zA-Z]+>` and wish to find all instance which are not in this pattern and regard them as errors, then form the error bit stream. For example:

```
   input data      <My> <name] <is> err <jianwei> <>li>
error bit stream   ..........1......111............1...
```
There are three errors, `]`, `err` and `>`, the error bit stream we want it shown as above.

### How can we achieve it?
We know that **Parabix** uses the *basis bit streams* to construct *character-class bit streams* in which each 1 bit indicates the presence of a significant character (or class of characters) in the parsing process. The *advance* operation is mentioned in class:

```
 input data     <My> <name] <is> err <jianwei> <>li>
     C0         1....1......1........1.........1....   character-class bit stream of <
L0=advance(C0)  .1....1......1........1.........1...   character-class bit stream after advance
```
The bit after advance is called cursor bit.

### ScanThru operation
*ScanThru* is an operation in **Parabix** that sets the cursor position immediately after a run of marker position in the input bit stream. *ScanThru* accepts two input parameters, *c* and *m*, where *c* denotes an initial set of cursor positions, and *m* denotes a set of “marked” lexical item positions. The ScanThru operation determines the cursor positions immediately following any run of marker positions by calculating `ScanThru(c,m)=(c+m)∧¬m`.

```
 input data     <My> <name] <is> err <jianwei> <>li>
     C0         1....1......1........1.........1....   character-class bit stream of <
L0=advance(C0)  .1....1......1........1.........1...   character-class bit stream after advance
    Alpha       .11...1111...11..111..1111111....11.   character-class bit stream of alphabet
```
   
Let's calculate `L1=ScanThru(L0,Alpha)=(L0+Alpha)∧¬Alpha`:
 
```
                <My> <name] <is> err <jianwei> <>li> 
 T0=L0+Alpha    ...1......1....1.111.........1..111.
   ¬Alpha       1..111....111..11...11.......1111..1
 L1=T0∧¬Alpha   ...1......1....1.............1..1...
```

Now we can detect the errors:

```
                <My> <name] <is> err <jianwei> <>li>
     C1         ...1...........1.............1..1..1      character-class bit stream of >
L0=advance(C0)  .1....1......1........1.........1...
   ¬Alpha       1..111....111..11...11.......1111..1
```

```
 E0=L0∧¬Alpha   ................................1...
   E1=L1∧¬C1    ..........1.........................
```

`E0` detects the error `<>`, `E1` detects the error `]`:
```
   input data      <My> <name] <is> err <jianwei> <>li>
error bit stream   ..........1......111............1...
   E0=L0∧¬Alpha    ................................1...
    E1=L1∧¬C1      ..........1.........................
```
But how can we detect the error `err`? What if we use `E2=T0∧¬L1`:

``
      E2=T0∧¬L1    .................111.............11.
``

The error bit stream `E2` is wrong. This is because the error `<>` which is detected by `E0` has affect on the following stream `li>`. Which indicates we need to handle the errors step by step, we should remove the error detected by `E0`, then consider the error detected by `E2`.
So the correct order should be:

```
 input data     <My> <name] <is> err <jianwei> <>li>
     C0         1....1......1........1.........1....   character-class bit stream of <
L0=advance(C0)  .1....1......1........1.........1...   character-class bit stream after advance
    Alpha       .11...1111...11..111..1111111....11.   character-class bit stream of alphabet
   ¬Alpha       1..111....111..11...11.......1111..1
 E0=L0∧¬Alpha   ................................1...   detect the error `<>`
 
                    Correct the error

                <My> <name] <is> err <jianwei> <li>
     C0         1....1......1........1.........1...   character-class bit stream of <
L0=advance(C0)  .1....1......1........1.........1..   character-class bit stream after advance
    Alpha       .11...1111...11..111..1111111...11.   character-class bit stream of alphabet 
 T0=L0+Alpha    ...1......1....1.111.........1....1
   ¬Alpha       1..111....111..11...11.......111..1
 L1=T0∧¬Alpha   ...1......1....1.............1....1
      C1        ...1...........1.............1....1   character-class bit stream of >
   E1=L1∧¬C1    ..........1........................   detect the error `]`
   E2=T0∧¬L1    .................111...............   detect the error `err`
```

Now we detect all the errors in correct order, but this make the detection more complicated. So how can we reduce the complexity and decide the correct priorities of different kinds of errors?
I don't know yet... Need futher consideration....
