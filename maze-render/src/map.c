#include "dm.h"

#define MAP_DISPLAY_OFFSET_X     ( (1024-700)/3 )
#define MAP_DISPLAY_OFFSET_Y     ( (768-600)/4 )
#define SLAB_1_OFFSET            0
#define SLAB_2_OFFSET            75
#define SLAB_3_OFFSET            (SLAB_2_OFFSET + 136)
#define SLAB_4_OFFSET            (SLAB_3_OFFSET + 68)
#define SLAB_5_OFFSET            (SLAB_4_OFFSET + 35)
#define SLAB_6_OFFSET            (SLAB_5_OFFSET + 2*36)
#define SLAB_7_OFFSET            (SLAB_6_OFFSET + 35)
#define SLAB_8_OFFSET            (SLAB_7_OFFSET + 68)
#define SLAB_9_OFFSET            (SLAB_8_OFFSET + 136)


#define MAX_NUM_TILESETS 5
#define CURRENT_TILESET  0

enum tileset_enum_t {
	TS_NONE = -1,
	TS_SIDE_1_SIZE_1,
	TS_SIDE_2_SIZE_1,
	TS_SIDE_3_SIZE_1,
	TS_SIDE_4_SIZE_1,
	TS_SIDE_1_SIZE_2,
	TS_SIDE_2_SIZE_2,
	TS_SIDE_3_SIZE_2,
	TS_SIDE_4_SIZE_2,
	TS_SIDE_1_SIZE_3,
	TS_SIDE_2_SIZE_3,
	TS_SIDE_3_SIZE_3,
	TS_SIDE_4_SIZE_3,
	TS_SIDE_1_SIZE_4,
	TS_SIDE_2_SIZE_4,
	TS_SIDE_3_SIZE_4,
	TS_SIDE_4_SIZE_4,
	TS_FRONT_1_SIZE_1,
	TS_FRONT_2_SIZE_1,
	TS_FRONT_3_SIZE_1,
	TS_FRONT_4_SIZE_1,
	TS_FRONT_1_SIZE_2,
	TS_FRONT_2_SIZE_2,
	TS_FRONT_3_SIZE_2,
	TS_FRONT_4_SIZE_2,
	TS_FRONT_1_SIZE_3,
	TS_FRONT_2_SIZE_3,
	TS_FRONT_3_SIZE_3,
	TS_FRONT_4_SIZE_3,
	TS_FRONT_1_SIZE_4,
	TS_FRONT_2_SIZE_4,
	TS_FRONT_3_SIZE_4,
	TS_FRONT_4_SIZE_4,
	TS_SIDE_1_SIZE_1_REV,
	TS_SIDE_2_SIZE_1_REV,
	TS_SIDE_3_SIZE_1_REV,
	TS_SIDE_4_SIZE_1_REV,
	TS_SIDE_1_SIZE_2_REV,
	TS_SIDE_2_SIZE_2_REV,
	TS_SIDE_3_SIZE_2_REV,
	TS_SIDE_4_SIZE_2_REV,
	TS_SIDE_1_SIZE_3_REV,
	TS_SIDE_2_SIZE_3_REV,
	TS_SIDE_3_SIZE_3_REV,
	TS_SIDE_4_SIZE_3_REV,
	TS_SIDE_1_SIZE_4_REV,
	TS_SIDE_2_SIZE_4_REV,
	TS_SIDE_3_SIZE_4_REV,
	TS_SIDE_4_SIZE_4_REV,
	TS_FRONT_1_SIZE_1_REV,
	TS_FRONT_2_SIZE_1_REV,
	TS_FRONT_3_SIZE_1_REV,
	TS_FRONT_4_SIZE_1_REV,
	TS_FRONT_1_SIZE_2_REV,
	TS_FRONT_2_SIZE_2_REV,
	TS_FRONT_3_SIZE_2_REV,
	TS_FRONT_4_SIZE_2_REV,
	TS_FRONT_1_SIZE_3_REV,
	TS_FRONT_2_SIZE_3_REV,
	TS_FRONT_3_SIZE_3_REV,
	TS_FRONT_4_SIZE_3_REV,
	TS_FRONT_1_SIZE_4_REV,
	TS_FRONT_2_SIZE_4_REV,
	TS_FRONT_3_SIZE_4_REV,
	TS_FRONT_4_SIZE_4_REV,
	TS_DIRECT_SIZE_1,
	TS_DIRECT_SIZE_2,
	TS_DIRECT_SIZE_3,
	TS_DIRECT_SIZE_4,
	TS_CEILING,
	TS_FLOOR
};

static int TILESET_MAP[MAX_NUM_TILESETS][70];

static int
is_wall(int wall_type)
{
	return wall_type == 1 || wall_type == -1;
}

static int WALL_SLABS[9] = {TS_NONE, TS_NONE, TS_NONE, TS_NONE,
							TS_NONE, TS_NONE, TS_NONE, TS_NONE,
							TS_NONE};

static void
determine_slabs()
{
	/*
	 * Map tiles the player is facing are ordered like the following
	 * with xx being the player's location.
	 *
	 * 23 24 25 26 27 28 29 30 31
	 * 14 15 16 17 18 19 20 21 22
	 *    07 08 09 10 11 12 13
	 *       02 03 04 05 06
	 *          00 xx 01
	 */

	const int *tiles = GAMESTATE.player.view.map_tiles;
	
	/* semi-ugly wall checking code to follow.. */

	/* Wall slabs are numbered from 0 to 8 across the screen from left
	 * to right. */

	/* slab 0 */
	if(is_wall(tiles[0]))
		WALL_SLABS[0] = TS_SIDE_1_SIZE_1;
	else if(is_wall(tiles[3]))
		WALL_SLABS[0] = TS_FRONT_1_SIZE_1;
	else if(is_wall(tiles[2]))
		WALL_SLABS[0] = TS_SIDE_1_SIZE_2;
	else if(is_wall(tiles[8]))
		WALL_SLABS[0] = TS_FRONT_1_SIZE_2;
	else if(is_wall(tiles[7]))
		WALL_SLABS[0] = TS_SIDE_1_SIZE_3;
	else if(is_wall(tiles[15]))
		WALL_SLABS[0] = TS_FRONT_1_SIZE_3;
	else if(is_wall(tiles[14]))
		WALL_SLABS[0] = TS_SIDE_1_SIZE_4;
	else if(is_wall(tiles[23]))
		WALL_SLABS[0] = TS_FRONT_1_SIZE_4;
	else
		WALL_SLABS[0] = TS_NONE;

	/* slab 1 */
	if(is_wall(tiles[3]))
		WALL_SLABS[1] = TS_SIDE_2_SIZE_1;
	else if(is_wall(tiles[9]))
		WALL_SLABS[1] = TS_FRONT_2_SIZE_1;
	else if(is_wall(tiles[8]))
		WALL_SLABS[1] = TS_SIDE_2_SIZE_2;
	else if(is_wall(tiles[16]))
		WALL_SLABS[1] = TS_FRONT_2_SIZE_2;
	else if(is_wall(tiles[15]))
		WALL_SLABS[1] = TS_SIDE_2_SIZE_3;
	else if(is_wall(tiles[24]))
		WALL_SLABS[1] = TS_FRONT_2_SIZE_3;	
	else
		WALL_SLABS[1] = TS_NONE;

	/* slab 2 */
	if(is_wall(tiles[9]))
		WALL_SLABS[2] = TS_SIDE_3_SIZE_1;
	else if(is_wall(tiles[17]))
		WALL_SLABS[2] = TS_FRONT_3_SIZE_1;
	else if(is_wall(tiles[16]))
		WALL_SLABS[2] = TS_SIDE_3_SIZE_2;
	else if(is_wall(tiles[25]))
		WALL_SLABS[2] = TS_FRONT_3_SIZE_2;	
	else
		WALL_SLABS[2] = TS_NONE;

	/* slab 3 */
	if(is_wall(tiles[17]))
		WALL_SLABS[3] = TS_SIDE_4_SIZE_1;
	else if(is_wall(tiles[26]))
		WALL_SLABS[3] = TS_FRONT_4_SIZE_1;
	else
		WALL_SLABS[3] = TS_NONE;

	/* slab 4 */
	if(is_wall(tiles[4]))
		WALL_SLABS[4] = TS_DIRECT_SIZE_1;
	else if(is_wall(tiles[10]))
		WALL_SLABS[4] = TS_DIRECT_SIZE_2;
	else if(is_wall(tiles[18]))
		WALL_SLABS[4] = TS_DIRECT_SIZE_3;
	else if(is_wall(tiles[27]))
		WALL_SLABS[4] = TS_DIRECT_SIZE_4;
	else
		WALL_SLABS[4] = TS_NONE;

	/* slab 5 */
	if(is_wall(tiles[19]))
		WALL_SLABS[5] = TS_SIDE_4_SIZE_1_REV;
	else if(is_wall(tiles[28]))
		WALL_SLABS[5] = TS_FRONT_4_SIZE_1_REV;
	else
		WALL_SLABS[5] = TS_NONE;

	/* slab 6 */
	if(is_wall(tiles[11]))
		WALL_SLABS[6] = TS_SIDE_3_SIZE_1_REV;
	else if(is_wall(tiles[19]))
		WALL_SLABS[6] = TS_FRONT_3_SIZE_1_REV;
	else if(is_wall(tiles[20]))
		WALL_SLABS[6] = TS_SIDE_3_SIZE_2_REV;
	else if(is_wall(tiles[29]))
		WALL_SLABS[6] = TS_FRONT_3_SIZE_2_REV;
	else
		WALL_SLABS[6] = TS_NONE;

	/* slab 7 */
	if(is_wall(tiles[5]))
		WALL_SLABS[7] = TS_SIDE_2_SIZE_1_REV;
	else if(is_wall(tiles[11]))
		WALL_SLABS[7] = TS_FRONT_2_SIZE_1_REV;
	else if(is_wall(tiles[12]))
		WALL_SLABS[7] = TS_SIDE_2_SIZE_2_REV;
	else if(is_wall(tiles[20]))
		WALL_SLABS[7] = TS_FRONT_2_SIZE_2_REV;
	else if(is_wall(tiles[21]))
		WALL_SLABS[7] = TS_SIDE_2_SIZE_3_REV;
	else if(is_wall(tiles[30]))
		WALL_SLABS[7] = TS_FRONT_2_SIZE_3_REV;
	else
		WALL_SLABS[7] = TS_NONE;

	/* slab 8 */
	if(is_wall(tiles[1]))
		WALL_SLABS[8] = TS_SIDE_1_SIZE_1_REV;
	else if(is_wall(tiles[5]))
		WALL_SLABS[8] = TS_FRONT_1_SIZE_1_REV;
	else if(is_wall(tiles[6]))
		WALL_SLABS[8] = TS_SIDE_1_SIZE_2_REV;
	else if(is_wall(tiles[12]))
		WALL_SLABS[8] = TS_FRONT_1_SIZE_2_REV;
	else if(is_wall(tiles[13]))
		WALL_SLABS[8] = TS_SIDE_1_SIZE_3_REV;
	else if(is_wall(tiles[21]))
		WALL_SLABS[8] = TS_FRONT_1_SIZE_3_REV;
	else if(is_wall(tiles[22]))
		WALL_SLABS[8] = TS_SIDE_1_SIZE_4_REV;
	else if(is_wall(tiles[31]))
		WALL_SLABS[8] = TS_FRONT_1_SIZE_4_REV;
	else
		WALL_SLABS[8] = TS_NONE;
}

static void
update_callback()
{
	/* figure out sides,fronts,sizes, etc..
	 * all this info gets placed into WALL_SLABS 
	 */
	determine_slabs();
}

static void
render_callback(float interpolation)
{
	int image_num, slab_num, x_offset;

	/*
	for(i=0; i<9; i++) {
		printf("slab %d: type %d, size %d\n", i, WALL_SLABS[i].type, WALL_SLABS[i].size);
	}
	*/

    SDL_Rect fill_rect = {MAP_DISPLAY_OFFSET_X, MAP_DISPLAY_OFFSET_Y, 700, 600};
    sdl_fill_screen(&fill_rect, 0, 0, 0);

    /* draw ceiling and floor */
    image_num = TILESET_MAP[CURRENT_TILESET][TS_CEILING];
    sdl_put_image(gfx_cache_get(image_num), MAP_DISPLAY_OFFSET_X, MAP_DISPLAY_OFFSET_Y);
    image_num = TILESET_MAP[CURRENT_TILESET][TS_FLOOR];
    sdl_put_image(gfx_cache_get(image_num), MAP_DISPLAY_OFFSET_X, MAP_DISPLAY_OFFSET_Y);

	/* use WALL_SLABS info and TILESET_MAP to render each image to the
	 * correct location on the screen */

#define PUT_TILE_AUX(number)                                            \
	slab_num = WALL_SLABS[ (number-1) ];								\
	if(slab_num > TS_NONE) {											\
		image_num = TILESET_MAP[CURRENT_TILESET][slab_num];				\
		x_offset = MAP_DISPLAY_OFFSET_X + SLAB_ ## number ## _OFFSET;	\
		sdl_put_image(gfx_cache_get(image_num), x_offset, MAP_DISPLAY_OFFSET_Y); \
	}

    PUT_TILE_AUX(1);
    PUT_TILE_AUX(2);
    PUT_TILE_AUX(3);
    PUT_TILE_AUX(4);
    PUT_TILE_AUX(6);
    PUT_TILE_AUX(7);
    PUT_TILE_AUX(8);
    PUT_TILE_AUX(9);

	/* Special case of the direct tile can have different offsets
	 * depending on how close the player is to the wall directly in
	 * front of the player. This should also be drawn lastly. */

	slab_num = WALL_SLABS[4];
	if(slab_num > TS_NONE)
	{
		switch(slab_num) {
		case TS_DIRECT_SIZE_1:
			x_offset = MAP_DISPLAY_OFFSET_X + SLAB_2_OFFSET;
			break;
		case TS_DIRECT_SIZE_2:
			x_offset = MAP_DISPLAY_OFFSET_X + SLAB_3_OFFSET;
			break;
		case TS_DIRECT_SIZE_3:
			x_offset = MAP_DISPLAY_OFFSET_X + SLAB_4_OFFSET;
			break;
		case TS_DIRECT_SIZE_4:
			x_offset = MAP_DISPLAY_OFFSET_X + SLAB_5_OFFSET;
			break;
		}
		image_num = TILESET_MAP[CURRENT_TILESET][slab_num];
		sdl_put_image(gfx_cache_get(image_num), x_offset, MAP_DISPLAY_OFFSET_Y);
	}
}

void
create_map()
{
    int i;

    GAMESTATE.map.data = malloc(sizeof(int*) * GAMESTATE.map.width);
    for(i=0; i<GAMESTATE.map.width; i++)
        GAMESTATE.map.data[i] = malloc(sizeof(int) * GAMESTATE.map.height);

	render_callback_add(render_callback, RL_MAP);
	update_callback_add(update_callback);
}

void
destroy_map()
{
    int i;

    if(!GAMESTATE.map.data)
        return;

    for(i=0; i<GAMESTATE.map.width; i++)
        free(GAMESTATE.map.data[i]);
    
    free(GAMESTATE.map.data);

    GAMESTATE.map.data = NULL;
}

int
read_map(char *filename)
{
    int x, y;
    FILE *fin = fopen(filename, "r");
    int result;

    destroy_map();

    result = fscanf(fin, "%d %d", &GAMESTATE.map.width, &GAMESTATE.map.height);
    
    if(result != 2)
        return -1;
    
    create_map();
    
    for(y=0; y<GAMESTATE.map.height; y++)
        for(x=0; x<GAMESTATE.map.width; x++) {
            result = fscanf(fin, "%d", &GAMESTATE.map.data[x][y]);
            if(result != 1)
                return -1;
        }    

    fclose(fin);

    return 0;
}

void
print_map()
{
    int x, y;
	
	LOG(">>> CURRENT MAP START <<<\n");
    for(y=0; y<GAMESTATE.map.height; y++) {
        for(x=0; x<GAMESTATE.map.width; x++)
            LOG("%d ", GAMESTATE.map.data[x][y]);
        LOG("\n");
    }
	LOG(">>> CURRENT MAP END <<<\n");
}

int
map_element_at(int x, int y)
{
    if(x < 0 || x >= GAMESTATE.map.width || y < 0 || y >= GAMESTATE.map.height)
        return -1;

    return GAMESTATE.map.data[x][y];
}

void
map_read_tileset(char *tileset_file)
{
	FILE * fin;
	int result;
	int tileset_num;
	unsigned char read_at_least_one = 0;

	LOG("reading tileset from \"%s\"\n", tileset_file);

	fin = fopen(tileset_file, "r");

	while(1) {
		/* first parameter is the tileset number */
		result = fscanf(fin, "%d", &tileset_num);

		/* if we've read at least on tileset then this could signal an
		 * EOF */
		if(read_at_least_one && result == EOF)
			break;

		assert(result == 1);
		
		/* read sides */
		result = fscanf(fin, "%d %d %d %d",
						&TILESET_MAP[tileset_num][TS_SIDE_1_SIZE_1],
						&TILESET_MAP[tileset_num][TS_SIDE_2_SIZE_1],
						&TILESET_MAP[tileset_num][TS_SIDE_3_SIZE_1],
						&TILESET_MAP[tileset_num][TS_SIDE_4_SIZE_1]);

		assert(result == 4);

		result = fscanf(fin, "%d %d %d %d",
						&TILESET_MAP[tileset_num][TS_SIDE_1_SIZE_2],
						&TILESET_MAP[tileset_num][TS_SIDE_2_SIZE_2],
						&TILESET_MAP[tileset_num][TS_SIDE_3_SIZE_2],
						&TILESET_MAP[tileset_num][TS_SIDE_4_SIZE_2]);
		assert(result == 4);
		
		result = fscanf(fin, "%d %d %d %d",
						&TILESET_MAP[tileset_num][TS_SIDE_1_SIZE_3],
						&TILESET_MAP[tileset_num][TS_SIDE_2_SIZE_3],
						&TILESET_MAP[tileset_num][TS_SIDE_3_SIZE_3],
						&TILESET_MAP[tileset_num][TS_SIDE_4_SIZE_3]);
		assert(result == 4);

		result = fscanf(fin, "%d %d %d %d",
						&TILESET_MAP[tileset_num][TS_SIDE_1_SIZE_4],
						&TILESET_MAP[tileset_num][TS_SIDE_2_SIZE_4],
						&TILESET_MAP[tileset_num][TS_SIDE_3_SIZE_4],
						&TILESET_MAP[tileset_num][TS_SIDE_4_SIZE_4]);
		assert(result == 4);

		/* read fronts */
		result = fscanf(fin, "%d %d %d %d",
						&TILESET_MAP[tileset_num][TS_FRONT_1_SIZE_1],
						&TILESET_MAP[tileset_num][TS_FRONT_2_SIZE_1],
						&TILESET_MAP[tileset_num][TS_FRONT_3_SIZE_1],
						&TILESET_MAP[tileset_num][TS_FRONT_4_SIZE_1]);
		assert(result == 4);

		result = fscanf(fin, "%d %d %d %d",
						&TILESET_MAP[tileset_num][TS_FRONT_1_SIZE_2],
						&TILESET_MAP[tileset_num][TS_FRONT_2_SIZE_2],
						&TILESET_MAP[tileset_num][TS_FRONT_3_SIZE_2],
						&TILESET_MAP[tileset_num][TS_FRONT_4_SIZE_2]);
		assert(result == 4);
		
		result = fscanf(fin, "%d %d %d %d",
						&TILESET_MAP[tileset_num][TS_FRONT_1_SIZE_3],
						&TILESET_MAP[tileset_num][TS_FRONT_2_SIZE_3],
						&TILESET_MAP[tileset_num][TS_FRONT_3_SIZE_3],
						&TILESET_MAP[tileset_num][TS_FRONT_4_SIZE_3]);
		assert(result == 4);

		result = fscanf(fin, "%d %d %d %d",
						&TILESET_MAP[tileset_num][TS_FRONT_1_SIZE_4],
						&TILESET_MAP[tileset_num][TS_FRONT_2_SIZE_4],
						&TILESET_MAP[tileset_num][TS_FRONT_3_SIZE_4],
						&TILESET_MAP[tileset_num][TS_FRONT_4_SIZE_4]);
		assert(result == 4);

		/* read reverse sides */
		result = fscanf(fin, "%d %d %d %d",
						&TILESET_MAP[tileset_num][TS_SIDE_1_SIZE_1_REV],
						&TILESET_MAP[tileset_num][TS_SIDE_2_SIZE_1_REV],
						&TILESET_MAP[tileset_num][TS_SIDE_3_SIZE_1_REV],
						&TILESET_MAP[tileset_num][TS_SIDE_4_SIZE_1_REV]);

		assert(result == 4);

		result = fscanf(fin, "%d %d %d %d",
						&TILESET_MAP[tileset_num][TS_SIDE_1_SIZE_2_REV],
						&TILESET_MAP[tileset_num][TS_SIDE_2_SIZE_2_REV],
						&TILESET_MAP[tileset_num][TS_SIDE_3_SIZE_2_REV],
						&TILESET_MAP[tileset_num][TS_SIDE_4_SIZE_2_REV]);
		assert(result == 4);
		
		result = fscanf(fin, "%d %d %d %d",
						&TILESET_MAP[tileset_num][TS_SIDE_1_SIZE_3_REV],
						&TILESET_MAP[tileset_num][TS_SIDE_2_SIZE_3_REV],
						&TILESET_MAP[tileset_num][TS_SIDE_3_SIZE_3_REV],
						&TILESET_MAP[tileset_num][TS_SIDE_4_SIZE_3_REV]);
		assert(result == 4);

		result = fscanf(fin, "%d %d %d %d",
						&TILESET_MAP[tileset_num][TS_SIDE_1_SIZE_4_REV],
						&TILESET_MAP[tileset_num][TS_SIDE_2_SIZE_4_REV],
						&TILESET_MAP[tileset_num][TS_SIDE_3_SIZE_4_REV],
						&TILESET_MAP[tileset_num][TS_SIDE_4_SIZE_4_REV]);
		assert(result == 4);

		/* read reverse fronts */
		result = fscanf(fin, "%d %d %d %d",
						&TILESET_MAP[tileset_num][TS_FRONT_1_SIZE_1_REV],
						&TILESET_MAP[tileset_num][TS_FRONT_2_SIZE_1_REV],
						&TILESET_MAP[tileset_num][TS_FRONT_3_SIZE_1_REV],
						&TILESET_MAP[tileset_num][TS_FRONT_4_SIZE_1_REV]);
		assert(result == 4);

		result = fscanf(fin, "%d %d %d %d",
						&TILESET_MAP[tileset_num][TS_FRONT_1_SIZE_2_REV],
						&TILESET_MAP[tileset_num][TS_FRONT_2_SIZE_2_REV],
						&TILESET_MAP[tileset_num][TS_FRONT_3_SIZE_2_REV],
						&TILESET_MAP[tileset_num][TS_FRONT_4_SIZE_2_REV]);
		assert(result == 4);
		
		result = fscanf(fin, "%d %d %d %d",
						&TILESET_MAP[tileset_num][TS_FRONT_1_SIZE_3_REV],
						&TILESET_MAP[tileset_num][TS_FRONT_2_SIZE_3_REV],
						&TILESET_MAP[tileset_num][TS_FRONT_3_SIZE_3_REV],
						&TILESET_MAP[tileset_num][TS_FRONT_4_SIZE_3_REV]);
		assert(result == 4);

		result = fscanf(fin, "%d %d %d %d",
						&TILESET_MAP[tileset_num][TS_FRONT_1_SIZE_4_REV],
						&TILESET_MAP[tileset_num][TS_FRONT_2_SIZE_4_REV],
						&TILESET_MAP[tileset_num][TS_FRONT_3_SIZE_4_REV],
						&TILESET_MAP[tileset_num][TS_FRONT_4_SIZE_4_REV]);
		assert(result == 4);

		/* read direct facing walls */
		result = fscanf(fin, "%d %d %d %d",
						&TILESET_MAP[tileset_num][TS_DIRECT_SIZE_1],
						&TILESET_MAP[tileset_num][TS_DIRECT_SIZE_2],
						&TILESET_MAP[tileset_num][TS_DIRECT_SIZE_3],
						&TILESET_MAP[tileset_num][TS_DIRECT_SIZE_4]);
		assert(result == 4);

		/* read ceiling and floor */
		result = fscanf(fin, "%d %d",
						&TILESET_MAP[tileset_num][TS_CEILING],
						&TILESET_MAP[tileset_num][TS_FLOOR]);
		assert(result == 2);

		read_at_least_one = 1;
	}
	
	fclose(fin);

}

/* used for astar routine */
int
map_move_cost_at(int x, int y)
{
	int wall_type = map_element_at(x, y);
	if(is_wall(wall_type))
		return -1;
	else
		return 10;
}
