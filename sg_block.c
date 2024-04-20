#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define N_THREADS 8
size_t bsize = 32; // 32 uint32_t * 32 uint32_t => 16KB

typedef struct {
    size_t jj_beg;
    size_t kk_beg;
        uint32_t *C;
    uint32_t *G;
    size_t V;
} ThreadArgs;

void *seg_multiply(void *arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    size_t jj_beg = args->jj_beg;
    size_t kk_beg = args->kk_beg;
    uint32_t *C = args->C;
  uint32_t *G = args->G;
  size_t V = args->V;

    size_t i, j, k;

    for (i = 0; i < V; i++) {
        for (k = kk_beg; k < (kk_beg + bsize < V ? kk_beg + bsize : V); k++)  {
            uint32_t r = G[i * V + k];
            for (j = jj_beg; j < (jj_beg + bsize < V ? jj_beg + bsize : V); j++) {
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

    // G_g = G;
    // V_g = V;
    // C_g = C;
    size_t kk, jj;
    //size_t inc = V / bsize;
    pthread_t threads[N_THREADS];
    ThreadArgs arguments[N_THREADS];
    size_t ctr = 0;

    for (kk = 0; kk < V; kk += bsize) {
        for (jj = 0; jj < V; jj += bsize) {
            if (ctr >= N_THREADS) {
                for (size_t t = 0; t < N_THREADS; t++) {
                    pthread_join(threads[t], NULL);
                }
                ctr = 0;
            }

            arguments[ctr].jj_beg = jj; //+ (size_t)((i % (size_t) 4) * bsize / (size_t)4);// (i / 2 + i % 2) * bsize / 4;// + ((i + 1) % 2) * bsize / 2; // jj offset = 0 for odd, = bsize/2 for even
            arguments[ctr].kk_beg = kk; //+ (size_t)((i / (size_t) 4) * bsize / (size_t)4);// (i / 2 + i % 2) * bsize / 4; // kk offset
            arguments[ctr].G = G;
            arguments[ctr].C = C;
            arguments[ctr].V = V;

            pthread_create(&threads[ctr], NULL, seg_multiply, (void*)&arguments[ctr]);
            ctr++;
        }
    }

    for (size_t t = 0; t < ctr; t++) {
        pthread_join(threads[t], NULL);
    }

    for (size_t i = 0; i < V; i++) {
        uint32_t max = 0;
        size_t maxIndex = i;
        for (size_t j = 0; j < V; j++) {
            if (j != i && G[i * V + j] == 0 && max < C[i * V + j]) {
                max = C[i * V + j];
                maxIndex = j;
            } 
        }
        R[i] = maxIndex;
    }
}