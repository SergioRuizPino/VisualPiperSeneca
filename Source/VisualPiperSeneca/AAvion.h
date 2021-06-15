/* 
*
* @author: Sergio Ruiz Pino
* @version : 0.1
*
*/

#pragma once
#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "AAvion.generated.h"


#define paqueteMax 1472

UCLASS()
class VISUALPIPERSENECA_API AAAvion : public ACharacter
{
	GENERATED_BODY()

public:
	AAAvion();
	virtual void Tick(float DeltaTime) override; 
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override; 

protected:
	virtual void BeginPlay() override;

private:	
	UCameraComponent* Cam;
	USpringArmComponent* brazo;
	APlayerController* Camara;
	void actualizarEstado(); //Actualiza Estado actual del avion segun datos recibidos
	float DistanciaZ(FVector actual);//Metodo que devuelve la distancia entre avion y terreno
};
