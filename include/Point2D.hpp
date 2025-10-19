/**
 * @file Point2D.hpp
 * @author Tom Gautot (tgautot.off@gmail.com)
 * @brief Simple 2D Point class used thouroughly in ImageBuilder 
 * @version 0.1
 * @date 2023-08-23
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef VIZIOR_POINT2D_HPP
#define VIZIOR_POINT2D_HPP

#include <math.h>

namespace vzr {


    class Point2D {
    public:
        Point2D();
        Point2D(int x, int y);
        Point2D(float x, float y);
        ~Point2D();
        Point2D rotateAroundBy(const Point2D& around, float ang);
        float len();
        float distTo(const Point2D& o);

        Point2D& operator+=(const Point2D& rhs);
        Point2D& operator-=(const Point2D& rhs);
        Point2D& operator*=(int rhs);
        Point2D& operator/=(int rhs);
        Point2D& operator*=(float rhs);
        Point2D& operator/=(float rhs);
        friend Point2D operator+(Point2D lhs, const Point2D& rhs);
        friend Point2D operator-(Point2D lhs, const Point2D& rhs);
        friend Point2D operator*(Point2D lhs, int rhs);
        friend Point2D operator/(Point2D lhs, int rhs);
        friend Point2D operator*(Point2D lhs, float rhs);
        friend Point2D operator/(Point2D lhs, float rhs);
        
        float x,y;    
    };
    extern const Point2D ORIGIN;
}

#endif