#include <Python.h>
#include <stdio.h>
#include "face.h"

#if 0

int main (int agrc, char *argv[])
{
    double face_result = 0.0;

    face_init();

    face_result = face_category();
    printf("face_result = %0.2lf\n", face_result);

    face_final();

    return 0;
}

#endif