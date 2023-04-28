#include <stdio.h>
#include "nec.h"

int main()
{
    int* a = 0;
    nec_push(a, 23);
    printf("IDEMOOO %d\n", a[0]);
    return 0;
}

