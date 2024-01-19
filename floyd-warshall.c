#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INFINITY 1000
#define INFINITY_STRING "inf"
#define INFINITY_STRING_TERMINATED "inf\n"
#define DELIMITER " "
#define BUF_SIZE 1024

void print_matrix (int **matrix, int dimensions);
int **init_matrix (int dimensions);
int floyd_warshall_algorithm (int **distances, int **predecessors,
                              int dimensions);
int **read_matrix_from_file (char *filename, int dimensions);
int count_matrix_dimensions_from_file (char *filename);

int
main (int argc, char *argv[])
{
    // Variable definition

    int **distances; // Matrix that stores the distances from node i to node j
    int **predecessors; // Matrix that stores the predecessor to j in the path
                        // from node i to node j
    int node_num = 0;   // The number of nodes in the graph

    char filename[BUF_SIZE]; // String containing input file name

    if (argc == 1)
        { // Interactive mode
            printf ("Enter the filename containing the matrix: ");
            if (fgets (filename, BUF_SIZE, stdin) != NULL)
                node_num = count_matrix_dimensions_from_file (filename);
            else
                {
                    printf ("Unable to read filename.\n");
                    return 1;
                }

            if (node_num == -1)
                {
                    printf ("\nThere was an error while reading from file.\n");
                    return 1;
                }
        }
    else if (argc == 2)
        { // Normal mode
            if ((!strcmp (argv[1], "-h") || !strcmp (argv[1], "--help")))
                {
                    printf ("Usage: floyd-warshall [filename.txt|OPTIONS]\n");
                    printf ("\n\t-h, --help\tshows this prompt\n");
                    printf ("\nIf no arguments are passed, the program will "
                            "start in interactive mode.\n");

                    return 0;
                }
            else
                {
                    strcpy (filename, argv[1]);
                }
        }
    else
        {
            printf ("Invalid arguments. Type floyd-warshall -h for help.\n");
            return 1;
        }

    if (node_num == 0)
        node_num = count_matrix_dimensions_from_file (filename);
    if (node_num == -1)
        {
            printf ("There was an error while reading the input file.\n");
            return 1;
        }

    distances = read_matrix_from_file (filename, node_num);
    if (distances == NULL)
        {
            printf ("\nUnable to initialise the distance matrix. Exiting.");
            return 1;
        }

    printf ("\nEntered matrix:\n");
    print_matrix (distances, node_num);

    predecessors = init_matrix (node_num);

    int result = floyd_warshall_algorithm (distances, predecessors, node_num);

    if (result)
        {
            printf (
                "\nA negative cost circuit was found: computation halted.");
        }

    printf ("\nThe final distance matrix is:\n");
    print_matrix (distances, node_num);

    if (!result)
        {
            printf ("\nThe final predecessor matrix is:\n");
            print_matrix (predecessors, node_num);
        }

    return 0;
}

void
print_matrix (int **matrix, int dimensions)
{
    for (int i = 0; i < dimensions; i++)
        {
            for (int j = 0; j < dimensions; j++)
                {
                    if (matrix[i][j] == INFINITY)
                        printf ("inf\t");
                    else
                        printf ("%d\t", matrix[i][j]);
                }
            printf ("\n");
        }
}

int **
init_matrix (int dimensions)
{
    // Create a new double pointer
    int **matrix;

    // Allocate space for the square matrix
    matrix = (int **)malloc (sizeof (int *) * dimensions);
    for (int k = 0; k < dimensions; k++)
        matrix[k] = (int *)malloc (sizeof (int) * dimensions);

    // Initialize all elements of the matrix
    for (int i = 0; i < dimensions; i++)
        {
            for (int j = 0; j < dimensions; j++)
                {
                    matrix[i][j] = i;
                }
        }

    // Return the allocated matrix
    return matrix;
}

int
floyd_warshall_algorithm (int **distances, int **predecessors, int dimensions)
{
    for (int u = 0; u < dimensions; u++)
        {
            for (int i = 0; i < dimensions; i++)
                {
                    for (int j = 0; j < dimensions; j++)
                        {
                            if (i != u && j != u)
                                {
                                    if (distances[i][u] + distances[u][j]
                                            < distances[i][j]
                                        && distances[i][u] != INFINITY
                                        && distances[u][j] != INFINITY)
                                        {
                                            distances[i][j]
                                                = distances[i][u]
                                                  + distances[u][j];

                                            predecessors[i][j] = u;
                                        }
                                }
                        }
                }

            for (int k = 0; k < dimensions; k++)
                {
                    if (distances[k][k] < 0)
                        {
                            return 1;
                        }
                }
        }

    return 0;
}

int **
read_matrix_from_file (char *filename, int dimensions)
{
    FILE *file = fopen (filename, "r");

    if (file == NULL)
        {
            return NULL;
        }

    char buffer[100];
    char *token;

    int read_int[dimensions * dimensions];

    int i = 0;

    while (fgets (buffer, sizeof (buffer), file))
        {
            token = strtok (buffer, DELIMITER);

            while (token != NULL)
                {
                    if (!strcmp (token, INFINITY_STRING)
                        || !strcmp (token, INFINITY_STRING_TERMINATED))
                        {
                            read_int[i] = INFINITY;
                        }
                    else
                        {
                            read_int[i] = strtol (token, NULL, 10);
                        }

                    i++;
                    token = strtok (NULL, DELIMITER);
                }
        }

    fclose (file);

    int **matrix = (int **)malloc (sizeof (int *) * dimensions);
    for (int k = 0; k < dimensions; k++)
        matrix[k] = (int *)malloc (sizeof (int) * dimensions);

    for (int i = 0; i < dimensions; i++)
        {
            for (int j = 0; j < dimensions; j++)
                {
                    matrix[i][j] = read_int[dimensions * i + j];
                }
        }

    return matrix;
}

int
count_matrix_dimensions_from_file (char *filename)
{
    FILE *file = fopen (filename, "r");

    if (file == NULL)
        return -1;

    char buffer[100];
    char *token;
    int element_counter = 0;

    if (fgets (buffer, sizeof (buffer), file) != NULL)
        {
            token = strtok (buffer, DELIMITER);

            while (token != NULL)
                {
                    element_counter++;
                    token = strtok (NULL, DELIMITER);
                }
        }
    else
        {
            fclose (file);
            return -1;
        }

    fclose (file);

    return element_counter;
}
