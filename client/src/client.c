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
	log_info(logger, "Hola! Soy un log");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "CLAVE");

	log_info(logger, "IP: %s", ip);
	log_info(logger, "PUERTO: %s", puerto);
	log_info(logger, "VALOR: %s", valor);


	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);
	log_info(logger, "TERMINE DE LEER");

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto); //TIENE EL SOCKET CLIENTE

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor,conexion); 

	// Armamos y enviamos el paquete
	log_info(logger, "MENSAJES A ENVIAR");
	paquete(conexion); 

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;
	nuevo_logger = 	log_create ("tp0.log", "INICIAR", true, LOG_LEVEL_INFO);

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
	nuevo_config = config_create ("cliente.config");

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;
	leido = readline("> ");
	while(strcmp(leido, "")!= 0){
		log_info(logger, "LEIDO: %s\n", leido);
		free(leido);
		leido=readline("> ");
	}

}

void paquete(int conexion)
{
	//Leemos y agrego lineas al paquete
	char* leido;
	t_paquete *paquete = crear_paquete();
	leido=readline("> ");
	while(strcmp(leido, "") !=0){
		agregar_a_paquete(paquete, leido, strlen(leido)+1);
		free(leido);
		leido=readline(">");
	};
	//Envio paquete
	enviar_paquete(paquete, conexion);
	eliminar_paquete(paquete);
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{	
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
}
