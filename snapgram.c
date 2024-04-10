#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Recommender engine
 * @G: graph represented as adjacency matrix. The matrix is a *flat* 2D-array,
 *     is accessed via single index: G[i][j] => G[i * @V + j].
 * @V: number of nodes in graph.
 * @R: recommendation for each user, as 1D-array of @V entries.
 *
 * This function is not be to implemented here, but in one of the sg_* files.
 */
void sg_recommender(uint32_t *G, size_t V, uint32_t *R);

uint32_t *read_graph(FILE *fi, size_t *_V)
{
	size_t V, E;
	uint32_t *G;

	if (fscanf(fi, "%zu\n%zu\n", &V, &E) != 2)
		goto err;

	/* Check that V is a non-0 power of two */
	if (!V || (V & (V - 1)))
		goto err;

	/* Align allocation a cache line (64 bytes) */
	G = aligned_alloc(64, V * V * sizeof(uint32_t));
	memset(G, 0, V * V * sizeof(uint32_t));

	/* Populate edges */
	while (E--) {
		size_t i, j;
		uint32_t l;

		if (fscanf(fi, "%zu %zu %"PRIu32"\n", &i, &j, &l) != 3)
			goto err;

		G[i * V + j] = l;
	}

	*_V = V;
	return G;

err:
	fprintf(stderr, "Incorrect input file format\n");
	exit(1);
}

void write_rec(FILE *fo, uint32_t *R, size_t V)
{
	size_t i;

	for (i = 0; i < V; i++)
		fprintf(fo, "%zu %"PRIu32"\n", i, R[i]);
}

int main(int argc, char *argv[])
{
	FILE *fi, *fo;
	size_t V;
	uint32_t *G, *R;

	/* Command line arguments */
	if (argc != 3) {
		fprintf(stderr, "Usage: %s infile outfile\n", argv[0]);
		exit(1);
	}

	/* Read input graph */
	fi = fopen(argv[1], "r");
	fo = fopen(argv[2], "w");
	if (!fi || !fo) {
		fprintf(stderr, "Could not open input or output file\n");
		exit(1);
	}
	G = read_graph(fi, &V);

	/* Align allocation a cache line (64 bytes) */
	R = aligned_alloc(64, V * sizeof(uint32_t));

	/* !!! Calling your implementation !!! */
	sg_recommender(G, V, R);

	/* Write output results */
	write_rec(fo, R, V);

	fclose(fi);
	fclose(fo);

	free(G);
	free(R);

	return 0;
}
