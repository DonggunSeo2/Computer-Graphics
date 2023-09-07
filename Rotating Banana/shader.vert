#version 410 core

layout(location=0) in vec3 in_Position;  //   z��ǥ 0���� �ΰ� ���� 3d �ϰ��־���
uniform mat4 transform;                  //2. ��ķ� ����������Ƿ� �������ϰ� ���� �� ����� ���� �� �ֵ��� �ϰ�
void main(void)
{
   vec4 p = vec4(in_Position.xyz,1);     //2. 4��������p�� �Էµ� vertex position in_Position.xyz �� 1 �ٿ� ����� �̿��� ȣ�����Ͼ �ڵ���� ���� ��
   p = transform*p;                      //   p�� transform �Ͽ�
   gl_Position= p;                       //   4���� �����̹Ƿ� �״�� p�� ���� gl_Position�� ����Ѵ�.
}
