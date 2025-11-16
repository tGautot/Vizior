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
        Point2D(const Point2D& p);
        Point2D(int x, int y);
        Point2D(double x, double y);
        ~Point2D();
        void rotateAroundBy(const Point2D& around, double ang);
        Point2D rotatedAroundBy(const Point2D& around, double ang);
        void rotateBy(double ang);
        Point2D rotatedBy(double ang);
        void normalize();
        Point2D normalized();
        double length();
        double lengthSquared();
        double distTo(const Point2D& o);

        Point2D& operator+=(const Point2D& rhs);
        Point2D& operator-=(const Point2D& rhs);
        Point2D& operator*=(int rhs);
        Point2D& operator/=(int rhs);
        Point2D& operator*=(double rhs);
        Point2D& operator/=(double rhs);
        friend Point2D operator-(const Point2D& p);
        friend Point2D operator+(const Point2D& lhs, const Point2D& rhs);
        friend Point2D operator-(const Point2D& lhs, const Point2D& rhs);
        friend Point2D operator*(const Point2D& lhs, int rhs);
        friend Point2D operator/(const Point2D& lhs, int rhs);
        friend Point2D operator*(const Point2D& lhs, double rhs);
        friend Point2D operator/(const Point2D& lhs, double rhs);
        friend Point2D operator*(int rhs, const Point2D& lhs);
        friend Point2D operator/(int rhs, const Point2D& lhs);
        friend Point2D operator*(double rhs, const Point2D& lhs);
        friend Point2D operator/(double rhs, const Point2D& lhs);
        
        double x,y;    
    };
    
    extern const Point2D ORIGIN;

    typedef Point2D Vec2;
}



#endif