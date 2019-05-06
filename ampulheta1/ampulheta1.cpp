// ampulheta1.cpp

// Este programa desenha uma ampulheta

// Autor - Márcio Inácio Santana

#include <vart/scene.h>
#include <vart/light.h>
#include <vart/arrow.h>
#include <vart/meshobject.h>
#include <vart/contrib/viewerglutogl.h>

#include <iostream>
#include <string.h>
#include <vector>

using namespace std;
using namespace VART;

int main(int argc, char* argv[]){
	ViewerGlutOGL::Init(&argc, argv); //Inicializa a GLUT
	static Scene scene; //Cria uma cena
	static ViewerGlutOGL viewer; //Cria uma viewer(Janela de aplicação)
	
	//Cria uma câmera (Observador da cena)
	Camera camera(Point4D(0, 0, 4), Point4D(0, 0, 0), Point4D(0, 1, 0));
	
	
	//Cria o MeshObject que representará a Pirâmide
	MeshObject piramide;
	
	//Cria os vértices da pirâmide
	piramide.SetVertices("0 0 0, 1 0 0, 1 1 0, 0 1 0, 0.5 0.5 1, 0 0 2, 1 0 2, 1 1 2, 0 1 2");
	piramide.ComputeVertexNormals();
	
	//Cria as faces da pirâmide
	piramide.AddFace("0 3 2 1"); //Face "de baixo"	
	piramide.AddFace("0 1 4"); //Face inferior "esquerda"
	piramide.AddFace("1 2 4"); //Face inferior "frontal"
	piramide.AddFace("2 3 4"); //Face inferior "direita"
	piramide.AddFace("0 4 3"); //Face inferior "traseira"
	piramide.AddFace("4 6 5"); //Face superior "esquerda"
	piramide.AddFace("4 7 6"); //Face superior "frontal"
	piramide.AddFace("4 8 7"); //Face superior "direita"
	piramide.AddFace("4 5 8"); //Face superior "traseira"
	piramide.AddFace("5 6 7 8"); //Face "de cima"
	
	//Dá um meteria à pirâmide
	piramide.SetMaterial(Material::PLASTIC_BLUE());
	
	//Adiciona a pirâmide à cena
	scene.AddObject(&piramide);
	
	//Adiciona Luz e câmera à cena
	scene.AddLight(Light::BRIGHT_AMBIENT());
	scene.AddCamera(&camera);
	
	//Configura a janela de exibição
	viewer.SetTitle("Ampulheta - V1");
	viewer.SetScene(scene);
	
	//Executa a aplicação
	scene.DrawLightsOGL(); //Configura o estado das luzes do OpenGL
	ViewerGlutOGL::MainLoop(); //Entra no loop main(loop de eventos) e nunca retorna
}
