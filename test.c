//http://antongerdelan.net/opengl/hellotriangle.html

#ifdef WIN_32
#define _CRT_SECURE_NO_DEPRECIATE
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "GLutilities.h"
#include "screenshot.h"

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>




#define GLSL_FRAGMENT_SHADER "mandelfrag.glsl"
#define GLSL_VERTEX_SHADER "test_vs.glsl"


int main() {

	//restart the log
	restart_gl_log();

	//call utility function to start GL and create window
	start_gl(-1, -1, "MANDELBROT");

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

	//define points for rectangle
	float points[] = {
		1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 
		-1.0f, 1.0f, 0.0f
	};

	//define colors for rectangle
	float colors[] = {
		1.0f, 0.0f, 0.0f, 
		0.0f, 1.0f, 0.0f, 
		0.0f, 0.0f, 1.0f, 
		1.0f, 1.0f, 1.0f,
	};

	GLuint rect = VAO_new(points, colors);
	
	GLuint shader_program = Shader_new(GLSL_VERTEX_SHADER, GLSL_FRAGMENT_SHADER);

	//don't waste time drawing hidden objects
	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK);		// cull back face
	glFrontFace(GL_CW);			// GL_CCW for counter clock-wise


	//Render stuff
	while (!glfwWindowShouldClose(g_window)) {
		//frame rate counter
		//_update_fps_counter(g_window);

		//wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		//if window not fullscreen
		//glViewport(0,0,g_gl_width, g_gl_height);

		Render(rect, shader_program, GL_TRIANGLE_FAN, 4);

		//take screenshot
		//screenshotBMP("test.bmp");

		//update other events
		glfwWaitEvents();

		//handle events
		if (GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(g_window, 1);
		}

		//display stuff
		glfwSwapBuffers(g_window);
	}


	//close GL Window
	glfwDestroyWindow(g_window);

	// close GL context and any other GLFW resources
	glfwTerminate();

	return 0;
}

