// 311432082 Edward Gutman
// tests used by Ron Even
#include <stdbool.h>

/**
 * pixel representation.
 * consists of three color channels - rgb.
 */

typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} pixel;

/**
 * This function does the smoothing of an image.
 * For each row in the pixel matrix, we calculate sum of each channel - of a 3x3 window around the first pixel.
 * (just like smoothing kernel).
 * Then, as we iterate on the row we update it: by adding the 3 new values from the new column, and subtracting
 * the 3 old values from the previous column on the window.
 * By doing so we get the sum of each pixel's channel values, but with a smaller amount of memory accesses.
 * After that, we divide each channel's sum by the kernel scale and update the destination image with the newly
 * found sums.
 *
 * @param dim the dimension of the image,
 * @param src source pixel matrix.
 * @param dst destination pixel matrix.
 * @param kernelScale a number to scale by the pixels.
 */

void smooth(int dim, pixel src[m][n], pixel dst[m][n], int kernelScale) {
    int i, j;
    int size = dim - 1;

    int rSum = 0, gSum = 0, bSum = 0;

    // iterate over each pixel in the border
    for (i = 1 ; i < size; i++) {
        for (j =  1 ; j < size ; j++) {

            // if its the first column, calculate the starting sum of 3x3 window around the current pixel,
            // for this row.
            if (j == 1) {
                // utilization of spatial and temporal locality, and combining few elements over one iteration.
                rSum = src[i-1][0].red + src[i-1][1].red + src[i-1][2].red +
                       src[i][0].red + src[i][1].red + src[i][2].red +
                       src[i+1][0].red + src[i+1][1].red + src[i+1][2].red;
                gSum = src[i-1][0].green + src[i-1][1].green + src[i-1][2].green +
                       src[i][0].green + src[i][1].green + src[i][2].green +
                       src[i+1][0].green + src[i+1][1].green + src[i+1][2].green;
                bSum = src[i-1][0].blue + src[i-1][1].blue + src[i-1][2].blue +
                       src[i][0].blue + src[i][1].blue + src[i][2].blue +
                       src[i+1][0].blue + src[i+1][1].blue + src[i+1][2].blue;
            } else {
                // if its not the first column, add and subtract the correct values for the new 3x3 window
                // also utilization of spatial and temporal locality, and combining few elements over one iteration.
                rSum = rSum + src[i-1][j+1].red + src[i][j+1].red + src[i+1][j+1].red
                       - src[i-1][j-2].red - src[i][j-2].red - src[i+1][j-2].red;
                gSum = gSum + src[i-1][j+1].green + src[i][j+1].green + src[i+1][j+1].green
                       - src[i-1][j-2].green - src[i][j-2].green - src[i+1][j-2].green;
                bSum = bSum + src[i-1][j+1].blue + src[i][j+1].blue + src[i+1][j+1].blue
                       - src[i-1][j-2].blue - src[i][j-2].blue - src[i+1][j-2].blue;
            }

            // divide each channel's sum by the kernel scale and update the destination image
            dst[i][j].red = rSum/kernelScale;
            dst[i][j].green = gSum/kernelScale;
            dst[i][j].blue = bSum/kernelScale;
        }
    }
}

/**
 * This function does the sharpening of an image.
 * For each row in the pixel matrix, we calculate sum of each channel - of a 3x3 window around the first pixel,
 * by adding 9 times the current pixel values and subtracting each of the surround pixel values.
 * (just like sharpening kernel).
 * Then, as we iterate on the row we update it: by subtracting the 3 new values from the new column, adding
 * the 3 old values from the previous column on the window, subtracting 10 times of the current pixel values,
 * and lasting adding 10 times the previous pixel values.
 * By doing so we get the sum of each pixel's channel values multiplied by the sharpening kernel,
 * but with a smaller amount of memory accesses.
 * After that, we divide each channel's sum by the kernel scale and update the destination image with the newly
 * found sums.
 *
 * @param dim the dimension of the image,
 * @param src source pixel matrix.
 * @param dst destination pixel matrix.
 * @param kernelScale a number to scale by the pixels.
 */

void sharp(int dim, pixel src[m][n], pixel dst[m][n], int kernelScale) {
    int i, j, r, g, b;
    int size = dim - 1;
    int rSum = 0, gSum = 0, bSum = 0;

    // iterate over each pixel in the border
    for (i = 1 ; i < size; i++) {
        for (j =  1 ; j < size ; j++) {

            // if its the first column, calculate the starting sum of 3x3 window around the current pixel,
            // for this row.
            if (j == 1) {
                // utilization of spatial and temporal locality.
                rSum = -src[i-1][0].red - src[i-1][1].red - src[i-1][2].red -
                       src[i][0].red + 9*src[i][1].red - src[i][2].red -
                       src[i+1][0].red - src[i+1][1].red - src[i+1][2].red;
                gSum = -src[i-1][0].green - src[i-1][1].green - src[i-1][2].green -
                       src[i][0].green + 9*src[i][1].green - src[i][2].green -
                       src[i+1][0].green - src[i+1][1].green - src[i+1][2].green;
                bSum = -src[i-1][0].blue - src[i-1][1].blue - src[i-1][2].blue -
                       src[i][0].blue + 9*src[i][1].blue - src[i][2].blue -
                       src[i+1][0].blue - src[i+1][1].blue - src[i+1][2].blue;
            } else {
                // if its not the first column, add and subtract the correct values for the new 3x3 window
                // also utilization of spatial and temporal locality.
                rSum = rSum - src[i-1][j+1].red - src[i][j+1].red - src[i+1][j+1].red
                       + src[i-1][j-2].red + src[i][j-2].red + src[i+1][j-2].red +
                       10*src[i][j].red - 10*src[i][j-1].red;
                gSum = gSum - src[i-1][j+1].green - src[i][j+1].green - src[i+1][j+1].green
                       + src[i-1][j-2].green + src[i][j-2].green + src[i+1][j-2].green +
                       10*src[i][j].green - 10*src[i][j-1].green;
                bSum = bSum - src[i-1][j+1].blue - src[i][j+1].blue - src[i+1][j+1].blue
                       + src[i-1][j-2].blue + src[i][j-2].blue + src[i+1][j-2].blue +
                       10*src[i][j].blue - 10*src[i][j-1].blue;
            }

            // divide each channel's sum by the kernel scale and update the destination image
            r = rSum/kernelScale;
            g = gSum/kernelScale;
            b = bSum/kernelScale;

            // truncate each pixel channel
            // avoiding additional functions calling.
            if (r < 0)
                r = 0;
            if (r > 255)
                r = 255;

            if (g < 0)
                g = 0;
            if (g > 255)
                g = 255;

            if (b < 0)
                b = 0;
            if (b > 255)
                b = 255;


            // update the destination image with the new pixel.
            dst[i][j].red = r;
            dst[i][j].green = g;
            dst[i][j].blue = b;
        }
    }
}

/**
 * This function does the smoothing of an image but with application of a filter.
 * Similliar to the smoothing function, but with each pixel iteration we search for the min and max
 * pixel sum values.
 * Then we remove the min and max pixel values from the the corresponding channels sum, apply the kernel scaling,
 * and update the destination image.
 *
 * @param dim the dimension of the image,
 * @param src source pixel matrix.
 * @param dst destination pixel matrix.
 * @param kernelScale a number to scale by the pixels.
 */

void smoothFilter(int dim, pixel src[m][n], pixel dst[m][n], int kernelScale) {
    int i, j;
    int size = dim - 1;
    int maxRow, maxCol, minRow, minCol;
    int pixelSum;
    int rSum = 0, gSum = 0, bSum = 0;

    // iterate over each pixel in the border
    for (i = 1 ; i < size; i++) {
        for (j =  1 ; j < size ; j++) {
            int maxIntensity = -1;
            int minIntensity = 766;

            // if its the first column, calculate the starting sum of 3x3 window around the current pixel,
            // for this row.
            if (j == 1) {
                // utilization of spatial and temporal locality, and combining few elements over one iteration.
                rSum = src[i-1][0].red + src[i-1][1].red + src[i-1][2].red +
                       src[i][0].red + src[i][1].red + src[i][2].red +
                       src[i+1][0].red + src[i+1][1].red + src[i+1][2].red;
                gSum = src[i-1][0].green + src[i-1][1].green + src[i-1][2].green +
                       src[i][0].green + src[i][1].green + src[i][2].green +
                       src[i+1][0].green + src[i+1][1].green + src[i+1][2].green;
                bSum = src[i-1][0].blue + src[i-1][1].blue + src[i-1][2].blue +
                       src[i][0].blue + src[i][1].blue + src[i][2].blue +
                       src[i+1][0].blue + src[i+1][1].blue + src[i+1][2].blue;
            } else {
                // if its not the first column, add and subtract the correct values for the new 3x3 window
                // utilization of spatial and temporal locality, and combining few elements over one iteration.
                rSum = rSum + src[i-1][j+1].red + src[i][j+1].red + src[i+1][j+1].red
                       - src[i-1][j-2].red - src[i][j-2].red - src[i+1][j-2].red;
                gSum = gSum + src[i-1][j+1].green + src[i][j+1].green + src[i+1][j+1].green
                       - src[i-1][j-2].green - src[i][j-2].green - src[i+1][j-2].green;
                bSum = bSum + src[i-1][j+1].blue + src[i][j+1].blue + src[i+1][j+1].blue
                       - src[i-1][j-2].blue - src[i][j-2].blue - src[i+1][j-2].blue;
            }


            // search for the min and max pixel channels' sum, in a 3x3 window around the current pixel.
            // utilization of spatial and temporal locality, and avoiding more nested for loops.
            pixelSum = src[i-1][j-1].red + src[i-1][j-1].green + src[i-1][j-1].blue;
            if (pixelSum > maxIntensity) {
                maxIntensity = pixelSum;
                maxRow = i-1;
                maxCol = j-1;
            }
            if (pixelSum <= minIntensity) {
                minIntensity = pixelSum;
                minRow = i-1;
                minCol = j-1;
            }

            pixelSum = src[i-1][j].red + src[i-1][j].green + src[i-1][j].blue;
            if (pixelSum > maxIntensity) {
                maxIntensity = pixelSum;
                maxRow = i-1;
                maxCol = j;
            }
            if (pixelSum <= minIntensity) {
                minIntensity = pixelSum;
                minRow = i-1;
                minCol = j;
            }

            pixelSum = src[i-1][j+1].red + src[i-1][j+1].green + src[i-1][j+1].blue;
            if (pixelSum > maxIntensity) {
                maxIntensity = pixelSum;
                maxRow = i-1;
                maxCol = j+1;
            }
            if (pixelSum <= minIntensity) {
                minIntensity = pixelSum;
                minRow = i-1;
                minCol = j+1;
            }

            pixelSum = src[i][j-1].red + src[i][j-1].green + src[i][j-1].blue;
            if (pixelSum > maxIntensity) {
                maxIntensity = pixelSum;
                maxRow = i;
                maxCol = j-1;
            }
            if (pixelSum <= minIntensity) {
                minIntensity = pixelSum;
                minRow = i;
                minCol = j-1;
            }

            pixelSum = src[i][j].red + src[i][j].green + src[i][j].blue;
            if (pixelSum > maxIntensity) {
                maxIntensity = pixelSum;
                maxRow = i;
                maxCol = j;
            }
            if (pixelSum <= minIntensity) {
                minIntensity = pixelSum;
                minRow = i;
                minCol = j;
            }
            pixelSum = src[i][j+1].red + src[i][j+1].green + src[i][j+1].blue;
            if (pixelSum > maxIntensity) {
                maxIntensity = pixelSum;
                maxRow = i;
                maxCol = j+1;
            }
            if (pixelSum <= minIntensity) {
                minIntensity = pixelSum;
                minRow = i;
                minCol = j+1;
            }
            pixelSum = src[i+1][j-1].red + src[i+1][j-1].green + src[i+1][j-1].blue;
            if (pixelSum > maxIntensity) {
                maxIntensity = pixelSum;
                maxRow = i+1;
                maxCol = j-1;
            }
            if (pixelSum <= minIntensity) {
                minIntensity = pixelSum;
                minRow = i+1;
                minCol = j-1;
            }
            pixelSum = src[i+1][j].red + src[i+1][j].green + src[i+1][j].blue;
            if (pixelSum > maxIntensity) {
                maxIntensity = pixelSum;
                maxRow = i+1;
                maxCol = j;
            }
            if (pixelSum <= minIntensity) {
                minIntensity = pixelSum;
                minRow = i+1;
                minCol = j;
            }
            pixelSum = src[i+1][j+1].red + src[i+1][j+1].green + src[i+1][j+1].blue;
            if (pixelSum > maxIntensity) {
                maxIntensity = pixelSum;
                maxRow = i+1;
                maxCol = j+1;
            }
            if (pixelSum <= minIntensity) {
                minIntensity = pixelSum;
                minRow = i+1;
                minCol = j+1;
            }

            // subtract the min and max pixel value from the the corresponding channel's sum.
            rSum = rSum - src[maxRow][maxCol].red - src[minRow][minCol].red;
            gSum = gSum - src[maxRow][maxCol].green - src[minRow][minCol].green;
            bSum = bSum - src[maxRow][maxCol].blue - src[minRow][minCol].blue;

            // divide each channel's sum by the kernel scale and update the destination image
            dst[i][j].red = rSum/kernelScale;
            dst[i][j].green = gSum/kernelScale;
            dst[i][j].blue = bSum/kernelScale;

            // add back the subtracted values from above (we use a continues sum across the row)
            rSum = rSum + src[maxRow][maxCol].red + src[minRow][minCol].red;
            gSum = gSum + src[maxRow][maxCol].green + src[minRow][minCol].green;
            bSum = bSum + src[maxRow][maxCol].blue + src[minRow][minCol].blue;

        }
    }
}


void doConvolution(Image *img, int option, int kernelScale) {
    int size = m*n*3;

    // create two pixel matrices
    pixel pixelsImg[m][n];
    pixel copy[m][n];

    // copy the source image to both matrices.
    memcpy(pixelsImg, img->data, size);
    memcpy(copy, pixelsImg, size);

    // call the correct convolution according to the given option.
    if (option == 1)
        smooth(m, pixelsImg, copy, kernelScale);

    if (option == 2)
        sharp(m, pixelsImg, copy, kernelScale);

    if (option == 3)
        smoothFilter(m, pixelsImg, copy, kernelScale);

    // copy the finished pixel matrix to the destination image,
    memcpy(img->data, copy, size);
}

void myfunction(Image *img, char* srcImgpName, char* blurRsltImgName, char* sharpRsltImgName, char* filteredBlurRsltImgName, char* filteredSharpRsltImgName, char flag) {
    int smooth = 1, sharp = 2, smoothFilter = 3;

    if (flag == '1') {
        // blur image
        doConvolution(img, smooth, 9);

        // write result image to file
        writeBMP(img, srcImgpName, blurRsltImgName);

        // sharpen the resulting image
        doConvolution(img, sharp, 1);

        // write result image to file
        writeBMP(img, srcImgpName, sharpRsltImgName);
    } else {
        // apply extermum filtered kernel to blur image
        doConvolution(img, smoothFilter, 7);

        // write result image to file
        writeBMP(img, srcImgpName, filteredBlurRsltImgName);

        // sharpen the resulting image
        doConvolution(img, sharp, 1);

        // write result image to file
        writeBMP(img, srcImgpName, filteredSharpRsltImgName);
    }
}
