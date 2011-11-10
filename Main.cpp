#include <iostream>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <sys/time.h>
#include <sstream>
#include <pthread.h>
#include <algorithm>
#include <vector>

#include "Buffer.h"
#include "Viewport.h"
#include "Scene.h"
#include "Sample.h"
#include "Sampler.h"

using namespace MB;

/**
  * The current size of the window
  */
int m_width = 500;
int m_height = 500;

/**
  * The current buffer.
  */
Buffer *m_buffer = new Buffer(m_width, m_height);
pthread_mutex_t m_bufferLock = PTHREAD_MUTEX_INITIALIZER;


/**
  * Version number of the current scene
  */
int m_version = 0;
pthread_mutex_t m_versionLock = PTHREAD_MUTEX_INITIALIZER;


/**
  * The size of the currently displayed samples.
  */
int m_minSampleSize = 1;
int m_maxSampleSize = 40;
int m_sampleSize = m_maxSampleSize;


/**
  * Replaces the current buffer with a new one
  */
void replaceBuffer(Buffer *buffer)
{
	pthread_mutex_lock(&m_bufferLock);
	Buffer *tmp = m_buffer;
	m_buffer = buffer;
	delete tmp;
	pthread_mutex_unlock(&m_bufferLock);
}


/**
  * The viewport.
  */
Viewport *m_viewport = new Viewport();


/**
  * The scene
  */
Scene *m_scene = new Mandelbrot();


/**
  * The time difference between frames in seconds is given
  * by dt and the frame rate by fps.
  */
float iterationDt = 0.0;
float framesPerSecond = 0.0;


/**
  * Indicates wheter key is currently pressed.
  */
bool keyPressed[128];


/**
  * Displays the contents of the buffer.
  */
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glRasterPos2i(-1, -1);
	pthread_mutex_lock(&m_bufferLock); 
	{
		glDrawPixels(m_buffer->width(), m_buffer->height(), GL_RGB, GL_FLOAT, m_buffer->data());
	}
	pthread_mutex_unlock(&m_bufferLock);

	glutSwapBuffers();
	glutPostRedisplay();
}


/**
  * Resizes the buffer if necessary.
  */
void reshape(int width, int height)
{
	pthread_mutex_lock(&m_versionLock);
	m_width = width;
	m_height = height;
	m_version++;
	pthread_mutex_unlock(&m_versionLock);
}


/**
  * Times to calculate dt between frames.
  */
struct timeval then, now;


/**
  * The main loop processes events.
  */
void processEvents()
{
	gettimeofday(&now, NULL);
	long ds = now.tv_sec - then.tv_sec;
	long du = now.tv_usec - then.tv_usec;
	iterationDt = (float) (ds + 1e-6*du);
	framesPerSecond = 1.0 / iterationDt;
	then = now;

	// General control
	if (keyPressed[27]) exit(0);

	// Movement (TODO: NE, SE, etc.)
	direction dm = NONE;
	if (keyPressed['w']) dm = UP;
	if (keyPressed['s']) dm = DOWN;
	if (keyPressed['a']) dm = LEFT;
	if (keyPressed['d']) dm = RIGHT;
	m_viewport->move(iterationDt, dm);

	direction dz = NONE;
	if (keyPressed['q']) dz = OUT;
	if (keyPressed['e']) dz = IN;
	m_viewport->zoom(iterationDt, dz);

	if (dm != NONE || dz != NONE) {
		pthread_mutex_lock(&m_versionLock);
		m_version++;
		m_sampleSize = m_maxSampleSize;
		pthread_mutex_unlock(&m_versionLock);
	} else if (m_sampleSize == m_maxSampleSize) {
		pthread_mutex_lock(&m_versionLock);
		m_version++;
		m_sampleSize = m_minSampleSize;
		pthread_mutex_unlock(&m_versionLock);
	}
}


/**
  * The main loop of the computing thread.
  */
void *computeScene(void *data) 
{
	int version = -1;

	while(true) {
		if (version == m_version) continue;							// Busy wait

		pthread_mutex_lock(&m_versionLock);
		Buffer *buffer = new Buffer(m_width, m_height);		
		version = m_version;	
		pthread_mutex_unlock(&m_versionLock);

		Sampler sampler(buffer->width(), buffer->height(), m_sampleSize);
		std::vector<Sample>::iterator it;

		// TODO parallelize further
		for (it = sampler.begin(); it != sampler.end(); it++) {
			if (m_version != version) break;						// Early abort

			m_viewport->project(*it, *buffer);
			m_scene->color(*it);
			buffer->put(*it);
		}

		if (m_version != version) {
			delete buffer;
			continue;												// Early abort
		}

		replaceBuffer(buffer);
	}

	return NULL;
}


/**
  * Handle keyboard input.
  */
void keyboard(unsigned char key, int x, int y)
{
	keyPressed[key] = true;
}


void keyboardUp(unsigned char key, int x, int y) 
{
	keyPressed[key] = false;
}


/**
  * Initialize OpenGL etc.
  */
void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	gettimeofday(&then, NULL);

	pthread_t threadId;
	if (pthread_create(&threadId, NULL, computeScene, NULL) != 0) {
		std::cerr << "Could not launch scene thread." << std::endl;
		exit(1);
	}
}


/**
  * Initializes the OpenGL, GLUT and starts the main loop.
  */
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	glutInitWindowPosition(10, 10);
	glutInitWindowSize(m_buffer->width(), m_buffer->height());

	glutCreateWindow("Mandelbrot");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(processEvents);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);

	init();
	glutMainLoop();

	return 0;
}
