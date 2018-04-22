#include "SDLGraphicsProgram.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"
int numIndices;

// Initialization function
// Returns a true or false value based on successful completion of setup.
// Takes in dimensions of window.
SDLGraphicsProgram::SDLGraphicsProgram(int w, int h):screenWidth(w),screenHeight(h){
  // Initialization flag
	bool success = true;
  // String to hold any errors that occur.
  std::stringstream errorStream;
  // The window we'll be rendering to
  gWindow = NULL;
  // Render flag

	// Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO)< 0){
		errorStream << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
		success = false;
	}
	else{
		//Use OpenGL 3.3 core
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    // We want to request a double buffer for smooth updating.
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		//Create window
		gWindow = SDL_CreateWindow( "Lab",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                screenWidth,
                                screenHeight,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

    // Check if Window did not create.
    if( gWindow == NULL ){
			errorStream << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}

		//Create an OpenGL Graphics Context
		gContext = SDL_GL_CreateContext( gWindow );
		if( gContext == NULL){
			errorStream << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}

    // Initialize GLAD Library
    if(!gladLoadGLLoader(SDL_GL_GetProcAddress)){
        errorStream << "Failed to iniitalize GLAD\n";
        success = false;
    }

		//Initialize OpenGL
		if(!initGL()){
			errorStream << "Unable to initialize OpenGL!\n";
			success = false;
		}

  }

  // If initialization did not work, then print out a list of errors in the constructor.
  if(!success){
    errorStream << "Failed to initialize!\n";
    std::string errors=errorStream.str();
    std::cout << errors << "\n";
  }else{
    std::cout << "No SDL, GLAD, or OpenGL, errors Detected\n\n";
  }

}

	// Proper shutdown and destroy initialized objects
	SDLGraphicsProgram::~SDLGraphicsProgram(){
	//Destroy window
	SDL_DestroyWindow( gWindow );
    // Point gWindow to NULL to ensure it points to nothing.
	gWindow = NULL;
  // Deallocate Program
  glDeleteProgram(shader);
	//Quit SDL subsystems
	SDL_Quit();
}


void SDLGraphicsProgram::GenerateBuffers(std::string file){
	int numVerts, numFaces;
	getObjDataCounts(file, numVerts, numFaces);
	// TODO: Add data to draw two triangles
	numIndices = numFaces * 3;
	GLfloat verts[numVerts * 3];

	// TODO: Add data to the indices
	GLuint indices[numFaces * 3];
	getObjData(file, verts, indices);




	for (int i = sizeof(verts)/sizeof(verts[0]) - 1; i > sizeof(verts)/sizeof(verts[0]) - 11; i--)
	{
		std::cout << verts[i] << std::endl;
	}
	// VertexArrays
	glGenVertexArrays(1, &VAOId);
	glBindVertexArray(VAOId);

	// Vertex Buffer Object (VBO)
	// Create a buffer (note we’ll see this pattern of code often in OpenGL)
	// TODO: Read this and understand what is going on
	glGenBuffers(1, &vertexPositionBuffer); // selecting the buffer is
											// done by binding in OpenGL
											// We tell OpenGL then how we want to use our selected(or binded) buffer with the arguments passed into the function.
	glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
  // Finally pass in our vertex data
	// TODO: Read this after you complete the lab--may be useful for when we
	// 			 add more than vertices to get color!
	glVertexAttribPointer(  0,   // Attribute 0, which will match layout in shader
						    					3,   // size (Number of components (2=x,y)  (3=x,y,z), etc.)
													GL_FLOAT, // Type of data
													GL_FALSE, // Is the data normalized
						    			0, // Stride - Amount of bytes between each vertex.
											// If we only have vertex data, then this is 0.
											// // That means our vertices(or whatever data) is tightly packed, one after the other.
											// // If we add in vertex color information(3 more floats), then this becomes 6, as we move 6*sizeof(float) to get to the next chunk of data.
											// If we have normals, then we need to jump 3*sizeof(GL_FLOAT) bytes to get
											// to our next vertex.
												0       // Pointer to the starting point of our data. If we are just grabbing vertices, this is 0. But if we have some other attribute, (stored in the same data structure), this may vary if the very first element is some different attribute.
			    							// If we had some data after (say normals), then we would have an offset of 3*sizeof(GL_FLOAT) for example
	);


	// Another Vertex Buffer Object (VBO)
	// This time for your index buffer.

	// TODO: Generate another buffer.

	glGenBuffers(1, &indexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

}

// Initialize OpenGL
// Setup any of our shaders here.
bool SDLGraphicsProgram::initGL(){
	//Success flag
	bool success = true;
	// TODO: be able to switch between poly and line

	// Setup geometry
	GenerateBuffers("./objects/cube.obj");

  // Setup shaders
  std::string vertexShader = LoadShader("./shaders/vert.glsl");
  std::string fragmentShader = LoadShader("./shaders/frag.glsl");

  shader = CreateShader(vertexShader,fragmentShader);

	// Use our shader
	glUseProgram(shader);
	    glEnable(GL_DEPTH_TEST);
	return success;
}


// Update OpenGL
void SDLGraphicsProgram::update()
{
	// Here we apply our camera
	// This first transformation is applied to the object
	// This is the model transform matrix.
	// That is, 'how do I move our model'
	// Here we see I have translated the model -1.0f away from its original location.
	glm::mat4 modelTransformMatrix = glm::translate(glm::mat4(),glm::vec3(0.0f,0.0f,0.0f));
	// Next we apply the 'view' matrix which creates perspective.
	// The first argument is 'field of view'
	// Then perspective
	// Then the near and far clipping plane.
	// Note I cannot see anything closer than 0.1f units from the screen.
	glm::mat4 projectionMatrix = glm::perspective(60.0f,(float)screenWidth/(float)screenHeight,0.1f,10.0f);

	// Note that we are now 'looking' inside the shader for a particular
	// variable. This means the name has to exactly match!
	GLint modelTransformMatrixUniformLocation =  glGetUniformLocation(shaderID,"modelTransformMatrix");
	GLint projectionMatrixUniformLocation = glGetUniformLocation(shaderID,"projectionMatrix");

	// Now update this information through our uniforms.
	// glUniformMatrix4v means a 4x4 matrix of floats
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE,&modelTransformMatrix[0][0]);
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}


// Render
// The render function gets called once per loop
void SDLGraphicsProgram::render(){// i is number of indices
    // Initialize clear color
    // This is the background of the screen.
    glViewport(0, 0, screenWidth, screenHeight);
    glClearColor( 0.2f, 0.2f, 0.2f, 1.f );

    //Clear color buffer and Depth Buffer
  	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		//Render data
		// TODO: 6 for square, 3 for tri
		glDrawElements(GL_TRIANGLES,
						numIndices, // The number of indicies, not triangles.
						GL_UNSIGNED_INT, // Make sure the data type matches
						nullptr);        // Offset pointer to the data. nullptr because we are currently bound
}


//Loops forever!
void SDLGraphicsProgram::loop(){
    // Main loop flag
    // If this is quit = 'true' then the program terminates.
    bool quit = false;
    // Event handler that handles various events in SDL
    // that are related to input and output
    SDL_Event e;
	GLenum currMode = GL_FILL;
	int currModel = 1; //1 for cube, 2 for bunny
    // Enable text input
    SDL_StartTextInput();
    // While application is running
    while(!quit){
      //Handle events on queue
      while(SDL_PollEvent( &e ) != 0){
        // User posts an event to quit
        // An example is hitting the "x" in the corner of the window.
        switch (e.type)
        {
        case SDL_QUIT:
            quit = true;
            break;
		case SDL_KEYDOWN:
			if (!strcmp(SDL_GetKeyName(e.key.keysym.sym), "W"))
			{
				if (currMode == GL_FILL)
				{
					glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
					currMode = GL_LINE;
				}
				else
				{
					glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
					currMode = GL_FILL;
				}
			}
			else if (!strcmp(SDL_GetKeyName(e.key.keysym.sym), "Q"))
			{
				quit = true;
			}
			else if (!strcmp(SDL_GetKeyName(e.key.keysym.sym), "1") && currModel == 2)
			{
				GenerateBuffers("./objects/cube.obj");
				currModel = 1;
			}
			else if (!strcmp(SDL_GetKeyName(e.key.keysym.sym), "2") && currModel == 1)
			{
				GenerateBuffers("./objects/bunny.obj");
				currModel = 2;
			}
			break;
        default:
            break;
        }
				// TODO: Add some input here

				}

			// Update our scene
			update();
      // Render using OpenGL
     render(); // TODO: potentially move this depending on your logic
								// for how you handle drawing a triangle or rectangle.
      //Update screen of our specified window
      SDL_GL_SwapWindow(getSDLWindow());
    }

    //Disable text input
    SDL_StopTextInput();
}


// Loads a shader and returns a string
std::string SDLGraphicsProgram::LoadShader(const std::string& fname){
		std::string result;
		// 1.) Get every line of data
		std::string line;
		std::ifstream myFile(fname.c_str());

		if(myFile.is_open()){
			while(getline(myFile,line)){
					result += line + '\n';
			}
		}
		// Close file
		myFile.close();
		return result;
}


unsigned int SDLGraphicsProgram::CreateShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource){

    // Create a new program
    unsigned int program = glCreateProgram();
    // Compile our shaders
    unsigned int myVertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    // Link our program
    // These have been compiled already.
    glAttachShader(program,myVertexShader);
    glAttachShader(program,myFragmentShader);
    // Link our programs that have been 'attached'
    glLinkProgram(program);
    glValidateProgram(program);

    // Once the shaders have been linked in, we can delete them.
    glDetachShader(program,myVertexShader);
    glDetachShader(program,myFragmentShader);

    glDeleteShader(myVertexShader);
    glDeleteShader(myFragmentShader);

    if(!CheckLinkStatus(program)){
        std::cout << "ERROR, shader did not link!\n";
    }

    return program;
}


unsigned int SDLGraphicsProgram::CompileShader(unsigned int type, const std::string& source){
  // Compile our shaders
  // id is the type of shader (Vertex, fragment, etc.)
  unsigned int id;

  if(type == GL_VERTEX_SHADER){
    id = glCreateShader(GL_VERTEX_SHADER);
  }else if(type == GL_FRAGMENT_SHADER){
    id = glCreateShader(GL_FRAGMENT_SHADER);
  }
  const char* src = source.c_str();
  // The source of our shader
  glShaderSource(id, 1, &src, nullptr);
  // Now compile our shader
  glCompileShader(id);

  // Retrieve the result of our compilation
  int result;
  // This code is returning any compilation errors that may have occurred!
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if(result == GL_FALSE){
      int length;
      glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
      char* errorMessages = new char[length]; // Could also use alloca here.
      glGetShaderInfoLog(id, length, &length, errorMessages);
      if(type == GL_VERTEX_SHADER){
        std::cout << "ERROR: GL_VERTEX_SHADER compilation failed!\n" << errorMessages << "\n";
      }else if(type == GL_FRAGMENT_SHADER){
        std::cout << "ERROR: GL_FRAGMENT_SHADER compilation failed!\n" << errorMessages << "\n";
      }
      // Reclaim our memory
      delete[] errorMessages;
      // Delete our broken shader
      glDeleteShader(id);
      return 0;
  }

  return id;
}

// Check to see if linking was successful
bool SDLGraphicsProgram::CheckLinkStatus(GLuint programID){
  // Link our shaders
  // id is the type of shader (Vertex, fragment, etc.)
  unsigned int id;

    // Retrieve the result of our compilation
    int result;
    // This code is returning any Linker errors that may have occurred!
    glGetProgramiv(id, GL_LINK_STATUS, &result);
    if(result == GL_FALSE){
      int length;
      glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
      char* errorMessages = new char[length]; // Could also use alloca here.
      glGetProgramInfoLog(id, length, &length, errorMessages);
      // Reclaim our memory
      std::cout << "ERROR in linking process " << errorMessages << "\n";
      delete[] errorMessages;
      return false;
    }
    return true;
}


// Get Pointer to Window
SDL_Window* SDLGraphicsProgram::getSDLWindow(){
  return gWindow;
}

// Helper Function to get OpenGL Version Information
void SDLGraphicsProgram::getOpenGLVersionInfo(){
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
  std::cout << "Version: " << glGetString(GL_VERSION) << "\n";
  std::cout << "Shading language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
}
