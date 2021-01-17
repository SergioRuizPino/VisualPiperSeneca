/* 
*
* @author: Sergio Ruiz Pino
* @version : 0.1
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <thread>
#include "SocketConnection.generated.h"

#define paqueteMax 1472
#define MAX_NUM_TFB_VTX 12
#define MAX_NUM_SEARCH_RAYS 6


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
	void recibirDatosSocket();
	void enviarDatosSocket();
	//FRunnableThread* ThreadEnvio = nullptr;
	//FRunnableThread* ThreadRecibido = nullptr;


	//Estructuras que se envian a DINAMICS
	typedef struct
	{
		unsigned char opcode;    // 01 hex
		unsigned char length;
		unsigned char obj_id;
		unsigned char error;

	} RTX_object_res;

	typedef struct
	{
		unsigned char opcode;    // 02 hex
		unsigned char length;
		unsigned char obj_id;
		unsigned char spare1;
		unsigned char flags;           // Bit 7: set for valid hit_id
		unsigned char terrain_type;
		unsigned char hit_obj_id;
		unsigned char spare2;

	} RTX_coll_res;

	typedef struct
	{
		unsigned char flags;
		unsigned char terrain_type;
		unsigned char hit_obj_id;
		unsigned char spare;
		float         distance;

	} RTX_search_data;

	typedef struct
	{
		unsigned char opcode;    // 07 hex
		unsigned char length;
		unsigned char obj_id;
		unsigned char num_rays;
		RTX_search_data   set[MAX_NUM_SEARCH_RAYS];

	} RTX_search_res;


	typedef struct
	{
		unsigned char opcode;    //
		unsigned char length;
		unsigned char cmd;
		unsigned char id;
	} RTX_Acknowledge_res;


	typedef struct
	{
		unsigned char terrain_type;
		unsigned char spare[3];
		float         height;

	} RTX_tfb_data;

	typedef struct
	{
		unsigned char opcode;    // 04 hex
		unsigned char length;    // 1 + 2*nb_vtx
		unsigned char obj_id;    // 0 for Host
		unsigned char nb_vtx;    // Max of 12
		RTX_tfb_data  set[MAX_NUM_TFB_VTX];

	} RTX_tfb_res;

	typedef struct
	{
		unsigned char opcode;    /* 24 hex */
		unsigned char length;
		unsigned char ref_id;    /* Echo of ref_id in request */
		unsigned char spare1;

	} RTX_loc_cframe_res;

	typedef struct
	{
		unsigned char opcode;    /* 26 hex */
		unsigned char length;
		unsigned char obj_id;
		unsigned char err_ref_id; /* If NOT ERR_BAD_OBJECT, then
									 echo of ref_id in request */

	} RTX_rw_setup_res;

	typedef struct
	{
		unsigned char opcode;    /* 27 hex */
		unsigned char length;
		unsigned char channel_no;
		unsigned char ref_id;    /* Echo of ref_id in request */

	} RTX_eye_offset_res;

	typedef struct
	{
		unsigned char   opcode;    /* SWITCH_CODE */
		unsigned char   length;
		unsigned short  sw_id_state;

	} RTX_switch_res;

	typedef struct
	{
		unsigned char   opcode;    /* GSWITCH_CODE */
		unsigned char   length;
		unsigned short  sw_id_state;

	} RTX_gswitch_res;

	typedef struct
	{
		unsigned char opcode;    /* LOCAL_ROT_CODE */
		unsigned char length;
		unsigned char ref_id;    /* Echo of ref_id in request */
		unsigned char spare;

	} RTX_loc_rot_res;

	typedef struct
	{
		unsigned char opcode;    /* GLOCAL_ROT_CODE */
		unsigned char length;
		unsigned char ref_id;    /* Echo of ref_id in request */
		unsigned char spare;

	} RTX_gloc_rot_res;

	typedef struct
	{
		unsigned char   opcode;    /* 2A hex */
		unsigned char   length;
		unsigned char   spare;
		unsigned char   sky_id;    /* Echo of sky_id in request */

	} RTX_sky_res;

};
