/*
* PROYECTO FINAL: DIORAMA
Semestre 2023-2
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>
#include <ctime>
#include <iostream>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include "Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include <chrono>

//Implementación del audio
#include "irrKlang.h"

const float toRadians = 3.14159265f / 180.0f;
const float PI = 3.14159265f;




//variables para animación
float toffsetu;
float toffsetv;

bool dia;
float glx;
float gly;
float glz;
float gl;

float rotColumpio, rotSyB;
float rotColumpioOffset, rotSyBOffset;
bool BanColumpio, BanSyB;
float incRot;

float rotBrazoIzqPiernaDer = 0;
float rotBrazoDerPiernaIzq = 0;
bool dirBrazosPiernas = false;

float rotBrazoD;
float rotBrazoDOffset;
bool BanRegreso;

float rotBrazoI;
float rotBrazoIOffset;
bool BanRegreso2;

//key frames

float rotMolino = 0;

float reproduciranimacion, habilitaranimacion, reinicioFrame;
bool animPersonaje;

float arrastreRio = 0;
float efecto = 0;
float recorrido = 0;

//Banderas
int bandia = 0;
/*unsigned t0, t1;
double time;*/

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;
Camera camIso;

Texture toroideTexture;
Texture plainTexture;
Texture pisoTexture;
Texture estatua1Texture;
Texture estatua2Texture;
Texture estatua3Texture;
Texture estatua4Texture;


Model Arboles;
Model CasaParque;
Model Fuente;
Model cabaña1;
Model cabaña2;
Model Reja;
Model luciernaga;
Model LamparaFarola;
Model Entrada;
Model CasaJapones;
Model Puente;
Model Base;
Model Banca;
Model Farola;
Model Aang;
Model Rocas;
Model Poste;
Model Tronco;
Model GregCuerpo;
Model GregBrazoIzq;
Model GregBrazoDer;
Model GregPiernaIzq;
Model GregPiernaDer;

Model Rio;
Model ViejoMolino;
Model MolinoAgua;
Model LinternaLenador;

Model Arbol_col;
Model Columpio;

//Model Casa2;
Model MordecaiCuerpo;
Model MordecaiBrazoIzq;
Model MordecaiBrazoDer;Model arbol;
Model arbusto;
Model cerezo;
Model LamparaP;

Skybox skyboxDia;
Skybox skyboxNoche;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

//PARA INPUT CON KEYFRAMES 
void inputKeyframes(bool* keys);

//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};
	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int estatuaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat estatuaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(estatuaVertices, estatuaIndices, 32, 6);
	meshList.push_back(obj6);
}

void CreateToroide() {

	const int numSegments = 36;  // Número de segmentos alrededor del toroide
	const int numSlices = 18;    // Número de segmentos a lo largo del toroide
	const float majorRadius = 1.0f;  // Radio mayor del toroide (desde el centro al exterior)
	const float minorRadius = 0.3f;  // Radio menor del toroide (tamaño del tubo)

	float vertices[(numSlices + 1) * (numSegments + 1) * 8];  // x, y, z, u, v, nx, ny, nz
	unsigned int indices[numSlices * numSegments * 6];

	int vertexIndex = 0;
	int index = 0;

	for (int i = 0; i <= numSlices; ++i) {
		float sliceAngle = 2.0f * PI * i / numSlices;
		float sliceCos = cos(sliceAngle);
		float sliceSin = sin(sliceAngle);

		for (int j = 0; j <= numSegments; ++j) {
			float segmentAngle = 2.0f * PI * j / numSegments;
			float segmentCos = cos(segmentAngle);
			float segmentSin = sin(segmentAngle);

			// Posiciones del vértice
			float x = (majorRadius + minorRadius * segmentCos) * sliceCos;
			float y = (majorRadius + minorRadius * segmentCos) * sliceSin;
			float z = minorRadius * segmentSin;

			// Coordenadas de textura (opcional)
			float u = static_cast<float>(j) / numSegments;
			float v = static_cast<float>(i) / numSlices;

			// Normales (en este caso, las mismas que las posiciones)
			float nx = x;
			float ny = y;
			float nz = z;

			// Agregar vértice al arreglo
			vertices[vertexIndex++] = x;
			vertices[vertexIndex++] = y;
			vertices[vertexIndex++] = z;
			vertices[vertexIndex++] = u;
			vertices[vertexIndex++] = v;
			vertices[vertexIndex++] = -nx;
			vertices[vertexIndex++] = -ny;
			vertices[vertexIndex++] = -nz;

			if (i < numSlices && j < numSegments) {
				int currRow = i * (numSegments + 1);
				int nextRow = (i + 1) * (numSegments + 1);

				// Primer triángulo
				indices[index++] = currRow + j;
				indices[index++] = nextRow + j;
				indices[index++] = currRow + j + 1;

				// Segundo triángulo
				indices[index++] = currRow + j + 1;
				indices[index++] = nextRow + j;
				indices[index++] = nextRow + j + 1;
			}
		}
	}
	//se genera el mesh del Toroide

	Mesh* toroide = new Mesh();
	toroide->CreateMesh(vertices, indices, (numSlices + 1) * (numSegments + 1) * 8, numSlices * numSegments * 6);
	meshList.push_back(toroide);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

///////////////////////////////KEYFRAMES/////////////////////

bool animacion = false;


//NEW// Keyframes
float posXkey = 0.0f, posYkey = 0.0f, posZkey = 0.0f;
float levAang = 0.0f, levRocs = 0.0f, levEst = 0.0f;
float inclRocs = 0, giroRocs = 0;

#define MAX_FRAMES 25 //Numero de frames
int i_max_steps = 100; //Interpolacion, mayor: fluida, menor: menos fluida
int i_curr_steps = 32;

typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float levAang;		
	float levRocs;		
	float levEst;		
	float levAang_Inc;	
	float levRocs_Inc;	
	float levEst_Inc;	
	float inclRocs;		
	float inclRocs_Inc;
	float giroRocs;
	float giroRocs_Inc;
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 32;			//introducir datos
bool play = false;
int playIndex = 0;

//
//void saveFrame(void)
//{
//	printf("---------------------------------------\n");
//	printf("Frameindex   = %d\n", FrameIndex);
//	printf("i_curr_steps = %d\n", i_curr_steps);
//
//	KeyFrame[FrameIndex].levAang = levAang;
//	KeyFrame[FrameIndex].movKey_y = movKey_z;
//	KeyFrame[FrameIndex].movKey_z = movKey_z;
//	KeyFrame[FrameIndex].giroKey = giroKey;
//
//	//Imprimir en una consola
//	//printf("\nKeyFrame[%d].movKey_x = %f\n", FrameIndex, movKey_x);
//	//printf("KeyFrame[%d].movKey_z = %f\n", FrameIndex, movKey_z);
//	//printf("KeyFrame[%d].giroKey = %f\n", FrameIndex, giroKey);
//
//	FrameIndex++;
//}

void resetElements(void)
{
	levAang = KeyFrame[0].levAang;
	levRocs = KeyFrame[0].levRocs;
	levEst = KeyFrame[0].levEst;
	inclRocs = KeyFrame[0].inclRocs;
	giroRocs = KeyFrame[0].giroRocs;
}

void interpolation(void)
{
	KeyFrame[playIndex].levAang_Inc = (KeyFrame[playIndex + 1].levAang - KeyFrame[playIndex].levAang) / i_max_steps;
	KeyFrame[playIndex].levRocs_Inc = (KeyFrame[playIndex + 1].levRocs - KeyFrame[playIndex].levRocs) / i_max_steps;
	KeyFrame[playIndex].levEst_Inc = (KeyFrame[playIndex + 1].levEst - KeyFrame[playIndex].levEst) / i_max_steps;
	KeyFrame[playIndex].inclRocs_Inc = (KeyFrame[playIndex + 1].inclRocs - KeyFrame[playIndex].inclRocs) / i_max_steps;
	KeyFrame[playIndex].giroRocs_Inc = (KeyFrame[playIndex + 1].giroRocs - KeyFrame[playIndex].giroRocs) / i_max_steps;

}


void animate(void)
{
	//Movimiento del objeto
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			printf("playindex : %d\n", playIndex);
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("Frame index= %d\n", FrameIndex);
				printf("Termina animacion\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			levAang += KeyFrame[playIndex].levAang_Inc;
			levRocs += KeyFrame[playIndex].levRocs_Inc;
			levEst += KeyFrame[playIndex].levEst_Inc;
			inclRocs += KeyFrame[playIndex].inclRocs_Inc;
			giroRocs += KeyFrame[playIndex].giroRocs_Inc;
			i_curr_steps++;
		}

	}
}

/* FIN KEYFRAMES*/


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	
	CreateObjects();
	CreateToroide();
	CreateShaders();

	camera = Camera(glm::vec3(-90.0f, 4.0f, 80.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 0.2f, 0.2f);//Ligada al planoXZ
	camIso = Camera(glm::vec3(-150.0f, 150.0f, 150.0f), glm::vec3(0.0f, 1.0f, 0.0f), -45.0f, -45.0f, 0.5f, 0.5f);//Isometrica
	
	//Original
	//camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	toroideTexture = Texture("Textures/toroide.jpg");
	toroideTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	estatua1Texture = Texture("Textures/Estatua1.png");
	estatua1Texture.LoadTextureA();
	estatua2Texture = Texture("Textures/Estatua2.png");
	estatua2Texture.LoadTextureA();
	estatua3Texture = Texture("Textures/Estatua3.png");
	estatua3Texture.LoadTextureA();
	estatua4Texture = Texture("Textures/Estatua4.png");
	estatua4Texture.LoadTextureA();
	Arboles = Model();
	Arboles.LoadModel("Models/Arboles.obj");



	CasaParque = Model();
	CasaParque.LoadModel("Models/CasaParque.obj");
	CasaJapones = Model();
	CasaJapones.LoadModel("Models/Japanese_Temple_Blender.obj");
	Fuente = Model();
	Fuente.LoadModel("Models/FuenteParque.obj");
	Puente = Model();
	Puente.LoadModel("Models/puente.obj");
	Columpio = Model();
	Columpio.LoadModel("Models/Columpio.obj");
	Aang = Model();
	Aang.LoadModel("Models/Aang.obj");
	Banca = Model();
	Banca.LoadModel("Models/banca_1.obj");
	Reja = Model();
	Reja.LoadModel("Models/Reja.obj");
	Entrada = Model();
	Entrada.LoadModel("Models/Entrada.obj");
	arbol = Model();
	arbol.LoadModel("Models/Arbol2.obj");
	arbusto = Model();
	arbusto.LoadModel("Models/Arbusto.obj");
	cerezo = Model();
	cerezo.LoadModel("Models/Tree 1.obj");
	Tronco = Model();
	Tronco.LoadModel("Models/tronco.obj");
	LamparaP = Model();
	LamparaP.LoadModel("Models/StreetLamp.obj");
	Base = Model();
	Base.LoadModel("Models/BaseMeditacion.obj");
	cabaña1 = Model();
	cabaña1.LoadModel("Models/Stone House.obj");
	cabaña2 = Model();
	cabaña2.LoadModel("Models/cottage.obj");
	Farola = Model();
	Farola.LoadModel("Models/Farola.obj");
	Poste = Model();
	Poste.LoadModel("Models/Poste.obj");
	LamparaFarola = Model();
	LamparaFarola.LoadModel("Models/lampara.obj");
	luciernaga = Model();
	luciernaga.LoadModel("Models/Luciernaga.obj");

	GregCuerpo = Model();
	GregCuerpo.LoadModel("Models/GregCuerpo.obj");
	GregBrazoIzq = Model();
	GregBrazoIzq.LoadModel("Models/GregBrazoIzq.obj");
	GregBrazoDer = Model();
	GregBrazoDer.LoadModel("Models/GregBrazoDer.obj");
	GregPiernaIzq = Model();
	GregPiernaIzq.LoadModel("Models/GregPiernaIzq.obj");
	GregPiernaDer = Model();
	GregPiernaDer.LoadModel("Models/GregPiernaDer.obj");


	Rio = Model();
	Rio.LoadModel("Models/rio.obj");
	ViejoMolino = Model();
	ViejoMolino.LoadModel("Models/ViejoMolino.obj");
	MolinoAgua = Model();
	MolinoAgua.LoadModel("Models/MolinoAgua.obj");
	LinternaLenador = Model();
	LinternaLenador.LoadModel("Models/LinternaLenador.obj");
	Rocas = Model();
	Rocas.LoadModel("Models/rocagira.obj");
	Rocas = Model();
	Rocas.LoadModel("Models/rocagira.obj");
	MolinoAgua = Model();
	MolinoAgua.LoadModel("Models/MolinoAgua.obj");
	LinternaLenador = Model();
	LinternaLenador.LoadModel("Models/LinternaLenador.obj");
	MordecaiCuerpo = Model();
	MordecaiCuerpo.LoadModel("Models/MordecaiCuerpo.obj");
	MordecaiBrazoDer = Model();
	MordecaiBrazoDer.LoadModel("Models/MordecaiBrazoDer.obj");
	MordecaiBrazoIzq = Model();
	MordecaiBrazoIzq.LoadModel("Models/MordecaiBrazoIzq.obj");
	Arbol_col = Model();
	Arbol_col.LoadModel("Models/arbol_columpio.obj");
	Columpio = Model();
	Columpio.LoadModel("Models/columpio.obj");
	std::vector<std::string> skyboxFacesDia;

	skyboxFacesDia.push_back("Textures/Skybox/Day-Skybox_rt.tga");
	skyboxFacesDia.push_back("Textures/Skybox/Day-Skybox_lf.tga");
	skyboxFacesDia.push_back("Textures/Skybox/Day-Skybox_dn.tga");
	skyboxFacesDia.push_back("Textures/Skybox/Day-Skybox_up.tga");
	skyboxFacesDia.push_back("Textures/Skybox/Day-Skybox_bk.tga");
	skyboxFacesDia.push_back("Textures/Skybox/Day-Skybox_ft.tga");
	

	std::vector<std::string> skyboxFacesNoche;

	skyboxFacesNoche.push_back("Textures/Skybox/Night-Skybox_rt.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/Night-Skybox_lf.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/Night-Skybox_dn.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/Night-Skybox_up.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/Night-Skybox_bk.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/Night-Skybox_ft.tga");

	skyboxDia = Skybox(skyboxFacesDia);
	skyboxNoche = Skybox(skyboxFacesNoche);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.55f, 0.62f,
		0.0f, 0.0f, -1.0f);


	//contador de luces puntuales
	unsigned int pointLightCount = 0;

	pointLights[0] = PointLight(0.5f, 0.5f, 0.5f,
		3.3f, 1.3f,
		-7.0f, 2.2f, 79.0f,
		1.0f, 0.6f, 0.0f);
	pointLightCount++;

	// Pagoda lampara
	pointLights[1] = PointLight(1.0f, 0.3f, 0.3f,
		3.5f, 1.3f,
		50.0f, 5.0f, -9.5f ,
		1.0f, 0.5f, 0.0f);
	pointLightCount++;

	pointLights[2] = PointLight(0.2f, 1.0f, 0.2f,
		5.5f, 2.3f,
		-65.0f, 8.0f, 75.0f,
		1.0f, 0.8f, 0.0f);
	pointLightCount++;
	/*
	pointLights[3] = PointLight(1.0f, 1.0f, 0.0f,
		2.5f, 3.3f,
		-75.0f, 5.0f, 70.0f,
		1.0f, 0.5f, 0.0f);
	pointLightCount++;
	*/

	unsigned int spotLightCount = 0;
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		1.0f, 0.1f,
		13.0f, 50.0f, -60.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
		1.0f, 0.1f,
		48.0f, 50.0f, 23.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	spotLights[2] = SpotLight(1.0f, 1.0f, 1.0f,
		1.0f, 0.1f,
		-75.0f, 50.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		25.0f);
	spotLightCount++;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
   		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
   	GLuint uniformColor = 0;
   	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
   	
   	//KEYFRAMES DECLARADOS INICIALES
	KeyFrame[0].levAang = 0.0f;  KeyFrame[0].levRocs = 0.0f;	KeyFrame[0].levEst = 0.0f; KeyFrame[0].inclRocs = 0.0f;      KeyFrame[0].giroRocs = 0.0;
	KeyFrame[1].levAang = 2.0f;  KeyFrame[1].levRocs = 1.5f;	KeyFrame[1].levEst = 1.5f; KeyFrame[1].inclRocs = 15.0;      KeyFrame[1].giroRocs = 20.0;
	KeyFrame[2].levAang = 4.0f;  KeyFrame[2].levRocs = 3.5f;	KeyFrame[2].levEst = 5.5f; KeyFrame[2].inclRocs = 30.0;      KeyFrame[2].giroRocs = 40.0;
	KeyFrame[4].levAang = 6.0f;  KeyFrame[3].levRocs = 5.5f;	KeyFrame[3].levEst = 5.5f; KeyFrame[3].inclRocs = 45.0;      KeyFrame[3].giroRocs = 60.0;
	KeyFrame[3].levAang = 6.0f;  KeyFrame[4].levRocs = 7.5f;	KeyFrame[4].levEst = 7.5f; KeyFrame[4].inclRocs = 30.0;      KeyFrame[4].giroRocs = 80.0;
	KeyFrame[5].levAang = 6.0f;  KeyFrame[5].levRocs = 7.5f;	KeyFrame[5].levEst = 6.5f; KeyFrame[5].inclRocs = 15.0;      KeyFrame[5].giroRocs = 100.0;
	KeyFrame[6].levAang = 6.0f;  KeyFrame[6].levRocs = 7.5f;	KeyFrame[6].levEst = 7.5f; KeyFrame[6].inclRocs = 0.0;       KeyFrame[6].giroRocs = 20.0;
	KeyFrame[7].levAang = 6.0f;  KeyFrame[7].levRocs = 7.5f;	KeyFrame[7].levEst = 6.5f; KeyFrame[7].inclRocs = -15.0;     KeyFrame[7].giroRocs = 140.0;
	KeyFrame[8].levAang = 6.0f;  KeyFrame[8].levRocs = 7.5f;	KeyFrame[8].levEst = 7.5f; KeyFrame[8].inclRocs = -30.0;     KeyFrame[8].giroRocs = 160.0;
	KeyFrame[9].levAang = 6.0f;  KeyFrame[9].levRocs = 7.5f;	KeyFrame[9].levEst = 6.5f; KeyFrame[9].inclRocs = -45.0;     KeyFrame[9].giroRocs = 180.0;
	KeyFrame[10].levAang = 6.0f; KeyFrame[10].levRocs = 7.5f;	KeyFrame[10].levEst = 7.5f; KeyFrame[10].inclRocs = -30.0;   KeyFrame[10].giroRocs = 200.0;
	KeyFrame[11].levAang = 6.0f; KeyFrame[11].levRocs = 7.5f;	KeyFrame[11].levEst = 6.5f; KeyFrame[11].inclRocs = -15.0;   KeyFrame[11].giroRocs = 220.0;
	KeyFrame[12].levAang = 6.0f; KeyFrame[12].levRocs = 7.5f;	KeyFrame[12].levEst = 7.5f; KeyFrame[12].inclRocs = 0.0;     KeyFrame[12].giroRocs = 240.0;
	KeyFrame[13].levAang = 6.0f; KeyFrame[13].levRocs = 7.5f;	KeyFrame[13].levEst = 6.5f; KeyFrame[13].inclRocs = 15.0;    KeyFrame[13].giroRocs = 260.0;
	KeyFrame[14].levAang = 6.0f; KeyFrame[14].levRocs = 7.5f;	KeyFrame[14].levEst = 7.5f; KeyFrame[14].inclRocs = 30.0;    KeyFrame[14].giroRocs = 280.0;
	KeyFrame[15].levAang = 6.0f; KeyFrame[15].levRocs = 7.5f;	KeyFrame[15].levEst = 6.5f; KeyFrame[15].inclRocs = 45.0;    KeyFrame[15].giroRocs = 300.0;
	KeyFrame[16].levAang = 6.0f; KeyFrame[16].levRocs = 5.5f;	KeyFrame[16].levEst = 5.5f; KeyFrame[16].inclRocs = 30.0;    KeyFrame[16].giroRocs = 320.0;
	KeyFrame[17].levAang = 4.0f; KeyFrame[17].levRocs = 3.5f;	KeyFrame[17].levEst = 3.5f; KeyFrame[17].inclRocs = 15.0;    KeyFrame[17].giroRocs = 340.0;
	KeyFrame[18].levAang = 2.0f; KeyFrame[18].levRocs = 1.5f;	KeyFrame[18].levEst = 1.5f; KeyFrame[18].inclRocs = 0.0f;       KeyFrame[18].giroRocs = 360.0;
	KeyFrame[19].levAang = 0.0f; KeyFrame[19].levRocs = 0.0f;	KeyFrame[19].levEst = 0.0f; KeyFrame[19].inclRocs = 0.0f;       KeyFrame[19].giroRocs = 0.0;


	//GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
	//	uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	//GLuint uniformColor = 0;
	//glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	////Inicialización de Variables de Animación
	//movVol = 0.0;
	//movVolOffset = 0.1;
	//rotVol = 0;
	//rotVolOffset = 5;
	//arriba = false;
	//
	//rotOnixOffset = 10;
	//avanzaOnix = false;
	//movXLuc = 0.0;
	//movXLucOffset = 0.1;
	//movZLuc = 0.0;
	//movZLucOffset = 0.1;
	//rotCuerLuc = 0;
	//rotLuc = 0;
	//rotLucOffset = 2;
	//avanza = false;
	/*gira = false;*/

	rotColumpio = 0.0f;
	rotColumpioOffset = 0.2f;
	BanColumpio = BanSyB = true;
	rotSyB = 0.0f;
	rotSyBOffset = 0.23f;
	incRot = 10.0f;

	glx = 1.0f;
	gly = 1.0f;
	glz = 1.0f;
	gl = 1.0f;
	
	//Variables para animación de Mordecai
	rotBrazoD = 0.0;
	rotBrazoDOffset = 1.0;
	BanRegreso = false;
	
	rotBrazoI = 0.0;
	rotBrazoIOffset = 1.0;
	BanRegreso2 = false;

	//key frames
	levAang = levEst = levRocs = 0.0f;
	inclRocs = 0.0f;
	giroRocs = 0.0f;

	// Obtención del tiempo actual
	auto tiempo_anterior = std::chrono::steady_clock::now();

	//##############################
	//Inicializar motor de Audio ###
	//##############################
	irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();
	
	if (!engine) //Se verifica si se pudo cargar el motor
		return 0;

	//variables
	irrklang::vec3df poss(0.0f, 0.0f, 0.0f);
	float dist = 0.0f;
	//#############################################################
	//#### Aquí van las declaraciones de los diferentes audios ####
	//#############################################################

	//_______**Soundtrack** _______
	irrklang::ISound* soundtrack = engine->play2D("Audios/Owns_Battle-Avatar_Soundtrack.mp3", true, false, true);
	//Soundtrack
	if (soundtrack) {
		soundtrack->setPan(0); // Se escucha al centro de las bocinas
		soundtrack->setVolume(0.0f); // Volumen del soundtrack
	}

	//______**Sonidos espaciales**______
	// Fuente de agua
	irrklang::vec3df position(-10.0f, 0.5f, -65.0f); //Posición del audio
	irrklang::ISound* s_FuenteAgua = engine->play3D("Audios/Fuente_agua_sonido.mp3", position, true, false, true);
	
	if (s_FuenteAgua) { //Configuración
		s_FuenteAgua->setMinDistance(1.0f); // Radio minímo distancia
		s_FuenteAgua->setVolume(0.2f); // Volumen del sonido ( 0 a 1)
	}

	//Tematica avatar
	irrklang::vec3df position_AvatarA(50.0f, 0.5f, 0.0f); //Posición del audio
	irrklang::ISound* temploAvatar = engine->play3D("Audios/Invacion Palacio.mp3", position_AvatarA, true, false, true);

	if (temploAvatar) { //Configuración
		temploAvatar->setMinDistance(1.0f); // Radio mínimo distancia
		temploAvatar->setVolume(1.0f); // Volumen del sonido ( 0 a 1)
	}

	irrklang::vec3df position_AvatarB(-45.0f, 0.5f, 0.0f); //Posición del audio
	irrklang::ISound* flautasAvatarB = engine->play3D("Audios/Avatar Soundtrack End.mp3", position_AvatarB, true, false, true);

	if (flautasAvatarB) { //Configuración
		flautasAvatarB->setMinDistance(1.0f); // Radio mínimo distancia
		flautasAvatarB->setVolume(0.8f); // Volumen del sonido ( 0 a 1)
	}

	//Tematica Más allá del jardín
	irrklang::vec3df position_g(7.2f, -0.7f, 70.0f); //Posición del audio
	irrklang::ISound* molino_sound = engine->play3D("Audios/Over_The_Garden_Wall.mp3", position_g, true, false, true);

	if (molino_sound) { //Configuración
		molino_sound->setMinDistance(1.0f); // Radio mínimo distancia
		molino_sound->setVolume(0.8f); // Volumen del sonido ( 0 a 1)
	}

	// Para la cámara XY
	glm::vec3 coordenadasM = glm::vec3(0.0f, 0.0f, 0.0f);
	
	
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Obtiene el tiempo transcurrido actual
		auto tiempo_actual = std::chrono::steady_clock::now();

		//Obtiene la diferencia de tiempo
		auto diferencia = std::chrono::duration_cast<std::chrono::milliseconds>
			(tiempo_actual - tiempo_anterior).count();

		//Verifica que pasen 60 segundos y cambia
		if (diferencia >= 60000) {
			dia = !dia;
			tiempo_anterior = tiempo_actual;
		}


		

		//Animacion luciernagas
		if ( mainWindow.getBanOnAnim() == true) {
			glx = 1.5 * sin(gl);
			gly = 2.0 * cos(gl);
			glz = 3.5 * cos(gl);
			gl += .01;
		}
		
		if (mainWindow.getBanOnAnim() == true) {
			rotMolino -= 0.3;
		}

		// ANIMACION SIMPLE: Columpio
		if (mainWindow.getBanOnAnim()) { // Rota
			if (rotColumpio < incRot && BanColumpio == true)
				rotColumpio += rotColumpioOffset * deltaTime;
			else if (rotColumpio > -incRot && BanColumpio == false)
				rotColumpio -= rotColumpioOffset * deltaTime;
			else {
				BanColumpio = !BanColumpio;
				if (incRot < 60.0f)
					incRot += 5.0f;
			}
		}
		else { // Se detiene y regresa al punto incial
			if (rotColumpio < -0.1f) {
				rotColumpio += rotColumpioOffset * deltaTime;
			}
			else if (rotColumpio > 0.1f) {
				rotColumpio -= rotColumpioOffset * deltaTime;
			}
			incRot = 0.0f;
		}

		//Animacion Mordecai
		if (mainWindow.getBanOnAnim()) { // Rota
			if (rotBrazoD < 50.0f && BanRegreso == false)
				rotBrazoD += rotBrazoDOffset * deltaTime;
			else if (rotBrazoD > -0.01f && BanRegreso == true)
				rotBrazoD -= rotBrazoDOffset * deltaTime;
			else {
				BanRegreso = !BanRegreso;
			}
		}

		if (mainWindow.getBanOnAnim()) { // Rota
			if (rotBrazoI < 0.10f && BanRegreso2 == false)
				rotBrazoI += rotBrazoIOffset * deltaTime;
			else if (rotBrazoI > -50.0f && BanRegreso2 == true)
				rotBrazoI -= rotBrazoIOffset * deltaTime;
			else {
				BanRegreso2 = !BanRegreso2;
			}
		}

		// Animacion Toroide
		if (mainWindow.getBanOnAnim()) {
			if (arrastreRio < 5) {
				arrastreRio += 0.09;
			}
			else if (recorrido < 84.5){
				recorrido += 0.09;
			}
			efecto = (0.3 * sin(3.14159 * 13.3 + deltaTime * 3));
		}

		//Recibir eventos del usuario
		glfwPollEvents();

		if (mainWindow.getCambioCamara() == false) {
			animPersonaje = camera.keyControlPersonaje(mainWindow.getsKeys(), deltaTime);
			camera.mouseControlPersonaje(mainWindow.getXChange(), 0.0f);
		}
		else {
			camIso.keyControl(mainWindow.getsKeys(), deltaTime);
			camIso.mouseControl(0.0f, 0.0f);
		}
		
		//para keyframes
		inputKeyframes(mainWindow.getsKeys());
		animate();
		//camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		//camIso.keyControl(mainWindow.getsKeys(), deltaTime);
		//camIso.mouseControl(0.0f, 0.0f);


		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//Cambio entre día y noche
		if (dia) {
			skyboxDia.DrawSkybox(camera.calculateViewMatrixPersonaje(), projection);
			mainLight.SetInten(0.55f, 0.62f);
			pointLightCount = 0;
		}
		else {
			skyboxNoche.DrawSkybox(camera.calculateViewMatrixPersonaje(), projection);
			mainLight.SetInten(0.2f, 0.2f);
			pointLightCount = 3;
		}

		if (mainWindow.getBanluz()) {
			spotLightCount = 3;
		}
		else {
			spotLightCount = 0;
		}
			

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		if (mainWindow.getCambioCamara() == false) {
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrixPersonaje()));
			glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
		}
		else {
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camIso.calculateViewMatrix()));
			glUniform3f(uniformEyePosition, camIso.getCameraPosition().x, camIso.getCameraPosition().y, camIso.getCameraPosition().z);
		}


		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glm::mat4 model(1.0);
		glm::mat4 modelLuc(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 modelSyB(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);//Se declara al inicio para que las texturas no se muevan

		glm::vec3 positionL = camera.getCameraPosition();
		glm::vec3 directionL = camera.getCameraDirection();

		//################################################//
		//####  Posición del personaje para el audio  ####//
		//################################################//

		irrklang::vec3df position(positionL.x, positionL.y, positionL.z); // position of the listener
		irrklang::vec3df lookDirection(-directionL.x, -directionL.y, -directionL.z);// the direction the listener looks into
		irrklang::vec3df velPerSecond(0.0f, 0.0f, 0.0f);    // only relevant for doppler effects
		irrklang::vec3df upVector(0.0f, 1.0f, 0.0f);     // where 'up' is in your 3D scene

		engine->setListenerPosition(position, lookDirection, velPerSecond, upVector);

		//Lógica de audio0
		if (soundtrack) {
			if (soundtrack->getVolume() > 0.025f || soundtrack->getVolume() < -0.001f)
				soundtrack->setVolume(soundtrack->getVolume() - soundtrack->getVolume());

			if (temploAvatar->getIsPaused() && flautasAvatarB->getIsPaused() && molino_sound->getIsPaused()) {
				if (soundtrack->getVolume() <= 0.015f)
					soundtrack->setVolume(soundtrack->getVolume() + 0.0005f * deltaTime);
			}
			else {
				if (soundtrack->getVolume() >= 0.0f)
					soundtrack->setVolume(soundtrack->getVolume() - 0.0005f * deltaTime);
			}
		}
		
		//Lógica de audio1
		if (temploAvatar) {
			poss = temploAvatar->getPosition();
			dist = sqrt(pow(poss.X - positionL.x, 2) + pow(poss.Y - positionL.y, 2) + pow(poss.Z - positionL.z, 2));
			if (dist > 60) {
				temploAvatar->setIsPaused(true);
			}
			else {
				temploAvatar->setIsPaused(false);
			}
		}

		//Lógica de audio2
		if (flautasAvatarB) {
			poss = flautasAvatarB->getPosition();
			dist = sqrt(pow(poss.X - positionL.x, 2) + pow(poss.Y - positionL.y, 2) + pow(poss.Z - positionL.z, 2));
			if (dist > 60) {
				flautasAvatarB->setIsPaused(true);
			}
			else {
				flautasAvatarB->setIsPaused(false);
			}
		}
		
		//Lógica de audio3
		if (molino_sound) {
			poss = molino_sound->getPosition();
			dist = sqrt(pow(poss.X - positionL.x, 2) + pow(poss.Y - positionL.y, 2) + pow(poss.Z - positionL.z, 2));
			if (dist > 60) {
				molino_sound->setIsPaused(true);
			}
			else {
				molino_sound->setIsPaused(false);
			}
		}
		//############
		//##  Piso  ##
		//############
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
		model = glm::scale(model, glm::vec3(13.0f, 1.0f, 14.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);

		//############################
		//### Creacion del toroide ###
		//############################
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(32.0f+arrastreRio+efecto, 0.3f+efecto, -51.0f+recorrido));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		toroideTexture.UseTexture();
		meshList[6]->RenderMesh();
		
		//Arboles
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-115.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arboles.RenderModel();

		//Arboles
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(120.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arboles.RenderModel();

		//Arboles
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f, 0.0f, 115.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arboles.RenderModel();

		//Arboles
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(12.0f, 0.0f, -140.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arboles.RenderModel();

		//Farola1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(15.0f , 0.0f, -60.0f ));
		model = glm::scale(model, glm::vec3(2.0f, 5.0f, 2.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Farola.RenderModel();

		//Farola2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(48.0f , 0.0f, 25.0f ));
		model = glm::scale(model, glm::vec3(2.0f, 5.0f, 2.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Farola.RenderModel();

		//Farola3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-77.0f , 0.0f, 0.0f ));
		model = glm::scale(model, glm::vec3(2.0f, 5.0f, 2.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Farola.RenderModel();

		//Farola Pagoda
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f , 0.0f, -10.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LamparaFarola.RenderModel();

		
		//**************
		//Columpio//
		//*************

		//Arbol colinpio
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-55.0f, 0.0f, 90.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 5.0f, 2.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arbol_col.RenderModel();


		//Arbol colinpio
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-70.0f, 0.0f, 70.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 5.0f, 2.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arbol_col.RenderModel();

		//Arbol colinpio
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-85.0f, 0.0f, 90.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 5.0f, 2.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arbol_col.RenderModel();


		//Arbol colinpio
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, 0.0f, 70.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 5.0f, 2.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arbol_col.RenderModel();

		//Arbol colinpio
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-115.0f, 0.0f, 90.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 5.0f, 2.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arbol_col.RenderModel();


		//Arbol colinpio
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-60.0f, 0.0f, 70.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 5.0f, 2.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arbol_col.RenderModel();





		//Arbol colinpio
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, 70.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 5.0f, 2.0f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arbol_col.RenderModel();

		//columpio
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-43.5f, 11.0f, 70.0f));
		model = glm::scale(model, glm::vec3(0.75f, 1.5f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotColumpio * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Columpio.RenderModel();



		//##########################//
		//####  Base Meditacion ####//
		//##########################//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-45.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Base.RenderModel();



		//##########################//
		//####       Aang       ####//
		//##########################//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-45.0f, 3.0f + levAang, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Aang.RenderModel();


		//##########################//
		//####      rocas       ####//
		//##########################//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-45.0f, 3.0f + levRocs, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, giroRocs * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, inclRocs * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Rocas.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-45.0f, 3.0f + levRocs, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, giroRocs * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, inclRocs * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Rocas.RenderModel();



		//##########################//
		//####   Templo chino	####//
		//##########################//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(80.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		modelaux = model;
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CasaJapones.RenderModel();

		//##########################//
		//#### Cabaña de madera ####//
		//##########################//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(70.0f, 0.0f, 85.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		modelaux = model;
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cabaña1.RenderModel();

		//##########################//
		//#### Cabaña de madera ####//
		//##########################//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 94.5f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		modelaux = model;
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//cabaña2.RenderModel();


		//##########################//
		//####   Luciernagas    ####//
		//##########################//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-70.0f+glx, 5.0f+gly, 75.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		modelaux = model;
		model = glm::rotate(model, 25 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		luciernaga.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-70.0f , 5.0f + gly, 67.0f + glx));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		modelaux = model;
		model = glm::rotate(model, 35 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		luciernaga.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-65.0f + glx, 8.0f , 80.0f + glz));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		modelaux = model;
		model = glm::rotate(model, 25 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		luciernaga.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-60.0f + gly, 3.0f + glz, 70.0f + glx));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		modelaux = model;
		model = glm::rotate(model, 35 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		luciernaga.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-72.0f + glz, 7.0f + glx, 77.0f + gly));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		modelaux = model;
		model = glm::rotate(model, 55 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		luciernaga.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-71.0f + gly, 2.5f + glz, 69.0f + glx));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		modelaux = model;
		model = glm::rotate(model, 55 * toRadians, glm::vec3(1.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		luciernaga.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-69.0f , 5.5f + glz, 78.0f + glz));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		modelaux = model;
		model = glm::rotate(model, 45 * toRadians, glm::vec3(1.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		luciernaga.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-75.0f , 3.5f + gly, 68.0f + glx));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		modelaux = model;
		model = glm::rotate(model, 75 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		luciernaga.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-65.0f + glz, 7.0f + glx, 77.0f + gly));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		modelaux = model;
		model = glm::rotate(model, 55 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		luciernaga.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-64.0f + gly, 2.5f + glz, 69.0f + glx));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		modelaux = model;
		model = glm::rotate(model, 55 * toRadians, glm::vec3(1.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		luciernaga.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-58.0f, 5.5f + glz, 78.0f + glz));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		modelaux = model;
		model = glm::rotate(model, 45 * toRadians, glm::vec3(1.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		luciernaga.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-67.0f, 3.5f + gly, 68.0f + glx));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		modelaux = model;
		model = glm::rotate(model, 75 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		luciernaga.RenderModel();


		//##########################//
		//####      Puente      ####//
		//##########################//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(37.0f,0.4f, 40.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Puente.RenderModel();

		//##########################//
		//#### Casa del Parque  ####//
		//##########################//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 0.0f, -95.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CasaParque.RenderModel();


		//##########################//
		//#### Fuente del Parque####//
		//##########################//
		model = modelaux;
		model = glm::translate(model, glm::vec3(-5.0f, -0.2f, 30.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Fuente.RenderModel();

		
		//##########################//
		//#### Rejas del parque ####//
		//##########################// Cada barra mide 0.188f
		model = modelaux;
		model = glm::translate(model, glm::vec3(34.0f, 0.0f, 40.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.5f, 2.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.0f + 0.188f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.0f + 0.188f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.0f + 0.188f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -6.0f + 0.376f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.0f + 0.188f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.0f + 0.188f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.0f + 0.188f));
		//model = glm::rotate(model, 90* toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Entrada.RenderModel();


		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -6.0f + 0.376f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();

		//######################//
		//#### Flora		####//
		//######################//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-23.0f , 0.0f, -70.0f  ));
		model = glm::scale(model, glm::vec3(1.0f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f , 0.0f, -70.0f + mainWindow.getposy()));
		model = glm::scale(model, glm::vec3(1.0f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.0f , 0.0f, -105.0f ));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-65.0f , 0.0f, -100.0f ));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-70.0f , 0.0f, -90.0f ));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-75.0f , 0.0f, -120.0f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-45.0f , 0.0f, -100.0f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-45.0f , 0.0f, -80.0f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-55.0f , 0.0f, -115.0f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-75.0f , 0.0f, -120.0f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-45.0f , 0.0f, -100.0f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-45.0f , 0.0f, -80.0f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-85.0f , 0.0f, -115.0f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-93.0f , 0.0f, -125.0f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-73.0f , 0.0f, -90.0f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f , 0.0f, -110.0f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-90.0f , 0.0f, -85.0f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-70.0f , 0.0f, -80.0f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f , 0.0f, -75.0f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		//--------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f , 0.0f, -105.0f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(65.0f , 0.0f, -100.0f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(70.0f , 0.0f, -90.0f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(75.0f , 0.0f, -120.0f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(45.0f , 0.0f, -100.0f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(45.0f , 0.0f, -80.0f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(55.0f , 0.0f, -115.0f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(75.0f , 0.0f, -120.0f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(45.0f , 0.0f, -100.0f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(45.0f , 0.0f, -80.0f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(85.0f , 0.0f, -115.0f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();



		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(93.0f , 0.0f, -125.0f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(73.0f , 0.0f, -90.0f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f , 0.0f, -110.0f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(90.0f , 0.0f, -85.0f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(70.0f , 0.0f, -80.0f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f , 0.0f, -75.0f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		//######################//
		//#### Cerezos		####//
		//######################//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(80.0f , 0.0f, 30.0f ));
		model = glm::scale(model, glm::vec3(2.5f, 3.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cerezo.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(80.0f , 0.0f, -30.0f ));
		model = glm::scale(model, glm::vec3(2.5f, 3.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cerezo.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(60.0f, 0.0f, 30.0f));
		model = glm::scale(model, glm::vec3(2.5f, 3.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cerezo.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(60.0f, 0.0f, -30.0f));
		model = glm::scale(model, glm::vec3(2.5f, 3.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cerezo.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(70.0f, 0.0f, 45.0f));
		model = glm::scale(model, glm::vec3(2.5f, 3.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cerezo.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(70.0f, 0.0f, -45.0f));
		model = glm::scale(model, glm::vec3(2.5f, 3.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cerezo.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(80.0f, 0.0f, 60.0f));
		model = glm::scale(model, glm::vec3(2.5f, 3.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cerezo.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(80.0f, 0.0f, -60.0f));
		model = glm::scale(model, glm::vec3(2.5f, 3.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cerezo.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(60.0f, 0.0f, 60.0f));
		model = glm::scale(model, glm::vec3(2.5f, 3.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cerezo.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(60.0f, 0.0f, -60.0f));
		model = glm::scale(model, glm::vec3(2.5f, 3.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cerezo.RenderModel();



		//----------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-14.5f , 0.0f, 30.5f ));
		model = glm::scale(model, glm::vec3(2.5f, 3.0f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cerezo.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-14.5f , 0.0f, -30.5f ));
		model = glm::scale(model, glm::vec3(2.5f, 3.0f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cerezo.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-76.0f , 0.0f, 30.5f ));
		model = glm::scale(model, glm::vec3(2.5f, 3.0f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cerezo.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-76.0f , 0.0f, -30.5f ));
		model = glm::scale(model, glm::vec3(2.5f, 3.0f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cerezo.RenderModel();


		//##########################//
		//####      Bancas      ####//
		//##########################//
		//1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-16.5f , 2.5f , -10.0f ));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca.RenderModel();

		//2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-16.5f , 2.5f, 10.0f ));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca.RenderModel();

		//3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-74.0f , 2.5f, 10.0f ));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca.RenderModel();

		//4
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-74.0f , 2.5f, -10.0f ));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca.RenderModel();

		//5
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-55.25f , 2.5f, 28.75 ));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca.RenderModel();

		//6
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-35.25f , 2.5f, 28.75f ));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca.RenderModel();

		//7
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-35.25f , 2.5f, -28.75 ));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca.RenderModel();

		//8
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-55.25f , 2.5f, -28.75f ));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca.RenderModel();

		//#####################################//
		//#### Greg - Personaje Principal #####//
		//#####################################//

		glm::vec3 posicionPersonaje = glm::vec3(camera.getCameraPosition().x, 2.2f, -9.0f + 9.0f + camera.getCameraPosition().z);

		model = glm::mat4(1.0);

		model = glm::translate(model, glm::vec3(camera.getCameraPosition().x, 2.2f, camera.getCameraPosition().z));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -camera.rotCamara() * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GregCuerpo.RenderModel();

		if (animPersonaje) {
			if (!dirBrazosPiernas) {
				rotBrazoDerPiernaIzq += 2.0;
				rotBrazoIzqPiernaDer -= 2.0;

				if (rotBrazoDerPiernaIzq > 60) {
					dirBrazosPiernas = !dirBrazosPiernas;
				}
			}
			else if (dirBrazosPiernas) {
				rotBrazoDerPiernaIzq -= 2.0;
				rotBrazoIzqPiernaDer += 2.0;

				if (rotBrazoDerPiernaIzq < -60) {
					dirBrazosPiernas = !dirBrazosPiernas;
				}
			}
		}

		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.35f, -0.4f, -0.09f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, rotBrazoDerPiernaIzq * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GregBrazoDer.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.35f, -0.4f, -0.09f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, rotBrazoIzqPiernaDer * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GregBrazoIzq.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.387f, -1.5f, 0.128f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, rotBrazoDerPiernaIzq * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GregPiernaIzq.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.387f, -1.5f, 0.128f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, rotBrazoIzqPiernaDer * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GregPiernaDer.RenderModel();


		//##########################//
		//####    Viejo Molino  ####//
		//##########################//

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(7.2f, -0.7f, 60.0f));
		model = glm::scale(model, glm::vec3(3.7f, 3.7f, 3.7f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ViejoMolino.RenderModel();

		//##########################//
		//####    Molino Agua   ####//
		//##########################//

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(36.2f, 11.5f, 87.0f));
		model = glm::scale(model, glm::vec3(2.5f, 3.0f, 3.0f));
		model = glm::rotate(model, rotMolino * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MolinoAgua.RenderModel();

		//##########################//
		//####      Tronco      ####//
		//##########################//

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-7.0f, -0.5f, 79.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 111 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tronco.RenderModel();


		//##########################//
		//#### Linterna Leñador ####//
		//##########################//

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-7.1f, 2.0f, 79.1f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, 80.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LinternaLenador.RenderModel();

		//##########################//
		//####    Mordecai      ####//
		//##########################//

		//Cuerpo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -65.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		modelaux = model;
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MordecaiCuerpo.RenderModel();
		
		//Brazo izquierdo
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.035f, 1.134f, -0.038f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotBrazoI * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MordecaiBrazoIzq.RenderModel();

		//Brazo derecho
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.124f, 1.148f, -0.044f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotBrazoD * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MordecaiBrazoDer.RenderModel();
		
		//#################
		//###  Estatuas ###		
		//#################
		
		//Estatua 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-60.0f, 10.5f, -15.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 0.0f, 17.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);//Para indicar trasparencia y traslucidez
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//Va antes de la textura
		estatua1Texture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();


		//Estatua 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, 10.5f, -15.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 0.0f, 17.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		estatua2Texture.UseTexture();
		meshList[5]->RenderMesh();

		//Estatua 3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-60.0f, 10.5f, 15.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 0.0f, 17.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		estatua3Texture.UseTexture();
		meshList[5]->RenderMesh();

		//Estatua 4
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, 10.5f, 15.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 0.0f, 17.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		estatua4Texture.UseTexture();
		meshList[5]->RenderMesh();

		//textura con movimiento
		toffsetu += (0.0005 * sin(-3.14159 * 1.7 + deltaTime * 3));
		toffsetv += 0.0005 * deltaTime;
		if (toffsetu > 1.0)
			toffsetu = 0.0;
		toffset = glm::vec2(toffsetu, toffsetv);

		//##########################//
		//####         Rio      ####//
		//##########################//

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, -0.21f, -10.0f));
		model = glm::scale(model, glm::vec3(1.3f, 1.4f, 11.8f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rio.RenderModel();

		glDisable(GL_BLEND);//Desactiva el blender

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	engine->drop(); //Eliminar audios
	return 0;
}

void inputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion < 1)
		{
			if (play == false && (FrameIndex > 1))
			{
				resetElements();
				//First Interpolation				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				printf("\n *--- Presiona R para habilitar reproducir de nuevo la animación ---*\n");
				habilitaranimacion = 0;

			}
			else
			{
				play = false;
			}
		}
	}
	if (keys[GLFW_KEY_R])
	{
		if (habilitaranimacion < 1)
		{
			reproduciranimacion = 0;
		}
	}
}

//Fin



