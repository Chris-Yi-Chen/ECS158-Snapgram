#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void sg_recommender(uint32_t *G, size_t V, uint32_t *R)
{
	uint32_t sum;
    uint32_t *C;
    C = aligned_alloc(64, V * V * sizeof(uint32_t));
	memset(C, 0, V * V * sizeof(uint32_t));

	for (size_t i = 0; i < V; i++) {
        uint32_t max = 0;
		size_t maxIndex = i;
		for (size_t j = 0; j < V; j++) {
			sum = 0;
			for (size_t k = 0; k < V; k++) {
				sum += G[i * V + k] * G[k * V + j];
			}
            C[i * V + j] = sum;
            if (j != i && G[i * V + j] == 0 && max < sum) {
				max = sum;
				maxIndex = j;
			}
            R[i] = maxIndex;
		}
	}

	// for (size_t i = 0; i < V; i++) {
	// 	uint32_t max = 0;
	// 	size_t maxIndex = i;
	// 	for (size_t j = 0; j < V; j++) {
    //         if (j == i) {
    //             continue;
    //         }
	// 		if (G[i * V + j] == 0 && max < C[i * V + j]) {
	// 			max = C[i * V + j];
	// 			maxIndex = j;
    //             printf("max: %d\n", max);
	// 		} 
	// 	}
	// 	R[i] = maxIndex;
	// }
}
