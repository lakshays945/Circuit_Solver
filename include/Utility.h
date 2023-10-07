#pragma once
#include <math.h>
struct Vec2D{
public:
    int x=0, y=0;
    Vec2D(){}
    Vec2D(int x, int y):x(x),y(y){}
    Vec2D operator+(const Vec2D& other){
        return Vec2D(x+other.x,y+other.y);
    }
    Vec2D operator-(const Vec2D& other){
        return Vec2D(x-other.x,y-other.y);
    }
};