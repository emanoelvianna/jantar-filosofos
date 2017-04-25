#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define SHMSZ 27

/**
 * comando de compilação:
 * gcc cliente.c -o cliente -lpthread
 * comando de execução:
 * ./cliente <mensagem_string>
 * exemplo de comando de execução:
 * ./cliente "Hello. How are you?"
 */
int main(int argc, char* argv[])
{
	//variáveis
	key_t sem_cliente_mensagem_key;
	key_t sem_servidor_mensagem_key;
	key_t mensagem_key;
	int sem_cliente_id;
	int sem_servidor_id;
	int mensagem_id;	
	sem_t* sem_cliente;
	sem_t* sem_servidor;
	char* mensagem;
	char* mensagem_aux;
	int numero_de_mensagens_lidas = 0;
	int max_buffer = 200;
	int i = 0;
	int valor_cliente, valor_servidor = -1;

	//efetua leitura do parâmetro do main
	mensagem_aux = argv[1];

	//nomes dos segmentos de memória compartilhada
	mensagem_key = 5678;
	sem_cliente_mensagem_key = 5679;
	sem_servidor_mensagem_key = 5680;

	//localiza os segmentos de memória compartilhada
	if((mensagem_id = shmget(mensagem_key, SHMSZ, IPC_CREAT | 0666)) < 0) 
	{
		perror("shmget");
		exit(1);
	}
	if((sem_cliente_id = shmget(sem_cliente_mensagem_key, sizeof(sem_t), IPC_CREAT | 0666)) < 0)
	{
		perror("shmget - semáforo cliente");
		exit(1);
	} 
	if((sem_servidor_id = shmget(sem_servidor_mensagem_key, sizeof(sem_t*), IPC_CREAT | 0666)) < 0)
	{
		perror("shmget - semáforo servidor");
		exit(1);
	}

	//anexa os segmentos de memória para o uso neste processo
	if((mensagem = shmat(mensagem_id, NULL, 0)) == (char *) -1) 
	{
		perror("shmat");
		exit(1);
	}
	if((sem_cliente = (sem_t*) shmat(sem_cliente_id, NULL, 0)) < 0) 
	{
		perror("shmat - semáforo do cliente");
		exit(1);
	}
	if((sem_servidor = (sem_t*) shmat(sem_servidor_id, NULL, 0)) < 0) 		
	{
		perror("shmat - semáforo do servidor");
		exit(1);
	}	

	//espera sinal
	sem_wait(&(*sem_cliente));	

	//copia mensagem para o segmento de memória compartilhada
	while(*mensagem_aux != '\0')
	{
		*mensagem = *mensagem_aux;
		mensagem_aux++;
		mensagem++;
	}

	//envia sinal
	sem_post(&(*sem_servidor));

	exit(0);
}
