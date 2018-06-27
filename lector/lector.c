#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <nfc/nfc.h>
#include <curl/curl.h>
#include <wiringPi.h>

nfc_device *pnd;
nfc_target nt;
nfc_context *context;
 
struct 
MemoryStruct {
  char *memory;
  size_t size;
};

void
print_nfc_target(const nfc_target *pnt, bool verbose)
{
  char *s;
  str_nfc_target(&s, pnt, verbose);
  printf("%s", s);
  nfc_free(s);
}

void
nfcInitListen(){
	const char *acLibnfcVersion = nfc_version();
	pnd = nfc_open(context, NULL);

	if (pnd == NULL) {
		printf("ERROR: %s", "Imposible conectar con el lector.");
		exit(EXIT_FAILURE);
	}
	if (nfc_initiator_init(pnd) < 0) {
		nfc_perror(pnd, "nfc_initiator_init");
		exit(EXIT_FAILURE);
	}
	printf("NFC reader: %s encendido\n", nfc_device_get_name(pnd));
}

int
CardTransmit(nfc_device *pnd, uint8_t * capdu, size_t capdulen, uint8_t * rapdu, size_t * rapdulen)
{
  int res;
  size_t  szPos;
  printf("=> ");
  for (szPos = 0; szPos < capdulen; szPos++) {
    printf("%02x ", capdu[szPos]);
  }
  printf("\n");
  if ((res = nfc_initiator_transceive_bytes(pnd, capdu, capdulen, rapdu, *rapdulen, 500)) < 0) {
    return -1;
  } else {
    *rapdulen = (size_t) res;
    printf("<= ");
    for (szPos = 0; szPos < *rapdulen; szPos++) {
      printf("%02x ", rapdu[szPos]);
    }
    printf("\n");
    return 0;
  }
}

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  mem->memory = realloc(mem->memory, mem->size + realsize + 1);
  if(mem->memory == NULL) {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}
void setupWiring(){
    wiringPiSetup () ;
    
    // wiringPi pin 0 = Physical pin 11 (or BCM pin 17) on Raspberry Pi 2 B
    pinMode (0, OUTPUT) ; 
    // wiringPi pin 2 = Physical pin 13 (or BCM pin 27) on Raspberry Pi 2 B
    pinMode (2, OUTPUT) ;   
    // green LED = A0 (CERR)
    digitalWrite (0, LOW) ;
    // red LED = A1 (ENABLE) activo a nivel bajo
    digitalWrite (2, HIGH) ;
}
void cleaning(){
  digitalWrite (0, LOW);
  digitalWrite (2, LOW);
}

void doorlock(int open){
  
  if (open){
    digitalWrite (0,HIGH); //green
    digitalWrite (2, LOW); //red
    delay (5000);
  }else{ //close
    digitalWrite (0, LOW); //green 
    digitalWrite (2, HIGH); //red
 }
}
void
checkUser(){
	
	uint8_t capdu[264];
	size_t capdulen;
	uint8_t rapdu[264];
	size_t rapdulen;
	
	size_t userlen = 0; 
	
	CURL *curl;
	CURLcode res;
	struct MemoryStruct chunk;
	chunk.memory = malloc(1);  
	chunk.size = 0; 
  
	const nfc_modulation nmMifare = {
		.nmt = NMT_ISO14443A,
		.nbr = NBR_106,
	};
	// nfc_set_property_bool(pnd, NP_AUTO_ISO14443_4, true);
	printf("Buscando objetivo...\n");
	while (nfc_initiator_select_passive_target(pnd, nmMifare, NULL, 0, &nt) <= 0);
	printf("Objetivo encontrado!\n");
	//print_nfc_target(&nt, false);  //ATQA|UID|SAK
	// Select application
	memcpy(capdu, "\x00\xA4\x04\x00\x06\xF2\x22\x22\x22\x22\x00", 11);
	capdulen=11;
	rapdulen=sizeof(rapdu);

	if (CardTransmit(pnd, capdu, capdulen, rapdu, &rapdulen) < 0){
		printf("Error al mandar el mensaje al móvil\n");
		free(chunk.memory);
		return;
	}
	if (rapdulen < 2 || rapdu[rapdulen-2] != 0x90 || rapdu[rapdulen-1] != 0x00){
		printf("Tipo incorrecto del mensaje recibido\n");
		if(rapdu[rapdulen-2] == 0x6a && rapdu[rapdulen-1]==0x82){
			printf("AID incorrecto\n");
			return;
		}
		else{
		printf("El usuario no ha iniciado sesión\n");
		memcpy(capdu, "\x4C\x4F\x47\x49\x4E\x5F\x45\x52\x52\x4F\x52", 11); //LOGIN_ERROR
        capdulen = 11;
        rapdulen = sizeof (rapdu);
        if (CardTransmit(pnd, capdu, capdulen, rapdu, &rapdulen) < 0) {
            printf("Error al mandar el mensaje al móvil\n");
		}
		free(chunk.memory);
		return;
		}
	}
	char user[rapdulen-1];
	char *rapduc = (char*) rapdu;
	strncpy(user,rapduc,rapdulen-2);
	user[rapdulen-2] = '\0';
	printf("Se ha recibido de la app el nombre: %s\n",user);
	
	char postuser[rapdulen + 3];
	strcpy(postuser,"user=");
	strcat(postuser,user);
	printf("postuser: %s\n",postuser);
	printf("%lu bytes de la variable postuser\n",strlen(postuser));

	  
	/* In windows, this will init the winsock stuff */
	curl_global_init(CURL_GLOBAL_ALL);

	/* get a curl handle */
	curl = curl_easy_init();
	if(curl) {
		/* First set the URL that is about to receive our POST. This URL can
		just as well be a https:// URL if that is what should receive the
		data. */
		curl_easy_setopt(curl, CURLOPT_URL,"http://10.7.2.60:2000/instrumentos_acceso105/access.php");
		curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 2000L);
		/* Now specify the POST data */
		curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS,postuser);
		/* send all data to this function  */
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		/* we pass our 'chunk' struct to the callback function */
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);


		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if(res != CURLE_OK){
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				  curl_easy_strerror(res));
			memcpy(capdu,"\x53\x45\x52\x56\x45\x52\x5F\x45\x52\x52\x4F\x52", 12); //SERVER_ERROR
			capdulen = 12;
			rapdulen = sizeof (rapdu);
			if (CardTransmit(pnd, capdu, capdulen, rapdu, &rapdulen) < 0) {
				printf("Error al mandar el mensaje al móvil\n");
			}				  
			free(chunk.memory);
			return;
		}else {
			/*
			 * Now, our chunk.memory points to a memory block that is chunk.size
			 * bytes big and contains the remote file.
			 *
			 * Do something nice with it!
			 */
			printf("%s\n",chunk.memory);
			printf("%lu bytes retrieved\n", (long)chunk.size);
			if(!strcmp(chunk.memory,"OK")){
				memcpy(capdu,"\x47\x52\x41\x4e\x54\x45\x44\x0A", 8); //GRANTED
				capdulen = 8;
				rapdulen = sizeof (rapdu);
				if (CardTransmit(pnd, capdu, capdulen, rapdu, &rapdulen) < 0) {
					printf("Error al mandar el mensaje al móvil\n");
				}				
				doorlock(1);
				doorlock(0);
			} else{
				printf("No puedes pasar! Zona restringida\n");
				memcpy(capdu,"\x44\x45\x4E\x49\x45\x44\x0A", 7); //DENIED
				capdulen = 7;
				rapdulen = sizeof (rapdu);
				if (CardTransmit(pnd, capdu, capdulen, rapdu, &rapdulen) < 0) {
					printf("Error al mandar el mensaje al móvil\n");
				}			  
			}
		}
		  
    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  free(chunk.memory);
  return;  
}
int
main(int argc, const char *argv[])
{
	// About GPIO & wiringPI Lib
    setupWiring();
	while(1){
		nfc_init(&context);
		if(context == NULL) {
			printf("Unable to init libnfc (malloc)\n");
			exit(EXIT_FAILURE);
		}
		nfcInitListen();
		checkUser();
		nfc_close(pnd);
		nfc_exit(context);
		sleep(2);
		printf("Empezando otra vez...\n");
	}
	exit(EXIT_SUCCESS);
}
