#include "nec.h"

int main()
{
    int* array = 0;

    for(int i = 0; i < 100; i++)
    {
        nec_push(array, i + 1);
    }

    const size_t size = nec_size(array);
    for(int i = 0; i < size / 2; i++)
    {
        nec_pop(array);
    }

    nec_free(array);
    return 0;
}

