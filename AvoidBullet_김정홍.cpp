#include<iostream>
#include<cmath>
#include <gl/glut.h>

using namespace std;

static int SpinAngle = 0;//ĳ���� x�� ȸ�� ����
static int SpinAngle2 = 0;//ĳ���� y�� ȸ�� ����

// �� ������ ���� �Ҹ��� ������
GLboolean IsSphereSmall = false; GLboolean IsSphereLarge = false;
GLboolean IsTorusSmall = false; GLboolean IsTorusLarge = false;
GLboolean IsTeapotSmall = false; GLboolean IsTeapotLarge = false;
GLboolean IsRightRotate = false; GLboolean IsUpRotate = false;

int  arrowKey = 0;// � ����Ű�� ���������� ���� ����
int gameStart = 0; //���ӻ��º���
int gameScore = 0; //���ھ��


float r = 0.0, g = 0.5, b = 0.5;//����� �ʱⰪ����
float posx = 0.0, posy = 0.0;//ĳ������ġ ����
float speed = 0.015;

const GLfloat red[] = { 0.8f, 0.0, 0.0, 1.0 };
const GLfloat blue[] = { 0.0, 0.2f, 1.0, 1.0 };
const GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
const GLfloat polished[] = { 100.0 };
const GLfloat dull[] = { 0.0 };
const GLfloat light_pos[] = { 0.0, 0.0, 0.0, 1.0 };


GLfloat ref_plane[] = { 2, 2, 2, 0. }; //�ؽ��� �������
GLUquadricObj* qobj; //��ü ������
#define stripeImageWidth 32
GLubyte stripeImage[4 * stripeImageWidth]; //�ؽ��� �迭
void MyStripeImage() { //�ؽ��� �����Լ�
	for (int j = 0; j < stripeImageWidth; j++) {
		stripeImage[4 * j] = 255;
		stripeImage[4 * j + 1] = (j < 8) ? 0 : 255;
		stripeImage[4 * j + 2] = (j < 8) ? 0 : 255;
		stripeImage[4 * j + 3] = 0;
	}
}

/*������Ŭ����*/
class Item {
public:
	float posx;
	float posy;
	float radius = 0.05;
	Item() {
		posx = ((rand() % 34) - 17.0) / 10.0;
		posy = ((rand() % 34) - 17.0) / 10.0;
	}
	float increaseSpeed = 0.005;

	
};

/*���ʹ� Ŭ����*/
class enemy{
public:
	float posx;
	float posy;
	float radius=0.05;
	float directionX = ((rand() % 7) - 3.0) / 100.0;
	float directionY = ((rand() % 7) - 3.0) / 100.0;
	enemy() {
		posx = ((rand() % 34) - 17.0) / 10.0;
		posy = ((rand() % 34) - 17.0) / 10.0;
	}
	/*�΋H���� ���鿡 ���� ���Ⱚ �������ִ� �Լ���*/

	void changeDirectionUp() {
		directionX = ((rand() % 7) - 3.0) / 100.0;
		directionY = ((rand() % 3)+1.0) / 100.0;
		//cout << "changeDir";
	}
	void changeDirectionDown() {
		directionX = ((rand() % 7) - 3.0) / 100.0;
		directionY = ((rand() % 3) -3.0) / 100.0;
		//cout << "changeDir";
	}
	void changeDirectionRight() {
		directionX = ((rand() % 3) +1.0) / 100.0;
		directionY = ((rand() % 7) - 3.0) / 100.0;
		//cout << "changeDir3";
	}
	void changeDirectionLeft() {
		directionX = ((rand() % 3) - 3.0) / 100.0;
		directionY = ((rand() % 7) - 3.0) / 100.0;
		//cout << "changeDir";
	}
};

const int enemyNum= 10; //�� �ִ� ����
int enemyCount = 0; //�� ���� ī��Ʈ
enemy enemyArr[enemyNum] = {};//������ �� ���� �迭

const int ItemNum = 2; //������ �ִ� ����
int ItemCount = 0; //������ ���� ī��Ʈ
Item ItemArr[ItemNum] = {};//������ ������ ���� �迭


/*�޴�Ŭ���ϸ� �ʱ�ȭ�ϰ� �������� ����*/
void clearBool() {
	IsSphereSmall = false;
	IsSphereLarge = false;
	IsTorusSmall = false;
	IsTorusLarge = false;


}

void InitLight() { //���� Ư��
	GLfloat light0_ambient[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat light0_diffuse[] = { 0.8, 0.7, 0.6, 1.0 };
	GLfloat light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	
	glEnable(GL_DEPTH_TEST); //���� ���� Ȱ��ȭ
	glEnable(GL_LIGHTING); //���� Ȱ��ȭ
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
}
void MyDisplay() { // ������ ���� ��ǥ�迡 ��ġ
	GLfloat LightPosition[] = { 0.0, 0.0, 1.5, 1.0 };
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	
	for (int i = 0; i < enemyCount; i++) {
		GLfloat Shapecolor[] = { 0.5,0.5,0.5 };
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Shapecolor);
		glPushMatrix();
		glTranslatef(enemyArr[i].posx, enemyArr[i].posy, 0);
		glutSolidSphere(0.05, 10, 10);
		glPopMatrix();
	}

	for (int i = 0; i < ItemCount; i++) {
		GLfloat Shapecolor[] = { 0.1,0.9,0.1 };
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Shapecolor);
		glPushMatrix();
		glTranslatef(ItemArr[i].posx, ItemArr[i].posy, 0);
		glutSolidSphere(0.05, 10, 10);
		glPopMatrix();
	}

	if (IsSphereSmall) {
		GLfloat Shapecolor[] = { r,g,b };
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Shapecolor);
		glPushMatrix();
		glTranslatef(posx,posy,0);
		glRotatef(SpinAngle, 1, 0, 0.0);
		glRotatef(SpinAngle2, 0, 1, 0.0);
		glutSolidSphere(0.1, 10, 10);
		glPopMatrix();
	}
	else if (IsSphereLarge) {
		GLfloat Shapecolor[] = { r,g,b };
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Shapecolor);
		glPushMatrix();
		glTranslatef(posx, posy, 0);
		glRotatef(SpinAngle, 1, 0, 0.0);
		glRotatef(SpinAngle2, 0, 1, 0.0);
		glutWireSphere(0.1, 10, 10);
		glPopMatrix();
	}
	else if (IsTorusSmall) {
		GLfloat Shapecolor[] = { r,g,b };
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Shapecolor);
		glPushMatrix();
		glTranslatef(posx, posy, 0);
		glRotatef(SpinAngle, 1, 0, 0.0);
		glRotatef(SpinAngle2, 0, 1, 0.0);
		glutSolidTorus(0.02, 0.07, 20, 10);
		glPopMatrix();
	}
	else if (IsTorusLarge) {
		GLfloat Shapecolor[] = { r,g,b };
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Shapecolor);
		glPushMatrix();
		glTranslatef(posx, posy, 0);
		glRotatef(SpinAngle, 1, 0, 0.0);
		glRotatef(SpinAngle2, 0, 1, 0.0);
		glutWireTorus(0.02, 0.07, 20, 10);
		glPopMatrix();
	}
	else if (IsTeapotSmall) {
		GLfloat Shapecolor[] = { r,g,b };
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Shapecolor);
		glPushMatrix();
		glRotatef(SpinAngle, 1, 0, 0.0);
		glRotatef(SpinAngle2, 0, 1, 0.0);
		glutSolidTeapot(0.2);
		glPopMatrix();
	}
	else if (IsTeapotLarge) {
		GLfloat Shapecolor[] = { r,g,b };
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Shapecolor);
		glPushMatrix();
		glRotatef(SpinAngle, 1, 0, 0.0);
		glRotatef(SpinAngle2, 0, 1, 0.0);
		glutSolidTeapot(0.3);
		glPopMatrix();
	}


	glFlush();
}

void MyReshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0, 0, 0, -100, 0, 1, 0);
}

/*���� �� ĳ������ �浹�����ϰ� �ε����� ������ �����޽��� ���*/
/*�����۰� �� ĳ������ �浹�����ϰ� �ε����� ���ǵ�� �ϰ� ���*/
void checkCollide() {
	if (gameStart == 1) {
		for (int i = 0; i < enemyCount; i++) {
			if (sqrtf((posx - enemyArr[i].posx)*(posx - enemyArr[i].posx) + (posy - enemyArr[i].posy)*(posy - enemyArr[i].posy)) < 0.15) {
				cout << "you DEAD! your score is "<<endl;
				cout << gameScore;
				gameStart = 2;
			}
		}
		for (int i = 0; i < ItemCount; i++) {
			if (sqrtf((posx - ItemArr[i].posx)*(posx - ItemArr[i].posx) + (posy - ItemArr[i].posy)*(posy - ItemArr[i].posy)) < 0.15) {
				cout << "you get speedItem!"<<endl;
				speed = speed+0.005;
				for (i; i < ItemCount-1; i++) {
					ItemArr[i].posx = ItemArr[i + 1].posx;
					ItemArr[i].posy = ItemArr[i + 1].posy;
				}
				ItemCount--;
			}
		}

	}
}

void MyTimer(int value) {
	if (gameStart != 2) {
		if (arrowKey == 1 && posx < 1.7) {
			posx = posx + speed;
			glutPostRedisplay();
		}
		//���� �������� ������ ������ �ʰ� �������� �̵�
		else if (arrowKey == 2 && posx > -1.7) {
			posx = posx - speed;
			glutPostRedisplay();
		}
		//�Ʒ��� �������� ������ ������ �ʰ� �Ʒ������� �̵�
		else if (arrowKey == 3 && posy > -1.7) {
			posy = posy - speed;
			glutPostRedisplay();
		}
		//���� �������� ������ ������ �ʰ� �������� �̵�
		else if (arrowKey == 4 && posy < 1.7) {
			posy = posy + speed;
			glutPostRedisplay();
		}

		checkCollide();
		gameScore = gameScore + 1;
		glutTimerFunc(20, MyTimer, 1);
	}
}

/*�� ���� Ÿ�̸�*/
void MyEnemySpawnTimer(int value) {
	if (gameStart != 2) {
		if (gameStart == 1) {
			if (enemyCount < enemyNum) {
				enemy MyEnemy;
				enemyArr[enemyCount] = MyEnemy;
				enemyCount++;
			}
		}
		glutTimerFunc(800, MyEnemySpawnTimer, 1);
	}
}

/*������ ���� Ÿ�̸�*/
void MyItemSpawnTimer(int value) {
	if (gameStart != 2) {
		if (gameStart == 1) {
			if (ItemCount < ItemNum) {
				Item MyItem;
				ItemArr[ItemCount] = MyItem;
				ItemCount++;
			}
		}
		glutTimerFunc(4000, MyItemSpawnTimer, 1);
	}
}

/*�� ������ Ÿ�̸�*/
void EnemyMoveTimer(int value) {
	if (gameStart != 2) {
		if (gameStart == 1) {

			for (int i = 0; i < enemyCount; i++) {

				if (enemyArr[i].posx >= 1.73) {
					//cout << "1";
					enemyArr[i].changeDirectionLeft();
					enemyArr[i].posx = enemyArr[i].posx + enemyArr[i].directionX;
					enemyArr[i].posy = enemyArr[i].posy + enemyArr[i].directionY;
				}
				else if (enemyArr[i].posx <= -1.73) {
					//cout << "2";
					enemyArr[i].changeDirectionRight();
					enemyArr[i].posx = enemyArr[i].posx + enemyArr[i].directionX;
					enemyArr[i].posy = enemyArr[i].posy + enemyArr[i].directionY;
				}
				else if (enemyArr[i].posy >= 1.73) {
					//cout << "3";
					enemyArr[i].changeDirectionDown();
					enemyArr[i].posx = enemyArr[i].posx + enemyArr[i].directionX;
					enemyArr[i].posy = enemyArr[i].posy + enemyArr[i].directionY;
				}
				else if (enemyArr[i].posy <= -1.73) {
					//cout << "4";
					enemyArr[i].changeDirectionUp();
					enemyArr[i].posx = enemyArr[i].posx + enemyArr[i].directionX;
					enemyArr[i].posy = enemyArr[i].posy + enemyArr[i].directionY;

				}
				else {
					enemyArr[i].posx = enemyArr[i].posx + enemyArr[i].directionX;
					enemyArr[i].posy = enemyArr[i].posy + enemyArr[i].directionY;
					//cout << "d";
				}
			}
		}
		glutPostRedisplay();
		glutTimerFunc(20, EnemyMoveTimer, 1);
	}
}
	

void MyKeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'a':
		IsRightRotate = 1;
		IsUpRotate = 0;
		SpinAngle2 = (SpinAngle2 - 15) % 360;
		glutPostRedisplay();
		break;
	case 's':
		IsRightRotate = 0;
		IsUpRotate = 1;
		SpinAngle = (SpinAngle + 15) % 360;
		glutPostRedisplay();
		break;
	case 'd':
		IsRightRotate = 1;
		IsUpRotate = 0;
		SpinAngle2 = (SpinAngle2 + 15) % 360;
		glutPostRedisplay();
		break;
	case 'w':
		IsRightRotate = 0;
		IsUpRotate = 1;
		SpinAngle = (SpinAngle - 15) % 360;
		glutPostRedisplay();
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void MySpecial(int key, int x, int y) {
	switch (key) {
		//������ ����Ű�� 1�� �Ҵ�
	case GLUT_KEY_RIGHT:
		arrowKey = 1;
		gameStart = 1;
		break;
		//���� ����Ű�� 2�� �Ҵ�
	case GLUT_KEY_LEFT:
		arrowKey = 2;
		gameStart = 1;
		break;
		//�Ʒ��� ����Ű�� 3�� �Ҵ�
	case GLUT_KEY_DOWN:
		arrowKey = 3;
		gameStart = 1;
		break;
		//���� ����Ű�� 4�� �Ҵ�
	case GLUT_KEY_UP:
		arrowKey = 4;
		gameStart = 1;
		break;

	default:
		break;
	}
	glutPostRedisplay();
}

void MyMainMenu(int entryID) {
	if (entryID == 3) exit(0);
	glutPostRedisplay();
}
//����� �޴�
void MySphereMenu(int entryID) {
	clearBool();
	if (entryID == 1) IsSphereSmall = true;
	else if (entryID == 2) IsSphereLarge = true;
	glutPostRedisplay();
}
//�䷯�� �޴�
void MyTorusMenu(int entryID) {
	clearBool();
	if (entryID == 1) IsTorusSmall = true;
	else if (entryID == 2) IsTorusLarge = true;
	glutPostRedisplay();
}

//�÷� �޴�
void MyColorMenu(int entryID) {
	if (entryID == 1) { r = 0.8, g = 0.0, b = 0.0; }
	else if (entryID == 2) { r = 0.0, g = 0.8, b = 0.0; }
	else if (entryID == 3) { r = 0.0, g = 0.0, b = 0.8; }
	glutPostRedisplay();
}

void MyInit() {
	

	//�� �޴��� ������Ű��
	GLint MySphereID = glutCreateMenu(MySphereMenu);
	glutAddMenuEntry("Solid", 1);
	glutAddMenuEntry("Wire", 2);

	GLint MyTorusID = glutCreateMenu(MyTorusMenu);
	glutAddMenuEntry("Solid", 1);
	glutAddMenuEntry("Wire", 2);

	GLint MyColorID = glutCreateMenu(MyColorMenu);
	glutAddMenuEntry("Red", 1);
	glutAddMenuEntry("Green", 2);
	glutAddMenuEntry("Blue", 3);

	GLint MyMainMenuID = glutCreateMenu(MyMainMenu);
	glutAddSubMenu("DrawSphere", MySphereID);
	glutAddSubMenu("DrawTorus", MyTorusID);
	glutAddSubMenu("ChangeColor", MyColorID);


	//��Ŭ���ϸ� ��������
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	MyStripeImage(); // Texture Image �����
// Texture image �ؽ��� �޸𸮿� �ø���
	glTexImage1D(GL_TEXTURE_1D, 0, 4, stripeImageWidth, 0, GL_RGBA, GL_UNSIGNED_BYTE, stripeImage);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR); // s�࿡ ���� �ؽ��� ��ǥ �ڵ� Mapping
	glTexGenfv(GL_S, GL_OBJECT_PLANE, ref_plane); // ����������κ��� �Ÿ��� ������ Texture ��ǥ ����
	glEnable(GL_TEXTURE_GEN_S); // s�࿡ ���� �ؽ��� �ڵ� Mapping Ȱ��ȭ
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT); // �ؽ��� �Ķ���� ���
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glEnable(GL_TEXTURE_1D); // �ؽ��� ���� Ȱ��ȭ
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(800, 0);
	glutCreateWindow("Light Animation");
	MyInit();
	InitLight();
	
	glutKeyboardFunc(MyKeyboard);
	glutDisplayFunc(MyDisplay);
	glutSpecialFunc(MySpecial);
	glutTimerFunc(20, MyTimer, 1);
	glutTimerFunc(100, MyEnemySpawnTimer, 1);
	glutTimerFunc(4000, MyItemSpawnTimer, 1);
	glutTimerFunc(20, EnemyMoveTimer, 1);
	glutReshapeFunc(MyReshape);

	glutMainLoop();
	return 0;
}