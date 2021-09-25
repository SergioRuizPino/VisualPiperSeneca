/*
*
* @author: Sergio Ruiz Pino
* @version : 0.1
*
*/


#include "AAvion.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

//Local del proyecto
bool colision = false;
int altura = 15;
FVector pos,girador;
FQuat rotar;
bool mover = true;
extern bool SimulacionActiva;
extern bool ModPosition;
extern bool activo;
extern bool ajustado;


AAAvion::AAAvion()
{

	PrimaryActorTick.bCanEverTick = true;
	this->brazo = CreateDefaultSubobject<USpringArmComponent>(TEXT("CMR"));
	this->Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camara principal"));
	this->caja = CreateDefaultSubobject<UBoxComponent>(TEXT("CREADA CAJA CHOQUE"));
	this->caja->SetWorldLocation(this->GetActorLocation());
	this->caja->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	this->brazo->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	this->brazo->TargetArmLength = 0.f;// 300.f;
	this->brazo->SetRelativeLocation(FVector(0, 0, 0));
	this->brazo->bEnableCameraLag = false;
	this->brazo->bEnableCameraRotationLag = false;
	this->Cam->AttachTo(this->brazo);
	this->caja->OnComponentBeginOverlap.AddDynamic(this, &AAAvion::OnBoxBeginOverlap);
}


void AAAvion::BeginPlay()
{
	Super::BeginPlay();
	colision = false;
	this->Camara = UGameplayStatics::GetPlayerController(this, 0); //Control de camara
	this->Camara->SetViewTargetWithBlend(this, 0.f,EViewTargetBlendFunction::VTBlend_Linear,0.f,false);
	this->BecomeViewTarget(Camara);
	this->SetActorLocation(pos);
	if (!activo) {
		pos.X = 0;
		pos.Y = 0;
		pos.Z = 40000;
	}
	else {
		pos.X = 250;
		pos.Y = 250;
		pos.Z = 2500;
	}
	
}


void AAAvion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	this->actualizarEstado();
	altura = this->DistanciaZ(this->GetActorLocation());

	//UE_LOG(LogTemp, Warning, TEXT("altura  %d"),altura); // debug

	
}

// Called to bind functionality to input
void AAAvion::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AAAvion::actualizarEstado() { //Actualizamos valores
		
	FVector movimiento;
	if (SimulacionActiva) {
		if (mover) {
			this->SetActorRelativeRotation(rotar);
			movimiento = rotar.GetForwardVector();
			movimiento.Z = 0;
			this->AddMovementInput(movimiento, 8000.5); //Aplicar movimiento seegún posicion
		}
		movimiento = this->GetActorLocation();
		movimiento.Z = pos.Z;
		this->SetActorLocation(movimiento);
	
	}

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
	return -1;
}

void  AAAvion::OnBoxBeginOverlap(UPrimitiveComponent* componenteChoque, AActor* ActorChoque, UPrimitiveComponent* componente, int32 indice, bool desde, const FHitResult& resultado) {
	FVector npos;
	if (!activo) {
		if (SimulacionActiva && !ModPosition) { //diferente choque segun cesium o terrenos
			UE_LOG(LogTemp, Warning, TEXT("CHOQUE"));
		//S	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "CHOQUE");
			colision = true;
			npos.X = 0;
			npos.Y = 0;
			npos.Z = 10000;
			this->SetActorLocation(npos);
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CHOQUE"));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "CHOQUE");
		colision = true;
		npos.X = 250;
		npos.Y = 250;
		npos.Z = 1000;
		this->SetActorLocation(npos);
	}
	
}