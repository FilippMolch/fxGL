//
// Created by Filipp on 23.12.2023.
//

#ifndef SOFT_3D_FXRASTERIZE_H
#define SOFT_3D_FXRASTERIZE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "fxMath.h"

#define FX_POINT 1
#define FX_LINE 2
#define FX_TRIANGLES 3

int fxDrawArray(int prim_type, int prim_count);

#endif //SOFT_3D_FXRASTERIZE_H
