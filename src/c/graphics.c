#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#endif
/*
#include <GL/gl.h>
#include <GL/glu.h>
*/
#include <stdio.h>
#include <stdlib.h>

#include "../h/graphics.h"
#include "../h/util.h"
#include "../h/system.h"

#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include "../../../glm/glm.hpp"
#include "../../../glm/gtc/matrix_transform.hpp"
#include "../../../glm/gtx/transform.hpp"

#include "../../../glm/gtc/quaternion.hpp"
#include "../../../glm/gtx/quaternion.hpp"
#include "../../../glm/gtx/euler_angles.hpp"

static SDL_Window *window = 0;
static SDL_GLContext context;
static SDL_DisplayMode mode;
static color clear_color={0,0,0,255};

u32 color_to_hex(color c)/*@bug assumes 32bit pixel*/
{
	u8 bpc=8;/*@todo actually handle variable size pixel*/
	if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
	{
		return ((c.r & 0xff) << bpc*3) + ((c.g & 0xff) << bpc*2) + ((c.b & 0xff) << bpc*1) + (c.a & 0xff << bpc*0);
	}
	else
	{
		return ((c.a & 0xff) << bpc*3) + ((c.b & 0xff) << bpc*2) + ((c.g & 0xff) << bpc*1) + (c.r & 0xff << bpc*0);
	}
}

/*
If width or height is 0 sets width and height from screen size (displaymode).
Returns the screen width and height.
*/
vec2 init_graphics()
{
	vec2 retval;
	char const *title=""; 
	int width=0;
	int height=0;
	int flags=SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL;

    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
	{
		printf( "Critical Error\n SDL_Init failed. Application will now exit.\n" );
		exit(-1);
	}
	if( SDL_GetCurrentDisplayMode(0, &mode) < 0 )
	{
		printf( "SDL_GetCurrentDisplayMode failed." );
	}
	/*linear texture filtering*/
	if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
	{
		printf( "SDL_SetHint failed." );
	}

	if( width==0 || height==0 )
	{
		width=mode.w;
		height=mode.h;
	}
	if( !( window = SDL_CreateWindow( title,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		width, height, SDL_WINDOW_SHOWN | flags ) ) )/*show the window by default*/
	{
		printf( "SDL_CreateWindow failed. Application will now exit." );
		exit(-1);
	}
	/*
	if( !( renderer = SDL_CreateRenderer( window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC ) ) )
	{
		printf( "SDL_CreateRenderer failed." );
	}

	if( SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 ) < 0 )
	{
		printf( "SDL_SetRenderDrawColor failed. Trying software fallback." );
		renderer=SDL_CreateRenderer( window, -1, SDL_RENDERER_SOFTWARE );
	}
	if( !IMG_Init( IMG_INIT_PNG ) )
	{
		printf( "IMG_Init failed." );
	}

	if( TTF_Init() ) {
		printf( "TTF_Init failed." );
	}

	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1"); 
	SDL_SetHintWithPriority(SDL_HINT_RENDER_VSYNC, "1", SDL_HINT_OVERRIDE); 
	if(SDL_GL_SetSwapInterval(-1)==-1)
	{
		SDL_GL_SetSwapInterval(1);
	}
	
	SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
	*/
	context = SDL_GL_CreateContext(window);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetSwapInterval(1);
	
	retval.x=width;
	retval.y=height;

	#ifndef __APPLE__
	glewExperimental = GL_TRUE;
	glewInit();
	#endif

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	/*no longer spawning a polling thread here - must be polled on the main thread on all platforms*/
	return retval;
}

/*
If using directx it will just be up to the use to the user to make sure his shaders are written in HLSL etc.
The function name is the same.
*/
typedef struct vertex_shader
{
	u32 vertex_shader_id;
} vertex_shader;
typedef struct fragment_shader
{
	u32 fragment_shader_id;
} fragment_shader;

vertex_shader *ctor_compile_vertex_shader(const char *source)
{
	vertex_shader *v=(vertex_shader*)malloc(sizeof(vertex_shader));
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShaderID, 1, &source , NULL);
	glCompileShader(VertexShaderID);
	v->vertex_shader_id=VertexShaderID;
#ifdef DEBUG
	{
		GLint is_compiled=0;
		glGetShaderiv(v->vertex_shader_id, GL_COMPILE_STATUS, &is_compiled);
		if(is_compiled == GL_FALSE)
		{
			GLint max_length = 0;
			glGetShaderiv(v->vertex_shader_id, GL_INFO_LOG_LENGTH, &max_length);

			GLchar *error_log=(GLchar*)malloc(max_length);
			glGetShaderInfoLog(v->vertex_shader_id, max_length, &max_length, error_log);
			printf("Error compiling vertex shader: %s\n",error_log);
			free(error_log);

			glDeleteShader(v->vertex_shader_id); 
			return NULL;
		}	
	}
#endif
	return v;
}
void dtor_vertex_shader(vertex_shader *v)
{
	glDeleteShader(v->vertex_shader_id);
	free(v);
}

fragment_shader *ctor_compile_fragment_shader(const char *source)
{
	fragment_shader *f=(fragment_shader*)malloc(sizeof(fragment_shader));
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShaderID, 1, &source , NULL);
	glCompileShader(FragmentShaderID);
	f->fragment_shader_id=FragmentShaderID;
#ifdef DEBUG
	{
		GLint is_compiled=false;
		glGetShaderiv(f->fragment_shader_id, GL_COMPILE_STATUS, &is_compiled);
		if(is_compiled == GL_FALSE)
		{
			GLint max_length = 0;
			glGetShaderiv(f->fragment_shader_id, GL_INFO_LOG_LENGTH, &max_length);

			GLchar *error_log=(GLchar *)malloc(max_length);
			glGetShaderInfoLog(f->fragment_shader_id, max_length, &max_length, error_log);
			printf(error_log);
			free(error_log);

			glDeleteShader(f->fragment_shader_id); 
			return NULL;
		}	
	}
#endif
	return f;
}
void dtor_fragment_shader(fragment_shader *f)
{
	glDeleteShader(f->fragment_shader_id);
	free(f);
}

typedef struct shader_program
{
	u32 program_id;
} shader_program;

shader_program *ctor_shader_program(vertex_shader *v, fragment_shader *f)
{
	u32 program_id=glCreateProgram();
	shader_program *s=(shader_program*)malloc(sizeof(shader_program));

	glAttachShader(program_id, v->vertex_shader_id);
	glAttachShader(program_id, f->fragment_shader_id);
	glLinkProgram(program_id);

	s->program_id=program_id;	
	return s;
}
void dtor_shader_program(shader_program *s)
{
	glDeleteProgram(s->program_id);
	free(s);
}

typedef struct texture 
{
	u32 id;
} texture;

texture *ctor_texture(char *path)
{
	texture *t=(texture*)malloc(sizeof(texture));
	
	SDL_Surface *surf=IMG_Load(path);
	
	
	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);
	t->id=textureID;
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, surf->w, surf->h, 0, GL_BGR, GL_UNSIGNED_BYTE, surf->pixels);//this copies last parameter right? cuz im freeing it

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	SDL_FreeSurface(surf);
	return t;	
}
void dtor_texture(texture *t)
{
	//@todo, free opengl shit and free wrapper
}

static glm::mat4 View = glm::lookAt(
	glm::vec3(3,3,3), // Camera is at (3,3,3), in World Space
	glm::vec3(3,3,0), // and looks at the origin
	glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
);
void render_model(float *vertices, u32 vertices_sz, float *uv, u32 uv_sz, shader_program *s, texture* t, vec3 translation, vec4 rotation, vec3 scale)
{
	
	// Get a handle for our "MVP" uniform
	GLuint matrix_id = glGetUniformLocation(s->program_id, "MVP");
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	//@todo change 16/9 to whatever the window size is, also why does this projection have to be done for each model? none of this changes?
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) 16 / (float) 9, 0.1f, 100.0f);
	
	// Or, for an ortho camera :
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
	
	// Camera matrix
	
	// Model matrix : an identity matrix (model will be at the origin)
	//printf("%f,%f,%f\n",translation.x,translation.y,translation.z);
	glm::mat4 Model = glm::translate(glm::mat4(),glm::vec3(translation.x,translation.y,translation.z)) * glm::rotate(rotation.w, glm::vec3(rotation.x,rotation.y,rotation.z)) * glm::scale(glm::mat4(),glm::vec3(scale.x,scale.y,scale.z));
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around

	// Get a handle for our "MVP" uniform
	// Only during the initialisation
	GLuint MatrixID = glGetUniformLocation(s->program_id, "MVP");
	
	// Send our transformation to the currently bound shader, in the "MVP" uniform
	// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
	glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &mvp[0][0]);

	glUseProgram(s->program_id);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	GLuint TextureID  = glGetUniformLocation(s->program_id, "myTextureSampler");

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices_sz, vertices, GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uv_sz, uv, GL_STATIC_DRAW);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t->id);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(TextureID, 0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
	0,                 
	3,             
	GL_FLOAT,         
	GL_FALSE,         
	0,               
	(void*)0    
	);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size : U+V => 2
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	glDrawArrays(GL_TRIANGLES, 0, vertices_sz); 
	glDisableVertexAttribArray(0); 
	glDisableVertexAttribArray(1);
} 
void clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void flip()
{
	SDL_GL_SwapWindow(window);
}
