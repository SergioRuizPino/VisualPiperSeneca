// NO USADO EN LA VERSION FINAL att SERGIO RUIZ PINO


#include "TerrainThread.h"
#pragma region Main Thread Code

int64 a = 0;
extern FVector pos;
TerrainThread::TerrainThread(TArray<FVector>  Vertices, TArray<int> triangulos, TArray<FVector> VectorN, TArray<FVector2D> US, TArray<FLinearColor> vertex, TArray<FProcMeshTangent> Tange, UProceduralMeshComponent* ter)
{
	UE_LOG(LogTemp, Warning, TEXT(" Hilo Dios Creado"));
	this->Continua = true;
	this->VerticesMalla = Vertices;
	this->triangulosMalla = triangulos;
	this->VectorNormales = VectorN;
	this->UVS = US;
	this->vertexColors = vertex;
	this->Tangente = Tange;
	ancho = 500;
	alto = 500;
	indice = 0;
	Espaciotri = 25;
	this->cret = ter;
	this->HiloCreador = FRunnableThread::Create(this, TEXT("Creado Hilo"));
}

TerrainThread::~TerrainThread()
{
	this->HiloCreador->Kill();
	delete HiloCreador;
}


bool TerrainThread::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("INIT HILO"));
	FString pathProjectoTerrenos2 = FPaths::ProjectDir() + FString("terrenos/") + "0,0" + ".raw";
	const TCHAR* RootPath2 = *(pathProjectoTerrenos2);
	this->Continua = true;
	if (FFileHelper::LoadFileToArray(pruebaFileBInary, RootPath2, 0)) { //lee corectamente
		UE_LOG(LogTemp, Warning, TEXT("%u bits desde el hilo"), pruebaFileBInary.Num());
	}
	else { UE_LOG(LogTemp, Warning, TEXT("hilo muerto")); }

		return true;
}

void TerrainThread::Stop()
{
	UE_LOG(LogTemp, Warning, TEXT(" MUERO ME"));
	this->Continua = false;

}

uint32 TerrainThread::Run() {

	this->VerticesTriangulos();
	this->TriangulosAnadir();
	Continua = false;
//	cret->CreateMeshSection_LinearColor(0, VerticesMalla, triangulosMalla, VectorNormales, UVS, vertexColors, Tangente, true);
	UE_LOG(LogTemp, Warning, TEXT(" ME termine RUN"));
	return 0;

}

void TerrainThread::TriangulosAnadir() { //AÑADE LOS TRIANGULOS DE LA MALLA

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


void  TerrainThread::VerticesTriangulos()
{
	float prueba = 0.0f;
	float uvSpacing = 1.0f / FMath::Max(ancho, alto);
	uint16 datos = 0, datos2 = 0;

	for (int32 yy = 0; yy < alto; yy++)
	{
		for (int32 xx = 0; xx < ancho; xx++)
		{
			if (this->indice > (pruebaFileBInary.Num() / 2)) {
				datos = 0;
			}
			else {
				memcpy(&datos, &pruebaFileBInary[this->indice], 2); //buffer  a variable
				this->indice = this->indice + 2;
				//uint16 xxxx = (vvvv >> 8 | vvvv << 8);
				if (datos > 8849)
				{  //si altura mayor que el punto más alto tieera 8848 , altura = 0
					datos = 0;
				}

				if (datos <= -1)
				{  //si altura menor 0 , altura = 0
					datos = 0;
				}

			}

			VerticesMalla.Add(FVector(xx * Espaciotri, yy * Espaciotri, datos));
			VectorNormales.Add(FVector(0.0f, 0.0f, 1.0f));
			UVS.Add(FVector2D(xx * uvSpacing, yy * uvSpacing));
			vertexColors.Add(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
			Tangente.Add(FProcMeshTangent(1.0f, 0.0f, 0.0f));
		}
	}
}