/*
*
* @author: Sergio Ruiz Pino
* @version : 0.1
*
*/


#pragma once

#include "CoreMinimal.h"
#include "Engine/DirectionalLight.h"
#include "XmlFile.h" 
#include "GameFramework/Actor.h"
#include "CicloDiaNoche.generated.h"


UCLASS()
class VISUALPIPERSENECA_API ACicloDiaNoche : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACicloDiaNoche();
	UPROPERTY(EditAnywhere,Category = "p")
		ADirectionalLight* LuzMv;
	UPROPERTY(EditAnywhere,Category = "p")
		float Movimiento;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
		float returnCloud();
	UFUNCTION(BlueprintCallable)
		float returnFog();
	UFUNCTION(BlueprintCallable)
		float returnFogSun();
	UFUNCTION(BlueprintCallable)
		float returnFogExp();
	UFUNCTION(BlueprintCallable)
		float returnFogExpAlt();
	UFUNCTION(BlueprintCallable)
		float RnieblaAlt();
	UFUNCTION(BlueprintCallable)
		bool CesiumOn();
	UFUNCTION(BlueprintCallable)
		bool ModPosInicio();
	UFUNCTION(BlueprintCallable)
		float ModLon();
	UFUNCTION(BlueprintCallable)
		float ModLat();
	UFUNCTION(BlueprintCallable)
        FVector nPos();
	UFUNCTION(BlueprintCallable)
		void Choque();

	void anularFog();
	void CalcularFog();
	void CalcularAltFog();
	bool LecturaXml(FXmlNode* node);
private:
	float nubeP = 0.1;
	float nieblap = 0.1;
	float sunF = 0.1;
	float Expofog = 0.1;
	float ExpofogAlt = 0.01;
	float alturaN = 0.1;
	float horaInterna = 12;
};
