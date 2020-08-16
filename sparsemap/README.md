### HEADER only fast implementation of a sparse map.
A page-table like sparse map structure. Look ups are always finite
number of loads.

```C
#include "SparseMemory.h"
#include "Address.h"
...
SparseMemory<long long int> S(5);
```

This is a map from "Address" -> T*
The key is always Address type.
5 is the number of levels in the lookup; also the number of loads
required to get to the pointer.

```C
Address A(0x55000);
long long int *a = new long long int;
```

Create key object A with value 0x55000. Address can be initialized
with any 64 bit value.

Create value object a. Has to always be created on the heap since the
map only includes a pointer to the value.

```C
// insert element into map
S.add(A,a);
// insert element if it does not already exist
ret = S.lookup(A);
  if (a != NULL)
      S.add(A,a)
      ````

### Unit test and Usage
To understand usage, see main.cc.





```
g++ main.cc
```

### Thanks
Derived from the Wisconsin GEMS 2.0 repository

