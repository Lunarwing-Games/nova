#include <stdlib.h>
#include <stdio.h>

#define ASSERT(_e, _msg) do \
    { if (!_e) { fprintf(stderr, "Bad Assert: %s\n", _msg); abort();}} while (0)