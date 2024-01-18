#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define INFINITY 1000
#define INFINITY_STRING "inf"
#define INFINITY_STRING_TERMINATED "inf\n"
#define DELIMITER " "
#define DEFAULT_FILE_NAME "input.txt"

void print_matrix(int **matrix, int dimensions);
int **init_matrix(int dimensions);
void enter_distances(int **matrix, int dimensions);
void floyd_warshall_algorithm(int **distances, int **predecessors, int dimensions);
int negative_cost_check(int **distances, int dimensions);
void normalize_matrix(int **matrix, int dimensions);
int **read_matrix_from_file(char *filename, int dimensions);

int main()
{
	// Variable definition
	
	int **distances;		// Matrix that stores the distances from node i to node j
	int	**predecessors;		// Matrix that stores the predecessor to j in the path from node i to node j
	int node_num;			// The number of nodes in the graph

	printf("Enter the number of nodes in the graph you want to analyse: ");
	scanf("%d", &node_num);

	char filename[100];
	printf("\nEnter the filename containing the matrix: ");
	scanf("%s", filename);

	if (strlen(filename) == 0)
		strcpy(filename, DEFAULT_FILE_NAME);

	distances = read_matrix_from_file(filename, node_num);
	if (distances == NULL) {
		printf("\nUnable to initialise the distance matrix. Exiting.");
		return 1;
	}

	printf("\nEntered matrix:\n");
	print_matrix(distances, node_num);

	predecessors = init_matrix(node_num);
	
	floyd_warshall_algorithm(distances, predecessors, node_num);

	printf("\nThe final distance matrix is:\n");
	print_matrix(distances, node_num);

	printf("\nThe final predecessor matrix is:\n");
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
					if (distances[i][u] + distances[u][j] < distances[i][j] && distances[i][u] != INFINITY && distances[u][j] != INFINITY) {
						printf("\tYes");
						distances[i][j] = distances[i][u] + distances[u][j];
						predecessors[i][j] = u;
						printf("\n");
						print_matrix(predecessors, dimensions);
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

int **read_matrix_from_file(char *filename, int dimensions) {
	FILE *file = fopen(filename, "r");
	char buffer[100];
	char *token;

	int read_int[dimensions * dimensions];

	int i = 0;

	while (fgets(buffer, sizeof(buffer), file)) {
		token = strtok(buffer, DELIMITER);

		while (token != NULL) {
			if (!strcmp(token, INFINITY_STRING) || !strcmp(token, INFINITY_STRING_TERMINATED)) {
				read_int[i] = INFINITY;
			} else {
				read_int[i] = strtol(token, NULL, 10);
			}
			
			i++;
			token = strtok(NULL, DELIMITER);
		}
	}

	int **matrix = (int**) malloc(sizeof(int*) * dimensions);
	for (int k = 0; k < dimensions; k++)
		matrix[k] = (int*) malloc(sizeof(int) * dimensions);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			matrix[i][j] = read_int[4 * i + j];
		}
	}

	return matrix;
}
