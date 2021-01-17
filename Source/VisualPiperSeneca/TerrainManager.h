// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TerrainGenerator.h"
#include "TerrainManager.generated.h"

UCLASS()
class VISUALPIPERSENECA_API ATerrainManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATerrainManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(EEndPlayReason::Type t) override;

private:	
	// Called every frame
	//bool PosNuevo();  //Indica si etamos en una posicion para crear un nuevo terreno
	void CrearTerreno(FString, int, int);
	//TMap<ATerrainGenerator*,TPair<FString, bool>> TerrenosActivos;
	TMap<FString,TPair<ATerrainGenerator*, bool>> TerrenosActivos;
	TMap<int, int> LimitesTerrenosSup;
	TMap<int, int> LimitesTerrenosInf;
	void InsertarLimites(int, int);
	void PrintMAP();
	FString DevuelveTerrenoActualString();
	void DevuelveTerrenoActualInt(int32*,int32*);
	double DistanciaTerrenosActivos(ATerrainGenerator*);
	void EliminaTerrenosActivos(ATerrainGenerator*);
	void EliminadorTerrenosTick();
	void CreadorTerrenosTick();
	double DistanciaPuntoMedio(ATerrainGenerator*);
	double DistanciaTerrenosNoActivo(int, int);
	bool EsLimite(int,int);
};
