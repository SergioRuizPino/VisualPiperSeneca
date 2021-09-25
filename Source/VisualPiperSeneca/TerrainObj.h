/*
*
* @author: Sergio Ruiz Pino
* @version : 0.1
*
*/


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
	TerrainThread* hilo; //creacion mediante hilos, desactivado debido a no mejora eficiencia
	class UBoxComponent* caja;
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* componenteChoque, AActor* ActorChoque, UPrimitiveComponent* componente, int32 jind, bool desde, const FHitResult& resultado);

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
	int x, y; //Posición Mapa 
	int realx, realy; //pos real
	int Espaciotri;
	FXmlFile ficheroInformacion;
	FString ficheroReferencia;
	TArray<uint8> pruebaFileBInary;
};
