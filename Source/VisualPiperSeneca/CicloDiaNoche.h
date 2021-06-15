// Fill out your copyright notice in the Description page of Project Settings.

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
	//UPROPERTY(EditAnywhere, Category = "p")
	//BP_Sky_Sphere* BPC;
	//UAtmosphericFogComponent* niebla;
	//AAtmosphericFog* a;
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
