/* 
*
* @author: Sergio Ruiz Pino
* @version : 0.1
*
*/

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AAvion.generated.h"

#define paqueteMax 1472

UCLASS()
class VISUALPIPERSENECA_API AAAvion : public ACharacter
{
	GENERATED_BODY()

public:
	AAAvion();
	virtual void Tick(float DeltaTime) override; //en cada frame
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override; 	// Called to bind functionality to input

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:	
	//TArray<char, TFixedAllocator<paqueteMax>> RTX_DATA;
	FVector siguientePos();
	void actualizarEstado(); //Actualiza Estado actual del avion segun datos recibidos
	void actualizarEnvio(); //Actualiza los datos para enviar.
	//void actualizarVectorMovimiento(FVector actual);
	struct EnviarDatos{  //Estrutura que se utilizara para enviar datos a Dynamics
		EnviarDatos();
	};

	struct RecibirDatos {  //Estructura que se usara para recibir datos de Dynamics
		RecibirDatos();
	};
	EnviarDatos* DynamicsEnvio;
	//static RecibirDatos DynamicsRecibe;
};
