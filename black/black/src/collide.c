#include "black.h"

int
collide_point_in_rect(float x, float y, SDL_Rect *rect)
{
    float min_x = rect->x;
    float min_y = rect->y;
    float max_x = min_x + rect->w;
    float max_y = min_y + rect->h;
    printf("checking for (%f, %f) within (%f, %f, %f, %f)\n", x, y, min_x, max_x, min_y, max_y);
    return x >= min_x && x < max_x && y >= min_y && y < max_y;
}

int
collide_lines_intersect(float x1, float y1, float x2,
                        float y2, float x3, float y3,
                        float x4, float y4)
{
    float den = (y2-y1)*(x4-x3) + (y3-y4)*(x2-x1);
    if(den == 0)
        return FALSE;

    float a = ((y3-y1)*(x2-x1) + (y1-y2)*(x3-x1)) / den;
    if((a <= 0) || (a >= 1))
        return FALSE;

    den = (y4-y3)*(x2-x1) + (y2-y1)*(x3-x4);
    if(den == 0)
        return FALSE;

    a = ((x4-x3)*(y1-y3) + (y4-y3)*(x3-x1)) / den;
    if((a <= 0) || (a >= 1))
        return FALSE;

    return TRUE;
}

int
collide_point_in_polygon(float x, float y,
                         float *polygon, int num_points)
{
    float x_end = 1e6;
    float y_end = 1e6 + 1.1;
    int intersections = 0;
    int do_last_step = FALSE;

    // check if the last step is necessary by checking if the first point is not equal to the last point
    if(polygon[2*num_points-2] != polygon[0] || polygon[2*num_points-1] != polygon[1])
        do_last_step = TRUE;

    // scan through the points
    int n = 2;
    float prev_x = polygon[0];
    float prev_y = polygon[1];
    for(; n<=num_points; n++) {
        float cur_x = polygon[2*n-2];
        float cur_y = polygon[2*n-1];
        if( collide_lines_intersect(x, y, x_end, y_end, cur_x, cur_y, prev_x, prev_y) )
            ++intersections;
        prev_x = cur_x;
        prev_y = cur_y;
    }

    // check initial point if necessary
    if(do_last_step) {
        prev_x = polygon[2*num_points-2];
        prev_y = polygon[2*num_points-1];
        float cur_x = polygon[0];
        float cur_y = polygon[1];
        if( collide_lines_intersect(x, y, x_end, y_end, cur_x, cur_y, prev_x, prev_y) )
            ++intersections;
    }

    if((intersections % 2) == 1)
        return TRUE;

    return FALSE;
}

int
collide_rect_intersect(SDL_Rect *bb1, SDL_Rect *bb2)
{
    float bb1_min_x = bb1->x, bb1_min_y = bb1->y;
    float bb1_max_x = bb1_min_x + bb1->w;
    float bb1_max_y = bb1_min_y + bb1->h;
    float bb2_min_x = bb2->x, bb2_min_y = bb2->y;
    float bb2_max_x = bb2_min_x + bb2->w;
    float bb2_max_y = bb2_min_y + bb2->h;

    float midpoint_1x = (bb1_max_x + bb1_min_x) / 2;
    float midpoint_2x = (bb2_max_x + bb2_min_x) / 2;
    float span_1x = bb1_max_x - midpoint_1x;
    float span_2x = bb2_max_x - midpoint_2x;

    if(abs(midpoint_2x - midpoint_1x) > (span_1x + span_2x))
        return FALSE;

    midpoint_1x = (bb1_max_y + bb1_min_y) / 2;
    midpoint_2x = (bb2_max_y + bb2_min_y) / 2;
    span_1x = bb1_max_y - midpoint_1x;
    span_2x = bb2_max_y - midpoint_2x;

    if(abs(midpoint_2x - midpoint_1x) > (span_1x + span_2x))
        return FALSE;

    return TRUE;
}

struct line_holder
{
    float x1, y1, x2, y2;
};

/* TODO: grow this instead of setting the max at 100 */
#define MAX_POLYGON_LENGTH 100
static char lines[sizeof(struct line_holder) * MAX_POLYGON_LENGTH * 2];
static void *lines1 = &lines[0];
static void *lines2 = &lines[sizeof(struct line_holder) * MAX_POLYGON_LENGTH];

// TODO: this is currently not thread-safe
int
collide_polygon_intersect(float *poly1, int poly1_num_points,
                          float *poly2, int poly2_num_points)
{
    // first test if any of the points are within the other polygon
    int n;
    for(n=0; n<poly1_num_points; n++)
    {
        float x = poly1[2*n];
        float y = poly1[2*n+1];
        if( collide_point_in_polygon(x, y, poly2, poly2_num_points) )
            return TRUE;
    }
    for(n=0; n<poly2_num_points; n++)
    {
        float x = poly2[2*n];
        float y = poly2[2*n+1];
        if( collide_point_in_polygon(x, y, poly1, poly1_num_points) )
            return TRUE;
    }

    // now do a brute force line check

    int do_last_step_for_1 = FALSE, do_last_step_for_2 = FALSE;

    // check if the last step is necessary by checking if the first point is not equal to the last point
    if(poly1[2*poly1_num_points-2] != poly1[0] || poly1[2*poly1_num_points-1] != poly1[1])
        do_last_step_for_1 = TRUE;
    if(poly2[2*poly2_num_points-2] != poly2[0] || poly2[2*poly2_num_points-1] != poly2[1])
        do_last_step_for_2 = TRUE;

    // collect the lines for polygon 1

    int lines1_count = 0;
    float prev_x = poly1[0];
    float prev_y = poly1[1];
    struct line_holder *lh = lines1;
    for(n=2; n<=poly1_num_points; n++) {
        float cur_x = poly1[2*n-2];
        float cur_y = poly1[2*n-1];

        lh->x1 = cur_x;
        lh->y1 = cur_y;
        lh->x2 = prev_x;
        lh->y2 = prev_y;

        prev_x = cur_x;
        prev_y = cur_y;
        ++lh;
        ++lines1_count;
    }

    if(do_last_step_for_1) {
        prev_x = poly1[2*poly1_num_points-2];
        prev_y = poly1[2*poly1_num_points-1];
        float cur_x = poly1[0];
        float cur_y = poly1[1];
        lh->x1 = cur_x;
        lh->y1 = cur_y;
        lh->x2 = prev_x;
        lh->y2 = prev_y;
        ++lines1_count;
    }

    // collect the lines for polygon 2

    int lines2_count = 0;
    prev_x = poly2[0];
    prev_y = poly2[1];
    lh = lines2;
    for(n=2; n<=poly2_num_points; n++) {
        float cur_x = poly2[2*n-2];
        float cur_y = poly2[2*n-1];

        lh->x1 = cur_x;
        lh->y1 = cur_y;
        lh->x2 = prev_x;
        lh->y2 = prev_y;

        prev_x = cur_x;
        prev_y = cur_y;
        ++lh;
        ++lines2_count;
    }

    if(do_last_step_for_2) {
        prev_x = poly2[2*poly2_num_points-2];
        prev_y = poly2[2*poly2_num_points-1];
        float cur_x = poly2[0];
        float cur_y = poly2[1];
        lh->x1 = cur_x;
        lh->y1 = cur_y;
        lh->x2 = prev_x;
        lh->y2 = prev_y;
        ++lines2_count;
    }

    // finally compare the line intersections

    struct line_holder *lh1 = lines1;
    struct line_holder *lh2 = lines2;
    int m;
    for(m=0; m<lines1_count; m++) {
        for(n=0; n<lines2_count; n++) {
            if( collide_lines_intersect(lh1->x1, lh1->y1, lh1->x2, lh1->y2,
                                        lh2->x1, lh2->y1, lh2->x2, lh2->y2) )
                return TRUE;
            ++lh2;
        }
        ++lh1;
    }

    return FALSE;
}
