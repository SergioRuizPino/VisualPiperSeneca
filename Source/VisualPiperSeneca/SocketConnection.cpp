/* Fill out your copyright notice in the Description page of Project Settings.
*
* @author: Sergio Ruiz Pino
* @version : 0.1
*
*/

#include "SocketConnection.h"
#include "Misc/MessageDialog.h"
#include "Sockets.h"
#include <Runtime\Sockets\Public\SocketSubsystem.h>
#include "GenericPlatform/GenericPlatformProcess.h"
#include <Runtime\Networking\Public\Interfaces\IPv4\IPv4Address.h>
#include "Common/UdpSocketBuilder.h"
#include "Common/UdpSocketReceiver.h"
#include "HAL/RunnableThread.h"
#include "Kismet/GameplayStatics.h"


uint8 RTX_DATA_RECIBIDO[paqueteMax];  //Bus de datos Recibir
uint8 RTX_DATA_ENVIO[paqueteMax];  //Bus de datos Enviar
extern bool colision;
extern FVector pos;
extern FQuat rotar;
extern int altura;
extern float niebla;
extern float hora;
extern float nubeht;
extern float nubeb;
float antigualon = 0 , antigualat = 0;
int a = 0;


// Sets default values
ASocketConnection::ASocketConnection()
{

	for (int i = 0; i < 1472; i++) {
		RTX_DATA_ENVIO[i] = 0;  //Iniciar Buffer de envio 
	}
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASocketConnection::BeginPlay()
{
	Super::BeginPlay();

	int pMax = paqueteMax;
	bool bCanBindAll = true;
	FIPv4Address posSocket(192, 128, 134, 184); //ip
	FIPv4Address posSocket2(192, 128, 134, 188); //ip

	//addr2 = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr(posSocket2.Value, 12000);
	//addr2->SetIp(posSocket2.Value);
	//addr2->SetPort(12000);//Puerto de conexion
	addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBindAll);
	//addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr(posSocket.Value, 12000);
	addr->SetIp(posSocket.Value);
	addr->SetPort(12000);//Puerto de conexion
	//FIPv4Endpoint Endpoint(posSocket2.Value, 12000);
	FUdpSocketBuilder wse("Creador UDP");
	this->conexion = wse.Build();

	if (this->conexion == nullptr) { 
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("Error ,revise los datos de conexion,se cerrara  Visual."));
			UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
		
		}

		if (!conexion->SetBroadcast(true)) { }
		if (!conexion->SetReuseAddr(true)) { }
		if (!conexion->SetNonBlocking(true)) { }
		conexion->SetRecvErr(true);
		conexion->SetReceiveBufferSize(paqueteMax, pMax);
		conexion->SetSendBufferSize(paqueteMax, pMax);
	
		if (this->conexion->Bind(*addr)) { UE_LOG(LogTemp, Warning, TEXT("")); }

		
}

void ASocketConnection::EndPlay(EEndPlayReason::Type) {
	this->conexion->Close();
	ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(this->conexion);
}

// Called every frame
void ASocketConnection::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	this->recibirDatosSocket();
	this->enviarDatosSocket();
	
	
}

//Metodo que permite escuchar datos del socket luego descodifica  para ser entendible
void ASocketConnection::recibirDatosSocket() {
	uint32 aux;
	int32 bytesleidos;
	if (this->conexion->HasPendingData(aux)) //Hay Datos?
	{
		if (this->conexion->RecvFrom(RTX_DATA_RECIBIDO, paqueteMax, bytesleidos, *addr)) { 
			this->Descodificar(bytesleidos);

		}else {
			return;
		}
	}
}

//Metodo que permite enviar datos al socket, primero se codifica para que dynamics entienda
void ASocketConnection::enviarDatosSocket() {
	int32 bytesEnviados;
	this->Codificar();
	if (this->conexion->SendTo(RTX_DATA_ENVIO, paqueteMax, bytesEnviados, *addr)) { //enviado correctamente
		
		return;
		}
		else {
	   //No se envio
		}

}


void ASocketConnection::Codificar() {   //Codifica los datos antes de enviar a Dynamics 

	uint8* punteroBufferCod;
	int  length = 0;
	RTX_header cabezeraEnvio;

	//Cabezera
	punteroBufferCod = &RTX_DATA_ENVIO[0];    //apuntamos a inicio buffer           
	punteroBufferCod = punteroBufferCod + sizeof(RTX_header); //movemos lo correspodiente a la cabecera

	//Respondemos segun request
	punteroBufferCod += this->setupRW(&length, punteroBufferCod) * 4; //Enviar Siempre Sino no rula
	punteroBufferCod += this->setObjRes(&length, punteroBufferCod) * 4;
	punteroBufferCod += this->AcknowledgeRes(&length, punteroBufferCod) * 4;

	
	//Cod
	if (SimulacionActiva) {
			punteroBufferCod += this->setTerrainFed(&length, punteroBufferCod) * 4; //Multiplicar por 4,DYNAMICS lo hace asi
		    punteroBufferCod += this->setColision(&length, punteroBufferCod) * 4; //Comprobar Siempre
		  //  punteroBufferCod += this->setSearchRes(&length, punteroBufferCod) * 4; // SEARCH_RES_CODE
	}

	cabezeraEnvio.status = 21; //21 Visual To Dynamics ==> OK
	cabezeraEnvio.length = length;
	punteroBufferCod = &RTX_DATA_ENVIO[0];
	memcpy(punteroBufferCod, &cabezeraEnvio, sizeof(RTX_header));
}


//Envio a Dynamics Set datos
int ASocketConnection::setTerrainFed(int* length, uint8* punteroBufferCod) {
	RTX_tfb_res tfb_res2;
	int longitud = 0;
	tfb_res2.opcode = TFB_RES_CODE;
	tfb_res2.obj_id = 0; //HOST_ID = 0
	tfb_res2.nb_vtx = 1;
	RTX_tfb_data prueba;
	prueba.height = altura; //Altura
	tfb_res2.set[0] = prueba;
	tfb_res2.length = sizeof(RTX_tfb_res);
	memcpy(punteroBufferCod, &tfb_res2, sizeof(RTX_tfb_res));
	longitud = sizeof(RTX_tfb_res);
	*length = *length + longitud;
	return longitud;
}

int ASocketConnection::setColision(int* length, uint8* punteroBufferCod) { //FUNCIONA
	int longitud = 0;
	RTX_coll_res col_res;
	if (colision) { //detectada colisión
		col_res.opcode = COLL_RES_CODE;
		col_res.length = sizeof(RTX_coll_res);
		col_res.flags = 0x80; // bit 7 set
		memcpy(punteroBufferCod, &col_res, sizeof(RTX_coll_res));
		this->SimulacionActiva = false; //dejarlo en estado de espera
		longitud = sizeof(RTX_coll_res);
		*length = *length + longitud;
	}
	return longitud;
}

int  ASocketConnection::setSearchRes(int* length, uint8* punteroBufferCod) {
	int longitud = 0;
	RTX_search_res res;
	res.opcode = SEARCH_RES_CODE;
	longitud = sizeof(RTX_search_res);
	res.length = longitud;
	memcpy(punteroBufferCod, &res, sizeof(RTX_search_res));
	*length = *length + longitud;
	return longitud;
}

int  ASocketConnection::setObjRes(int* length, uint8* punteroBufferCod) {
	int longitud = 0;
	this->flobj = false;
	RTX_object_res resp;
	resp.obj_id = this->ID_obj;
	resp.error = 0;
	resp.opcode = OBJECT_CODE;
	longitud = sizeof(RTX_object_res);
	memcpy(punteroBufferCod, &resp,sizeof(RTX_object_res));
	*length = *length + longitud;
	return longitud;
}

int ASocketConnection::AcknowledgeRes(int* length, uint8* punteroBufferCod) {
	RTX_Acknowledge_res Acko;
	Acko.opcode = ACKNOWLEDGE_RESPONSE;
	Acko.length = sizeof(RTX_Acknowledge_res);
	*length = *length + sizeof(RTX_Acknowledge_res);
	memcpy(punteroBufferCod, &Acko, sizeof(RTX_Acknowledge_res));
	return sizeof(RTX_Acknowledge_res);
}

int ASocketConnection::setupRW(int* length, uint8* punteroBufferCod) {
	RTX_rw_setup_res rwsetup;
	this->flrw = false;
	rwsetup.opcode = RWAY_SETUP_CODE;
	rwsetup.obj_id = 1;
	rwsetup.err_ref_id = this->ID_ref;
	rwsetup.length = sizeof(RTX_rw_setup_res);
	*length = *length + sizeof(RTX_rw_setup_res);
	memcpy(punteroBufferCod, &rwsetup, sizeof(RTX_rw_setup_res));
	return sizeof(RTX_rw_setup_res);
}

//des
void ASocketConnection::Descodificar(int bytesleidos) { //Recibimos y enviamos datos a la visualización tras descodificar el envio
	if (bytesleidos>0)
	{
		
		RTX_header* Cabecera;
		int length = 0;
		uint8* punteroBufferDes = &RTX_DATA_RECIBIDO[0];
		Cabecera = (RTX_header*)punteroBufferDes;
		punteroBufferDes +=  sizeof(RTX_header);

		if ((int)Cabecera->status) {  //0 = activa, 1 =  inactiva  //Si paramos desde IOS o choque, cabecera cambia
			SimulacionActiva = false;
		}
		else {
			SimulacionActiva = true;
		}

		while (length < Cabecera->length)
		{
		 switch ((char)*punteroBufferDes) { //según contenido de lo que llegue
		    case RWAY_SETUP_CODE: length = length + this->setRway((RTX_rw_setup_req*)punteroBufferDes); punteroBufferDes = punteroBufferDes + sizeof(RTX_rw_setup_req) ; break;
			case OBJECT_SPH_CODE: length = length + this->setObjectSph((RTX_object_sph_req*)punteroBufferDes); punteroBufferDes = punteroBufferDes + sizeof(RTX_object_sph_req) ; break;
			case SWITCH_CODE: length = length + this->setSwitch((RTX_switch_req*)punteroBufferDes); punteroBufferDes = punteroBufferDes + sizeof(RTX_switch_req) ; break;
			case FOG_REQUEST: length = length + this->setFog((RTX_fog_req*)punteroBufferDes); punteroBufferDes = punteroBufferDes + sizeof(RTX_fog_req) ; break;
			case TIME_OF_DAY_REQUEST: length = length + this->setDayH((RTX_time_of_day_req*)punteroBufferDes); punteroBufferDes = punteroBufferDes + sizeof(RTX_time_of_day_req) ; break;
			case CLOUD_LAYER_REQUEST: length = length + this->setCloud((RTX_cloud_layer_req*)punteroBufferDes); punteroBufferDes = punteroBufferDes + sizeof(RTX_cloud_layer_req) ; break;
			case EYE_OFFSET_CODE: length = length + this->setEye((RTX_eye_offset_req*)punteroBufferDes); punteroBufferDes = punteroBufferDes + sizeof(RTX_eye_offset_req); break;
			case GSWITCH_CODE: length = length + this->setGs((RTX_gswitch_req*)punteroBufferDes); punteroBufferDes = punteroBufferDes + sizeof(RTX_gswitch_req); break;
			case LOCAL_ROT_CODE: length = length + this->setRot((RTX_loc_rot_req*)punteroBufferDes); punteroBufferDes = punteroBufferDes + sizeof(RTX_loc_rot_req) ; break;
			case SKY_CODE:  length = length + this->setSky((RTX_sky_req*)punteroBufferDes); punteroBufferDes = punteroBufferDes + sizeof(RTX_sky_req); break;
			case TFB_VTX_CODE: length = length + this->setTfb((RTX_tfb_vtx_req*)punteroBufferDes); punteroBufferDes = punteroBufferDes + 16; break;
			case OBJECT_CODE: length = length + this->objaux((RTX_object_res*)punteroBufferDes); punteroBufferDes = punteroBufferDes + sizeof(RTX_object_res); break;
			case COLL_RES_CODE : length = length + this->objRes((RTX_coll_res*)punteroBufferDes); punteroBufferDes = punteroBufferDes + sizeof(RTX_coll_res); break;
		    default: *punteroBufferDes = *punteroBufferDes + 1;length = length+1; break;
			}
		}

	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("NO RESPUESTA VALIDA  "));
	}

}

//Set datos Recibidos
int ASocketConnection::setRway(RTX_rw_setup_req* rw) {
	this->flrw = true;
	this->latitud = rw->lat;
	this->lon = rw->lon;
	this->onoff = rw->on_off;
	this->altitud = rw->ht; 
	ID_ref = rw->ref_id;
	//UE_LOG(LogTemp, Warning, TEXT("respuesta  rw  %d"),rw->ref_id);
	return (int)rw->length;
}


int ASocketConnection::setObjectSph(RTX_object_sph_req* sobj) {
	FRotator girador;
	FQuat quaternion;
	this->flobj = true;
	this->ID_obj = sobj->obj_id;
	//UE_LOG(LogTemp, Warning, TEXT(" objectsphr  obj id %i yaw %f altura  %f lat %f lon %f  pitch %f roll %f   %i  %i"),sobj->obj_id, sobj->h,sobj->ht,sobj->lat,sobj->lon,sobj->p,sobj->r,sobj->length,sizeof(RTX_object_sph_req));
	/*if (sobj->lon > antigualon) { pos.X = pos.X - 2; }
	if (sobj->lat > antigualat) { pos.Y = pos.Y - 2; }
	if (sobj->lon < antigualon) { pos.X = pos.X + 2; }
	if (sobj->lat < antigualat) { pos.Y = pos.Y + 2; }*/
	//MIRAR GET_EARTH_CENTRE_CART EN DYNAMICS
	

	if (sobj->obj_id == HOST_ID) {  //POSICION AVION
	//	pos.X = 15000 * cos(sobj->lat) * cos(sobj->lon);//sobj->lon; //NOPE CORREGIR MEJOR
		//pos.Y = 15000 * cos(sobj->lat) * sin(sobj->lon);//sobj->lat;    R cos(lat) sin(lon)
		pos.Z = sobj->ht;

		antigualat = sobj->lat;
		antigualon = sobj->lon;




		
		girador = FRotator(sobj->p, sobj->h, sobj->r); 	//h heading z p pitcth eje y , R roll eje x,	//ht altitud
		quaternion = FQuat(girador);
		rotar = quaternion;
	}
	return (int)sobj->length;
}

int ASocketConnection::setSwitch(RTX_switch_req* swiobj) {
	this->flswit = true;
	//UE_LOG(LogTemp, Warning, TEXT("switch  %d "), swiobj->sw_id_state);
	return (int)swiobj->length;
}


int ASocketConnection::setTfb(RTX_tfb_vtx_req* tfb) {
	//UE_LOG(LogTemp, Warning, TEXT("tfb  %d   %d "), tfb->length,tfb->obj_id);
	return (int)tfb->length;
}

int ASocketConnection::setSky(RTX_sky_req* sky) {
	//UE_LOG(LogTemp, Warning, TEXT("sky "));
	return sky->length;
}
int ASocketConnection::setCloud(RTX_cloud_layer_req* nube) {
	UE_LOG(LogTemp, Warning, TEXT("CLOUDY %d  %d "),nube->base_ht,nube->thickness);  
	nubeb = nube->base_ht;
	nubeht = nube->thickness;
	return (int)nube->length;
}

int ASocketConnection::setEye(RTX_eye_offset_req* ojo) {
//	UE_LOG(LogTemp, Warning, TEXT("OJO   x  %f    y %f   z  %f"), ojo->x,ojo->y,ojo->z);
	return (int)ojo->length;
}

int ASocketConnection::setFog(RTX_fog_req* foj) {
	this->flfog = true;
	niebla = foj->vis;
	UE_LOG(LogTemp, Warning, TEXT("niebla %f spare %d  flag %d"), foj->vis,(int)foj->spare,(int)foj->flags);
	return(int)foj->length;

}

int ASocketConnection::setDayH(RTX_time_of_day_req* dia) {
	this->fldayh = true;
	hora = dia->time;
	UE_LOG(LogTemp, Warning, TEXT("hora  %f "), dia->time);
	return (int)dia->length;
}

int ASocketConnection::setRot(RTX_loc_rot_req* rot) {
	//UE_LOG(LogTemp, Warning, TEXT("rot  %f "), rot->r);
	return (int)rot->length;
}

int ASocketConnection::setGs(RTX_gswitch_req* gs) {
//	UE_LOG(LogTemp, Warning, TEXT("gs  %i "), gs->sw_id_state);
	return (int)gs->length*3;
}

int ASocketConnection::objaux(RTX_object_res* obj) {
//	UE_LOG(LogTemp, Warning, TEXT("objs  %i "), obj->length);
	return obj->length;
}

int ASocketConnection::objRes(RTX_coll_res* res) {


	return res->length;
}