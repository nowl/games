#ifndef __COLLIDE_H__
#define __COLLIDE_H__

#include <SDL.h>

int collide_point_in_rect(float x, float y, SDL_Rect *rect);

int collide_lines_intersect(float x1, float y1, float x2,
                            float y2, float x3, float y3,
                            float x4, float y4);

int collide_point_in_polygon(float x, float y, float *polygon, int num_points);

int collide_rect_intersect(SDL_Rect *bb1, SDL_Rect *bb2);

int collide_polygon_intersect(float *poly1, int poly1_num_points,
                              float *poly2, int poly2_num_points);

#endif /* __COLLIDE_H__ */
