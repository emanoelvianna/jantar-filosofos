#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define SHMSZ     27

/**
 * comando de compilação:
 * gcc servidor.c -o servidor
 * comando de execução:
 * ./servidor
 */

int main()
{
	int i;
	char c;
	int shmid;
	key_t key;
	char *shm, *s;
	int max_buffer = 20;

	/*
	 * We'll name our shared memory segment
	 * "5678".
	 */
	key = 5678;

	/*
	 * Create the segment.
	 */
	if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
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
	 * Now put some things into the memory for the
	 * other process to read.
	 */
	s = shm;

	//for (c = 'a'; c <= 'z'; c++)
	//	*s++ = c;
	//*s = NULL;

	for (i = 0; i < max_buffer; i++)
	{	
		*s++ = ' ';
	}
	*s = NULL;

	/*
	 * Finally, we wait until the other process 
	 * changes the first character of our memory
	 * to '*', indicating that it has read what 
	 * we put there.
	 */
	while(true)
	{
		while (*shm != '*'){sleep(1);}

		i=0;
		for (s = shm; *s != NULL; s++)
		{
			putchar(*s);
			i++;
		}putchar('\n');putchar('\n');

		//printf("ALGUÉM LEU!!!\n");

		if(i > max_buffer)
		{
			max_buffer = i;
		}
		
		s = shm;
		for (i = 0; i < max_buffer; i++)
			*s++ = ' ';
	}
	exit(0);
}
