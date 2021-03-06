/*
*
* @author: Sergio Ruiz Pino
* @version : 0.1
*
*/


#include "AAvion.h"
#include "Kismet/GameplayStatics.h"

//Local del proyecto
bool colision = false;
int altura = 15;
int ii = 25;
FVector pos,girador;
FQuat rotar;

AAAvion::AAAvion()
{
	PrimaryActorTick.bCanEverTick = true;
	this->brazo = CreateDefaultSubobject<USpringArmComponent>(TEXT("CMR"));
	this->Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camara principal"));
	this->brazo->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	
	this->brazo->TargetArmLength = 300.f;
	this->brazo->SetRelativeLocation(FVector(0, 0, 0));
	this->brazo->bEnableCameraLag = false;
	this->brazo->bEnableCameraRotationLag = false;
	this->Cam->AttachTo(this->brazo);
}


void AAAvion::BeginPlay()
{
	Super::BeginPlay();
	colision = false;
	this->Camara = UGameplayStatics::GetPlayerController(this, 0); //Control de camara
	this->Camara->SetViewTargetWithBlend(this, 0.f,EViewTargetBlendFunction::VTBlend_Linear,0.f,false);
	this->BecomeViewTarget(Camara);
}


void AAAvion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	this->actualizarEstado();
	altura = this->DistanciaZ(this->GetActorLocation());
	//UE_LOG(LogTemp, Warning, TEXT("altura  %d"),altura);
	//if (altura >= this->GetActorLocation().Z) { colision = true; }

}

// Called to bind functionality to input
void AAAvion::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AAAvion::actualizarEstado() { //Actualizamos valores
		this->SetActorLocation(pos);
		this->SetActorRelativeRotation(rotar);
}

float AAAvion::DistanciaZ(FVector actual)
{
	UWorld* Pantalla{ this->GetWorld() };  //Instancia de la escena actual

	if (Pantalla)//instacia activa
	{
		FVector Avion{ actual.X, actual.Y, actual.Z };  
		FVector Suelo{ actual.X, actual.Y, 0 };           
		FHitResult col; //creamos una colision
		Pantalla->LineTraceSingleByObjectType(OUT col,Avion,Suelo,FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic),FCollisionQueryParams());
		if (col.GetActor()) return col.ImpactPoint.Z; //Altura donde se produjo colision
	}
	return 0;
}
