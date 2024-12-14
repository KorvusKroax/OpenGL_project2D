#pragma once

const double pi = 3.14159265358979323846; //3.141592653589793238462643383279502884197169399375105820974944;

/*
bool getLineIntersection(float p0_x, float p0_y, float p1_x, float p1_y, float p2_x, float p2_y, float p3_x, float p3_y, float *i_x, float *i_y)
{
    float s1_x = p1_x - p0_x;
    float s1_y = p1_y - p0_y;
    float s2_x = p3_x - p2_x;
    float s2_y = p3_y - p2_y;

    float den = (-s2_x * s1_y + s1_x * s2_y)
    float s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / den;
    float t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / den;

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        *i_x = p0_x + (t * s1_x);
        *i_y = p0_y + (t * s1_y);
        return true;
    }
    return false;
}
*/

char lineIntersection(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float* px = 0, float* py = 0)
{
    float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
    float u = -(((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den);

    *px = x1 + t * (x2 - x1);
    *py = y1 + t * (y2 - y1);

    if (0 <= t && t <= 1 && 0 <= u && u <= 1) {
        return 3; // crossing on both line
    }
    if (0 <= u && u <= 1) {
        return 2; // crossing on second line only
    }
    if (0 <= t && t <= 1) {
        return 1; // crossing on first line only
    }
    return 0; // no crossing
}

bool rayIntersection(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float* px = 0, float* py = 0)
{
    float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
    if (0 <= t && t <= 1) {

        float u = -(((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den);
        if (0 <= u) {
            *px = x1 + t * (x2 - x1);
            *py = y1 + t * (y2 - y1);
            return true; // ray hit the line
        }
    }
    return false; // no crossing
}

void clipBehind(float *x1, float *y1, float x2, float y2)
{
    float d = *y1 - y2; if (d == 0) d = 1;
    float s = *y1 / (*y1 - y2);

    *x1 = *x1 + s * (x2 - (*x1));
    *y1 = *y1 + s * (y2 - (*y1)); if (*y1 == 0) *y1 = 1;
}

int cross(int x1, int y1, int x2, int y2, int px, int py)
{
    return (x2 - x1) * (py - y1) - (y2 - y1) * (px - x1);
    // n > 0: p is left
    // n = 0: p is on line
    // n < 0: p is right
}

bool lineIntersectTriangle(float lx1, float ly1, float lx2, float ly2, float tx1, float ty1, float tx2, float ty2, float tx3, float ty3)
{
    bool l1, l2, t1, t2, t3;

    t1 = cross(lx1, ly1, lx2, ly2, tx1, ty1) >= 0;
    t2 = cross(lx1, ly1, lx2, ly2, tx2, ty2) >= 0;
    t3 = cross(lx1, ly1, lx2, ly2, tx3, ty3) >= 0;
    if (t1 == t2 && t2 == t3) return false;

    l1 = cross(tx1, ty1, tx2, ty2, lx1, ly1) >= 0;
    l2 = cross(tx1, ty1, tx2, ty2, lx2, ly2) >= 0;
    t3 = cross(tx1, ty1, tx2, ty2, tx3, ty3) >= 0;
    if (l1 == l2 && l2 != t3) return false;

    l1 = cross(tx2, ty2, tx3, ty3, lx1, ly1) >= 0;
    l2 = cross(tx2, ty2, tx3, ty3, lx2, ly2) >= 0;
    t1 = cross(tx2, ty2, tx3, ty3, tx1, ty1) >= 0;
    if (l1 == l2 && l2 != t1) return false;

    l1 = cross(tx3, ty3, tx1, ty1, lx1, ly1) >= 0;
    l2 = cross(tx3, ty3, tx1, ty1, lx2, ly2) >= 0;
    t2 = cross(tx3, ty3, tx1, ty1, tx2, ty2) >= 0;
    if (l1 == l2 && l2 != t2) return false;

    return true;
}

bool pointInTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y)//, bool withBias = false)
{
    return
        cross(x1, y1, x2, y2, x, y) <= 0 && // + (!withBias || (y2 - y1 < 0 || (y2 - y1 == 0 && x2 - x1 > 0)) ? 0 : -1) <= 0 &&
        cross(x2, y2, x3, y3, x, y) <= 0 && // + (!withBias || (y3 - y2 < 0 || (y3 - y2 == 0 && x3 - x2 > 0)) ? 0 : -1) <= 0 &&
        cross(x3, y3, x1, y1, x, y) <= 0;   // + (!withBias || (y1 - y3 < 0 || (y1 - y3 == 0 && x1 - x3 > 0)) ? 0 : -1) <= 0;
}
