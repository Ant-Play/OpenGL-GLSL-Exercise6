#include <gl/glew.h>
#include <GL/glut.h>
#include<GL/freeglut.h>
#include <cmath>

constexpr const auto PI = 3.1415926;

GLint WIDTH = 800;
GLint HEIGHT = 600;
GLfloat light0_position[] = { 0.0f, 5.0f, -16.0f, 1.0f };

//��ת����
GLfloat xrot = 0;
GLfloat yrot = 0;
GLfloat xold = 0;
GLfloat yold = 0;

GLuint framebuffer;//FBO֡�������
GLuint renderbuffer;//RBO��Ⱦ�������
GLuint texture;//֡��������Ӧ����

GLfloat aspect;
GLfloat fovy = 45.0f;
bool isRotate = false;
bool isOpenMagnifier = false;
bool isChangeMagnifierSize = false;

struct Magnifier
{
	GLint mousex, mousey;//�����ص�����ֱ�ӵõ������꣬�Դ������Ͻ�Ϊԭ��
	GLfloat worldx, worldy;//�Ŵ󾵵�ȫ�����꣬���������任�õ�
	GLfloat scale = 10.0f;
	GLfloat radius = 2.0f;
};
Magnifier magnifier;

struct camer {
	float eyePos[3] = { 0,0,28 };
	float lookAt[3] = { 0,0,0 };
	float upDir[3] = { 0,1,0 };
};
camer ca;

void set_light();

void init() {
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);			//������Ȳ���
	set_light();
}

void initFBO()
{
	glGenFramebuffers(1, &framebuffer); //����֡�������
	glGenRenderbuffers(1, &renderbuffer); //������Ⱦ�������
	glGenTextures(1, &texture);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	init();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void set_light() {
	//static const GLfloat light_position[] = { 0.0f, 5.0f, -16.0f, 1.0f };
	//static const GLfloat light1_position[] = { 0.0f, 5.0f, -7.0f, 1.0f };
	static const GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	static const GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	static const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	//glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	//glEnable(GL_DEPTH_TEST);
}

//��Ļ����ת����������
void GLPOS() {
	//ģ�ͣ�ͶӰ����,�ӿ�
	GLint viewport[4];
	GLdouble modelview[16], projection[16];
	GLfloat winx, winy, winz;
	GLdouble pos[3];

	//���ͶӰ����,���ģ�;���,����ӿ�
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	winx = (GLfloat)magnifier.mousex;
	winy = (GLfloat)viewport[3] - (GLfloat)magnifier.mousey;

	//�����Ļ���ض�Ӧ�������������ֵ  
	//glReadPixels(magnifier.mousex, magnifier.mousey, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winz);
	winz = 0.96f;

	//�����Ļ�����Ӧ����������
	gluUnProject((GLdouble)winx, (GLdouble)winy, (GLdouble)winz, modelview, projection, viewport, &pos[0], &pos[1], &pos[2]);

	magnifier.worldx = (GLfloat)pos[0];
	magnifier.worldy = (GLfloat)pos[1];
}

void draw_scene(bool mag) {

	//�����Ļ
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//�����ӵ�
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(ca.eyePos[0], ca.eyePos[1], ca.eyePos[2], ca.lookAt[0], ca.lookAt[1], ca.lookAt[2], ca.upDir[0], ca.upDir[1], ca.upDir[2]);
	glViewport(0, 0, WIDTH, HEIGHT);

	//����ͶӰ�任ǰ�������������������������ı任������Ӱ��ͶӰ����
	//�ڴ��ڸı亯��reshape������glMatrixMode(GL_PROJECTION)������׶,����glMatrixMode(GL_MODELVIEW)��Ϊģ�ͱ任
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, aspect, 1.0f, 100.0f);
	glViewport(0, 0, WIDTH, HEIGHT);

	glMatrixMode(GL_MODELVIEW);
	if (mag) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//�޸�fov
		gluPerspective(magnifier.scale, aspect, 1.0f, 100.0f);

		glMatrixMode(GL_MODELVIEW);
		glTranslated(-magnifier.worldx, -magnifier.worldy, 0);
	}else if(!mag && isOpenMagnifier){
		GLPOS();
	}
	//���ù�Դ
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	glPushMatrix();

	//��ת
	glRotatef(xrot, 1.0, 0.0, 0.0);
	glRotatef(yrot, 0.0, 1.0, 0.0);
	
	//�������
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glPushMatrix();
	//��һ����
	{
		GLfloat ambient[] = { 0.0f,0.52f,0.0f,1.0f };
		GLfloat diffuse[] = { 0.86f,0.54f,0.0f,1.0f };

		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);

		glTranslated(0.0f, 0.0f, -16.0f);
		glutSolidTeapot(4.5);
	}
	glPopMatrix();
	glPushMatrix();
	//�ڶ�����
	{
		GLfloat ambient[] = { 0.12f,0.0f,0.0f,1.0f };
		GLfloat diffuse[] = { 0.81f,0.0f,0.0f,1.0f };

		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);

		glTranslated(0.0f, 0.0f, -7.0f);
		glutSolidTeapot(3);
	}
	glPopMatrix();
	glPushMatrix();

	glDepthMask(GL_FALSE);
	//��������
	{
		GLfloat ambient[] = { 0.2f,0.63f,0.5f,1.0f };
		GLfloat diffuse[] = { 0.57f,0.83f,0.62f,0.2f };

		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);

		glTranslated(0.0f, 0.0f, -1.15f);
		glutSolidTeapot(2);

	}
	glPopMatrix();
	glPushMatrix();
	//���ĸ���
	{
		GLfloat ambient[] = { 0.2f,0.3f,0.2f,1.0f };
		GLfloat diffuse[] = { 1.0f,1.0f,1.0f,0.35f };

		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);

		glTranslated(0, 0, 3);
		glutSolidTeapot(1.2);
	}
	glPopMatrix();
	glPushMatrix();
	//�������
	{
		GLfloat ambient[] = { 0.2f,0.63f,0.5f,1.0f };
		GLfloat diffuse[] = { 0.57f,0.83f,0.62f,0.2f };

		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);

		glTranslated(0.0f, 0.0f, 6.0f);
		glutSolidTeapot(1);
	}
	glDepthMask(GL_TRUE);
	glPopMatrix();
	glPopMatrix();
}

void display() {
	draw_scene(false);

	if (isOpenMagnifier) {
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		// ����һ����ɫ��������
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);// �������ӵ���ǰ�󶨵�֡�������

		// Ϊ��Ⱥ�ģ�帽������һ��renderbuffer����
		glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

		draw_scene(true);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		draw_scene(false);

		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBegin(GL_TRIANGLE_FAN);
		glTexCoord2f(0.5, 0.5);
		glVertex2f(magnifier.worldx, magnifier.worldy);
		int n = 80;     //�����n��ʾ�ö���λ���Բ�ľ��ȣ����Կ������󾫶�
		for (double i = 0; i <= n; i++)
		{
			glTexCoord2f(0.5 + 0.5 * cos(i * 2 * PI / n), 0.5 + 0.5 * sin(i * 2 * PI / n));
			glVertex2f(magnifier.worldx + magnifier.radius * cos(i * 2 * PI / n), magnifier.worldy + magnifier.radius * sin(i * 2 * PI / n));
		}
		glEnd();

		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
	}

	glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		isRotate = true;
		xold = x - yrot;
		yold = -y + xrot;
	}else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		isRotate = false;
	}else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){

		magnifier.mousex = x;
		magnifier.mousey = y;
		isOpenMagnifier = !isOpenMagnifier;
	}
}

void mouse_motion(int x, int y) {
	if (isRotate) {
		yrot = x - xold;
		xrot = y + yold;
	}
	if (isOpenMagnifier)
	{
		magnifier.mousex = (GLfloat)x;
		magnifier.mousey = (GLfloat)y;
	}

}

void mouse_wheel(int button, int dir, int x, int y) {
	if (dir > 0 && magnifier.scale > 1) {
		magnifier.scale -= 2;
	}
	else if (dir < 0 && magnifier.scale < 10) {
		magnifier.scale += 2;
	}
}
void MousePassiveMove(int x, int y)
{

	if (isOpenMagnifier)
	{
		magnifier.mousex = (GLfloat)x;
		magnifier.mousey = (GLfloat)y;
	}
}

void reshape(int w, int h) {
	aspect = (float)w / (h ? h : 1);//ƽ��ͷ����ݺ�ȣ�Ҳ���ǿ�ȳ��Ը߶ȣ�(h)?h:1��˼����h=0����h=1

	WIDTH = w;
	HEIGHT = h;
	glViewport(0, 0, w, h);

	//����ͶӰ�任ǰ�������������������������ı任������Ӱ��ͶӰ����
	//�ڴ��ڸı亯��reshape������glMatrixMode(GL_PROJECTION)������׶,����glMatrixMode(GL_MODELVIEW)��Ϊģ�ͱ任
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(fovy, aspect, 1.0f, 100.0f);
	glViewport(0, 0, WIDTH, HEIGHT);

	glMatrixMode(GL_MODELVIEW);

}

void idle() {
	glutPostRedisplay();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("BLEND");

	//���ʹ��FBO�Ļ����˾䲻��ȱ��
	GLenum err = glewInit();

	init();
	initFBO();
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutMouseFunc(mouse);
	glutMouseWheelFunc(mouse_wheel);
	glutMotionFunc(mouse_motion);
	glutReshapeFunc(reshape);
	glutPassiveMotionFunc(MousePassiveMove);

	glutMainLoop();
	return 0;
}