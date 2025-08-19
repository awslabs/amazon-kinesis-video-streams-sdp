# MISRA Compliance

The SDP library files conform to the [MISRA C:2012](https://www.misra.org.uk/misra-c)
guidelines, with the deviations listed below. Compliance is checked with Coverity static analysis.
Since the SDP library is designed for various platforms, including Linux and embedded devices, it
needs to have a very small memory footprint and be efficient. To achieve that and to increase the
performace of the library, it deviates from some MISRA rules.

Additionally, [MISRA configuration file](./test/coverity/misra.config) contains the project wide deviations.

### Suppressed with Coverity Comments
To find the deviation references in the source files run grep on the source code
with ( Assuming rule 18.2 violation; with justification in point 1 ):
```
grep 'MISRA Ref 18.2.1' . -rI
```
#### Rule 10.8

_Ref 10.8.1_

- MISRA C-2012 Rule 10.8 states that value of composite expressions should not be cast
  to variables of different signedness. In this library, array of bytes are used to
  process data. Functions which fill the arrays with data update an index to an
  offset. To know the amount of data added to the array, the beginning address of the
  array has to be subtracted from the index. When the two pointers are subracted, it
  results in a signed value. It is verified however that the value will always be positive.
  And thus, can be casted and added to a size_t variable (which is unsigned).
