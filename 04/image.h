//
// Created by Alan Peixinho on 9/12/15.
//

#ifndef LAB04_IMAGE_H
#define LAB04_IMAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Image{
    int n;
    int* pixel;
} Image;

Image *readImageP5(const char *format);
void destroyImage(Image** image);

#endif //LAB04_IMAGE_H