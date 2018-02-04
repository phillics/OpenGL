#pragma once
/************************************************************************************
OpenGL Utilities

Purpose:  Provides basic graphics capabilities through GLFW and GLEW

Log
OpenGL
Window
Event
VAO
Shader
Render

Author:     Spencer Phillips
Last Edit:	8/24/2017

TODO:

frame rate

Seperate Files:
shapes:		definitions for creating and using basic shapes
	points, lines, triangle, quad, poly, circle, elipse, arc

<filename> shapes.c
<contains> 
	code for vertex and frag shaders
	creates shapes given arrays of points and colors...

graphing:  	useful tools to construct graphs
************************************************************************************/

#pragma once
#ifndef GL_UTILS_H
#define GL_UTILS_H



//Graphics Libraries
#include <GL/glew.h>	//include GLEW and a new version of GL on Windows
#include <GLFW/glfw3.h>	//GLFW helper library

//General Libraries
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>		//for ... args

//definitions 
#define GL_LOG_FILE "gl.log"
#define MAX_SHADER_LENGTH 262144

/************************************************************************************
GLOBAL VARIABLES
************************************************************************************/

//GLFW Window Variables
int g_gl_width;
int g_gl_height;
GLFWwindow *g_window;


//MANDELFRAG
//double mand_scale = 0.001;


//Frame Rate
double previous_seconds;
int frame_count;


/************************************************************************************
GENERAL UTILITIES

These functions are for housekeeping::: will eventually be moved
************************************************************************************/

char* readFileToString(const char *file_name, int max_length);



/************************************************************************************
LOG FILE		PRINT FUNCTIONS
These functions
************************************************************************************/
int restart_gl_log();
int gl_log(const char *message, ...);
int gl_log_error(const char *message, ...);
void gl_log_params();

const char* GL_type_to_string(GLenum type);
//void print_shader_info_log(GLuint shader_index);
//void print_program_info_log(GLuint sp);
//void print_all(GLuint sp);

//const char* GL_type_to_string(unsigned int type);

/************************************************************************************
GLFW FUNCTIONS
************************************************************************************/

/*Initializes OpenGL, sets up log files, and sets up callback functions
Also creates a glfw window of size (width x height)
	if you want the window to be full screen, call start_gl(NULL, NULL, "title");
*/
int start_gl(int width, int height, char* windowTitle);




/************************************************************************************
EVENT FUNCTIONS
These are mostly program specific and are largely unimplemented
By default, esc key will close window.
************************************************************************************/

void Event_ErrorCallback(int error, const char* description);

void Event_WindowSizeCallback(GLFWwindow* window, int width, int height);

void Event_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

void Event_CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);

void Event_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

/************************************************************************************
VAO FUNCTIONS
************************************************************************************/

GLuint VAO_new(float* points, float* colors);

void VAO_delete(GLuint vao);

/************************************************************************************
SHADER FUNCTIONS
************************************************************************************/

GLuint Shader_new(const char *vert_file_name, const char *frag_file_name);

void Shader_delete(GLuint sp);


/************************************************************************************
RENDER FUNCTIONS
************************************************************************************/

void Render_clear();

void Render_Color(float r, float g, float b);

void Render(GLuint vao, GLuint shader, GLuint renderMode, int size);



#endif