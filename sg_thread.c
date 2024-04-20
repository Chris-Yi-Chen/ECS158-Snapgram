#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define N_THREADS 8

typedef struct {
    size_t beg;
    size_t end;
    uint32_t *C;
    uint32_t *G;
    size_t V;

} ThreadArgs;


void *multiply(void *arg) {
	ThreadArgs* args = (ThreadArgs*)arg;
	size_t beg = args->beg;
	size_t end = args->end;
	uint32_t *C = args->C;
    uint32_t *G = args->G;
    size_t V = args->V;

    size_t i,j,k;

	for (i = beg; i < end; i++) {
		for (k = 0; k < V; k++) {
			uint32_t r = G[i * V + k];
			for (j = 0; j < V; j++) {
				C[i * V + j] += r * G[k * V + j];
			}
		}
	}
    pthread_exit(NULL);
}

void sg_recommender(uint32_t *G, size_t V, uint32_t *R)
{
	uint32_t *C;
	C = aligned_alloc(64, V * V * sizeof(uint32_t));
	memset(C, 0, V * V * sizeof(uint32_t));

    size_t i,j;



	pthread_t threads[N_THREADS];
	int chunkSize = V / N_THREADS;
    ThreadArgs arguments[N_THREADS];
	size_t beg = 0;
	for ( i = 0; i < N_THREADS; i++) {
		size_t end = beg + chunkSize;
		arguments[i].beg = beg;
		arguments[i].end = end;
		arguments[i].G = G;
		arguments[i].C = C;
		arguments[i].V = V;

        pthread_create(&threads[i], NULL, multiply, (void*)&arguments[i]);
		beg = end;
    }

	for (i = 0; i < N_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

	for (i = 0; i < V; i++) {
		uint32_t max = 0;
		size_t maxIndex = i;
		for (j = 0; j < V; j++) {
			if (j != i && G[i * V + j] == 0 && max < C[i * V + j]) {
				max = C[i * V + j];
				maxIndex = j;
			} 
		}
		R[i] = maxIndex;
	}

}
