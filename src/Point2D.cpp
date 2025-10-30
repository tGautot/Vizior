#include "Point2D.hpp"

namespace vzr {
    Point2D::Point2D(): x(0), y(0){}
    Point2D::Point2D(const Point2D& p): x(p.x), y(p.y){}
    Point2D::Point2D(int x, int y) : x(1.0f*x), y(1.0f*y){}
    Point2D::Point2D(double x, double y) : x(x), y(y){}
    Point2D::~Point2D(){}


    void Point2D::rotateAroundBy(const Point2D& around, double radians){
        Point2D diff = *this - around;
        double r = diff.distTo(ORIGIN);
        double ang = std::atan2(diff.y, diff.x);
        ang += radians;
        
        x = (std::cos(ang) * r) + around.x;
        y = (std::sin(ang) * r) + around.y;
    }

    Point2D Point2D::rotatedAroundBy(const Point2D& around, double radians){
        Point2D newPoint = *this;
        newPoint.rotateAroundBy(around, radians);
        return newPoint;
    }

    double Point2D::len(){
        return std::sqrt(x*x + y*y);
    }

    double Point2D::distTo(const Point2D& o){
        Point2D diff = *this - o;
        return std::sqrt(diff.x*diff.x + diff.y*diff.y);
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
    Point2D& Point2D::operator*=(double rhs){
        this->x *= rhs; 
        this->y *= rhs;
        return *this;
    }
    Point2D& Point2D::operator/=(double rhs){
        this->x /= rhs; 
        this->y /= rhs;
        return *this;
    }

    Point2D operator+(const Point2D& lhs, const Point2D& rhs){
        return Point2D(
            lhs.x + rhs.x,
            lhs.y + rhs.y
        );
    }
    Point2D operator-(const Point2D& lhs, const Point2D& rhs){
        return Point2D(
            lhs.x - rhs.x,
            lhs.y - rhs.y
        );
    }
    Point2D operator*(const Point2D& lhs, int rhs){
        return Point2D(
            lhs.x * rhs,
            lhs.y * rhs
        );
    }
    Point2D operator/(const Point2D& lhs, int rhs){
        return Point2D(
            lhs.x / rhs,
            lhs.y / rhs
        );
    }
    Point2D operator*(const Point2D& lhs, double rhs){
        return Point2D(
            lhs.x * rhs,
            lhs.y * rhs
        );
    }
    Point2D operator/(const Point2D& lhs, double rhs){
        return Point2D(
            lhs.x / rhs,
            lhs.y / rhs
        );
    }
    

    const Point2D ORIGIN = {0,0};

}