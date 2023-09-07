//201921147 ������
//Texture mapping ����

#version 150 core

out vec4 out_Color;
uniform vec4 color;
uniform vec3 lightPosition;   // lightPosition ���� �� �ֵ��� �ϰ�
uniform vec3 lightColor;      // lightColor ���� �� �ֵ��� �ϰ�
uniform float shininess;      // shininess ���� �� �ֵ��� �ϰ�
uniform vec3 ambientLight;    // ambientLight ���� �� �ֵ��� �ϰ�
uniform vec3 cameraPosition;  // cameraPosition ���� �� �ֵ��� �ϰ�
uniform sampler2D diffTex;    // �ֺ������� �� �������� sampler2D �������� diffTex ���� �� �ֵ��� �ϰ�
uniform sampler2D bumpTex;    // �ֺ������� �� �������� sampler2D �������� bumpTex ���� �� �ֵ��� �ϰ�
in vec3 normal;               // fragment shader������ vertex shader�� ���� ���� normal ����
in vec3 worldPosition;        // fragment shader������ vertex shader�� ���� ���� worldPosition ����
in vec2 texCoords;            // fragment shader������ vertex shader�� ���� ���� texCoords ����

mat3 getTBN(vec3 N){          // T B N ���
   vec3 Q1 = dFdx(worldPosition), Q2 = dFdy(worldPosition);
   vec2 st1 = dFdx(texCoords), st2 = dFdy(texCoords);
   float D = st1.s*st2.t-st2.s*st1.t;
   return mat3(normalize(( Q1*st2.t - Q2*st1.t )*D),
   normalize((-Q1*st2.s + Q2*st1.s )*D), N);
}

void main(void)
{
   vec3 l = lightPosition - worldPosition;                             // ���� lightPosition - worldPosition�� �̿��Ͽ� L vector�� ���
   vec3 L = normalize(l);                                              // ���� 1�� ��������� �븻������
   vec3 N = normalize(normal);                                         // vertex shader�� ���� ���� normal vector�� �̿��Ͽ� N vector�� ���

   mat3 tbn = getTBN(N);                                               // getTBN�� �븻 �ְ� �̺��ؼ� ���
   float dBdu = texture(bumpTex, texCoords + vec2(0.00001,0)).r
              - texture(bumpTex, texCoords - vec2(0.00001,0)).r;       // u�������� �̺��ؼ� ���
   float dBdv = texture(bumpTex, texCoords + vec2(0,0.00001)).r
              - texture(bumpTex, texCoords - vec2(0,0.00001)).r;       // v�������� �̺��ؼ� ���

   N = normalize(N - dBdu*tbn[0]*100 - dBdv*tbn[1]*100);               // ���� ����� N

   vec3 V = normalize(cameraPosition - worldPosition);                 // cameraPosition - worldPosition ���� V vector�� ���
   vec3 R = 2 * dot(L,N)*N - L;                                        // L, N vector�� �̿��Ͽ� R vector�� ���
   vec3 I = lightColor/dot(l,l);                                       // ������ ���� ���
   vec4 c4 = texture(diffTex,texCoords);                               // Texture ��� ��
   vec3 c = c4.rgb * max(0,dot(L,N))*I + c4.rgb * ambientLight ;       // �� ������ ���� L��N DOT ���δ�Ʈ�� ���� 0~1 ���� ����� ���� max�� ���� color.rgb �����ְ� I ���� ���� ambientLight ������
   c += pow(max(0,dot(R,V)),shininess)*I;                              // I �����ְ� spaqular light�� 1�� ����̴� ����
   out_Color = vec4(pow(c,vec3(1/2.2)),1);                             // ���� �ٿ��� rgb���� 2.2���� 1�� �ؼ� vec4�� �� ����
                                                                       // �� R, L, N, V vector�� ambient light color, shineness, ��ü�� color�� �̿��Ͽ� phong shading ����� ���

}