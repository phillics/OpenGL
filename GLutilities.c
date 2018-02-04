#include "Glutilities.h"



/************************************************************************************
GENERAL UTILITIES

These functions are for housekeeping::: will eventually be moved
************************************************************************************/

//be careful, returns null pointer if error
char* readFileToString(const char *file_name, int max_length) {
	char* contents;
	long fileSize;

	FILE *filePtr = fopen(file_name, "rb");

	if (!filePtr) {
		printf("ERROR: File Could Not Be Opened\n");
		//log error
		gl_log_error("ERROR: File Could Not Be Opened: ", file_name);
		contents = "\0";
		return contents;
	}

	fseek(filePtr, 0, SEEK_END);
	fileSize = ftell(filePtr);
	rewind(filePtr);

	if (fileSize > max_length) {
		printf("ERROR: File Too Long To Read\n");
		gl_log_error("ERROR: File Too Long To Read: ", file_name);
		contents = "\0";
		return contents;
	}

	//allocate space for string
	contents = malloc((fileSize + 1) * (sizeof(char)));
	fread(contents, sizeof(char), fileSize, filePtr);

	fclose(filePtr);
	contents[fileSize] = 0;	//set last char to '/0'

	return contents;
}


/************************************************************************************
LOG FILE		PRINT FUNCTIONS
These functions
************************************************************************************/
int restart_gl_log() {
	FILE *file = fopen(GL_LOG_FILE, "w");

	if (!file) {
		fprintf(stderr, "ERROR: could not open GL_LOG_FILE log file %s for writing\n", GL_LOG_FILE);
		return 0;
	}

	time_t now = time(NULL);
	char *date = ctime(&now);
	fprintf(file, "GL_LOG_FILE log. local time %s\n", date);
	fclose(file);
	return 1;
}

//Takes an array of messages and appends them to the log file
int gl_log(const char *message, ...) {
	va_list argptr;
	FILE *file = fopen(GL_LOG_FILE, "a");

	if (!file) {
		fprintf(stderr, "ERROR: could not open GL_LOG_FILE %s file for appending\n", GL_LOG_FILE);
		return 0;
	}

	va_start(argptr, message);
	vfprintf(file, message, argptr);
	va_end(argptr);

	fclose(file);
	return 1;
}

//Same as gl_log, but also displays messages in stderr
int gl_log_error(const char *message, ...) {
	va_list argptr;
	FILE *file = fopen(GL_LOG_FILE, "a");
	if (!file) {
		fprintf(stderr, "ERROR: could not open GL_LOG_FILE %s file for appending\n",
			GL_LOG_FILE);
		return 0;
	}
	//print to file
	va_start(argptr, message);
	vfprintf(file, message, argptr);
	va_end(argptr);

	//print to stderr
	va_start(argptr, message);
	vfprintf(stderr, message, argptr);
	va_end(argptr);

	fclose(file);
	return 1;
}

//finds values of OpenGL parameters and prints them to log file
void gl_log_params() {
	GLenum params[] = {
		GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
		GL_MAX_CUBE_MAP_TEXTURE_SIZE,
		GL_MAX_DRAW_BUFFERS,
		GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
		GL_MAX_TEXTURE_IMAGE_UNITS,
		GL_MAX_TEXTURE_SIZE,
		GL_MAX_VARYING_FLOATS,
		GL_MAX_VERTEX_ATTRIBS,
		GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
		GL_MAX_VERTEX_UNIFORM_COMPONENTS,
		GL_MAX_VIEWPORT_DIMS,
		GL_STEREO,
	};
	const char* names[] = {
		"GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
		"GL_MAX_CUBE_MAP_TEXTURE_SIZE",
		"GL_MAX_DRAW_BUFFERS",
		"GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
		"GL_MAX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_TEXTURE_SIZE",
		"GL_MAX_VARYING_FLOATS",
		"GL_MAX_VERTEX_ATTRIBS",
		"GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_VERTEX_UNIFORM_COMPONENTS",
		"GL_MAX_VIEWPORT_DIMS",
		"GL_STEREO",
	};
	gl_log("GL Context Params:\n");
	//char msg[256];
	// integers - only works if the order is 0-10 integer return types
	for (int i = 0; i < 10; i++) {
		int v = 0;
		glGetIntegerv(params[i], &v);
		gl_log("%s %i\n", names[i], v);
	}
	// others
	int v[2];
	v[0] = v[1] = 0;
	glGetIntegerv(params[10], v);
	gl_log("%s %i %i\n", names[10], v[0], v[1]);
	unsigned char s = 0;
	glGetBooleanv(params[11], &s);
	gl_log("%s %u\n", names[11], (unsigned int)s);
	gl_log("-----------------------------\n");
}

//look up glGetActiveUniform to get the rest of the list
const char* GL_type_to_string(GLenum type) {
	switch (type) {
	case GL_BOOL: return "bool";
	case GL_INT: return "int";
	case GL_FLOAT: return "float";
	case GL_FLOAT_VEC2: return "vec2";
	case GL_FLOAT_VEC3: return "vec3";
	case GL_FLOAT_VEC4: return "vec4";
	case GL_FLOAT_MAT2: return "mat2";
	case GL_FLOAT_MAT3: return "mat3";
	case GL_FLOAT_MAT4: return "mat4";
	case GL_SAMPLER_2D: return "sampler2D";
	case GL_SAMPLER_3D: return "sampler3D";
	case GL_SAMPLER_CUBE: return "samplerCube";
	case GL_SAMPLER_2D_SHADOW: return "sampler2DShadow";
	default: break;
	}
	return "other";

}

/*Shader Debugging Function: Identifies which line of which shader has an error*/
void gl_log_shader(GLuint shader_index) {
	int max_length = 2048;
	int actual_length = 0;
	char log[2048];
	glGetShaderInfoLog(shader_index, max_length, &actual_length, log);
	printf("shader info log for GL index %i:\n%s\n", shader_index, log);
	gl_log("shader info log for GL index %i:\n%s\n", shader_index, log);
}

void gl_print_shader_info_log(GLuint program) {
	int max_length = 2048;
	int actual_length = 0;
	char log[2048];
	glGetProgramInfoLog(program, max_length, &actual_length, log);
	printf("program info log for GL index %u:\n%s", program, log);
}

void gl_print_all(GLuint program) {
	printf("----------------------------\nshader program %i info:\n", program);
	int params = -1;
	glGetProgramiv(program, GL_LINK_STATUS, &params);
	printf("GL_LINK_STATUS = %i\n", params);

	glGetProgramiv(program, GL_ATTACHED_SHADERS, &params);
	printf("GL_ATTACHED SHADERS = %i\n", params);
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &params);


	printf(" GL_ACTIVE_ATTRIBUTES = %i\ n", params); 
	for (GLuint i = 0; i < (GLuint)params; i++) {
		char name[64];
		int max_length = 64;
		int actual_length = 0;
		int size = 0;
		GLenum type;
		glGetActiveAttrib(program, i, max_length, &actual_length, &size, &type, name);
		if (size > 1) {
			for (int j = 0; j < size; j++) {
				char long_name[64];
				sprintf(long_name, "% s[% i]", name, j);
				int location = glGetAttribLocation(program, long_name);
				printf(" %i) type:% s name:% s location:% i\ n", i, GL_type_to_string(type), long_name, location);
			}
		}
		else {
			int location = glGetAttribLocation(program, name);
			printf("     %i)   type:%s  name:%s  location:%i\n", i, GL_type_to_string(type), name, location);
		}
	}

	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &params); 
	printf(" GL_ACTIVE_UNIFORMS = %i\ n", params);
	for (GLuint i = 0; i < (GLuint)params; i++) {
		char name[64]; 
		int max_length = 64; 
		int actual_length = 0; 
		int size = 0; 
		GLenum type; 
		glGetActiveUniform(program, i, max_length, &actual_length, &size, &type, name);
		if (size > 1) {
			for (int j = 0; j < size; j++) {
				char long_name[64]; sprintf(long_name, "% s[% i]", name, j); 
				int location = glGetUniformLocation(program, long_name); 
				printf(" %i) type:% s name:% s location:% i\ n", i, GL_type_to_string(type), long_name, location);
			}
		}
		else { 
			int location = glGetUniformLocation(program, name); 
			printf(" %i) type:% s name:% s location:% i\ n", i, GL_type_to_string(type), name, location); 
		}
	} 
	gl_print_shader_info_log(program);

}

/************************************************************************************
GLFW FUNCTIONS
************************************************************************************/

start_gl(int width, int height, char* windowTitle) {
	gl_log("starting GLFW %s", glfwGetVersionString());

	glfwSetErrorCallback(Event_ErrorCallback);
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 0;
	}

	/* We must specify 3.2 core if on Apple OS X -- other O/S can specify
	anything here. I defined 'APPLE' in the makefile for OS X */
#ifdef APPLE
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

	//create window
	if (width <= 0) {
		//create full screen window
		GLFWmonitor* mon = glfwGetPrimaryMonitor();
		const GLFWvidmode* vmode = glfwGetVideoMode(mon);

		g_window = glfwCreateWindow(vmode->width, vmode->height, windowTitle, mon, NULL);
	}
	else {
		//create window of size width x height
		g_window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);
	}

	if (!g_window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	
	glfwSetWindowSizeCallback(g_window, Event_WindowSizeCallback);
	glfwMakeContextCurrent(g_window);

	//multi-sample Anti-Aliasing: more passes is cleaner (16 for screenshots)
	glfwWindowHint(GLFW_SAMPLES, 4);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	gl_log_params();

	// get version info
	const GLubyte *renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte *version = glGetString(GL_VERSION);		// version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);
	gl_log("renderer: %s\nversion: %s\n", renderer, version);

	return 1;
}


/************************************************************************************
EVENT FUNCTIONS
These are mostly program specific and are largely unimplemented
By default, esc key will close window.
************************************************************************************/

void Event_ErrorCallback(int error, const char* description) {
	fputs(description, stderr);
	gl_log_error("%s\n", description);
}

void Event_WindowSizeCallback(GLFWwindow* window, int width, int height) {
	g_gl_width = width;
	g_gl_height = height;
	printf("width %i height %i\n", width, height);
	/* update any perspective matrices used here */
}

/*For this example: use arrow keys to move, +/- to zoom*/
void Event_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {


	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_DOWN: //move down
			break;
		case GLFW_KEY_UP: //move up
			break;
		case GLFW_KEY_RIGHT: // move right
			break;
		case GLFW_KEY_LEFT: // move left
			break;
		case GLFW_KEY_KP_SUBTRACT: //zoom out
			//mand_scale *= .99;
			break;
		case GLFW_KEY_KP_ADD: //zoom in
			//mand_scale *= 1.01;
			break;
		}
	}
}

void Event_CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);

void Event_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

/************************************************************************************
VAO FUNCTIONS
************************************************************************************/

GLuint VAO_new(float* points, float* colors) {
	//copy this chunk of memory onto graphics card using VBO
	//Vertex Buffer Object - pointer to emptybuffer storing points
	//generate empty buffer, set it as current buffer  by binding, then copy the points
	GLuint points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), points, GL_STATIC_DRAW);

	//create second VBO containing colours.
	//we could put them both into a single vertex buffer and use glVertexAttribPointer to describe the different layouts
	GLuint colors_vbo;
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), colors, GL_STATIC_DRAW);

	/* create the VAO, binding each VBO in turn*/
	//most meshes use a collection of vertex buffer objects to hold vertex points, texture-coordinates, vertex normals, etc.
	//Vertex Array Object - remembers all vertex buffers and the memory layout of each one
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	//0 means define layout for attribute number 0
	//3 means that variables are vec3 made from every 3 floats

	//TODO:  delete buffers? BAD BAD really BAD don't do this 
	//glDeleteBuffers(1, points_vbo);
	//glDeleteBuffers(1, colors_vbo);

	return vao;
}

void VAO_delete(GLuint vao) {

	//no idea what to do here
}


/************************************************************************************
SHADER FUNCTIONS
************************************************************************************/

//meant for internal use only, ONLY DO DURING DEVELOPMENT
int _validate_shader(GLuint sp) {
	glValidateProgram(sp);
	GLint params = -1;
	glGetProgramiv(sp, GL_VALIDATE_STATUS, &params);
	if (GL_TRUE != params) {
		gl_log_error("program %i GL_VALIDATE_STATUS = GL_FALSE\n", sp);
		gl_log_shader(sp);
		return 0;
	}
	gl_log("program %i GL_VALIDATE_STATUS = GL_TRUE\n", sp);
	return 1;
}

//meant for internal use only
int _create_program(GLuint vert, GLuint frag, GLuint* program) {
	*program = glCreateProgram();
	gl_log("created programme %u. attaching shaders %u and %u...\n", *program,
		vert, frag);
	glAttachShader(*program, vert);
	glAttachShader(*program, frag);
	// link the shader programme. if binding input attributes do that before link
	glLinkProgram(*program);
	GLint params = -1;
	glGetProgramiv(*program, GL_LINK_STATUS, &params);

	if (GL_TRUE != params) {
		gl_log_error("ERROR: could not link shader programme GL index %u\n",
			*program);
		gl_log_shader(*program);
		return 0;
	}
	//ONLY DO DURING DEVELOPMENT
	_validate_shader(*program);

	// delete shaders here to free memory
	glDeleteShader(vert);
	glDeleteShader(frag);
	return 1;
}

//meant for internal use only
int _create_shader(const char* file_name, GLuint* shader, GLenum type) {

	gl_log("creating shader from %s...\n", file_name);

	char* shader_string = readFileToString(file_name, MAX_SHADER_LENGTH);
	*shader = glCreateShader(type);
	const GLchar *p = (const GLchar *)shader_string;
	glShaderSource(*shader, 1, &p, NULL);
	glCompileShader(*shader);

	// check for compile errors
	int params = -1;
	glGetShaderiv(*shader, GL_COMPILE_STATUS, &params);

	if (GL_TRUE != params) {
		gl_log_error("ERROR: GL shader index %i did not compile\n", *shader);
		gl_log_shader(*shader);
		return 0; // or exit or something
	}

	gl_log("shader compiled. index %i\n", *shader);
	gl_log_shader(*shader);
	return 1;
}


GLuint Shader_new(const char *vert_file_name, const char *frag_file_name) {
	GLuint vert, frag, program;

	//TODO:  handle return values
	_create_shader(vert_file_name, &vert, GL_VERTEX_SHADER);
	_create_shader(frag_file_name, &frag, GL_FRAGMENT_SHADER);
	_create_program(vert, frag, &program);

	return program;
}





void Shader_delete(GLuint sp) {
	//no idea what to do here
}

/************************************************************************************
RENDER FUNCTIONS
************************************************************************************/

void Render_clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Render_Color(float r, float g, float b) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

void Render(GLuint vao, GLuint shader, GLuint renderMode, int size) {
	//attach shader program
	glUseProgram(shader);

	//bind vao for rendering
	glBindVertexArray(vao);

	//draw points 0-size from currently bound vao with current in-use shader
	glDrawArrays(renderMode, 0, size);

}

