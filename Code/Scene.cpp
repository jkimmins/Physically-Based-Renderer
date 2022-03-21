
#include <iostream>
#include <fstream>
#include <string>

#include "Scene.h"

Scene::Scene()
{
	_shaderModelMatLocation = 0;
	_shaderViewMatLocation = 0;
	_shaderProjMatLocation = 0;
	_shaderWSLightPosLocation = 0;

	_shaderRoughnessLocation = 0.0f;
	_shaderMetallicLocation = 0.0f;

	BuildShaders();
	
	// Set up the viewing matrix
	// This represents the camera's orientation and position
	_viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,-3.5f) );

	// Set up a projection matrix
	_projMatrix = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);
}

Scene::~Scene()
{
}

void Scene::Update( float deltaTs )
{	
	_modelMatrixCube1 = glm::scale(glm::translate(glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(0, 1, 0)), glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.5f, 0.5f, 0.5f));
	_modelMatrixCube2 = glm::scale(glm::translate(glm::rotate(glm::mat4(1.0f),90.0f, glm::vec3(0, 1, 0)), glm::vec3(0.0f, -0.7f, 0.0f)), glm::vec3(0.5f, 0.5f, 0.5f));
	_modelMatrixCube3 = glm::scale(glm::translate(glm::rotate(glm::mat4(1.0f),90.0f, glm::vec3(0, 1, 0)), glm::vec3(-1.5f, 0.0f, 0.0f)), glm::vec3(0.5f, 0.5f, 0.5f));
	_modelMatrixCube4 = glm::scale(glm::translate(glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(0, 1, 0)), glm::vec3(-1.5f, -0.7f, 0.0f)), glm::vec3(0.5f, 0.5f, 0.5f));
}

void Scene::Draw()
{
	// Activate the shader program
	glUseProgram( _shaderProgram );

	// Send view and projection matrices to OpenGL
	glUniformMatrix4fv(_shaderViewMatLocation, 1, GL_FALSE, glm::value_ptr(_viewMatrix) );
	glUniformMatrix4fv(_shaderProjMatLocation, 1, GL_FALSE, glm::value_ptr(_projMatrix) );
	

	// Draw Cube 1
	glUniform1f(_shaderMetallicLocation, 0.1f);
	glUniform1f(_shaderRoughnessLocation, 0.1f);
	glUniformMatrix4fv(_shaderModelMatLocation, 1, GL_FALSE, glm::value_ptr(_modelMatrixCube1) );
	_cubeModel.Draw( );

	// Draw Cube 2
	glUniform1f(_shaderMetallicLocation, 0.1f);
	glUniform1f(_shaderRoughnessLocation, 0.9f);
	glUniformMatrix4fv(_shaderModelMatLocation, 1, GL_FALSE, glm::value_ptr(_modelMatrixCube2) );	
	_cubeModel.Draw( );
		
	// Draw Cube 3
	glUniform1f(_shaderMetallicLocation, 0.9f);
	glUniform1f(_shaderRoughnessLocation, 0.1f);
	glUniformMatrix4fv(_shaderModelMatLocation, 1, GL_FALSE, glm::value_ptr(_modelMatrixCube3) );
	_cubeModel.Draw( );

	// Draw Cube 4
	glUniform1f(_shaderMetallicLocation, 0.9f);
	glUniform1f(_shaderRoughnessLocation, 0.9f);
	glUniformMatrix4fv(_shaderModelMatLocation, 1, GL_FALSE, glm::value_ptr(_modelMatrixCube4));
	_cubeModel.Draw();

	glUseProgram( 0 );
}

bool Scene::CheckShaderCompiled( GLint shader )
{
	GLint compiled;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
	if ( !compiled )
	{
		GLsizei len;
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &len );

		// OpenGL will store an error message as a string that we can retrieve and print
		GLchar* log = new GLchar[len+1];
		glGetShaderInfoLog( shader, len, &len, log );
		std::cerr << "ERROR: Shader compilation failed: " << log << std::endl;
		delete [] log;

		return false;
	}
	return true;
}

void Scene::BuildShaders()
{
	std::string vertFilename("vertShader.txt");
	std::string fragFilename("fragShader.txt");

	std::ifstream vertFile( vertFilename );
	char *vShaderText = NULL;

	if( vertFile.is_open() )
	{
		// Find out how many characters are in the file
		vertFile.seekg( 0, vertFile.end );
		int length = (int)vertFile.tellg();
		vertFile.seekg( 0, vertFile.beg );

		// Create buffer
		vShaderText = new char[length];

		// Transfer data from file to buffer
		vertFile.read( vShaderText, length );

		// Check it reached the end of the file
		if( !vertFile.eof() )
		{
			vertFile.close();
			std::cerr << "WARNING: could not read vertex shader from file: " << vertFilename << std::endl;
			return;
		}

		// Find out how many characters were actually read
		length = (int)vertFile.gcount();

		vShaderText[length - 1] = 0;

		vertFile.close();
	}
	else
	{
		std::cerr << "WARNING: could not open vertex shader from file: " << vertFilename << std::endl;
		return;
	}

	std::ifstream fragFile( fragFilename );
	char *fShaderText = NULL;

	if( fragFile.is_open() )
	{
		// Find out how many characters are in the file
		fragFile.seekg( 0, fragFile.end );
		int length = (int)fragFile.tellg();
		fragFile.seekg( 0, fragFile.beg );

		// Create our buffer
		fShaderText = new char[length];

		// Transfer data from file to buffer
		fragFile.read( fShaderText, length );

		// Check it reached the end of the file
		if( !fragFile.eof() )
		{
			fragFile.close();
			std::cerr << "WARNING: could not read fragment shader from file: " << fragFilename << std::endl;
			return;
		}

		// Find out how many characters were actually read
		length = (int)fragFile.gcount();

		fShaderText[length - 1] = 0;

		fragFile.close();
	}
	else
	{
		std::cerr << "WARNING: could not open fragment shader from file: " << fragFilename << std::endl;
		return;
	}

	// The 'program' stores the shaders
	_shaderProgram = glCreateProgram();

	// Create the vertex shader
	GLuint vShader = glCreateShader( GL_VERTEX_SHADER );
	// Give GL the source for it
	glShaderSource( vShader, 1, &vShaderText, NULL );
	// Compile the shader
	glCompileShader( vShader );
	// Check it compiled and give useful output if it didn't work!
	if( !CheckShaderCompiled( vShader ) )
	{
		std::cerr<<"ERROR: failed to compile vertex shader"<<std::endl;
		return;
	}
	// This links the shader to the program
	glAttachShader( _shaderProgram, vShader );

	// Same for the fragment shader
	GLuint fShader = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fShader, 1, &fShaderText, NULL );
	glCompileShader( fShader );
	if( !CheckShaderCompiled( fShader ) )
	{
		std::cerr<<"ERROR: failed to compile fragment shader"<<std::endl;
		return;
	}
	glAttachShader( _shaderProgram, fShader );

	// This makes sure the vertex and fragment shaders connect together
	glLinkProgram( _shaderProgram );
	// Check this worked
	GLint linked;
	glGetProgramiv( _shaderProgram, GL_LINK_STATUS, &linked );
	if ( !linked )
	{
		GLsizei len;
		glGetProgramiv( _shaderProgram, GL_INFO_LOG_LENGTH, &len );

		GLchar* log = new GLchar[len+1];
		glGetProgramInfoLog( _shaderProgram, len, &len, log );
		std::cerr << "ERROR: Shader linking failed: " << log << std::endl;
		delete [] log;

		return;
	}

	// Define matrices which we will send to the shader
	// Retrieve the locations of the shader's matrix uniform variables
	glUseProgram( _shaderProgram );
	_shaderModelMatLocation = glGetUniformLocation( _shaderProgram, "modelMat" );
	_shaderViewMatLocation = glGetUniformLocation( _shaderProgram, "viewMat" );
	_shaderProjMatLocation = glGetUniformLocation( _shaderProgram, "projMat" );
	_shaderWSLightPosLocation = glGetUniformLocation( _shaderProgram, "worldSpaceLightPos" );

	_shaderRoughnessLocation = glGetUniformLocation(_shaderProgram, "roughness");
	_shaderMetallicLocation = glGetUniformLocation(_shaderProgram, "metallic");
}
