// gerador_de_terreno.cpp

// Este programa mostra como criar um terreno usando uma "fita de triângulos",
// um tipo de malha definida no "framework" VART

// Autor - Márcio Inácio Santana

#include <vart/scene.h>
#include <vart/light.h>
#include <vart/arrow.h>
#include <vart/meshobject.h>
#include <vart/mesh.h>
#include <vart/contrib/viewerglutogl.h>

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

//Definindo a quantidade de linhas e colunas da matriz de pontos
#define NUM_LINHAS 20
#define NUM_COLUNAS 20

using namespace std;
using namespace VART;

int main(int argc, char* argv[]){
	ViewerGlutOGL::Init(&argc, argv); //Inicializa a GLUT
	static Scene scene; //Cria uma cena
	static ViewerGlutOGL viewer; //Cria um viewer (Janela de aplicação)
	
	//Cria uma câmera (Observador da cena)
	//camera(centro_de_projecao, aponta_para, para_cima)
	Camera camera(Point4D(30, 10, 10), Point4D(0, 10, 0), Point4D(0, 0, 1));
	
	//Cria o MeshObject que representará o terreno
	MeshObject terreno;
	
	//Para gerar coordenadas z "aleatórias" adiante
	srand((int) time(0));
	
	//Definindo os vértices do terreno
	vector<Point4D> vertices;
	for(int lin = 0; lin < NUM_LINHAS; ++lin){
		for(int col = 0; col < NUM_COLUNAS; ++col){
			//Gera um número aletório entre zero e um, isso garate que 
			//uma rampa terá, no máximo, 45% como valor absoluto de inclinação
			double z = double(rand())/RAND_MAX;
			vertices.push_back(Point4D(lin, col, z));
		}
	}
	terreno.SetVertices(vertices);
	
	//Definindo malhas de fitas de triângulos para constituir o terreno	
	for (int lin = 0; lin < NUM_LINHAS - 1; lin++){
		Mesh fita_de_triangulos;
		fita_de_triangulos.type = Mesh::MeshType::TRIANGLE_STRIP;
		fita_de_triangulos.indexVec.reserve(NUM_COLUNAS);
		for(int col = 0; col < NUM_COLUNAS; col++){
			fita_de_triangulos.indexVec.push_back(lin*NUM_COLUNAS + col);
			fita_de_triangulos.indexVec.push_back((lin + 1)*NUM_COLUNAS + col);
		}
		terreno.AddMesh(fita_de_triangulos);
	}
	
	//Calculando as normais das faces
	terreno.ComputeVertexNormals();
	
	//Dando um material para o terreno
	terreno.SetMaterial(Material::PLASTIC_GREEN());
	
	//Adicionando o terreno à cena
	scene.AddObject(&terreno);
	
	//Adiciona luzes e câmera à cena
	scene.AddLight(Light::BRIGHT_AMBIENT());
	scene.AddCamera(&camera);
	
	//Configura a janela de exibição
	viewer.SetTitle("Gerador de terrenos");
	viewer.SetScene(scene);
	
	//Executa a aplicação
	scene.DrawLightsOGL(); //Configura o estado das luzes do OpenGL
	ViewerGlutOGL::MainLoop(); //Entra no loop main(loop de eventos) e nunca retorna
	return 0;
}
