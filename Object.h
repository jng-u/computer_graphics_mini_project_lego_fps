#ifndef __OBJECT_H__
#define __OBJECT_H__
#include "Header.h"

class Object{
public:
    Object();
    ~Object();
    // setter
    void setVelocity(const float &x, const float &y, const float &z=0);
    void setPosition(const Vec3f &position);
    void setPosition(const float &x, const float &y, const float &z=0);
    void setWidth(const Vec3f &width);
    void setWidth(const float &w, const float &h, const float &d=0);
    void setColor(const Vec3f &color, const float &a);
    void setColor(const float &r, const float &g, const float &b, const float &a);
    void setActivate(const bool &activate_flag);
    // getter
    Vec3f getVelocity(){ return velocity; }
    Vec3f getPosition(){ return position; }
    Vec3f getWidth(){ return width; }
    Vec3f getColor(){ return color; }
    float getAlpha(){ return alpha; }
    bool isActivate(){ return activateFlag; }
    // operation
    virtual void draw()=0;
    void move();
    void move(const vector<Object*> *objList);
    bool isCollision(Object *obj);

protected:
    Vec3f velocity;         // 중력, 이동을 위한 velocity값
    Vec3f position;         // 좌표
    Vec3f width;            // 길이 x, y, z축 방향
    Vec3f color;            // object color 설정
    float alpha;            // object alpha 값
    bool activateFlag;      // 활성화/비활성화
};
#endif