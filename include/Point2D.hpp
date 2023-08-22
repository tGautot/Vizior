#ifndef VIZIOR_POINT2D_HPP
#define VIZIOR_POINT2D_HPP

#include <math.h>

namespace Vizior {


    class Point2D {
    public:
        Point2D();
        Point2D(int x, int y);
        ~Point2D();
        Point2D rotateAroundBy(const Point2D& around, float ang);
        float distTo(const Point2D& o);

        Point2D& operator+=(const Point2D& rhs);
        Point2D& operator-=(const Point2D& rhs);
        Point2D& operator*=(int rhs);
        Point2D& operator/=(int rhs);
        friend Point2D operator+(Point2D lhs, const Point2D& rhs);
        friend Point2D operator-(Point2D lhs, const Point2D& rhs);
        friend Point2D operator*(Point2D lhs, int rhs);
        friend Point2D operator/(Point2D lhs, int rhs);
        
        int x,y;    
    };
    extern const Point2D ORIGIN;
}

#endif