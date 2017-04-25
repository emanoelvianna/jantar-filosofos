#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define SHMSZ 27

/**
 * comando de compilação:
 * gcc servidor.c -o servidor -lpthread
 * comando de execução:
 * ./servidor
 */
int main()
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

	//nomes dos segmentos de memória compartilhada
	mensagem_key = 5678;
	sem_cliente_mensagem_key = 5679;
	sem_servidor_mensagem_key = 5680;

	//cria os segmentos de memória
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

	//escreve valores iniciais na memória compartilhada
	mensagem_aux = mensagem;
	for (i = 0; i < max_buffer; i++)
	{	
		*mensagem_aux++ = ' ';
	}
	*mensagem_aux = (long) NULL;

	//inicializa os semáforos
	sem_init(&(*sem_cliente), 1, 1);
	sem_init(&(*sem_servidor), 1, 0);

	//sempre que um cliente envia uma mensagem, efetua uma leitura na área de memória compartilhada 
	while(true)
	{
		//espera sinal
		sem_wait(&(*sem_servidor));		

		//imprime mensagem
		printf("MENSAGEM_%d->",numero_de_mensagens_lidas);
		i=0;
		for(mensagem_aux = mensagem; *mensagem_aux != (long) NULL; mensagem_aux++)
		{
			putchar(*mensagem_aux);
			i++;
		}
		printf("\n\n");

		//atualiza tamanho maximo de mensagem lida
		if(i > max_buffer)
		{
			max_buffer = i;
		}	
		
		//limpa a área da memória compartilhada
		mensagem_aux = mensagem;
		for (i = 0; i < max_buffer; i++)
		{
			*mensagem_aux++ = ' ';
		}		

		//atualiza o número de mensagens lidas
		numero_de_mensagens_lidas++;

		//envia sinal
		sem_post(&(*sem_cliente));	
	}

	exit(0);
}
