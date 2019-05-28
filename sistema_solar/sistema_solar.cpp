//sistema_solar.cpp

/* Este programa desenha um sistema solar contendo o sol
a terra e a lua, a terra gira em torno de si mesma e do sol e 
a lua gira em torno da terra.
*/

#include <vart/scene.h>
#include <vart/light.h>
#include <vart/sphere.h>
#include <vart/transform.h>
#include <vart/rotationaction.h>
#include <vart/contrib/viewerglutogl.h>
#include <vart/texture.h>

#include <iostream>
#include <string.h>
#include <vector>

using namespace std;
using namespace VART;

class MyIHClass : public VART::ViewerGlutOGL::IdleHandler
{
    public:
        MyIHClass() : earthSelfRadians(0), luaRadians(0), earthSunRadians(0) {}
        virtual ~MyIHClass() {}
      
        virtual void OnIdle() {
			earthSelfRadians += 0.05; 
            earthSunRadians += 0.001;
            luaRadians += 0.015;
            
            terraSunRotPtr->MakeZRotation(earthSunRadians);
            
            terraERotPtr->MakeZRotation(earthSelfRadians);
            
            luaRotPtr->MakeZRotation(luaRadians);
            
            viewerPtr->PostRedisplay();
        }
   
    //protected:
        Transform* luaRotPtr;
        Transform* terraSunRotPtr;
        Transform* terraERotPtr;
        
    private:
		float earthSelfRadians;
        float redRadians;
        float luaRadians;
        float earthSunRadians;
};

int main(int argc, char* argv[]){
	if(argc < 4){
		cout << "Informe o nome dos arquivos com as texturas do sol, da terra e da lua, respectivamente.";
		return 0;
	}
	
	ViewerGlutOGL::Init(&argc, argv); //Inicializa a GLUT
	static Scene scene; //Cria uma cena
	static ViewerGlutOGL viewer; //Cria uma viewer(Janela de aplicação)
	
	//Cria uma câmera (Observador da cena)
	Camera camera(Point4D(0, 0, 10), Point4D(0, 0, 0), Point4D(0, 1, 0));
	
	//Cria o sol a terra e a lua
	Sphere sol(1.0);
	Sphere terra(0.5);
	Sphere lua(0.1);
	
	//Cria os materiais e as texturas 
	Texture texturaSol;
	Material matSol;
	Texture texturaTerra;
	Material matTerra;
	Texture texturaLua;
	Material matLua;
	
	//Carrega e aplica as texturas
	if(texturaSol.LoadFromFile(argv[1]) && texturaTerra.LoadFromFile(argv[2])  && texturaLua.LoadFromFile(argv[3])){
		matSol.SetTexture(texturaSol);
		matTerra.SetTexture(texturaTerra);
		matLua.SetTexture(texturaLua);
	}else{
		cout << "Não foi possível ler o arquivo de textura.";
		return 0;
	}
	
	//Aplica os materiais
	sol.SetMaterial(matSol);
	terra.SetMaterial(matTerra);
 	lua.SetMaterial(matLua);
 	

	//Faz a terra girar em torno de si mesma
	Transform terraRotation;
	Transform terraPosition;
	terraPosition.MakeTranslation(3.5, 0, 0);
	terraRotation.AddChild(terra);
	terraPosition.AddChild(terraRotation);

	//Faz a lua girar em torno da terra
	Transform luaPosition;
	Transform luaRotation;
	luaPosition.AddChild(lua);
	luaRotation.AddChild(luaPosition);
	terraPosition.AddChild(luaRotation);
	//Translação em relação à posição da terra
	luaPosition.MakeTranslation(1, 0, 0);

	
	//Faz a terra girar em torno do centro(sol)
	Transform terraRotAroundSun;
	terraRotAroundSun.AddChild(terraPosition);

	scene.AddObject(&terraRotAroundSun);
	scene.AddObject(&sol);
		
	//Adiciona Luz e câmera à cena
	scene.AddLight(Light::BRIGHT_AMBIENT());
	scene.AddCamera(&camera);
	
	//Set up the Idle Handler
 	MyIHClass idleHandler;
    idleHandler.terraERotPtr = &terraRotation;
	idleHandler.terraSunRotPtr = &terraRotAroundSun;
	idleHandler.luaRotPtr = &luaRotation;
     
	//Configura a janela de exibição
	viewer.SetTitle("Sistema Solar");
	viewer.SetScene(scene);
	viewer.SetIdleHandler(&idleHandler); // attach the idle handler to the viewer
	
	//Executa a aplicação
	scene.DrawLightsOGL(); //Configura o estado das luzes do OpenGL
	ViewerGlutOGL::MainLoop(); //Entra no loop main(loop de eventos) e nunca retorna
}
