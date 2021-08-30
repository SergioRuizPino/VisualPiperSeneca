/*
*
* @author: Sergio Ruiz Pino
* @version : 0.1
*
*/


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TerrainObj.h"
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
//	TerrainThread* prueba;
	//bool PosNuevo();  //Indica si etamos en una posicion para crear un nuevo terreno
	void CrearTerreno(FString);
	//TMap<ATerrainGenerator*,TPair<FString, bool>> TerrenosActivos;
	TMap<FString,TPair<ATerrainObj*, bool>> TerrenosActivos;
	TMap<FString, TPair<int, int>> TerrenosPosReal;
	TMap<FString, TPair<int, int>> Dimensiones;
	TMap<int, int> LimitesTerrenosSup;
	TMap<int, int> LimitesTerrenosInf;
	void InsertarLimites(int, int);
	void PrintMAP();
	FString DevuelveTerrenoActualString();
	void DevuelveTerrenoActualInt(int32*,int32*);
	double DistanciaTerrenosActivos(FString posicion);
	void EliminaTerrenosActivos(ATerrainObj*);
	void EliminadorTerrenosTick();
	void CreadorTerrenosTick();
	double DistanciaPuntoMedio(ATerrainObj*);
	double DistanciaTerrenosNoActivo(int, int);
	bool EsLimite(int,int);
	void CalcularPos(FString fichero, int x, int y);
	int ajustarPosicionMapaYNegativa(int x, int y);
	int ajustarPosicionMapa(int x, int y);
	int ajustarPosicionMapaNegativa(int x, int y);
	int ajustarPosicionMapaY(int x, int y);
	int lecturaXmlLargo(const FXmlNode* ficher);
	int lecturaXmlAncho(const FXmlNode* ficher);
	void InsertarDatosTer(FString fichero, int x, int y);
};
