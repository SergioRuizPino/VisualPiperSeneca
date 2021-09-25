/* 
*
* @author: Sergio Ruiz Pino
* @version : 0.1
*
*/


#include "TerrainManager.h"


// Sets default values

extern FVector  pos; // POS DEL AVION
extern bool activo; //Cesium Estado

ATerrainManager::ATerrainManager()
{
	
}

// Called when the game starts or when spawned
void ATerrainManager::BeginPlay()
{
	int distancias = 0;
	FString pp;
	FString lf, lf2;
	FString rigaux;
	FString rig;
	TArray<FString> paths,pathsxml;
	IFileManager& FileManager = IFileManager::Get();
	FString pathProjectoTerrenos = FPaths::ProjectDir() + FString("terrenos/");
	const TCHAR* root = *(pathProjectoTerrenos);
	const TCHAR* extension = _T(".raw");

	FileManager.FindFiles(paths, root, extension);


	for (int i = 0; i < paths.Num(); i++) {
		FXmlFile ficheroAuxiliar;
		pp = paths[i];

		pp.Split(",", &lf, &rigaux);
		rigaux.Split(".", &lf2, &rig);
		InsertarLimites(FCString::Atoi(*lf),FCString::Atoi(*lf2));
		rigaux = lf + "," + lf2;
		UE_LOG(LogTemp, Warning, TEXT("valor ter %s "), *rigaux);
		TerrenosActivos.Add(rigaux, TPair<ATerrainObj*, bool>(nullptr, false));
		this->CalcularPos(rigaux, FCString::Atoi(*lf), FCString::Atoi(*lf2));
		InsertarDatosTer(rigaux, FCString::Atoi(*lf), FCString::Atoi(*lf2));
	}

	paths.Empty();
	PrimaryActorTick.bCanEverTick = true;
	Super::BeginPlay();
	PrintMAP();
}


void ATerrainManager::EndPlay(EEndPlayReason::Type t) {
	TerrenosActivos.Empty();

}


// Called every frame
void ATerrainManager::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);
	if (activo) {
		this->CreadorTerrenosTick();
		this->EliminadorTerrenosTick();
	}
}

void ATerrainManager::CrearTerreno(FString fichero) {//crea terreno en a,b,0cambia su estado a activo 
	TPair<int,int>* posic = TerrenosPosReal.Find(fichero);
	FVector Location(0, 0, 0.0f);
	//FRotator Rotation(0.0f, 0.0f, 0.0f);
	const FTransform SpawnLocAndRotation(Location);
	FActorSpawnParameters SpawnInfo;
	//GetWorld()->SpawnActor<ATerrainGenerator>(Location, Rotation, SpawnInfo);
	ATerrainObj* ter = GetWorld()->SpawnActorDeferred<ATerrainObj>(ATerrainObj::StaticClass(), SpawnLocAndRotation,nullptr,nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	ter->setInicio(fichero);
    
	ter->SetActorLocation(FVector(posic->Key,posic->Value, 0));
	ter->AttachToComponent(this->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	TerrenosActivos.Emplace(fichero, TPair<ATerrainObj*, bool>(ter, true));
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

//Debug fun
void ATerrainManager::PrintMAP(){

	for (const TPair<int, int>& pair : LimitesTerrenosSup)
	{
		UE_LOG(LogTemp, Warning, TEXT("valor MAP terreno superior key %i valor %i"), pair.Key, pair.Value);

	}

	for (const TPair<int, int>& pair : LimitesTerrenosInf)
	{
		UE_LOG(LogTemp, Warning, TEXT("valor MAP terreno inferior key %i valor %i"), pair.Key, pair.Value);

	}
	for (const TPair<FString, TPair<int, int>>& pair : TerrenosPosReal)
	{
		UE_LOG(LogTemp, Warning, TEXT("valor MAP Activos %s  pos x   %d    pos y %d "), *pair.Key,pair.Value.Key,pair.Value.Value);
	}

	for (const TPair<FString, TPair<int, int>>& pair : Dimensiones)
	{
		UE_LOG(LogTemp, Warning, TEXT("Dimensiones %s  pos x   %d    pos y %d "), *pair.Key, pair.Value.Key, pair.Value.Value);
	}

	for (const TPair<FString, TPair<ATerrainObj*, bool>>& pair : TerrenosActivos)
	{

		UE_LOG(LogTemp, Warning, TEXT("valor MAP %s activo %i"), *pair.Key, pair.Value.Value);

	}

}

void ATerrainManager::EliminaTerrenosActivos(ATerrainObj* ter) { //elimina terreno segun distancia

		const  FString* fichero;
		fichero = TerrenosActivos.FindKey(TPair<ATerrainObj*, bool>(ter, true));
		FString f(*(fichero));
		TerrenosActivos.Emplace(f, TPair<ATerrainObj*, bool>(nullptr, false));
		if (!ter->Destroy()) {
			UE_LOG(LogTemp, Warning, TEXT("Un terreno indestructible")); 
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Un terreno que ha sido destruido"));
		}
}

double ATerrainManager::DistanciaTerrenosActivos(FString posicion) { //Determinamos terrenos activo y posicion, en base a esto eliminaremos los terrenos no necesarios
	TPair<int, int>* posic = TerrenosPosReal.Find(posicion);
	double x = FMath::Square((pos.X - posic->Key));
	double y = FMath::Square((pos.Y - posic->Value));
	return FMath::Sqrt((x + y));

}


double ATerrainManager::DistanciaTerrenosNoActivo(int tx,int ty) { //determinamos posición inactivo para crearlo
	FString terreno((FString::FromInt(tx) + "," + FString::FromInt(ty)));
	TPair<int, int>* posic = TerrenosPosReal.Find(terreno);
	double x = FMath::Square((pos.X - (posic->Key)));
	double y = FMath::Square((pos.Y - (posic->Value)));
	//UE_LOG(LogTemp, Warning, TEXT("calculo en funcion %i  %i %f"), posic->Key, posic->Value, FMath::Sqrt((x + y))); debug
	return FMath::Sqrt((x + y));

}


FString ATerrainManager::DevuelveTerrenoActualString(){ //Devuelve el terreno en que estamos actualmente en String DEPRECATED NO VALIDO
	int x = pos.X / 12500;
	int y = pos.Y / 12500;
	return FString((FString::FromInt(x)+","+FString::FromInt(y)+".raw"));
}

void ATerrainManager::DevuelveTerrenoActualInt(int32* x, int32* y) { //Devuelve el terreno en que estamos actualmente en enteros
	FString lf, rigaux,pp,auxiliar;
	TPair<int, int>* anterior;
	for (TPair<FString, TPair<int, int>>& pair : TerrenosPosReal)
	{
		anterior = Dimensiones.Find(pair.Key);

				if (pos.X >= pair.Value.Key && pos.X <= anterior->Key + pair.Value.Key) {   // + pair.Value.Key->DevolverAncho()
					if (pos.Y >= pair.Value.Value && pos.Y <= pair.Value.Value + anterior->Value) { // + + pair.Value.Key->DevolverLargo()

						pp = pair.Key;
						pp.Split(",", &lf, &rigaux);
						*x = FCString::Atoi(*lf);
						*y = FCString::Atoi(*rigaux);
						//UE_LOG(LogTemp, Warning, TEXT("ACTUAL %d %d"), *x, *y); debug
						return;

					}

				}
	}

}

double ATerrainManager::DistanciaPuntoMedio(ATerrainObj* ter) { //Devuelve la distancia entre el avion y el punto medio de un terreno que le pasemos 
	int xMediaT = (ter->DevolverXreal() + ((ter->DevolverAncho() * ter->DevolverDistancia()) / 2));
	int YMediaT = (ter->DevolverYreal() + ((ter->DevolverLargo() * ter->DevolverDistancia()) / 2));
	double x = FMath::Square((pos.X - xMediaT));
	double y = FMath::Square((pos.Y - YMediaT));
	return FMath::Sqrt((x + y)); //Punto medio	
}


void ATerrainManager::EliminadorTerrenosTick() {

		for (const TPair<FString, TPair<ATerrainObj*, bool>>& pair : TerrenosActivos)
		{

			if (pair.Value.Value) {//Si activo, solo tendremos en cuenta activados
				if (DistanciaTerrenosActivos(pair.Key) >= 60000) { //Si distancia es limite
					EliminaTerrenosActivos(pair.Value.Key);
				}

			}
		}


}

void ATerrainManager::CreadorTerrenosTick() {
	int32 actualX, actualY;
	DevuelveTerrenoActualInt(&actualX,&actualY);

	for (int i = actualX - 1; i <= actualX + 1; i++) {
		for (int j = actualY - 1; j <= actualY + 1; j++) {
			
			//if (i != actualX || j != actualX) {
			
				FString terreno(FString::FromInt(i) + "," + FString::FromInt(j));
				if (false){ //es limite,


			
				}else {
					if (TerrenosActivos.Contains(terreno))
					{ //no es limite , debe contenerlo

						if (!TerrenosActivos[terreno].Value) 
						{
							//Si no esta creado se comprueba condicion para crear
							
							if (DistanciaTerrenosNoActivo(i,j) < 50000)
							{
					
									CrearTerreno(terreno);
							}

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



void ATerrainManager::CalcularPos(FString fichero,int x,int y) {
	int realxx, realyy;
	if (x >= 0) {
		realxx = this->ajustarPosicionMapa(x,y); //x
		if (y >= 0) {
			realyy =this->ajustarPosicionMapaY(x,y);	//+x+y
		}
		else {
			realyy = this->ajustarPosicionMapaYNegativa(x,y);	//+x-y
		}
	}
	else {
		realxx = this->ajustarPosicionMapaNegativa(x,y);//-x
		if (y >= 0) {
			realyy = this->ajustarPosicionMapaY(x,y);	  //-x+y
		}
		else {//-x-y
			realyy = this->ajustarPosicionMapaYNegativa(x,y);
		}
	}
	TerrenosPosReal.Add(fichero, TPair<int, int>(realxx, realyy));
}


int ATerrainManager::ajustarPosicionMapa(int x, int y) {
	int aux = 0;
	for (int i = 0; i < x; i++) { //quitar igual
		FXmlFile ficheroAuxiliar;
		ficheroAuxiliar.LoadFile(FPaths::ProjectDir() + FString("terrenos/") + FString::FromInt(i) + "," + FString::FromInt(y) + ".xml", EConstructMethod::ConstructFromFile); //fichaload

		if (ficheroAuxiliar.IsValid()) {
			aux = aux + lecturaXmlAncho(ficheroAuxiliar.GetRootNode());
		}
		else {
			aux = aux + 1000* 1;
		}

	}
	return aux;
}

int ATerrainManager::ajustarPosicionMapaNegativa(int x, int y) {
	int aux = 0;
	for (int i = 0; i > x; i--) { //quitar igual
		FXmlFile ficheroAuxiliar;
		ficheroAuxiliar.LoadFile(FPaths::ProjectDir() + FString("terrenos/") + FString::FromInt(i) + "," + FString::FromInt(y) + ".xml", EConstructMethod::ConstructFromFile); //fichaload

		if (ficheroAuxiliar.IsValid()) {
			aux = aux - lecturaXmlAncho(ficheroAuxiliar.GetRootNode());
		}
		else {
			aux = aux - 1000 * 1;
		}

	}
	return aux;
}



int ATerrainManager::ajustarPosicionMapaY(int x, int y) {
	int aux = 0;
	for (int i = 0; i < y; i++) { //quitar igual
		FXmlFile ficheroAuxiliar;
		ficheroAuxiliar.LoadFile(FPaths::ProjectDir() + FString("terrenos/") + FString::FromInt(x) + "," + FString::FromInt(i) + ".xml", EConstructMethod::ConstructFromFile); //fichaload

		if (ficheroAuxiliar.IsValid()) {
			aux = aux + lecturaXmlLargo(ficheroAuxiliar.GetRootNode());
		}
		else {
			aux = aux + 1000 * 1;
		}

	}
	return aux;
}

int ATerrainManager::ajustarPosicionMapaYNegativa(int x,int y) {
	int aux = 0;
	for (int i = 0; i > y; i--) { //quitar igual
		FXmlFile ficheroAuxiliar;
		ficheroAuxiliar.LoadFile(FPaths::ProjectDir() + FString("terrenos/") + FString::FromInt(x) + "," + FString::FromInt(i) + ".xml", EConstructMethod::ConstructFromFile); //fichaload

		if (ficheroAuxiliar.IsValid()) {
			aux = aux - lecturaXmlLargo(ficheroAuxiliar.GetRootNode());
		}
		else {
			aux = aux - 1000 * 1;
		}

	}
	return aux;
}



int ATerrainManager::lecturaXmlAncho(const FXmlNode* ficher) { //generico no hijos mas comodo < </> </> </> />
	int aux = 0, aux2 = 0,aux3 = 0;
	if (ficher != nullptr)
	{
		const FXmlNode* nodee = ficher->FindChildNode("ANCHO");
		if (nodee != nullptr) {
			aux = FCString::Atoi(*nodee->GetContent());

		}
		else {
			aux = 1000;
		}

		const FXmlNode* nodee2 = ficher->FindChildNode("DISTANCIA");
		if (nodee2 != nullptr) {
			aux2 = FCString::Atoi(*nodee2->GetContent());
		}
		else {
			aux2 = 1;
		}

		const FXmlNode* nodee3 = ficher->FindChildNode("LANCHO");
		if (nodee3 != nullptr) {
			aux3 = FCString::Atoi(*nodee3->GetContent());
		}
		else {
			aux3 = 1;
		}

		return (aux * aux2*	aux3);
	}
	else {
		return 0;
	}
}

int ATerrainManager::lecturaXmlLargo(const FXmlNode* ficher) { //generico no hijos mas comodo < </> </> </> />
	int aux = 0, aux2 = 0,aux3 = 0;
	if (ficher != nullptr)
	{
		const FXmlNode* nodee = ficher->FindChildNode("ALTO");
		if (nodee != nullptr) {
			aux = FCString::Atoi(*nodee->GetContent());
		}
		else {
			aux = 1000;
		}

		const FXmlNode* nodee2 = ficher->FindChildNode("DISTANCIA");
		if (nodee2 != nullptr) {
			aux2 = FCString::Atoi(*nodee2->GetContent());
		}
		else {
			aux2 = 1;
		}

		const FXmlNode* nodee3 = ficher->FindChildNode("LLARGO");
		if (nodee3 != nullptr) {
			aux3 = FCString::Atoi(*nodee3->GetContent());
		}
		else {
			aux3 = 1;
		}

		return (aux * aux2* aux3);
	}
	else {
		return 0;
	}
}


void ATerrainManager::InsertarDatosTer(FString fichero, int x, int y) {
	FXmlFile ficheroAuxiliar;
	int xx, yy;
	ficheroAuxiliar.LoadFile(FPaths::ProjectDir() + FString("terrenos/") + FString::FromInt(x) + "," + FString::FromInt(y) + ".xml", EConstructMethod::ConstructFromFile); //fichaload

	if (ficheroAuxiliar.IsValid()) {
		xx = lecturaXmlAncho(ficheroAuxiliar.GetRootNode());
	}
	else {
		xx = 1000;
	}

	if (ficheroAuxiliar.IsValid()) {
		yy = lecturaXmlLargo(ficheroAuxiliar.GetRootNode());
	}
	else {
		yy = 1000;
	}
	this->Dimensiones.Add(fichero,TPair<int, int>(xx,yy));
}


//X ANCHO Y LARGO