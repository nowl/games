#ifndef PLAYER_H__
#define PLAYER_H__

void player_init();

void player_attempt_move_left();
void player_attempt_move_right();
void player_attempt_move_forward();
void player_attempt_move_backward();
void player_attempt_rotate_ccw();
void player_attempt_rotate_cw();

#endif	/* PLAYER_H__ */
