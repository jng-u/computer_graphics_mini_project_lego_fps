#ifndef __GAMEMANAGER_H__
#define __GAMEMANAGER_H__
#include "Header.h"
#include "Object.h"
#include "Block.h"
#include "Button.h"
#include "Player.h"

class GameManager{
public:
    GameManager();
    ~GameManager();

    void init();
    void setLight();

    //draw func
    void drawRect(const int &x, const int &y, const int &w, const int &h, const Vec3f &color, const float &alpha);
    void drawScope();
    void drawAxis();
    void drawFloor();
    void drawBlocks();
    void drawPreview();
    void drawMenu();
    void drawToggleMessage(void *font, const char *msg);

    //glut function
    void draw();
    void resize(const int &width, const int &height);
    void keyboard(const unsigned char &key, const int &x, const int &y);
    void keyboardUp(const unsigned char &key, const int &x, const int &y);
    void specialKey(const int &key, const int &x, const int &y);
    void mouse(const int &button, const int &state, const int &x, const int &y);
    void mouseWheel(const int &button, const int &state, const int &x, const int &y);
    void motion(const int &x, const int &y);
    void passiveMotion(const int &x, const int &y);
    void idle();

    //operation
    void keyOperation();
    void objectAction();
    void rotate(float &angle, const float &d){
        angle += d;
        if(angle<=0) angle+=360;
        if(360<angle) angle-=360;
    }
    //토글 메시지 출력 설정
    void setToggle(const char *msg, int time);
    //블록 놓기
    void putBlock();
    //버튼이 좌클릭 되었을 때의 event
    void buttonAction(const int &x, const int &y);
    //블록 선택 <-> 색깔 선택
    void swapMenuMode();
    //이전행동 취소
    void undo();
    //취소행동 다시하기
    void redo();

    //read, write
    void readBuildData(const char* path);
    void writeBuildData(const char* path);

private:
    int windowWidth, windowHeight;      // window Size
    bool keyState[256];                 // save key state

    bool gameMode=false;                // mode선택 / 0:build모드 1:game모드

    vector<Block*> *Building;           // 레고로 만듬
    Block *currBlock;                   // 현재 선택한 블록
    stack<Block*> *undoList;            // 실행취소한 블록을 담아둔다.

    Player* hero;
    
    int viewDistance;                   // camera 거리
    float viewTheta, viewPhi;           // camera 각도
    float viewZoom;                     // camera zoom 

    vector<Button*> *blockButtons;      // block button모음
    vector<Button*> *colorButtons;      // color button모음
    bool menuButtonMode=false;          // menu mode /0:block 1:color

    Vec2f mouseOld;                     // mouse drag를 위한 좌표저장 
    bool dragLeftDownFlag=false;        // mouse drag flag 

    bool toggleFlag=false;              // toggle msg를 출력할지 안할지 결정해줌
    string toggleMsg="";                // 출력할 toggle 메시지
    int duration;                       // toggle 메시지를 띄울 시간 (ms)
    clock_t toggleOldtime;              // 경과시간 체크를 위해 이전 시간 저장
    
    clock_t prev_time;                  // fps체크를 위한 이전 시간 변수
};
#endif