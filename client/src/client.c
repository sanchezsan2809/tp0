#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();
	if(logger == NULL){
		printf("Error: No se pudo crear el logger\n");
		return EXIT_FAILURE;
	}
	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger,"Hola! Soy un log");
	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();
	if(config == NULL){
		log_error(logger, "No se pudo cargar el archivo de configuración.");
		return EXIT_FAILURE;
	}
	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "CLAVE");

	if(ip == NULL){
		log_error(logger, "No se encontró la IP en el archivo de configuración.");
		return EXIT_FAILURE;
	}
	if(puerto == NULL){
		log_error(logger, "No se encontró el puerto en el archivo de configuración.");
		return EXIT_FAILURE;
	}
	if(valor == NULL){
		log_error(logger, "No se encontró la clave en el archivo de configuración.");
		return EXIT_FAILURE;
	}
	
	// Loggeamos el valor de config
	
	log_info(logger,"El valor de config es: %s", valor);
	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);
	if(conexion == -1){
		log_error(logger, "Error al conectar con el servidor. ");
		terminar_programa(conexion, logger, config);
		return EXIT_FAILURE;
	}
	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);
	// Armamos y enviamos el paquete
	paquete(conexion);
	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("client/tp0.log","Loguear TP0",true,LOG_LEVEL_INFO);

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("client/cliente.config");
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");
	while(leido != NULL && strcmp(leido, "") != 0){
		log_info(logger, "%s",leido);
		free(leido);
		leido = readline("> ");
	}
	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	if(leido) free(leido);

	// ¡No te olvides de liberar las lineas antes de regresar!

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();
	// Leemos y esta vez agregamos las lineas al paquete
	leido = readline("> ");
	while(leido != NULL && strcmp(leido,"") != 0){
		agregar_a_paquete(paquete, leido, (strlen(leido) + 1));
		free(leido);
		leido = readline("> ");
	}
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	if(leido)free(leido);
	enviar_paquete(paquete, conexion);
	eliminar_paquete(paquete);	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	liberar_conexion(conexion);
	log_destroy(logger);
	config_destroy(config);
}
