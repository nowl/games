#ifndef __ASTAR_H__
#define __ASTAR_H__

/* Find the best path from (x,y) to (dest_x, dest_y) through a cost matrix of
 * width and height. The best path is returned in [x1,y1,x2,y2,..] format
 * into path with path_length length. It is up to the caller to free memory
 * allocated for path.
 */
void astar_find_path(float *costs, int width, int height,
                     int x, int y, int dest_x, int dest_y,
                     int *path, int *path_length);

#endif /* __ASTAR_H__ */
