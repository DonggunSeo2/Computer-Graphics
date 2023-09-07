//201921147 ������
//Lighted Hebe ����

#version 150 core

out vec4 out_Color;
uniform vec4 color;
uniform vec3 lightPosition;   // lightPosition ���� �� �ֵ��� �ϰ�
uniform vec3 lightColor;      // lightColor ���� �� �ֵ��� �ϰ�
uniform float shininess;      // shininess ���� �� �ֵ��� �ϰ�
uniform vec3 ambientLight;    // ambientLight ���� �� �ֵ��� �ϰ�
uniform vec3 cameraPosition;  // cameraPosition ���� �� �ֵ��� �ϰ�
in vec3 normal;               //h. fragment shader������ vertex shader�� ���� ���� normal ����
in vec3 worldPosition;        //h. fragment shader������ vertex shader�� ���� ���� worldPosition ����
void main(void)
{
   vec3 l = lightPosition - worldPosition;                             //i. ���� lightPosition - worldPosition�� �̿��Ͽ� L vector�� ���
   vec3 L = normalize(l);                                              //���� 1�� ��������� �븻������
   vec3 N = normalize(normal);                                         //j. vertex shader�� ���� ���� normal vector�� �̿��Ͽ� N vector�� ���
   vec3 V = normalize(cameraPosition - worldPosition);                 //h. cameraPosition - worldPosition ���� V vector�� ���
   vec3 R = 2 * dot(L,N)*N - L;                                        //k. L, N vector�� �̿��Ͽ� R vector�� ���
   vec3 I = lightColor/dot(l,l);                                       //l. ������ ���� ���
   vec3 c = color.rgb * max(0,dot(L,N))*I + color.rgb * ambientLight ; //l. �� ������ ���� L��N DOT ���δ�Ʈ�� ���� 0~1 ���� ����� ���� max�� ���� color.rgb �����ְ� I ���� ���� ambientLight ������
   c += pow(max(0,dot(R,V)),shininess)*I;                              //l. I �����ְ� spaqular light�� 1�� ����̴� ����
   out_Color = vec4(pow(c,vec3(1/2.2)),1);                             //l. ���� �ٿ��� rgb���� 2.2���� 1�� �ؼ� vec4�� �� ����
                                                                       //l. �� R, L, N, V vector�� ambient light color, shineness, ��ü�� color�� �̿��Ͽ� phong shading ����� ���
}