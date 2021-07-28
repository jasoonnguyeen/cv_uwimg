#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"

#define TWOPI 6.2831853

void l1_normalize(image im)
{
    // normalize
    for (int k = 0; k < im.c; ++k)
        for (int j = 0; j < im.h; ++j)
            for (int i = 0; i < im.w; ++i)
            {
                im.data[(j * im.w) + i + (im.w * im.h * k)] = 
                (float)im.data[(j * im.w) + i + (im.w * im.h * k)] / (float)(im.h*im.w);
            }
}

image make_box_filter(int w)
{
    image result = make_image(w, w, 1);

    // fill in 1
    for (int i = 0; i < w; ++i)
        for (int j = 0; j < w; ++j)
            result.data[i * w + j] = 1.0f;

    // normalize
    l1_normalize(result);

    return result;
}

image convolve_image(image im, image filter, int preserve)
{
    image result;

    // case 1 - filter and im have the same number of channels
    if (im.c == filter.c && preserve != 1)
    {
        result = make_image(im.w, im.h, 1);
    }
    else if (preserve == 1) // case 2 - preserve is set to 1
    {
        result = make_image(im.w, im.h, im.c);
    }

    // get values from filter
    int centerW, centerH, idxW, idxH;

    centerW = filter.w / 2;
    centerH = filter.h / 2;

    for (int z = 0; z < result.c; ++z)
        for (int j = 0; j < result.h; ++j)
            for (int i = 0; i < result.w; ++i)
            {
                if (filter.c == 1)
                {
                    float sum = 0.0f, valueOfImg = 0.0f;

                    for (int g = -centerH; g <= centerH; ++g)
                        for (int h = -centerW; h <= centerW; ++h)
                        {
                            if (i + h < 0 || j + g < 0 || i + h > im.w || j + g > im.h)
                                valueOfImg = 0.0f;
                            else
                                valueOfImg = im.data[(j + g) * im.w + (i + h) + (im.h * im.w * z)];

                            idxW = h + centerW;
                            idxH = g + centerH;

                            sum += filter.data[idxH * filter.w + idxW] * valueOfImg;
                        }

                    result.data[j * result.w + i + (result.h * result.w * z)] = sum;
                }
                else
                {
                    //TODO
                }
            }

    return result;
}

image make_highpass_filter()
{
    image kernel = make_image(3, 3, 1);

    kernel.data[0] = 0.0f;  kernel.data[1] = -1.0f; kernel.data[2] = 0.0f;
    kernel.data[3] = -1.0f; kernel.data[4] = 4.0f;  kernel.data[5] = -1.0f;
    kernel.data[6] = 0.0f;  kernel.data[7] = -1.0f; kernel.data[8] = 0.0f;

    // normalize
    l1_normalize(kernel);

    return kernel;
}

image make_sharpen_filter()
{
    image kernel = make_image(3, 3, 1);

    kernel.data[0] = 0.0f;  kernel.data[1] = -1.0f; kernel.data[2] = 0.0f;
    kernel.data[3] = -1.0f; kernel.data[4] = 5.0f;  kernel.data[5] = -1.0f;
    kernel.data[6] = 0.0f;  kernel.data[7] = -1.0f; kernel.data[8] = 0.0f;

    // normalize
    l1_normalize(kernel);

    return kernel;
}

image make_emboss_filter()
{
    image kernel = make_image(3, 3, 1);

    kernel.data[0] = -2.0f; kernel.data[1] = -1.0f; kernel.data[2] = 0.0f;
    kernel.data[3] = -1.0f; kernel.data[4] = 1.0f;  kernel.data[5] = -1.0f;
    kernel.data[6] = 0.0f;  kernel.data[7] = 1.0f;  kernel.data[8] = 2.0f;

    // normalize
    l1_normalize(kernel);

    return kernel;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: TODO

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO

image make_gaussian_filter(float sigma)
{
    // TODO
    return make_image(1, 1, 1);
}

image add_image(image a, image b)
{
    // TODO
    return make_image(1, 1, 1);
}

image sub_image(image a, image b)
{
    // TODO
    return make_image(1, 1, 1);
}

image make_gx_filter()
{
    // TODO
    return make_image(1, 1, 1);
}

image make_gy_filter()
{
    // TODO
    return make_image(1, 1, 1);
}

void feature_normalize(image im)
{
    // TODO
}

image *sobel_image(image im)
{
    // TODO
    return calloc(2, sizeof(image));
}

image colorize_sobel(image im)
{
    // TODO
    return make_image(1, 1, 1);
}
