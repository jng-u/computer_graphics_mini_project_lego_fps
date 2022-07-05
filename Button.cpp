#include "Button.h"

Button::Button(){
    activateFlag = false;
}

Button::Button(char *path){
    setPath(path);
}

Button::~Button(){

}

void Button::setPath(char *path){
    bmpPath = path;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int width, height, channels;
    uchar *img = readImageData(bmpPath, &width, &height, &channels);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
}

void Button::draw(){
    if(!activateFlag) return;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex2f(position.x, position.y);
        glTexCoord2f(width.x/width.y, 0);
        glVertex2f(position.x+width.x, position.y);
        glTexCoord2f(width.x/width.y, width.y/width.x);
        glVertex2f(position.x+width.x, position.y+width.y);
        glTexCoord2f(0, width.y/width.x);
        glVertex2f(position.x, position.y+width.y);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

bool Button::isMouseOn(const float &x, const float &y){
    if(!activateFlag) return false;
    float dx = x-(position.x + width.x/2);
    float dy = y-(position.y + width.y/2);
    if(width.x/2 < abs(dx)) return false;
    if(width.y/2 < abs(dy)) return false;
    return true;
}