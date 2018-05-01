# Error Bit Stream

Suppose we have the regular expression `T[AG]+C` and wish to find all instance which are not in this pattern and regard them as errors, then form the error bit stream. For example:

```
   input data      TGACTAGGAATAGCGAATAGAGGAGCTCAGC
error bit stream   .........1....111..........1...
```
There are three errors, `A`, `GAA` and `C`, the error bit stream we want it shown as above.

### How can we achieve it?
**Parabix** uses the *basis bit streams* to construct *character-class bit streams* in which each 1 bit indicates the presence of a significant character (or class of characters) in the parsing process. The *advance* operation is as following:

```
 input data     TGACTAGGAATAGCGAATAGAGGAGCTCAGC
     C0         1...1.....1......1........1....   character-class bit stream of T
L0=advance(C0)  .1...1.....1......1........1...   character-class bit stream after advance
```
The bit after advance is called cursor bit.

### ScanThru operation
*ScanThru* is an operation in **Parabix** that sets the cursor position immediately after a run of marker position in the input bit stream. *ScanThru* accepts two input parameters, *c* and *m*, where *c* denotes an initial set of cursor positions, and *m* denotes a set of “marked” lexical item positions. The ScanThru operation determines the cursor positions immediately following any run of marker positions by calculating `ScanThru(c,m)=(c+m)∧¬m`.

```
 input data     TGACTAGGAATAGCGAATAGAGGAGCTCAGC
     C0         1...1.....1......1........1....   character-class bit stream of T
     C1         ...1.........1...........1.1..1   character-class bit stream of C
    Alpha       .11..1111..11.111.1111111...11.   character-class bit stream of A OR G
L0=advance(C0)  .1...1.....1......1........1...   character-class bit stream after advance
```
   
Let's calculate `L1=ScanThru(L0,Alpha)=(L0+Alpha)∧¬Alpha`:
 
```
                TGACTAGGAATAGCGAATAGAGGAGCTCAGC
 T0=L0+Alpha    ...1.....1...1111........1.111.
   ¬Alpha       1..11....11..1...1.......111..1
 L1=T0∧¬Alpha   ...1.....1...1...........1.1...
```

Now we can detect the errors:

```
                TGACTAGGAATAGCGAATAGAGGAGCTCAGC
  E0=L0∧¬Alpha  ...........................1...      detect the error `C`
   E1=L1∧¬C1    .........1.....................      detect the error `A`
```

`E0` detects the error `TC`, `E1` detects the error `A`:
```
   input data      TGACTAGGAATAGCGAATAGAGGAGCTCAGC
error bit stream   .........1....111..........1...   this is the error bit stream we want
   E0=L0∧¬Alpha    ...........................1...
    E1=L1∧¬C1      .........1.....................
```
But how can we detect the error `GAA`? What if we use `E2=T0∧¬L1`:

```
   input data      TGACTAGGAATAGCGAATAGAGGAGCTCAGC
   T0=L0+Alpha     ...1.....1...1111........1.111.
   L1=T0∧¬Alpha    ...1.....1...1...........1.1...
    E2=T0∧¬L1      ..............111...........11. 
```

The error bit stream `E2` is wrong!

This is because the error `C` which is detected by `E0` has affect on the following stream `AG`. Which indicates we need to handle the errors step by step, we should remove the error detected by `E0`, then consider the error detected by `E1` and `E2`.
So the correct order should be:

```
 input data     TGACTAGGAATAGCGAATAGAGGAGCTCAGC
     C0         1...1.....1......1........1....   character-class bit stream of T
L0=advance(C0)  .1...1.....1......1........1...   character-class bit stream after advance
    Alpha       .11..1111..11.111.1111111...11.   character-class bit stream of alphabet
   ¬Alpha       1..11....11..1...1.......111..1
 E0=L0∧¬Alpha   ...........................1...   detect the error `C`
 
                    Correct the error by deletion

                TGACTAGGAATAGCGAATAGAGGAGCTAGC
     C0         1...1.....1......1........1...   character-class bit stream of T
L0=advance(C0)  .1...1.....1......1........1..   character-class bit stream after advance
    Alpha       .11..1111..11.111.1111111..11.   character-class bit stream of alphabet 
   ¬Alpha       1..11....11..1...1.......11..1
 T0=L0+Alpha    ...1.....1...1111........1...1
 L1=T0∧¬Alpha   ...1.....1...1...........1...1
      C1        ...1.........1...........1...1   character-class bit stream of C
   E1=L1∧¬C1    .........1....................   detect the error `A`
   E2=T0∧¬L1    ..............111.............   detect the error `GAA`
```

The problem is that different kinds of anomalies can be affected by each other, which will lead to a wrong detection, so the detection and correction (by addition, deletion or substitution) of different kinds of anomalies need to follow a certain sequence, and the sequence should be decided by the pattern.
