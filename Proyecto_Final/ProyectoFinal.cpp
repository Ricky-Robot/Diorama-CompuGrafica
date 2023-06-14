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
float movVol;
float movVolOffset;
int rotVol;
int rotVolOffset;
bool arriba;
float movOnix;
float movOnixOffset;
int rotOnix;
int rotOnixOffset;
bool avanzaOnix;
float movXLuc;
float movXLucOffset;
float movZLuc;
float movZLucOffset;
int rotCuerLuc;
int rotLuc;
int rotLucOffset;
bool avanza;
bool gira;

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

//key frames
float levAang;
float levRocs, inclRocs, giroRocs;
float levEst;

float rotMolino = 0;

bool animPersonaje;

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
Texture AgaveTexture;
Texture humo;



Model Arboles;
Model CasaParque;
Model Fuente;
Model cabaña1;
Model cabaña2;
Model Reja;
Model Columpio;
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

//Model Casa2;
Model Personaje2;
Model arbol;
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



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	
	CreateObjects();
	CreateToroide();
	CreateShaders();

	camera = Camera(glm::vec3(-120.0f, 4.0f, 80.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 0.2f, 0.2f);//Ligada al planoXZ
	//camIso = Camera(glm::vec3(-150.0f, 150.0f, 150.0f), glm::vec3(0.0f, 1.0f, 0.0f), -45.0f, -45.0f, 0.5f, 0.5f);//Isometrica
	
	//Original
	//camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	toroideTexture = Texture("Textures/toroide.jpg");
	toroideTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	humo = Texture("Textures/humo.tga");
	humo.LoadTextureA();


	//Pueblo = Model();
	//Pueblo.LoadModel("Models/PuebloCentro.obj");
	Arboles = Model();
	Arboles.LoadModel("Models/Arboles.obj");
	//Voltorb = Model();
	//Voltorb.LoadModel("Models/Voltorb.obj");
	//Electrode = Model();
	//Electrode.LoadModel("Models/Electrode.obj");
	//Onix = Model();
	//Onix.LoadModel("Models/Onix.obj");


	/*
	//Avatar
	LucCuerpo = Model();
	LucCuerpo.LoadModel("Models/LucarioCuerpo.obj");
	LucCabeza = Model();
	LucCabeza.LoadModel("Models/LucarioCabeza.obj");
	LucCola = Model();
	LucCola.LoadModel("Models/LucarioCola.obj");
	LucBraDer = Model();
	LucBraDer.LoadModel("Models/LucarioBraDer.obj");
	LucBraIzq = Model();
	LucBraIzq.LoadModel("Models/LucarioBraIzq.obj");
	LucPierDer = Model();
	LucPierDer.LoadModel("Models/LucarioPierDer.obj");
	LucPierIzq = Model();
	LucPierIzq.LoadModel("Models/LucarioPierIzq.obj");
	*/


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
	Personaje2 = Model();
	Personaje2.LoadModel("Models/Mordecai.obj");
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
	
	//Inicialización de Variables de Animación
	movVol = 0.0;
	movVolOffset = 0.1;
	rotVol = 0;
	rotVolOffset = 5;
	arriba = false;
	movOnix = 90.0f;
	movOnixOffset = 0.5f;
	rotOnix = 0;
	rotOnixOffset = 10;
	avanzaOnix = false;
	movXLuc = 0.0;
	movXLucOffset = 0.1;
	movZLuc = 0.0;
	movZLucOffset = 0.1;
	rotCuerLuc = 0;
	rotLuc = 0;
	rotLucOffset = 2;
	avanza = false;
	gira = false;

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

	//#############################################################
	//#### Aquí van las declaraciones de los diferentes audios ####
	//#############################################################

	//_______**Soundtrack** _______
	irrklang::ISound* soundtrack = engine->play2D("Audios/Owns_Battle-Avatar_Soundtrack.mp3", true, false, true);
	//Radio de sonido
	//Soundtrack
	if (soundtrack) {
		soundtrack->setPan(0); // Se escucha al centro de las bocinas
		soundtrack->setVolume(0.05f); // Volumen del soundtrack
	}

	//______**Sonidos espaciales**______
	// Fuente de agua
	irrklang::vec3df position(-10.0f, 0.5f, -65.0f); //Posicion del audio
	irrklang::ISound* s_FuenteAgua = engine->play3D("Audios/Fuente_agua_sonido.mp3", position, true, false, true);
	
	if (s_FuenteAgua) { //Configuración
		s_FuenteAgua->setMinDistance(2.0f); // Radio minimo distancia
		s_FuenteAgua->setVolume(1.0f); // Volumen del sonido ( 0 a 1)
	}

	//Tematica avatar
	irrklang::vec3df position_AvatarA(80.0f, 0.5f, 0.0f); //Posicion del audio
	irrklang::ISound* temploAvatar = engine->play3D("Audios/Invacion Palacio.mp3", position_AvatarA, true, false, true);

	if (temploAvatar) { //Configuración
		temploAvatar->setMinDistance(16.0f); // Radio minimo distancia
		temploAvatar->setVolume(1.0f); // Volumen del sonido ( 0 a 1)
	}

	irrklang::vec3df position_AvatarB(-45.0f, 0.5f, 0.0f); //Posicion del audio
	irrklang::ISound* flautasAvatarB = engine->play3D("Audios/Avatar Soundtrack End.mp3", position_AvatarB, true, false, true);

	if (flautasAvatarB) { //Configuración
		flautasAvatarB->setMinDistance(11.0f); // Radio minimo distancia
		flautasAvatarB->setVolume(1.0f); // Volumen del sonido ( 0 a 1)
	}

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

		//Animación Avatar
		if (rotLuc < 20 && gira == false && mainWindow.getBanOnAnim() == true) {
			rotLuc += rotLucOffset * deltaTime;
			if (rotLuc < 21 && rotLuc > 19) {
				gira = true;
			}
		}
		else if (rotLuc > -20 && gira == true && mainWindow.getBanOnAnim() == true) {
			rotLuc -= rotLucOffset * deltaTime;
			if (rotLuc < -19 && rotLuc > -21) {
				gira = false;
			}
		}


		if (movXLuc < 100.5f && avanza == false && mainWindow.getBanOnAnim() == true) {
			movXLuc += movXLucOffset * deltaTime;
			if (movXLuc < 101.0f && movXLuc > 100.0f ) {
				rotCuerLuc = 1;
				avanza = true;
			}
		}
		else if (movZLuc < 120.5f && rotCuerLuc == 1 && mainWindow.getBanOnAnim() == true) {
			movZLuc += movVolOffset * deltaTime;
			if (movZLuc < 121.0f && movZLuc > 120.0f) {
				rotCuerLuc = 2;
			}
		}
		else if (movXLuc > 0.0f && rotCuerLuc == 2 && mainWindow.getBanOnAnim() == true) {
			movXLuc -= movXLucOffset * deltaTime;
			if (movXLuc < 0.5f && movXLuc > -0.5f) {
				rotCuerLuc = 3;
			}
		}
		else if (movZLuc > 0.0f && rotCuerLuc == 3 && mainWindow.getBanOnAnim() == true) {
			movZLuc -= movVolOffset * deltaTime;
			if (movZLuc < 0.5f && movZLuc > -0.5f) {
				rotCuerLuc = 0;
				avanza = false;
			}
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

		// ANIMACION SIMPLE: Sube y Baja
		if (mainWindow.getBanOnAnim()) { // Rota
			if (rotSyB < 26.0f && BanSyB == true)
				rotSyB += rotSyBOffset * deltaTime;
			else if (rotSyB > 0.0f && BanSyB == false)
				rotSyB -= rotSyBOffset * deltaTime;
			else {
				BanSyB = !BanSyB;
			}
		}
		else { // Se detiene y regresa al punto incial
			if (rotSyB < -0.1f) {
				rotSyB += rotColumpioOffset * deltaTime;
			}
			else if (rotColumpio > 0.1f) {
				rotSyB -= rotColumpioOffset * deltaTime;
			}
		}


		//Recibir eventos del usuario
		glfwPollEvents();
		animPersonaje = camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), 0.0f);
		//camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		//camIso.keyControl(mainWindow.getsKeys(), deltaTime);
		//camIso.mouseControl(0.0f, 0.0f);

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//skyboxDia.DrawSkybox(camera.calculateViewMatrix(), projection);
		//skyboxDia.DrawSkybox(camIso.calculateViewMatrix(), projection);

		//Cambio entre día y noche
		if (dia) {
			skyboxDia.DrawSkybox(camera.calculateViewMatrix(), projection);
			mainLight.SetInten(0.55f, 0.62f);
			pointLightCount = 0;
		}
		else {
			skyboxNoche.DrawSkybox(camera.calculateViewMatrix(), projection);
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

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camIso.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
		//glUniform3f(uniformEyePosition, camIso.getCameraPosition().x, camIso.getCameraPosition().y, camIso.getCameraPosition().z);


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
		model = glm::translate(model, glm::vec3(32.0f, 0.3f, -51.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		toroideTexture.UseTexture();
		meshList[5]->RenderMesh();
		
		//Arboles
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-115.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Arboles.RenderModel();

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
		//Arboles.RenderModel();

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
		

		//##########################//
		//####	  Mordecai      ####//
		//##########################//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -65.0f));
		
		model = glm::scale(model, glm::vec3(1.25f, 1.25f, 1.25f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Personaje2.RenderModel();

		
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


		//######################//
		//#### Arbustos		####//
		//######################//

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-115.0f + mainWindow.getposx(), 0.0f, 0.0f + mainWindow.getposy()));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//arbusto.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-115.0f + mainWindow.getposx(), 0.0f, 0.0f + mainWindow.getposy()));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//arbusto.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-115.0f + mainWindow.getposx(), 0.0f, 0.0f + mainWindow.getposy()));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//arbusto.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-115.0f + mainWindow.getposx(), 0.0f, 0.0f + mainWindow.getposy()));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//arbusto.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-115.0f + mainWindow.getposx(), 0.0f, 0.0f + mainWindow.getposy()));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//arbusto.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-115.0f + mainWindow.getposx(), 0.0f, 0.0f + mainWindow.getposy()));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//arbusto.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-115.0f + mainWindow.getposx(), 0.0f, 0.0f + mainWindow.getposy()));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//arbusto.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-115.0f + mainWindow.getposx(), 0.0f, 0.0f + mainWindow.getposy()));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//arbusto.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-115.0f + mainWindow.getposx(), 0.0f, 0.0f + mainWindow.getposy()));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//arbusto.RenderModel();

		//##########################//
		//#### Lamapara Parque  ####//
		//##########################//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-115.0f + mainWindow.getposx(), 0.0f, 0.0f + mainWindow.getposy()));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//LamparaP.RenderModel();

		//2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-115.0f + mainWindow.getposx(), 0.0f, 0.0f + mainWindow.getposy()));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//LamparaP.RenderModel();

		//3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-115.0f + mainWindow.getposx(), 0.0f, 0.0f + mainWindow.getposy()));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//LamparaP.RenderModel();

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

		//##########################//
		//#### Greg - Personaje Principal ####//
		//##########################//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(camera.getCameraPosition().x, 2.2f, 9.0f + camera.getCameraPosition().z));
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

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.35f + camera.getCameraPosition().x, 1.8f, 8.91f + camera.getCameraPosition().z));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, rotBrazoDerPiernaIzq * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GregBrazoDer.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.35f + camera.getCameraPosition().x, 1.8f, 8.91f + camera.getCameraPosition().z));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, rotBrazoIzqPiernaDer * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GregBrazoIzq.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.387f + camera.getCameraPosition().x, 0.7f, 9.128f + camera.getCameraPosition().z));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, rotBrazoDerPiernaIzq * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GregPiernaIzq.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.387f + camera.getCameraPosition().x, 0.7f, 9.128f + camera.getCameraPosition().z));
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



		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);//Para indicar trasparencia y traslucidez
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//Va antes de la textura

		//textura con movimiento del humo
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

	return 0;
}



