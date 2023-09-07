//201921147 서동건
//Lighted Hebe 과제

#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> 
#include <glm/gtc/type_ptr.hpp>                                     // 각 파일로부터 데이터 포인터를 아름답게 얻어오기위한 기능
#include <glm/gtx/transform.hpp>                                    // mat4 매트리스 rotate함수 쓰기위해 glm한테 불러옴
#include <vector>
#include "toys.h"                                                   // 첨부되어있는 toys.h를 이용
#include "j3a.hpp"                                                  // 첨부되어있는 j3a.hpp 헤더파일 사용하기위해 불러옴

const float PI = 3.14159265358979f;                                 // 변하지 않는 PI값 실수형으로 const로 선언
#pragma comment (lib,"opengl32")
#pragma comment (lib,"glfw3")
#pragma comment (lib,"glew32s")

void render(GLFWwindow* window);
void init();
float cameratheta = 0;                                                    //b. 전역변수로 cameratheta 만듬
float cameraPhi = 0;                                                      //b. 전역변수로 cameraPhi 라디안 만듬
float cameraDistance = 5;                                                 //d. 원점을 중심으로 z축으로 5만큼 떨어져있는 카메라 초기위치 cameraDistance 5로 초기화
float fovy = 80 * PI / 180;                                               // 화각 초기값 80 * PI / 180 로 초기화

void cursorPosCallback(GLFWwindow* win, double xpos, double ypos) {       //a. 마우스 입력받기위해 cursorPosCallback 함수 선언, 윈도우에 연결
	static double lastX = 0;                                              // lastX 0으로 초기화
	static double lastY = 0;                                              // lastY 0으로 초기화
	if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_1)) {                   //b. 마우스가 눌렸을 때
		double dx = xpos - lastX;                                         //b. 마지막으로 기억하고 있는 cursor 위치 현재위치
		double dy = ypos - lastY;                                         //b. 마지막으로 기억하고 있는 cursor 위치 현재위치
		int w, h;
		glfwGetWindowSize(win, &w, &h);                                   //glfw로부터 window의 w,h값 받아옴
		cameratheta -= dx / w * PI;  //b. 마우스는 뒤집혀있으므로 - 붇힘, 마지막으로 기억하고 있는 cursor 위치 현재위치 윈도우 크기로 나누고 적당한 값 곱해 전역번수 cameratheta 조정
		cameraPhi -= dy / h * PI;    //b. 마우스는 뒤집혀있으므로 - 붇힘, 마지막으로 기억하고 있는 cursor 위치 현재위치 윈도우 크기로 나누고 적당한 값 곱해 전역번수 cameraPhi 조정
		if (cameraPhi < -PI / 2 + 0.01) {  // Viewing에서 phi 값이 -pi/2+0.01 < phi < pi/2-0.01 사이에 있도록
			cameraPhi = -PI / 2 + 0.01;
		}
		if (cameraPhi > PI / 2 - 0.01) {   // Viewing에서 phi 값이 -pi/2+0.01 < phi < pi/2-0.01 사이에 있도록
			cameraPhi = PI / 2 - 0.01;
		}
		printf("%.3f %.3f\n", cameratheta, cameraPhi);
	}  
	lastX = xpos;  //b. last 업데이트를 통해 버튼의 상태와 상관없이 마지막으로 기억하고있는 cursor 위치 업데이트
	lastY = ypos;  //b. last 업데이트를 통해 버튼의 상태와 상관없이 마지막으로 기억하고있는 cursor 위치 업데이트
}
void ScrollCallback(GLFWwindow* win, double xoffset, double yoffset) {    //a. 마우스 입력받기위해 ScrollCallback 함수 선언, 윈도우에 연결
	//cameraDistance = cameraDistance * pow(1.05, yoffset);
	fovy += yoffset / 30;            //c. 전역변수로 저장하고 있는 fovy(화각) 값 조정
	if (fovy < 0) {                  // FoV 값도 0< FoV < PI 가 되도록
		fovy = 0;
	}
	if (fovy > PI) {                 // FoV 값도 0< FoV < PI 가 되도록
		fovy = PI;
	}
}


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 8);                                 // 경계부분 부드럽게 해주기 위해 
	GLFWwindow* window = glfwCreateWindow(640, 480, "Title", 0, 0);  // GLFW를 이용하여 창을 열고
	glfwSetCursorPosCallback(window, cursorPosCallback);             //a. cursorPosCallback함수 만들고 window 연결
	glfwSetScrollCallback(window, ScrollCallback);                   //a. cursorPosCallback함수 만들고 window 연결
	glfwMakeContextCurrent(window);                                  // 이 Window의 context를 현재 context로 설정, 지정
	glewInit();                                                      // glewInit()를 수행하여 상위 버전의 OpenGL 함수들을 활성화
	init();                                                          // Init()함수 호출
	while (!glfwWindowShouldClose(window)) {                         // rendering 과정과 입력 처리를 위한 while loop를 만들고 윈도우 닫고싶지 않을때까지
		render(window);                                              // render()함수 호출을 통해 rendering 과정을 수행
		glfwPollEvents();                                            // 사용자의 입력상태 GLFW 기억하고있어라
	}

}

GLuint vertexBuffer = 0;                                             // 초기화 과정에서 vertex 위치 정보를 위한 array buffer 생성
GLuint normalBuffer = 0;                                             //b. 초기화 과정에서 normal 위치 정보를 위한 normal buffer 생성
GLuint vertexArray = 0;                                              // 초기화 과정에서 vertex array 생성
GLuint elementBuffer = 0;                                            // 초기화 과정의 마지막으로 element array buffer 생성
Program program;                                                     // 전역변수로 Program 변수를 선언

using namespace glm;                                                 // 네임스페이스 간결화를 위해 선언
using namespace std;                                                 // 네임스페이스 간결화를 위해 선언

vec3 lightPosition = vec3(3, 10, 5);   // * 초기 빛 값 3,10,5 로 설정 후 fragment shader로 보내줌
vec3 lightColor = vec3(100);           // * 초기 빛 색 100으로 흰색으로 줌
vec3 ambientLight = vec3(0.5);         // * 초기 ambientLight 0.5로 설정 후 fragment shader로 보내줌

void init() {                                                        // init 정의
	loadJ3A("Hebe.j3a");                                             // j3a.cpp를 이용해 Hebe.j3a파일을 읽고 Hebe 버텍스,엘리먼트 어레이 정보 각 Buffer에 전달 후 렌더링
	program.loadShaders("shader.vert", "shader.frag");                                             // 초기화 과정에서 첨부되어있는 shader.vert, shader.frag shader파일을 읽어서 프로그램 제작
	glGenBuffers(1, &vertexBuffer);                                                                // 버텍스 버퍼를 만들고
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);                                                   // 버퍼를 하이라이팅 시켜 사용하게 만든 뒤
	glBufferData(GL_ARRAY_BUFFER, nVertices[0] * sizeof(glm::vec3), vertices[0], GL_STATIC_DRAW);  // 여러 덩어리 중 첫번째 데이터 넣음

	glGenBuffers(1, &normalBuffer);                                                                //b. 노말 버퍼를 만들고
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);                                                   //b. 버퍼를 하이라이팅 시켜 사용하게 만든 뒤
	glBufferData(GL_ARRAY_BUFFER, nVertices[0] * sizeof(glm::vec3), normals[0], GL_STATIC_DRAW);   //b. J3A 파일을 읽을 때 얻은 normals[0]의 data를 저장. 여러 덩어리 중 첫번째 데이터 넣음

	glGenVertexArrays(1, &vertexArray);                                                            // 버텍스 어레이 만들고
	glBindVertexArray(vertexArray);                                                                // 하이라이팅 시켜 사용하게 만든 뒤
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);                                                   // 버퍼를 하이라이팅 시켜 사용하게 만든 뒤
	glEnableVertexAttribArray(0);                                                                  // vertex attribute 0번에 array buffer를 연결하는데
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0 );                                               // 그0번자리에 버텍스당 숫자 3개씩 FLOAT 형으로 들어간다

	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);                                                   //c. 버퍼를 하이라이팅 시켜 사용하게 만든 뒤
	glEnableVertexAttribArray(1);                                                                  //c. vertex attribute 1번에 normal buffer를 연결하는데 (버텍스 어트리뷰트 일련번호 1번으로 함)
	glVertexAttribPointer(1, 3, GL_FLOAT, 0, 0, 0);                                                //c. 그1번자리에 버텍스당 숫자 3개씩 FLOAT 형으로 바인드된 버퍼에서 들어간다(0,0,0)

	glGenBuffers(1, &elementBuffer);                                                                           // 엘리먼트 버퍼를 만들고
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);                                                      // 하이라이팅 시켜 사용하게 만든 뒤
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nTriangles[0] * sizeof(glm::u32vec3), triangles[0], GL_STATIC_DRAW); // 엘리먼트 데이터 넣음
}

void render(GLFWwindow* window) {                                    // render 정의
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);                          // 프레임버퍼 사이즈 지정
	glViewport(0, 0, w, h);                                          // rendering 과정에서는 먼저 그림그릴 공간인 viewport를 설정
	glClearColor(0.2f, 0, 0.4f, 0);                                  // 화면을 클리어할때 쓸 색상 만든 후
	glEnable(GL_DEPTH_TEST);                                         //a. 뒷면에 앞면 위에 그려지지 않다록 depth test를 켜고
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);                //a. 매 프레임마다 depth buffer를 지워주도록 함

	glUseProgram(program.programID);                                 // program을 사용하도록 한 뒤

	vec3 initialCameraPosition = vec3(0, 0, cameraDistance);                     //d. rendering 과정에서 먼저, 원점을 중심으로 z축으로 5만큼 떨어져 있는 카메라의 초기 위치를 전역변수에 저장하고 있는 카메라 방향 만큼 돌려 카메라의 위치를 계산
	mat4 cameraRotationMatrix1 = rotate(cameraPhi, vec3(1, 0, 0));               //x축으로 phi만큼 돌리는 매트릭스 만듬
	mat4 cameraRotationMatrix2 = rotate(cameratheta, vec3(0, 1, 0));             //y축으로 phi만큼 돌리는 매트릭스 만듬
	vec3 cameraPosition = cameraRotationMatrix2 * cameraRotationMatrix1 * vec4(initialCameraPosition,1);

	glfwSwapInterval(1);                                                         // Windows의 경우에는 framerate가 너무 빨라 생각보다 매우 빨리 돌아가는 경우 해결하기 위해 삽입

	mat4 viewMat = glm::lookAt(cameraPosition, vec3(0, 0, 0), vec3(0, 1, 0));    //e. glm::lookAt함수를 이용해 계산된 카메라위치, 보고싶은 물체의 위치(원점), view up vector(y축)에 해당하는 viewing transform을 계산. At 뒤에 아무것도 안씀 -> right 핸드 시스템
	mat4 projMat = glm::perspective(fovy, w / (float)h, 0.01f, 1000.f);          //f. glm::perspective 를 이용하여 전역변수로 저장된 화각과, 윈도우(또는 framebuffer)의 크기로 부터 계산한 종횡비(aspect ratio), 적절한 far, near 값에 해당되는 projection matrix를 계산. 바나나 보이도록 프로젝션을 같이 진행, 멀리가도 사라지지 않도록 far 1000까지
	GLuint modelMatLocation = glGetUniformLocation(program.programID, "modelMat"); // vertex shader에 uniform으로 설정된 행렬의 Location을 얻어오고 (프로그램 유니폼 세팅을 위해)
	glUniformMatrix4fv(modelMatLocation, 1, 0, value_ptr(mat4(1)));              //g. model transform은 단위 행렬(identity matrix, glm::mat4(1))로 함. 만든 행렬을 uniform 행렬 변수로 전달. 우리가 보내줄 데이터 4by4 float형 행렬1개, glm은 트랜스포즈 안해도 되니까 0(false),
	// 실제로 데이터의 벨류에 해당하는 부분의 포인터 가지고 와서 modelMat 데이터 넘겨주기 가능

	GLuint viewMatLocation = glGetUniformLocation(program.programID, "viewMat"); // vertex shader에 uniform으로 설정된 행렬의 Location을 얻어오고 (프로그램 유니폼 세팅을 위해)
	glUniformMatrix4fv(viewMatLocation, 1, 0, value_ptr(viewMat));               // 만든 행렬을 uniform 행렬 변수로 전달. 우리가 보내줄 데이터 4by4 float형 행렬1개, glm은 트랜스포즈 안해도 되니까 0(false),
	//h. 계산한 3개의 행렬을 shader의 uniform으로 각각 전달

	GLuint projMatLocation = glGetUniformLocation(program.programID, "projMat"); // vertex shader에 uniform으로 설정된 행렬의 Location을 얻어오고 (프로그램 유니폼 세팅을 위해)
	glUniformMatrix4fv(projMatLocation, 1, 0, value_ptr(projMat));               // 만든 행렬을 uniform 행렬 변수로 전달. 우리가 보내줄 데이터 4by4 float형 행렬1개, glm은 트랜스포즈 안해도 되니까 0(false),
	//h. 계산한 3개의 행렬을 shader의 uniform으로 각각 전달

	GLuint colorLocation = glGetUniformLocation(program.programID, "color");                   // 프로그램 유니폼 세팅을 위해 프로그램과 변수이름으로 로케이션을 찾아냄
	glUniform4fv(colorLocation, 1, value_ptr(diffuseColor[0]));                                // 우리가 보내줄 데이터 파일에 써있는 컬러[0]번, 보내줄데이터타입=float, 배열로 묶어 한번에 보냄
	//e. 이외에 필요한 color(vec4) uniform으로 shader에 전달

	GLuint shininessLocation = glGetUniformLocation(program.programID, "shininess");           // 프로그램 유니폼 세팅을 위해 프로그램과 변수이름으로 로케이션을 찾아냄
	glUniform1f(shininessLocation, shininess[0]);                                              // 우리가 보내줄 데이터 파일에 있는 shininess[0]번, 보내줄데이터타입=float
	//e. 이외에 필요한 shineness(float) uniform으로 shader에 전달

	GLuint lightPositionLocation = glGetUniformLocation(program.programID, "lightPosition");   //d. shader에 uniform으로 설정된 행렬의 Location을 얻어오고 (프로그램 유니폼 세팅을 위해)
	glUniform3fv(lightPositionLocation, 1, value_ptr(lightPosition));                          //d. 만든 행렬을 uniform 행렬 변수로 전달. 우리가 보내줄 데이터
	//d. rendering 과정에서는 light의 위치 uniform으로 fragment shader에 전달

	GLuint lightColorLocation = glGetUniformLocation(program.programID, "lightColor");         //d. shader에 uniform으로 설정된 행렬의 Location을 얻어오고 (프로그램 유니폼 세팅을 위해)
	glUniform3fv(lightColorLocation, 1, value_ptr(lightColor));                                //d. 만든 행렬을 uniform 행렬 변수로 전달. 우리가 보내줄 데이터
	//d. rendering 과정에서는 light의 lightColor uniform으로 fragment shader에 전달

	GLuint ambientLightLocation = glGetUniformLocation(program.programID, "ambientLight");     //d. shader에 uniform으로 설정된 행렬의 Location을 얻어오고 (프로그램 유니폼 세팅을 위해)
	glUniform3fv(ambientLightLocation, 1, value_ptr(ambientLight));                            //d. 만든 행렬을 uniform 행렬 변수로 전달. 우리가 보내줄 데이터
	//e. 이외에 필요한 ambient light(vec3) uniform으로 shader에 전달

	GLuint cameraPositionLocation = glGetUniformLocation(program.programID, "cameraPosition"); //d. shader에 uniform으로 설정된 행렬의 Location을 얻어오고 (프로그램 유니폼 세팅을 위해)
	glUniform3fv(cameraPositionLocation, 1, value_ptr(cameraPosition));                        //d. 만든 행렬을 uniform 행렬 변수로 전달. 우리가 보내줄 데이터
	//d. rendering 과정에서는 camera의 위치 uniform으로 fragment shader에 전달
	
	glBindVertexArray(vertexArray);                                              // vertex array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);                        // element array를 각각 bind한 후
	glDrawElements(GL_TRIANGLES, nTriangles[0] * 3, GL_UNSIGNED_INT, 0);   
	// 파일에서 읽은 삼각형 수 * 3 만큼 인덱스를 사용하여 큰 모델을 들고오기 위해 65356을 벗어나는 UNSIGNED_INT 형으로 버텍스 개수에 따라 triangles을 그림

	glfwSwapBuffers(window);                                                     // triangle 그리는 과정이 끝나면 buffer를 swap 한다
}