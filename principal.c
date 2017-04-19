#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

pthread_mutex_t* ponteiro_mutex_garfos;
pthread_t* ponteiro_threads_filosofos;
int quantidade_filosofos;

double get_time()
{
	struct timeval time_value;
	gettimeofday(&time_value, 0);
	//retorna o momento atual do programa em segundos
	return time_value.tv_sec + (time_value.tv_usec / 1e6);
}

void* funcao_filosofo(void* thread_argument)
{
	long filosofo_id = (long) thread_argument;

	double tempo_inicial = get_time();
	double tempo_atual = 0.0;
	double tempo_decorrido = 0.0;

	while(tempo_decorrido < 10.0)
	{

		tempo_atual = get_time();
		tempo_decorrido = tempo_atual - tempo_inicial;
	}

	printf("filosofo:%ld - tempo:%f\n",filosofo_id,tempo_decorrido);

	pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    //quantidade de filosofos e passada por parametro no main
	quantidade_filosofos = atoi(argv[1]);

	//devem existir pelo menos 2 filosofos, confirmei com o professor!!!
	if(quantidade_filosofos < 2)
	{
		quantidade_filosofos = 2;
	}

	//alocando ponteiros
	ponteiro_mutex_garfos = (pthread_mutex_t*) malloc(quantidade_filosofos * sizeof(pthread_mutex_t));
	ponteiro_threads_filosofos = (pthread_t*) malloc(quantidade_filosofos * sizeof(pthread_t));

	for(int i = 0; i < quantidade_filosofos; i++)
	{
		//inicializando mutex
		pthread_mutex_init(&ponteiro_mutex_garfos[i], NULL);
	}

	for(long i = 0; i < quantidade_filosofos; i++)
	{
		//criando filosofos
		pthread_create(&ponteiro_threads_filosofos[i], NULL, funcao_filosofo, (void*)i);
	}

	for(int i = 0; i < quantidade_filosofos; i++)
	{
		pthread_join(ponteiro_threads_filosofos[i], NULL);
	}
}
