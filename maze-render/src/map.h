#ifndef MAP_H__
#define MAP_H__

void create_map();
void destroy_map();
int read_map(char *filename);
void print_map();
int map_element_at(int x, int y);
int map_move_cost_at(int x, int y);

void map_read_tileset(char *tileset_file);

#endif  /* MAP_H__ */
