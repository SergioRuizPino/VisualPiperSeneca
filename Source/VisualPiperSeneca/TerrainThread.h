// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "HAL/Runnable.h"

/**
 * 
 */
class VISUALPIPERSENECA_API TerrainThread : public FRunnable
{
public:
	TerrainThread(TArray<FVector>  Vertices,TArray<int> triangulos, TArray<FVector> VectorN, TArray<FVector2D> US, TArray<FLinearColor> vertex, TArray<FProcMeshTangent> Tange,UProceduralMeshComponent* terr);
	virtual ~TerrainThread() override;
	bool Continua;
    // Overriden from FRunnable
    bool Init() override; // Do your setup here, allocate memory, ect.
    uint32 Run() override; // Main data processing happens here
    void Stop() override; // Clean up any memory you allocated here
	TArray<FVector> VerticesMalla;
	TArray<int> triangulosMalla;
	TArray<FVector> VectorNormales;
	TArray<FVector2D> UVS;
	TArray<FLinearColor> vertexColors;
	TArray<FProcMeshTangent> Tangente;
	TArray<uint8> pruebaFileBInary;
	void VerticesTriangulos();
	void TriangulosAnadir();
	int ancho;
	int alto;
	int indice;
	int Espaciotri;

	
private :
    FRunnableThread* HiloCreador;
	UProceduralMeshComponent* cret;


};