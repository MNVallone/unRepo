#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* clave;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	log_info(logger,"Hola! Soy un log");

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	clave = config_get_string_value(config, "CLAVE");
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");

	log_info(logger, "Clave: %s, Direccion IP: %s, Numero de Puerto: %s \n", clave, ip, puerto);


	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	// Loggeamos el valor de config


	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(clave, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	//terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("tp0.log", "TP0", true, LOG_LEVEL_INFO);

	if (nuevo_logger == NULL){
		printf("No se ha podido crear el logger \n");
		exit(1);
	}

	return nuevo_logger;
}

t_config*  iniciar_config()
{
	t_config* nuevo_config;

	if ((nuevo_config = config_create("../cliente.config")) == NULL){
		printf("No pude leer la config \n");
		exit(2);
	}

	return nuevo_config;
}


void leer_consola(t_log* logger)
{
	// Vamos a leer las líneas que ingrese el usuario por consola
	char *linea;
    while (1) {
        linea = readline(">");
        if (!linea) {
            break;
        }
		if (!strncmp(linea, "", 4)) {
            free(linea);
            break;
        }
        if (linea) {
            add_history(linea);
			log_info(logger, linea);
        }
      
        printf("%s\n", linea);
        free(linea);
    }


//free(leido);
	// El resto, las vamos leyendo y logueando hasta recibir un string vacío


	// ¡No te olvides de liberar las lineas antes de regresar!

}

void paquete(int conexion)
{
	char* linea;
	t_paquete* paquete;
	paquete = crear_paquete();
	linea = readline(">");

	// Leemos y esta vez agregamos las lineas al paquete
 	while(strcmp(linea, "") != 0){
		agregar_a_paquete(paquete, linea, strlen(linea) + 1);
		free(linea);
		linea = readline(">");
 	}
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(linea);

	enviar_paquete(paquete, conexion);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	if(logger != NULL){
		log_destroy(logger);
	}

	if (config != NULL){
		config_destroy(config);
	}

	if (conexion != NULL){
		liberar_conexion(conexion);
	}
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
}
