//201921147 서동건
//Triangles 과제

#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "toys.h"                                                   //2. 첨부되어있는 toys.h를 이용

#pragma comment (lib,"opengl32")
#pragma comment (lib,"glfw3")
#pragma comment (lib,"glew32s")

void render(GLFWwindow* window);
void init();

int main()
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(640, 480, "Title", 0, 0);  //   GLFW를 이용하여 창을 열고
	glfwMakeContextCurrent(window);                                  //1. 이 Window의 context를 현재 context로 설정, 지정
	glewInit();                                                      //1.5glewInit()를 수행하여 상위 버전의 OpenGL 함수들을 활성화
	init();                                                          //   Init()함수 호출
	while (!glfwWindowShouldClose(window)) {                         //6. rendering 과정과 입력 처리를 위한 while loop를 만들고 윈도우 닫고싶지 않을때까지
		render(window);                                              //   render()함수 호출을 통해 rendering 과정을 수행
		glfwPollEvents();                                            //   사용자의 입력상태 GLFW 기억하고있어라
	}

}

GLuint vertexBuffer = 0;                                             //3. 초기화 과정에서 vertex 위치 정보를 위한 array buffer 생성
GLuint vertexArray = 0;                                              //4. 초기화 과정에서 vertex array 생성
GLuint elementBuffer = 0;                                            //5. 초기화 과정의 마지막으로 element array buffer 생성
Program program;                                                     //2. 전역변수로 Program 변수를 선언

using namespace glm;                                                 //네임스페이스 간결화를 위해 선언
using namespace std;                                                 //네임스페이스 간결화를 위해 선언

void init() {                                                        //init 정의
	program.loadShaders("shader.vert", "shader.frag");                                     //2. 초기화 과정에서 첨부되어있는 shader.vert, shader.frag shader파일을 읽어서 프로그램 제작
	vector<vec3> vertices = { {0,1,0},{-0.5,0,0},{0.5,0,0},{-1,-1,0},{0,-1,0},{1,-1,0} };  //3. 삼각형 3개를 만들기위해 만든 6개의 vertex 정보를
	vector<u16vec3> elements = {{0,1,2},{1,3,4},{2,4,5}};                                  //5. vertex의 index를 지정하여 elements에 저장 후
	glGenBuffers(1, &vertexBuffer);                                                                  //3. 버텍스 버퍼를 만들고
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);                                                     //   버퍼를 하이라이팅 시켜 사용하게 만든 뒤
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);  //3. 데이터 넣음

	glGenVertexArrays(1, &vertexArray);                                                              //4. 버텍스 어레이 만들고
	glBindVertexArray(vertexArray);                                                                  //   하이라이팅 시켜 사용하게 만든 뒤
	glEnableVertexAttribArray(0);                                                                    //4. vertex attribute 0번에 array buffer를 연결하는데
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0 );                                                 //   그0번자리에 버텍스당 숫자 3개씩 FLOAT 형으로 들어간다

	glGenBuffers(1, &elementBuffer);                                                                           //5. 엘리먼트 버퍼를 만들고
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);                                                      //   하이라이팅 시켜 사용하게 만든 뒤
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u16vec3) * elements.size(), elements.data(), GL_STATIC_DRAW); //   엘리먼트 데이터 넣음
}

void render(GLFWwindow* window) {                                    //   render 정의
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);                          //   프레임버퍼 사이즈 지정
	glViewport(0, 0, w, h);                                          //7. rendering 과정에서는 먼저 그림그릴 공간인 viewport를 설정
	glClearColor(0.2, 0, 0.4, 0);                                    //   화면을 클리어할때 쓸 색상 만든 후
	glClear(GL_COLOR_BUFFER_BIT);                                    //   클리어(지움)

	glUseProgram(program.programID);                                 //7. program을 사용하도록 한 뒤
	glBindVertexArray(vertexArray);                                  //   vertex array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);            //   element array를 각각 bind한 후
	glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_SHORT, 0);           //8. 인덱스를 사용하여 triangles를 그리고

	glfwSwapBuffers(window);                                         //9. triangle 그리는 과정이 끝나면 buffer를 swap 한다
}