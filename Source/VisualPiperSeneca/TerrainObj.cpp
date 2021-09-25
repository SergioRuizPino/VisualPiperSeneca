/*
*
* @author: Sergio Ruiz Pino
* @version : 0.1
*
*/


#include "TerrainObj.h"
#include "Components/BoxComponent.h"
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



	this->caja = CreateDefaultSubobject<UBoxComponent>(TEXT("CREADA CAJA CHOQUE"));
	this->caja->SetWorldLocation(this->GetActorLocation());
	this->caja->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	this->caja->OnComponentBeginOverlap.AddDynamic(this, &ATerrainObj::OnBoxBeginOverlap);
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
		UE_LOG(LogTemp, Warning, TEXT("%u bits cargados del fichero"), pruebaFileBInary.Num());
	}
	else { //sino es valido todo por defecto a 0
		UE_LOG(LogTemp, Warning, TEXT("No valido fichero"));
	}

	//Guardamos posición x,y
	pp.Split(",", &lf, &rigaux);
	this->x = FCString::Atoi(*lf);
	this->y = FCString::Atoi(*rigaux);
	//this->hilo = new TerrainThread(VerticesMalla, triangulosMalla, VectorNormales, UVS, vertexColors, Tangente,this->Terreno);
}

void ATerrainObj::setFichero(FString fiche) {
	this->ficheroReferencia = fiche;
	//proteger si fiche = null o no cumple con mascara x,y
}

void ATerrainObj::TriangulosAnadir() { //AÑADE LOS TRIANGULOS DE LA MALLA 

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


	this->indice = 0;
	

}


void ATerrainObj::PostLoad()
{

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

void  ATerrainObj::OnBoxBeginOverlap(UPrimitiveComponent* componenteChoque, AActor* ActorChoque, UPrimitiveComponent* componente, int32 jind, bool desde, const FHitResult& resultado) {
	FVector npos; // ya lo hace avion
	//UE_LOG(LogTemp, Warning, TEXT("CHOQUE terreno ")); lo hace el avion
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "CHOQUE terreno");
	//colision = true;
	//npos.X = 0;
	//npos.Y = 0;
	//npos.Z = 10000;
	//this->SetActorLocation(npos);

}