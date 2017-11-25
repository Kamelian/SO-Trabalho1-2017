#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>
#include <time.h>

#define MAX_CELL_DESCRIPTION 500
#define MAX_CELLS 25

struct Cell {
    int north;
    int west;
    int south;
    int east;
    int up;
    int down;
    char cellDescription[MAX_CELL_DESCRIPTION];
    int object;
    int treasure;
};

int InitializeMapFile(struct Cell cells[]);
void PrintMap(struct Cell cells[], int nCells);

int main()
{
    struct Cell cells[MAX_CELLS];
    int nCells;
    nCells = InitializeMapFile(cells);

    PrintMap(cells, nCells);

    return 0;
}

int InitializeMapFile(struct Cell cells[]){
    FILE *f;

    f = fopen("map.txt", "r");
    int nCells = 0;
    char line[255];

    while(fscanf(f, "%d %d %d %d %d %d %d %d\n",
                 &cells[nCells].north,
                 &cells[nCells].south,
                 &cells[nCells].west,
                 &cells[nCells].east,
                 &cells[nCells].up,
                 &cells[nCells].down,
                 &cells[nCells].object,
                 &cells[nCells].treasure) != EOF){

        strcpy(cells[nCells].cellDescription, "");
        while(fgets(line, sizeof(line),f) != NULL){
            if(strcmp(line, "\n")!=0){
                strcat(cells[nCells].cellDescription, line);
            }else{
                break;
            }
        }
        cells[nCells].cellDescription[strlen(cells[nCells].cellDescription) - 1] = 0;
        //printf("\n %s\n", cells[nCells].cellDescription);
        nCells++;
    }

    fclose(f);
    return nCells;
}

void PrintMap(struct Cell cells[], int nCells) {
    int i;
    for (i = 0; i < nCells; i++) {
        printf("\n==== CELL %i ====", i);
        printf("\nDescription: %s", cells[i].cellDescription);
        printf("\nNorth: %i", cells[i].north);
        printf("\nEast: %i", cells[i].east);
        printf("\nSouth: %i", cells[i].south);
        printf("\nWest: %i", cells[i].west);
        printf("\nUp: %i", cells[i].up);
        printf("\nDown: %i", cells[i].down);
        printf("\nObject: %i", cells[i].object);
        printf("\nTresaure: %i", cells[i].treasure);
        printf("\n\n");
    }
}
