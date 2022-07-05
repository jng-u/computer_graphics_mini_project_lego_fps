#include "GameManager.h"
GameManager* GameManager::instance = NULL;

GameManager* GameManager::getInstance(){
    if(instance!=NULL) instance = new GameManager;
    return instance;
}

GameManager::GameManager(){
    Building = new vector<Block*>;
    currBlock = new Block();
    viewDistance = 100;
    theta = 0;
    phi = 90;
}

GameManager::~GameManager(){
}

void GameManager::init(){
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    setLight();
}

void GameManager::setLight(){
    GLfloat ambientLight[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat diffuseLight[] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat specularLight[] = {0.9f, 0.9f, 0.9f, 1.0f};
    GLfloat specularMaterial[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
    glMateriali(GL_FRONT, GL_SHININESS, 20);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat light_position[] = {30.0f, 100.0f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void draw_axis(){
    glLineWidth(3);
    glBegin(GL_LINES);
        glColor3f(1, 0, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(4, 0, 0);
        
        glColor3f(0, 1, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 4, 0);
        
        glColor3f(0, 0, 1);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 4);
    glEnd();
    glLineWidth(1);
}

void GameManager::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    double y_dst = viewDistance*cos(phi/180*PI);
    double z_dst = viewDistance*sin(phi/180*PI)*cos(theta/180*PI);
    double x_dst = viewDistance*sin(phi/180*PI)*sin(theta/180*PI);
    gluLookAt(x_dst, y_dst, z_dst, 0, 0, 0, 0, (phi<=180)? 1: -1, 0);

    for(int i=0; i<Building->size(); i++){
        Building->at(i)->draw();
    }

    if(currBlock->isActivate()) currBlock->draw();

	glFlush();
    glutSwapBuffers();
}

void GameManager::resize(int width, int height){
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)width/height, 1, 500);
    glMatrixMode(GL_MODELVIEW);
}

void GameManager::keyboard(unsigned char key, int x, int y){
    switch(key){
    case '1':
        currBlock->setBlock(BLOCK_111);
        break;
    case '2':
        currBlock->setBlock(BLOCK_121);
        break;
    case '3':
        currBlock->setBlock(BLOCK_222);
        break;
    case '4':
        currBlock->setBlock(BLOCK_242);
        break;
    }
}

void GameManager::specialKey(int key,  int x, int y){
    switch(key){
    case GLUT_KEY_UP:
        phi += ROTATION_SPEED;
        if(360 < phi) phi-=360; 
        break;
    case GLUT_KEY_LEFT:
        theta += ROTATION_SPEED;
        if(360 < theta) theta-=360; 
        break;
    case GLUT_KEY_DOWN:
        phi -= ROTATION_SPEED;
        if(phi <= 0) phi+=360;
        break;
    case GLUT_KEY_RIGHT:
        theta -= ROTATION_SPEED;
        if(theta <= 0) theta+360;; 
        break;
    case GLUT_KEY_PAGE_UP:
        if(0 < viewDistance) viewDistance--;
        break;
    case GLUT_KEY_PAGE_DOWN:
        viewDistance++;
        break;
    }
    glutPostRedisplay();
}

void GameManager::idle(void){

}