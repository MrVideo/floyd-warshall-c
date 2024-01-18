#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define INFINITY 1000

void print_matrix(int **matrix, int dimensions);
int **init_matrix(int dimensions);
void enter_distances(int **matrix, int dimensions);
void floyd_warshall_algorithm(int **distances, int **predecessors, int dimensions);
int negative_cost_check(int **distances, int dimensions);
void normalize_matrix(int **matrix, int dimensions);

int main()
{
	// Variable definition
	
	int **distances;		// Matrix that stores the distances from node i to node j
	int	**predecessors;		// Matrix that stores the predecessor to j in the path from node i to node j
	int node_num;			// The number of nodes in the graph

	printf("Enter the number of nodes in the graph you want to analyse: ");
	scanf("%d", &node_num);

	distances = init_matrix(node_num);
	predecessors = init_matrix(node_num);
	
	printf("Enter the distances between all nodes below:\n");
	enter_distances(distances, node_num);
	// print_matrix(distances, node_num);

	floyd_warshall_algorithm(distances, predecessors, node_num);
	// normalize_matrix(distances, node_num);
	
	/*
	if (negative_cost_check(distances, node_num) == 1) {
		printf("Negative cost circuit found!\n");
	} else {
		printf("No negative cost circuit found.\n");
	}
	*/

	printf("The final distance matrix is:\n");
	print_matrix(distances, node_num);

	printf("The final predecessor matrix is:\n");
	print_matrix(predecessors, node_num);

	return 0;
}

void print_matrix(int **matrix, int dimensions)
{
	for (int i = 0; i < dimensions; i++) {
		for (int j = 0; j < dimensions; j++) {
			if (matrix[i][j] == INFINITY)
				printf("inf\t");
			else printf("%d\t", matrix[i][j]);
		}
		printf("\n");
	}
}

int **init_matrix(int dimensions)
{
	// Create a new double pointer
	int **matrix;

	// Allocate space for the square matrix
	matrix = (int**) malloc(sizeof(int*) * dimensions);
	for (int k = 0; k < dimensions; k++)
		matrix[k] = (int*) malloc(sizeof(int) * dimensions);

	// Initialize all elements of the matrix
	for (int i = 0; i < dimensions; i++) {
		for (int j = 0; j < dimensions; j++) {
			matrix[i][j] = i;
		}
	}

	// Return the allocated matrix
	return matrix;
}

void enter_distances(int **matrix, int dimensions)
{
	char input[100];

	for (int i = 0; i < dimensions; i++) {
		for (int j = 0; j < dimensions; j++) {
			printf("Distance between nodes %d and %d: ", i, j);
			scanf("%s", input);

			if (!strcmp(input, "inf")) {
				matrix[i][j] = INFINITY;
			} else {
				matrix[i][j] = strtod(input, NULL);
			}
		}
		printf("\n");
	}
}

void floyd_warshall_algorithm(int **distances, int **predecessors, int dimensions)
{
	for (int u = 0; u < dimensions; u++) {
		printf("\nTriangulation on node %d", u);
		for (int i = 0; i < dimensions; i++) {
			for (int j = 0; j < dimensions; j++) {
				if (i != u && j != u) {
						printf("\nd[%d][%d] + d[%d][%d] < d[%d][%d]?", i, u, u, j, i, j);
						printf("\n%d + %d < %d", distances[i][u], distances[u][j], distances[i][j]);
					if (distances[i][u] + distances[u][j] < distances[i][j]) {
						printf("\tYes");
						distances[i][j] = distances[i][u] + distances[u][j];
						predecessors[i][j] = u;
					}
				}
			}
		}

		printf("\n");
		for (int k = 0; k < dimensions; k++) {
			if (distances[k][k] < 0) {
				printf("\nNegative cost circuit found! Halting.\n");
				return;
			}
		}
		/*
		if (distances[i][i] < 0) {
			printf("\nNegative cost circuit found! Halting.\n");
			return;
		}
		*/
	}
}

int negative_cost_check(int **distances, int dimensions) {
	for (int i = 0; i < dimensions; i++) {
		if (distances[i][i] < 0)
			return 1;
	}

	return 0;
}

void normalize_matrix(int **matrix, int dimensions) {
	for (int i = 0; i < dimensions; i++) {
		for (int j = 0; j < dimensions; j++) {
			if (matrix[i][j] <= -INT_MAX / 4 || matrix[i][j] >= INT_MAX / 4)
				matrix[i][j] = INT_MAX;
		}
	}
}
