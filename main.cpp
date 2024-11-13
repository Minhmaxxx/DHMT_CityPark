#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <Vector_Matrix.h>

using namespace std;
//Thêm tam giác màu
static const float coloredTriangle[] = {
	//position			//colors
	 1.0, -1.0,  0.0,	1.0, 0.0, 0.0,//bottom right
	-1.0, -1.0,  0.0,	0.0, 1.0, 0.0,//bottom left
	 0.0,  1.0,  0.0,	0.0, 0.0, 1.0//top
};
typedef struct
{
	float xyzw[4];

	float normal[4];
} Vertex;

Vertex Vertices[] =
{
	// v0-v1-v2 (front)
	{ {  0.5f,	 0.5f,	0.5f, 1.0 },{ 0, 0, 1, 0 } },
	{ { -0.5f,	 0.5f,	0.5f, 1.0 },{ 0, 0, 1, 0 } },
	{ { -0.5f,	-0.5f,	0.5f, 1.0 },{ 0, 0, 1, 0 } },
	// v2-v3-v0
	{ { -0.5f,	-0.5f,	0.5f, 1.0 },{ 0, 0, 1, 0 } },
	{ {  0.5f,	-0.5f,	0.5f, 1.0 },{ 0, 0, 1, 0 } },
	{ {  0.5f,	 0.5f,	0.5f, 1.0 },{ 0, 0, 1, 0 } },

	// v0-v3-v4 (right)
	{ { 0.5f,	 0.5f,	 0.5f, 1.0 },{ 1, 0, 0, 0 } },
	{ { 0.5f,	-0.5f,   0.5f, 1.0 },{ 1, 0, 0, 0 } },
	{ { 0.5f,	-0.5f,  -0.5f, 1.0 },{ 1, 0, 0, 0 } },
	// v4-v5-v0
	{ { 0.5f,	-0.5f,	-0.5f, 1.0 },{ 1, 0, 0, 0 } },
	{ { 0.5f,	 0.5f,	-0.5f, 1.0 },{ 1, 0, 0, 0 } },
	{ { 0.5f,	 0.5f,   0.5f, 1.0 },{ 1, 0, 0, 0 } },

	// v0-v5-v6 (top)
	{ {  0.5f,	0.5f,	 0.5f, 1.0 },{ 0, 1, 0, 0 } },
	{ {  0.5f,	0.5f,	-0.5f, 1.0 },{ 0, 1, 0, 0 } },
	{ { -0.5f,	0.5f,	-0.5f, 1.0 },{ 0, 1, 0, 0 } },
	// v6-v1-v0
	{ { -0.5f,	0.5f,	-0.5f, 1.0 },{ 0, 1, 0, 0 } },
	{ { -0.5f,	0.5f,	 0.5f, 1.0 },{ 0, 1, 0, 0 } },
	{ {  0.5f,	0.5f,	 0.5f, 1.0 },{ 0, 1, 0, 0 } },

	// v1-v6-v7 (left)
	{ { -0.5f,	 0.5f,	 0.5f, 1.0 },{ -1, 0, 0, 0 } },
	{ { -0.5f,	 0.5f,	-0.5f, 1.0 },{ -1, 0, 0, 0 } },
	{ { -0.5f,	-0.5f,	-0.5f, 1.0 },{ -1, 0, 0, 0 } },
	// v7-v2-v1
	{ { -0.5f,	-0.5f,	-0.5f, 1.0 },{ -1, 0, 0, 0 } },
	{ { -0.5f,	-0.5f,	 0.5f, 1.0 },{ -1, 0, 0, 0 } },
	{ { -0.5f,	 0.5f,	 0.5f, 1.0 },{ -1, 0, 0, 0 } },

	// v7-v4-v3 (bottom)
	{ { -0.5f,	 -0.5f,	-0.5f, 1.0 },{ 0,-1, 0, 0 } },
	{ {  0.5f,	-0.5f,	-0.5f, 1.0 },{ 0,-1, 0, 0 } },
	{ {  0.5f,	-0.5f,	 0.5f, 1.0 },{ 0,-1, 0, 0 } },
	// v3-v2-v7
	{ {  0.5f,	-0.5f,	 0.5f, 1.0 },{ 0,-1, 0, 0 } },
	{ { -0.5f,	-0.5f,	 0.5f, 1.0 },{ 0,-1, 0, 0 } },
	{ { -0.5f,	-0.5f,	-0.5f, 1.0 },{ 0,-1, 0, 0 } },

	// v4-v7-v6 (back)
	{ {  0.5f,	-0.5f,	-0.5f, 1.0 },{ 0, 0,-1, 0 } },
	{ { -0.5f,	-0.5f,	-0.5f, 1.0 },{ 0, 0,-1, 0 } },
	{ { -0.5f,	 0.5f,	-0.5f, 1.0 },{ 0, 0,-1, 0 } },
	// v6-v5-v4
	{ { -0.5f,	 0.5f,	-0.5f, 1.0 },{ 0, 0,-1, 0 } },
	{ {  0.5f,	 0.5f,	-0.5f, 1.0 },{ 0, 0,-1, 0 } },
	{ {  0.5f,	-0.5f,	-0.5f, 1.0 },{ 0, 0,-1, 0 } }
};

const size_t BufferSize = sizeof(Vertices);
const size_t VertexSize = sizeof(Vertices[0]);
const size_t NormalOffset = sizeof(Vertices[0].xyzw);

// Định nghĩa stack cho ma trận
class MatrixStack {
	int    index;
	int    size;
	mat4* matrices;

	public:
		MatrixStack(int numMatrices = 32) :index(0), size(numMatrices)
		{
			matrices = new mat4[numMatrices];
		}

		~MatrixStack()
		{
			delete[]matrices;
		}

		// phép toán đẩy vào
		mat4& push(const mat4& m) {
			assert(index + 1 < size);
			matrices[index++] = m;
			return matrices[index];
		}

		// phép toán lấy ra
		mat4& pop(void) {
			assert(index - 1 >= 0);
			index--;
			return matrices[index];
		}
};

MatrixStack  mvstack;

int CurrentWidth = 700,
CurrentHeight = 700;

GLuint
VaoId[2],
VboId[2],
VertexShaderId,
FragmentShaderId,
ProgramId;

GLfloat yf = 0, xf = 0;

mat4
model_mat_cpp,
view_mat_cpp,
projection_mat_cpp;

int
model_mat_location,
view_mat_location,
projection_mat_location;

// Ma trận Xoay, Tỉ lệ, Tịnh tiến
float
R[] = { 0.0f, 0.0f, 0.0f },
S[] = { 1.0f, 1.0f, 1.0f },
T[] = { 0.0f, 0.0f, 0.0f };
//=============Sử dụng cho Camera============
int midWindowX;
int midWindowY;


bool keys[256]; // Array to keep track of pressed keys
bool mouseLocked = true;
float yaw = 0.0f;
float pitch = 0.0f;
float cameraX = 0.0f;
float cameraY = 0.0f;
float cameraZ = 5.0f;
float moveSpeed = 0.5f;
float jumpSpeed = 0.2f;
float gravity = 0.010f;
bool isJumping = false;
bool isGrounded = true;

// ======Dùng kiểm tra tịnh tiến, quay, co giãn====
bool is_translated = false,
is_rotated = false,
is_scaled = false;

//---------------------------------------------Khai báo các biến sử dụng để vẽ------------------------------------------
float Alpha = 0.0f;
//thiết lập chỉ mục cho menu
enum {
	//chair
	chair0,
	chair1,
	chair2,
	chair3,
	chair4,
	chair5,
	chair6,
	chair7,
	chair8,
	//table
	table0,
	table1,
	table2,
	table3,
	table4,
	table5,
	table6,
	table7,
	table8,
	Chair_TableJointx,
	Quit
};
GLfloat chair_tablex[Chair_TableJointx] = {
	0.0,//chair0 ghe giao vien
	0.0,//chair 1
	0.0,//chair 2
	0.0,//chair 3
	0.0,//chair 4
	0.0,//chair 5
	0.0,//chair 6
	0.0,//chair 7
	0.0,//chair 8
	0.0,//table 0 ( ban giao vien )
	0.0,//table 1
	0.0,//table 2
	0.0,//table 3
	0.0,//table 4
	0.0,//table 5
	0.0,//table 6
	0.0,//table 7
	0.0//table 8
};
//doi tuong tinh tien
GLint chair_table_x = 0;
enum {
	ngankeo0,
	ngankeo1,
	ngankeo2,
	ngankeo3,
	ngankeo4,
	ngankeo5,
	ngankeo6,
	ngankeo7,
	ngankeo8,
	ngankeoJoint
};
GLfloat ngankeo_z[ngankeoJoint]{
	0.0,// ngan keo 0
	0.0,// ngan keo 1
	0.0,// ngan keo 2
	0.0,// ngan keo 3
	0.0,// ngan keo 4
	0.0,// ngan keo 5
	0.0,// ngan keo 6
	0.0,// ngan keo 7
	0.0// ngan keo 8
};
GLint ngankeo = ngankeo1;
enum {
	windown1,
	windown2,
	windown3,
	windown4,
	windown5,
	windown6,
	windownJoint_z
};
GLfloat windown_rotate[windownJoint_z]{
	0.0,//windown1
	0.0,//windown2
	0.0,//windown3
	0.0,//windown4
	0.0,//windown5
	0.0//windown6
};


//--------------------------------------------------Kết thúc khai báo các biến vẽ---------------------------------

string ReadShaderSourceFile(string fileName) {
	fstream reader(fileName.c_str());
	string line;
	string code = "";
	while (getline(reader, line)) {
		code += line + "\n";
	}
	reader.close();
	return code;
}
// ------------------------------------------
void CreatVaoVbo()
{
	glGenVertexArrays(2, &VaoId[0]);
	glGenBuffers(2, &VboId[0]);
	glBindVertexArray(VaoId[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(coloredTriangle), coloredTriangle, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0); // Bật vertex_position
	glEnableVertexAttribArray(1); // Bật vertex_color
	glBindVertexArray(VaoId[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VboId[1]);
	glBufferData(GL_ARRAY_BUFFER, BufferSize, Vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, VertexSize, (void*)0);
	// Layout location 2 cho vertex_normal
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)NormalOffset);

	// Bật thuộc tính đỉnh
	glEnableVertexAttribArray(0); // Bật vertex_position
	glEnableVertexAttribArray(2); // Bật vertex_normal

	// Bật kiểm tra độ sâu và ẩn con trỏ chuột
	glEnable(GL_DEPTH_TEST);
	glutSetCursor(GLUT_CURSOR_NONE);
}
// ------------------------------------------
void CreatShaders()
{
	string vertexSrc = ReadShaderSourceFile("./vs.shader");
	string fragmentSrc = ReadShaderSourceFile("./fs.shader");

	const GLchar* VertexShader = vertexSrc.c_str();
	const GLchar* FragmentShader = fragmentSrc.c_str();

	VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShaderId, 1, &VertexShader, NULL);
	glCompileShader(VertexShaderId);

	FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShaderId, 1, &FragmentShader, NULL);
	glCompileShader(FragmentShaderId);

	ProgramId = glCreateProgram();
	glAttachShader(ProgramId, VertexShaderId);
	glAttachShader(ProgramId, FragmentShaderId);
	glLinkProgram(ProgramId);
	glUseProgram(ProgramId);
}
// ------------------------------------------
void CloseFunc()
{
	glUseProgram(0);

	glDetachShader(ProgramId, VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);

	glDeleteShader(FragmentShaderId);
	glDeleteShader(VertexShaderId);

	glDeleteProgram(ProgramId);

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VboId[0]);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoId[0]);
}
//--------------------------------------------
//-------------------------Ánh sáng----------------------------------
#pragma region light
bool light1 = true, light2 = true;

void updateLightUniforms() {
	//light1 = 1; light2 = 1;
	glUniform1i(glGetUniformLocation(ProgramId, "enable_light_1"), light1 ? 0 : 1);
	glUniform1i(glGetUniformLocation(ProgramId, "enable_light_2"), light2 ? 1 : 0);
}

void onl1() {
	light1 = true;
	updateLightUniforms();
}

void offl1() {
	light1 = false;
	updateLightUniforms();
}


// ************************************Gọi màu trong vs.shader************************************
void setInt(const string& name, int value) {
	glUniform1i(glGetUniformLocation(ProgramId, name.c_str()), value);
}
//-----------Tạo khối hộp cube đơn vị--------
void cube()
{
	mvstack.push(model_mat_cpp);
	mat4 instance = identity_mat4();
	mat4 model_cube = model_mat_cpp * instance;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_cube.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model_mat_cpp = mvstack.pop();
}
//-------------------------------

void ground() {
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(0.0, -1.05, -3.5)) * scale(vec3(15.0, 0.01, 15.0));
	setInt("color", 5);
	cube();
	model_mat_cpp = mvstack.pop();
}

//--------------ve phong hoc------------
#pragma region room and door
void sky()
{
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(0.0, 50.0, -25.0)) * scale(vec3(200.0, 0.01, 200.0));
	setInt("color", 16);
	cube();
	model_mat_cpp = mvstack.pop();
}

//------Vẽ ghế ăn----------
void chair() {
	mvstack.push(model_mat_cpp);
	//mat ghe
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * scale(vec3(0.6, 0.05, 0.6));
	setInt("color", 1);
	cube();
	model_mat_cpp = mvstack.pop();
	//chong tua trai
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(-0.275, 0.325, -0.275)) * scale(vec3(0.05, 0.6, 0.05));
	setInt("color", 2);
	cube();
	model_mat_cpp = mvstack.pop();
	//chong tua phai
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(0.275, 0.325, -0.275)) * scale(vec3(0.05, 0.6, 0.05));
	setInt("color", 2);
	cube();
	model_mat_cpp = mvstack.pop();
	//tua
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(0.0, 0.45, -0.275)) * scale(vec3(0.5, 0.3, 0.02));
	setInt("color", 1);
	cube();
	model_mat_cpp = mvstack.pop();
	//chan phai tren
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(0.275, -0.325, 0.275)) * scale(vec3(0.05, 0.6, 0.05));
	setInt("color", 2);
	cube();
	model_mat_cpp = mvstack.pop();
	//chan trai tren
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(-0.275, -0.325, 0.275)) * scale(vec3(0.05, 0.6, 0.05));
	setInt("color", 2);
	cube();
	model_mat_cpp = mvstack.pop();
	//chan phai duoi
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(0.275, -0.325, -0.275)) * scale(vec3(0.05, 0.6, 0.05));
	setInt("color", 2);
	cube();
	model_mat_cpp = mvstack.pop();
	//chan trai duoi
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(-0.275, -0.325, -0.275)) * scale(vec3(0.05, 0.6, 0.05));
	setInt("color", 2);
	cube();
	model_mat_cpp = mvstack.pop();
	model_mat_cpp = mvstack.pop();
}
//--------Vẽ CÁC ghế ăn---------
void chairs(float tx, float ty, float tz, float sx, float sy, float sz, float tmp, int j) {
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(tmp, 0.0, 0.0)) * translate(vec3(tx, ty, tz)) * scale(vec3(sx, sy, sz)) * rotate_y(360 / j);
	chair();
	model_mat_cpp = mvstack.pop();
}

//----------Vẽ bàn----------
#define chieucaoban 0.7
#define chieudaimat 1
#define chieurongmat 0.6
#define dodaymatban 0.01
#define dodaychanban 0.05
#define chieudaingan 0.8
#define chieurongngan 0.4
#define chieucaongan 0.5

// ------ Thông số cho cây ------
#define chieucao_than 0.8      // Chiều cao của thân cây
#define duong_kinh_than 0.1    // Đường kính của thân cây
#define chieucao_la 1.5f       // Chiều cao của phần lá (tán cây)
#define duong_kinh_la 1.0f     // Đường kính lớn nhất của phần lá

namespace banhoc {
	void matban()
	{
		mvstack.push(model_mat_cpp);
		setInt("color", 1);
		mat4 instance = identity_mat4();
		instance = translate(vec3(0.0, chieucaoban, chieurongmat / 2.0)) *
			scale(vec3(chieudaimat, dodaymatban, chieurongmat));

		mat4 model_torso = model_mat_cpp * instance;

		glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_torso.m);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model_mat_cpp = mvstack.pop();
	}

	void chanban()
	{
		mvstack.push(model_mat_cpp);
		setInt("color", 2);
		mat4 instance = identity_mat4();
		instance = translate(vec3(0, chieucaoban / 2.0, 0)) * scale(vec3(dodaychanban, chieucaoban, dodaychanban));

		mat4 model_left_upper_arm = model_mat_cpp * instance;

		glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_left_upper_arm.m);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model_mat_cpp = mvstack.pop();
	}

	void nganban()
	{
		mvstack.push(model_mat_cpp);
		setInt("color", 1);
		mat4 instance = identity_mat4();
		/*instance = translate(vec3(0, chieucaongan, chieurongmat / 2.0)) * scale(vec3(chieudaimat - (dodaychanban * 2.0), dodaymatban, chieurongmat - (dodaychanban * 2.0)));*/
		instance = translate(vec3(0, chieucaongan - 1, chieurongmat / 2.0)) * scale(vec3(chieudaimat - (dodaychanban * 2.0), dodaymatban, chieurongmat - (dodaychanban * 2.0)));
		mat4 model_torso = model_mat_cpp * instance;

		glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_torso.m);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model_mat_cpp = mvstack.pop();
	}

	void tamchanban() {
		mvstack.push(model_mat_cpp);
		setInt("color", 1);
		mat4 instance = identity_mat4();
		instance = translate(vec3(0, chieucaoban - 0.14, chieurongmat / 2 - 0.3)) * scale(vec3(chieudaimat - (dodaychanban * 2.0) - 0.05, 0.15, dodaymatban));
		mat4 model_torso = model_mat_cpp * instance;
		glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_torso.m);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model_mat_cpp = mvstack.pop();
	}
	void veban() {
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(0, -1, 0));
		// Mặt bàn
		banhoc::matban();

		// Chân bàn 1
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp *
			translate(vec3(-chieudaimat / 2.0 + dodaychanban, 0, dodaychanban));
		banhoc::chanban();
		model_mat_cpp = mvstack.pop();

		//Chân bàn 2
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp *
			translate(vec3(-chieudaimat / 2.0 + dodaychanban, 0, chieurongmat - dodaychanban));
		banhoc::chanban();
		model_mat_cpp = mvstack.pop();

		// Chân bàn 3
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp *
			translate(vec3(chieudaimat / 2.0 - dodaychanban, 0, dodaychanban));
		banhoc::chanban();
		model_mat_cpp = mvstack.pop();

		// Chân bàn 4
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp *
			translate(vec3(chieudaimat / 2.0 - dodaychanban, 0, chieurongmat - dodaychanban));
		banhoc::chanban();
		model_mat_cpp = mvstack.pop();

		//Ngăn bàn
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp *
			translate(vec3());
		banhoc::tamchanban();
		model_mat_cpp = mvstack.pop();

		model_mat_cpp = mvstack.pop();
	}
}
// Ve hang rao 
namespace hangrao {
	void veCot(float x, float y, float z, float width, float height, float depth) {
		mvstack.push(model_mat_cpp);

		// Màu sắc cột hàng rào (màu gỗ)
		setInt("color", 10);  // Giả sử màu số 10 là màu gỗ

		mat4 instance = identity_mat4();
		// Dịch chuyển tới vị trí cần vẽ
		instance = translate(vec3(x, y, z)) *
			scale(vec3(width, height, depth)); // Kích thước cột

		mat4 model_column = model_mat_cpp * instance;
		glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_column.m);

		// Vẽ cột (giả sử sử dụng một hình hộp)
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model_mat_cpp = mvstack.pop();
	}
	void veThanhNgang(float x, float y, float z, float width, float height, float depth) {
		mvstack.push(model_mat_cpp);

		// Màu sắc thanh ngang (màu gỗ)
		setInt("color", 10);

		mat4 instance = identity_mat4();
		// Dịch chuyển tới vị trí cần vẽ
		instance = translate(vec3(x, y/2, z)) *
			scale(vec3(width, height, depth)); // Kích thước thanh ngang

		mat4 model_bar = model_mat_cpp * instance;
		glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_bar.m);

		// Vẽ thanh ngang (giả sử sử dụng hình hộp)
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model_mat_cpp = mvstack.pop();
	}
	void veHangRao(float startX, float startY, float startZ, float spacing, int numColumns,
		float columnWidth, float columnHeight, float columnDepth,
		float barWidth, float barHeight, float barDepth) {
		
		// Vẽ các cột
	
		for (int i = 0; i < numColumns; ++i) {
			float x = startX + i * spacing;
		veCot(x, startY, startZ, columnWidth, columnHeight, columnDepth);
		}
		// Vẽ thanh ngang nối các cột
		for (int i = 1; i < numColumns - 2; ++i) {

			float x = startX + i * spacing + spacing / 2.0f;
			float z = startZ;  
			veThanhNgang(x, -0.8+startY + columnHeight / 2.0f, z, barWidth, barHeight, barDepth);
			veThanhNgang(x, -0.8+startY - columnHeight / 2.0f, z, barWidth, barHeight, barDepth);
		}
	}
}
namespace cay {

	// Hàm vẽ thân cây (hình trụ)
	void vethancay() {
		mvstack.push(model_mat_cpp);
		setInt("color", 10);  // Đặt màu cho thân cây (màu nâu hoặc màu phù hợp cho gỗ)

		mat4 instance = identity_mat4();
		instance = translate(vec3(0.0, chieucao_than / 2.0, 0.0)) * // Đặt thân cây từ gốc tọa độ đi lên
			scale(vec3(duong_kinh_than, chieucao_than, duong_kinh_than)); // Kích thước hình trụ cho thân cây

		mat4 model_trunk = model_mat_cpp * instance;

		glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_trunk.m);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model_mat_cpp = mvstack.pop();
	}

	// Hàm vẽ tán lá (hình nón)
	void velacay() {
		mvstack.push(model_mat_cpp);
		setInt("color", 5);  // Đặt màu cho tán lá (màu xanh lá cây hoặc phù hợp)

		mat4 instance = identity_mat4();
		instance = translate(vec3(0.0, chieucao_than + chieucao_la / 2.0, 0.0)) *  // Đặt tán lá ở trên đỉnh của thân cây
			scale(vec3(duong_kinh_la, chieucao_la, duong_kinh_la)); // Kích thước của hình nón cho tán lá

		mat4 model_leaves = model_mat_cpp * instance;

		glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_leaves.m);
		glDrawArrays(GL_TRIANGLES, 0, 36);  // Vẽ tán lá với hình nón

		model_mat_cpp = mvstack.pop();
	}

	// Hàm vẽ cây hoàn chỉnh với thân cây và tán lá
	void vecay() {
		mvstack.push(model_mat_cpp);

		// Vẽ thân cây
		vethancay();

		// Vẽ tán lá
		velacay();

		// Khôi phục lại ma trận gốc
		model_mat_cpp = mvstack.pop();
	}

	// Hàm vẽ cây với các tham số biến đổi (dịch chuyển và thay đổi kích thước)
	void vecay(float tx, float ty, float tz, float sx, float sy, float sz) {
		mvstack.push(model_mat_cpp);

		// Dịch chuyển và thay đổi kích thước thân cây
		model_mat_cpp = model_mat_cpp * translate(vec3(tx, -1.15, tz)) * scale(vec3(sx, sy, sz));
		cay::vethancay(); // Vẽ thân cây

		mvstack.push(model_mat_cpp);

		// Dịch chuyển để vẽ tán lá (tán lá có thể được vẽ ở phía trên thân cây)
		model_mat_cpp = model_mat_cpp * translate(vec3(0, 0, 0)); // Dịch chuyển tán lá lên trên
		cay::velacay(); // Vẽ tán lá, với radius_tan là bán kính của tán lá
		model_mat_cpp = model_mat_cpp * translate(vec3(0.5, 0.1, 0.5)); // Dịch chuyển tán lá lên trên
		cay::velacay();
		model_mat_cpp = model_mat_cpp * translate(vec3(-0.5, 0, 0.5)); // Dịch chuyển tán lá lên trên
		cay::velacay();
		model_mat_cpp = model_mat_cpp * translate(vec3(-0.5, 0.1, -0.5)); // Dịch chuyển tán lá lên trên
		cay::velacay();
		
		model_mat_cpp = mvstack.pop();

		model_mat_cpp = mvstack.pop();
	}

} 


void vebanhoc(float tx, float ty, float tz, float sx, float sy, float sz, GLfloat ngankeo) {
	mvstack.push(model_mat_cpp);

	model_mat_cpp = model_mat_cpp * translate(vec3(tx, ty, tz)) * scale(vec3(sx, sy, sz));
	banhoc::veban();
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(0, 0, ngankeo));
	banhoc::nganban();
	model_mat_cpp = mvstack.pop();

	model_mat_cpp = mvstack.pop();
}




void DisplayFunc(void)
{
	model_mat_cpp = identity_mat4();
	model_mat_location = glGetUniformLocation(ProgramId, "model_mat_shader");
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_mat_cpp.m);

	if (is_translated) {
		model_mat_cpp = model_mat_cpp * translate(vec3(T[0], T[1], T[2]));
		glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_mat_cpp.m);
	}
	if (is_rotated) {
		model_mat_cpp = model_mat_cpp * rotate_x(R[0]) * rotate_y(R[1]) * rotate_z(R[2]);
		glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_mat_cpp.m);
	}
	if (is_scaled) {
		model_mat_cpp = model_mat_cpp * scale(vec3(S[0], S[1], S[2]));  // Áp dụng tỉ lệ
		glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_mat_cpp.m);
	}

	glMatrixMode(GL_MODELVIEW);  // Chọn phép chiếu phối cảnh
	// Các tham số của hàm lookat
	vec3 eye(cameraX, cameraY, cameraZ),
		at(cameraX + sin(yaw), cameraY - pitch, cameraZ - cos(yaw)),
		up(0.0f, 1.0f, 0.0f);

	view_mat_cpp = lookat(eye, at, up);
	view_mat_location = glGetUniformLocation(ProgramId, "view_mat_shader");
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view_mat_cpp.m);

	float znear = 0.1f, zfar = 100.0f, fov = 90.0f, aspect = (float)CurrentWidth / CurrentHeight;
	projection_mat_cpp = perspective(fov, aspect, znear, zfar);
	projection_mat_location = glGetUniformLocation(ProgramId, "projection_mat_shader");
	glUniformMatrix4fv(projection_mat_location, 1, GL_FALSE, projection_mat_cpp.m);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//-----------------Let's draw together :))------------------
	// Mặt đất
	ground();

	// Bầu trời
	sky();

	// Bàn ăn
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(chair_tablex[table0], 0.1f, -5.5f)) * rotate_y(180);
	vebanhoc(-1.2, 0, 0, 1.2, 1.2, 0.8, ngankeo_z[ngankeo0]);
	cay::vecay(5, 0, 0, 1.4, 1.6, 1);
	cay::vecay(-5, 0, 0, 1.4, 1.6, 1);
	cay::vecay(5, 0, 3, 1.4, 1.6, 1);
	hangrao::veHangRao(-2, -0.75, -8, 1, 10, 0.1, 0.8, 0.1, 3, 0.1, 0.1); //float startX, float startY, float startZ, float spacing( khoảng cách giữa các cột)
	//, int numColumns,float columnWidth, float columnHeight, float columnDepth,
	//float barWidth, float barHeight, float barDepth) {
	model_mat_cpp = mvstack.pop();

	glutSwapBuffers();
}

//--------------Các hàm cho camera--------------
void updateCamera() {

	if (isJumping) {
		cameraY += jumpSpeed;
		jumpSpeed -= gravity;
		if (cameraY <= 0.0f) {
			cameraY = 0.0f;
			isJumping = false;
			isGrounded = true;
			jumpSpeed = 0.2f;
		}
	}
	glutPostRedisplay();
}
// ------------------------------------------
void ReshapeFunc(int Width, int Height)
{
	CurrentWidth = Width;
	CurrentHeight = Height;
	midWindowX = Width / 2;
	midWindowY = Height / 2;
	glViewport(0, 0, CurrentWidth, CurrentHeight);
}
// ------------------------------------------
bool phim1 = true, phim2 = true;
void IdleFunc(void)
{
	onl1();
	glutPostRedisplay();
}
//--------Con lăn chuột---------------
// Hàm callback xử lý cuộn chuột
void mouseWheel(int button, int direction, int x, int y) {
	if (direction > 0) {  // Cuộn chuột lên (phóng to)
		S[0] *= 1.1f;  // Phóng to đối tượng
		S[1] *= 1.1f;
		S[2] *= 1.1f;
	}
	else {  // Cuộn chuột xuống (thu nhỏ)
		S[0] /= 1.1f;  // Thu nhỏ đối tượng
		S[1] /= 1.1f;
		S[2] /= 1.1f;
	}

	// Sau khi thay đổi tỉ lệ, yêu cầu vẽ lại
	glutPostRedisplay();
}

// ------------------------------------------
void KeyboardFunc(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		glutLeaveMainLoop();
		break;
	//----------------camera----------------
	case 'w':
		cameraX += sin(yaw) * moveSpeed;
		cameraZ -= cos(yaw) * moveSpeed;
		break;
	case 's':
		cameraX -= sin(yaw) * moveSpeed;
		cameraZ += cos(yaw) * moveSpeed;
		break;
	case 'd':
		cameraX += sin(yaw - 3.1415926535 / 2) * moveSpeed;
		cameraZ -= cos(yaw - 3.1415926535 / 2) * moveSpeed;
		break;
	case 'a':
		cameraX += sin(yaw + 3.1415926535 / 2) * moveSpeed;
		cameraZ -= cos(yaw + 3.1415926535 / 2) * moveSpeed;
		break;
	case ' ':
		if (isGrounded) {
			isJumping = true;
			isGrounded = false;
		}
		keys[key] = true;
		break;
	case 'x':
		R[0] -= 10.0f;
		is_rotated = true;
		break;
	case 'X':
		R[0] += 10.0f;
		is_rotated = true;
		break;

	case 'y':
		R[1] -= 10.0f;
		is_rotated = true;
		break;
	case 'Y':
		R[1] += 10.0f;
		is_rotated = true;
		break;

	case 'z':
		R[2] -= 10.0f;
		is_rotated = true;
		break;
	case 'Z':
		R[2] += 10.0f;
		is_rotated = true;
		break;

	case '=':
	case '+':
		S[0] *= 1.05f;
		S[1] *= 1.05f;
		S[2] *= 1.05f;
		is_scaled = true;
		break;

	case '-':
	case '_':
		S[0] /= 1.05f;
		S[1] /= 1.05f;
		S[2] /= 1.05f;
		is_scaled = true;
		break;

	case 'j':
		T[0] -= 0.05f;
		is_translated = true;
		break;
	case 'l':
		T[0] += 0.05f;
		is_translated = true;
		break;
	case 'i':
		T[1] += 0.05f;
		is_translated = true;
		break;
	case ',':
		T[1] -= 0.05f;
		is_translated = true;
		break;
	case 'h':
		T[2] -= 0.05f;
		is_translated = true;
		break;
	case 'g':
		T[2] += 0.05f;
		is_translated = true;
		break;
	}
}
// ----------------------------------------

void SpecialFunc(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		T[0] -= 0.05f;
		is_translated = true;
		break;
	case GLUT_KEY_RIGHT:
		T[0] += 0.05f;
		is_translated = true;
		break;
	case GLUT_KEY_UP:
		T[1] += 0.05f;
		is_translated = true;
		break;
	case GLUT_KEY_DOWN:
		T[1] -= 0.05f;
		is_translated = true;
		break;
	case GLUT_KEY_ALT_R:
		T[2] -= 0.05f;
		is_translated = true;
		break;
	case GLUT_KEY_CTRL_R:
		T[2] += 0.05f;
		is_translated = true;
		break;
	}
}
void danhsachlenh() {
	cout << "g : Tinh tien vat the ve gan camera" << endl;
	cout << "h : Tinh tien vat the ra xa camera  " << endl;
	cout << "w : Camera len  " << endl;
	cout << "s : Camera xuong  " << endl;
	cout << "a : Camera sang trai  " << endl;
	cout << "d : Camera sang phai  " << endl;
	cout << "+ : Phong to " << endl;
	cout << "- : Thu nho " << endl;
	cout << ", : Dua camera len tren " << endl;
	cout << "i : Dua camera xuong duoi " << endl;
	cout << endl;
}
// ------------------------------------------
int main(int argc, char* argv[])
{
	danhsachlenh();
	glutInit(&argc, argv);
	glutInitContextVersion(4, 0);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutInitWindowSize(CurrentWidth, CurrentHeight);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutCreateWindow("khai");

	glewExperimental = GL_TRUE;
	glewInit();

	CreatVaoVbo();
	CreatShaders();

	glutDisplayFunc(DisplayFunc);
	glutReshapeFunc(ReshapeFunc);
	glutIdleFunc(IdleFunc);
	glutKeyboardFunc(KeyboardFunc);
	glutSpecialFunc(SpecialFunc);
	glutCloseFunc(CloseFunc);
	glutMouseWheelFunc(mouseWheel);
	glutMainLoop();
	return 0;
}
