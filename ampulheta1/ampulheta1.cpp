// ampulheta1.cpp

// Este programa desenha uma ampulheta

// Autor - Márcio Inácio Santana

#include <vart/scene.h>
#include <vart/light.h>
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
	Camera camera(Point4D(0.5, 4, 1), Point4D(0.5, 0, 1), Point4D(0, 0, 1));
	
	
	//Cria o MeshObject que representará a ampulheta
	MeshObject ampulheta;
	
	vector<Point4D>vertices = {Point4D(0, 0, 0), Point4D(1, 0, 0), Point4D(1, 1, 0), Point4D(0, 1, 0),
		Point4D(0.5, 0.5, 1), Point4D(0, 0, 2), Point4D(1, 0, 2), Point4D(1, 1, 2), Point4D(0, 1, 2)};
	//vertices = "0 0 0, 1 0 0, 1 1 0, 0 1 0, 0.5 0.5 1, 0 0 2, 1 0 2, 1 1 2, 0 1 2"
	
	//Cria os vértices da ampulheta
	ampulheta.SetVertices(vertices);
	
	//Cria as faces da ampulheta
	ampulheta.AddFace("0 3 2 1"); //Face "de baixo"	
	ampulheta.AddFace("0 1 4"); //Face inferior "esquerda"
	ampulheta.AddFace("1 2 4"); //Face inferior "frontal"
	ampulheta.AddFace("2 3 4"); //Face inferior "direita"
	ampulheta.AddFace("0 4 3"); //Face inferior "traseira"
	ampulheta.AddFace("4 6 5"); //Face superior "esquerda"
	ampulheta.AddFace("4 7 6"); //Face superior "frontal"
	ampulheta.AddFace("4 8 7"); //Face superior "direita"
	ampulheta.AddFace("4 5 8"); //Face superior "traseira"
	ampulheta.AddFace("5 6 7 8"); //Face "de cima"
	
	//Calculando as normais dos vértices baseadas na média das normais
	//das faces compartilhadas por eles
	ampulheta.ComputeVertexNormals();
	
	//Dá um meterial à ampulheta
	ampulheta.SetMaterial(Material::PLASTIC_BLUE());
	
	//Adiciona a ampulheta à cena
	scene.AddObject(&ampulheta);
	
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
