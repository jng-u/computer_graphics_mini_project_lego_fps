#include "GameManager.h"

GameManager gm;

void draw() {
	gm.draw();
}

void resize(int width, int height){
    gm.resize(width, height);
}

void keyboard(unsigned char key, int x, int y){
    gm.keyboard(key, x, y);
}

void keyboardUp(unsigned char key, int x, int y){
    gm.keyboardUp(key, x, y);
}

void specialKey(int key,  int x, int y){
    gm.specialKey(key, x, y);
}

void mouse(int button, int state, int x, int y){
    gm.mouse(button, state, x, y);
}

void mouseWheel(int button, int state, int x, int y){
    gm.mouseWheel(button, state, x, y);
}

void motion(int x, int y){
    gm.motion(x, y);
}

void passiveMotion(int x, int y){
    gm.passiveMotion(x, y);
}

void entry(int state){
}

void idle(void){
    gm.idle();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("LJW's MINI PROJECT");
	gm.init();

    glutDisplayFunc(draw);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialKey);
    glutMouseFunc(mouse);
    glutMouseWheelFunc(mouseWheel);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(passiveMotion);
    glutEntryFunc(entry);
    glutIdleFunc(idle);
    
	glutMainLoop();
	return 0;
}
