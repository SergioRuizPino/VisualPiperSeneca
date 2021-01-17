// Fill out your copyright notice in the Description page of Project Settings.



#include "Math/UnrealMathUtility.h"
#include "TerrainGenerator.h"

//TArray<FString> paths;
uint32 aux33 = 0;

// Sets default values
ATerrainGenerator::ATerrainGenerator()
{

	if (ficheroReferencia != "") {
		setInicio(ficheroReferencia);
		UE_LOG(LogTemp, Warning, TEXT("SI ESTA VACIO ES UN ERROR %s"), *ficheroReferencia);
	}
	else {
		ancho = 1000;
		alto = 1000;
		Espaciotri = 25;
	}

	Terreno = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("TerrenoProcedural"));
	Terreno->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	aux33 = 0;
	PrimaryActorTick.bCanEverTick = false;
}



void ATerrainGenerator::setInicio(FString fiche) {
	ficheroReferencia = fiche;
	ficheroInformacion.LoadFile(FPaths::ProjectDir() + FString("terrenos/") + fiche + ".xml", EConstructMethod::ConstructFromFile); //fichaload
	FString pathProjectoTerrenos2 = FPaths::ProjectDir() + FString("terrenos/") + fiche +".raw";// 
	const TCHAR* RootPath2 = *(pathProjectoTerrenos2);

	if (ficheroInformacion.IsValid()) {
		UE_LOG(LogTemp, Warning, TEXT("archivo abierto"));
		const FXmlNode* nodee = ficheroInformacion.GetRootNode();
		lecturaXml(nodee->GetFirstChildNode());
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("No encuentra"));

	}

	if (FFileHelper::LoadFileToArray(pruebaFileBInary, RootPath2, 0)) {
		UE_LOG(LogTemp, Warning, TEXT("%u bits"), pruebaFileBInary.Num());
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("No valido"));
	}
}

void ATerrainGenerator::setFichero(FString fiche) {
	ficheroReferencia = fiche;
}


void ATerrainGenerator::EndPlay(EEndPlayReason::Type t) {
	//if(t == EEndPlayReason::Type::Quit)
	aux33 = 0;
	LimpiarMalla();
}

// Called when the game starts or when spawned
/*void ATerrainGenerator::BeginPlay()
{
	//IFileManager& FileManager = IFileManager::Get();
	//UE_LOG(LogTemp, Warning, TEXT("Encontro la fun: %s"), *pathProjectoTerrenos);
	//FileManager.FindFiles(paths, RootPath, extension);
}*/

// Called every frame
void ATerrainGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
/*
void ATerrainGenerator::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}*/

void ATerrainGenerator::lecturaXml(const FXmlNode* ficher) { //generico no hijos mas comodo < </> </> </> />
	const FXmlNode* nodee = ficher;
	if (nodee != nullptr)
	{
		FString tag = nodee->GetTag();
		
		if (tag.Contains("ANCHO")) {
			ancho = FCString::Atoi(*nodee->GetContent());
			UE_LOG(LogTemp, Warning, TEXT("ancho %s"), *nodee->GetContent());
		}

		if (tag.Contains("ALTO")) {
			alto = FCString::Atoi(*nodee->GetContent());
			UE_LOG(LogTemp, Warning, TEXT("alto %s"), *nodee->GetContent());
		}
		if (tag.Contains("DISTANCIA")) {
			Espaciotri = FCString::Atoi(*nodee->GetContent());
			UE_LOG(LogTemp, Warning, TEXT("distancua %s"), *nodee->GetContent());
		}

		//UE_LOG(LogTemp, Warning, TEXT("valor %s"), *nodee->GetContent());
		if (nodee->GetNextNode() != nullptr)
		{
			lecturaXml(nodee->GetNextNode());  //x-y-z-x-y....

		}

	}

}

void ATerrainGenerator::TriangulosAnadir(){ //AÑADE LOS TRIANGULOS DE LA MALLA

	for (int32 yy = 0; yy < alto - 1; yy++)
	{
		for (int32 xx = 0; xx < ancho - 1; xx++)
		{
			triangulosMalla.Add(xx + (yy * ancho));					
			triangulosMalla.Add(xx + (yy * ancho) + ancho);			
			triangulosMalla.Add(xx + (yy * ancho) + ancho + 1);		

			triangulosMalla.Add(xx + (yy * ancho));					
			triangulosMalla.Add(xx + (yy * ancho) + ancho + 1);		
			triangulosMalla.Add(xx + (yy * ancho) + 1);				
		}
	}
}


void  ATerrainGenerator::VerticesTriangulos()
{
	float prueba = 0.0f;
	float uvSpacing = 1.0f / FMath::Max(ancho, alto);
	uint16 datos = 0, datos2 = 0;

	for (int32 yy = 0; yy < alto; yy++)
	{
		for (int32 xx = 0; xx < ancho; xx++)
		{

			memcpy(&datos, &pruebaFileBInary[aux33], 2); //buffer  a variable

			//uint16 xxxx = (vvvv >> 8 | vvvv << 8);
			if (datos > 8849) {  //si altura mayor que el punto más alto tieera 8848 , altura = 0
				datos = 0;
			}

			if (datos < -1) {  //si altura menor 0 , altura = 0
				datos = 0;
			}
			
			VerticesMalla.Add(FVector(xx * Espaciotri, yy * Espaciotri,datos));
			VectorNormales.Add(FVector(0.0f, 0.0f, 1.0f));
			aux33 = aux33 + 2;

			UVS.Add(FVector2D(xx * uvSpacing, yy * uvSpacing));
			vertexColors.Add(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
			Tangente.Add(FProcMeshTangent(1.0f, 0.0f, 0.0f));
		}
	}
}

void ATerrainGenerator::OnConstruction(const FTransform& Transform) //Costruccion terreno
{

	this->TriangulosAnadir();
	this->VerticesTriangulos();

	Terreno->CreateMeshSection_LinearColor(0, VerticesMalla, triangulosMalla, VectorNormales, UVS, vertexColors, Tangente, false);
	//UMaterial* a();
	//Terreno->SetMaterial(0, a);
}


void ATerrainGenerator::LimpiarMalla() {

	VerticesMalla.Empty();
	triangulosMalla.Empty();
	VectorNormales.Empty();
	UVS.Empty();
	vertexColors.Empty();
	Tangente.Empty();
}

void ATerrainGenerator::BeginDestroy() {
	Super::BeginDestroy();
	LimpiarMalla();
	aux33 = 0;
}

/*



/*FXmlFile fich;
FString pathProjectoTerrenos = FPaths::ProjectDir() + FString("terrenos/");
const TCHAR* RootPath = *(pathProjectoTerrenos);
const TCHAR* extension = _T(".xml");


void  ATerrainGenerator::DatosTerreno() { //añadir elementos de prueba
	VerticesMalla.Add(FVector(0.0f, 0.0f, 0.0f)); //0
	VerticesMalla.Add(FVector(50.0f, 0.0f, 0.0f)); //1
	VerticesMalla.Add(FVector(0.0f, 50.0f, 0.0f)); //2
	VerticesMalla.Add(FVector(50.0f, 50.0f, 00.0f)); //3
	VerticesMalla.Add(FVector(100.0f, 0.0f, 00.0f)); //4
	VerticesMalla.Add(FVector(100.0f, 50.0f, 00.0f)); //5

	triangulosMalla.Add(0);
	triangulosMalla.Add(2);
	triangulosMalla.Add(3);

	triangulosMalla.Add(0);
	triangulosMalla.Add(3);
	triangulosMalla.Add(1);

	triangulosMalla.Add(1);
	triangulosMalla.Add(4);
	triangulosMalla.Add(5);

	triangulosMalla.Add(1);
	triangulosMalla.Add(5);
	triangulosMalla.Add(3);

	UVS.Init(FVector2D(1.0f, 1.0f), 7);
	VectorNormales.Init(FVector(1.0f, 1.0f, 1.0f), 7);
	vertexColors.Init(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), 7);
	Tangente.Init(FProcMeshTangent(1.0f, 0.0f, 0.0f), 7);
}

*/

/*
void AAvion::lecturaXml(const FXmlNode* ficher) { //generico < < </> </> </> /> />
//	FXmlNode* node = fichero.GetRootNode();
	const FXmlNode* nodee = ficher;
	if (nodee != nullptr) {

		if (nodee->GetFirstChildNode() != nullptr)
		{
			lecturaXml(nodee->GetFirstChildNode());
			if (nodee->GetNextNode() != nullptr)
			{
				lecturaXml(nodee->GetNextNode());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("valor %s"), *nodee->GetContent());
			}
			//UE_LOG(LogTemp, Warning, TEXT("NULO"));
		}
		else {
			if (nodee->GetNextNode() != nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("valor %s"), *nodee->GetContent());
				lecturaXml(nodee->GetNextNode());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("valor %s"), *nodee->GetContent());
			}

		}

	}

}*/

