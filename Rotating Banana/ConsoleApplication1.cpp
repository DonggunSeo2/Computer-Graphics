//201921147 서동건
//Rotating Banana 과제

#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> 
#include <glm/gtc/type_ptr.hpp>                                     //각 파일로부터 데이터 포인터를 아름답게 얻어오기위한 기능
#include <glm/gtx/transform.hpp>                                    //4.2 mat4 매트리스 rotate함수 쓰기위해 glm한테 불러옴
#include <vector>
#include "toys.h"                                                   //첨부되어있는 toys.h를 이용
#include "j3a.hpp"                                                  //1. 첨부되어있는 j3a.hpp 헤더파일 사용하기위해 불러옴

const float PI = 3.14159265358979f;                                 //변하지 않는 PI값 실수형으로 const로 선언
#pragma comment (lib,"opengl32")
#pragma comment (lib,"glfw3")
#pragma comment (lib,"glew32s")

void render(GLFWwindow* window);
void init();
float  theta = 0;                                                    //3. 전역변수로 main 프로그램에서 회전 각(radian) 만듬

int main()
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(640, 480, "Title", 0, 0);  // GLFW를 이용하여 창을 열고
	glfwMakeContextCurrent(window);                                  // 이 Window의 context를 현재 context로 설정, 지정
	glewInit();                                                      // glewInit()를 수행하여 상위 버전의 OpenGL 함수들을 활성화
	init();                                                          // Init()함수 호출
	while (!glfwWindowShouldClose(window)) {                         // rendering 과정과 입력 처리를 위한 while loop를 만들고 윈도우 닫고싶지 않을때까지
		render(window);                                              // render()함수 호출을 통해 rendering 과정을 수행
		glfwPollEvents();                                            // 사용자의 입력상태 GLFW 기억하고있어라
	}

}

GLuint vertexBuffer = 0;                                             // 초기화 과정에서 vertex 위치 정보를 위한 array buffer 생성
GLuint vertexArray = 0;                                              // 초기화 과정에서 vertex array 생성
GLuint elementBuffer = 0;                                            // 초기화 과정의 마지막으로 element array buffer 생성
Program program;                                                     // 전역변수로 Program 변수를 선언

using namespace glm;                                                 // 네임스페이스 간결화를 위해 선언
using namespace std;                                                 // 네임스페이스 간결화를 위해 선언

void init() {                                                        //init 정의
	loadJ3A("banana.j3a");                                           //1. j3a.cpp를 이용해 banana.j3a파일을 읽고 바나나 버텍스,엘리먼트 어레이 정보 각 Buffer에 전달 후 렌더링
	program.loadShaders("shader.vert", "shader.frag");                                             // 초기화 과정에서 첨부되어있는 shader.vert, shader.frag shader파일을 읽어서 프로그램 제작
	glGenBuffers(1, &vertexBuffer);                                                                // 버텍스 버퍼를 만들고
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);                                                   // 버퍼를 하이라이팅 시켜 사용하게 만든 뒤
	glBufferData(GL_ARRAY_BUFFER, nVertices[0] * sizeof(glm::vec3), vertices[0], GL_STATIC_DRAW);  //여러 덩어리 중 첫번째 데이터 넣음

	glGenVertexArrays(1, &vertexArray);                                                            // 버텍스 어레이 만들고
	glBindVertexArray(vertexArray);                                                                // 하이라이팅 시켜 사용하게 만든 뒤
	glEnableVertexAttribArray(0);                                                                  // vertex attribute 0번에 array buffer를 연결하는데
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0 );                                               // 그0번자리에 버텍스당 숫자 3개씩 FLOAT 형으로 들어간다

	glGenBuffers(1, &elementBuffer);                                                                           // 엘리먼트 버퍼를 만들고
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);                                                      // 하이라이팅 시켜 사용하게 만든 뒤
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nTriangles[0] * sizeof(glm::u32vec3), triangles[0], GL_STATIC_DRAW); // 엘리먼트 데이터 넣음
}

void render(GLFWwindow* window) {                                    // render 정의
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);                          // 프레임버퍼 사이즈 지정
	glViewport(0, 0, w, h);                                          // rendering 과정에서는 먼저 그림그릴 공간인 viewport를 설정
	glClearColor(0.2f, 0, 0.4f, 0);                                  // 화면을 클리어할때 쓸 색상 만든 후
	glClear(GL_COLOR_BUFFER_BIT);                                    // 클리어(지움)

	glUseProgram(program.programID);                                 // program을 사용하도록 한 뒤

	GLuint colorLocation = glGetUniformLocation(program.programID, "color");  // 프로그램 유니폼 세팅을 위해 프로그램과 변수이름으로 로케이션을 찾아냄
	glUniform4fv(colorLocation, 1, value_ptr(vec4(1, 1, 0, 1)));              // 우리가 보내줄 데이터 vec4숫자4개-노란색, 보내줄데이터타입=float, 배열로 묶어 한번에 보냄
	//실제로 데이터의 벨류에 해당하는 부분의 포인터 가지고 와서 1,1,0,1 float형 데이터 넘겨주기 가능

	theta += 5.0f/180*PI;    //3~4 세타값 5도를 180*PI로 나누어 라디안으로 만들고 매번 randering 프레임마다 그 값을 약간씩 증가시킨다
	glfwSwapInterval(1);                                             //Windows의 경우에는 framerate가 너무 빨라 생각보다 매우 빨리 돌아가는 경우 해결하기 위해 삽입
	mat4 rotationMatrix = rotate(theta,vec3(0,1,0));                 //4.2 y축으로 돌리기 위해 rotate함수을 사용해 이 회전각에 해당하는 회전 매트릭스 행렬(4by4) 만듬
	GLuint transformLocation = glGetUniformLocation(program.programID, "transform"); // 4.3 vertex shader에 uniform으로 설정된 행렬의 Location을 얻어오고 (프로그램 유니폼 세팅을 위해)
	glUniformMatrix4fv(transformLocation, 1, 0, value_ptr(rotationMatrix));          // 4.4 만든 행렬을 uniform 행렬 변수로 전달. 우리가 보내줄 데이터 4by4 float형 행렬1개, glm은 트랜스포즈 안해도 되니까 0(false),
	//실제로 데이터의 벨류에 해당하는 부분의 포인터 가지고 와서 rotationMatrix 데이터 넘겨주기 가능
	glBindVertexArray(vertexArray);                                  // vertex array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);            // element array를 각각 bind한 후
	glDrawElements(GL_TRIANGLES, nTriangles[0] * 3, GL_UNSIGNED_INT, 0);   
	//4.5 파일에서 읽은 삼각형 수 * 3 만큼 인덱스를 사용하여 큰 모델을 들고오기 위해 65356을 벗어나는 UNSIGNED_INT 형으로 버텍스 개수에 따라 triangles을 그림

	glfwSwapBuffers(window);                                         // triangle 그리는 과정이 끝나면 buffer를 swap 한다
}