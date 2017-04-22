/*
 * shm-client - client program to demonstrate shared memory.
 */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define SHMSZ     27

/**
 * comando de compilação:
 * gcc cliente.c -o cliente
 * comando de execução:
 * ./cliente <mensagem_string>
 *
 * exemplo:
 * ./cliente "Don't ask what your country can do for you, but what you can do for your country."
 */

int main(int argc, char* argv[])
{
	char* mensagem = argv[1];	
	//printf("string:%s\n\n",mensagem);

	

	int shmid;
	key_t key;
	char *shm, *s;

	/*
	 * We need to get the segment named
	 * "5678", created by the server.
	 */
	key = 5678;

	/*
	 * Locate the segment.
	 */
	if ((shmid = shmget(key, SHMSZ, 0666)) < 0) {
		perror("shmget");
		exit(1);
	}

	/*
	 * Now we attach the segment to our data space.
	 */
	if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
		perror("shmat");
		exit(1);
	}

	/*
	 * Now read what the server put in the memory.
	 */
	//for (s = shm; *s != NULL; s++)
	//	putchar(*s);
	//putchar('\n');

	/*
	 * Finally, change the first character of the 
	 * segment to '*', indicating we have read 
	 * the segment.
	 */


	*shm = '*';
	shm++;

	while(*mensagem != '\0')
	{
		*shm = *mensagem;
		shm++;
		mensagem++;
	}

	exit(0);
}
