#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "data.h" // NEMENIT, inak vas kod neprejde testom !!!

// chybove hlasenia
#define ERR_UNIT_COUNT "ERR_UNIT_COUNT"
#define ERR_ITEM_COUNT "ERR_ITEM_COUNT"
#define ERR_WRONG_ITEM "ERR_WRONG_ITEM"
#define ERR_SLOTS "ERR_SLOTS"

#define MAX_UNITS 5
#define MAX_POSITION 5
UNIT army_1[MAX_UNITS]; //the max amount of units
UNIT army_2[MAX_UNITS];
int amount_1, amount_2; //amount of units
bool is_game_over = false;

typedef struct {
    UNIT * unit;
    int alive;
} POSITION;

POSITION battlefield_1[MAX_POSITION]; // position from 0 to 4
POSITION battlefield_2[MAX_POSITION];

int max(int a, int b) { //searching for the maximum
    return (a > b) ? a : b;
}

void place_units_on_battlefield(UNIT * army, int amount, POSITION * battlefield) {
    for (int i = 0; i < amount; i++) {
        // place units on the battlefield
        battlefield[i].unit = &army[i];  // pointer to a unit
        battlefield[i].alive = 1;        // unit is alive
    }
}

ITEM * find_item(char * item_name) { //func to find an item
    for (int i = 0; i < NUMBER_OF_ITEMS; i++) { //checking all items
        if (strcmp(item_name, items[i].name) == 0) { //if it matches
            return (ITEM *)&items[i]; //return a pointer to the found item
        }
    }
    printf("%s\n", ERR_WRONG_ITEM); //error
    return 0;
}

void introduce_armies(UNIT * army, int amount, int army_num) {
    printf("Army %d\n", army_num); //number of the army
    for (int i = 0; i < amount; i++) { //to print all units
        printf("%*sUnit: %d\n", 4, "", i);
        printf("%*sName: %s\n",4,"", army[i].name);
        printf("%*sHP: %d\n", 4, "", army[i].hp);
        printf("%*sItem 1: %s,%d,%d,%d,%d,%d\n", 4, "", army[i].item1->name, army[i].item1->att, army[i].item1->def,
            army[i].item1->slots, army[i].item1->range, army[i].item1->radius);
        if (army[i].item2 != NULL) {
            printf("%*sItem 2: %s,%d,%d,%d,%d,%d\n", 4, "", army[i].item2->name, army[i].item2->att, army[i].item2->def,
            army[i].item2->slots, army[i].item2->range, army[i].item2->radius);
        }
        putchar('\n');
    }
}

int create_armada_1() {
    int amount; //amount of units
    char buffer[MAX_NAME+1];
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d", &amount);
    if (amount < 1 || amount > 5) { //invalid numbers
        printf("%s\n", ERR_UNIT_COUNT);
        return 0;
    }
    else {
        for (int i = 0; i < amount; i++) {
            char line[4 * MAX_NAME + 1]; //so the size would match
            fgets(line, sizeof(line), stdin); // reading all line

            char item1[MAX_NAME + 1] = "", item2[MAX_NAME + 1] = "", item3[MAX_NAME + 1] = "";
            int count = sscanf(line, "%s %s %s %s", army_1[i].name, item1, item2, item3);
            if (count < 2 || count > 3) {
                printf("%s\n",ERR_ITEM_COUNT);
                return 0;
            }
            if (count == 2) {
                item2[0] = '\0'; // then there is not a second item
            }
            army_1[i].item1 = find_item(item1);
            if (army_1[i].item1 == NULL) {
                return 0;
            }

            if (item2[0] != '\0') {     // if there was a second item
                army_1[i].item2 = find_item(item2);
            } else {
                army_1[i].item2 = NULL; //no second item
            }

            int total_slots = army_1[i].item1->slots;
            if (army_1[i].item2 != NULL) { //if there is a second item
                total_slots += army_1[i].item2->slots;
            }
            if (total_slots > 2) { //MAX number of slots is 2
                printf("%s\n",ERR_SLOTS);
                return 0;
            }
            army_1[i].hp = 100;
        }
        amount_1 = amount;
    }
    return 1;
}

int create_armada_2() {
    int amount;
    char buffer[MAX_NAME+1];
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d", &amount);
    if (amount < 1 || amount > 5) {
        printf("%s\n", ERR_UNIT_COUNT);
        return 0;
    }
    else {
        for (int i = 0; i < amount; i++) {
            char line[4 * MAX_NAME + 1]; //so the size would match
            fgets(line, sizeof(line), stdin); // reading all line

            char item1[MAX_NAME + 1] = "", item2[MAX_NAME + 1] = "", item3[MAX_NAME + 1] = ""; //item3 to check for errors
            int count = sscanf(line, "%s %s %s %s", army_2[i].name, item1, item2, item3);
            if (count < 2 || count > 3) {
                printf("%s\n",ERR_ITEM_COUNT);
                return 0;
            }
            if (count == 2) {
                item2[0] = '\0'; // then there is not a second item
            }
            army_2[i].item1 = find_item(item1);
            if (army_2[i].item1 == NULL) {
                return 0;
            }

            if (item2[0] != '\0') {     // if there was a second item
                army_2[i].item2 = find_item(item2);
            } else {
                army_2[i].item2 = NULL;
            }

            int total_slots = army_2[i].item1->slots;
            if (army_2[i].item2 != NULL) {
                total_slots += army_2[i].item2->slots;
            }
            if (total_slots > 2) {
                printf("%s\n",ERR_SLOTS);
                return 0;
            }
            army_2[i].hp = 100;
        }
        amount_2 = amount;
    }
    return 1;//everything's fine
}

void print_units(POSITION * battlefield, int amount, int army_num) {
    printf("%d: ", army_num); //num of the army
    for (int i = 0; i < amount; i++) {
        if (battlefield[i].unit != NULL && battlefield[i].unit->hp > 0) { //if unit is alive
            printf("%s,%d ", battlefield[i].unit->name, battlefield[i].unit->hp);
        }
    }
    putchar('\n');
}

bool check_army(UNIT * army, int amount, POSITION * battlefield) {
    for (int i = 0; i < amount; i++) {
        if (battlefield[i].unit != NULL) { //if someone is alive
            return false;
        }
    }
    is_game_over = true; //if everyone are dead -> end the game
    return true;
}
void winner() {
    bool army1_dead = check_army(army_1, amount_1, battlefield_1);
    bool army2_dead = check_army(army_2, amount_2, battlefield_2);
    if (army1_dead && !army2_dead) {
        printf("WINNER: 2\n");
    }
    else if (army2_dead && !army1_dead) {
        printf("WINNER: 1\n");
    }
    else {
        printf("NO WINNER\n");
    }
}
void update_army(UNIT * army, int * amount, POSITION * battlefield, int army_num) {
    int index = 0;
    for (int i = 0; i < *amount; i++) {
        if (army[i].hp > 0) { //if a unit has >0 hp
            if (i != index) {
                army[index] = army[i]; // move the unit
            }
            index++;
        }
    }
    *amount = index; //update amount

    for (int i = 0; i < *amount; i++) { //who is alive
        battlefield[i].unit = &army[i];
        battlefield[i].alive = 1;
    }
    for (int i = index; i < MAX_POSITION; i++) { //others are dead
        battlefield[i].unit = NULL;
        battlefield[i].alive = 0;
    }
}

void first_army_attack() {
    for (int i = 0; i < amount_1; i++) { // i is position of unit in army 1

        UNIT *attacker = battlefield_1[i].unit; //define the attacker
        if (attacker == NULL) continue; //skip if attacker is dead

        if (i <= attacker->item1->range) { // check the range
            char line[256] = {0}; //line to store dmg
            sprintf(line, "1,%s,%s: ", attacker->name, attacker->item1->name);

            for (int j = 0; j < amount_2 && j < attacker->item1->radius + 1; j++) { // check the range
                                                                                    //add 1 to include the center position and surroundings
                if (battlefield_2[j].alive) {
                    UNIT * defender = battlefield_2[j].unit; //define the defender

                    int total_def = defender->item1->def; // calculating total defense of a unit
                    if (defender->item2 != NULL) { //if there is a second item
                        total_def += defender->item2->def;
                    }
                    int DMG = max(1, attacker->item1->att - total_def); //calculate DMG

                    char hit[64]; //dmg to 1 unit
                    sprintf(hit, "[%s,%d] ", defender->name, DMG);
                    strcat(line,hit); //copying to total line
                    defender->hp -= DMG; //updating hp
                }
            }
            printf("%s\n",line);
        }
        if (attacker->item2 != NULL && i < amount_1 && i <= attacker->item2->range) { //if there is a second item + check the range
            char line[256] = {0}; //line to store dmg
            sprintf(line, "1,%s,%s: ", attacker->name, attacker->item2->name);

            for (int j = 0; j < amount_2 && j < attacker->item2->radius + 1; j++) { // check the range
                                                                                    //add 1 to include the center position and surroundings
                int DMG = 0; //update dmg
                int total_def = 0; //update def
                if (battlefield_2[j].alive) {
                    UNIT * defender = battlefield_2[j].unit; //define the defender
                    total_def = army_2[j].item1->def;
                    if (defender->item2 != NULL) { //if there is a second item
                        total_def += defender->item2->def;
                    }
                    DMG = max(1, attacker->item2->att - total_def);
                    char hit[64]; //dmg to 1 unit
                    sprintf(hit, "[%s,%d] ", defender->name, DMG);
                    strcat(line,hit); //copying to line
                    defender->hp -= DMG; //update hp
                }
            }
            printf("%s\n",line);
        }
    }
}
void second_army_attack() {
    for (int i = 0; i < amount_2; i++) { // i is position of unit in army 2

        UNIT *attacker = battlefield_2[i].unit; //define attacker
        if (attacker == NULL) continue; //skip if the attacker is dead

        if (i <= attacker->item1->range) { // check the range
            char line[256] = {0}; //create new line for dmg
            sprintf(line, "2,%s,%s: ", attacker->name, attacker->item1->name);

            for (int j = 0; j < amount_1 && j < attacker->item1->radius + 1; j++) { // check the range

                if (battlefield_1[j].alive) {
                    UNIT * defender = battlefield_1[j].unit;

                    int total_def = defender->item1->def;  // calculating defense
                    if (defender->item2 != NULL) { //if there is a second item
                        total_def += defender->item2->def;
                    }
                    int DMG = max(1, attacker->item1->att - total_def); //choosing the DMG
                    char hit[64]; //dmg to 1 unit
                    sprintf(hit, "[%s,%d] ", defender->name, DMG);
                    strcat(line,hit); //copy to line
                    defender->hp -= DMG; //update hp
                }
            }
            printf("%s\n",line);
        }
        if (attacker->item2 != NULL && i < amount_2 && i <= attacker->item2->range) {
            char line[256] = {0};
            sprintf(line, "2,%s,%s: ", attacker->name, attacker->item2->name);
            for (int j = 0; j < amount_1 && j < attacker->item2->radius + 1; j++) { // Ensure valid range and no out of bounds
                int DMG = 0;
                int total_def = 0;
                if (battlefield_1[j].alive) {
                    UNIT * defender = battlefield_1[j].unit;
                    total_def = defender->item1->def;
                    if (defender->item2 != NULL) {
                        total_def += defender->item2->def;
                    }
                    DMG = max(1, attacker->item2->att - total_def);
                    char hit[64]; //dmg to 1 unit
                    sprintf(hit, "[%s,%d] ", defender->name, DMG);
                    strcat(line,hit); //copying to line
                    defender->hp -= DMG; //update hp
                }
            }
            printf("%s\n",line);
        }
    }
}

int main(const int argc, char *argv[]) {
    int n = -1; //if n wasnt entered

    if (argc > 1) {
        n = atoi(argv[1]); //convert string to an int
    }
    if (!create_armada_1())  return 0;
    if (!create_armada_2())  return 0;
    int round = 1;

    if (n == -1) { //if N was not entered
        place_units_on_battlefield(army_1, amount_1, battlefield_1);
        place_units_on_battlefield(army_2, amount_2, battlefield_2);

        introduce_armies(army_1, amount_1, 1);
        introduce_armies(army_2, amount_2, 2);

        while (!is_game_over) {
            printf("Round %d\n", round);
            round++;
            print_units(battlefield_1, amount_1, 1);
            print_units(battlefield_2, amount_2, 2);

            first_army_attack();
            second_army_attack();

            update_army(army_1, &amount_1, battlefield_1, 1);
            update_army(army_2, &amount_2, battlefield_2, 2);

            check_army(army_1, amount_1, battlefield_1);
            check_army(army_2, amount_2, battlefield_2);

            print_units(battlefield_1, amount_1, 1);
            print_units(battlefield_2, amount_2, 2);
            putchar('\n');
        }
        winner();
    }
    else if (n==0) { //if N is 0
        place_units_on_battlefield(army_1, amount_1, battlefield_1);
        place_units_on_battlefield(army_2, amount_2, battlefield_2);

        introduce_armies(army_1, amount_1, 1);
        introduce_armies(army_2, amount_2, 2);
    }
    else { //if N > 0
        int pocet_kruhov = 0;
        place_units_on_battlefield(army_1, amount_1, battlefield_1);
        place_units_on_battlefield(army_2, amount_2, battlefield_2);

        introduce_armies(army_1, amount_1, 1);
        introduce_armies(army_2, amount_2, 2);
        while (pocet_kruhov < n) {
            printf("Round %d\n", round);
            print_units(battlefield_1, amount_1, 1);
            print_units(battlefield_2, amount_2, 2);

            first_army_attack();
            second_army_attack();

            update_army(army_1, &amount_1, battlefield_1, 1);
            update_army(army_2, &amount_2, battlefield_2, 2);

            check_army(army_1, amount_1, battlefield_1);
            check_army(army_2, amount_2, battlefield_2);

            print_units(battlefield_1, amount_1, 1);
            print_units(battlefield_2, amount_2, 2);
            putchar('\n');
            if (is_game_over) {
                winner();
                break;
            } else {
                pocet_kruhov++;
                round++;
            }
        }
    }
    return 0;
}
