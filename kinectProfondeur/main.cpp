#include <iostream>
#include <cstdlib>
#include <libfreenect.hpp>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <pthread.h>

#include <time.h>
#include <unistd.h>

using namespace std;

freenect_context *f_ctx;
freenect_device *f_dev;

uint8_t *depth_mid, *depth_front;

GLuint depthText;

pthread_t kinectThread;
pthread_mutex_t kinectMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t frame = PTHREAD_COND_INITIALIZER;

int profondeurLimite = 700;

bool continuer = true;
bool gotDepth = false;

int window;

int WinWidth, WinHeight;

/**
 * Dessin
 */
void Draw(){

	pthread_mutex_lock(&kinectMutex);

	while(!gotDepth)
		pthread_cond_wait(&frame, &kinectMutex);

	uint8_t *tmp;

	/*if(gotDepth){
		tmp = depth_front;
		depth_front = depth_mid;
		depth_mid = tmp;
		gotDepth = false;
	}*/

	pthread_mutex_unlock(&kinectMutex);

	glBindTexture(GL_TEXTURE_2D, depthText);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, depth_mid);

	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_QUADS);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glTexCoord2f(1, 0); glVertex3i(-25,0,0);
	glTexCoord2f(0, 0); glVertex3i(WinWidth,0,0);
	glTexCoord2f(0, 1); glVertex3i(WinWidth,WinHeight,0);
	glTexCoord2f(1, 1); glVertex3i(-25,WinHeight,0);
	glEnd();

	glutSwapBuffers();
}

/**
 * Redimmensionnement
 */
void Resize(int width, int height){
	glViewport(0,0,width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,width, height, 0, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	WinWidth = width;
	WinHeight = height;
}

/**
 * Initialisation OpenGL
 */
void InitGL(int width, int height){

	glClearColor(1.0f, 1.0f, 1.0f, 0.2f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_FLAT);

	glGenTextures(1, &depthText);
	glBindTexture(GL_TEXTURE_2D, depthText);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	Resize(width, height);
}

/**
 * Traitement des données de profondeur
 */
void depth_cb(freenect_device* dev, void* v_depth, uint32_t timestamp){

	uint16_t* depth =(uint16_t*)v_depth;

	pthread_mutex_lock(&kinectMutex);

	int R = 128;
	int V = 0;
	int B = 0;

	for(int i = 0; i < 640*480; ++i){
		if(depth[i] <= profondeurLimite){
			depth_mid[3*i+0] = R;
			depth_mid[3*i+1] = V;
			depth_mid[3*i+2] = B;
		}
	}
	gotDepth = true;
	pthread_cond_signal(&frame);
	pthread_mutex_unlock(&kinectMutex);

}

/**
 * Thread de capture des donnée du Kinect
 */
void * kinectCapt(void* arg){

	cout << "Thread de capture des données lancé." << endl;

	freenect_set_led(f_dev, LED_BLINK_GREEN);

	freenect_set_depth_callback(f_dev, depth_cb);
	freenect_set_depth_mode(f_dev, freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM,FREENECT_DEPTH_11BIT ));

	freenect_start_depth(f_dev);

	while(continuer && freenect_process_events(f_ctx) >= 0){

	}

	cout << "Liberation du kinect... ";

	freenect_set_led(f_dev, LED_OFF);

	freenect_stop_depth(f_dev);
	freenect_close_device(f_dev);
	freenect_shutdown(f_ctx);

	cout << "OK!" << endl;

	return NULL;
}

void onKeyDown(unsigned char key, int x, int y){
	if(key == 27){
		continuer = false;
		delete[] depth_mid;
		delete[] depth_front;
		pthread_join(kinectThread, NULL);
		glutDestroyWindow(window);
		exit(0);
	}
	if(key == 'z'){
		profondeurLimite -= 10;
		cout << profondeurLimite << endl;
	}
	if(key == 's'){
		profondeurLimite += 10;
		cout << profondeurLimite << endl;
	}
}

int main(int argc, char **argv) {

	srand(time(NULL));

	/**
	 * Allocation pour dessin texture
	 */

	depth_mid = new uint8_t[640*480*3];
	for(int i = 0; i < 640*480; ++i){
		depth_mid[i*3 + 0] = 255;
		depth_mid[i*3 + 1] = 255;
		depth_mid[i*3 + 2] = 255;
	}

	/**
	 * Initialisation du KINECT
	 */

	int res;

	cout << "Initialisation du pilote: ";
	flush(cout);

	if (freenect_init(&f_ctx, NULL) < 0) {
		cout << "Erreur d'initialisation." << endl;
		return 1;
	}

	cout << "Pilote initialisé." << endl;

	freenect_set_log_level(f_ctx, FREENECT_LOG_DEBUG);

	freenect_select_subdevices(f_ctx,(freenect_device_flags) (FREENECT_DEVICE_MOTOR | FREENECT_DEVICE_CAMERA));

	int devices_number = freenect_num_devices(f_ctx);
	cout << "Kinect détecté: " << devices_number << endl;

	if (devices_number < 1) {
		cout << "Erreur: pas de kinect branché. Fermeture." << endl;
		return 1;
	}

	cout << "Initialisation du Kinect: ";
	flush(cout);
	if (freenect_open_device(f_ctx, &f_dev, 0) < 0) {
		cout << "Erreur d'initialisation." << endl;
		return 1;
	}
	cout << "OK!" << endl;

	/**
	 * Lancement du Thread de capture des données
	 */

	res = pthread_create(&kinectThread, NULL, kinectCapt, NULL);
	if(res){
		cout << "Erreur lors de la création du thread de capture des données." << endl;
		return 1;
	}

	/**
	 * Initialisation de la fenetre OPENGL
	 */

	cout << "Initialisation fenetre: ";

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

	window = glutCreateWindow("Kinect profondeur");

	glutDisplayFunc(&Draw);
	glutIdleFunc(&Draw);
	glutReshapeFunc(&Resize);
	glutKeyboardFunc(&onKeyDown);

	cout << "OK!" << endl;

	InitGL(640, 480);

	glutFullScreen();

	glutMainLoop();

	return 0;
}
