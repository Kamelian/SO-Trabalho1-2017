//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>
#include <time.h>

#define MAX_PLAYER_NAME 100
#define PLAYER_INITIAL_HEALTH 100
#define PLAYER_INITIAL_OFFENCE 100
#define PLAYER_INITIAL_DEFENSE 100
#define PLAYER_INITIAL_VISIBILITY 1
#define PLAYER_INITIAL_CELL 0
#define PLAYER_INITIAL_OBJECT -1
#define PLAYER_INITIAL_TREASURE -1

#define MAX_CELL_DESCRIPTION 500
#define MAX_CELLS 25

#define MAX_OBJECT_NAME 20
#define MAX_OBJECTS 3

#define MAX_NAME_MONSTER 100
#define MONSTER_INITIAL_HEALTH 100
#define MONSTER_INITIAL_OFFENCE 100
#define MONSTER_INITIAL_DEFENSE 100
#define MONSTER_INITIAL_CELL 12

struct Player {
    char name[MAX_PLAYER_NAME];
    int health;
    int offence;
    int defence;
    int visibility;
    int location;
    int item1;
    int item2;
    int item3;
    int treasure;
};

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

struct Object {
    char name[MAX_OBJECT_NAME];
    int healthBonus;
    int offenceBonus;
    int defenceBonus;
    int visibilityBonus;
};

struct Monster {
    char name[MAX_NAME_MONSTER];
    int health;
    int offence;
    int defense;
    int location;
};

void PrintTitle();

//Debugging methods that should be commented
void PrintPlayer(struct Player player);
void PrintMap(struct Cell cells[], int nCells);
void PrintObject(struct Object object[], int nObjects);
void PrintMonster(struct Monster monster);
//End of Debugging methods

// Initialization
void InitializePlayer(struct Player *pplayer);
void InitializeMap(struct Cell cells[], int *pnCells); //não é preciso ponteiro, porque o vector já é um ponteiro
void InitializeObject(struct Object object[], int *pnObjects);
void InitializeMonster(struct Monster *pmonster);

// main loop
void MovePlayer(struct Player *pplayer, struct Cell cells[]);
void MoveMonster(struct Monster *pmonster, struct Cell cells[]);
void GrabObject(struct Player *pplayer, struct Cell cells[], struct Object object[]);
void Combat(struct Player *pPlayer, struct Monster *pmonster);
int EndGame(struct Player *pplayer, struct Cell cells[]);

//AUXILIAR METHODS
int RandomNumber(int numberOfRandoms);
void GrabTreasure(struct Player *pplayer, struct Cell cells[]);
void HackComputer(struct Cell cells[], struct Player *pPlayer);
int AttackMove(struct Player *pplayer, struct Monster *pmonster, int attacker);

int main() {
    setlocale(LC_ALL, "Portuguese");
    system("MODE 100,40");

    struct Player player;
    struct Cell cells[MAX_CELLS];
    struct Object objects[MAX_OBJECTS];
    struct Monster monster;

    int nCells, nObjects;

    nCells = 20;
    nObjects = 3;

    PrintTitle();

    InitializePlayer(&player);
    //PrintPlayer(player);

    InitializeMap(cells, &nCells);
    //PrintMap(cells, nCells);

    InitializeObject(objects, &nObjects);
    //PrintObject(objects, nObjects);

    InitializeMonster(&monster);
    //PrintMonster(monster);

    while (EndGame(&player, cells) == 0) {
        MovePlayer(&player, cells);
        GrabObject(&player, cells, objects);
        MoveMonster(&monster, cells);
        //combate
        //verificar fim de jogo
    }

    system("pause");

    return 0;
}

void PrintTitle() {
    printf("\n");
    printf("   /$$$$$$$$ /$$                                                                                  \n");
    printf("  |__  $$__/| $$                                                                                  \n");
    printf("     | $$   | $$$$$$$   /$$$$$$         /$$$$$$   /$$$$$$$  /$$$$$$$  /$$$$$$   /$$$$$$   /$$$$$$ \n");
    printf("     | $$   | $$__  $$ /$$__  $$       /$$__  $$ /$$_____/ /$$_____/ |____  $$ /$$__  $$ /$$__  $$\n");
    printf("     | $$   | $$  \\ $$| $$$$$$$$      | $$$$$$$$|  $$$$$$ | $$        /$$$$$$$| $$  \\ $$| $$$$$$$$\n");
    printf("     | $$   | $$  | $$| $$_____/      | $$_____/ \\____  $$| $$       /$$__  $$| $$  | $$| $$_____/\n");
    printf("     | $$   | $$  | $$|  $$$$$$$      |  $$$$$$$ /$$$$$$$/|  $$$$$$$|  $$$$$$$| $$$$$$$/|  $$$$$$$\n");
    printf("     |__/   |__/  |__/ \\_______/       \\_______/|_______/  \\_______/ \\_______/| $$____/  \\_______/\n");
    printf("                                                                              | $$                \n");
    printf("                                                                              | $$                \n");
    printf("                                                                              |__/                \n");
    printf("  ................................................................................................\n");
    printf("  ================================================================================================\n");
    printf("  ||   The noise wakes you up in your cell. Distant sounds of struggle indicate that something  ||\n");
    printf("  || serious is happening. A metallic click is heard and you realize that all the doors of your ||\n");
    printf("  ||                  cell block have been unlocked! You now have two choices:                  ||\n");
    printf("  ||                                                                                            ||\n");
    printf("  ||           Hide in the shadows of an unlocked cell, on a dangerous revolted prison          ||\n");
    printf("  ||                                            OR                                              ||\n");
    printf("  ||         Confront furious inmates and armed guards, to reach the light of the exit.         ||\n");
    printf("  ||                                                                                            ||\n");
    printf("  ||                               IT'S TIME TO MAKE YOUR CHOICE!                               ||\n");
    printf("  ================================================================================================\n\n");
}

void InitializePlayer(struct Player *pplayer) {
    printf("Enter your convict name: ");
    scanf("%s", (*pplayer).name);
    (*pplayer).health = PLAYER_INITIAL_HEALTH;
    (*pplayer).offence = PLAYER_INITIAL_OFFENCE;
    (*pplayer).defence = PLAYER_INITIAL_DEFENSE;
    (*pplayer).visibility = PLAYER_INITIAL_VISIBILITY;
    (*pplayer).location = PLAYER_INITIAL_CELL;
    (*pplayer).item1 = PLAYER_INITIAL_OBJECT;
    (*pplayer).item2 = PLAYER_INITIAL_OBJECT;
    (*pplayer).item3 = PLAYER_INITIAL_OBJECT;
    (*pplayer).treasure = PLAYER_INITIAL_TREASURE;
}

void PrintPlayer(struct Player player) {
    printf("\n==== PLAYER ====");
    printf("\nName: %s", player.name);
    printf("\nHealth: %d", player.health);
    printf("\nOffence: %d", player.offence);
    printf("\nDefense: %d", player.defence);
    printf("\nVisibility: %d", player.visibility);
    printf("\nLocation: %d", player.location);
    printf("\nObject1: %d", player.item1);
    printf("\nObject2: %d", player.item2);
    printf("\nObject3: %d", player.item3);
    printf("\nTreasure: %d", player.treasure);
    printf("\n\n");
}

void InitializeMap(struct Cell cells[], int *pnCells) {
    *pnCells = 20; //define o nº de células do mapa

    // Cell 0 - Maximum 1
    cells[0].north = -1;
    cells[0].south = 3;
    cells[0].west = -1;
    cells[0].east = -1;
    cells[0].up = -1;
    cells[0].down = -1;
    strcpy(cells[0].cellDescription, "\nYou are in the familiar confines of your cell.\n\nAfter nine years watching the sky through those bars,you still ask yourself:\n\"Why do i have to pay for another men's crime.\"\n\n");
    cells[0].object = -1;
    cells[0].treasure = -1;

    // Cell 1 - Maximum 2
    cells[1].north = -1;
    cells[1].south = -1;
    cells[1].west = -1;
    cells[1].east = 3;
    cells[1].up = -1;
    cells[1].down = 14; // Tunel 1
    strcpy(cells[1].cellDescription, "\nYou are in maximum security cell n.2.\n\nThe cell organization of the bank robber, whom you consider your only\nfriend in this hell hole, fell's upon you.\nBut you notice something strange!\n\n");
    cells[1].object = -1;
    cells[1].treasure = -1;

    // Cell 2 - Maximum 3
    cells[2].north = 3;
    cells[2].south = -1;
    cells[2].west = -1;
    cells[2].east = -1;
    cells[2].up = -1;
    cells[2].down = -1;
    strcpy(cells[2].cellDescription, "\nYou are in maximum security cell n.3.\n\nThe bunk bed was put up against the wall. \nAt first you do not understand why, but as you study the cell, \nyou realize that there was a purpose for this action.\n");
    cells[2].object = -1;
    cells[2].treasure = -1;

    // Cell 3 - Maximum security block
    cells[3].north = 0;
    cells[3].south = 2;
    cells[3].west = 1;
    cells[3].east = 4;
    cells[3].up = 18; // Airduct 1
    cells[3].down = -1;
    strcpy(cells[3].cellDescription, "\nYou are in maximum security block.\n\n");
    cells[3].object = -1;
    cells[3].treasure = -1;

    // Cell 4 - The yard
    cells[4].north = -1;
    cells[4].south = 9;
    cells[4].west = 3;
    cells[4].east = 5;
    cells[4].up = -1;
    cells[4].down = 17; // Tunel 4
    strcpy(cells[4].cellDescription, "Estás no pátio da prisão");
    cells[4].object = -1;
    cells[4].treasure = -1;

    // Cell 5 - Minimum Security Wing
    cells[5].north = 6;
    cells[5].south = 8;
    cells[5].west = 4;
    cells[5].east = 7;
    cells[5].up = -1;
    cells[5].down = -1;
    strcpy(cells[5].cellDescription, "Estás na ala de segurança mínima.");
    cells[5].object = -1;
    cells[5].treasure = -1;

    // Cell 6 - Minimum 1
    cells[6].north = -1;
    cells[6].south = 5;
    cells[6].west = -1;
    cells[6].east = -1;
    cells[6].up = -1;
    cells[6].down = -1;
    strcpy(cells[6].cellDescription, "Cela Minima 1");
    cells[6].object = -1;
    cells[6].treasure = -1;

    // Cell 7 - Minimum 2
    cells[7].north = -1;
    cells[7].south = -1;
    cells[7].west = 5;
    cells[7].east = -1;
    cells[7].up = -1;
    cells[7].down = -1;
    strcpy(cells[7].cellDescription, "\nYou are in minimum security cell n.2.\nA scent of death attacks your nostrils.\nFull of wounds, he sees the lifeless body of the infamous prisoner of the cell 3.\nChildren killers are not tolerated by the other prisoners.\nA metallic gleam captures your attention! a sharp screwdriver!\nThis can be a good weapon to help you.\n\n");
    cells[7].object = 0; //Sharp screwdriver
    cells[7].treasure = -1;

    // Cell 8 - Minimum 3
    cells[8].north = 5;
    cells[8].south = -1;
    cells[8].west = -1;
    cells[8].east = -1;
    cells[8].up = -1;
    cells[8].down = 16; // Tunel 3
    strcpy(cells[8].cellDescription, "Cela Mínima 3");
    cells[8].object = -1;
    cells[8].treasure = -1;

    // Cell 9 - Corridor
    cells[9].north = 4;
    cells[9].south = 12;
    cells[9].west = 10;
    cells[9].east = 11;
    cells[9].up = -1;
    cells[9].down = -1;
    strcpy(cells[9].cellDescription, "Corredor");
    cells[9].object = -1;
    cells[9].treasure = -1;

    // Cell 10 - Laundry
    cells[10].north = -1;
    cells[10].south = -1;
    cells[10].west = -1;
    cells[10].east = 9;
    cells[10].up = 19; // Airduct 2
    cells[10].down = -1;
    strcpy(cells[10].cellDescription, "Estás na Lavandaria");
    cells[10].object = 2; //uniform
    cells[10].treasure = -1;

    // Cell 11 - Mess hall
    cells[11].north = -1;
    cells[11].south = -1;
    cells[11].west = 9;
    cells[11].east = -1;
    cells[11].up = -1;
    cells[11].down = -1;
    strcpy(cells[11].cellDescription, "Estás no refeitório");
    cells[11].object = 1; // Metal tray
    cells[11].treasure = -1;

    // Cell 12 - Guards room
    cells[12].north = 9;
    cells[12].south = -1;
    cells[12].west = -1;
    cells[12].east = -1;
    cells[12].up = -1;
    cells[12].down = -1;
    strcpy(cells[12].cellDescription, "Estás na sala dos guardas");
    cells[12].object = -1;
    cells[12].treasure = -1;

    // Cell 13 - Exit Gate
    cells[13].north = -1;
    cells[13].south = -1;
    cells[13].west = -1;
    cells[13].east = -1;
    cells[13].up = -1;
    cells[13].down = 15; //Tunel 2
    strcpy(cells[13].cellDescription, "Estás em frente ao portão de saída");
    cells[13].object = -1;
    cells[13].treasure = -1;

    // Cell 14 - Tunel 1
    cells[14].north = -1;
    cells[14].south = 15;
    cells[14].west = -1;
    cells[14].east = -1;
    cells[14].up = 1;
    cells[14].down = -1;
    strcpy(cells[14].cellDescription, "Estás no tunel 1");
    cells[14].object = -1;
    cells[14].treasure = -1;

    // Cell 15 - Tunel 2
    cells[15].north = 14;
    cells[15].south = -1;
    cells[15].west = -1;
    cells[15].east = -1;
    cells[15].up = 10; // laudry
    cells[15].down = -1;
    strcpy(cells[15].cellDescription, "Estás no tunel 2");
    cells[15].object = -1;
    cells[15].treasure = -1;

    // Cell 16 - Tunel 3
    cells[16].north = -1;
    cells[16].south = -1;
    cells[16].west = 17;
    cells[16].east = -1;
    cells[16].up = 8; // Minimum 3
    cells[16].down = -1;
    strcpy(cells[16].cellDescription, "Estás no tunel 3");
    cells[16].object = -1;
    cells[16].treasure = -1;

    // Cell 17 - Tunel 4
    cells[17].north = -1;
    cells[17].south = -1;
    cells[17].west = -1;
    cells[17].east = 16;
    cells[17].up = 4; // Yard
    cells[17].down = -1;
    strcpy(cells[17].cellDescription, "Estás no tunel 4");
    cells[17].object = -1;
    cells[17].treasure = -1;

    // Cell 18 - Airduct 1
    cells[18].north = -1;
    cells[18].south = 19;
    cells[18].west = -1;
    cells[18].east = -1;
    cells[18].up = -1;
    cells[18].down = 3; // Maximum 3
    strcpy(cells[18].cellDescription, "Estás na conduta de ventilação 1");
    cells[18].object = -1;
    cells[18].treasure = -1;

    // Cell 19 - Airduct 2
    cells[19].north = 18;
    cells[19].south = -1;
    cells[19].west = -1;
    cells[19].east = -1;
    cells[19].up = -1;
    cells[19].down = 10; // Laundry room
    strcpy(cells[19].cellDescription, "Estás na conduta de ventilação 2");
    cells[19].object = -1;
    cells[19].treasure = -1;
}

void PrintMap(struct Cell cells[], int nCells) {
    int i;
    for (i = 0; i < nCells; i++) {
        printf("\n==== CELL %i - %s ====", i, cells[i].cellDescription);
        printf("\nUp: %i", cells[i].up);
        printf("\nNorth: %i\tWest: %i\tSouth: %i\tEast: %i", cells[i].north, cells[i].west, cells[i].south, cells[i].east);
        printf("\nDown: %i", cells[i].down);
        printf("\nObject: %i", cells[i].object);
        printf("\nTresaure: %i", cells[i].treasure);
        printf("\n\n");
    }
}

void InitializeObject(struct Object object[], int *pnObjects) {
    // Object 0
    strcpy(object[0].name, "Sharp screwdriver");
    object[0].healthBonus = 0;
    object[0].offenceBonus = 50;
    object[0].defenceBonus = 0;
    object[0].visibilityBonus = 0;

    // Object 1
    strcpy(object[1].name, "Metal tray");
    object[1].healthBonus = 0;
    object[1].offenceBonus = 0;
    object[1].defenceBonus = 50;
    object[1].visibilityBonus = 0;

    // Object 2
    strcpy(object[2].name, "prision guard uniform");
    object[2].healthBonus = 0;
    object[2].offenceBonus = 0;
    object[2].defenceBonus = 0;
    object[2].visibilityBonus = -1;
}

void PrintObject(struct Object object[], int nObjects) {
    int i;
    for (i = 0; i < nObjects; i++) {
        printf("\n==== OBJECT %i ====", i);
        printf("\nName: %s", object[i].name);
        printf("\nHealth Bonus: %i", object[i].healthBonus);
        printf("\nOffence Bonus: %i", object[i].offenceBonus);
        printf("\nDefense Bonus: %i", object[i].defenceBonus);
        printf("\nVisibility Bonus: %i", object[i].visibilityBonus);
        printf("\n\n");
    }
}

void InitializeMonster(struct Monster *pmonster) {
    strcpy((*pmonster).name, "The Jailer!");
    (*pmonster).health = MONSTER_INITIAL_HEALTH;
    (*pmonster).offence = MONSTER_INITIAL_OFFENCE;
    (*pmonster).defense = MONSTER_INITIAL_DEFENSE;
    (*pmonster).location = MONSTER_INITIAL_CELL;
}

void PrintMonster(struct Monster monster) {
    printf("\n==== MONSTER ====");
    printf("\nName: %s", monster.name);
    printf("\nHealth: %d", monster.health);
    printf("\nOffence: %d", monster.offence);
    printf("\nDefence: %d", monster.defense);
    printf("\nCell: %d", monster.location);
    printf("\n\n");
}

void MovePlayer(struct Player *pplayer, struct Cell cells[]) {
    char moveOptions[60] = "What door do you want to cross?";
    char moveNorth[5] = " [N]";
    char moveEast[5] = " [E]";
    char moveSouth[5] = " [S]";
    char moveWest[5] = " [W]";
    char moveUp[5] = " [U]";
    char moveDown[5] = " [D]";
    char choosenDoor;
    char possibleChoices[12] = "";

    if(cells[(*pplayer).location].north != -1) {
        strcat(moveOptions, moveNorth);
        strcat(possibleChoices, "nN");
    }
    if(cells[(*pplayer).location].east != -1) {
        strcat(moveOptions, moveEast);
        strcat(possibleChoices, "eE");
    }
    if(cells[(*pplayer).location].south != -1) {
        strcat(moveOptions, moveSouth);
        strcat(possibleChoices, "sS");
    }
    if(cells[(*pplayer).location].west != -1) {
        strcat(moveOptions, moveWest);
        strcat(possibleChoices, "wW");
    }
    if(cells[(*pplayer).location].up != -1) {
        strcat(moveOptions, moveUp);
        strcat(possibleChoices, "uU");
    }
    if(cells[(*pplayer).location].down != -1) {
        strcat(moveOptions, moveDown);
        strcat(possibleChoices, "dD");
    }

    fflush(stdin);
    printf("\n%s : ", moveOptions);
    scanf("%c", &choosenDoor);

    if(strchr(possibleChoices, choosenDoor) == 0) {
        printf("There is no door in that direction!\n");
    } else if (choosenDoor == 'N' || choosenDoor == 'n') {
        (*pplayer).location = cells[(*pplayer).location].north;
    } else if (choosenDoor == 'E' || choosenDoor == 'e') {
        (*pplayer).location = cells[(*pplayer).location].east;
    } else if (choosenDoor == 'S' || choosenDoor == 's') {
        (*pplayer).location = cells[(*pplayer).location].south;
    } else if (choosenDoor == 'W' || choosenDoor == 'w') {
        (*pplayer).location = cells[(*pplayer).location].west;
    } else if (choosenDoor == 'U' || choosenDoor == 'u') {
        (*pplayer).location = cells[(*pplayer).location].up;
    } else if (choosenDoor == 'D' || choosenDoor == 'd') {
        (*pplayer).location = cells[(*pplayer).location].down;
    }

    printf("\n%s", cells[(*pplayer).location].cellDescription);

    //O método GRABOBJECT podia ser chamado aqui dentro??
    //Assim o GRABOBJECT não precisava de ser executado em cada iteração do loop while que existe no main.

    //O método GRABTRESAURE podia ser chamado aqui e o GRABTRESAURE chamava o HACK COMPUTER?
    //Parece fazer mais sentido com o enunciado, o jogador apanha o tesouro (só quando o encontra), não existe condição a correr no while Loop do main
    //e só depois é chamada uma função auxiliar que é o HackComputer



    if (pplayer->location == 12) {
        HackComputer(cells, &*pplayer);
    }
}

void GrabObject(struct Player *pplayer, struct Cell cells[], struct Object object[]) {
    if(cells[(*pplayer).location].object != -1) {
        printf("You have found a %s\n", object[cells[(*pplayer).location].object].name );

        (*pplayer).offence += object[cells[(*pplayer).location].object].offenceBonus;
        (*pplayer).defence += object[cells[(*pplayer).location].object].defenceBonus;
        (*pplayer).visibility += object[cells[(*pplayer).location].object].visibilityBonus;

        printf("Has you grab the item you feel a new strenght\n");
        printf("Offence: %d\tDefence: %d\t Visibility:%d\n", (*pplayer).offence, (*pplayer).defence, (*pplayer).visibility);

        if((*pplayer).item1 == -1) {
            (*pplayer).item1 = cells[(*pplayer).location].object;
        } else if((*pplayer).item2 == -1) {
            (*pplayer).item2 = cells[(*pplayer).location].object;
        } else if((*pplayer).item3 == -1) {
            (*pplayer).item3 = cells[(*pplayer).location].object;
        }
        PrintPlayer(*pplayer);  //DEBUGGING
    }
}

void MoveMonster(struct Monster *pmonster, struct Cell cells[]) {

    printf("\nOrigem do monstro:%s\n", cells[(*pmonster).location].cellDescription); //DEBUGGING

    char possibleChoices[6] = "";
    int numberOfExits;
    char choosenDoor[1] = "";

    if(cells[(*pmonster).location].north != -1) {strcat(possibleChoices, "N");}
    if(cells[(*pmonster).location].east != -1) {strcat(possibleChoices, "E");}
    if(cells[(*pmonster).location].south != -1) {strcat(possibleChoices, "S");}
    if(cells[(*pmonster).location].west != -1) {strcat(possibleChoices, "W");}
    if(cells[(*pmonster).location].up != -1) {strcat(possibleChoices, "U");}
    if(cells[(*pmonster).location].down != -1) {strcat(possibleChoices, "D");}

    numberOfExits = strlen(possibleChoices);

    //printf("\nNumber of exits:%d\n", numberOfExits); //DEBUGGING
    int choosenExit = RandomNumber(numberOfExits);
    //printf("\nChoosen exit:%d\n", choosenExit); //DEBUGGING

    fflush(stdin);
    strncpy(choosenDoor, possibleChoices+choosenExit, 1);
    //printf("\nChoosen Door:%s\n", choosenDoor);  //DEBUGGING

    if (*choosenDoor == 'N') {
        (*pmonster).location = cells[(*pmonster).location].north;
    } else if (*choosenDoor == 'E') {
        (*pmonster).location = cells[(*pmonster).location].east;
    } else if (*choosenDoor == 'S') {
        (*pmonster).location = cells[(*pmonster).location].south;
    } else if (*choosenDoor == 'W') {
        (*pmonster).location = cells[(*pmonster).location].west;
    } else if (*choosenDoor == 'U') {
        (*pmonster).location = cells[(*pmonster).location].up;
    } else if(*choosenDoor == 'D') {
        (*pmonster).location = cells[(*pmonster).location].down;
    }

    printf("\nDestino do monstro:%s\n", cells[(*pmonster).location].cellDescription); //DEBUGGING
}

void combat(struct Player *pplayer, struct Monster *pmonster) {
    if((*pplayer).location==(*pmonster).location && (*pplayer).visibility > 0) {
        printf("In the middle of the confusion your bitter enemy %s has found you.\nA fight between the two of you is inevitable", (*pmonster).name);
        int damage = 0;
        int roundCounter = 0;
        do{
            damage = AttackMove(*pplayer, *pmonster, roundCounter%2);

            if((*pplayer).health == 0){
                printf("\nThe fight was brutal! And in the end you lie back in your cell in a pool of your own blood. Your attempt to escape has been brought to an end. \nBut at least you live to fight another day.");
                printf("\n\n\nTHE END");
            }else if((*pmonster).health == 0){
                printf("\nThe fight was brutal! But the Jailer lies on the floor in a pool of is own blood. Your attempt to escape has just become a little more easier. \nYou start to feel that maybe you'll the little of day!");
            }else if(roundCounter%2 == 0){ // roundCounter % 2 = 0, monster attacks
                printf("The jailer throws at you. You prepare to receive the blow.");
                (*pplayer).health -= damage;
                printf("\nYou get a hard blow! \nYou feel %d of your health leave you. Now you only have %d health\n", damage, (*pplayer).health);
            }else if(roundCounter%2 == 0){ // roundCounter % 2 = 1, player attacks
                (*pmonster).health -= damage;
                printf("You hit him with a well-placed blow! \nHe looks more tired, like %d of is health left him. Now the Jailer only has %d health", damage, (*pplayer).health);
            }
        }while ((*pplayer).health != 0 || (*pmonster).health != 0);
    } else if((*pplayer).location==(*pmonster).location && (*pplayer).visibility == 0) {
        printf("In the middle of the confusion your bitter enemy %s has found you.\nBut your disguise has a prison guard, allows you to slip his wrath.", (*pmonster).name);
    }
}

int EndGame(struct Player *pplayer, struct Cell cells[]) {
    if( (*pplayer).treasure == 1 && (*pplayer).location == 13 ) {
        return 1;
    } else {
        return 0;
    }
}

/*AUXILIAR METHODOS*/

int RandomNumber(int numberOfRandoms) {
    srand(time(0)); //use current time as seed for random generator
    int randomNumber = 0;
    randomNumber = rand() % numberOfRandoms;
    //printf("Random number is:%d\n", randomNumber);  //DEBUGGING
    return randomNumber;
}

void GrabTreasure(struct Player *pplayer, struct Cell cells[]) {
    if(cells[(*pplayer).location].treasure == 0) {
        //A function that runs the computer ideia

    }
}

void HackComputer(struct Cell cells[], struct Player *pPlayer) {
    printf("Hack as started\n");
    char date[10];
    char buffer[20];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    int i, n;

    i = tm.tm_mon + 1;
    n = tm.tm_mday;

    itoa(i,buffer,10); //converts int to string
    strcpy(date,buffer); //copies converteds int to a string
    itoa(n,buffer,10); //converts int to string
    strcat(date,buffer); //concatenates int to string
    //printf("\n\nDATE: %s", date);  //DEBUGGING

    int counter = 3;

    do {
        char answer[255];
        printf("Insert personal code: \n");
        scanf("%s", &answer);

        if (strcmp(answer, date)!=0) {
            printf("Wrong code\n");
            counter--;
            printf("\nYou have %d tries\n", counter);
        } else if(strcmp(answer, date)==0) {
            printf("Accepted code\n");
            cells[12].west = 13;
            pPlayer->treasure = 1;
            counter=0;
            printf("A DOOR JUST OPEN");
        } else {
            printf("HACK FALIED!");
        }

    } while (counter>0);

}

int AttackMove(struct Player *pplayer, struct Monster *pmonster, int attacker){ //só quero ler os valores

    int multiplier = RandomNumber(6);
    printf("\nMULTIPLIER IS:%d\n", multiplier);  //DEBUGGING
    int damage = 0;

    if (attacker == 1){
        damage = ((player).offence / 10) * multiplier - (((monster).defense/100)) * (multiplier + 5);
        printf("Player makes a damage of %d\n", damage);
    }else{
        damage = ((monster).offence / 10) * multiplier - (((player).defence/100)) * (multiplier + 5);
        printf("Monster makes a damage of %d\n", damage);
    }

    return damage;
}
