#version 410 core

layout(location=0) in vec3 in_Position;  //   z좌표 0으로 두고 원래 3d 하고있었음
uniform mat4 transform;                  //2. 행렬로 돌리고싶으므로 유니폼하게 선언 해 행렬을 받을 수 있도록 하고
void main(void)
{
   vec4 p = vec4(in_Position.xyz,1);     //2. 4차원벡터p는 입력된 vertex position in_Position.xyz 에 1 붙여 행렬을 이용해 호모지니어스 코디네잇 만든 후
   p = transform*p;                      //   p에 transform 하여
   gl_Position= p;                       //   4차원 벡터이므로 그대로 p를 보내 gl_Position을 계산한다.
}
