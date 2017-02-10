//
//  CCPoint.cpp
//  GLES3
//
//  Created by staff on 17/2/3.
//  Copyright © 2017年 George. All rights reserved.
//

#include "CCPoint.h"
#include <string>
#include <vector>

CCPoint::CCPoint(float x, float y) {
    _x = x;
    _y = y;
}

CCPoint::~CCPoint() {
    
}

float CCPoint::getX() const {
    return _x;
}

float CCPoint::getY() const {
    return _y;
}

CCSize::CCSize(void) : width(0), height(0)
{
}

CCSize::CCSize(float w, float h) : width(w), height(h)
{
}

CCSize::CCSize(const CCSize& other) : width(other.width), height(other.height)
{
}

CCSize::CCSize(const Vector2& point) : width(point.x), height(point.y)
{
}

CCSize& CCSize::operator= (const CCSize& other)
{
    setSize(other.width, other.height);
    return *this;
}

CCSize& CCSize::operator= (const Vector2& point)
{
    setSize(point.x, point.y);
    return *this;
}

CCSize CCSize::operator+(const CCSize& right) const
{
    return CCSize(this->width + right.width, this->height + right.height);
}

CCSize CCSize::operator-(const CCSize& right) const
{
    return CCSize(this->width - right.width, this->height - right.height);
}

CCSize CCSize::operator*(float a) const
{
    return CCSize(this->width * a, this->height * a);
}

CCSize CCSize::operator/(float a) const
{
    assert(a!=0);
    return CCSize(this->width / a, this->height / a);
}

void CCSize::setSize(float w, float h)
{
    this->width = w;
    this->height = h;
}

bool CCSize::equals(const CCSize& target) const
{
    return (fabs(this->width  - target.width)  < __FLT_EPSILON__)
    && (fabs(this->height - target.height) < __FLT_EPSILON__);
}

const CCSize CCSize::ZERO = CCSize(0, 0);

// implementation of CCRect

CCRect::CCRect(void)
{
    setRect(0.0f, 0.0f, 0.0f, 0.0f);
}

CCRect::CCRect(float x, float y, float width, float height)
{
    setRect(x, y, width, height);
}
CCRect::CCRect(const Vector2& pos, const CCSize& dimension)
{
    setRect(pos.x, pos.y, dimension.width, dimension.height);
}

CCRect::CCRect(const CCRect& other)
{
    setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
}

CCRect& CCRect::operator= (const CCRect& other)
{
    setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
    return *this;
}

void CCRect::setRect(float x, float y, float width, float height)
{
    // CGCCRect can support width<0 or height<0
    // CCASSERT(width >= 0.0f && height >= 0.0f, "width and height of CCRect must not less than 0.");
    
    origin.x = x;
    origin.y = y;
    
    size.width = width;
    size.height = height;
}

bool CCRect::equals(const CCRect& rect) const
{
    return (origin==rect.origin) &&
            (size.equals(rect.size));
}

float CCRect::getMaxX() const
{
    return origin.x + size.width;
}

float CCRect::getMidX() const
{
    return origin.x + size.width / 2.0f;
}

float CCRect::getMinX() const
{
    return origin.x;
}

float CCRect::getMaxY() const
{
    return origin.y + size.height;
}

float CCRect::getMidY() const
{
    return origin.y + size.height / 2.0f;
}

float CCRect::getMinY() const
{
    return origin.y;
}

bool CCRect::containsPoint(const Vector2& point) const
{
    bool bRet = false;
    
    if (point.x >= getMinX() && point.x <= getMaxX()
        && point.y >= getMinY() && point.y <= getMaxY())
    {
        bRet = true;
    }
    
    return bRet;
}

bool CCRect::intersectsRect(const CCRect& rect) const
{
    return !(     getMaxX() < rect.getMinX() ||
             rect.getMaxX() <      getMinX() ||
             getMaxY() < rect.getMinY() ||
             rect.getMaxY() <      getMinY());
}

bool CCRect::intersectsCircle(const Vector2 &center, float radius) const
{
    Vector2 CCRectangleCenter((origin.x + size.width / 2),
                         (origin.y + size.height / 2));
    
    float w = size.width / 2;
    float h = size.height / 2;
    
    float dx = fabs(center.x - CCRectangleCenter.x);
    float dy = fabs(center.y - CCRectangleCenter.y);
    
    if (dx > (radius + w) || dy > (radius + h))
    {
        return false;
    }
    
    Vector2 circleDistance(fabs(center.x - origin.x - w),
                        fabs(center.y - origin.y - h));
    
    if (circleDistance.x <= (w))
    {
        return true;
    }
    
    if (circleDistance.y <= (h))
    {
        return true;
    }
    
    float cornerDistanceSq = powf(circleDistance.x - w, 2) + powf(circleDistance.y - h, 2);
    
    return (cornerDistanceSq <= (powf(radius, 2)));
}

void CCRect::merge(const CCRect& CCRect)
{
    float minX = std::min(getMinX(), CCRect.getMinX());
    float minY = std::min(getMinY(), CCRect.getMinY());
    float maxX = std::max(getMaxX(), CCRect.getMaxX());
    float maxY = std::max(getMaxY(), CCRect.getMaxY());
    setRect(minX, minY, maxX - minX, maxY - minY);
}

CCRect CCRect::unionWithRect(const CCRect & rect) const
{
    float thisLeftX = origin.x;
    float thisRightX = origin.x + size.width;
    float thisTopY = origin.y + size.height;
    float thisBottomY = origin.y;
    
    if (thisRightX < thisLeftX)
    {
        std::swap(thisRightX, thisLeftX);   // This CCRect has negative width
    }
    
    if (thisTopY < thisBottomY)
    {
        std::swap(thisTopY, thisBottomY);   // This CCRect has negative height
    }
    
    float otherLeftX = rect.origin.x;
    float otherRightX = rect.origin.x + rect.size.width;
    float otherTopY = rect.origin.y + rect.size.height;
    float otherBottomY = rect.origin.y;
    
    if (otherRightX < otherLeftX)
    {
        std::swap(otherRightX, otherLeftX);   // Other CCRect has negative width
    }
    
    if (otherTopY < otherBottomY)
    {
        std::swap(otherTopY, otherBottomY);   // Other CCRect has negative height
    }
    
    float combinedLeftX = std::min(thisLeftX, otherLeftX);
    float combinedRightX = std::max(thisRightX, otherRightX);
    float combinedTopY = std::max(thisTopY, otherTopY);
    float combinedBottomY = std::min(thisBottomY, otherBottomY);
    
    return CCRect(combinedLeftX, combinedBottomY, combinedRightX - combinedLeftX, combinedTopY - combinedBottomY);
}

const CCRect CCRect::ZERO = CCRect(0, 0, 0, 0);

CCData::CCData() {
    
}

CCData::CCData(unsigned char* bytes, ssize_t size) {
    _bytes = bytes;
    _size = size;
}

CCData::~CCData() {
    delete _bytes;
    _bytes = NULL;
    _size = 0;
}

CCData& CCData::operator=(const CCData& other) {
    copy(other._bytes, other._size);
    return *this;
}

CCData& CCData::operator=(CCData&& other) {
    move(other);
    return *this;
}

// 移动构造函数
void CCData::move(CCData& data) {
    _bytes = data._bytes;
    _size = data._size;
    
    data._bytes = NULL;
    data._size = 0;
}

// 拷贝构造函数
void CCData::copy(const unsigned char* bytes, const ssize_t size) {
    if (size > 0) {
        _size = size;
        _bytes = (unsigned char*)malloc(sizeof(unsigned char*) * size);
        memcpy(_bytes, bytes, _size);
    }
}

void CCData::fastSet(unsigned char* bytes, const ssize_t size)
{
    _bytes = bytes;
    _size = size;
}


unsigned char* CCData::getBytes() {
    return _bytes;
}

ssize_t CCData::getSize() {
    return _size;
}

