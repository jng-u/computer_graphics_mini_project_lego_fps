#include "GameManager.h"

GameManager::GameManager(){
}

GameManager::~GameManager(){
    Block::deleteObjParser();
    for(int i=0; i<Building->size(); i++) delete Building->at(i);
    delete Building;
    delete currBlock;
    while(!undoList->empty()){
        delete undoList->top();
        undoList->pop();
    }
    delete undoList;
    for(int i=0; i<blockButtons->size(); i++) delete blockButtons->at(i);
    delete blockButtons;
    for(int i=0; i<colorButtons->size(); i++) delete colorButtons->at(i);
    delete colorButtons;

    cout<<"End\n";
}

void GameManager::init(){
    glEnable(GL_SCISSOR_TEST);

    // antialiasing
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);

    // use alpha channel
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);	

    // depth setting
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    
    // back face culling
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    // set light
    setLight();

    // set texture
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // ingnore key repeat
    glutIgnoreKeyRepeat(1);
    
    // init block
    Building = new vector<Block*>;
    currBlock = new Block();
    undoList = new stack<Block*>();
    // init camera
    viewDistance = 100;
    viewTheta = 45;
    viewPhi = 45;
    viewZoom = 1;
    // block button set
    blockButtons = new vector<Button*>;
    blockButtons->push_back(new Button(BLOCK_111_IMG_PATH));
    blockButtons->push_back(new Button(BLOCK_121_IMG_PATH));
    blockButtons->push_back(new Button(BLOCK_141_IMG_PATH));
    blockButtons->push_back(new Button(BLOCK_441_IMG_PATH));
    blockButtons->push_back(new Button(BLOCK_222_IMG_PATH));
    blockButtons->push_back(new Button(BLOCK_242_IMG_PATH));
    for(int i=0; i<blockButtons->size(); i++){
        blockButtons->at(i)->setPosition(MENU_WIDTH/2*(i%2), MENU_WIDTH/2*((i)/2));
        blockButtons->at(i)->setWidth(MENU_WIDTH/2, MENU_WIDTH/2);
        blockButtons->at(i)->setActivate(true);
    }
    // color button set
    colorButtons = new vector<Button*>;
    colorButtons->push_back(new Button(COLOR_RED_IMG_PATH));
    colorButtons->push_back(new Button(COLOR_ORANGE_IMG_PATH));
    colorButtons->push_back(new Button(COLOR_YELLOW_IMG_PATH));
    colorButtons->push_back(new Button(COLOR_GREEN_IMG_PATH));
    colorButtons->push_back(new Button(COLOR_SKYBLUE_IMG_PATH));
    colorButtons->push_back(new Button(COLOR_BLUE_IMG_PATH));
    colorButtons->push_back(new Button(COLOR_PINK_IMG_PATH));
    colorButtons->push_back(new Button(COLOR_PURPLE_IMG_PATH));
    colorButtons->push_back(new Button(COLOR_WHITE_IMG_PATH));
    colorButtons->push_back(new Button(COLOR_BLACK_IMG_PATH));
    for(int i=0; i<colorButtons->size(); i++){
        colorButtons->at(i)->setPosition(MENU_WIDTH/2*(i%2), MENU_WIDTH*0.325*((i)/2));
        colorButtons->at(i)->setWidth(MENU_WIDTH/2, MENU_WIDTH*0.325);
        colorButtons->at(i)->setActivate(false);
    }

    // temporary player class test
    hero = new Player;
    hero->setPosition(10, 0, 10);
    hero->setWidth(5, 20, 5);
 
    prev_time = clock();
}

void GameManager::setLight(){
    GLfloat ambientLight[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat diffuseLight[] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat specularLight[] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat specularMaterial[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
    glMateriali(GL_FRONT, GL_SHININESS, 20);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat light_position[] = {0.0f, 100.0f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void GameManager::drawScope(){
    glDepthRange(0, 0.01);
    glViewport(0, 0, windowWidth, windowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-windowWidth/2, windowWidth/2, -windowHeight/2, windowHeight/2);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(0, 0, 0);
    if(viewZoom>1) gluDisk(gluNewQuadric(), (windowHeight-100)/2, sqrt(windowWidth*windowWidth + windowHeight*windowHeight)/2, 50, 1);
    glLineWidth(2.5);
    glBegin(GL_LINES);
        glVertex2f(-30, 0);
        glVertex2f(30, 0);
        glVertex2f(0, -30);
        glVertex2f(0, 30);
    glEnd();


    glFlush();
}

void GameManager::drawRect(const int &x, const int &y, const int &w, const int &h, const Vec3f &color, const float &alpha){
    glViewport(x, y, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluOrtho2D(0, w, 0, h);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
    glDisable(GL_LIGHTING);

    glColor4f(color.x, color.y, color.z, alpha);
    glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(w, 0);
        glVertex2f(w, h);
        glVertex2f(0, h);
    glEnd();

    glEnable(GL_LIGHTING);
    glFlush();
}

void GameManager::drawAxis(){
    glLineWidth(1);
    glBegin(GL_LINES);
        glColor3f(1, 0, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(25, 0, 0);
        
        glColor3f(0, 1, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 25, 0);
        
        glColor3f(0, 0, 1);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 25);
    glEnd();
    glLineWidth(1);
}

void GameManager::drawFloor(){
    Block *tmp = new Block(BLOCK_441);
    tmp->setColor(1, 1, 0, 1);

    int floorSize=7;

    for(int i=0; i<floorSize; i++){
        for(int j=0; j<floorSize; j++){
            tmp->setPosition(-floorSize*10+20*i, -2.05, -floorSize*10+20*j);
            tmp->draw();
        }
    }

}

void GameManager::drawBlocks(){
    glDepthRange(0.02, 1);
    glViewport(0, 0, windowWidth, windowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45/viewZoom, (float)windowWidth/windowHeight, 1, 500);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    if(!gameMode){
        double y_dst = viewDistance*cos(viewPhi/180*PI);
        double z_dst = viewDistance*sin(viewPhi/180*PI)*cos(viewTheta/180*PI);
        double x_dst = viewDistance*sin(viewPhi/180*PI)*sin(viewTheta/180*PI);
        gluLookAt(x_dst, y_dst, z_dst, 0, 0, 0, 0, (viewPhi<=180)? 1: -1, 0);
    } else {
        hero->setCamera();
    }

    drawFloor();
    if(!gameMode) drawAxis();
    for(int i=0; i<Building->size(); i++){
        Building->at(i)->draw();
    }
    currBlock->draw();
    
	glFlush();
}

void GameManager::drawPreview(){ 
    glDepthRange(0.01, 0.02);
    drawRect(windowWidth-PREVIEW_WIDTH, windowHeight-PREVIEW_WIDTH, PREVIEW_WIDTH, PREVIEW_WIDTH, 
            {0.5, 0.5, 0.5}, 0.6);

    glDepthRange(0, 0.01);
    glViewport(windowWidth-PREVIEW_WIDTH, windowHeight-PREVIEW_WIDTH, PREVIEW_WIDTH, PREVIEW_WIDTH);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 1, 500);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(50, 50, 50, 0, 0, 0, 0, 1, 0);

    drawAxis();
    if(currBlock->isActivate()) 
        currBlock->draw(Vec3f{0.0f, 0.0f, 0.0f}, currBlock->getColor(), 1.0f);
    
    glFlush();   
}

void GameManager::drawMenu(){
    glDepthRange(0.01, 0.02);
    drawRect(0, 0, MENU_WIDTH, windowHeight, {0.5, 0.5, 0.5}, 0.6);
            
    glDepthRange(0, 0.01);
    glViewport(0, 0, MENU_WIDTH, windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, MENU_WIDTH, 0, windowHeight);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glDisable(GL_LIGHTING);

    for(int i=0; i<blockButtons->size(); i++){
        blockButtons->at(i)->draw();
    }
    
    for(int i=0; i<colorButtons->size(); i++){
        colorButtons->at(i)->draw();
    }
    
    glEnable(GL_LIGHTING);
	glFlush();
}

void GameManager::drawToggleMessage(void *font, const char *msg){
    glDepthRange(0, 1);
    glViewport(0, 0, windowWidth, windowHeight);
    glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	
    glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-10, 10, -10, 10);
	
    glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

    float fontWidth = 0;
	for (unsigned int i = 0; i < strlen(msg); i++) {
        fontWidth += glutBitmapWidth(font, msg[i]);
    }
    fontWidth *= 20.0/windowWidth;

	glColor3f(1, 1, 1);
	glRasterPos2f(-fontWidth/2, -7.7);
	for (unsigned int i = 0; i < strlen(msg); i++) {
		glutBitmapCharacter(font, msg[i]);
	}
    
    glColor4f(0, 0, 0, 0.5);
    glBegin(GL_QUADS);
        glVertex2f(-2.5, -7);
        glVertex2f(-2.5, -8);
        glVertex2f(2.5, -8);
        glVertex2f(2.5, -7);
    glEnd();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopAttrib();
}

void GameManager::draw() {
    keyOperation();
    objectAction();

    glClearColor(0.75, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
    drawBlocks();
    if(!gameMode){
        drawPreview();
        drawMenu();
    }

    if(gameMode) drawScope();
    if(toggleFlag) drawToggleMessage(GLUT_BITMAP_TIMES_ROMAN_24, toggleMsg.c_str());

    glutSwapBuffers();
    glFlush();
    // std::cout<<"draw : "<<CLOCKS_PER_SEC/(double)(clock()-prev_time)<<"\n";
    while((double)(clock()-prev_time)/CLOCKS_PER_SEC < 1.0f/FPS);
    prev_time = clock();
}

void GameManager::resize(const int &width, const int &height){
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)width/height, 1, 500);
    glMatrixMode(GL_MODELVIEW);
}

void GameManager::keyboard(const unsigned char &key, const int &x, const int &y){
    keyState[key] = true;    
    glutPostRedisplay();
}

void GameManager::keyboardUp(const unsigned char &key, const int &x, const int &y){
    keyState[key] = false;
    glutPostRedisplay();
}

void GameManager::specialKey(const int &key, const int &x, const int &y){
    switch(key){

    }
    glutPostRedisplay();
}

void GameManager::mouse(const int &button, const int &state, const int &x, const int &y){
    if(!gameMode){
        int wy = windowHeight-y;
        if(button == GLUT_LEFT_BUTTON){
            if(x <= MENU_WIDTH){
                //menu영역 
                if(!state){ 
                    buttonAction(x, wy);
                }
            } else {
                if(!state){        
                    //화면 돌리기 
                    dragLeftDownFlag = true;
                    mouseOld.x = x;
                    mouseOld.y = y;
                } else {
                    dragLeftDownFlag = false;
                }
            }
        } else if(button == GLUT_RIGHT_BUTTON){
            if(x <= MENU_WIDTH){
                if(!state){ 
                    swapMenuMode();
                }
            }
        }
    } else {
        if(button==GLUT_LEFT_BUTTON){

        } else if(button==GLUT_RIGHT_BUTTON){
            if(!state){
                viewZoom = 3;
            } else {
                viewZoom = 1;
            }
        }
    }
    glutPostRedisplay();
}

void GameManager::mouseWheel(const int &button, const int &state, const int &x, const int &y){
    if(state<0) {
        viewDistance++;
    } else {
        if(5 < viewDistance) viewDistance--;
    }
    glutPostRedisplay();
}

void GameManager::motion(const int &x, const int &y){
    if(!gameMode){
        if(!dragLeftDownFlag) return;
        rotate(viewTheta, -ROTATION_SENSITIVITY*(x-mouseOld.x));
        rotate(viewPhi, -ROTATION_SENSITIVITY*(y-mouseOld.y));
        if(270<viewPhi && viewPhi<=360) viewPhi = 360;
        else if(viewPhi>90) viewPhi = 90;
        mouseOld.x = x;
        mouseOld.y = y;
    } else {
        hero->rotate(-ROTATION_SENSITIVITY*(x-mouseOld.x), -ROTATION_SENSITIVITY*(y-mouseOld.y));
        glutWarpPointer(windowWidth/2, windowHeight/2);
        mouseOld.x = windowWidth/2;
        mouseOld.y = windowHeight/2;
    }
    glutPostRedisplay();
}

void GameManager::passiveMotion(const int &x, const int &y){
    if(!gameMode) return ;
    hero->rotate(-ROTATION_SENSITIVITY*(x-mouseOld.x), -ROTATION_SENSITIVITY*(y-mouseOld.y));
    glutWarpPointer(windowWidth/2, windowHeight/2);
    mouseOld.x = windowWidth/2;
    mouseOld.y = windowHeight/2;
    glutPostRedisplay();
}

void GameManager::idle(void){
    if(clock()-toggleOldtime > duration) toggleFlag = false;
}

void GameManager::keyOperation(){
    if(keyState[27]) {
        if(currBlock->isActivate()){
            currBlock->clear();
            swapMenuMode();
        } else {
            exit(0);
        }
    }
    if(keyState['g']) {
        if(currBlock->isActivate()){
            currBlock->clear();
            swapMenuMode();
        }
        keyState['g'] = false;
        gameMode = !gameMode;
        if(gameMode) glutSetCursor(GLUT_CURSOR_NONE);
        else glutSetCursor(GLUT_CURSOR_INHERIT);
        glutWarpPointer(windowWidth/2, windowHeight/2);
        mouseOld.x = windowWidth/2;
        mouseOld.y = windowHeight/2;
    }
    if(!gameMode){
        if(keyState[13]){        //Enter key
            putBlock();
        } else if(keyState[' ']){
            currBlock->rotate();
        } else if(keyState['a']){
            currBlock->setVelocity(-5, 0, 0);
        } else if(keyState['d']){
            currBlock->setVelocity(5, 0, 0);
        } else if(keyState['w']){
            currBlock->setVelocity(0, 0, -5);
        } else if(keyState['s']){
            currBlock->setVelocity(0, 0, 5);
        } else if(keyState['q']){
            currBlock->setVelocity(0, 2, 0);
        } else if(keyState['e']){
            if(currBlock->getPosition().y > 0) currBlock->setVelocity(0, -2, 0);
        } else if(keyState['i']){
            writeBuildData("./data/build_data.txt");
        } else if(keyState['o']){
            readBuildData("./data/build_data.txt");
        } else if(keyState['u']){
            undo();
        } else if(keyState['y']){
            redo();
        }
    } else {
        float xwv, zwv;
        xwv = CHARACTER_SPEED*sin(hero->getAngle()/180*PI);
        zwv = CHARACTER_SPEED*cos(hero->getAngle()/180*PI);
        if(keyState['w'] && keyState['a']){
            xwv = CHARACTER_SPEED*sin((hero->getAngle()+45)/180*PI);
            zwv = CHARACTER_SPEED*cos((hero->getAngle()+45)/180*PI);
            hero->setVelocity(-xwv, hero->getVelocity().y, -zwv);
        } else if(keyState['w'] && keyState['d']){
            xwv = CHARACTER_SPEED*sin((hero->getAngle()-45)/180*PI);
            zwv = CHARACTER_SPEED*cos((hero->getAngle()-45)/180*PI);
            hero->setVelocity(-xwv, hero->getVelocity().y, -zwv);
        } else if(keyState['s'] && keyState['a']){
            xwv = CHARACTER_SPEED*sin((hero->getAngle()+135)/180*PI);
            zwv = CHARACTER_SPEED*cos((hero->getAngle()+135)/180*PI);
            hero->setVelocity(-xwv, hero->getVelocity().y, -zwv);
        } else if(keyState['s'] && keyState['d']){
            xwv = CHARACTER_SPEED*sin((hero->getAngle()-135)/180*PI);
            zwv = CHARACTER_SPEED*cos((hero->getAngle()-135)/180*PI);
            hero->setVelocity(-xwv, hero->getVelocity().y, -zwv);
        } else if(keyState['w']){
            hero->setVelocity(-xwv, hero->getVelocity().y, -zwv);
        } else if(keyState['a']){
            hero->setVelocity(-zwv, hero->getVelocity().y, xwv);
        } else if(keyState['s']){
            hero->setVelocity(xwv, hero->getVelocity().y, zwv);
        } else if(keyState['d']){
            hero->setVelocity(zwv, hero->getVelocity().y, -xwv);
        }
        if(keyState[' ']){
            if(hero->getVelocity().y==0){
                hero->setVelocity(hero->getVelocity().x, 3, hero->getVelocity().z);
            }
        }
    }
}

void GameManager::objectAction(){
    if(currBlock->isActivate()) currBlock->move();
    if(gameMode) for(int i=0; i<Building->size(); i++) Building->at(i)->move((vector<Object*>*)Building);
    hero->move((vector<Object*>*)Building);
}

void GameManager::setToggle(const char *msg, int time){
    toggleFlag = true;
    toggleMsg = msg;
    duration = time;
    toggleOldtime = clock();
}

void GameManager::putBlock(){
    //Check Collision
    for(int i=0; i<Building->size(); i++){
        if(currBlock->isCollision(Building->at(i))){
            setToggle("There is already exist other block!", 1000);
            return ;
        }
    }
    //Check is Select
    if(currBlock->isActivate()){
        Block *n = new Block(currBlock);
        n->setColor(currBlock->getColor(), 1.0f);
        Building->push_back(n);
        currBlock->clear();
        if(menuButtonMode) swapMenuMode();
        while(!undoList->empty()){
            delete undoList->top();
            undoList->pop();
        }
    }
}

void GameManager::buttonAction(const int &x, const int &y){
    if(!menuButtonMode){
        if(blockButtons->at(0)->isMouseOn(x, y)) 
            currBlock->setBlock(BLOCK_111);
        else if(blockButtons->at(1)->isMouseOn(x, y)) 
            currBlock->setBlock(BLOCK_121);
        else if(blockButtons->at(2)->isMouseOn(x, y)) 
            currBlock->setBlock(BLOCK_141);
        else if(blockButtons->at(3)->isMouseOn(x, y)) 
            currBlock->setBlock(BLOCK_441);
        else if(blockButtons->at(4)->isMouseOn(x, y)) 
            currBlock->setBlock(BLOCK_222);
        else if(blockButtons->at(5)->isMouseOn(x, y)) 
            currBlock->setBlock(BLOCK_242);
        else 
            swapMenuMode();
        swapMenuMode();
    } else {
        if(colorButtons->at(0)->isMouseOn(x, y)) 
            currBlock->setColor(1, 0, 0, 0.8);      //red
        else if(colorButtons->at(1)->isMouseOn(x, y)) 
            currBlock->setColor(1, 0.5, 0, 0.8);    //orange
        else if(colorButtons->at(2)->isMouseOn(x, y)) 
            currBlock->setColor(1, 1, 0, 0.8);      //yellow
        else if(colorButtons->at(3)->isMouseOn(x, y)) 
            currBlock->setColor(0, 1, 0, 0.8);      //green
        else if(colorButtons->at(4)->isMouseOn(x, y)) 
            currBlock->setColor(0, 1, 1, 0.8);      //skyblue
        else if(colorButtons->at(5)->isMouseOn(x, y)) 
            currBlock->setColor(0, 0.5, 1, 0.8);    //blue
        else if(colorButtons->at(6)->isMouseOn(x, y)) 
            currBlock->setColor(1, 0, 0.5, 0.8);    //pink
        else if(colorButtons->at(7)->isMouseOn(x, y)) 
            currBlock->setColor(0.5, 0, 1, 0.8);    //purple
        else if(colorButtons->at(8)->isMouseOn(x, y)) 
            currBlock->setColor(1, 1, 1, 0.8);      //white
        else if(colorButtons->at(9)->isMouseOn(x, y)) 
            currBlock->setColor(0, 0, 0, 0.8);      //black
    }
}

void GameManager::swapMenuMode(){
    if(!menuButtonMode){
        for(int i=0; i<blockButtons->size(); i++)
            blockButtons->at(i)->setActivate(false);
        for(int i=0; i<colorButtons->size(); i++)
            colorButtons->at(i)->setActivate(true);
        menuButtonMode = true;
    } else {
        for(int i=0; i<blockButtons->size(); i++)
            blockButtons->at(i)->setActivate(true);
        for(int i=0; i<colorButtons->size(); i++)
            colorButtons->at(i)->setActivate(false);
        menuButtonMode = false;
    }
}

void GameManager::undo(){
    if(Building->empty()) {
        setToggle("There is nothing to do.", 1000);
        return ;
    }
    undoList->push(Building->back());
    Building->pop_back();
}

void GameManager::redo(){
    if(undoList->empty()) {
        setToggle("There is nothing to do.", 1000);
        return ;
    }
    Building->push_back(undoList->top());
    undoList->pop();
}

void GameManager::readBuildData(const char *path){
    FILE *f = fopen(path, "r+");
	if (f == NULL) {
		printf("No such files : %s\n", path);
		return;
	}

    Building->clear();
    Block *n = new Block();
	while (true) {
		char lineHeader[128]; // Expected max length of 1 line = 128bytes;
		int line = fscanf(f, "%s", lineHeader);
		if (line == EOF) break;
		if (strcmp(lineHeader, "p") == 0) {
			Vec3f position;
			fscanf(f, "%f %f %f\n", &position.x, &position.y, &position.z);
            n->setPosition(position);
		}
		else if (strcmp(lineHeader, "c") == 0) {
			Vec3f color;
			fscanf(f, "%f %f %f\n", &color.x, &color.y, &color.z);
			n->setColor(color, 1);
		}
		else if (strcmp(lineHeader, "r") == 0) {
            int rotate;
			fscanf(f, "%d\n", &rotate);
            if(rotate==1) n->rotate();
            Building->push_back(new Block(n));
            n->clear();
		}
		else if (strcmp(lineHeader, "b") == 0) {
			int block;
			fscanf(f, "%d\n", &block);
            n->setBlock(block);
		}
	}
    delete n;
	fclose(f);
    setToggle("The file was loaded successfully.", 1000);
}

void GameManager::writeBuildData(const char *path){
	FILE *f = fopen(path, "w+");	

    for(int i=0; i<Building->size(); i++){
		fprintf(f, "b %d\n", Building->at(i)->getBlock());
		fprintf(f, "p %f %f %f\n", Building->at(i)->getPosition().x, Building->at(i)->getPosition().y, Building->at(i)->getPosition().z);
		fprintf(f, "c %f %f %f\n", Building->at(i)->getColor().x, Building->at(i)->getColor().y, Building->at(i)->getColor().z);
		fprintf(f, "r %d\n", (Building->at(i)->isRotate())? 1:0);
    }
	fclose(f);
    setToggle("The file was saved successfully.", 1000);
}