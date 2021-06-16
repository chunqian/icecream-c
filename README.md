# IceCream-C

Never use printf to debug again

Single header, implemented in C99 of [IceCraem](https://github.com/gruns/icecream).

Based on [rxi's log.c](https://github.com/rxi/log.c)

## use

import:

```c
#include "icecream.h"
```

**The exmple when there are no parameters**

```c
ic()
```

output

```c
🍦 test/test.c:9 in main()
```

**The example of printing string**

```c
char a[] = "hello";

ic_str(a, "ccc");
ic_str2("bbb", a);
ic_str3("bbb", a, a);
ic_str4("bbb", a, "777", "888");
```

output

```c
🍦 a = "hello" 
🍦 "bbb" = "bbb", a = "hello" 
🍦 "bbb" = "bbb", a = "hello", a = "hello" 
🍦 "bbb" = "bbb", a = "hello", "777" = "777", "888" = "888" 
```

**The example of printing int**

```c
int b = 3;

ic_int(1, b * 2);
ic_int2(b * 3, b - 1);
ic_int3(b * 3, b - 1, b * 12);
ic_int4(b, b * 3, b - 1, b * 12);
```

output

```c
🍦 1 = 1 
🍦 b * 3 = 9, b - 1 = 2 
🍦 b * 3 = 9, b - 1 = 2, b * 12 = 36 
🍦 b = 3, b * 3 = 9, b - 1 = 2, b * 12 = 36 
```

**The example of printing hex**

```c
int b = 3;

ic_hex(15, b * 2);
ic_hex2(b * 3, b - 1);
ic_hex3(b * 3, b - 1, b * 12);
ic_hex4(b, b * 3, b - 1, b * 12);
```

output

```c
🍦 15 = 0xf 
🍦 b * 3 = 0x9, b - 1 = 0x2 
🍦 b * 3 = 0x9, b - 1 = 0x2, b * 12 = 0x24 
🍦 b = 0x3, b * 3 = 0x9, b - 1 = 0x2, b * 12 = 0x24 
```

**The example of printing float**

```c
int b = 3;

ic_float(1.0);
ic_float2(1.0, (float)b);
ic_float3((float)b, (float)b * 2, 1.0);
ic_float4((float)b, (float)b / 3, 2.0, (float)b);
```

output

```c
🍦 1.0 = 1.000000 
🍦 1.0 = 1.000000, (float)b = 3.000000 
🍦 (float)b = 3.000000, (float)b * 2 = 6.000000, 1.0 = 1.000000 
🍦 (float)b = 3.000000, (float)b / 3 = 1.000000, 2.0 = 2.000000, (float)b = 3.000000 
```

**The example of printing pointer**

```c
char a[] = "hello";
int b = 3;
int *p = &b;

ic_ptr(a, "ddd");
ic_ptr2(a, &b);
ic_ptr3(a, &b, p);
ic_ptr4(a, &b, p, &p);
```

output

```c
🍦 a = 0x7ffeef5223ac 
🍦 a = 0x7ffeef5223ac, &b = 0x7ffeef5223b4 
🍦 a = 0x7ffeef5223ac, &b = 0x7ffeef5223b4, p = 0x7ffeef5223b4 
🍦 a = 0x7ffeef5223ac, &b = 0x7ffeef5223b4, p = 0x7ffeef5223b4, &p = 0x7ffeef5223a0 
```

## License
This library is free software; you can redistribute it and/or modify it under
the terms of the MIT license. See [LICENSE](LICENSE) for details.
