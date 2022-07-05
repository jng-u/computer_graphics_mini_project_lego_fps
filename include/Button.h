#ifndef __BUTTON_H__
#define __BUTTON_H__
#include "Header.h"
#include "Object.h"
#include "bmpfuncs.h"

class Button : public Object{
public:
    Button();
    Button(char *path);
    ~Button();

    void setPath(char *path);

    void action();
    void draw();

    bool isMouseOn(const float &x, const float &y);
private:
    char *bmpPath;
    GLuint texture;
};

#endif