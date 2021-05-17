#ifndef __COLOR_H__
#define __COLOR_H__

/**
 * A color to display on the screen.
 * The color is represented by its red, green, and blue components.
 * Each component must be between 0 (black) and 1 (white).
 */
typedef struct {
    float r;
    float g;
    float b;
} rgb_color_t;

/**
 * A color to display on the screen.
 * The color is represented by its hue, saturation, and lightness components.
 * Hue is represented as an angle in degrees.
 * Saturation and lightness components must be between 0 and 1.
 */
typedef struct {
    double h;
    double s;
    double l;
} hsl_color_t;

/**
 * Converts HSL color to RGB.
 *
 * @param hsl_col the color represented in HSL as type hsl_color_t.
 * @return the same color represented in RGB as type rgb_color_t.
 */
rgb_color_t color_convert_hsl_to_rgb(hsl_color_t hsl_col);

/**
 * Converts RGB color to HSL.
 *
 * @param rgb_col the color represented in RGB as type rgb_color_t.
 * @return the same color represented in HSL as type hsl_color_t.
 */
hsl_color_t color_convert_rgb_to_hsl(rgb_color_t rgb_col);

#endif // #ifndef __COLOR_H__
