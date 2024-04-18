#include <stdint.h>
#include <stdlib.h>
#include <string.h>


void sg_recommender(uint32_t *G, size_t V, uint32_t *R)
{
	// uint32_t sum;
    uint32_t *C;
    C = aligned_alloc(64, V * V * sizeof(uint32_t));
	memset(C, 0, V * V * sizeof(uint32_t));

	// for (size_t i = 0; i < V; i++) {
	// 	uint32_t max = 0;
	// 	size_t maxIndex = i;
	// 	for (size_t j = 0; j < V; j++) {
	// 		for (size_t k = 0; k < V; k++) {
	// 			C[i * V + j] += G[i * V + k] * G[k * V + j];
	// 		}
	//		// If not itself, 
	// 		if (j != i && G[i * V + j] == 0 && max < C[i * V + j]) {
	// 			max = C[i * V + j];
	// 			maxIndex = j;
	// 		}
	// 		R[i] = maxIndex;
	// 	}
	// }

	/* ijk */
	for (size_t i = 0; i < V; i++) {
		for (size_t j = 0; j < V; j++) {
			for (size_t k = 0; k < V; k++) {
				C[i * V + j] += G[i * V + k] * G[k * V + j];
			}
		}
	}

	/* jik */
	for (size_t j = 0; j < V; j++) {
		for (size_t i = 0; i < V; i++) {
			for (size_t k = 0; k < V; k++) {
				C[i * V + j] += G[i * V + k] * G[k * V + j];
			}
		}
	}

	/* kij */
	for (size_t k = 0; k < V; k++) {
		for (size_t i = 0; i < V; i++) {
			for (size_t j = 0; j < V; j++) {
				C[i * V + j] += G[i * V + k] * G[k * V + j];
			}
		}
	}

	/* ikj */
	for (size_t i = 0; i < V; i++) {
		for (size_t k = 0; k < V; k++) {
			for (size_t j = 0; j < V; j++) {
				C[i * V + j] += G[i * V + k] * G[k * V + j];
			}
		}
	}

	/* jki */
	for (size_t j = 0; j < V; j++) {
		for (size_t k = 0; k < V; k++) {
			for (size_t i = 0; i < V; i++) {
				C[i * V + j] += G[i * V + k] * G[k * V + j];
			}
		}
	}

	/* kji */
	for (size_t k = 0; k < V; k++) {
		for (size_t j = 0; j < V; j++) {
			for (size_t i = 0; i < V; i++) {
				C[i * V + j] += G[i * V + k] * G[k * V + j];
			}
		}
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
