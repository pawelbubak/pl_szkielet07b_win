#ifndef OBJ3D_H_INCLUDED
#define OBJ3D_H_INCLUDED
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
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

class Obj3d
{
public:
    vector<vec4> vertices;
    vector<vec4> normals;
    vector<vec4> vertexNormals;
    vector<vec2> texCoords;
    vector<vec4> c1;
    vector<vec4> c2;
    vector<vec4> c3;
    GLuint vao;
    GLuint diffTexture;
    GLuint normTexture;
    GLuint heighTexture;
    glm::mat4 M;
    void loadFromOBJ(string filename);
    void loadTexture(Texture *texture);
};

#endif // OBJ3D_H_INCLUDED
