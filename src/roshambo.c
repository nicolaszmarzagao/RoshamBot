#include <stdio>
#include <time.h>
#include <stdlib.h>

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



enum Shapes get_bot_shape()
{
	srand(time(NULL));
	return (enum Shapes)(rand() % 3 + 1); // random number between 1 and 3
}


enum Result determine_victor(enum Shapes player, enum Shapes bot)
{
    if (player == bot) return DRAW;
    
    if ((player == ROCK && bot == SCISSORS) ||
        (player == PAPER && bot == ROCK) ||
        (player == SCISSORS && bot == PAPER)) {
        return WIN;
    }
    
    return LOSE;
}

void update_scores(struct Score p_score, struct Score b_score, enum Result result) 
{
	if (result == WIN)
	{
		p_score.wins  += 1;
		b_score.loses += 1;
	}
	else if (result == LOSE)
	{
		p_score.loses += 1;
		b_score.wins  += 1;
	}
	else 
	{
		p_score.draws += 1;
		b_score.draws += 1;
	}
}

enum Result play_round(struct Score p_score, struct Score b_score, enum Shapes player_shape)
{
	enum Shapes bot_shape = get_bot_shape();
	enum Result result    = determine_victor(player_shape, bot_shape);

	update_scores(p_score, b_score, result);

	return result;		
}

	
