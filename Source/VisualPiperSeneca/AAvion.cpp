/* Fill out your copyright notice in the Description page of Project Settings.
*
* @author: Sergio Ruiz Pino
* @version : 0.1
*
*/


#include "AAvion.h"
#include "Kismet/GameplayStatics.h"

//Local 

int ii = -25000;
extern uint8 RTX_DATA_RECIBIDO[paqueteMax];  //Bus de datos Recibir
extern uint8 RTX_DATA_ENVIO[paqueteMax];  //Bus de datos Enviar
extern int32 bytesleidos;
FVector pos;

AAAvion::AAAvion()
{
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAAvion::BeginPlay()
{/*

	if (!connected) { //Sino no nos conectamos, no podemos funcionar ,DESCOMENTAR ukinet TRAS PRUEBAS
		//UE_LOG(LogTemp, Warning, TEXT("No Conectado"));
		//FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("No se ha podido realizar la conexion con Dynamics, revise los datos de conexion,se cerrara  Visual."));
		//UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
		//GIsRequestingExit = 1; Esta intruccion en el constructor no puede ir  cierra el editor, ademas es una instruccion sucia.
	}*/
	Super::BeginPlay();
	APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0); //Control de camara
	OurPlayerController->SetViewTargetWithBlend(this, 1.f);

}

// Called every frame
void AAAvion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	siguientePos();

}

// Called to bind functionality to input
void AAAvion::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FVector AAAvion::siguientePos() { //DEBUG METODO
	FVector siguiente = GetActorLocation();
	//siguiente = actualizarVectorMovimiento(siguiente);

	ii += 50;
	siguiente.X = 5000;
	siguiente.Y = ii;
	siguiente.Z = 3000;
	//if (ii <= 10000) { siguiente.Y = ii; }else{ siguiente.Y = 10000; }
	//if (ii <= 3000) { siguiente.Z = ii; }else{ siguiente.Z = 3000; }
	if (ii >= 270000) { ii = -26000; }
	//UE_LOG(LogTemp, Warning, TEXT("cONEXTADO %i"), ii);
	SetActorLocation(siguiente);
	pos = siguiente;
	return siguiente;

}

void AAAvion::actualizarEnvio() {
	this->DynamicsEnvio = (EnviarDatos*) RTX_DATA_ENVIO;
	//Transformaciones al movimiento
	FVector siguiente = GetActorLocation();







	SetActorLocation(siguiente);
}

void AAAvion::actualizarEstado() {
	if (bytesleidos) {

	}

}
