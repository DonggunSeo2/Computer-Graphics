//201921147 ������
//Texture mapping ����

#version 410 core

layout(location=0) in vec3 in_Position;  // location=0���� �ΰ� ���� 3d �ϰ��־���
layout(location=1) in vec3 in_Normal;    // location=1���� �ΰ� ���� 3d �ϰ��־���
layout(location=2) in vec2 in_TexCoords; // location=2���� �ΰ� ���� 2d �ϰ��־���
uniform mat4 modelMat;                   // ��ķ� ����������Ƿ� �������ϰ� ���� �� ����� ���� �� �ֵ��� �ϰ�
uniform mat4 viewMat;                    // ��ķ� ����������Ƿ� �������ϰ� ���� �� ����� ���� �� �ֵ��� �ϰ�
uniform mat4 projMat;                    // ��ķ� ����������Ƿ� �������ϰ� ���� �� ����� ���� �� �ֵ��� �ϰ�
                                         // Vertex shader������ uniform���� ���� 3���� ����� �̿��Ͽ� vertex attribute�� ���� vertex ��ġ(model coordinate)��
out vec3 normal;                         // normal out�Ͽ� fragment shader�� ����
out vec3 worldPosition;                  // world coordinate �ý��ۿ� �ִ� Position out�Ͽ� fragment shader�� ����
out vec2 texCoords;                      // texCoords fragment shader�� ����
void main(void)
{
   vec4 p = vec4(in_Position.xyz,1);                      // 4��������p�� �Էµ� vertex position in_Position.xyz �� 1 �ٿ� ����� �̿��� ȣ�����Ͼ �ڵ���� ���� ��
   p = projMat*viewMat*modelMat*p;                        // p�� m v p ��Ʈ���� �Ͽ� world coordinate->camera coordinate->normalized coordinate�� ��ȯ�ϰ�
   normal = normalize((modelMat*vec4(in_Normal,0)).xyz);  // ���� vertex shader���� normal vector�� world coordinate �󿡼��� ���� ��� : ��ü�� ���ư��Ƿ� ���� ���߱����� in_Normal�� 0�� �ٿ� translation ������ modelMat ���ѵ� normalize(���̸� 1�� ����) ������ -> ������
   worldPosition = vec3(modelMat*vec4(in_Position,1));    // vertex shader������ ���� vertex�� world coordinate�� ��� : modeling coordinate �ý��ۿ��ִ� in_Position�� modelMat ���ؼ� world coordinate �ý��ۿ� ����
   texCoords = in_TexCoords;                              // texCoords ���� interpulation�ؼ� ������
   gl_Position= p;                                        // �� ����� gl_Position�� ����. 4���� �����̹Ƿ� �״�� p�� ���� gl_Position�� ����Ѵ�.
}
