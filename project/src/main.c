/*
    Last Name, First Name: Rebosio, Alessandro
    Student ID: 0001130557
    Group: B
    Email: alessandro.rebosio@studio.unibo.it
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#define MAX 250

/* Define the map structure, which holds data from the input file */
typedef struct
{
    unsigned short rows, cols;
    unsigned Ccell, Cheight;
    int **matrix;
} Map;

/* Define the structure for a coordinate */
typedef struct
{
    unsigned short x, y;
} Coordinate;

/* Define the heap structure */
typedef struct
{
    unsigned size, capacity;
    struct HeapNode
    {
        Coordinate pos;
        int cost;
    } *nodes;
} Heap;

/* Reads input data from file, returns the struct */
Map *read_input(char *filename);
/* Frees memory used by the map, returns 1 if successful */
int destroy_map(Map *map);
/* Computes the shortest path using Dijkstra's algorithm */
void dijkstra(Map *map);
/* Recursively prints the shortest path */
void print_path(Coordinate path[MAX][MAX], int x, int y);

/* Initializes the heap */
Heap *init_heap(unsigned capacity);
/* Inserts a new node into the heap, returns 1 if successful */
int push(Heap *heap, int x, int y, int cost);
/* Extracts and returns the node with the lowest cost */
struct HeapNode pop(Heap *heap);
/* Swaps two nodes */
void swap(struct HeapNode *a, struct HeapNode *b);
/* Frees memory used by the heap */
void destroy_heap(Heap *heap);

/* Global variables */
Map *map;

/* Checks command line arguments,
   creates and verifies map structure initialization,
   finds the shortest path, prints it,
   and deallocates all memory. */
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <file_input>\n", argv[0]);
        return EXIT_FAILURE;
    }
    if (!(map = read_input(argv[1])))
    {
        fprintf(stderr, "Map init failed\n");
        return EXIT_FAILURE;
    }

    dijkstra(map);

    destroy_map(map);
    return EXIT_SUCCESS;
}

Map *read_input(char *filename)
{
    FILE *fp = fopen(filename, "r");
    Map *m = calloc(1, sizeof(Map));
    unsigned short i, j;

    if (fp && m)
    {
        fscanf(fp, "%u %u %hu %hu", &m->Ccell, &m->Cheight, &m->rows, &m->cols);
        m->matrix = calloc(m->rows, sizeof(int *));
        for (i = 0; i < m->rows; i++)
        {
            m->matrix[i] = calloc(m->cols, sizeof(int));
            for (j = 0; j < m->cols; j++)
            {
                fscanf(fp, "%d", &m->matrix[i][j]);
            }
        }

        fclose(fp);
        return m;
    }

    return NULL;
}

int destroy_map(Map *map)
{
    unsigned short i;

    if (map)
    {
        for (i = 0; i < map->rows; i++)
            free(map->matrix[i]);

        free(map->matrix);
        free(map);

        return 1;
    }

    return 0;
}

void dijkstra(Map *map)
{
    int directions[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    unsigned short i, j;

    int visited[MAX][MAX] = {0};
    Coordinate path[MAX][MAX];
    int dist[MAX][MAX];

    Heap *heap = init_heap(map->rows * map->cols);

    assert(map != NULL);

    for (i = 0; i < map->rows; i++)
    {
        for (j = 0; j < map->cols; j++)
        {
            path[i][j].x = path[i][j].y = USHRT_MAX;
            dist[i][j] = INT_MAX;
        }
    }

    dist[0][0] = 0;
    push(heap, 0, 0, map->Ccell);

    while (heap->size > 0)
    {
        struct HeapNode current = pop(heap);

        visited[current.pos.x][current.pos.y] = 1;

        for (i = 0; i < 4; i++)
        {
            int nx = current.pos.x + directions[i][0];
            int ny = current.pos.y + directions[i][1];
            if (nx >= 0 && nx < map->rows && ny >= 0 && ny < map->cols)
            {
                int height_diff = map->matrix[nx][ny] - map->matrix[current.pos.x][current.pos.y];
                int new_cost = current.cost + map->Ccell + map->Cheight * height_diff * height_diff;
                if (!visited[nx][ny] && new_cost < dist[nx][ny])
                {
                    push(heap, nx, ny, new_cost);
                    dist[nx][ny] = new_cost;
                    path[nx][ny] = current.pos;
                }
            }
        }
    }

    print_path(path, map->rows - 1, map->cols - 1);
    printf("-1 -1\n%d\n", dist[map->rows - 1][map->cols - 1]);

    destroy_heap(heap);
}

void print_path(Coordinate path[MAX][MAX], int x, int y)
{
    if (x == -1 || y == -1)
        return;

    if (path[x][y].x != USHRT_MAX || path[x][y].y != USHRT_MAX)
        print_path(path, path[x][y].x, path[x][y].y);
    printf("%d %d\n", x, y);
}

Heap *init_heap(unsigned capacity)
{
    Heap *heap = calloc(1, sizeof(Heap));

    if (!heap)
        return NULL;

    heap->nodes = calloc(capacity, sizeof(struct HeapNode));
    heap->capacity = capacity;

    return heap;
}

int push(Heap *heap, int x, int y, int cost)
{
    struct HeapNode node;
    int i;

    if (heap->size >= heap->capacity)
        return 0;

    node.pos.x = x;
    node.pos.y = y;
    node.cost = cost;

    heap->nodes[heap->size] = node;
    for (i = heap->size++; i > 0 && heap->nodes[i].cost < heap->nodes[(i - 1) / 2].cost; i = (i - 1) / 2)
        swap(&heap->nodes[i], &heap->nodes[(i - 1) / 2]);

    return 1;
}

struct HeapNode pop(Heap *heap)
{
    unsigned short i, smallest;
    struct HeapNode node;

    if (heap->size == 0)
    {
        struct HeapNode emptyNode = {{USHRT_MAX, USHRT_MAX}, INT_MAX};
        return emptyNode;
    }

    node = heap->nodes[0];
    heap->nodes[0] = heap->nodes[--heap->size];
    for (i = 0; 2 * i + 1 < heap->size; i = smallest)
    {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        smallest = left;

        if (right < heap->size && heap->nodes[right].cost < heap->nodes[left].cost)
            smallest = right;
        if (heap->nodes[i].cost > heap->nodes[smallest].cost)
            swap(&heap->nodes[i], &heap->nodes[smallest]);
    }
    return node;
}

void swap(struct HeapNode *a, struct HeapNode *b)
{
    struct HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void destroy_heap(Heap *heap)
{
    assert(heap != NULL);

    free(heap->nodes);
    free(heap);
}
