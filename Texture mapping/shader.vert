//201921147 서동건
//Texture mapping 과제

#version 410 core

layout(location=0) in vec3 in_Position;  // location=0으로 두고 원래 3d 하고있었음
layout(location=1) in vec3 in_Normal;    // location=1으로 두고 원래 3d 하고있었음
layout(location=2) in vec2 in_TexCoords; // location=2으로 두고 원래 2d 하고있었음
uniform mat4 modelMat;                   // 행렬로 돌리고싶으므로 유니폼하게 선언 해 행렬을 받을 수 있도록 하고
uniform mat4 viewMat;                    // 행렬로 돌리고싶으므로 유니폼하게 선언 해 행렬을 받을 수 있도록 하고
uniform mat4 projMat;                    // 행렬로 돌리고싶으므로 유니폼하게 선언 해 행렬을 받을 수 있도록 하고
                                         // Vertex shader에서는 uniform으로 받은 3개의 행렬을 이용하여 vertex attribute로 받은 vertex 위치(model coordinate)를
out vec3 normal;                         // normal out하여 fragment shader로 전달
out vec3 worldPosition;                  // world coordinate 시스템에 있는 Position out하여 fragment shader로 전달
out vec2 texCoords;                      // texCoords fragment shader로 전달
void main(void)
{
   vec4 p = vec4(in_Position.xyz,1);                      // 4차원벡터p는 입력된 vertex position in_Position.xyz 에 1 붙여 행렬을 이용해 호모지니어스 코디네잇 만든 후
   p = projMat*viewMat*modelMat*p;                        // p에 m v p 매트릭스 하여 world coordinate->camera coordinate->normalized coordinate로 변환하고
   normal = normalize((modelMat*vec4(in_Normal,0)).xyz);  // 역시 vertex shader에서 normal vector의 world coordinate 상에서의 방향 계산 : 물체가 돌아가므로 형을 맞추기위해 in_Normal에 0을 붙여 translation 없에고 modelMat 곱한뒤 normalize(길이를 1로 만들어서) 보내줌 -> 스위즐
   worldPosition = vec3(modelMat*vec4(in_Position,1));    // vertex shader에서는 먼저 vertex의 world coordinate를 계산 : modeling coordinate 시스템에있는 in_Position에 modelMat 곱해서 world coordinate 시스템에 보냄
   texCoords = in_TexCoords;                              // texCoords 각각 interpulation해서 보내줌
   gl_Position= p;                                        // 그 결과를 gl_Position에 저장. 4차원 벡터이므로 그대로 p를 보내 gl_Position을 계산한다.
}
