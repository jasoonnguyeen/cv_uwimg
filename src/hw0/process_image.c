#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    int step = im.w * im.h * c;
    if (x < 0 && y < im.h && c < im.c)
    {
        x = 0;
        return im.data[y * im.w + x + step];
    }
    else if (x < im.w && x >= 0 && y < 0 && c < im.c)
    {
        y = 0;
        return im.data[y * im.w + x + step];
    }
    else if (x < im.w && x >= 0 && y > im.h && c < im.c)
    {
        y = im.h - 1;
        return im.data[y * im.w + x + step];
    }
    else if (x > im.w && x >= 0 && y < im.h && c < im.c)
    {
        x = im.w - 1;
        return im.data[y * im.w + x + step];
    }
    else if (x > im.w && y > im.h && c < im.c)
    {
        y = im.h - 1;
        x = im.w - 1;
        return im.data[y * im.w + x + step];
    }
    else if (x < im.w && y < im.h && c < im.c)
    {
        return im.data[y * im.w + x + step];
    }
    else
    {
        return 0;
    }
}

void set_pixel(image im, int x, int y, int c, float v)
{
    if (x < im.w && y < im.h && c < im.c)
    {
        int step = im.w * im.h * c;
        im.data[y * im.w + x + step] = v;
    }
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);

    int idx;

    for (int k = 0; k < im.c; ++k)
        for (int j = 0; j < im.h; ++j)
        {
            for (int i = 0; i < im.w; ++i)
            {
                idx = j * im.w + i + im.w * im.h * k;
                copy.data[idx] = im.data[idx];
            }
        }

    // save_image(copy, "results/copy_image_result");

    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);

    image gray = make_image(im.w, im.h, 1);

    int gray_idx, color_idx_r, color_idx_g, color_idx_b;

    for (int j = 0; j < im.h; ++j)
        for (int i = 0; i < im.w; ++i)
        {
            gray_idx = j * im.w + i;

            color_idx_r = j * im.w + i + im.w * im.h * 0;
            color_idx_g = j * im.w + i + im.w * im.h * 1;
            color_idx_b = j * im.w + i + im.w * im.h * 2;

            gray.data[gray_idx] = 0.299 * im.data[color_idx_r] + 0.587 * im.data[color_idx_g] + 0.114 * im.data[color_idx_b];
        }

    // save_image(gray, "results/rgb_to_grayscale_result");

    return gray;
}

void shift_image(image im, int c, float v)
{
    assert(im.c == 3);

    int color_idx;

    for (int j = 0; j < im.h; ++j)
        for (int i = 0; i < im.w; ++i)
        {
            color_idx = j * im.w + i + im.w * im.h * c;

            im.data[color_idx] = im.data[color_idx] + v;
        }

    // save_image(im, "results/shift_image_result");
}

void clamp_image(image im)
{
    assert(im.c == 3);

    int idx;

    for (int k = 0; k < im.c; ++k)
        for (int j = 0; j < im.h; ++j)
        {
            for (int i = 0; i < im.w; ++i)
            {
                idx = j * im.w + i + im.w * im.h * k;

                if (im.data[idx] > 1)
                    im.data[idx] = 1;
                else if (im.data[idx] < 0)
                    im.data[idx] = 0;
            }
        }
}

// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c);
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ((a < c) ? a : c) : ((b < c) ? b : c);
}

void rgb_to_hsv(image im)
{
    int color_idx_1st, color_idx_2nd, color_idx_3rd;
    float R, G, B;
    float H, _H, S, V, C, m;

    assert(im.c == 3);

    for (int j = 0; j < im.h; ++j)
    {
        for (int i = 0; i < im.w; ++i)
        {
            // indexes
            color_idx_1st = j * im.w + i + im.w * im.h * 0;
            color_idx_2nd = j * im.w + i + im.w * im.h * 1;
            color_idx_3rd = j * im.w + i + im.w * im.h * 2;

            // R, G, B
            R = im.data[color_idx_1st];
            G = im.data[color_idx_2nd];
            B = im.data[color_idx_3rd];

            // H, S, V
            V = three_way_max(R, G, B);
            m = three_way_min(R, G, B);

            C = V - m;

            if (R == 0 && G == 0 && B == 0)
            {
                V = 0;
                S = 0;
                C = 0;
            }
            else
            {
                S = C / V;
            }

            if (C == 0)
            {
                H = 0;
            }
            else
            {
                if (V == R)
                {
                    _H = ((G - B) / C) + 0;
                }
                else if (V == G)
                {
                    _H = ((B - R) / C) + 2;
                }
                else if (V == B)
                {
                    _H = ((R - G) / C) + 4;
                }

                if (_H < 0)
                {
                    H = (_H / 6) + 1;
                }
                else
                {
                    H = _H / 6;
                }
            }

            // RGB -> HSV
            im.data[color_idx_1st] = H;
            im.data[color_idx_2nd] = S;
            im.data[color_idx_3rd] = V;
        }
    }

    // save_png(im, "results/rgb_to_hsv_result");
}

void hsv_to_rgb(image im)
{
    int color_idx_1st, color_idx_2nd, color_idx_3rd;
    float R, G, B;
    float H, S, V, C, m;
    float _H;
    float X1, X2, X3, X;

    assert(im.c == 3);

    for (int j = 0; j < im.h; ++j)
    {
        for (int i = 0; i < im.w; ++i)
        {
            // indexes
            color_idx_1st = j * im.w + i + im.w * im.h * 0;
            color_idx_2nd = j * im.w + i + im.w * im.h * 1;
            color_idx_3rd = j * im.w + i + im.w * im.h * 2;

            // H, S, V
            H = im.data[color_idx_1st];
            S = im.data[color_idx_2nd];
            V = im.data[color_idx_3rd]; // max(R,G,B)

            C = S * V;

            if (S == 0 && V == 0 && H == 0)
            {
                C = 0;
                R = 0;
                G = 0;
                B = 0;

                m = 0;
            }

            if (C != 0)
            {
                m = V - C; // m: min(R,G,B), V: max(R,G,B)

                _H = H * 6;

                if (_H > 0 && _H <= 1)
                {
                    R = V;
                    G = C * _H + m;
                    B = m;
                }
                else if (_H > 1 && _H <= 2)
                {
                    R = C * _H + m;
                    G = V;
                    B = m;
                }
                else if (_H > 2 && _H <= 3)
                {
                    R = m;
                    G = V;
                    B = C * (_H - 2) + m;
                }
                else if (_H > 3 && _H <= 4)
                {
                    R = m;
                    G = C * (_H - 2) + m;
                    B = V;
                }
                else if (_H > 4 && _H <= 5)
                {
                    R = C * (_H - 4) + m;
                    G = m;
                    B = V;
                }
                else if (_H > 5 && _H <= 6)
                {
                    R = V;
                    G = m;
                    B = C * (_H - 4) + m;
                }
            }

            // RGB -> HSV
            im.data[color_idx_1st] = R;
            im.data[color_idx_2nd] = G;
            im.data[color_idx_3rd] = B;
        }
    }

    //save_png(im, "results/hsv_to_rgb_result");
}
