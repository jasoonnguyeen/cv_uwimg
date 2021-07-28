#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    int channel_step = im.w * im.h * c;

    int l, k;

    if (fabs(ceilf(x) - x) <= fabs(floorf(x) - x))
        l = ceilf(x);
    else
        l = floorf(x);

    if (fabs(ceilf(y) - y) <= fabs(floorf(y) - y))
        k = ceilf(y);
    else
        k = floorf(y);

    if (l < 0)  l = 0;
    if (k < 0)  k = 0;

    if (l > im.w)   l = im.w - 1;
    if (k > im.h)   k = im.h - 1;

    return im.data[k * im.w + l + channel_step];
}

image nn_resize(image im, int w, int h)
{
    image result = make_image(w, h, im.c);

    float ratio_w, ratio_h, nnX, nnY;

    if (im.w <= w)
        ratio_w = (float)im.w / w;
    else
        ratio_w = (float)(im.w - 1) / w;

    if (im.h <= h)
        ratio_h = (float)im.h / h;
    else
        ratio_h = (float)(im.h - 1) / h;

    for (int k = 0; k < result.c; k++)
        for (int j = 0; j < result.h; j++)
            for (int i = 0; i < result.w; i++)
            {
                nnX = floorf(i * ratio_w);
                nnY = floorf(j * ratio_h);

                result.data[j * result.w + i + result.w * result.h * k] = nn_interpolate(im, nnX, nnY, k);
            }

    //save_png(result, "results/nn_resize_result");

    return result;
}

// ========================================
//      Bilinear Interpolate Formula
// ========================================

// z00 is the function value at (0,0), the lower left corner
// z10 is the function value at (1,0), the lower right corner
// z01 is the function value at (0,1), the upper left corner
// z11 is the function value at (1,1), the upper right corner
// If (x,y) is any point inside the unit square, the interpolation at that point is the following weighted average of the values at the four corners:
// F(x,y) = z00*(1-x)*(1-y) + z10*x*(1-y) + z01*(1-x)*y + z11*x*y

float bilinear_interpolate(image im, float x, float y, int c)
{
    int channel_step = im.w * im.h * c;

    float z00, z10, z01, z11;

    int l, k, a, b;

    if (fabs(ceilf(x) - x) <= fabs(floorf(x) - x))
        l = ceilf(x);
    else
        l = floorf(x);

    if (fabs(ceilf(y) - y) <= fabs(floorf(y) - y))
        k = ceil(y);
    else
        k = floorf(y);

    if (l < 0) l = 0;
    if (k < 0) k = 0;

    if (l > im.w)   l = im.w - 1;
    if (k > im.h)   k = im.h - 1;

    a = x - l;
    b = y - k;
 
    z00 = im.data[k * im.w + l + channel_step];
    z10 = im.data[k * im.w + (l + 1) + channel_step];
    z01 = im.data[(k + 1) * im.w + l + channel_step];
    z11 = im.data[(k + 1) * im.w + (l + 1) + channel_step];

    return z00 * (1 - a) * (1 - b) + z10 * a * (1 - b) + z01 * (1 - a) * b + z11 * a * b;
}

image bilinear_resize(image im, int w, int h)
{
    float ratio_w, ratio_h;

    image result = make_image(w, h, im.c);

    if (im.w <= w)
        ratio_w = (float)im.w / w;
    else
        ratio_w = (float)(im.w - 1) / w;

    if (im.h <= h)
        ratio_h = (float)im.h / h;
    else
        ratio_h = (float)(im.h - 1) / h;

    for (int k = 0; k < result.c; k++)
        for (int j = 0; j < result.h; j++)
            for (int i = 0; i < result.w; i++)
            {
                float nnX = floorf(i * ratio_w);
                float nnY = floorf(j * ratio_h);

                result.data[j * result.w + i + result.w * result.h * k] = bilinear_interpolate(im, nnX, nnY, k);
            }

    //save_png(result, "results/bilinear_resize_result");

    return result;
}
