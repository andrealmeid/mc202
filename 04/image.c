//
// Created by Alan Peixinho on 9/12/15.
//

#include "image.h"
#include <stdio.h>
#include <stdlib.h>

void skipComments(FILE *fp)
{
    //skip for comments
    while (fgetc(fp) == '#') {
        while (fgetc(fp) != '\n');
    }
    fseek(fp,-1,SEEK_CUR);
}

Image *readImageP5(const char *filename)
{
    int *img = NULL;
    FILE    *fp=NULL;
    unsigned char   *data8=NULL;
    char    type[10];
    int     n, p,xsize,ysize,max;

    fp = fopen(filename,"rb");
    if (fp == NULL){
        printf("Error opening image.\n");
        exit(1);
    }

    if (fscanf(fp,"%s\n",type) != 1){
        printf("Reading error\n");
        exit(1);
    }

    if((strcmp(type,"P5")==0)) {
        skipComments(fp);
        if (fscanf(fp, "%d %d\n", &xsize, &ysize) != 2) {
            printf("Reading error\n");
            exit(1);
        }

        if (fscanf(fp, "%d\n", &max) != 1) {
            printf("Reading error\n");
            exit(1);
        }

        n = xsize*ysize;
        data8 = calloc(n, sizeof(unsigned char));
        if (fread(data8,sizeof(unsigned char),n,fp)!=n) {
            printf("Reading error\n");
            exit(1);
        }

        img = calloc(n, sizeof(int));
        for (p = 0; p < n; ++p) {
            img[p] = (int)data8[p];
        }
    }

    free(data8);
    fclose(fp);

    Image* image = (Image*)calloc(1, sizeof(Image));
    image->n = n;
    image->pixel = img;

    return(image);
}

void destroyImage(Image** image) {
    free((*image)->pixel);
    free(*image);
    *image = NULL;
}

