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
#include <Runtime\Networking\Public\Interfaces\IPv4\IPv4Address.h>
#include "HAL/RunnableThread.h"
#include "Kismet/GameplayStatics.h"


int32 aux2 = 0;
uint8 RTX_DATA_RECIBIDO[paqueteMax];  //Bus de datos Recibir
uint8 RTX_DATA_ENVIO[paqueteMax];  //Bus de datos Enviar
int* puntero =(int*) &RTX_DATA_RECIBIDO;
int32 bytesleidos;
bool connected;

// Sets default values
ASocketConnection::ASocketConnection()
{
	//Hilo->Create(&ASocketConnection::recibirDatosSocket,"Hil");
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//ThreadEnvio->Create(recibirDatosSocket(), TEXT("FPrimeNumberWorker"));
	FIPv4Address posSocket(192, 168, 1, 62); //ip
	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	addr->SetIp(posSocket.Value);
	addr->SetPort(13);//Puerto de conexion
	this->conexion = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), true);
	connected = this->conexion->Connect(*addr);
	
}

// Called when the game starts or when spawned
void ASocketConnection::BeginPlay()
{

	Super::BeginPlay();
	for (int i = 0; i < 1400; i++) {
		RTX_DATA_ENVIO[i] = i;
	}
	if (!connected) { //Sino no nos conectamos, no podemos funcionar ,DESCOMENTAR interior TRAS PRUEBAS
		//FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("No se ha podido realizar la conexion con Dynamics, revise los datos de conexion,se cerrara  Visual."));
		//UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
		//GIsRequestingExit = 1; Esta intruccion en el constructor cierra el editor, ademas es una instruccion sucia.
	}
	
}

void ASocketConnection::EndPlay(EEndPlayReason::Type) {
	connected = false;
	bytesleidos = 0;
	
 }

// Called every frame
void ASocketConnection::Tick(float DeltaTime)
{
	//UE_LOG(LogTemp, Warning, TEXT("CONECTADOO %s"), this->conexion->GetConnectionState());
	if (this->conexion->GetConnectionState() == SCS_Connected) {
		Super::Tick(DeltaTime);
		enviarDatosSocket();
		recibirDatosSocket();
	}else {
		//FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("Conexion interrumpida, revise los datos de conexion,se cerrara  Visual."));
		//UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
	}

}

void ASocketConnection::recibirDatosSocket() {
	uint32 aux;
	size_t era=4;
	if (this->conexion->HasPendingData(aux)) //Hay Datos?
	{
		if (this->conexion->Recv(RTX_DATA_RECIBIDO, aux, bytesleidos)) { //Si lectura falla, no estamos conectados
		//	memccpy(puntero, RTX_DATA_RECIBIDO, 4, era);
			RTX_DATA_RECIBIDO[4] = RTX_DATA_RECIBIDO[3];
			RTX_DATA_RECIBIDO[5] = RTX_DATA_RECIBIDO[2];
			RTX_DATA_RECIBIDO[6] = RTX_DATA_RECIBIDO[1];
			RTX_DATA_RECIBIDO[7] = RTX_DATA_RECIBIDO[0];
			memcpy(&aux2, RTX_DATA_RECIBIDO+4, aux);
			UE_LOG(LogTemp, Warning, TEXT("valor recibido del socket  %i"),aux2);
		}else {
			UE_LOG(LogTemp, Warning, TEXT("Perdi la conexion tio"));
		}
			
	}

}

void ASocketConnection::enviarDatosSocket() {
	int32 bytesEnviados;

    RTX_object_res* t;
    t = (RTX_object_res*)&RTX_DATA_ENVIO;  //CASTING


	if (this->conexion->Send(RTX_DATA_ENVIO, paqueteMax, bytesEnviados)) { //Si envio falla, no conectados
	//	UE_LOG(LogTemp, Warning, TEXT("bytes enviados %i"), bytesEnviados);
	} else {
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("Conexion interrumpida, revise los datos de conexion,se cerrara  Visual."));
		UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
	}
	
}


/*
										Fichero VISUAL5.H


										typedef struct
{
 float		          timestamp;// not used at present // make a float union
 unsigned short int length;   // number of 32 bit words in following request
 unsigned char      status;   // Dynamics to Visual  .......t.b.d.
															// Visual to Dynamics, 21= visual ready and running
 char
#define OBJECT_CODE             0x01
#define COLL_RES_CODE           0x02
#define TFB_VTX_CODE            0x03
#define TFB_RES_CODE            0x04
#define EVENT_CODE              0x05
#define SEARCH_RAY_CODE         0x06
#define SEARCH_RES_CODE         0x07
 some other values defined (F1 car) these can be relocated later
#define CLOUD_LAYER_REQUEST     0x10
#define TIME_OF_DAY_REQUEST     0x11
#define FOG_REQUEST             0x12
#define SWITCH_CODE             0x13
#define GSWITCH_CODE            0x16
#define LOCAL_ROT_CODE          0x17
#define GLOCAL_ROT_CODE         0x18
#define ACKNOWLEDGE_RESPONSE    0x1e

#define MODIFY_AIR_OBJECT_CODE  0x20

#define LOC_CFRAME_CODE         0x24
#define OBJECT_SPH_CODE         0x25
#define RWAY_SETUP_CODE         0x26
#define EYE_OFFSET_CODE         0x27
#define SKY_CODE                0x2A


#define SCREEN_SAVER_ON         0x0001
#define CRASH_SEQUENCE_ON       0x0002
#define EXIT_GFX                0x007C   // bit 2  3  4 5 & 6 set

			   4 3 2
SHUTDOWN       0 0 1 (1<<2)
REBOOT         0 1 0 (2<<2)
FORCE          0 1 1 (3<<2)
POWEROFF       1 0 0 (4<<2)
FORCEIFHUNG    1 0 1 (5<<2)
LOGOFF         1 1 0 (6<<2)
EXIT           1 1 1 (7<<2)

int RR_decode_vis_buffer()
{
   int             i,loop=0;
   unsigned short  length = 0;
   char            chksum,chksum2=0;
   RTX_header     *header;
   RTX_object_res         *obj;
   RTX_coll_res           *col_res;
   RTX_search_res         *ser_res;
   RTX_tfb_res            *tfb_res;
   RTX_Acknowledge_res    *ack;
   RTX_rw_setup_res       *rw;
   RTX_loc_cframe_res     *cf;
   RTX_eye_offset_res     *eye;
   RTX_gswitch_res        *gsw;
   RTX_switch_res         *sw;
   RTX_loc_rot_res        *lrot;
   RTX_sky_res            *sky;
   int                    id;

   Buffer_ptr     = RTX_Data;                // reset buffer ptr to base of buffer

   header         = (RTX_header * ) RTX_Data;

   chksum         = header->chksum;                 // used later to check data integrity
   header->length = header->length;

   Buffer_ptr     = (RTX_Data + sizeof(RTX_header));


   if (crash_on)
      --crash_on;


   while ( length < header->length )
   {
      ++loop;
//#ifdef DUMMY
      if ( (loop > MAX_PACKETSIZE) //a simple kbhit may cut short data processing
            && kbhit() )
      {
//       cprintf("Packet error ! ");
         //in case of corrupted data and loop out of control
         break;
      }
//#endif

      switch( (char)*Buffer_ptr )               // response object type
      {
         case OBJECT_CODE:
            // Usually this means an error in object request

            obj = (RTX_object_res *)  Buffer_ptr;

            if (obj->obj_id == HOST_ID)
            {
               if (obj->error == ERR_NO_TFB_VTX)
                  Tfb_vtx_set_ok = FALSE;
            }

            Buffer_ptr += obj->length*4;
            length     += obj->length;
            break;


         case COLL_RES_CODE: //este case esta comentado
            /*
               this will be recieved if gfx detects motion
               has transversed a polygon.
               when this is called do reset (if required)

               1. set collision_detect = 1
               2. set crash_on = 1
               3. screen_saver_on = 0
           //comentario

crash_on = 40;
screen_saver_on = 0;
collision_detect = 0;


//  reset_dynamics();
col_res = (RTX_coll_res*)Buffer_ptr;

Buffer_ptr += col_res->length * 4;
length += col_res->length;
dyn_state = DYN_HALT;


//          graphics_collision = TRUE;
break;

         case TFB_RES_CODE:

             tfb_res = (RTX_tfb_res*)Buffer_ptr;

             Terrain_feedback_response(tfb_res);

             Buffer_ptr += tfb_res->length * 4;

             length += tfb_res->length;
             break;


         case SEARCH_RES_CODE:

             ser_res = (RTX_search_res*)Buffer_ptr;

             Buffer_ptr += ser_res->length * 4;
             length += ser_res->length;

             break;

         case ACKNOWLEDGE_RESPONSE:


             ack = (RTX_Acknowledge_res*)Buffer_ptr;

             Buffer_ptr += ack->length * 4;
             length += ack->length;
             Tfb_vtx_set_ok = TRUE;   //Hmm...!
#ifdef DYN_DOING_FOG
             Vis_setup_ok = TRUE;   //For now
#endif
#ifdef DYN_DOING_CLD
             Cld_setup_ok = TRUE;   //For now
#endif
#ifdef DYN_DOING_TOD
             Tod_setup_ok = TRUE;   //For now
#endif

             Sw_setup_ok = TRUE;    //For now
             break;



         case RWAY_SETUP_CODE:

             // This acknowledges receipt of a runway setup request
             rw = (RTX_rw_setup_res*)Buffer_ptr;

             // If this response matches our last runway request
             // then stop sending request

             if (rw->err_ref_id == ERR_BAD_OBJECT)
                 RR_runway_setup_ok = FALSE;
             else
             {
                 if ((rw->obj_id == Rw_id) && (rw->err_ref_id == Rw_ref_id))
                 {
                     RR_runway_setup_ok = TRUE;
                     if (Rw_on_flag)
                     {
                         //NB. This is only intended for use when we first go
                         //in to RUN mode, ie. we dont reset this
                         RR_vis_db_loaded = TRUE;

                         //The first valid TFB for this new runway will be
                         //received in the NEXT transmission, NOT THIS ONE,
                         //so make sure this case is called after TFB_RES_CODE

                     }
                 }
             }


             Buffer_ptr += rw->length * 4;
             length += rw->length;

             break;

         case LOC_CFRAME_CODE:

             // This acknowledges receipt of a local coord frame request
             cf = (RTX_loc_cframe_res*)Buffer_ptr;

             if (cf->ref_id == Cf_ref_id)
             {
                 Cframe_setup_ok = TRUE;
                 // Increment ref_id, ready for next time
                 Cf_ref_id++;
                 if (Cf_ref_id > 120)
                     Cf_ref_id = 100;
             }

             Buffer_ptr += cf->length * 4;
             length += cf->length;

             break;

#ifdef DO_EYEPOINT
         case EYE_OFFSET_CODE:

             // This acknowledges receipt of an eyepoint request
             eye = (RTX_eye_offset_res*)Buffer_ptr;

             if (eye->ref_id == Eye_ref_id)
                 Eye_setup_ok = TRUE;

             Buffer_ptr += eye->length * 4;
             length += eye->length;

             break;
#endif

         case GSWITCH_CODE:

             // This acknowledges receipt of a group switch request
             gsw = (RTX_gswitch_res*)Buffer_ptr;

#ifdef DO_LT_INTENSITY
             if (gsw->sw_id_state & SW_ON_BIT)   //Only look at the active one
             {
                 //Clear bits not relating to number
                 id = gsw->sw_id_state & (0xFFFF - SW_ON_BIT - SW_ALL_BIT);
                 if (id == Lt_intensity)
                     Int_setup_ok = TRUE;
             }
#endif

             Buffer_ptr += gsw->length * 4;
             length += gsw->length;

             break;

         case SWITCH_CODE:

             // This acknowledges receipt of a switch request
             sw = (RTX_switch_res*)Buffer_ptr;

#ifdef DO_WINDSOCK
             if (sw->sw_id_state & SW_ON_BIT)   //Only look at the active one
             {
                 //Clear bits not relating to number
                 id = sw->sw_id_state & (0xFFFF - SW_ON_BIT - SW_ALL_BIT);
                 if (id == Wind_strength + MIN_WINDSOCK_LEVEL_SW)
                     Wind_level_setup_ok = TRUE;
             }
#endif

             Buffer_ptr += sw->length * 4;
             length += sw->length;

             break;

         case LOCAL_ROT_CODE:

             // This acknowledges receipt of a switch request
             lrot = (RTX_loc_rot_res*)Buffer_ptr;

#ifdef DO_WINDSOCK
             if (lrot->ref_id == Wind_dir_ref_id)
                 Wind_dir_setup_ok = TRUE;
#endif


             //Buffer_ptr += sw->length * 4;
             Buffer_ptr += lrot->length * 4;
             //length     += sw->length;
             length += lrot->length;

             break;

         case GLOCAL_ROT_CODE:
             length += Buffer_ptr[1];
             Buffer_ptr += Buffer_ptr[1] * 4;
             break;

#ifdef DO_SKY
         case SKY_CODE:

             // This acknowledges receipt of a sky request
             sky = (RTX_sky_res*)Buffer_ptr;

             //This test is no bullet proof because it does not
             //allow for changing the scale of the same id as before
             if (sky->sky_id == IOS_sky_id)
                 Sky_setup_ok = TRUE;

             Buffer_ptr += sky->length * 4;
             length += sky->length;

             break;
#endif

         default:

             printf("\nUnrecognised opcode %0x hex", Buffer_ptr[0]);

             // Update length even if we don't recognise opcode
             length += Buffer_ptr[1];
             Buffer_ptr += Buffer_ptr[1] * 4;

             break;
      }
   }


   Buffer_ptr = (RTX_Data + sizeof(RTX_header));                // reset buffer ptr to base of buffer

   for (i = 0; i < 4 * header->length; ++i)
       chksum2 += Buffer_ptr[i]; // calculate checksum

    //Should be checking chksum at start!

   return 0;
}
*/