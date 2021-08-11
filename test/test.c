#include "../icecream.h"

int main(int argc, char const *argv[])
{
    /* code */
    char str_a[] = "hello";
    char str_b[] = "world";

    int int_a = 32767;
    int int_b = -32768;

    long long_a = 2141483647;
    long long_b = -2147483648;

    float float_a = 1.0;
    float float_b = 2.0;

    double double_a = 1.0;
    double double_b = 2.0;

    int *ptr_a = &str_a;
    int *ptr_b = &str_b;

    ic();

    ic_str(str_a, str_b, "!");

    ic_int(int_a, int_b, 999);

    ic_long(long_a, long_b, 999999);

    ic_hex(int_a, int_b, 999);

    ic_float(float_a, float_b, 999.0);

    ic_double(double_a, double_b, 999.0);

    ic_ptr(ptr_a, ptr_b, "!");
    return 0;
}
