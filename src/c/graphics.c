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

#include <vector>

#include "../h/graphics.h"
#include "../h/util.h"
#include "../h/system.h"

#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include <fbxsdk.h>

static SDL_Window *window = 0;
static SDL_GLContext context;
static SDL_DisplayMode mode;
static color clear_color={0,0,0,255};
void log_error()
{
	printf(SDL_GetError());
}
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
	glEnable(GL_CULL_FACE);

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


//@nm
GLuint loadDDS(const char * imagepath){
#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII
	unsigned char header[124];

	FILE *fp; 
 
	/* try to open the file */ 
	fp = fopen(imagepath, "rb"); 
	if (fp == NULL){
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar(); 
		return 0;
	}
   
	/* verify the type of file */ 
	char filecode[4]; 
	fread(filecode, 1, 4, fp); 
	if (strncmp(filecode, "DDS ", 4) != 0) { 
		fclose(fp); 
		return 0; 
	}
	
	/* get the surface desc */ 
	fread(&header, 124, 1, fp); 

	unsigned int height      = *(unsigned int*)&(header[8]);
	unsigned int width	     = *(unsigned int*)&(header[12]);
	unsigned int linearSize	 = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC      = *(unsigned int*)&(header[80]);
 
	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */ 
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize; 
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char)); 
	fread(buffer, 1, bufsize, fp); 
	/* close the file pointer */ 
	fclose(fp);

	unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4; 
	unsigned int format;
	switch(fourCC) 
	{ 
	case FOURCC_DXT1: 
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; 
		break; 
	case FOURCC_DXT3: 
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; 
		break; 
	case FOURCC_DXT5: 
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; 
		break; 
	default: 
		free(buffer); 
		return 0; 
	}

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);	
	
	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16; 
	unsigned int offset = 0;

	/* load the mipmaps */ 
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) 
	{ 
		unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize; 
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,  
			0, size, buffer + offset); 
	 
		offset += size; 
		width  /= 2; 
		height /= 2; 

		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if(width < 1) width = 1;
		if(height < 1) height = 1;

	} 

	free(buffer); 

	return textureID;
}

typedef struct texture 
{
	u32 id;
} texture;
texture *ctor_texture_dds(char *path)//@temp
{
	texture *t=(texture*)malloc(sizeof(texture));
	t->id=loadDDS(path);
	return t;	
}
texture *ctor_texture(char *path)
{
	texture *t=(texture*)malloc(sizeof(texture));
	
	SDL_Surface *surf=IMG_Load(path);
	
	
	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);
	t->id=textureID;
	// "Bind" the newly created texture : all future texture functions will modify this texture
	//@bug this should be done at render time
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
	glDeleteTextures(1,&t->id);
	free(t);
}

//@nm
bool load_obj_to_vectors(const char * path, 
	std::vector<vec3> & out_vertices, 
	std::vector<vec2> & out_uvs,
	std::vector<vec3> & out_normals)
{//@todo this is broken now that the obj just uses vectors of floats instead of vecs
	printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<vec3> temp_vertices; 
	std::vector<vec2> temp_uvs;
	std::vector<vec3> temp_normals;


	FILE * file = fopen(path, "r");
	if( file == NULL ){
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		getchar();
		return false;
	}

	while( 1 ){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		
		if ( strcmp( lineHeader, "v" ) == 0 ){
			vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);
		}else if ( strcmp( lineHeader, "vt" ) == 0 ){
			vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}else if ( strcmp( lineHeader, "vn" ) == 0 ){
			vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		}else if ( strcmp( lineHeader, "f" ) == 0 ){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9){
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				fclose(file);
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}else{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for( unsigned int i=0; i<vertexIndices.size(); i++ ){

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];
		
		// Get the attributes thanks to the index
		vec3 vertex = temp_vertices[ vertexIndex-1 ];
		vec2 uv = temp_uvs[ uvIndex-1 ];
		vec3 normal = temp_normals[ normalIndex-1 ];
		
		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);
	
	}
	fclose(file);
	return true;
}

typedef struct animation
{
	char *name;
	std::vector<FbxTakeInfo *> takes;
	
	f32 last_time;
	f32 cur_time;

	double start_time;
	double end_time;

	FbxNode *node;
	FbxScene *scene;
} animation;
FbxMatrix get_animation_transform(animation *anim,f32 time)//between start_time and end_time
{	
	FbxAnimEvaluator* mySceneEvaluator = anim->scene->GetAnimationEvaluator();
	return mySceneEvaluator->GetNodeGlobalTransform(anim->node, anim->end_time);
}
typedef struct model
{
	std::vector<f32> vertices;
	std::vector<f32> uvs;
	std::vector<f32> normals;
	u8 poly_type;//number of vertices per polygon

	FbxMatrix cur_transform;
	animation anim;//map or array or something, curanim
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
} model;
void model_update_animation(model *m)
{
	//m->anim.timestamp.
	m->cur_transform=get_animation_transform(&m->anim,0);
}
void model_set_animation(model *m, char *name)
{//@todo

}

model *ctor_model_obj(char *path)
{
	model *o=new model;
	o->poly_type=3;
	//@todo @bug @broken load_obj_to_vectors(path,o->vertices,o->uvs,o->normals);

	glGenBuffers(1, &o->vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, o->vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, o->vertices.size() *sizeof(f32), &o->vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &o->uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, o->uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, o->uvs.size() *sizeof(f32), &o->uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &o->normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, o->normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, o->normals.size() * sizeof(f32), &o->normals[0], GL_STATIC_DRAW);

	return o;
}

//@nm
model *ctor_model_fbx(char *pFilename)
{//@leak
	using namespace std;
	model *f=new model;
	f->poly_type=3;
	//load_obj_to_vectors(path,o->vertices,o->uvs,o->normals);
	{
		FbxManager *pSdkManager = FbxManager::Create();
		int lFileMajor, lFileMinor, lFileRevision;
		int lSDKMajor,  lSDKMinor,  lSDKRevision;
		int i, lAnimStackCount;
		bool lStatus;
		char lPassword[1024];

		// Get the version number of the FBX files generated by the
		// version of FBX SDK that you are using.
		FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

		// Create an importer.
		FbxImporter* lImporter = FbxImporter::Create(pSdkManager,"");
		
		// Initialize the importer by providing a filename.
		const bool lImportStatus = lImporter->Initialize(pFilename, -1, pSdkManager->GetIOSettings() );

		FbxScene *pScene = FbxScene::Create(pSdkManager, "pScene");

		// Get the version number of the FBX file format.
		lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

		if( !lImportStatus )  // Problem with the file to be imported
		{
			FbxString error = lImporter->GetStatus().GetErrorString();
			printf("Call to FbxImporter::Initialize() failed.");
			printf("Error returned: %s", error.Buffer());

			if (lImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
			{
				printf("FBX version number for this FBX SDK is %d.%d.%d",
					lSDKMajor, lSDKMinor, lSDKRevision);
				printf("FBX version number for file %s is %d.%d.%d",
					pFilename, lFileMajor, lFileMinor, lFileRevision);
			}
		}

		printf("FBX version number for this FBX SDK is %d.%d.%d",
			lSDKMajor, lSDKMinor, lSDKRevision);

		if (lImporter->IsFBX())
		{
			printf("FBX version number for file %s is %d.%d.%d",
				pFilename, lFileMajor, lFileMinor, lFileRevision);
		}

		// Import the scene.
		lStatus = lImporter->Import(pScene);

		if(lStatus == false &&     // The import file may have a password
			lImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
		{
			printf("password?");
		}

		/*
		if(0){
			auto m=pScene->GetRootNode()->GetChild(1)->GetMesh();
			auto polycount=m->GetPolygonCount();
			auto polys=m->GetPolygonVertices();
				//printf("\npolycount:%d\n",polycount);
			for(int i=0; i<polycount; i+=3)
			{
				auto factor=10000.0f;
				//printf("\ntri:%f,%f,%f\n",polys[i]/factor,polys[i+1]/factor,polys[i+2]/factor);
				f->vertices.push_back({polys[i]/factor,polys[i+1]/factor,polys[i+2]/factor});
			}

			{

			}
		}
		*/
		{
			FbxNode* pFbxRootNode = pScene->GetRootNode();

			if(pFbxRootNode)
			{
				//@todo this is adding all the children and shit to the same model it should construct all the models from the scene
				for(int i = 0; i < pFbxRootNode->GetChildCount(); i++)
				{
					FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);

					if(pFbxChildNode->GetNodeAttribute() == NULL)
						continue;

					FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();

					if(AttributeType != FbxNodeAttribute::eMesh)
						continue;

					FbxMesh* pMesh = (FbxMesh*) pFbxChildNode->GetNodeAttribute();

					FbxVector4* pVertices = pMesh->GetControlPoints();

					for (int j = 0; j < pMesh->GetPolygonCount(); j++)
					{
						int iNumVertices = pMesh->GetPolygonSize(j);
						f->poly_type=iNumVertices;
						//printf("\nPOLY_TYPE: %d\n",f->poly_type);
						for (int k = 0; k < iNumVertices; k++)
						{
							int iControlPointIndex = pMesh->GetPolygonVertex(j, k);

							f->vertices.push_back((float)pVertices[iControlPointIndex].mData[0]);
							f->vertices.push_back((float)pVertices[iControlPointIndex].mData[1]);
							f->vertices.push_back((float)pVertices[iControlPointIndex].mData[2]);
						}
					}
					
					FbxArray<FbxVector4> norms;
					pMesh->GetPolygonVertexNormals(norms);
					//printf("norm sz: %d\n\n",norms.Size());
						
					for(int i=0; i<norms.Size(); i++)
					{
						f->normals.push_back((f32)norms[i][0]);
						f->normals.push_back((f32)norms[i][1]);
						f->normals.push_back((f32)norms[i][2]);
					}

					FbxLayerElementArrayTemplate<FbxVector2>* uvVertices= 0;
					pMesh->GetTextureUV(&uvVertices);
					if(uvVertices)
					{
						for (int k = 0; k < pMesh->GetPolygonCount(); k++)
						{
							for (int j = 0; j < pMesh->GetPolygonSize(i); ++j) {
								FbxVector2 uv = (*uvVertices)[pMesh->GetTextureUVIndex(k, j)];

								f->uvs.push_back((f32)uv[0]);
								f->uvs.push_back((f32)uv[1]);
							}
						}
					}

					
					printf("\nanim_stack_count: %d\n",lImporter->GetAnimStackCount());
					for(int lAnimStackCount=0; lAnimStackCount < lImporter->GetAnimStackCount(); lAnimStackCount++) 
					{
						auto lTakeInfo=lImporter->GetTakeInfo(lAnimStackCount);
						f->anim.takes.push_back(lTakeInfo);
						f->anim.name = lTakeInfo->mName.Buffer();

						f->anim.start_time = lTakeInfo->mLocalTimeSpan.GetStart().GetMilliSeconds();
						f->anim.end_time = lTakeInfo->mLocalTimeSpan.GetStop().GetMilliSeconds();

						f->anim.scene=pScene;
						f->anim.node=pFbxChildNode;
						//f->anim.last_time=now;

						//FbxAnimEvaluator* mySceneEvaluator = pScene->GetAnimationEvaluator();
						
						//f->anim.cur_transform = mySceneEvaluator->GetNodeGlobalTransform(pFbxChildNode, f->anim.end_time);
						/*
						printf("\n\nanim_start_time: %f\n\n",f->anim.start_time);
						for(int x=0; x<4; x++)
						{
							for(int y=0; y<4; y++)
							{
								printf("matrix element %d,%d: %f",x,y,f->anim.cur_transform.Get(x,y));
							}
						}
						*/

						//printf("\n%s\n",lTakeName.Buffer());
					}
					
				}
			}
		}
		
		// Destroy the importer
		lImporter->Destroy();
	}

	{
		glGenBuffers(1, &f->vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, f->vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, f->vertices.size() *sizeof(f32), &f->vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &f->uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, f->uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, f->uvs.size() *sizeof(f32), &f->uvs[0], GL_STATIC_DRAW);

		glGenBuffers(1, &f->normalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, f->normalbuffer);
		glBufferData(GL_ARRAY_BUFFER, f->normals.size() * sizeof(f32), &f->normals[0], GL_STATIC_DRAW);	
	}

	return f;
}
void dtor_model(model *m)
{
	//@todo @leak free opengl shit
	free(m);
}

void render_model(model *o,shader_program *s, texture* t, vec3 translation, vec4 rotation, vec3 scale)
{	

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, o->vertexbuffer);
	glVertexAttribPointer(
	0,                 
	o->poly_type,
	GL_FLOAT,         
	GL_FALSE,         
	0,               
	(void*)0    
	);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, o->uvbuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size : U+V => 2
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// 3rd attribute buffer : normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, o->normalbuffer);
	glVertexAttribPointer(
		2,                                // attribute
		3, 				                  // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);
	
	{
		static glm::mat4 View = glm::lookAt(
			glm::vec3(3,3,3), // Camera is at (3,3,3), in World Space
			glm::vec3(0,0,0), // and looks at the origin
			glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		//@todo change 16/9 to whatever the window size is, also why does this projection have to be done for each model? none of this changes?
		static glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) 16 / (float) 9, 0.1f, 100.0f);

		glm::mat4 Model = glm::translate(glm::mat4(),glm::vec3(translation.x,translation.y,translation.z)) * glm::rotate(rotation.w, glm::vec3(rotation.x,rotation.y,rotation.z)) * glm::scale(glm::mat4(),glm::vec3(scale.x,scale.y,scale.z));

		glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around
		
		GLuint model_id = glGetUniformLocation(s->program_id, "M");
		glUniformMatrix4fv(model_id, 1, GL_FALSE, &Model[0][0]);
		
		GLuint view_id = glGetUniformLocation(s->program_id, "V");
		glUniformMatrix4fv(view_id, 1, GL_FALSE, &View[0][0]);
		
		GLuint matrix_id = glGetUniformLocation(s->program_id, "MVP");
		glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &mvp[0][0]);

		GLuint anim_transform_id = glGetUniformLocation(s->program_id, "anim_transform");
		glUniformMatrix4fv(anim_transform_id, 1, GL_FALSE, (GLfloat*)&o->cur_transform[0][0]);
		
		glUseProgram(s->program_id);

		GLuint TextureID  = glGetUniformLocation(s->program_id, "myTextureSampler");
		GLuint LightID = glGetUniformLocation(s->program_id, "LightPosition_worldspace");

		glm::vec3 lightPos = glm::vec3(0,2,4);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
		
		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t->id);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);

		glDrawArrays(o->poly_type==3?GL_TRIANGLES:GL_QUADS, 0, o->vertices.size()); 
	}

	glDisableVertexAttribArray(0); 
	glDisableVertexAttribArray(1); 
	glDisableVertexAttribArray(2);
}

void clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void flip()
{
	SDL_GL_SwapWindow(window);
}
