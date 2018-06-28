#version 330

layout (triangles) in;
layout (triangle_strip) out;
layout (max_vertices=3) out;


uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform float amount;

in vec4 gColor[];
out vec4 iColor;

void main(void) {
    int i;
    vec4 v0 = gl_in[0].gl_Position;
    vec4 v1 = gl_in[1].gl_Position;
    vec4 v2 = gl_in[2].gl_Position;

    vec4 a=v1-v0;
    vec4 b=v2-v0;

    vec4 n = amount*normalize(vec4(cross(b.xyz,a.xyz),0));


    for (i=0;i<gl_in.length();i++) {
        gl_Position=P*V*M*(gl_in[i].gl_Position+n);
        iColor=gColor[i];
        EmitVertex();
    }

    EndPrimitive();


    /*
    vec4 srodek = vec4(0,0,0,0);

    for(i = 0; i < gl_in.length(); i++) {
        srodek += gl_in[i].gl_Position;
    }
    srodek = srodek/3;

    for(i = 0; i < gl_in.length(); i++) {
        gl_Position=gl_in[i].gl_Position;
        iColor=gColor[i];
        EmitVertex();
        gl_Position=srodek;
        EmitVertex();
        EndPrimitive();
    }
    */
}
