#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
// NEMENIT, inak vas kod neprejde testom !!!
#include "data.h"
int is_number(const char * c) {
	while (*c) {
		if (!isdigit(*c)) return 0;
		c++;
	}
	return 1;
}
char *read_file(const char * file_name) {
	FILE * file = fopen(file_name, "r");
	if (file == NULL) {
		fprintf(stderr, ERR_FILE);
		return 0;
	}
	size_t buffer_size = 1024; //starting value
	char *buffer = malloc(buffer_size);

	char line[MAX_LINE];
	size_t position = 0; //position in file

	while (fgets(line, MAX_LINE, file)) { //reading a line
		size_t len_line = strlen(line); //getting its length
		if (position + len_line > buffer_size) {  //if
			buffer_size *= 2;		//increase buffer size
			buffer = realloc(buffer, buffer_size);
		}
		memcpy(buffer+position, line, len_line);
		position += len_line;
	}
	fclose(file);
	return buffer;
}
int parse_key_value(char* line, const char* key, char* value) {
	char* pos = strstr(line, key);
	if (!pos) {
		fprintf(stderr, ERR_MISSING_ATTRIBUTE);
		return 0;
	}
	pos += strlen(key);  // skip the key

	while (*pos && (isspace((unsigned char)*pos) || *pos == ':')) pos++;  // skip spaces and colon
	if (*pos == '"') {
		pos++;
		char *start = pos;
		while (*pos && *pos != '"') pos++;
		strncpy(value, start, pos - start);//copy to value
		value[pos - start] = '\0';
		return 1;
	}
	else if (*pos != '\0' && !isspace(*pos)) {
		char *start = pos;
		while (*pos && !isspace(*pos) && *pos != ',' && *pos != '}') pos++;
		if (pos == start) {
			fprintf(stderr, ERR_MISSING_VALUE);
			return 0;
		}

		strncpy(value, start, pos - start);
		value[pos - start] = '\0';

		if (!is_number(value)) {
			fprintf(stderr, ERR_BAD_VALUE);
			return 0;
		}
	}
}
int parse_items(char * data, ITEM **items) {			//dealing with items
	int num_of_items = 0;
	int capacity = 10; //if 10 units have 1 item
	*items = malloc(capacity * sizeof(ITEM));
	char* pos = data;
	while (*pos) {
		if (*pos == '{') {
			pos++;
			if (num_of_items >= capacity) {
				capacity *= 2;
				*items = realloc(*items, capacity * sizeof(ITEM)); //increase capacity
			}

			char value[MAX_LINE];
			// connecting values
			if (!parse_key_value(pos, "\"name\"", value)) return 0;
			strncpy((*items)[num_of_items].name, value, MAX_NAME);

			if (!parse_key_value(pos, "\"att\"", value)) return 0;
			(*items)[num_of_items].att = atoi(value);

			if (!parse_key_value(pos, "\"def\"", value)) return 0;
			(*items)[num_of_items].def = atoi(value);

			if (!parse_key_value(pos, "\"slots\"", value)) return 0;
			(*items)[num_of_items].slots = atoi(value);

			if (!parse_key_value(pos, "\"range\"", value)) return 0;
			(*items)[num_of_items].range = atoi(value);

			if (!parse_key_value(pos, "\"radius\"", value)) return 0;
			(*items)[num_of_items].radius = atoi(value);

			num_of_items++;
		}
		pos++;
	}
	return num_of_items;
}
void print_items(ITEM *items, int count) {
	for (int i = 0; i < count; i++) {
		printf("Name: %s\n", items[i].name);
		printf("Attack: %d\n", items[i].att);
		printf("Defense: %d\n", items[i].def);
		printf("Slots: %d\n", items[i].slots);
		printf("Range: %d\n", items[i].range);
		printf("Radius: %d\n", items[i].radius);
		putchar('\n');
	}
}
const ITEM * find_item(const char * name, int item_count, ITEM * items) {
	for (int i = 0; i < item_count; i++) {
		if (strcmp(items[i].name, name) == 0) {
			return &items[i];
		}
	}
	fprintf(stderr, ERR_WRONG_ITEM);
	return 0;
}
int parse_armies(char *line, UNIT *units, int item_count, ITEM *items) {
    char* pos = line;
    int unit_count = 0; //number of units in army

    while (*pos && (isspace((unsigned char)*pos))) pos++; //skipping all spaces
	if (*pos && isdigit((unsigned char)*pos)) { //if it is a number
        unit_count = atoi(pos);

        if (unit_count < MIN_ARMY || unit_count > MAX_ARMY) { //not allowed army sizes
            fprintf(stderr, ERR_UNIT_COUNT);
            return 0;
        }
    	while (*pos && *pos != '\n') pos++;  //skipping til \n
    	if (*pos == '\n') pos++; //go to the next line
    }

    for (int i = 0; i < unit_count; i++) {
        char *line_start = pos;
        while (*pos && *pos != '\n') pos++;
        if (*pos == '\n') {
            *pos = '\0';
            pos++;
        }

        char *word = strtok(line_start, " ");
        if (word) {
            strncpy(units[i].name, word, MAX_NAME);
            units[i].name[MAX_NAME] = '\0';
            units[i].hp = 100;
        }
        word = strtok(NULL, " ");
        int total_slots = 0;
        if (word) {
            units[i].item1 = find_item(word, item_count, items);
            total_slots += units[i].item1->slots;
        } else {
            fprintf(stderr, ERR_ITEM_COUNT);
            return 0;
        }
        word = strtok(NULL, " ");
        if (word) {
            units[i].item2 = find_item(word, item_count, items);
            total_slots += units[i].item2->slots;
            if (total_slots > 2) {
                fprintf(stderr, ERR_SLOTS);
                return 0;
            }
        } else {
            units[i].item2 = NULL;
        }
        word = strtok(NULL, " ");
        if (word) {
            fprintf(stderr, ERR_ITEM_COUNT);
            return 0;
        }
    }
    return unit_count;
}
void print_army(UNIT *units, int count, int army_num) {
	printf("Army %d\n", army_num);
	for (int i = 0; i < count; i++) {
		printf("%*sUnit: %d\n", 4, "", i);
		printf("%*sName: %s\n",4,"", units[i].name);
		printf("%*sHP: %d\n", 4, "", units[i].hp);
		printf("%*sItem 1: %s,%d,%d,%d,%d,%d\n", 4, "", units[i].item1->name, units[i].item1->att, units[i].item1->def,
			units[i].item1->slots, units[i].item1->range, units[i].item1->radius);
		if (units[i].item2 != NULL) {
			printf("%*sItem 2: %s,%d,%d,%d,%d,%d\n", 4, "", units[i].item2->name, units[i].item2->att, units[i].item2->def,
			units[i].item2->slots, units[i].item2->range, units[i].item2->radius);
		}
		putchar('\n');
	}
}
int main(const int argc, char *argv[]) {
  	if (argc == 1) {
  		char *data = read_file(ITEMS_FILE); //read the data
  		if (data == 0) { //to end program
  			return 0;
  		}
  		ITEM *items = NULL;
  		int count = parse_items(data, &items); //find all items
  		if (count != 0) {
  			print_items(items, count);
  		}
  		free(items);
  		free(data);
  		return 0;
  	}
	else if (argc == 3) {
		UNIT army1[MAX_ARMY];  // to store units
		UNIT army2[MAX_ARMY];
		int army1_count = 0, army2_count = 0;
		char *data = read_file(ITEMS_FILE); //read the data
		if (data == 0) { //to end program
			return 0;
		}
		ITEM *items = NULL;
		int item_count = parse_items(data, &items); //find all items

		char *army1_data = read_file(argv[1]);
		if (army1_data == NULL) {
			free(items);
			free(data);
			return 0;
		}
		char *army2_data = read_file(argv[2]);
		if (army2_data == NULL) {
			free(army1_data); //cuz its already there
			free(items);
			free(data);
			return 0;
		}
		army1_count = parse_armies(army1_data, army1, item_count, items);
		army2_count = parse_armies(army2_data, army2, item_count, items);
		if (army1_count == 0 || army2_count == 0) {
			free(army1_data);
			free(army2_data);
			free(items);
			free(data);
			return 0;
		}
		print_army(army1, army1_count, 1);  // print armies
		print_army(army2, army2_count, 2);
		free(army1_data);
		free(army2_data);
		free(items);
		free(data);
		return 0;
	}
    else {
	    fprintf(stderr, ERR_CMD);
    	return 0;
    }
	return 0;
}
