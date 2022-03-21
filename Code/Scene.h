
#include "Cube.h"

// The GLM library contains vector and matrix functions and classes
#include <GLM/glm.hpp> // This is the main GLM header
#include <GLM/gtc/matrix_transform.hpp> // This one lets us use matrix transformations
#include <GLM/gtc/type_ptr.hpp> // This one gives us access to a utility function which makes sending data to OpenGL nice and easy



class Scene
{
public:

	Scene();
	~Scene();

	void Update( float deltaTs );
	void Draw();


protected:

	Cube _cubeModel;

	glm::mat4 _modelMatrixCube1;
	glm::mat4 _modelMatrixCube2;
	glm::mat4 _modelMatrixCube3;
	glm::mat4 _modelMatrixCube4;
		
	glm::mat4 _viewMatrix;
	
	// Set up a projection matrix
	glm::mat4 _projMatrix;

	GLuint _shaderProgram;

	int _shaderModelMatLocation;
	int _shaderViewMatLocation;
	int _shaderProjMatLocation;

	int _shaderWSLightPosLocation;

	float _shaderMetallicLocation;
	float _shaderRoughnessLocation;

	bool CheckShaderCompiled( GLint shader );
	void BuildShaders();
};
