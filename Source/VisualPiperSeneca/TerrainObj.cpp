// Fill out your copyright notice in the Description page of Project Settings.


#include "TerrainObj.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ATerrainObj::ATerrainObj()
{
	PrimaryActorTick.bCanEverTick = false;
	Terreno = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("TerrenoProcedural"));

	if (this->ficheroReferencia != "") {
		setInicio(this->ficheroReferencia);
		UE_LOG(LogTemp, Warning, TEXT("SI ESTA VACIO ES UN ERROR %s"), *ficheroReferencia);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("no fichero "));
	}




	//StoredMaterial->
//	bool a;
	
	FString sPath = FPaths::ProjectDir() + FString("Materiales/00_Mat.00_Mat"); //"/Game/Materiales/00.uasset";

	/*UMaterial* mat = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), nullptr, *sPath));

	if (mat != nullptr) {
		DynamicMaterialInst = UMaterialInstanceDynamic::Create(mat, Terreno);
		UE_LOG(LogTemp, Warning, TEXT("material "));
	}*/



	/*static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial(*FString("/Game/Materiales/MaterialTerreno.MaterialTerreno")); // /"  "/Game/Materiales/01_Mat.01_Mat"
    if (FoundMaterial.Succeeded())
	{
		StoredMaterial = FoundMaterial.Object;
		DynamicMaterialInst = UMaterialInstanceDynamic::Create(StoredMaterial, Terreno);
		UE_LOG(LogTemp, Warning, TEXT(" entrada de material "));
	}
	//UE_LOG(LogTemp, Warning, TEXT(" value ruta %sMateriales/M_Water_Ocean.uasset") ,*FPaths::ProjectDir());*/
	this->indice = 0;
	this->Terreno->SetMobility(EComponentMobility::Static);
	//this->terenorap = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("HierarchicalInstancedStaticMesh"));
	//SetRootComponent(this->terenorap);
	Terreno->bUseAsyncCooking = true;

}

// Called when the game starts or when spawned
void ATerrainObj::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ATerrainObj::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ATerrainObj::setInicio(FString fiche) {

	FString pp = fiche;
	FString lf, lf2;
	FString rigaux;
	FString rig;

	this->ficheroReferencia = fiche;
	this->ficheroInformacion.LoadFile(FPaths::ProjectDir() + FString("terrenos/") + fiche + ".xml", EConstructMethod::ConstructFromFile); //fichaload
	FString pathProjectoTerrenos2 = FPaths::ProjectDir() + FString("terrenos/") + fiche + ".raw";// 

	


	const TCHAR* RootPath2 = *(pathProjectoTerrenos2);

	if (this->ficheroInformacion.IsValid()) {
		lecturaXml(this->ficheroInformacion.GetRootNode());
	}
	else {
		this->ancho = 1000;
		this->alto = 1000;
		this->Espaciotri = 1;
	}

	if (FFileHelper::LoadFileToArray(pruebaFileBInary, RootPath2, 0)) { //lee corectamente
		UE_LOG(LogTemp, Warning, TEXT("%u bits"), pruebaFileBInary.Num());
	}
	else { //sino es valido todo por defecto a 0
		UE_LOG(LogTemp, Warning, TEXT("No valido"));
	}

	//Guardamos posición x,y
	pp.Split(",", &lf, &rigaux);
	this->x = FCString::Atoi(*lf);
	this->y = FCString::Atoi(*rigaux);
	//CalcularPos(); //calculamos posición real en el software
	//this->hilo = new TerrainThread(VerticesMalla, triangulosMalla, VectorNormales, UVS, vertexColors, Tangente,this->Terreno);
}

void ATerrainObj::setFichero(FString fiche) {
	this->ficheroReferencia = fiche;
	//proteger si fiche = null o no cumple con mascara x,y
}

void ATerrainObj::TriangulosAnadir() { //AÑADE LOS TRIANGULOS DE LA MALLA  //4194222948 bytes

	for (int32 yy = 0; yy < alto-1 ; yy++)
	{
		for (int32 xx = 0; xx < ancho-1 ; xx++)
		{
			triangulosMalla.Add(xx + (yy * ancho));
			triangulosMalla.Add(xx + (yy * ancho) + ancho);
			triangulosMalla.Add(xx + (yy * ancho) + ancho + 1);

			triangulosMalla.Add(xx + (yy* ancho));
			triangulosMalla.Add(xx + (yy* ancho) + ancho + 1);
			triangulosMalla.Add(xx + (yy* ancho) + 1);
		}
	}
}


void  ATerrainObj::VerticesTriangulos()
{
	float prueba = 0.0f;
	float Uv = 1.0f / FMath::Max(ancho, alto);
	uint16 datos = 0, datos2 = 0;

	for (int32 yy = 0; yy < alto; yy++)
	{
		for (int32 xx = 0; xx < ancho; xx++)
		{
			if (this->indice >= (pruebaFileBInary.Num())) {
				datos = 1;
			}
			else {
				memcpy(&datos, &pruebaFileBInary[this->indice], 2); //buffer  a variable
				this->indice = this->indice + 2;
				//uint16 xxxx = (vvvv >> 8 | vvvv << 8);
				//if (datos > 8849)
				//{  //si altura mayor que el punto más alto tierra 8848 , altura = 1
				//	datos =1 ;
				//}

				//if (datos <= 0)
				//{  //si altura menor igual 0 , altura = 1  //NO LIMITAR DATOS DE ALTURA
				//	datos = 1;
				//}
				//datos = 10000;
			}
			VerticesMalla.Add(FVector(xx *this->anchoEscala* Espaciotri, yy *this->altoEscala* Espaciotri, datos));
			VectorNormales.Add(FVector(0.0f, 0.0f, 1.0f));
			UVS.Add(FVector2D(xx * Uv, yy* Uv));
			vertexColors.Add(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
			Tangente.Add(FProcMeshTangent(1.0f, 0.0f, 0.0f));
		}
	}
}

void ATerrainObj::OnConstruction(const FTransform& Transform) //Costruccion terreno
{

	this->TriangulosAnadir();
	this->VerticesTriangulos();
	bool a = true;
	
	//UMaterialInterface* Material = Terreno->GetMaterial(0);
	//DynamicMaterialInst = UMaterialInstanceDynamic::Create(Material, Terreno);
	TArray<FLinearColor> vertexColors2;

   // this->Textura(FPaths::ProjectDir() + FString("Materiales/00.bmp"), a, this->ancho, this->alto);
//	Terreno->SetMaterial(0, DynamicMaterialInst);
	
	//DynamicMaterialInst->SetScalarParameterValue(TEXT("EmissiveStrength"), 76.9);
	//DynamicMaterialInst->SetVectorParameterValue(TEXT("Colour"), FLinearColor::Red); //FPaths::ProjectDir() + FString("Materiales/") + FString::FromInt(this->x) + "," + FString::FromInt(this->y) + ."BMP"
	//this->DynamicMaterialInst->SetTextureParameterValue(FName("Texture"), this->Textura(FPaths::ProjectDir() + FString("Materiales/") + "0,1.BMP", a, this->ancho, this->alto));


	Terreno->CreateMeshSection_LinearColor(0, VerticesMalla, triangulosMalla, VectorNormales, UVS, vertexColors, Tangente, true);
	//Terreno->SetMaterial

	//UMaterial* a();
	//POSICIIÓN DE EMPIECE  0, 0

	//Terreno->SetMaterial(0, a);
	//while (hilo->Continua) {}
	//Terreno->sta
	//Terreno->CreateMeshSection(0, VerticesMalla, triangulosMalla, VectorNormales, UVS, Tangente, true);
	//Terreno->UpdateMeshSection_LinearColor(0, VerticesMalla, triangulosMalla, VectorNormales, UVS, vertexColors, Tangente, true);
//	Terreno->bEnableAutoLODGeneration = true;
	
	//Terreno->CreateMeshSection_LinearColor(0, this->hilo->VerticesMalla, this->hilo->triangulosMalla, this->hilo->VectorNormales, this->hilo->UVS, this->hilo->vertexColors, this->hilo->Tangente, true);

	this->indice = 0;
	//terenorap->cr

}


void ATerrainObj::PostLoad()
{
	//Super::PostLoad();
	///this->TriangulosAnadir();
	//this->VerticesTriangulos();
	//Terreno->CreateMeshSection_LinearColor(0, VerticesMalla, triangulosMalla, VectorNormales, UVS, vertexColors, Tangente, true);
}

void ATerrainObj::LimpiarMalla() {

	VerticesMalla.Empty();
	triangulosMalla.Empty();
	VectorNormales.Empty();
	UVS.Empty();
	vertexColors.Empty();
	Tangente.Empty();
}

void ATerrainObj::BeginDestroy() {
	Super::BeginDestroy();
	LimpiarMalla();
	//delete this->hilo;
	this->indice = 0;
}

void ATerrainObj::lecturaXml(const FXmlNode* ficher) { //generico no hijos mas comodo < </> </> </> />
	if (ficher != nullptr)
	{

		const FXmlNode* node = ficher->FindChildNode("ANCHO");
		if (node != nullptr) {
			this->ancho = FCString::Atoi(*node->GetContent());
		}
		else {
			this->ancho = 1000;
		}

		const FXmlNode* nodee = ficher->FindChildNode("ALTO");
		if (nodee != nullptr) {
			this->alto = FCString::Atoi(*nodee->GetContent());
		}
		else {
			this->alto = 1000;
		}

		const FXmlNode* nodee2 = ficher->FindChildNode("DISTANCIA");
		if (nodee2 != nullptr) {
			this->Espaciotri = FCString::Atoi(*nodee2->GetContent());
		}
		else {
			this->Espaciotri = 1;
		}

		const FXmlNode* nodee3 = ficher->FindChildNode("LANCHO");
		if (nodee2 != nullptr) {
			this->anchoEscala = FCString::Atoi(*nodee3->GetContent());
		}
		else {
			this->anchoEscala = 20;
		}

		const FXmlNode* nodee4 = ficher->FindChildNode("LLARGO");
		if (nodee2 != nullptr) {
			this->altoEscala = FCString::Atoi(*nodee4->GetContent());
		}
		else {
			this->altoEscala = 20;
		}
	}
	else {
		this->ancho = 1000;
		this->alto = 1000;
		this->anchoEscala = 20;
		this->altoEscala = 20;
		this->Espaciotri = 1;
	}


}

const int ATerrainObj::DevolverXreal() const {
	return this->realx;
}

const int ATerrainObj::DevolverYreal() const {
	return this->realy;
}

const int ATerrainObj::DevolverAncho() const {
	return this->ancho;
}

const int ATerrainObj::DevolverLargo() const {
	return this->alto;
}
const int ATerrainObj::DevolverDistancia() const {
	return this->Espaciotri;
}


void ATerrainObj::EndPlay(EEndPlayReason::Type t) {
	this->indice = 0;
	this->LimpiarMalla();
}

/*
UTexture2D* ATerrainObj::Textura2(const FString& FullFilePath, bool& IsValid, int32& Width, int32& Height, TArray<FLinearColor>& OutPixels)
{
	//Clear any previous data
	OutPixels.Empty();

	IsValid = false;
	UTexture2D* LoadedT2D = NULL;

	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::BMP);

	//Load From File
	TArray<uint8> RawFileData;
	if (!FFileHelper::LoadFileToArray(RawFileData, *FullFilePath)) return NULL;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	//Create T2D!
	if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
	{
		TArray<uint8> UncompressedRGBA;
		if (ImageWrapper->GetRaw(ERGBFormat::RGBA, 8, UncompressedRGBA))
		{
			LoadedT2D = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_R8G8B8A8);

			//Valid?
			if (!LoadedT2D) return NULL;
			//~~~~~~~~~~~~~~

			//Out!
			Width = ImageWrapper->GetWidth();
			Height = ImageWrapper->GetHeight();

			const TArray<uint8>& ByteArray = UncompressedRGBA;
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

			for (int32 v = 0; v < ByteArray.Num(); v += 4)
			{
				if (!ByteArray.IsValidIndex(v + 3))
				{
					break;
				}

				OutPixels.Add(
					FLinearColor(
						ByteArray[v],		//R
						ByteArray[v + 1],		//G
						ByteArray[v + 2],		//B
						ByteArray[v + 3] 		//A
					)
				);
			}
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

			//Copy!
			void* TextureData = LoadedT2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
			FMemory::Memcpy(TextureData, UncompressedRGBA.GetData(), UncompressedRGBA.Num());
			LoadedT2D->PlatformData->Mips[0].BulkData.Unlock();

			//Update!
			LoadedT2D->UpdateResource();
		}
	}

	// Success!
	IsValid = true;
	
	return LoadedT2D;
	
}*/


UTexture2D* ATerrainObj::Textura(const FString& ImagePath, bool& IsValid, int32& OutWidth, int32& OutHeight)
{
	UTexture2D* Texture = nullptr;
	IsValid = false;

	// To avoid log spam, make sure it exists before doing anything else.
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*ImagePath))
	{
		return nullptr;
	}

	TArray<uint8> CompressedData;
	if (!FFileHelper::LoadFileToArray(CompressedData, *ImagePath))
	{
		return nullptr;
	}
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::BMP);
	
	if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(CompressedData.GetData(), CompressedData.Num()))
	{
		TArray<uint8> UncompressedRGBA;

		if (ImageWrapper->GetRaw(ERGBFormat::RGBA, 8, UncompressedRGBA))
		{
			Texture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_R8G8B8A8);

			if (Texture != nullptr)
			{
				IsValid = true;

				OutWidth = ImageWrapper->GetWidth();
				OutHeight = ImageWrapper->GetHeight();

				void* TextureData = Texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
				FMemory::Memcpy(TextureData, UncompressedRGBA.GetData(), UncompressedRGBA.Num());
				Texture->PlatformData->Mips[0].BulkData.Unlock();
				Texture->UpdateResource();
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("valido"));
	return Texture;
}
