/* 
*
* @author: Sergio Ruiz Pino
* @version : 0.1
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <thread>
#include "Math/UnrealMathUtility.h"
#include "EstructurasComunicacion.h"
#include "SocketConnection.generated.h"


#define paqueteMax 1472



UCLASS()
class VISUALPIPERSENECA_API ASocketConnection : public AActor
{
	GENERATED_BODY()
	
public:	
	ASocketConnection();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type) override;

private:
	FSocket* conexion;

	TSharedPtr<FInternetAddr> addr;
	TSharedPtr<FInternetAddr> addr2;

	//Flags request response
	bool flrw = false;
	bool flobj = false;
	bool flfog = false;
	bool flswit = false;
	bool fldayh = false;

	//Counter

	int ID_obj = 37;
	int ID_ref = 0;
	float latitud = 0;
	float lon = 0;
	float altitud = 0;
	char onoff;
	int estado = 0;
	

	//Funciones Internas
	void recibirDatosSocket();
	void enviarDatosSocket();
	void Descodificar(int bytesleidos);
	void Codificar();

	//Funciones Reque Des
	int setRway(RTX_rw_setup_req* rw);
	int setObjectSph(RTX_object_sph_req* sobj);
	int setSwitch(RTX_switch_req* swiobj);
	int setFog(RTX_fog_req* foj);
	int setDayH(RTX_time_of_day_req* dia);
	int setCloud(RTX_cloud_layer_req* nube);
	int setEye(RTX_eye_offset_req* ojo);
	int setRot(RTX_loc_rot_req* rot);
	int setGs(RTX_gswitch_req* gs);
	int setSky(RTX_sky_req* sky);
	int setTfb(RTX_tfb_vtx_req* tfb);
	int objaux(RTX_object_res* obj);
	int objRes(RTX_coll_res* res);
	//Funciones Reque Cod
	int setTerrainFed(int* length,uint8* punteroBufferCod);
	int setColision(int* length, uint8* punteroBufferCod);
	int setSearchRes(int* length, uint8* punteroBufferCod);
	int setupRW(int* length, uint8* punteroBufferCod);
	int setObjRes(int* length, uint8* punteroBufferCod);
	int AcknowledgeRes(int* length, uint8* punteroBufferCod);
  

	//FRunnableThread* ThreadEnvio = nullptr;
	//FRunnableThread* ThreadRecibido = nullptr;



};
