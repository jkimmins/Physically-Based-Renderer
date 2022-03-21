
// This is the main SDL include file
#include <SDL/SDL.h>
#include "glew.h"

#include "Scene.h"

// iostream is so we can output error messages to console
#include <iostream>
#include <string>

// An initialisation function, mainly for GLEW
// This will also print to console the version of OpenGL we are using
bool InitGL()
{
	// GLEW has a problem with loading core OpenGL
	// The temporary workaround is to enable its 'experimental' features
	glewExperimental = GL_TRUE;

	GLenum err = glewInit();
	if( GLEW_OK != err )
	{
		// Problem: glewInit failed, something is seriously wrong.
		std::cerr<<"Error: GLEW failed to initialise with message: "<< glewGetErrorString(err) <<std::endl;
		return false;
	}
	std::cout<<"INFO: Using GLEW "<< glewGetString(GLEW_VERSION)<<std::endl;

	std::cout<<"INFO: OpenGL Vendor: "<< glGetString( GL_VENDOR ) << std::endl;
	std::cout<<"INFO: OpenGL Renderer: "<< glGetString( GL_RENDERER ) << std::endl;
	std::cout<<"INFO: OpenGL Version: "<< glGetString( GL_VERSION ) << std::endl;
	std::cout<<"INFO: OpenGL Shading Language Version: "<< glGetString( GL_SHADING_LANGUAGE_VERSION ) << std::endl;
	
	return true;
}

int main(int argc, char *argv[])
{
	// Initialisation phase
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		// Something went very wrong in initialisation, all we can do is exit
		std::cout<<"Whoops! Something went very wrong, cannot initialise SDL :("<<std::endl;
		return -1;
	}

	// Choose OpenGL version
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
	// Minor version number (3):
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
	// Use the latest version and can't use deprecated functions
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

	// Create a window
	const float pointOne = 0.1;

	int winPosX = 100;
	int winPosY = 100;
	int winWidth = 640;
	int winHeight = 640;
	SDL_Window *window = SDL_CreateWindow("PBR",
		winPosX, winPosY,
		winWidth, winHeight,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	// Tell SDL that we want the window to be shown and that it can be resized

	// Choose the window the renderer should be used for
	SDL_Renderer * renderer = SDL_CreateRenderer( window, -1, 0 );

	// Create an OpenGL context
	SDL_GLContext glcontext = SDL_GL_CreateContext( window );

	// Call our initialisation function to set up GLEW and print out some GL info to console
	if( !InitGL() )
	{
		return -1;
	}

	// Work out how much time passes from frame to frame
	unsigned int lastTime = SDL_GetTicks();

	// Enable the depth test to make sure triangles in front are always in front no matter the order they are drawn
	glEnable(GL_DEPTH_TEST);

	Scene myScene;

	// This loop will keep going round until the program is exit
	bool go = true;
	while( go )
	{

		// Check for any input events
		SDL_Event incomingEvent;
		// SDL_PollEvent will check if there is an event in the queue
		while( SDL_PollEvent( &incomingEvent ) )
		{
			// Decide what to do with event
			switch( incomingEvent.type )
			{
			case SDL_QUIT:
				// To quit we need to set our 'go' bool to false so that we can escape out of the game loop
				go = false;
				break;
			}
		}

		// Update world
		unsigned int current = SDL_GetTicks();
		// Work out the change in time between the previous frame and the current one
		float deltaTs = (float) (current - lastTime) / 1000.0f;
		// Use the current time as the next frame's previous time
		lastTime = current;
		
		myScene.Update( deltaTs );

		// Draw world

		// Specify the colour to clear the framebuffer to
		glClearColor(0.0f,0.0f,0.0f,0.0f);
		// This writes the above colour to the colour part of the framebuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		myScene.Draw();

		// This tells the renderer to actually show its contents to the screen
		SDL_GL_SwapWindow( window );

		// Limiter in case we're running really quick
		if( deltaTs < (1.0f/50.0f) )
		{
			SDL_Delay((unsigned int) (((1.0f/50.0f) - deltaTs)*1000.0f) );
		}
	}

	// Cleanup
	SDL_GL_DeleteContext( glcontext );
	SDL_DestroyWindow( window );
	SDL_Quit();

	return 0;
}