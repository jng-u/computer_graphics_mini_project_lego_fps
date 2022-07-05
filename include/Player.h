#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "Header.h"
#include "ObjParser.h"
#include "Object.h"

class Bullet;

class Player : public Object {
public:
    Player();
    ~Player();

    void setAngle(const float &angle);    

    float getAngle() { return theta; }

    void setCamera();
    void setCameraAngle(const float &theta, const float &phi);
    void rotate(const float &theta, const float &phi);
    
    void draw();
    void draw(const Vec3f &color, const float &a);
    void draw(const Vec3f &position, const Vec3f &color, const float &a);
    
    bool onMouseCheck(float x, float y);



private:
    float angle;                    // object 각도 (y축)

    float theta, phi;               //camera 변수

    Bullet* bullet;
    vector<Bullet*> *bullets;
};

class Bullet : public Object {
public:
    Bullet(){
        width = {2, 2, 2};
    }
    ~Bullet(){}

    void move(const vector<Object*> *objList){
        position.x += velocity.x;
        position.y += velocity.y;
        position.z += velocity.z;
        for(int i=0; i<objList->size(); i++){
            if(this == objList->at(i)) continue;
            if(isCollision(objList->at(i))){
                objList->at(i)->setColor(1, 0, 0, 1);
                return ;
            }
        }
    }
    void draw(){
        glPushMatrix();
        glTranslatef(position.x, position.y, position.z);
        glColor3f(0.3, 0.3, 0.3);
        gluSphere(gluNewQuadric(), 1, 10, 10);
        glPopMatrix();
    }
private:
};

#endif