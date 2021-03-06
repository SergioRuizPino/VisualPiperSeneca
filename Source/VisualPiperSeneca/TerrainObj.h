// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XmlFile.h" 
#include "TerrainThread.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "ProceduralMeshComponent.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Materials/MaterialInterface.h"
#include "TerrainObj.generated.h"

UCLASS()
class VISUALPIPERSENECA_API ATerrainObj : public AActor
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	ATerrainObj();
	virtual void Tick(float DeltaTime) override;
	void lecturaXml(const FXmlNode* fichero);
	void setFichero(FString fiche);
	void setInicio(FString abrir);
	const int DevolverXreal() const;
	const int DevolverYreal() const;
	const int DevolverAncho() const;
	const int DevolverLargo() const;
	const int DevolverDistancia() const;
	TerrainThread* hilo;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginDestroy() override;
	virtual void PostLoad() override;

private:
	UProceduralMeshComponent* Terreno;
	TArray<FVector> VerticesMalla;
	TArray<int> triangulosMalla;
	TArray<FVector> VectorNormales;
	TArray<FVector2D> UVS;
	TArray<FLinearColor> vertexColors;
	TArray<FProcMeshTangent> Tangente;
	UMaterial* StoredMaterial;
	UMaterialInstanceDynamic* DynamicMaterialInst;
	void TriangulosAnadir();
	void VerticesTriangulos();
	void LimpiarMalla();
	UTexture2D* Textura(const FString& FullFilePath, bool& IsValid, int32& Width, int32& Height);
	int ancho;
	int alto;
	int anchoEscala;
	int altoEscala;
	int32 indice = 0;
	int x, y; //Posici�n Mapa 
	int realx, realy;
	int Espaciotri;
	FXmlFile ficheroInformacion;
	FString ficheroReferencia;
	TArray<uint8> pruebaFileBInary;
};
