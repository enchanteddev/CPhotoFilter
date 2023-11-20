#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"


char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}


void BW(Matrix imageR, Matrix imageG, Matrix imageB){
    int x = imageR.n;
    int y = imageR.m;
    Matrix imageBW;
    imageBW = new_matrix(y, x);
    for (int i = 0; i < y; i++){
        for (int j = 0; j < x; j++){
            uint8_t r = 0.2126 * get_value(imageR, i, j);
            uint8_t g = 0.7152 * get_value(imageG, i, j);
            uint8_t b = 0.0722 * get_value(imageB, i, j);
            // printf("%d %d ", b, get_value(imageB, i, j));
            set_value(imageBW, i, j, r + g + b);
        }        
    }

    FILE *writefp;
    writefp = fopen("yournameBW1.ppm", "w");
    fprintf(writefp, "P2\n");
    fprintf(writefp, "%d %d\n%d\n", x, y, 255);
    for (int i = 0; i < y; i++){
        for (int j = 0; j < x; j++){
            fprintf(writefp, "%d ", get_value(imageBW, i, j));
        }
        
    }
    fclose(writefp);
}


Matrix BlurMatrix(Matrix imageR, int px){
    int x = imageR.n;
    int y = imageR.m;
    
    Matrix blurMatrix;
    blurMatrix = new_matrix(y, x);

    for (int i = 0; i < y; i++){
        int net_value = 0;
        for (int j = 0; j < x; j++){
            int up      = i <  px     ? 0     : i - px;
            int down    = i >= y - px ? y - 1 : i + px;
            int left    = j <  px     ? 0     : j - px;
            int right   = j >= x - px ? x - 1 : j + px;
            
            int cells = (down - up + 1) * (right - left + 1);
            if (net_value == 0){
                for (int k = up; k <= down; k++){
                    for (int l = left; l <= right; l++){
                        net_value += get_value(imageR, k, l);
                    }
                }
            } else {
                for (int l = up; l <= down; l++){
                    int r = j >= x - px ? 0 : get_value(imageR, l, right);
                    int l_ = left == 0   ? 0 : get_value(imageR, l, left - 1);
                    net_value += r - l_;
                }
            }

            set_value(blurMatrix, i, j, net_value / cells);            
        }
    }
    return blurMatrix;
}


void Blur(Matrix imageR, Matrix imageG, Matrix imageB, int px, char* fp){
    int x = imageR.n;
    int y = imageR.m;

    Matrix bR = BlurMatrix(imageR, px);
    Matrix bG = BlurMatrix(imageG, px);
    Matrix bB = BlurMatrix(imageB, px);

    FILE *writefp;
    writefp = fopen(concat("blur", fp), "w");
    fprintf(writefp, "P3\n");
    fprintf(writefp, "%d %d\n%d\n", x, y, 255);
    for (int i = 0; i < y; i++){
        for (int j = 0; j < x; j++){
            fprintf(writefp, "%d %d %d ", get_value(bR, i, j), get_value(bG, i, j), get_value(bB, i, j));
        }
        
    }
    fclose(writefp);
}


int main(int argc, char* argv[]){
    if (argc != 3){
        printf("File name and blr px");
        return 1;
    }
    FILE *fp;
    fp = fopen(argv[1], "r");
    char format[4];
    int x, y, m;
    fscanf(fp, "%s", format);
    fscanf(fp, "%d %d %d\n", &x, &y, &m);
    printf("%s %d %d %d", format, x, y, m);

    // int r, g, b;
    // fscanf(fp, "%d %d %d", &r, &g, &b);
    // printf("%d %d %d\n", r, g, b);

    // return;

    Matrix imageR, imageG, imageB;
    imageR = new_matrix(y, x);
    imageG = new_matrix(y, x);
    imageB = new_matrix(y, x);
    for (int i = 0; i < y; i++){
        for (int j = 0; j < x; j++){
            int r, g, b;
            fscanf(fp, "%d %d %d", &r, &g, &b);
            // if (r != 0) printf("%d %d %d\n", r, g, b);
            set_value(imageR, i, j, r);
            set_value(imageG, i, j, g);
            set_value(imageB, i, j, b);
        }
    }
    fclose(fp);

    // BW(imageR, imageG, imageB);   
    Blur(imageR, imageG, imageB, atoi(argv[2]), argv[1]);
}