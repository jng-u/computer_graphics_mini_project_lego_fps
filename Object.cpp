#include "Object.h"

Object::Object(){
    position.x=0; position.y=0; position.z=0;
    width.x=0; width.y=0; width.z=0;
    color.x=0; color.y=0; color.z=0;
    velocity = {0, 0, 0};
    activateFlag = true;
}

Object::~Object(){

}

void Object::setVelocity(const float &x, const float &y, const float &z){
    velocity.x = x;
    velocity.y = y;
    velocity.z = z;
}

void Object::setPosition(const Vec3f &position){
    setPosition(position.x, position.y, position.z);
}

void Object::setPosition(const float &x, const float &y, const float &z){
    position.x = x;
    position.y = y;
    position.z = z;
}

void Object::setWidth(const Vec3f &width){
    setWidth(width.x, width.y, width.z);
}

void Object::setWidth(const float &w, const float &h, const float &d){
    width.x = w;
    width.y = h;
    width.z = d;
}

void Object::setColor(const Vec3f &color, const float &a){
    this->color = color;
    alpha = a;
}

void Object::setColor(const float &r, const float &g, const float &b, const float &a){
    color.x = r;
    color.y = g;
    color.z = b;
    alpha = a;
}

void Object::setActivate(const bool &activate_flag){
    this->activateFlag = activate_flag;
}

void Object::move(){
    position.x += velocity.x;
    position.y += velocity.y;
    position.z += velocity.z;
    velocity = {0, 0, 0};
}

void Object::move(const vector<Object*> *objList){
    // Gravity operation
    velocity.y -= GRAVITY;
    position.y += velocity.y;
    if(position.y < 0) {
        position.y = 0;
        velocity.y = 0;
    } else {
        for(int i=0; i<objList->size(); i++){
            if(this == objList->at(i)) continue;
            if(isCollision(objList->at(i))){
                if(0 < velocity.y){
                    velocity.y=-0.01;
                    position.y = objList->at(i)->getPosition().y - width.y;
                } else {
                    position.y = objList->at(i)->getPosition().y + objList->at(i)->getWidth().y;
                    velocity.y=0;
                }
                break;        
            }
        }
    }

    position.x += velocity.x;
    position.z += velocity.z;
    for(int i=0; i<objList->size(); i++){
        if(this == objList->at(i)) continue;
        if(isCollision(objList->at(i))){
            position.x -= velocity.x;
            position.z -= velocity.z;
            break;
        }
    }
    // cout<<velocity.x<<" "<<velocity.z<<endl;
    // cout<<position.x<<" "<<position.z<<" "<<position.y<<endl;
    velocity.x = 0;
    velocity.z = 0;
}

bool Object::isCollision(Object *obj){
    /*
    Vec3f tp = obj->getPosition();
    Vec3f tw = obj->getWidth();
    int ix = abs(velocity.x/width.x) + 1;
    int iy = abs(velocity.y/width.y) + 1;
    int iz = abs(velocity.z/width.z) + 1;
    for(int i=0; i<ix; i++){
        for(int j=0; j<iy; j++){
            for(int k=0; k<iz; k++){
                float px, py, pz, dx, dy, dz;
                dx = (i != ix-1)? i*width.x*velocity.x/abs(velocity.x) : velocity.x;
                dy = (j != iy-1)? i*width.y*velocity.y/abs(velocity.y) : velocity.y;
                dz = (k != iz-1)? i*width.z*velocity.z/abs(velocity.z) : velocity.z;
                px = position.x + dx;
                py = position.y + dy;
                pz = position.z + dz;
                if( px <= tp.x + tw.x && tp.x <= px + width.x  &&
                    py <= tp.y + tw.y && tp.y <= py + width.y  &&
                    pz <= tp.z + tw.z && tp.z <= pz + width.z ){
                    if(px - dx > tp.x+tw.x || tp.x > px+width.x - dx )
                        velocity.x = (dx>0)? dx+(tp.x-(px+width.x))-0.01 : dx+((tp.x+tw.x)-px)+0.01;
                    if(py - dy > tp.y+tw.y || tp.y > px+width.y - dy )
                        velocity.y = (dy>0)? dy+(tp.y-(py+width.y))-0.01 : dy+((tp.y+tw.y)-py)+0.01;
                    if(pz - dz > tp.z+tw.z || tp.z > px+width.z - dz )
                        velocity.z = (dz>0)? dz+(tp.z-(pz+width.z))-0.01 : dz+((tp.z+tw.z)-pz)+0.01;
                    return true;
                }
            }
        }
    }
    return false;
    */
    Vec3f tp = obj->getPosition();
    Vec3f tw = obj->getWidth();
    if( position.x < tp.x + tw.x && tp.x < position.x + width.x  &&
        position.y < tp.y + tw.y && tp.y < position.y + width.y  &&
        position.z < tp.z + tw.z && tp.z < position.z + width.z ){
        Vec3f prevP = {position.x-velocity.x, position.y-velocity.y, position.z-velocity.z};
        Vec3f twP = {tp.x+tw.x, tp.y+tw.y, tp.z+tw.z};
        if(!(prevP.y >= twP.y || tp.y >= position.y+width.y - velocity.y )){
            if(prevP.x >= twP.x || tp.x >= position.x+width.x - velocity.x ){
                velocity.x = (velocity.x>0)? velocity.x-(tp.x-(position.x+width.x)) : velocity.z-((tp.x+tw.x)-position.x);
            } else if(prevP.z >= tw.z || tp.z > position.x+width.z - velocity.z ){
                velocity.z = (velocity.z>0)? velocity.z-(tp.z-(position.z+width.z)) : velocity.z-((tp.z+tw.z)-position.z);
            }
        }
        return true;
    }
    return false;
    // Vec3f tp = obj->getPosition();
    // Vec3f tw = obj->getWidth();
    // if( position.x < tp.x + tw.x && tp.x < position.x + width.x  &&
    //     position.y < tp.y + tw.y && tp.y < position.y + width.y  &&
    //     position.z < tp.z + tw.z && tp.z < position.z + width.z ){
    //     return true;
    // }
    // return false;
}