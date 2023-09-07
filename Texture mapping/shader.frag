//201921147 서동건
//Texture mapping 과제

#version 150 core

out vec4 out_Color;
uniform vec4 color;
uniform vec3 lightPosition;   // lightPosition 받을 수 있도록 하고
uniform vec3 lightColor;      // lightColor 받을 수 있도록 하고
uniform float shininess;      // shininess 받을 수 있도록 하고
uniform vec3 ambientLight;    // ambientLight 받을 수 있도록 하고
uniform vec3 cameraPosition;  // cameraPosition 받을 수 있도록 하고
uniform sampler2D diffTex;    // 주변을보고 잘 가져오는 sampler2D 형식으로 diffTex 받을 수 있도록 하고
uniform sampler2D bumpTex;    // 주변을보고 잘 가져오는 sampler2D 형식으로 bumpTex 받을 수 있도록 하고
in vec3 normal;               // fragment shader에서는 vertex shader로 부터 얻은 normal 얻음
in vec3 worldPosition;        // fragment shader에서는 vertex shader로 부터 얻은 worldPosition 얻음
in vec2 texCoords;            // fragment shader에서는 vertex shader로 부터 얻은 texCoords 얻음

mat3 getTBN(vec3 N){          // T B N 계산
   vec3 Q1 = dFdx(worldPosition), Q2 = dFdy(worldPosition);
   vec2 st1 = dFdx(texCoords), st2 = dFdy(texCoords);
   float D = st1.s*st2.t-st2.s*st1.t;
   return mat3(normalize(( Q1*st2.t - Q2*st1.t )*D),
   normalize((-Q1*st2.s + Q2*st1.s )*D), N);
}

void main(void)
{
   vec3 l = lightPosition - worldPosition;                             // 역시 lightPosition - worldPosition을 이용하여 L vector를 계산
   vec3 L = normalize(l);                                              // 길이 1로 만들기위해 노말라이즈
   vec3 N = normalize(normal);                                         // vertex shader로 부터 얻은 normal vector를 이용하여 N vector를 계산

   mat3 tbn = getTBN(N);                                               // getTBN에 노말 넣고 미분해서 계산
   float dBdu = texture(bumpTex, texCoords + vec2(0.00001,0)).r
              - texture(bumpTex, texCoords - vec2(0.00001,0)).r;       // u방향으로 미분해서 계산
   float dBdv = texture(bumpTex, texCoords + vec2(0,0.00001)).r
              - texture(bumpTex, texCoords - vec2(0,0.00001)).r;       // v방향으로 미분해서 계산

   N = normalize(N - dBdu*tbn[0]*100 - dBdv*tbn[1]*100);               // 새로 계산한 N

   vec3 V = normalize(cameraPosition - worldPosition);                 // cameraPosition - worldPosition 으로 V vector를 계산
   vec3 R = 2 * dot(L,N)*N - L;                                        // L, N vector를 이용하여 R vector를 계산
   vec3 I = lightColor/dot(l,l);                                       // 들어오는 빛의 밝기
   vec4 c4 = texture(diffTex,texCoords);                               // Texture 들고 옴
   vec3 c = c4.rgb * max(0,dot(L,N))*I + c4.rgb * ambientLight ;       // 색 결정을 위해 L과N DOT 프로덕트한 값에 0~1 사이 만들기 위해 max를 씌워 color.rgb 곱해주고 I 곱한 다음 ambientLight 더해줌
   c += pow(max(0,dot(R,V)),shininess)*I;                              // I 곱해주고 spaqular light는 1로 흰색이니 생략
   out_Color = vec4(pow(c,vec3(1/2.2)),1);                             // 알파 붙여서 rgb각각 2.2분의 1승 해서 vec4로 색 보냄
                                                                       // 즉 R, L, N, V vector와 ambient light color, shineness, 물체의 color를 이용하여 phong shading 결과를 계산

}