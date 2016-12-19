#include "stdlib.h"
#include "stdio.h"

#include "deque.h"


typedef struct Point {
    int x;
    int y;
    int available;
    int visited;
    int distance;
} Point;


typedef struct Map {
    Point ** field;
    int height;
    int width;
} Map;


void read_data(Map * map) {
    int height, width;
    scanf("%d %d", &height, &width);

    map->height = height;
    map->width = width;

    map->field = (Point **)malloc(sizeof(Point *) * height);
    for (int row = 0; row < height; ++row) {
        map->field[row] = (Point *)malloc(sizeof(Point) * width);
    }

    char buffer[width + 1];
    for (int row = 0; row < height; ++row) {
        scanf("%s", buffer);
        for (int column = 0; column < width; ++column) {
            Point * point = &(map->field[row][column]);

            switch(buffer[column]) {
            case '.':
                point->available = 1;
                break;
            case '#':
                point->available = 0;
                break;
            }

            point->x = row;
            point->y = column;
            point->visited = 0;
            point->distance = -1;
        }
    }
}


void add_candidate(Map * map, Deque * deque, int x, int y, int distance) {
    Point * candidate = &(map->field[x][y]);
    if (candidate->available && ! candidate->visited) {
        candidate->visited = 1;
        candidate->distance = distance;
        deque_pushback(deque, candidate);
    }
}


void mark_cells(Map * map) {
    Deque deque;
    deque_init(&deque, default_destroy_list_node);

    map->field[0][0].distance = 0;
    map->field[0][0].visited = 1;
    deque_pushfront(&deque, &(map->field[0][0]));

    Point * point;

    while ((point = deque_front(&deque)) != NULL) {
        int x = point->x;
        int y = point->y;

        deque_popfront(&deque);
        
        if (x == map->height && y == map->width) {
            break;
        }

        int distance = point->distance + 1;

        if (x > 0) {
            add_candidate(map, &deque, x - 1, y, distance);
        }

        if (x < map->height - 1) {
            add_candidate(map, &deque, x + 1, y, distance);
        }

        if (y > 0) {
            add_candidate(map, &deque, x, y - 1, distance);
        }

        if (y < map->width - 1) {
            add_candidate(map, &deque, x, y + 1, distance);
        }
    }

    deque_destroy(&deque);
}


int add_result_candidate(Map * map, Deque * deque, Point ** point, int x, int y, int distance) {
    Point * tmp = &(map->field[x][y]);
    if (tmp->distance != distance - 1) {
        return 0;
    }

    deque_pushback(deque, tmp);
    *point = tmp;

    return 1;
}


void print_solution(Map * map) {
    Point * point = &(map->field[map->height - 1][map->width - 1]);
    int distance = point->distance;

    if (distance == -1) {
        printf("No way\n");
        return;
    }

    Deque deque;
    deque_init(&deque, default_destroy_list_node);
    deque_pushback(&deque, point);
    
    while (distance > 0) {
        int x = point->x;
        int y = point->y;

        int is_added = 0;

        if (x > 0) {
            is_added = add_result_candidate(map, &deque, &point, x - 1, y, distance);
        }
        
        if (x < map->height - 1 && !is_added) {
            is_added = add_result_candidate(map, &deque, &point, x + 1, y, distance);
        }
        
        if (y > 0 && !is_added) {
            is_added = add_result_candidate(map, &deque, &point, x, y - 1, distance);
        } 
        
        if (y < map->width - 1 && !is_added) {
            is_added = add_result_candidate(map, &deque, &point, x, y + 1, distance);
        }

        --distance;
    }

    while ((point = deque_back(&deque)) != NULL) {
        deque_popback(&deque);
        printf("(%d, %d)\n", point->x, point->y);
    }

    deque_destroy(&deque);
}


void deallocate(Map * map) {
    for (int row = 0; row < map->height; ++row) {
        free(map->field[row]);
    }

    free(map->field);
}


int main() {
    Map map;

    read_data(&map);
    mark_cells(&map);
    print_solution(&map);
    deallocate(&map);

    return 0;
}
