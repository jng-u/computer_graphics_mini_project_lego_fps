#ifndef __BLOCK_H__
#define __BLOCK_H__
#include "Header.h"
#include "ObjParser.h"
#include "Object.h"

class Block : public Object {
public:
    Block();
    Block(const int &block);
    Block(Block *blk);      // 복사 생성자
    ~Block();
    void setBlock(const int &block);
    int getBlock(){ return block; }
    bool isRotate(){ return rotateFlag; }

    inline void draw();
    inline void draw(const Vec3f &color, const float &a);
    void draw(const Vec3f &position, const Vec3f &color, const float &a);
    
    void clear(){
        block=-1;
        setPosition(0, 0, 0);
        setWidth(0, 0, 0);
        setColor(0, 0, 0, 0);
        activateFlag = false;
        rotateFlag = false;
    }
    void rotate();
    void rotateFunc();

    bool onMouseCheck(float x, float y);

    static void deleteObjParser(){
        for(int i=0; i<6; i++) delete objParser[i];
    }
private:
    static ObjParser *objParser[6];
    int block = -1;
    bool rotateFlag;        // y축방향으로 회전 /0:0도 1:90도
};

#endif