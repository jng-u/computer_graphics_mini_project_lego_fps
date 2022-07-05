#include "Player.h"

Player::Player(){
    theta = 0;
    phi = 90;
    bullet = new Bullet;
}

void Player::setAngle(const float &angle){
    this->angle = angle;
}

void Player::setCamera(){
    float x_org = position.x+width.x/2;
    float y_org = position.y+width.y;
    float z_org = position.z+width.z/2;
    float y_dir = cos(phi/180*PI);
    float z_dir = sin(phi/180*PI)*cos(theta/180*PI);
    float x_dir = sin(phi/180*PI)*sin(theta/180*PI);
    gluLookAt(x_org, y_org, z_org, x_org-x_dir, y_org-y_dir, z_org-z_dir, 0, (phi<=180)? 1: -1, 0);
}

void Player::setCameraAngle(const float &theta, const float &phi){
    this->theta = theta;
    this->phi = phi;
}

void Player::rotate(const float &theta, const float &phi){
    this->theta += theta;
    if(this->theta <= 0) this->theta+=360;
    else if(360 < this->theta) this->theta-=360;

    this->phi += phi;
    if(this->phi < 1) this->phi=1;
    if(180 < this->phi) this->phi=180;
}

void Player::draw(){
    draw(color, alpha);
}

void Player::draw(const Vec3f &color, const float &a){
    draw(position, color, a);
}
 
void Player::draw(const Vec3f &position, const Vec3f &color, const float &a){
    if(!activateFlag) return;
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);

    glTranslatef(-width.x/2, 0, -width.z/2);
    glRotatef(angle, 0, 1, 0);
     
    glColor4f(color.x, color.y, color.z, a);
    // glBegin(GL_TRIANGLES);
    //     for(int i=0; i<op->getFaceSize(); i+=3){
    //         Vec3f n = op->normal[op->normalIdx[i]-1];
    //         Vec3f v = op->vertices[op->vertexIdx[i]-1];
    //         glNormal3f(n.x, n.y, n.z);
    //         glVertex3f(v.x, v.y, v.z);
            
    //         n = op->normal[op->normalIdx[i+1]-1];
    //         v = op->vertices[op->vertexIdx[i+1]-1];
    //         glNormal3f(n.x, n.y, n.z);
    //         glVertex3f(v.x, v.y, v.z);
            
    //         n = op->normal[op->normalIdx[i+2]-1];
    //         v = op->vertices[op->vertexIdx[i+2]-1];
    //         glNormal3f(n.x, n.y, n.z);
    //         glVertex3f(v.x, v.y, v.z);
    //     }
    // glEnd();
    glPopMatrix();
}