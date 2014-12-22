#include "r3cube.h"
#include "r3_init.h"

int r3_init(r3cube *cube)
{
    if (!cube) {
        return -1;
    }
    cube->initialized = 1;
    return 0;
}

// vim: et ts=4 sw=4 sts=-1 :
