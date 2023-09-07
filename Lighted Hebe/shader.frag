//201921147 서동건
//Lighted Hebe 과제

#version 150 core

out vec4 out_Color;
uniform vec4 color;
uniform vec3 lightPosition;   // lightPosition 받을 수 있도록 하고
uniform vec3 lightColor;      // lightColor 받을 수 있도록 하고
uniform float shininess;      // shininess 받을 수 있도록 하고
uniform vec3 ambientLight;    // ambientLight 받을 수 있도록 하고
uniform vec3 cameraPosition;  // cameraPosition 받을 수 있도록 하고
in vec3 normal;               //h. fragment shader에서는 vertex shader로 부터 얻은 normal 얻음
in vec3 worldPosition;        //h. fragment shader에서는 vertex shader로 부터 얻은 worldPosition 얻음
void main(void)
{
   vec3 l = lightPosition - worldPosition;                             //i. 역시 lightPosition - worldPosition을 이용하여 L vector를 계산
   vec3 L = normalize(l);                                              //길이 1로 만들기위해 노말라이즈
   vec3 N = normalize(normal);                                         //j. vertex shader로 부터 얻은 normal vector를 이용하여 N vector를 계산
   vec3 V = normalize(cameraPosition - worldPosition);                 //h. cameraPosition - worldPosition 으로 V vector를 계산
   vec3 R = 2 * dot(L,N)*N - L;                                        //k. L, N vector를 이용하여 R vector를 계산
   vec3 I = lightColor/dot(l,l);                                       //l. 들어오는 빛의 밝기
   vec3 c = color.rgb * max(0,dot(L,N))*I + color.rgb * ambientLight ; //l. 색 결정을 위해 L과N DOT 프로덕트한 값에 0~1 사이 만들기 위해 max를 씌워 color.rgb 곱해주고 I 곱한 다음 ambientLight 더해줌
   c += pow(max(0,dot(R,V)),shininess)*I;                              //l. I 곱해주고 spaqular light는 1로 흰색이니 생략
   out_Color = vec4(pow(c,vec3(1/2.2)),1);                             //l. 알파 붙여서 rgb각각 2.2분의 1승 해서 vec4로 색 보냄
                                                                       //l. 즉 R, L, N, V vector와 ambient light color, shineness, 물체의 color를 이용하여 phong shading 결과를 계산
}