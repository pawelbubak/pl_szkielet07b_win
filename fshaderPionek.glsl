#version 330

uniform sampler2D diffuseMap; //Zmienna reprezentujaca jednostke teksturujaca
uniform sampler2D normalMap;  //Zmienna reprezentujaca jednostke teksturujaca
uniform sampler2D heightMap;  //Zmienna reprezentujaca jednostke teksturujaca
uniform samplerCube skybox;
uniform mat4 V;
uniform mat4 P;
uniform mat4 M;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec4 i_l; //wektor do swiatla(przestrzen oka)
in vec4 i_v; //wektor do obserwatora(przestrzen oka)
in vec4 i_n; //wektor normalny (przestrzen oka)
in vec4 i_l2; //wektor do swiatla 2 (przestrzen oka)
in vec2 iTexCoord0; //wspolrzedne teksturowania
in float kolor;

vec2 parallaxTexCoords(vec4 v, vec2 t, float h, float s){
    vec2 ti = -h*v.xy/s; //przyrost
    float hi = -h/s;  //przysrost
    vec2 tc = t; //aktualne
    float hc = h;
    float ht = h * texture(heightMap,tc).r;
    if(v.z <= 0) discard;
    while(hc > ht) {
        tc = tc+ti;
        if(tc.x < 0 || tc.x > 1 || tc.y < 0 || tc.y > 1) discard;
        hc = hc+hi;
        ht = h*texture(heightMap,tc).r;
    }
    return tc;
}

void main(void) {
	vec4 v=normalize(i_v);
    vec2 tc=iTexCoord0 ; //parallaxTexCoords(v, iTexCoord0,0.02,100);// ;
	vec4 n=normalize(vec4(texture(normalMap, tc).rgb * 2 - 1,0));
    vec4 l=normalize(i_l);
    vec4 r=reflect(-l,n);
    vec4 l2=normalize(i_l2);
    vec4 r2=reflect(-l2,n);


    vec4 ka=vec4(1,1,1,0); //Kolor obiektu w swietle otoczenia
    vec4 kd=texture(diffuseMap,tc); //Kolor obiektu w swietle rozproszonym
    vec4 ks=vec4(1,1,1,0); //Kolor obiektu w swietle odbitym

    vec4 la=vec4(0.3,0.3,0.3,0); //Kolor swiatla otoczenia
    vec4 ld=vec4(1,1,1,1); //Kolor swiatla rozpraszanego
    vec4 ls=vec4(1,1,1,0); //Kolor swiatla odbijanego

    float nl=max(dot(n,l),0); //Kosinus kata pomiedzy wektorami do swiatla i normalnym
    float rv=pow(max(dot(r,v),0),100); //Kosinus kata pomiedzy wektorami do obserwatora i odbitym, podniesiony do wykladnika Phonga

    float nl2=max(dot(n,l2),0); //Kosinus kata pomiedzy wektorami do swiatla i normalnym
    float rv2=pow(max(dot(r2,v),0),100); //Kosinus kata pomiedzy wektorami do obserwatora i odbitym, podniesiony do wykladnika Phonga

    vec4 kolor_otoczenia= ka*la*kd;
    vec4 kolor_odbity1=ks*ls*vec4(rv,rv,rv,0);
    vec4 kolor_rozproszony1=kd*ld*vec4(nl,nl,nl,0);

    vec4 kolor_odbity2=ks*ls*vec4(rv2,rv2,rv2,0);
    vec4 kolor_rozproszony2=kd*ld*vec4(nl2,nl2,nl2,0);
    vec4 suma_kolor=kolor_otoczenia+kolor_odbity1+kolor_odbity2+kolor_rozproszony1+kolor_rozproszony2;
    vec4 sciemnienie= vec4(0.95*kolor,0.95*kolor,0.95*kolor,0)*(kolor_rozproszony1+kolor_rozproszony2);

	pixelColor= suma_kolor-sciemnienie;
}
