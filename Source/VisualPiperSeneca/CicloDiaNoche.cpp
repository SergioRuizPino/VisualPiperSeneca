/*
*
* @author: Sergio Ruiz Pino
* @version : 0.1
*
*/


#include "CicloDiaNoche.h"

float hora = 12;
float niebla = 40000 ;
float nubeht = 0.1;
float nubeb = 0.1;
float antiguaHora = 1;
float ttt = 0;
bool activo = false;
FRotator gir;
FQuat quat;
FVector nueva;
extern double lonO;
extern double latO;
extern bool  ModPosition;
// Sets default values
ACicloDiaNoche::ACicloDiaNoche()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void ACicloDiaNoche::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACicloDiaNoche::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	if (hora != antiguaHora) {
		this->horaInterna = hora;
		antiguaHora = hora;
		UE_LOG(LogTemp, Warning, TEXT("hora cambiada    %f "), this->horaInterna);
		gir.Yaw = 0;
		gir.Roll = 0;
		if (hora >= 0 && hora < 1) { gir.Pitch = 95;}
		if (hora >= 1 && hora < 2) { gir.Pitch = 92; }
		if (hora >= 2 && hora < 3) { gir.Pitch = 90; }
		if (hora >= 3 && hora < 4) { gir.Pitch = 88; }
		if (hora >= 4 && hora < 5) { gir.Pitch = 86; }
		if (hora >= 5 && hora < 6) { gir.Pitch = 82; }
		if (hora >= 6 && hora < 7) { gir.Pitch = 65; }
		if (hora >= 7 && hora < 8) { gir.Pitch = 40; }
		if (hora >= 8 && hora < 9) { gir.Pitch = 0; }
		if (hora >= 9 && hora < 10) { gir.Pitch = -10; }
		if (hora >= 10 && hora <= 11) { gir.Pitch = -20; }
		if (hora >= 11 && hora <= 12) { gir.Pitch = -40; }
		if (hora >= 12 && hora <= 13) { gir.Pitch = -60; }
		if (hora >= 13 && hora <= 14) { gir.Pitch = -80; }
		if (hora >= 14 && hora <= 15) { gir.Pitch = -90; }
		if (hora >= 15 && hora <= 16) { gir.Pitch = -100; }
		if (hora >= 16 && hora <= 17) { gir.Pitch = -120; }
		if (hora >= 17 && hora <= 18) { gir.Pitch = -140; }
		if (hora >= 18 && hora <= 19) { gir.Pitch = -160; }
		if (hora >= 19 && hora <= 20) { gir.Pitch = -180; }
		if (hora >= 20 && hora <= 21) { gir.Pitch = 160; }
		if (hora >= 21 && hora <= 22) { gir.Pitch = 110; }
		if (hora >= 22 && hora <= 23) { gir.Pitch = 105; }
		if (hora >= 23 && hora <= 24) { gir.Pitch = 100; }

		quat = FQuat(gir);
		this->LuzMv->SetActorRotation(gir);
	}

	Movimiento = 0.0004166;//movimiento velocidad a tiempo real
	
	if (this->LuzMv != nullptr) {
		this->LuzMv->AddActorLocalRotation(FRotator(-DeltaTime*Movimiento, 0, 0));
		//UE_LOG(LogTemp, Warning, TEXT("  estado rotacion   %f     f "), this->LuzMv->GetActorRotation().Pitch);
	}

}

float ACicloDiaNoche::returnCloud() {
	float aux = nubeht - nubeb;
	if (aux < 0 || aux >=20000) {
		this->nubeP = 0;
	}else {
		this->nubeP = aux / 17;
	}
	return this->nubeP;
}

float ACicloDiaNoche::returnFog() {

	if (niebla >= 30000) {
		this->anularFog();
	}
	else {
		this->nieblap = this->nieblap / 5000; 	 
	}
	return this->nieblap;
}

float ACicloDiaNoche::returnFogSun() {
	float aux = nubeht - nubeb;
	if (aux <= 0) {

		if (niebla <= 30000) {
			aux = niebla - 30000;
			if (aux < 0) { aux = aux * (-1); }
			this->sunF = aux / 1764;
		}else{ this->sunF = 1; }

	}else {
		aux = aux / 17;
		this->sunF = aux / 15;
	}


	return this->sunF;
}

float ACicloDiaNoche::returnFogExp() { 
	CalcularFog();
	return this->Expofog;
}

float ACicloDiaNoche::returnFogExpAlt() {  //+pequeno - transicion  //10 Max
	if (niebla > 30000) {
		this->anularFog();
	}else {
		CalcularAltFog();
	}
	return this->ExpofogAlt;
}

float ACicloDiaNoche::RnieblaAlt() {  
	this->alturaN =1;
	return this->alturaN;
}



void ACicloDiaNoche::anularFog() {
	this->Expofog = 0.00001;
	this->ExpofogAlt = 0.1;
	this->nieblap = 0.1;
}

void ACicloDiaNoche::CalcularFog() {
	if (niebla > 30000) {
		this->anularFog();
	}else {

		float aux = niebla - 30000;
		if (aux < 0) { aux = aux * (-1); }
		aux = aux / 600000;
		aux = aux * 140.879 + (float)((30000 - niebla) / 6000);
		if (niebla < 30000 && niebla >15000) {
			aux = aux / 25;
		}
		if (niebla < 15000 && niebla >10000) {
			aux = aux / 20;
		}
		if (niebla < 10000 && niebla >5000) {
			aux = aux / 15;
		}
		if (niebla > 5000) {
			aux = aux / 0.9;
		}
		this->Expofog = aux;
	}}

void ACicloDiaNoche::CalcularAltFog() {
	if (niebla > 30000) {
		this->anularFog();
	}
	else {
		float aux = niebla / 30000;
		if (niebla < 6000) { aux = aux / 10.342; }
		this->ExpofogAlt = aux;
	}


}

bool ACicloDiaNoche::CesiumOn() {
	activo = false; //Partimos de que cesium es no esta operativo


	FXmlFile ficheroAuxiliar;
	ficheroAuxiliar.LoadFile(FPaths::ProjectDir() + FString("Config") + ".xml", EConstructMethod::ConstructFromFile); //fichaload

	if (ficheroAuxiliar.IsValid()) {
		activo = this->LecturaXml(ficheroAuxiliar.GetRootNode());
	}

	return activo;
}

bool ACicloDiaNoche::LecturaXml(FXmlNode* node) {
	bool act = false;
	int aux = 0;
	if (node != nullptr)
	{
		const FXmlNode* nodee = node->FindChildNode("CESIUMOFF");
		if (nodee != nullptr) {
			aux = FCString::Atoi(*nodee->GetContent());
			if (aux == 1) {
				act = true;
			}
			else {
				act = false;
			}

		}
		UE_LOG(LogTemp, Warning, TEXT("valor activo %d"), act);
		return act;
	}
	else {
		return act;
	}
}


	bool ACicloDiaNoche::ModPosInicio(){
		return ModPosition;
	}

	float ACicloDiaNoche::ModLat(){
		return latO;
	}

	float ACicloDiaNoche::ModLon(){
		return lonO;
	}

	FVector ACicloDiaNoche::nPos() {
		nueva.X = lonO;
		nueva.Y = latO;
		nueva.Z = 220;
		return nueva;
	}





