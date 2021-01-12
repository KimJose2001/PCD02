#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
//#include <time.h>
#define THREADS 4 //numero de threads
long int N = 100; //numero de execuções
long int SOMA;
long int respond;
long int request;
long int local;

//Manna-Pnuelli

//Algoritmo Cliente
void client(void* i){
	long int aux = (long) i;
	for(long int j = 0; j < local; j++) {
		while(respond != aux){
			request = aux;
		}
		SOMA++;
		printf("Thread %ld -> Soma %ld \n", i, SOMA);
		respond = 0;
	}
	pthread_exit(NULL);
}

//Algoritmo Servidor
void server(void * i){
	//loop forever
	while(1){
		while(request == 0);
			respond = request;
		while(respond != 0);
			request = 0;
	}
	pthread_exit(NULL);
}

int main(void){
	//clock_t start, end;
	//double time;
	long int i;
	pthread_t thread[THREADS];
	pthread_t thread_server;
	
	local = N/THREADS; //para que de correto o numero de execuções tem que ser multiplo do numero de Threads
	SOMA = 0;
	request = 0;
	respond = 0;
	//start = clock();
	pthread_create(&thread_server, NULL, server, NULL);
	
	for(i = 0; i < THREADS; i++){
		pthread_create(&thread[i], NULL, client, (void*) i);
	}

	for(i = 0; i < THREADS; i++){
		pthread_join(thread[i], NULL);
	}
	//end = clock();
	//time = ((double) (end - start))/ CLOCKS_PER_SEC;
	printf("SOMA = %ld\n", SOMA);
	//printf("%f seconds to execute", time);
	return 0;
}

