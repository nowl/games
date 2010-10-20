#include "dm.h"

static void
determine_view()
{
    int x = GAMESTATE.player.map_location_x;
    int y = GAMESTATE.player.map_location_y;
    int *tiles = GAMESTATE.player.view.map_tiles;
    
	/* negating the y offset since in out map y increases downward but
	 * in the models below y increases upward */
#define TILE_FILL(n, x_off, y_off) tiles[(n)] = map_element_at(x+(x_off), y-(y_off));

    switch(GAMESTATE.player.facing) {
    case WEST:
		/*
		 * 23 24 25 26 27 28 29 30 31
         * 14 15 16 17 18 19 20 21 22
         *    07 08 09 10 11 12 13
         *       02 03 04 05 06
         *          00 xx 01
		 */
		TILE_FILL(0,0,-1);
        TILE_FILL(1,0,1);
        TILE_FILL(2,-1,-2);
        TILE_FILL(3,-1,-1);
        TILE_FILL(4,-1,0);
        TILE_FILL(5,-1,1);
        TILE_FILL(6,-1,2);
        TILE_FILL(7,-2,-3);
        TILE_FILL(8,-2,-2);
        TILE_FILL(9,-2,-1);
        TILE_FILL(10,-2,0);
        TILE_FILL(11,-2,1);
        TILE_FILL(12,-2,2);
        TILE_FILL(13,-2,3);
        TILE_FILL(14,-3,-4);
        TILE_FILL(15,-3,-3);
        TILE_FILL(16,-3,-2);
        TILE_FILL(17,-3,-1);
        TILE_FILL(18,-3,0);
        TILE_FILL(19,-3,1);
        TILE_FILL(20,-3,2);
        TILE_FILL(21,-3,3);
        TILE_FILL(22,-3,4);
        TILE_FILL(23,-4,-4);
        TILE_FILL(24,-4,-3);
        TILE_FILL(25,-4,-2);
        TILE_FILL(26,-4,-1);
        TILE_FILL(27,-4,0);
		TILE_FILL(28,-4,1);
        TILE_FILL(29,-4,2);
        TILE_FILL(30,-4,3);
        TILE_FILL(31,-4,4);
        break;
	case EAST:
		/*
		 * 23 24 25 26 27 28 29 30 31
         * 14 15 16 17 18 19 20 21 22
         *    07 08 09 10 11 12 13
         *       02 03 04 05 06
         *          00 xx 01
		 */
		TILE_FILL(0,0,1);
        TILE_FILL(1,0,-1);
        TILE_FILL(2,1,2);
        TILE_FILL(3,1,1);
        TILE_FILL(4,1,0);
        TILE_FILL(5,1,-1);
        TILE_FILL(6,1,-2);
        TILE_FILL(7,2,3);
        TILE_FILL(8,2,2);
        TILE_FILL(9,2,1);
        TILE_FILL(10,2,0);
        TILE_FILL(11,2,-1);
        TILE_FILL(12,2,-2);
        TILE_FILL(13,2,-3);
        TILE_FILL(14,3,4);
        TILE_FILL(15,3,3);
        TILE_FILL(16,3,2);
        TILE_FILL(17,3,1);
        TILE_FILL(18,3,0);
        TILE_FILL(19,3,-1);
        TILE_FILL(20,3,-2);
        TILE_FILL(21,3,-3);
        TILE_FILL(22,3,-4);
        TILE_FILL(23,4,4);
        TILE_FILL(24,4,3);
        TILE_FILL(25,4,2);
        TILE_FILL(26,4,1);
        TILE_FILL(27,4,0);
		TILE_FILL(28,4,-1);
        TILE_FILL(29,4,-2);
        TILE_FILL(30,4,-3);
        TILE_FILL(31,4,-4);
        break;
	case NORTH:
		/*
		 * 23 24 25 26 27 28 29 30 31
         * 14 15 16 17 18 19 20 21 22
         *    07 08 09 10 11 12 13
         *       02 03 04 05 06
         *          00 xx 01
		 */
        TILE_FILL(0,-1,0);
        TILE_FILL(1,1,0);
        TILE_FILL(2,-2,1);
        TILE_FILL(3,-1,1);
        TILE_FILL(4,0,1);
        TILE_FILL(5,1,1);
        TILE_FILL(6,2,1);
        TILE_FILL(7,-3,2);
        TILE_FILL(8,-2,2);
        TILE_FILL(9,-1,2);
        TILE_FILL(10,0,2);
        TILE_FILL(11,1,2);
        TILE_FILL(12,2,2);
        TILE_FILL(13,3,2);
        TILE_FILL(14,-4,3);
        TILE_FILL(15,-3,3);
        TILE_FILL(16,-2,3);
        TILE_FILL(17,-1,3);
        TILE_FILL(18,0,3);
        TILE_FILL(19,1,3);
        TILE_FILL(20,2,3);
        TILE_FILL(21,3,3);
        TILE_FILL(22,4,3);
        TILE_FILL(23,-4,4);
        TILE_FILL(24,-3,4);
        TILE_FILL(25,-2,4);
        TILE_FILL(26,-1,4);
        TILE_FILL(27,0,4);
        TILE_FILL(28,1,4);
        TILE_FILL(29,2,4);
        TILE_FILL(30,3,4);
        TILE_FILL(31,4,4);
        break;
	case SOUTH:
		/*
		 * 23 24 25 26 27 28 29 30 31
         * 14 15 16 17 18 19 20 21 22
         *    07 08 09 10 11 12 13
         *       02 03 04 05 06
         *          00 xx 01
		 */
		TILE_FILL(0,1,0);
        TILE_FILL(1,-1,0);
        TILE_FILL(2,2,-1);
        TILE_FILL(3,1,-1);
        TILE_FILL(4,0,-1);
        TILE_FILL(5,-1,-1);
        TILE_FILL(6,-2,-1);
        TILE_FILL(7,3,-2);
        TILE_FILL(8,2,-2);
        TILE_FILL(9,1,-2);
        TILE_FILL(10,0,-2);
        TILE_FILL(11,-1,-2);
        TILE_FILL(12,-2,-2);
        TILE_FILL(13,-3,-2);
        TILE_FILL(14,4,-3);
        TILE_FILL(15,3,-3);
        TILE_FILL(16,2,-3);
        TILE_FILL(17,1,-3);
        TILE_FILL(18,0,-3);
        TILE_FILL(19,-1,-3);
        TILE_FILL(20,-2,-3);
        TILE_FILL(21,-3,-3);
        TILE_FILL(22,-4,-3);
        TILE_FILL(23,4,-4);
        TILE_FILL(24,3,-4);
        TILE_FILL(25,2,-4);
        TILE_FILL(26,1,-4);
        TILE_FILL(27,0,-4);
		TILE_FILL(28,-1,-4);
        TILE_FILL(29,-2,-4);
        TILE_FILL(30,-3,-4);
        TILE_FILL(31,-4,-4);
        break;
    }
}

static void display_loc() {
	printf("moving to: %d,%d, facing: %s\n",
		   GAMESTATE.player.map_location_x,
		   GAMESTATE.player.map_location_y,
		   FACING_DIR_TO_TEXT[GAMESTATE.player.facing]);
}

void
player_attempt_move_left()
{
	/* TODO: should test for walls, etc */
	switch(GAMESTATE.player.facing) {
	case WEST:
		++GAMESTATE.player.map_location_y;
		break;
	case SOUTH:
		++GAMESTATE.player.map_location_x;
		break;
	case EAST:
		--GAMESTATE.player.map_location_y;
		break;
	case NORTH:
		--GAMESTATE.player.map_location_x;
		break;
	}
	
	display_loc();
}

void
player_attempt_move_right()
{
	/* TODO: should test for walls, etc */
	switch(GAMESTATE.player.facing) {
	case WEST:
		--GAMESTATE.player.map_location_y;
		break;
	case SOUTH:
		--GAMESTATE.player.map_location_x;
		break;
	case EAST:
		++GAMESTATE.player.map_location_y;
		break;
	case NORTH:
		++GAMESTATE.player.map_location_x;
		break;
	}

	display_loc();
}

void
player_attempt_move_forward()
{
	/* TODO: should test for walls, etc */
	switch(GAMESTATE.player.facing) {
	case WEST:
		--GAMESTATE.player.map_location_x;
		break;
	case SOUTH:
		++GAMESTATE.player.map_location_y;
		break;
	case EAST:
		++GAMESTATE.player.map_location_x;
		break;
	case NORTH:
		--GAMESTATE.player.map_location_y;
		break;
	}

	display_loc();
}

void
player_attempt_move_backward()
{
	/* TODO: should test for walls, etc */
	switch(GAMESTATE.player.facing) {
	case WEST:
		++GAMESTATE.player.map_location_x;
		break;
	case SOUTH:
		--GAMESTATE.player.map_location_y;
		break;
	case EAST:
		--GAMESTATE.player.map_location_x;
		break;
	case NORTH:
		++GAMESTATE.player.map_location_y;
		break;
	}

	display_loc();
}

void
player_attempt_rotate_ccw()
{
	switch(GAMESTATE.player.facing) {
	case WEST:
		GAMESTATE.player.facing = SOUTH;
		break;
	case SOUTH:
		GAMESTATE.player.facing = EAST;
		break;
	case EAST:
		GAMESTATE.player.facing = NORTH;
		break;
	case NORTH:
		GAMESTATE.player.facing = WEST;
		break;
	}
	display_loc();
}

void
player_attempt_rotate_cw()
{
	switch(GAMESTATE.player.facing) {
	case WEST:
		GAMESTATE.player.facing = NORTH;
		break;
	case NORTH:
		GAMESTATE.player.facing = EAST;
		break;
	case EAST:
		GAMESTATE.player.facing = SOUTH;
		break;
	case SOUTH:
		GAMESTATE.player.facing = WEST;
		break;
	}
	display_loc();
}

static int
event_callback(SDL_Event *event)
{
	switch(event->type) {
	case SDL_KEYDOWN:
		/* many of these keysyms don't work in a switch statement,
		 * hence the else ifs */
		if(event->key.keysym.sym == SDLK_a) {
			player_attempt_move_left();
			return 1;
		} else if(event->key.keysym.sym == SDLK_d) {
			player_attempt_move_right();
			return 1;
		} else if(event->key.keysym.sym == SDLK_w) {
			player_attempt_move_forward();
			return 1;
		} else if(event->key.keysym.sym == SDLK_s) {
			player_attempt_move_backward();
			return 1;
		} else if(event->key.keysym.sym == SDLK_q) {
			player_attempt_rotate_ccw();
			return 1;
		} else if(event->key.keysym.sym == SDLK_e) {
			player_attempt_rotate_cw();
			return 1;
		}
		break;
	}

	return 0;
}

void
player_init()
{
	GAMESTATE.player.map_location_x = 3;
    GAMESTATE.player.map_location_y = 1;
    GAMESTATE.player.facing = EAST;

	/* add key-handling events */
	event_callback_add(event_callback, EM_MAP);
	update_callback_add(determine_view);
}
