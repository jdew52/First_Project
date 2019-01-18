#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void stat_update(int *point_count);
void save_stats(void);
void load_stats(void);
void battle(void);
void level_up(int *exp, int *exp_cap, int *point_count);

typedef struct{
	int health;
	int attack;
	int defense;
	int level; //can possibly function as how many experience points are given for defeating an enemy
} Person;

Person player;
Person enemy = {3, 3, 3, 3};

int main(void){
	int choice_num, exp = 0, exp_cap = 15, point_count;
	char choice_char;

	srand(time(NULL));

	printf("1. New Game\n2. Load Game\n");
	scanf("%d", &choice_num);
	printf("\n");
	
	if (choice_num == 2){
		load_stats();
	} else if (choice_num == 1) {
		player.health = 0;
		player.attack = 0;
		player.defense = 0;
		player.level = 1;

		point_count = 10;

		printf("There are three skills you may put points into:\n");
		printf("1. Health\n2. Attack\n3. Defense\n");
		stat_update(&point_count);

		printf("Do you want to save(Y/N): ");
        scanf(" %c", &choice_char);

        if (choice_char == 'Y'){
            save_stats();
        }
	}

	while (player.health > 0) {
        battle();

        if (player.health > 0) {
            exp += enemy.level;
            if (exp >= exp_cap) {
                level_up(&exp, &exp_cap, &point_count);
            }

            printf("Do you want to save(Y/N): ");
            scanf(" %c", &choice_char);

            if (choice_char == 'Y') {
                save_stats();
            }
        }
    }


	return 0;
}

void level_up(int *exp, int *exp_cap, int *point_count){
    *exp -= *exp_cap;
    *exp_cap += 5;
    player.level += 1;
    *point_count += 3;
    printf("You have gained a new level!\nNew level: %d\n", player.level);
}

void stat_update(int *point_count){
	int stat_inc;
	printf("You have %d points to spend\n", *point_count);
	printf("How many points would you like to put into Health: ");
	scanf("%d", &stat_inc);
	player.health += stat_inc;
	*point_count -= stat_inc;
	printf("You have %d points to spend\n", *point_count);
	printf("How many points would you like to put into Attack: ");
	scanf("%d", &stat_inc);
	player.attack += stat_inc;
	*point_count -= stat_inc;
	printf("You have %d points to spend\n", *point_count);
	printf("How many points would you like to put into Defense: ");
	scanf("%d", &stat_inc);
	player.defense += stat_inc;
	*point_count -= stat_inc;
}

void save_stats(void){
	FILE *out_data = fopen("save_data.txt", "w");
	fprintf(out_data, "%d\n", player.health);
	fprintf(out_data, "%d\n", player.attack);
	fprintf(out_data, "%d\n", player.defense);
	fprintf(out_data, "%d\n", player.level);
	fclose(out_data);
}

void load_stats(void){
	FILE *in_data = fopen("save_data.txt", "r");
	fscanf(in_data, "%d", &player.health);
	fscanf(in_data, "%d", &player.attack);
	fscanf(in_data, "%d", &player.defense);
	fscanf(in_data, "%d", &player.level);
	fclose(in_data);
}

void battle(void){
    int damage, choice, chance;
    int esc = 0;
    while ((player.health > 0 && enemy.health > 0)&&(esc == 0)) {
        printf("Your health: %d\n", player.health);
        printf("Enemy health: %d\n", enemy.health);
        printf("1. Attack\n2. Run\n");
        scanf("%d", &choice);
        if (choice == 1) {
            damage = (((rand() % player.attack) + 1) - ((rand() % enemy.defense) + 1));
            if (damage >= 1) {
                enemy.health -= damage;
                printf("You did %d damage.\n", damage);
            } else {
                printf("You tried... but missed.\n");
            }
        } else {
            chance = (rand() % 10) + 1;
            if (chance >= 5) {
                esc = 1;
            }
        }

        if (esc == 1){
            printf("You escaped!\n");
            break;
        }

        if (enemy.health <= 0){
            printf("You beat its ass!\n");
            break;
        }

        damage = (((rand() % enemy.attack) + 1) - ((rand() % player.defense) + 1));

        if (damage >= 1) {
            printf("The enemy did %d damage.\n", damage);
            player.health -= damage;
        } else {
            printf("You dodged the enemy's attack.\n");
        }

        if (player.health <= 0){
            printf("You got merked!");
        }
    }
}