/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "SpeedOfLight",              /* Team name */

    "He JiaYun",     /* First member full name */
    "luke05@126.com",  /* First member email address */

    "Zhao Yufan",                   /* Second member full name (leave blank if none) */
    "13615254999@163.com"                    /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst) 
{
   int i, j;

    for (i = 0; i < dim; i=i+4)
{
	for (j = 0; j < dim; j=j+1){
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)]; 
        
	    dst[RIDX(dim-1-j, i+1, dim)] = src[RIDX(i+1, j, dim)]; 
       
	    dst[RIDX(dim-1-j, i+2, dim)] = src[RIDX(i+2, j, dim)]; 
        
	    dst[RIDX(dim-1-j, i+3, dim)] = src[RIDX(i+3, j, dim)];
} 
}

}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_functions() 
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);   
    add_rotate_function(&rotate, rotate_descr);   
    /* ... Register additional test functions here */
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p) 
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) 
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) 
	for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) 
	    accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}
static pixel avgz(int dim, int i, int j, pixel *src) 
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = i-1; ii <= i+1; ii=ii+3) 
	for(jj = j-1; jj <=j+1; jj++) 
{
	    accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);
accumulate_sum(&sum, src[RIDX(ii+1, jj, dim)]);
accumulate_sum(&sum, src[RIDX(ii+2, jj, dim)]);
      
}

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}

/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst) 
{
   int i, j;
for (j = 0; j < dim; j++)
{
	    dst[RIDX(0, j, dim)] = avg(dim, 0, j, src);
	    dst[RIDX(1, j, dim)] = avg(dim, 1, j, src);
            dst[RIDX(dim-1, j, dim)] = avg(dim, dim-1, j, src);
	    dst[RIDX(dim-2, j, dim)] = avg(dim, dim-2, j, src);
   }

   for (i = 0; i < dim; i++)
{
	   dst[RIDX(i, 0, dim)] = avg(dim, i, 0, src);
           dst[RIDX(i, 1, dim)] = avg(dim, i, 1, src);
           dst[RIDX(i, dim-1, dim)] = avg(dim, i, dim-1, src);
           dst[RIDX(i, dim-2, dim)] = avg(dim, i, dim-2, src);
   }


    for (i = 1; i < dim-2; i++)
	for (j = 1; j < dim-2; j++)
	   dst[RIDX(i, j, dim)] = avgz(dim, i, j, src);


}


/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    /* ... Register additional test functions here */
}

