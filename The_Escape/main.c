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

//void PrintTitle();
//void PrintTitleText();

//Debugging methods that should be commented
void PrintPlayer(struct Player player);
void PrintMap(struct Cell cells[], int nCells);
void PrintObject(struct Object object[], int nObjects);
void PrintMonster(struct Monster monster);
//End of Debugging methods

// Initialization
void InitializePlayer(struct Player *pplayer, char *argv[], int argc);
int InitializeMap(struct Cell cells[]); //não é preciso ponteiro, porque o vector já é um ponteiro
int InitializeObject(struct Object object[]); //, int *pnObjects);
void InitializeMonster(struct Monster *pmonster);

// main loop

void PrintTitle();
void PrintTitleText();

void MovePlayer(struct Player *pplayer, struct Cell cells[], struct Object objects[]);
void MoveMonster(struct Monster *pmonster, struct Cell cells[]);
void GrabObject(struct Player *pplayer, struct Cell cells[], struct Object object[]);
void addObjectToPlayer(struct Player *pPlayer, struct Object object[], int idObject);
void Combat(struct Player *pPlayer, struct Monster *pmonster);
int EndGame(struct Player *pplayer, struct Cell cells[]);

//AUXILIAR METHODS
int RandomNumber(int numberOfRandoms);
void GrabTreasure(struct Player *pplayer, struct Cell cells[]);
void HackComputer(struct Cell cells[], struct Player *pPlayer);
int AttackMove(struct Player *pplayer, struct Monster *pmonster, int attacker);

int superFlag = 0;

int main(int argc, char *argv[]) {

    setlocale(LC_ALL, "Portuguese");
    system("MODE 103,1040");

    struct Player player;
    struct Cell cells[MAX_CELLS];
    struct Object objects[MAX_OBJECTS];
    struct Monster monster;

    int nCells, nObjects;

    //nObjects = 3;

    PrintTitle();
    PrintTitleText();

    nCells = InitializeMap(cells);
    //PrintMap(cells, nCells);

    InitializePlayer(&player, argv, argc);
    //PrintPlayer(player);

    nObjects = InitializeObject(objects); //, &nObjects);
    //PrintObject(objects, nObjects);


    //addObjectToPlayer(&player, objects, atoi(argv[4]));

    InitializeMonster(&monster);
    //PrintMonster(monster);

    while (EndGame(&player, cells) == 0) {
        MovePlayer(&player, cells, objects);
        MoveMonster(&monster, cells);
        Combat(&player, &monster);
        //ClearScreen();
    }

    return 0;
}

void PrintTitle() {
    FILE *f;
    f = fopen("title.txt", "r");
    char line[103];
    while(fgets(line, sizeof(line),f) != NULL){
        printf("%s", line);
    }
}

void PrintTitleText() {
    FILE *f;
    f = fopen("title_text.txt", "r");
    char line[103];
    while(fgets(line, sizeof(line),f) != NULL){
        printf("%s", line);
    }
}

void ClearScreen(){ //NOT USED APAGAR
    int line;
    int collumn;
    for (line = 0; line <=50; line++){
        for(collumn = 0; collumn <= 100; collumn++){
            printf("%c",' ');
        }
        printf("\n");
    }
}

void InitializePlayer(struct Player *pplayer, char *argv[], int argc) {
    printf("\n\n  Enter your convict name: ");
    fflush(stdin);
    scanf("%s", (*pplayer).name);

    if(argv[1]!= '\0' && atoi(argv[1]) == 1234 && argc == 5){
        (*pplayer).health = atoi(argv[2]);
        (*pplayer).location = atoi(argv[3]);
        (*pplayer).item1 = atoi(argv[4]);
        superFlag =1;
    }else{
        (*pplayer).health = PLAYER_INITIAL_HEALTH;
        (*pplayer).location = PLAYER_INITIAL_CELL;
        (*pplayer).item1 = PLAYER_INITIAL_OBJECT;
    }
        (*pplayer).offence = PLAYER_INITIAL_OFFENCE;
        (*pplayer).defence = PLAYER_INITIAL_DEFENSE;
        (*pplayer).visibility = PLAYER_INITIAL_VISIBILITY;
        (*pplayer).item2 = PLAYER_INITIAL_OBJECT;
        (*pplayer).item3 = PLAYER_INITIAL_OBJECT;
        (*pplayer).treasure = PLAYER_INITIAL_TREASURE;
    //PrintPlayer(*pplayer);
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

int InitializeMap(struct Cell cells[]) {
    FILE *f;

    f = fopen("map.txt", "r");
    int nCells = 0;
    char line[500];

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

int InitializeObject(struct Object object[]){ //, int *pnObjects) {
    int nObjects = 0;

    // Object 0
    strcpy(object[0].name, "Sharp screwdriver");
    object[0].healthBonus = 0;
    object[0].offenceBonus = 50;
    object[0].defenceBonus = 0;
    object[0].visibilityBonus = 0;
    nObjects++;

    // Object 1
    strcpy(object[1].name, "Metal tray");
    object[1].healthBonus = 0;
    object[1].offenceBonus = 0;
    object[1].defenceBonus = 50;
    object[1].visibilityBonus = 0;
    nObjects++;

    // Object 2
    strcpy(object[2].name, "prision guard uniform");
    object[2].healthBonus = 0;
    object[2].offenceBonus = 0;
    object[2].defenceBonus = 0;
    object[2].visibilityBonus = -1;
    nObjects++;

    return nObjects;;
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

void MovePlayer(struct Player *pplayer, struct Cell cells[], struct Object objects[]) {

    printf("\n  ##################################################################################################  \n");

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

    GrabObject(&*pplayer,cells, objects);

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

        addObjectToPlayer(&*pplayer, object, cells[(*pplayer).location].object);

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

void addObjectToPlayer(struct Player *pplayer, struct Object object[], int idObject){
        if(superFlag == 1){
            (*pplayer).offence += object[idObject].offenceBonus;
            (*pplayer).defence += object[idObject].defenceBonus;
            (*pplayer).visibility += object[idObject].visibilityBonus;
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

void Combat(struct Player *pplayer, struct Monster *pmonster) {
    if((*pplayer).location==(*pmonster).location && (*pplayer).visibility > 0) {
        printf("In the middle of the confusion your bitter enemy %s has found you.\nA fight between the two of you is inevitable", (*pmonster).name);
        int damage = 0;
        int roundCounter = 0;

        do{
            damage = AttackMove(&*pplayer, &*pmonster, roundCounter%2);


            if(roundCounter%2 == 0){ // roundCounter % 2 = 0, monster attacks
                printf("The jailer throws at you. You prepare to receive the blow.");
                (*pplayer).health -= damage;
                printf("\nYou get a hard blow! \nYou feel %d of your health leave you. Now you only have %d health\n", damage, (*pplayer).health);
            }else if(roundCounter%2 == 1){ // roundCounter % 2 = 1, player attacks
                (*pmonster).health -= damage;
                printf("You hit him with a well-placed blow! \nHe looks more tired, like %d of is health left him. Now the Jailer only has %d health", damage, (*pplayer).health);
            }

        }while ((*pplayer).health > 0 && (*pmonster).health > 0);

        if((*pplayer).health <= 0){
            printf("\nThe fight was brutal! And in the end you lie back in your cell in a pool of your own blood. Your attempt to escape has been brought to an end. \nBut at least you live to fight another day.");
            printf("\n\n\nTHE END");
        }else if((*pmonster).health <= 0){
            printf("\nThe fight was brutal! But the Jailer lies on the floor in a pool of is own blood. Your attempt to escape has just become a little more easier. \nYou start to feel that maybe you'll the little of day!");
        }

    } else if((*pplayer).location==(*pmonster).location && (*pplayer).visibility == 0) {
        printf("In the middle of the confusion your bitter enemy %s has found you.\nBut your disguise has a prison guard, allows you to slip his wrath.", (*pmonster).name);
    }
}

int EndGame(struct Player *pplayer, struct Cell cells[]) {
    if( (*pplayer).treasure == 1 && (*pplayer).location == 13 ) {
        return 1;
    }
    if ((*pplayer).health <=0){
        return 1;
    }
    else {
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
    printf("Remenber people will allways be people\n");
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
        scanf("%s", answer);

        if (strcmp(answer, date)!=0) {
            printf("INCORRECT CODE\n");
            counter--;
            printf("\nYou have %d tries\n", counter);
        } else if(strcmp(answer, date)==0) {
            printf("CORREC CODE\n");
            cells[12].west = 13;
            pPlayer->treasure = 1;
            counter=0;
            printf("|nYou ear a loud unocking sound from outside the guard's room. You've just got the code right and the exit gate his opening.");

            //MUDAR DESCRIÇÂO DA CELULA 13

        } else {
            printf("\nHACK FALIED!");
        }

    } while (counter>0);

}

int AttackMove(struct Player *pplayer, struct Monster *pmonster, int attacker){ //só quero ler os valores

    int multiplier = RandomNumber(6);
    printf("\nMULTIPLIER IS:%d\n", multiplier);  //DEBUGGING
    int damage = 0;

    if (attacker == 1){
        damage = ((*pplayer).offence / 10) * multiplier - (((*pmonster).defense/100)) * (multiplier + 5);
        printf("Player makes a damage of %d\n", damage);
    }else{
        damage = ((*pmonster).offence / 10) * multiplier - (((*pplayer).defence/100)) * (multiplier + 5);
        printf("Monster makes a damage of %d\n", damage);
    }

    return damage;
}
