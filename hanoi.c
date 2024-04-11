#include <stdio.h>
#include <stdlib.h>

#define TOWER_NUMBER 3
#define LOG_ENABLE 1

static int **create_towers(int disk_number)
{
	int **towers = malloc(sizeof(int *) * TOWER_NUMBER);
	if (towers == NULL)
		return NULL;
	int *towers_memory = calloc(TOWER_NUMBER * disk_number, sizeof(int));	
	if (towers_memory == NULL)
	{
		free(towers);
		return NULL;
	}
	for (int i = 0; i < disk_number; ++i)
		towers_memory[i] = i + 1;
	for (int i = 0; i < TOWER_NUMBER; ++i)
		towers[i] = towers_memory + sizeof(int) * disk_number * i;
	return towers;
}

static void destroy_towers(int **towers)
{
	free(*towers);
	free(towers);
}

/*
static void print_towers(int **towers, int disk_number)
{
	printf("Tower %i", 1);
	for (int i = 1; i < TOWER_NUMBER; ++i)
		printf(" Tower %i", i + 1);
	puts("\n");
	for (int i = 0; i < disk_number; ++i)
	{
		if (towers[0][i] == 0) 
				printf("%7s", "");
		else
				printf("%-7i", towers[0][i]);
		for (int j = 1; j < TOWER_NUMBER; ++j)
		{
			if (towers[j][i] == 0) 
				printf("%7s ", "");
			else
				printf("%-7i ", towers[j][i]);
		}
		puts("");

	}
}
*/

static void move(int **towers, int disk_number, int origin, int dest)
{
	int current_location = 0;
	int next_location = 0;

	while (current_location < disk_number && towers[origin][current_location] == 0)
		++current_location;
	while (next_location < disk_number && towers[dest][next_location] == 0)
		++next_location;
	--next_location;
	if (current_location == disk_number || next_location == -1)
		return;

	towers[dest][next_location] = towers[origin][current_location];
	towers[origin][current_location] = 0;

	if (LOG_ENABLE)
		printf("%i -> %i\n", origin + 1, dest + 1);
}

static int next_free_location(int current_location, int target_location)
{
	return (target_location + 1) % TOWER_NUMBER == current_location ? (target_location + 2) % TOWER_NUMBER : (target_location + 1) % TOWER_NUMBER;
}

static void solve(int **towers, int disk_number, int current_disk, int current_location, int next_location)
{
	if (current_disk == 0)
	{
		move(towers, disk_number, current_location, next_location);
		return;
	}
	solve(towers, disk_number, current_disk - 1, current_location, next_free_location(current_location, next_location));
	move(towers, disk_number, current_location, next_location);
	solve(towers, disk_number, current_disk - 1, next_free_location(current_location, next_location), next_location);
}

int main(int argc, char **argv)
{
	int disk_number = 0;
	if (argc < 2 || argc > 3)
	{
		fprintf(stderr, "Wrong amount of args\n");
		fprintf(stderr, "Usage: \n");
		fprintf(stderr, "%s number [--count]\n", argv[0]);
		return 1;
	}
	disk_number = atoi(argv[1]);
	if (argc == 3)
	{
		unsigned long long int count = 1;
		for (int i = 0; i < disk_number - 1; ++i)
			count = count * 2 + 1 ;
		printf("%llu\n", count);
		return 0;
	}
	int **towers = create_towers(disk_number);
	if (towers == NULL)
	{
		fprintf(stderr, "Error creating towers\n");
		return 1;
	}
	solve(towers, disk_number, disk_number - 1, 0, TOWER_NUMBER - 1);

	destroy_towers(towers);
	return 0;
}
