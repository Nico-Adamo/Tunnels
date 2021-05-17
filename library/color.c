#include "color.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double color_get_c(double saturation, double lightness) {
    return ((1 - fabs(2 * lightness - 1)) * saturation);
}

double color_get_x(double hue, double c) {
    return (c * (1- fabs(fmod(hue / 60.0, 2) - 1)));
}

double color_get_m(double lightness, double c) {
    return (lightness - c/2.0);
}

rgb_color_t color_convert_hsl_to_rgb(hsl_color_t hsl_col) {

    rgb_color_t rgb_col;

    while (hsl_col.h >= 360) {
        hsl_col.h -= 360;
    }

    double c = color_get_c(hsl_col.s, hsl_col.l);
    double x = color_get_x(hsl_col.h, c);
    double m = color_get_m(hsl_col.l, c);

    if (0 <= hsl_col.h && hsl_col.h < 60) {
        rgb_col.r = c;
        rgb_col.g = x;
        rgb_col.b = 0;
    } else if (60 <= hsl_col.h && hsl_col.h < 120) {
        rgb_col.r = x;
        rgb_col.g = c;
        rgb_col.b = 0;
    } else if (120 <= hsl_col.h && hsl_col.h < 180) {
        rgb_col.r = 0;
        rgb_col.g = c;
        rgb_col.b = x;
    } else if (180 <= hsl_col.h && hsl_col.h < 240) {
        rgb_col.r = 0;
        rgb_col.g = x;
        rgb_col.b = c;
    } else if (240 <= hsl_col.h && hsl_col.h < 300) {
        rgb_col.r = x;
        rgb_col.g = 0;
        rgb_col.b = c;
    } else if (300 <= hsl_col.h && hsl_col.h < 360) {
        rgb_col.r = c;
        rgb_col.g = 0;
        rgb_col.b = x;
    }

    rgb_col.r = (rgb_col.r + m);
    rgb_col.g = (rgb_col.g + m);
    rgb_col.b = (rgb_col.b + m);

    return rgb_col;
}

hsl_color_t color_convert_rgb_to_hsl(rgb_color_t rgb_col) {

    double r = rgb_col.r;
    double g = rgb_col.g;
    double b = rgb_col.b;

    double c_max = fmax(fmax(r, g), b);
    double c_min = fmin(fmin(r, g), b);
    double delta = c_max - c_min;

    hsl_color_t hsl_col;

    // Hue calculation

    if (delta == 0.0) {
        hsl_col.h = 0;
    } else if (c_max == r) {
        hsl_col.h = (g - b)/delta;
    } else if (c_max == g) {
        hsl_col.h =((b - r)/delta + 2.0);
    } else {
        hsl_col.h = ((r - g)/delta + 4.0);
    }
    hsl_col.h *= 60.0;
    if (hsl_col.h < 0) {hsl_col.h += 360.0;}

    // Lightness calculation
    hsl_col.l = (c_min + c_max) / 2.0;

    // Saturation calculation
    if (delta == 0.0) {
        hsl_col.s = 0.0;
    } else {
        hsl_col.s = delta / (1 - fabs((c_min + c_max) - 1));
    }

    return hsl_col;

}
