#ifndef ROSHAMBO_H
#define ROSHAMBO_H

struct Score
{
	unsigned int wins;
	unsigned int draws;
	unsigned int loses;
};

enum Shapes 
{
	ROCK     = 1,
	PAPER    = 2,
	SCISSORS = 3,
};

enum Result
{
	WIN  = 1,
	DRAW = 2,
	LOSE = 3,
};

struct Score init_score();
enum Shapes get_bot_shape();
enum Result determine_victor(enum Shapes player, enum Shapes bot);
void update_scores(struct Score p_score, struct Score b_score, enum Result result);
enum Result play_round(struct Score p_score, struct Score b_score, enum Shapes player_shape);

#endif	
