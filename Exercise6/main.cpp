#include <GL/glut.h>

constexpr const auto WIDTH = 500;
constexpr const	auto HEIGHT = 500;

GLfloat xrot = 0;
GLfloat yrot = 0;
GLfloat xold = 0;
GLfloat yold = 0;


void set_light() {
	static const GLfloat light_position[] = { 0.0f, 5.0f, -16.0f, 1.0f };
	static const GLfloat light1_position[] = { 0.0f, 5.0f, -7.0f, 1.0f };
	static const GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	static const GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	static const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

void display() {

	//�����Ļ
	glClearColor(0.5, 0.5, 0.5, 0.5);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//�����ӵ�
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f, 0.0f, 28.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f);
	glRotatef(xrot, 1.0, 0.0, 0.0);
	glRotatef(yrot, 0.0, 1.0, 0.0);
	//glTranslatef(0, 0, 3);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	//���ù�Դ
	set_light();

	//�������
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//��һ����
	{
		GLfloat ambient[] = { 0.0,0.52,0.0,1.0 };
		GLfloat diffuse[] = { 0.86,0.54,0.0,1.0 };

		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);

		glTranslated(0.0, 0.0, -16.0);
		glColor4f(1, 0, 0, 1);
		glutSolidTeapot(4.5);
	}
	glPopMatrix();
	glPushMatrix();
	//�ڶ�����
	{
		GLfloat ambient[] = { 0.12,0.0,0.0,1.0 };
		GLfloat diffuse[] = { 0.81,0.0,0.0,1.0 };

		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);

		glTranslated(0, 0, -7);
		glutSolidTeapot(3);
	}
	glPopMatrix();
	glPushMatrix();

	glDepthMask(GL_FALSE);
	//��������
	{
		GLfloat ambient[] = { 0.2,0.63,0.5,1 };
		GLfloat diffuse[] = { 0.57,0.83,0.62,0.2 };

		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);

		glTranslated(0, 0, -1.15);
		glutSolidTeapot(2);

	}
	glPopMatrix();
	glPushMatrix();
	//���ĸ���
	{
		GLfloat ambient[] = { 0.2,0.3,0.2,1 };
		GLfloat diffuse[] = { 1,1,1,0.35 };

		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);

		glTranslated(0, 0, 3);
		glutSolidTeapot(1.2);
	}
	glPopMatrix();
	glPushMatrix();
	//�������
	{
		GLfloat ambient[] = { 0.2,0.63,0.5,1 };
		GLfloat diffuse[] = { 0.57,0.83,0.62,0.2 };

		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);

		glTranslated(0, 0, 6);
		glutSolidTeapot(1);
	}

	glDepthMask(GL_TRUE);
	glPopMatrix();
	glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		xold = x - yrot;
		yold = -y + xrot;
	}
}

void mouse_motion(int x, int y) {
	yrot = x - xold;
	xrot = y + yold;

	glutPostRedisplay();
}

void reshape(int w, int h) {
	float aspect = (float)w / (h ? h : 1);//ƽ��ͷ����ݺ�ȣ�Ҳ���ǿ�ȳ��Ը߶ȣ�(h)?h:1��˼����h=0����h=1

	//glViewport(0, 0, w, h);

	//����ͶӰ�任ǰ�������������������������ı任������Ӱ��ͶӰ����
	//�ڴ��ڸı亯��reshape������glMatrixMode(GL_PROJECTION)������׶,����glMatrixMode(GL_MODELVIEW)��Ϊģ�ͱ任
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(75.0f, aspect, 1.0f, 100.0f);
	glViewport(0, 0, w, h);

	glMatrixMode(GL_MODELVIEW);

}

//��Ļ����ת����������
void GLPOS(int x,int y,GLdouble pos[3]){
	//ģ�ͣ�ͶӰ����,�ӿ�
	GLint viewport[4];
	GLdouble modelview[16], projection[16];
	GLfloat winx, winy, winz;

	//���ͶӰ����,���ģ�;���,����ӿ�
	glGetDoublev(GL_MODELVIEW, modelview);
	glGetDoublev(GL_PROJECTION, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	winx = (GLfloat)x;
	winy = viewport[3] - (GLfloat)y;

	//�����Ļ���ض�Ӧ�������������ֵ  
	glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winz);
	//�����Ļ�����Ӧ����������
	gluUnProject(winx, winy, winz, modelview, projection, viewport, &pos[0], &pos[1], &pos[2]);

}
int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("BLEND");

	//glEnable(GL_DEPTH_TEST);

	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(mouse_motion);
	glutReshapeFunc(reshape);

	glutMainLoop();
	return 0;
}