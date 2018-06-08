#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <tuple>
#include "obj3d.h"

using namespace glm;
using namespace std;

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
                            ));
        }
        else if (tokens[0]=="vn")
        {
            in.push_back(vec3(stod(tokens[1]),
                              stod(tokens[2]),
                              stod(tokens[3])
                            ));
        }
        else if (tokens[0]=="vt")
        {
            it.push_back(vec2(stod(tokens[1]),
                              stod(tokens[2])
                            ));
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
