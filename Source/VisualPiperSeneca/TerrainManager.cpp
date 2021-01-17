// Fill out your copyright notice in the Description page of Project Settings.


#include "TerrainManager.h"


// Sets default values

extern FVector  pos;
//UE_LOG(LogTemp, Warning, TEXT("Encontro la fun: %s"), *pathProjectoTerrenos);
int aaa = 1;

ATerrainManager::ATerrainManager()
{
	TArray<FString> paths;
	IFileManager& FileManager = IFileManager::Get();
	FString pathProjectoTerrenos = FPaths::ProjectDir() + FString("terrenos/");

	const TCHAR* root = *(pathProjectoTerrenos);
	const TCHAR* extension = _T(".raw");
 	// Set this actor to call Tick() every frame.
	FileManager.FindFiles(paths, root, extension);

	for (int i = 0; i < paths.Num(); i++) {
//		UE_LOG(LogTemp, Warning, TEXT("Encontro la fun: %s"), *paths[i]);

		FString pp = paths[i];
		FString lf,lf2;
		FString rigaux;
		FString rig;

		pp.Split(",", &lf, &rigaux);
		rigaux.Split(".", &lf2, &rig);

		InsertarLimites(FCString::Atoi(*lf),FCString::Atoi(*lf2));

		pp.Split(".", &rigaux, &rig);

		//UE_LOG(LogTemp, Warning, TEXT("valor cadena cortada  %s "), *rigaux);

		//TerrenosActivos.Add(*paths[i], TPair<ATerrainGenerator*, bool>(nullptr, false));
		TerrenosActivos.Add(*rigaux, TPair<ATerrainGenerator*, bool>(nullptr, false));
	}


	PrimaryActorTick.bCanEverTick = true;
	paths.Empty();

}

// Called when the game starts or when spawned
void ATerrainManager::BeginPlay()
{
	Super::BeginPlay();
	
}


void ATerrainManager::EndPlay(EEndPlayReason::Type t) {
	//paths.Empty();
//	TerrenosActivos.Empty();

}


// Called every frame
void ATerrainManager::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	CreadorTerrenosTick();
	EliminadorTerrenosTick();
}

void ATerrainManager::CrearTerreno(FString fichero,int a,int b) {//crea terreno en a,b,0cambia su estado a activo 
	FVector Location(a, b, 0.0f);
	//FRotator Rotation(0.0f, 0.0f, 0.0f);
	const FTransform SpawnLocAndRotation(Location);
	FActorSpawnParameters SpawnInfo;
	//GetWorld()->SpawnActor<ATerrainGenerator>(Location, Rotation, SpawnInfo);
	ATerrainGenerator* ter = GetWorld()->SpawnActorDeferred<ATerrainGenerator>(ATerrainGenerator::StaticClass(), SpawnLocAndRotation,nullptr,nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	ter->setInicio(fichero);
	//ter->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	TerrenosActivos.Emplace(fichero, TPair<ATerrainGenerator*, bool>(ter, true));
	ter->FinishSpawning(SpawnLocAndRotation);
}

void ATerrainManager::InsertarLimites(int x, int y) {  // x del terreno, y del terreno debido a la división elegido (x,y)

		if (LimitesTerrenosSup.Contains(x)) {

			if (LimitesTerrenosSup[x] < y) {
				LimitesTerrenosSup.Emplace(x, y);
			}
		}
		else {
			LimitesTerrenosSup.Add(x, y);
		}

		if (LimitesTerrenosInf.Contains(x)) {

			if (LimitesTerrenosInf[x] > y) {
				LimitesTerrenosInf.Emplace(x, y);
			}
		}
		else {
			LimitesTerrenosInf.Add(x, y);
		}
	
}

void ATerrainManager::PrintMAP(){

	for (const TPair<int, int>& pair : LimitesTerrenosSup)
	{
		UE_LOG(LogTemp, Warning, TEXT("valor MAP terreno superior key %i valor %i"), pair.Key, pair.Value);

	}

	for (const TPair<int, int>& pair : LimitesTerrenosInf)
	{
		UE_LOG(LogTemp, Warning, TEXT("valor MAP terreno inferior key %i valor %i"), pair.Key, pair.Value);

	}
	/*for (const TPair<FString, TPair<ATerrainGenerator*, bool>>& pair : TerrenosActivos)
	{
		UE_LOG(LogTemp, Warning, TEXT("valor MAP Activos %s"), *pair.Key);
		pair.Value;
	}*/

	for (const TPair<FString, TPair<ATerrainGenerator*, bool>>& pair : TerrenosActivos)
	{
		UE_LOG(LogTemp, Warning, TEXT("valor MAP %s activo %i") ,*pair.Key, pair.Value.Value);
		//pair.Value;
	}

}

void ATerrainManager::EliminaTerrenosActivos(ATerrainGenerator* ter) { //elimina terreno segun distancia

	if (DistanciaTerrenosActivos(ter) >= 40000) {
		const  FString* fichero;
		fichero = TerrenosActivos.FindKey(TPair<ATerrainGenerator*, bool>(ter, true));
		FString f(*(fichero));
		TerrenosActivos.Emplace(f, TPair<ATerrainGenerator*, bool>(nullptr, false));
		if (!ter->Destroy()) {
			UE_LOG(LogTemp, Warning, TEXT("Un terreno inmortal !!!!!!"));
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Un terreno que muere"));
		}
	}
}

double ATerrainManager::DistanciaTerrenosActivos(ATerrainGenerator* ter) { //Determinamos terrenos activo y posicion, en base a esto eliminaremos los terrenos no necesarios
	double x = FMath::Square((pos.X - ter->GetActorLocation().X));
	double y = FMath::Square((pos.Y - ter->GetActorLocation().Y));
	return FMath::Sqrt((x + y));

}


double ATerrainManager::DistanciaTerrenosNoActivo(int tx,int ty) { //determinamos posición inactivo para crearlo
	double x = FMath::Square((pos.X - (tx*25000)));
	double y = FMath::Square((pos.Y - (ty*25000)));
//	UE_LOG(LogTemp, Warning, TEXT("calculo en funcion %i  %i %f"), tx, ty, FMath::Sqrt((x + y)));
	return FMath::Sqrt((x + y));

}


FString ATerrainManager::DevuelveTerrenoActualString(){ //Devuelve el terreno en que estamos actualmente en String
	int x = pos.X / 25000;
	int y = pos.Y / 25000;
	return FString((FString::FromInt(x)+","+FString::FromInt(y)+".raw"));
}

void ATerrainManager::DevuelveTerrenoActualInt(int32* x, int32* y) { //Devuelve el terreno en que estamos actualmente en enteros
	*x = (int32)(pos.X / 25000);
	*y = (int32)(pos.Y / 25000);
}

double ATerrainManager::DistanciaPuntoMedio(ATerrainGenerator* ter) { //Devuelve la distancia entre el avion y el punto medio de un terreno que le pasemos
		const  FString* fichero;
		fichero = TerrenosActivos.FindKey(TPair<ATerrainGenerator*, bool>(ter , true));
		FString f(*(fichero));

		return 1.2f;
}


void ATerrainManager::EliminadorTerrenosTick() {
	try {
		for (const TPair<FString, TPair<ATerrainGenerator*, bool>>& pair : TerrenosActivos)
		{

			//UE_LOG(LogTemp, Warning, TEXT("valor MAP %s activo %i"), *pair.Key, pair.Value.Value);
			if (pair.Value.Value) {//Si activo, solo tendremos en cuenta activados
				if (DistanciaTerrenosActivos(pair.Value.Key) >= 40000) { //Si distancia es limite
					EliminaTerrenosActivos(pair.Value.Key);
				}

			}
		}

	}
	catch (const std::exception e) {
		

	}
}

void ATerrainManager::CreadorTerrenosTick() {
	int32 actualX, actualY;
	DevuelveTerrenoActualInt(&actualX,&actualY);

	for (int i = actualX - 1; i <= actualX + 1; i++) {
		for (int j = actualY - 1; j <= actualY + 1; j++) {
			
			//if (i != actualX || j != actualX) {
			
				FString terreno(FString::FromInt(i) + "," + FString::FromInt(j));
				if (false){ //es limite





			
				}else {
					if (TerrenosActivos.Contains(terreno))
					{ //no es limite , debe contenerlo
					//	UE_LOG(LogTemp, Warning, TEXT("String %s"), *terreno);
						if (!TerrenosActivos[terreno].Value) 
						{
							//Si no esta creado se comprueba condicion para crear
							
							if (DistanciaTerrenosNoActivo(i,j) < 10000)
							{
					
								UE_LOG(LogTemp, Warning, TEXT("creado %i %i %i %i"), i,j,i*25000,j*25000);
									CrearTerreno(terreno, (i * 25000), (j * 25000));
							}
							/*if (DistanciaTerrenosActivos(TerrenosActivos[terreno].Key) < 15500) {
								CrearTerreno(terreno, actualX * 25000, actualY * 25000);
							}*/


						}

					}
				}
			//}

		}

	}
}

bool ATerrainManager::EsLimite(int tx, int ty) {
	bool limite = false;

	if (LimitesTerrenosSup.Contains(tx)) {
		if (LimitesTerrenosSup[tx] == ty) {
			limite = true;
		}

	}

	if (LimitesTerrenosInf.Contains(tx)) {
		if (LimitesTerrenosInf[tx] == ty) {
			limite = true;
		}
	}
	return limite;

}