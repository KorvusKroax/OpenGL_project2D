#pragma once

#include <color.h>

#include <cstring>
#include <math.h>

class Canvas
{
    public:
        unsigned int width;
        unsigned int height;
        int* pixels;

        Canvas();
        Canvas(unsigned int width, unsigned int height);
        ~Canvas();

        void clearCanvas();
        void fillCanvas(Color color);
        void setPixel(int x, int y, Color color);
        bool getPixel(int x, int y, Color* color = 0);
        void setPixels(int x, int y, Canvas* canvas);
        bool getPixels(int x, int y, unsigned int w, unsigned int h, Canvas* canvas);
        void drawAALine(int x1, int y1, int x2, int y2, Color color);
        void drawLine(int x1, int y1, int x2, int y2, Color color);
        void drawBezierCurve_quadratic(int x1, int y1, int xc, int yc, int x2, int y2, float resolution, Color color);
        void drawBezierCurve_cubic(int x1, int y1, int x1c, int y1c, int x2c, int y2c, int x2, int y2, float resolution, Color color);
        void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, Color color);
        void drawFilledTriangle(int x1, int y1, int x2, int y2, int x3, int y3, Color color);//, bool withBias = false)
        void drawRectangle(int x, int y, int w, int h, Color color);
        void drawFilledRectangle(int x, int y, int w, int h, Color color);
        void drawMidpointCircle(int cx, int cy, int r, Color color);
        void drawCircle(int cx, int cy, int r, Color color);
        void drawFilledCircle(int cx, int cy, int r, Color color);
        void floodFill(int x, int y, Color color);
        void spanFill(int x, int y, Color color);
};
