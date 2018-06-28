#ifndef OBJ3D_H_INCLUDED
#define OBJ3D_H_INCLUDED
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <tuple>

using namespace glm;
using namespace std;

class Texture
{
public:
    GLuint diffTexture;
    GLuint normTexture;
    GLuint heightTexture;
    Texture();
    Texture(char* diff, char* heigh, char* norm);
    GLuint readTexture(char* filename);
};

#endif // OBJ3D_H_INCLUDED

