// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "XmlFile.h" 
#include "ProceduralMeshComponent.h"
//#include "RuntimeMeshComponent.h"
#include "TerrainGenerator.generated.h"

UCLASS()
class VISUALPIPERSENECA_API ATerrainGenerator : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATerrainGenerator();
	void lecturaXml(const FXmlNode* fichero);
	void setFichero(FString fiche);
	virtual void Tick(float DeltaTime) override;
	void setInicio(FString abrir);



	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginDestroy() override;
private:
	UProceduralMeshComponent* Terreno;
	TArray<FVector> VerticesMalla;
	TArray<int> triangulosMalla;
	TArray<FVector> VectorNormales;
	TArray<FVector2D> UVS;
	TArray<FLinearColor> vertexColors;
	TArray<FProcMeshTangent> Tangente;
	void TriangulosAnadir();
	void VerticesTriangulos();
	void LimpiarMalla();
	int ancho;
	int alto;
	float Espaciotri;
	FXmlFile ficheroInformacion;
	FString ficheroReferencia;
	TArray<uint8> pruebaFileBInary;




};
