#include "Point2D.hpp"

namespace Vizior {
    Point2D::Point2D(): x(0), y(0){}
    Point2D::Point2D(int x, int y) : x(1.0f*x), y(1.0f*y){}
    Point2D::Point2D(float x, float y) : x(x), y(y){}
    Point2D::~Point2D(){}

    Point2D Point2D::rotateAroundBy(const Point2D& around, float rot){
        Point2D diff = *this - around;
        float r = diff.distTo(ORIGIN);
        float ang = atan2(diff.y, diff.x);
        ang += M_PI * rot / 180;
        Point2D rotated = Point2D(
            (cos(ang) * r) + around.x,
            (sin(ang) * r) + around.y
        );
        return rotated;
    }

    float Point2D::len(){
        return sqrt(x*x + y*y);
    }

    float Point2D::distTo(const Point2D& o){
        Point2D diff = *this - o;
        return sqrt(diff.x*diff.x + diff.y*diff.y);
    }

    Point2D& Point2D::operator+=(const Point2D& rhs){
        this->x += rhs.x; 
        this->y += rhs.y;
        return *this;
    }
    Point2D& Point2D::operator-=(const Point2D& rhs){
        this->x -= rhs.x; 
        this->y -= rhs.y;
        return *this;
    }
    Point2D& Point2D::operator*=(int rhs){
        this->x *= rhs; 
        this->y *= rhs;
        return *this;
    }
    Point2D& Point2D::operator/=(int rhs){
        this->x /= rhs; 
        this->y /= rhs;
        return *this;
    }
    Point2D& Point2D::operator*=(float rhs){
        this->x *= rhs; 
        this->y *= rhs;
        return *this;
    }
    Point2D& Point2D::operator/=(float rhs){
        this->x /= rhs; 
        this->y /= rhs;
        return *this;
    }

    Point2D operator+(Point2D lhs, const Point2D& rhs){
        return Point2D(
            lhs.x + rhs.x,
            lhs.y + rhs.y
        );
    }
    Point2D operator-(Point2D lhs, const Point2D& rhs){
        return Point2D(
            lhs.x - rhs.x,
            lhs.y - rhs.y
        );
    }
    Point2D operator*(Point2D lhs, int rhs){
        return Point2D(
            lhs.x * rhs,
            lhs.y * rhs
        );
    }
    Point2D operator/(Point2D lhs, int rhs){
        return Point2D(
            lhs.x / rhs,
            lhs.y / rhs
        );
    }
    Point2D operator*(Point2D lhs, float rhs){
        return Point2D(
            lhs.x * rhs,
            lhs.y * rhs
        );
    }
    Point2D operator/(Point2D lhs, float rhs){
        return Point2D(
            lhs.x / rhs,
            lhs.y / rhs
        );
    }
    

    const Point2D ORIGIN = Point2D();

}