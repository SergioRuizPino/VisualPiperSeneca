// Fill out your copyright notice in the Description page of Project Settings.


#include "CicloDiaNoche.h"

float hora = 12;
float niebla =40000 ;
float nubeht = 0.1;
float nubeb = 0.1;
float antiguaHora = 1;
float ttt = 0;
bool activo = false;
FRotator gir;
FQuat quat;
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
		/*this->horaInterna = hora;
		antiguaHora = hora;
		UE_LOG(LogTemp, Warning, TEXT("Delta    %f "), this->horaInterna);

		if (this->horaInterna >= 21 || this->horaInterna <= 7) {

			gir = FRotator(75+ this->horaInterna/12, 0, 0); 	//h heading z p pitcth eje y , R roll eje x,	//ht altitud

		
		}
		else {
			
		}

		quat = FQuat(gir);
		this->LuzMv->SetActorRotation(gir);*/
	}

	if (this->LuzMv->GetActorRotation().Pitch > 75) {
		Movimiento = 2;//0.4;
	}
	else {
		Movimiento = 5;//7.5;
	}

	if (this->LuzMv != nullptr) {
		this->LuzMv->AddActorLocalRotation(FRotator(DeltaTime*Movimiento, 0, 0));
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

float ACicloDiaNoche::returnFogExpAlt() {  //+pequeño - transicion  //10 Max
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
				}else {
				act = false;
			}

		}
		UE_LOG(LogTemp, Warning, TEXT("valor activo %d"), act);
		return act;
	}else {
		return act;
	}




}


