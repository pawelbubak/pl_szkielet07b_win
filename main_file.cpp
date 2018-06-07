/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE
#define PIONEK 1
#define SKOCZEK 2
#define GONIEC 3
#define WIEZA 4
#define HETMAN 5
#define KROL 6
#define PUSTE 0
//JAKIS KOMENTARZ
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
};

vector<string> split(string str,char sep=' ')
{
    vector<string> result;
    string sub;

    istringstream iss(str);

    while(getline(iss,sub,sep))
    {
        result.push_back(sub);
    }

    return result;
}

ivec3 facevert(string str)
{
    vector<string> tokens=split(str,'/');

    return ivec3(stoi(tokens[0]),stoi(tokens[1]),stoi(tokens[2]));
}

void Obj3d::loadFromOBJ(string filename)
{
    ifstream myfile;
    myfile.open (filename.c_str(), ios::in);

    vector<vec4> iv;
    vector<vec3> in;
    vector<vec2> it;
    vector<tuple<ivec3,ivec3,ivec3> > ifa;

    string line;

    while ( getline (myfile,line) )
    {
        vector<string> tokens=split(line);
        if (tokens[0]=="v")
        {
            // printf("%s \n %s \n %s\n",tokens[1].c_str(),tokens[2].c_str(),tokens[3].c_str());
            iv.push_back(vec4(stod(tokens[1]),
                              stod(tokens[2]),
                              stod(tokens[3]),
                              1
                             )
                        );
        }
        else if (tokens[0]=="vn")
        {
            in.push_back(vec3(stod(tokens[1]),
                              stod(tokens[2]),
                              stod(tokens[3])
                             )
                        );
        }
        else if (tokens[0]=="vt")
        {
            it.push_back(vec2(stod(tokens[1]),
                              stod(tokens[2])
                             )
                        );
        }
        else if (tokens[0]=="f")
        {
            ifa.push_back(make_tuple(
                              facevert(tokens[1]),
                              facevert(tokens[2]),
                              facevert(tokens[3])
                          ));
        }
    }





    myfile.close();


    vec4 center=vec4(0,0,0,0);

    for(int i=0; i<iv.size(); i++)
        center=center+iv[i];

    center=center/(float)iv.size();

    //printf("%f %f %f %f \n",center.x,center.y,center.z,center.w);

    center.w=0;

    for (int i=0; i<iv.size(); i++)
        iv[i]=iv[i]-center;


    vertices.clear();
    texCoords.clear();
    vertexNormals.clear();
    c1.clear();
    c2.clear();
    int licznik;
    licznik=0;

    vec3 suma;
    suma= vec3(0,0,0);
    for (int i=0; i<ifa.size(); i++)
    {

        licznik=0;
        suma= vec3(0,0,0);

        for (int o=0; o<ifa.size(); o++)
        {
            int j=o;//(i-80+o)%ifa.size();
            if (iv[get<0>(ifa[i])[0]-1]==iv[get<0>(ifa[j])[0]-1])
            {
                suma= suma + in[get<0>(ifa[j])[2]-1];
                licznik++;
            }
            if (iv[get<0>(ifa[i])[0]-1]==iv[get<1>(ifa[j])[0]-1])
            {
                suma= suma + in[get<1>(ifa[j])[2]-1];
                licznik++;
            }
            if (iv[get<0>(ifa[i])[0]-1]==iv[get<2>(ifa[j])[0]-1])
            {
                suma= suma + in[get<2>(ifa[j])[2]-1];
                licznik++;
            }
            if (licznik==6)
            {
                break;
            }
        }

        suma=normalize(suma);
        vertexNormals.push_back(vec4(suma,0));

        licznik=0;
        suma= vec3(0,0,0);
        for (int o=0; o<ifa.size(); o++)
        {
            int j=o;//(i-80+o)%ifa.size();
            if (iv[get<1>(ifa[i])[0]-1]==iv[get<0>(ifa[j])[0]-1])
            {
                suma=suma + in[get<0>(ifa[j])[2]-1];
                licznik++;
            }
            if (iv[get<1>(ifa[i])[0]-1]==iv[get<1>(ifa[j])[0]-1])
            {
                suma= suma + in[get<1>(ifa[j])[2]-1];
                licznik++;
            }
            if (iv[get<1>(ifa[i])[0]-1]==iv[get<2>(ifa[j])[0]-1])
            {
                suma= suma +in[get<2>(ifa[j])[2]-1];
                licznik++;
            }
            if (licznik==6)
            {
                break;
            }
        }
        suma=normalize(suma);
        vertexNormals.push_back(vec4(suma,0));


        licznik=0;
        suma= vec3(0,0,0);
        for (int o=0; o<ifa.size(); o++)
        {
            int j=o;//(i-80+o)%ifa.size();
            if (iv[get<2>(ifa[i])[0]-1]==iv[get<0>(ifa[j])[0]-1])
            {
                suma= suma + in[get<0>(ifa[j])[2]-1];
                licznik++;
            }
            if (iv[get<2>(ifa[i])[0]-1]==iv[get<1>(ifa[j])[0]-1])
            {
                suma= suma + in[get<1>(ifa[j])[2]-1];
                licznik++;
            }
            if (iv[get<2>(ifa[i])[0]-1]==iv[get<2>(ifa[j])[0]-1])
            {
                suma=suma + in[get<2>(ifa[j])[2]-1];
                licznik++;
            }
            if (licznik==6)
            {
                break;
            }
        }
koniec:
        cout << licznik;

        suma=normalize(suma);
        vertexNormals.push_back(vec4(suma,0));


        vertices.push_back(iv[get<0>(ifa[i])[0]-1]);
        vertices.push_back(iv[get<1>(ifa[i])[0]-1]);
        vertices.push_back(iv[get<2>(ifa[i])[0]-1]);

        texCoords.push_back(it[get<0>(ifa[i])[1]-1]);
        texCoords.push_back(it[get<1>(ifa[i])[1]-1]);
        texCoords.push_back(it[get<2>(ifa[i])[1]-1]);

        // vertexNormals.push_back(vec4(in[get<0>(ifa[i])[2]-1],0));
        // vertexNormals.push_back(vec4(in[get<1>(ifa[i])[2]-1],0));
        // vertexNormals.push_back(vec4(in[get<2>(ifa[i])[2]-1],0));
    }

    for ( int i=0; i<vertices.size(); i+=3)
    {

        // Shortcuts for vertices
        glm::vec4 & v0 = vertices[i+0];
        glm::vec4 & v1 = vertices[i+1];
        glm::vec4 & v2 = vertices[i+2];

        // Shortcuts for UVs
        glm::vec2 & uv0 = texCoords[i+0];
        glm::vec2 & uv1 = texCoords[i+1];
        glm::vec2 & uv2 = texCoords[i+2];

        // Edges of the triangle : position delta
        glm::vec4 deltaPos1 = v1-v0;
        glm::vec4 deltaPos2 = v2-v0;

        // UV delta
        glm::vec2 deltaUV1 = uv1-uv0;
        glm::vec2 deltaUV2 = uv2-uv0;


        float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
        glm::vec4 tangent = (deltaPos1 * deltaUV2.y   - deltaPos2 * deltaUV1.y)*r;
        glm::vec4 bitangent = (deltaPos2 * deltaUV1.x   - deltaPos1 * deltaUV2.x)*r;


        // Set the same tangent for all three vertices of the triangle.
        // They will be merged later, in vboindexer.cpp
        c1.push_back(tangent);
        c1.push_back(tangent);
        c1.push_back(tangent);

        // Same thing for bitangents
        c2.push_back(bitangent);
        c2.push_back(bitangent);
        c2.push_back(bitangent);
    }

    vector <int> vert;
    for ( int i=0; i<vertices.size(); i+=1)
    {
        vec4 tangent=vec4(0,0,0,0);
        vec4 bitangent=vec4(0,0,0,0);

        vert.clear();
        for (int j=0; j<vertices.size(); j+=1)
        {
            if (vertices[i]==vertices[j])
            {
                tangent+=c1[j];
                bitangent+=c2[j];
                vert.push_back(j);
            }
        }
        for (int j=0; j<vert.size(); j++)
        {
            c1[vert[j]]=normalize(tangent);
            c2[vert[j]]=normalize(bitangent);
            //  cout<< j;
        }
    }




}


//########################################################################################################################
void wczytajGre();
float speed_x = 0; // [radiany/s]
float speed_y = 0; // [radiany/s]

float aspect=1; //Stosunek szerokości do wysokości okna


int gra[8][8];
Obj3d pionek;
Obj3d skoczek;
Obj3d goniec;
Obj3d wieza;
Obj3d hetman;
Obj3d krol;
Obj3d szachownica;
GLuint  diffTexDrewno;
GLuint normalTexDrewno;
GLuint  heightTexDrewno;

GLuint  diffTexBricks;
GLuint normalTexBricks;
GLuint  heightTexBricks;

//Uchwyty na VAO i bufory wierzchołków

GLuint bufVertices; //Uchwyt na bufor VBO przechowujący tablicę współrzędnych wierzchołków
GLuint bufNormals; //Uchwyt na bufor VBO przechowujący tablicę wektorów normalnych
GLuint bufTexCoords; //Uchwyt na bufor VBO przechowujący tablicę współrzędnych teksturowania
GLuint bufC1; //Uchwyt na bufor VBO przechowujący pierwszą kolumnę moacierzy TBN^-1
GLuint bufC2; //Uchwyt na bufor VBO przechowujący drugą kolumnę moacierzy TBN^-1
GLuint bufC3; //Uchwyt na bufor VBO przechowujący trzecią kolumnę moacierzy TBN^-1


//Uchwyty na shadery
ShaderProgram *shaderProgramPionek; //Wskaźnik na obiekt reprezentujący program cieniujący.
ShaderProgram *shaderProgramSzachownica;


//Procedura obsługi błędów
void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

//Procedura obsługi klawiatury
void key_callback(GLFWwindow* window, int key,
                  int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_LEFT)
            speed_y = -3.14;
        if (key == GLFW_KEY_RIGHT)
            speed_y = 3.14;
        if (key == GLFW_KEY_UP)
            speed_x = -3.14;
        if (key == GLFW_KEY_DOWN)
            speed_x = 3.14;
    }


    if (action == GLFW_RELEASE)
    {
        if (key == GLFW_KEY_LEFT)
            speed_y = 0;
        if (key == GLFW_KEY_RIGHT)
            speed_y = 0;
        if (key == GLFW_KEY_UP)
            speed_x = 0;
        if (key == GLFW_KEY_DOWN)
            speed_x = 0;
    }
}

//Procedura obługi zmiany rozmiaru bufora ramki
void windowResize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height); //Obraz ma być generowany w oknie o tej rozdzielczości
    if (height!=0)
    {
        aspect=(float)width/(float)height; //Stosunek szerokości do wysokości okna
    }
    else
    {
        aspect=1;
    }
}


GLuint readTexture(char* filename)
{
    GLuint tex;
    glActiveTexture(GL_TEXTURE0);

    //Wczytanie do pamięci komputera
    std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
    unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
    //Wczytaj obrazek
    unsigned error = lodepng::decode(image, width, height, filename);

    //Import do pamięci karty graficznej
    glGenTextures(1,&tex); //Zainicjuj jeden uchwyt
    glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
    //Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) image.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    return tex;
}


//Tworzy bufor VBO z tablicy
GLuint makeBuffer(void *data, int vertexCount, int vertexSize)
{
    GLuint handle;

    glGenBuffers(1,&handle);//Wygeneruj uchwyt na Vertex Buffer Object (VBO), który będzie zawierał tablicę danych
    glBindBuffer(GL_ARRAY_BUFFER,handle);  //Uaktywnij wygenerowany uchwyt VBO
    glBufferData(GL_ARRAY_BUFFER, vertexCount*vertexSize, data, GL_STATIC_DRAW);//Wgraj tablicę do VBO

    return handle;
}

//Przypisuje bufor VBO do atrybutu
void assignVBOtoAttribute(ShaderProgram *shaderProgram,const char* attributeName, GLuint bufVBO, int vertexSize)
{
    GLuint location=shaderProgram->getAttribLocation(attributeName); //Pobierz numer slotu dla atrybutu
    glBindBuffer(GL_ARRAY_BUFFER,bufVBO);  //Uaktywnij uchwyt VBO
    glEnableVertexAttribArray(location); //Włącz używanie atrybutu o numerze slotu zapisanym w zmiennej location
    glVertexAttribPointer(location,vertexSize,GL_FLOAT, GL_FALSE, 0, NULL); //Dane do slotu location mają być brane z aktywnego VBO
}

//Przygotowanie do rysowania pojedynczego obiektu
void prepareObject(ShaderProgram *shaderProgram, Obj3d * model)
{
    vec4 * vertices= model->vertices.data();//Models::CubeInternal::vertices;
    vec4 * normals=model->vertexNormals.data();//Models::CubeInternal::normals;
    vec2 * texCoords=model->texCoords.data();//Models::CubeInternal::texCoords;
    vec4 * c1=model->c1.data();//Models::CubeInternal::c1;
    vec4 * c2=model->c2.data();//Models::CubeInternal::c2;
    vec4 * c3=model->c3.data();//Models::CubeInternal::c3;
    int  vertexCount=model->vertices.size();//Models::CubeInternal::vertexCount;

    //Zbuduj VBO z danymi obiektu do narysowania
    bufVertices=makeBuffer(vertices, vertexCount, sizeof(vertices[0])); //VBO ze współrzędnymi wierzchołków
    bufNormals=makeBuffer(normals, vertexCount, sizeof(vertices[0]));//VBO z wektorami normalnymi wierzchołków
    bufTexCoords=makeBuffer(texCoords, vertexCount, sizeof(texCoords[0]));//VBO ze współrzędnymi teksturowania
    bufC1=makeBuffer(c1, vertexCount, sizeof(vertices[0]));//VBO z pierwszą kolumną macierzy TBN dla każdego wierzchoła
    bufC2=makeBuffer(c2, vertexCount, sizeof(vertices[0]));//VBO z drugą kolumną macierzy TBN dla każdego wierzchoła
    bufC3=makeBuffer(c3, vertexCount, sizeof(vertices[0]));//VBO z trzecią kolumną macierzy TBN dla każdego wierzchoła

    //Zbuduj VAO wiążący atrybuty z konkretnymi VBO
    glGenVertexArrays(1,&(model->vao)); //Wygeneruj uchwyt na VAO i zapisz go do zmiennej globalnej

    glBindVertexArray(model->vao); //Uaktywnij nowo utworzony VAO

    assignVBOtoAttribute(shaderProgram,"vertex",bufVertices,4); //"vertex" odnosi się do deklaracji "in vec4 vertex;" w vertex shaderze
    assignVBOtoAttribute(shaderProgram,"normal",bufNormals,4); //"normal" odnosi się do deklaracji "in vec4 normal;" w vertex shaderze
    assignVBOtoAttribute(shaderProgram,"texCoord0",bufTexCoords,2); //"texCoord0" odnosi się do deklaracji "in vec2 texCoord0;" w vertex shaderze
    assignVBOtoAttribute(shaderProgram,"c1",bufC1,4); //"c1" odnosi się do deklaracji "in vec4 c1;" w vertex shaderze
    assignVBOtoAttribute(shaderProgram,"c2",bufC2,4); //"c2" odnosi się do deklaracji "in vec4 c2;" w vertex shaderze
    assignVBOtoAttribute(shaderProgram,"c3",bufC3,4); //"c3" odnosi się do deklaracji "in vec4 c3;" w vertex shaderze

    glBindVertexArray(0); //Dezaktywuj VAO
}


//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window)
{


    //************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
    glClearColor(0, 0, 0, 1); //Czyść ekran na czarno
    glEnable(GL_DEPTH_TEST); //Włącz używanie Z-Bufora
    glfwSetKeyCallback(window, key_callback); //Zarejestruj procedurę obsługi klawiatury
    glfwSetFramebufferSizeCallback(window,windowResize); //Zarejestruj procedurę obsługi zmiany rozmiaru bufora ramki

    shaderProgramPionek=new ShaderProgram("vshaderPionek.glsl",NULL,"fshaderPionek.glsl"); //Wczytaj program cieniujący
    shaderProgramSzachownica=new ShaderProgram("vshaderPionek.glsl",NULL,"fshaderPionek.glsl");

    prepareObject(shaderProgramPionek,&pionek);
    prepareObject(shaderProgramSzachownica,&szachownica);

    diffTexDrewno=readTexture("download.png");
    normalTexDrewno=readTexture("wood_norm.png");
    heightTexDrewno=readTexture("wood_height.png");

    diffTexBricks=readTexture("szachownica_diffuse.png");
    normalTexBricks=readTexture("wood_norm.png");
    heightTexBricks=readTexture("wood_height.png");

    pionek.diffTexture=diffTexDrewno;
    pionek.normTexture=normalTexDrewno;
    pionek.heighTexture=heightTexDrewno;

    szachownica.diffTexture=diffTexBricks;
    szachownica.normTexture=normalTexBricks;
    szachownica.heighTexture=heightTexBricks;

    wczytajGre();
}

//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram()
{
    delete shaderProgramPionek; //Usunięcie programu cieniującego

    glDeleteVertexArrays(1,&szachownica.vao);
    glDeleteVertexArrays(1,&pionek.vao); //Wykasuj VAO

    //Wykasuj bufory  VBO
    glDeleteBuffers(1,&bufVertices);
    glDeleteBuffers(1,&bufNormals);
    glDeleteBuffers(1,&bufTexCoords);
    glDeleteBuffers(1,&bufC1);
    glDeleteBuffers(1,&bufC2);
    glDeleteBuffers(1,&bufC3);

    //Wykasuj tekstury
    glDeleteTextures(1,&diffTexDrewno);
    glDeleteTextures(1,&normalTexDrewno);
    glDeleteTextures(1,&heightTexDrewno);
    //Wykasuj tekstury
    glDeleteTextures(1,&diffTexBricks);
    glDeleteTextures(1,&normalTexBricks);
    glDeleteTextures(1,&heightTexBricks);

}

void drawObject(ShaderProgram *shaderProgram, mat4 mP, mat4 mV, mat4 mM, Obj3d * model)
{
    //Włączenie programu cieniującego, który ma zostać użyty do rysowania
    //W tym programie wystarczyłoby wywołać to raz, w setupShaders, ale chodzi o pokazanie,
    //że mozna zmieniać program cieniujący podczas rysowania jednej sceny
    shaderProgram->use();

    //Przekaż do shadera macierze P,V i M.
    //W linijkach poniżej, polecenie:
    //  shaderProgram->getUniformLocation("P")
    //pobiera numer przypisany zmiennej jednorodnej o podanej nazwie
    //UWAGA! "P" w powyższym poleceniu odpowiada deklaracji "uniform mat4 P;" w vertex shaderze,
    //a mP w glm::value_ptr(mP) odpowiada argumentowi  "mat4 mP;" TYM pliku.
    //Cała poniższa linijka przekazuje do zmiennej jednorodnej P w vertex shaderze dane z argumentu mP niniejszej funkcji
    //Pozostałe polecenia działają podobnie.
    //Poniższe polecenia są z grubsza odpowiednikami glLoadMatrixf ze starego opengla
    glUniformMatrix4fv(shaderProgram->getUniformLocation("P"),1, false, glm::value_ptr(mP));
    glUniformMatrix4fv(shaderProgram->getUniformLocation("V"),1, false, glm::value_ptr(mV));
    glUniformMatrix4fv(shaderProgram->getUniformLocation("M"),1, false, glm::value_ptr(mM));

    //Powiąż zmienne typu sampler2D z jednostkami teksturującymi
    glUniform1i(shaderProgram->getUniformLocation("diffuseMap"),0);
    glUniform1i(shaderProgram->getUniformLocation("normalMap"),1);
    glUniform1i(shaderProgram->getUniformLocation("heightMap"),2);


    //Przypisz tekstury do jednostek teksturujących
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,model->diffTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,model->normTexture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D,model->heighTexture);

    //Aktywuj VAO  (powiązania pomiędzy atrybutami i VBO)
    glBindVertexArray(model->vao);

    //Narysowanie obiektu
    glDrawArrays(GL_TRIANGLES,0,model->vertices.size());


    //Dezaktywuj VAO
    glBindVertexArray(0);
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float angle_x, float angle_y)
{
    //************Tutaj umieszczaj kod rysujący obraz******************l

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //Wykonaj czyszczenie bufora kolorów i głębokości

    glm::mat4 P = glm::perspective(50 * PI / 180, aspect, 1.0f, 50.0f); //Wylicz macierz rzutowania

    glm::mat4 V = glm::lookAt( //Wylicz macierz widoku
                      glm::vec3(0.0f, 0.0f, -20.0f),
                      glm::vec3(0.0f, 0.0f, 0.0f),
                      glm::vec3(0.0f, 1.0f, 0.0f));
    V= rotate(V,-PI/4,vec3(1,0,0));
    V = glm::rotate(V, angle_y, glm::vec3(0, 1, 0));
    V = glm::rotate(V, angle_x, glm::vec3(1, 0, 0));



    for (int i=0; i<8; i++)
    {
        for (int j=0; j<8; j++)
        {

            switch (abs(gra[i][j]))
            {
            case 0:
                break;
            case PIONEK:
                pionek.M = glm::mat4(1.0f);
                pionek.M=translate(pionek.M,vec3(0.8,2.9,0.8));
                pionek.M=pionek.M=translate(pionek.M,vec3((j-4)*1.6,0,(i-4)*1.6));
                drawObject(shaderProgramPionek,P,V,pionek.M,&pionek);
                break;
            case SKOCZEK:
                skoczek.M = glm::mat4(1.0f);
                skoczek.M=translate(skoczek.M,vec3(0.8,2.9,0.8));
                skoczek.M=skoczek.M=translate(skoczek.M,vec3((j-4)*1.6,0,(i-4)*1.6));
                drawObject(shaderProgramPionek,P,V,skoczek.M,&skoczek);
                break;
            case GONIEC:
                goniec.M = glm::mat4(1.0f);
                goniec.M=translate(goniec.M,vec3(0.8,2.9,0.8));
                goniec.M=goniec.M=translate(goniec.M,vec3((j-4)*1.6,0,(i-4)*1.6));
                drawObject(shaderProgramPionek,P,V,goniec.M,&goniec);
                break;
            case WIEZA:
                wieza.M = glm::mat4(1.0f);
                wieza.M=translate(wieza.M,vec3(0.8,2.9,0.8));
                wieza.M=wieza.M=translate(wieza.M,vec3((j-4)*1.6,0,(i-4)*1.6));
                drawObject(shaderProgramPionek,P,V,wieza.M,&wieza);
                break;
            case HETMAN:
                hetman.M = glm::mat4(1.0f);
                hetman.M=translate(hetman.M,vec3(0.8,2.9,0.8));
                hetman.M=hetman.M=translate(hetman.M,vec3((j-4)*1.6,0,(i-4)*1.6));
                drawObject(shaderProgramPionek,P,V,hetman.M,&hetman);
                break;
            case KROL:
                krol.M = glm::mat4(1.0f);
                krol.M=translate(krol.M,vec3(0.8,2.9,0.8));
                krol.M=krol.M=translate(krol.M,vec3((j-4)*1.6,0,(i-4)*1.6));
                drawObject(shaderProgramPionek,P,V,krol.M,&krol);
                break;
            }


        }
    }

    //Wylicz macierz modelu rysowanego obiektu

    szachownica.M= glm::mat4(1.0f);
    drawObject(shaderProgramSzachownica,P,V,szachownica.M, &szachownica);

    //Przerzuć tylny bufor na przedni
    glfwSwapBuffers(window);


}

void wczytajGre()
{
    for(int i=0; i < 8; i++)
    {
        for (int j=0; j<8; j++)
        {
            gra[i][j]=0;
            if(j==1)
                gra[i][j]=PIONEK;
            if(j==6)
                gra[i][j]=-PIONEK;
        }
    }
    gra[0][0]=WIEZA;
    gra[0][7]=-WIEZA;
    gra[7][0]=WIEZA;
    gra[7][7]=-WIEZA;
    gra[1][0]=SKOCZEK;
    gra[6][0]=SKOCZEK;
    gra[1][7]=-SKOCZEK;
    gra[6][7]=-SKOCZEK;
    gra[2][0]=GONIEC;
    gra[5][0]=GONIEC;
    gra[2][7]=-GONIEC;
    gra[5][7]=-GONIEC;
    gra[3][0]=HETMAN;
    gra[3][7]=-HETMAN;
    gra[4][0]=KROL;
    gra[4][7]=-KROL;
}



int main(void)
{
    pionek.loadFromOBJ("hetman.obj");
    szachownica.loadFromOBJ("szachownica.obj");

    GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

    glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

    if (!glfwInit())   //Zainicjuj bibliotekę GLFW
    {
        fprintf(stderr, "Nie można zainicjować GLFW.\n");
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

    if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
    {
        fprintf(stderr, "Nie można utworzyć okna.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
    glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

    if (glewInit() != GLEW_OK)   //Zainicjuj bibliotekę GLEW
    {
        fprintf(stderr, "Nie można zainicjować GLEW.\n");
        exit(EXIT_FAILURE);
    }

    initOpenGLProgram(window); //Operacje inicjujące

    float angle_x = 0; //Kąt obrotu obiektu
    float angle_y = 0; //Kąt obrotu obiektu

    glfwSetTime(0); //Wyzeruj licznik czasu

    //Główna pętla
    while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
    {
        angle_x += speed_x*glfwGetTime(); //Zwiększ kąt o prędkość kątową razy czas jaki upłynął od poprzedniej klatki
        angle_y += speed_y*glfwGetTime(); //Zwiększ kąt o prędkość kątową razy czas jaki upłynął od poprzedniej klatki
        glfwSetTime(0); //Wyzeruj licznik czasu
        drawScene(window,angle_x,angle_y); //Wykonaj procedurę rysującą
        glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
    }

    freeOpenGLProgram();

    glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
    glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
    exit(EXIT_SUCCESS);
}
