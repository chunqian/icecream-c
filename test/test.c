#include "../icecream.h"

int main(int argc, char const *argv[])
{
    /* code */
    char a[] = "hello";
    int b = 3;
    int *p = &b;
    ic();
    ic(a);

    ic_str(a, "ccc");
    ic_str2("bbb", a);
    ic_str3("bbb", a, a);
    ic_str4("bbb", a, "777", "888");

    ic_int(1, b * 2);
    ic_int2(b * 3, b - 1);
    ic_int3(b * 3, b - 1, b * 12);
    ic_int4(b, b * 3, b - 1, b * 12);

    ic_hex(15, b * 2);
    ic_hex2(b * 3, b - 1);
    ic_hex3(b * 3, b - 1, b * 12);
    ic_hex4(b, b * 3, b - 1, b * 12);

    ic_float(1.0);
    ic_float2(1.0, (float)b);
    ic_float3((float)b, (float)b * 2, 1.0);
    ic_float4((float)b, (float)b / 3, 2.0, (float)b);

    ic_ptr(a, "ddd");
    ic_ptr2(a, &b);
    ic_ptr3(a, &b, p);
    ic_ptr4(a, &b, p, &p);
    return 0;
}
