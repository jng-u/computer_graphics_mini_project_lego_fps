#include "Block.h"

ObjParser *Block::objParser[6];

Block::Block(){
    activateFlag = false;
    rotateFlag = false;
    if(objParser[BLOCK_111] == NULL) objParser[BLOCK_111] = new ObjParser("./resources/111.obj");
    if(objParser[BLOCK_121] == NULL) objParser[BLOCK_121] = new ObjParser("./resources/121.obj");
    if(objParser[BLOCK_141] == NULL) objParser[BLOCK_141] = new ObjParser("./resources/141.obj");
    if(objParser[BLOCK_441] == NULL) objParser[BLOCK_441] = new ObjParser("./resources/441.obj");
    if(objParser[BLOCK_222] == NULL) objParser[BLOCK_222] = new ObjParser("./resources/222.obj");
    if(objParser[BLOCK_242] == NULL) objParser[BLOCK_242] = new ObjParser("./resources/242.obj");
}

Block::Block(const int &block){
    setBlock(block);
}

Block::Block(Block *blk){
    setBlock(blk->getBlock());
    setPosition(blk->getPosition());
    setWidth(blk->getWidth());
    setColor(blk->getColor(), blk->getAlpha());
    this->rotateFlag = blk->rotateFlag;
    this->activateFlag = blk->activateFlag;
}

Block::~Block(){
}

void Block::setBlock(const int &block){    
    this->block = block;
    switch(block){
    case BLOCK_111:
        setWidth(5, 2, 5);
        break;
    case BLOCK_121:
        setWidth(10, 2, 5);
        break;
    case BLOCK_141:
        setWidth(20, 2, 5);
        break;
    case BLOCK_441:
        setWidth(20, 2, 20);
        break;
    case BLOCK_222:
        setWidth(10, 6, 10);
        break;
    case BLOCK_242:
        setWidth(20, 6, 10);
        break;
    }
    activateFlag = true;
    setColor(1, 1, 0, 0.8f);
}

inline void Block::draw(){
    draw(position, color, alpha);
}

inline void Block::draw(const Vec3f &color, const float &a){
    draw(position, color, a);
}

void Block::draw(const Vec3f &position, const Vec3f &color, const float &a){
    if(!activateFlag) return;
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    if(rotateFlag) rotateFunc();
    glColor4f(color.x, color.y, color.z, a);
    ObjParser *op = objParser[block];
    glBegin(GL_TRIANGLES);
        for(int i=0; i<op->getFaceSize(); i+=3){
            Vec3f n = op->normal[op->normalIdx[i]-1];
            Vec3f v = op->vertices[op->vertexIdx[i]-1];
            glNormal3f(n.x, n.y, n.z);
            glVertex3f(v.x, v.y, v.z);
            
            n = op->normal[op->normalIdx[i+1]-1];
            v = op->vertices[op->vertexIdx[i+1]-1];
            glNormal3f(n.x, n.y, n.z);
            glVertex3f(v.x, v.y, v.z);
            
            n = op->normal[op->normalIdx[i+2]-1];
            v = op->vertices[op->vertexIdx[i+2]-1];
            glNormal3f(n.x, n.y, n.z);
            glVertex3f(v.x, v.y, v.z);
        }
    glEnd();
    glPopMatrix();
}

void Block::rotateFunc(){
    glTranslatef(0, 0, width.z);
    glRotatef(90, 0, 1, 0);
}

void Block::rotate(){
    rotateFlag = !rotateFlag;
    float tmp = width.x;
    setWidth(width.z, width.y, tmp);
}