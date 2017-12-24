#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "stack.h"

#define UP    0
#define RIGHT 1
#define DOWN  2
#define LEFT  3


void print_maze(house *s) {

	if (s->wall_down) printf("_");
	else              printf(" ");
	
	if (s->wall_right) printf("|");
	else               printf(" ");
	
	if (s->right) print_maze(s->right);
	else {
		printf("\n");
		if (s->down) {
			while (s->left) s = s->left;
			print_maze(s->down);
		}
	}
	
}

void clear_visited(house *s) {
	
	s->visited = 0;
	
	if (s->right) clear_visited(s->right);
	else {
		if (s->down) {
			while (s->left) s = s->left;
			clear_visited(s->down);
		}
	}
	
}

void up_walls(house *s) {
	
	s->wall_up    = 1;
	s->wall_right = 1;
	s->wall_down  = 1;
	s->wall_left  = 1;
	
	if (s->right) up_walls(s->right);
	else {
		if (s->down) {
			while (s->left) s = s->left;
			up_walls(s->down);
		}
	}
	
}

house* verify_unvisited(house *s) {

	if (!s->visited) return s;
	
	if (s->right) return verify_unvisited(s->right);
	else {
		if (s->down) {
			while (s->left) s = s->left;
			return verify_unvisited(s->down);
		}
	}

	return NULL;
	
}

void verify_reachability(house *s) {
	
	s->visited = 1;
	
	if (s->up    && !s->up->visited    && !s->wall_up)    verify_reachability(s->up);
	if (s->right && !s->right->visited && !s->wall_right) verify_reachability(s->right);
	if (s->down  && !s->down->visited  && !s->wall_down)  verify_reachability(s->down);
	if (s->left  && !s->left->visited  && !s->wall_left)  verify_reachability(s->left);

}

void joint_all(house *s) {

	clear_visited(s);
	verify_reachability(s);
	house *c = verify_unvisited(s);
	
	while (c) {
		
		int wall_vect[4];
		int count_walls = 0;
		
		if (c->up    && c->wall_up)    wall_vect[count_walls++] = UP;
		if (c->right && c->wall_right) wall_vect[count_walls++] = RIGHT;
		if (c->down  && c->wall_down)  wall_vect[count_walls++] = DOWN;
		if (c->left  && c->wall_left)  wall_vect[count_walls++] = LEFT;
		
		if (count_walls > 0) {
			int knock_wall = wall_vect[rand() % count_walls];
			
			if (knock_wall == UP) {
				c->wall_up = 0;
				c->up->wall_down = 0;
			}
			else if (knock_wall == RIGHT) {
				c->wall_right = 0;
				c->right->wall_left = 0;
			}
			else if (knock_wall == DOWN) {
				c->wall_down = 0;
				c->down->wall_up = 0;
			}
			else if (knock_wall == LEFT) {
				c->wall_left = 0;
				c->left->wall_right = 0;
			}
		}
		
		clear_visited(s);
		verify_reachability(s);
		c = verify_unvisited(s);
	}
}	
	

int find_the_way(house *s, house *e) {

	if (s == e) return 1;

	s->visited = 1;

	int direction_vect[4];
	int count_direction = 0;

	if (s->up    && !s->up->visited)    direction_vect[count_direction++] = UP;
	if (s->right && !s->right->visited) direction_vect[count_direction++] = RIGHT;
	if (s->down  && !s->down->visited)  direction_vect[count_direction++] = DOWN;
	if (s->left  && !s->left->visited)  direction_vect[count_direction++] = LEFT;

	if (!count_direction) return 0;
	
	house *next;
	int direction = direction_vect[rand() % count_direction];
	
	if (direction == UP) {
		next = s->up;
		
		s->wall_up       = 0;
		s->up->wall_down = 0;		
	}
	else if (direction == RIGHT) {
		next = s->right;
		
		s->wall_right       = 0;
		s->right->wall_left = 0;
	}
	else if (direction == DOWN) {
		next = s->down;
		
		s->wall_down     = 0;
		s->down->wall_up = 0;
	}
	else if (direction == LEFT) {
		next = s->left;
		
		s->wall_left        = 0;
		s->left->wall_right = 0;
	}
	
	return find_the_way(next,e);

}

// BFS
void knockout_walls(house *c) {

	c->visited = 1;
		
	int direction_vect[4];
	int count_direction = 0;

	if (c->up    && !c->up->visited)    direction_vect[count_direction++] = UP;
	if (c->right && !c->right->visited) direction_vect[count_direction++] = RIGHT;
	if (c->down  && !c->down->visited)  direction_vect[count_direction++] = DOWN;
	if (c->left  && !c->left->visited)  direction_vect[count_direction++] = LEFT;
	
	if (!count_direction) return;
	
	house *next;
	int direction = direction_vect[rand() % count_direction];
	
	if (direction == UP) {
		next = c->up;
	
		c->wall_up       = 0;
		c->up->wall_down = 0;		
	}
	else if (direction == RIGHT) {
		next = c->right;
	
		c->wall_right       = 0;
		c->right->wall_left = 0;
	}
	else if (direction == DOWN) {
		next = c->down;
		
		c->wall_down     = 0;
		c->down->wall_up = 0;
	}
	else if (direction == LEFT) {
		next = c->left;

		c->wall_left        = 0;
		c->left->wall_right = 0;
	}
			
	return knockout_walls(next);

}

void generate_maze(maze *m) {

    house ***aux_m = (house***) malloc (m->height * sizeof(house**));
    for (unsigned int i = 0; i < m->height; ++i) {
        aux_m[i] = (house**) malloc (m->weigth * sizeof(house*));
    }

	for (unsigned int i = 0; i < m->height; ++i) {
        for (unsigned int j = 0; j < m->weigth; ++j) {
			aux_m[i][j] = (house*) malloc (sizeof(house));
			
			aux_m[i][j]->up    = (house*) malloc (sizeof(house));
            aux_m[i][j]->right = (house*) malloc (sizeof(house));
            aux_m[i][j]->down  = (house*) malloc (sizeof(house));
            aux_m[i][j]->left  = (house*) malloc (sizeof(house));
			
			aux_m[i][j]->up    = NULL;
			aux_m[i][j]->right = NULL;
			aux_m[i][j]->down  = NULL;
			aux_m[i][j]->left  = NULL;
			
			aux_m[i][j]->wall_up    = 1;
			aux_m[i][j]->wall_right = 1;
			aux_m[i][j]->wall_down  = 1;
			aux_m[i][j]->wall_left  = 1;
			
			aux_m[i][j]->visited = 0;
			aux_m[i][j]->id = i * m->weigth + j;
		}
	}
	
    for (unsigned int i = 0; i < m->height; ++i) {
        for (unsigned int j = 0; j < m->weigth; ++j) {
            if (i > 0)             aux_m[i][j]->up    = aux_m[i-1][j];
			if (j < m->weigth - 1) aux_m[i][j]->right = aux_m[i][j+1];
			if (i < m->height - 1) aux_m[i][j]->down  = aux_m[i+1][j];
			if (j > 0)             aux_m[i][j]->left  = aux_m[i][j-1];
        }
    }
	
	m->start = aux_m[0][0];
	m->exit  = aux_m[m->height-1][m->weigth-1];
	
	free(aux_m);
	
	srand(time(NULL));
	int ok = find_the_way(m->start,m->exit);
	while (!ok) { 
		clear_visited(m->start);
		up_walls(m->start);
		ok = find_the_way(m->start,m->exit);
	}

	house *c = verify_unvisited(m->start);
	while (c) {
		knockout_walls(c);
		c = verify_unvisited(m->start);
	}
	
	joint_all(m->start);
	m->exit->wall_right = 0;
	m->start->wall_left = 0;
	
	print_maze(m->start);

}

int main(int argc, char *argv[]) {

	maze *m;

	if (argc != 3) {
		printf("Correct usage:\n");
		printf("%s <weigth_maze> <height_maze>\n",argv[0]);
		return -1;
	}

	m = (maze*) malloc (sizeof(maze));

	m->weigth = atoi(argv[1]);
	m->height = atoi(argv[2]);
	m->size   = m->height * m->weigth;

	generate_maze(m);

//	print_maze(m);

	free(m);

	return 0;

}
