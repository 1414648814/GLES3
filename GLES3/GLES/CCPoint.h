//
//  CCPoint.h
//  GLES
//
//  Created by staff on 16/12/13.
//  Copyright © 2016年 George1994. All rights reserved.
//

#ifndef CCPoint_h
#define CCPoint_h

#include <stdio.h>
#include "CCVector2.h"
#include "CCVector3.h"
#include "CCVector4.h"
#include "CCMatrix.h"

class CCPoint {
public:
    CCPoint(float x, float y);
    ~CCPoint();
    
    float getX() const;
    float getY() const;
private:
    float _x;
    float _y;
};

class CCSize
{
public:
    /**Width of the Size.*/
    float width;
    /**Height of the Size.*/
    float height;
public:
    /**Conversion from Vec2 to Size.*/
    operator Vector2() const
    {
        return Vector2(width, height);
    }
    
public:
    /**
     @{
     Constructor.
     @param width Width of the size.
     @param height Height of the size.
     @param other Copy constructor.
     @param point Conversion from a point.
     */
    CCSize();
    CCSize(float width, float height);
    CCSize(const CCSize& other);
    explicit CCSize(const Vector2& point);
    /**@}*/
    
    /**
     * @js NA
     * @lua NA
     */
    CCSize& operator= (const CCSize& other);
    /**
     * @js NA
     * @lua NA
     */
    CCSize& operator= (const Vector2& point);
    /**
     * @js NA
     * @lua NA
     */
    CCSize operator+(const CCSize& right) const;
    /**
     * @js NA
     * @lua NA
     */
    CCSize operator-(const CCSize& right) const;
    /**
     * @js NA
     * @lua NA
     */
    CCSize operator*(float a) const;
    /**
     * @js NA
     * @lua NA
     */
    CCSize operator/(float a) const;
    /**
     Set the width and height of Size.
     * @js NA
     * @lua NA
     */
    void setSize(float width, float height);
    /**
     Check if two size is the same.
     * @js NA
     */
    bool equals(const CCSize& target) const;
    /**Size(0,0).*/
    static const CCSize ZERO;
};

/**Rectangle area.*/
class CCRect
{
public:
    /**Low left point of rect.*/
    Vector2 origin;
    /**Width and height of the rect.*/
    CCSize  size;
    
public:
    /**
     Constructor an empty Rect.
     * @js NA
     */
    CCRect();
    /**
     Constructor a rect.
     * @js NA
     */
    CCRect(float x, float y, float width, float height);
    /**
     Constructor a rect.
     * @js NA
     */
    CCRect(const Vector2& pos, const CCSize& dimension);
    /**
     Copy constructor.
     * @js NA
     * @lua NA
     */
    CCRect(const CCRect& other);
    /**
     * @js NA
     * @lua NA
     */
    CCRect& operator= (const CCRect& other);
    /**
     Set the x, y, width and height of Rect.
     * @js NA
     * @lua NA
     */
    void setRect(float x, float y, float width, float height);
    /**
     Get the left of the rect.
     * @js NA
     */
    float getMinX() const; /// return the leftmost x-value of current rect
    /**
     Get the X coordinate of center point.
     * @js NA
     */
    float getMidX() const; /// return the midpoint x-value of current rect
    /**
     Get the right of rect.
     * @js NA
     */
    float getMaxX() const; /// return the rightmost x-value of current rect
    /**
     Get the bottom of rect.
     * @js NA
     */
    float getMinY() const; /// return the bottommost y-value of current rect
    /**
     Get the Y coordinate of center point.
     * @js NA
     */
    float getMidY() const; /// return the midpoint y-value of current rect
    /**
     Get top of rect.
     * @js NA
     */
    float getMaxY() const; /// return the topmost y-value of current rect
    /**
     Compare two rects.
     * @js NA
     */
    bool equals(const CCRect& rect) const;
    /**
     Check if the points is contained in the rect.
     * @js NA
     */
    bool containsPoint(const Vector2& point) const;
    /**
     Check the intersect status of two rects.
     * @js NA
     */
    bool intersectsRect(const CCRect& rect) const;
    /**
     Check the intersect status of the rect and a circle.
     * @js NA
     */
    bool intersectsCircle(const Vector2& center, float radius) const;
    /**
     Get the min rect which can contain this and rect.
     * @js NA
     * @lua NA
     */
    CCRect unionWithRect(const CCRect & rect) const;
    /**Compute the min rect which can contain this and rect, assign it to this.*/
    void merge(const CCRect& rect);
    /**An empty Rect.*/
    static const CCRect ZERO;
};

// 可以用来存储图像数据
class CCData {
public:
    CCData();
    CCData(unsigned char* bytes, ssize_t size);
    ~CCData();
    
    CCData& operator=(const CCData& other);
    CCData& operator=(CCData&& other);
    
    // 移动构造函数
    void move(CCData& data);

    // 拷贝构造函数
    void copy(const unsigned char* bytes, const ssize_t size);
    void fastSet(unsigned char* bytes, const ssize_t size);
    
    unsigned char* getBytes();
    
    ssize_t getSize();
    
protected:
    unsigned char* _bytes;
    ssize_t _size;
};

//class Object {
//public:
//    Object();
//    ~Object();
//};
//
//class Array : public Object {
//public:
//    Array();
//    ~Array();
//    
//    void push(Object*);
//    Object* pop();
//    
//private:
//    
//};

#endif /* CCPoint_h */
