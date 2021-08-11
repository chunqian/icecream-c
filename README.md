# IceCream-C

Never use printf to debug again

Single header, implemented in C99 of [IceCraem](https://github.com/gruns/icecream).

Based on [rxi's log.c](https://github.com/rxi/log.c)

<img align="center" src="https://github.com/chunqian/icecream-c/blob/master/test/test.png" width="70%">

## use

import:

```c
#include "icecream.h"
```

**The exmple when there are no parameters**

```c
ic();
```

output

```c
🍦 test/test.c:24 in main()
```

**The example of printing string**

```c
char str_a[] = "hello";
char str_b[] = "world";

ic_str(str_a, str_b, "!");
```

output

```c
🍦 str_a = "hello", str_b = "world", "!" = "!"
```

**The example of printing int**

```c
int int_a = 32767;
int int_b = -32768;

ic_int(int_a, int_b, 999);
```

output

```c
🍦 int_a = 32767, int_b = -32768, 999 = 999
```

**The example of printing long**

```c
long long_a = 2141483647;
long long_b = -2147483648;

ic_long(long_a, long_b, 999999);
```

output

```c
🍦 long_a = 2141483647, long_b = -2147483648, 999999 = 999999
```

**The example of printing hex**

```c
int int_a = 32767;
int int_b = -32768;

ic_hex(int_a, int_b, 999);
```

output

```c
🍦 int_a = 0x7fff, int_b = 0xffff8000, 999 = 0x3e7
```

**The example of printing float**

```c
float float_a = 1.0;
float float_b = 2.0;

ic_float(float_a, float_b, 999.0);
```

output

```c
🍦 float_a = 1.00, float_b = 2.00, 999.0 = 999.00
```

**The example of printing double**

```c
double double_a = 1.0;
double double_b = 2.0;

ic_double(double_a, double_b, 999.0);
```

output

```c
🍦 double_a = 1.0000, double_b = 2.0000, 999.0 = 999.0000
```

**The example of printing pointer**

```c
char str_a[] = "hello";
char str_b[] = "world";
int *ptr_a = &str_a;
int *ptr_b = &str_b;

ic_ptr(ptr_a, ptr_b, "!");
```

output

```c
🍦 ptr_a = 0x7ffee2bcf378, ptr_b = 0x7ffee2bcf380, "!" = 0x10d031e3e
```

## License
This library is free software; you can redistribute it and/or modify it under
the terms of the MIT license. See [LICENSE](LICENSE) for details.
