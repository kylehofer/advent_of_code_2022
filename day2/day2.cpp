
#include <stdio.h>
#include <stdint.h> 
#include "pico/stdlib.h"
#include "day2.h"

typedef struct {
    uint32_t win; 
    uint32_t draw;
    int score;
} ResponseResult;

typedef struct {
    uint32_t responseScore[MOVES_SIZE];
    int score;
} Strategy;

/**
 * @brief Part 1 of Day 2
 * The problem consists of an input which contains a list of Rock Paper Scissors moves and responses.
 * We've been given the brief that for each Move, we will respond with the response in the list.
 * Our goal to is to calculate our total score by adding the value of our response, and adding a value for the result of the game.
 * The values are as follows:
 *  ** MOVES **
 * A: Rock, B: Paper, C: Scissors
 * ** Response **
 * X: Rock, Y: Paper, Z: Scissors
 * ** Scoring **
 * Win: 6, Draw: 3, Loss: 0
 * Rock: 1, Paper: 2, Scissors: 3
 * 
 * For this challenge I created a struct that stores the 3 possible responses, with what they win to
 * and what they lose to. Along with their respective scores. For example Rock (A) wins against Scissors (C)
 * loses to Paper (B), and has a Score of 1. The indexing of the array is handled by using an Enum to denote each move,
 * both giving us a handy max array size, and easy array indexing.
 * Lastly to calculate the value I'm using branchless operators.
 * 
 * @return int 
 */
int partOne(const uint32_t input[], uint32_t size)
{
    ResponseResult responseResults[RESPONSE_SIZE] =
    {
         { C, A, 1 },
         { A, B, 2 },
         { B, C, 3 }
    };

    int32_t movesCount = size - 1;
    uint32_t score = 0;

    while (movesCount > 0)
    {
        ResponseResult player = responseResults[input[movesCount--]];
        uint32_t opponent = input[movesCount--];


        score += player.score +
            ((player.win == opponent) * WIN) +
            ((player.draw == opponent) * DRAW);
    }
    
    return score;
}

/**
 * @brief Part 2 of Day 2
 * The problem consists of an input which contains a list of Rock Paper Scissors moves and responses.
 * We've been given the brief that for each Move, we will respond with the response in the list.
 * Our goal to is to calculate our total score by adding the value of our response, and adding a value for the result of the game.
 * Different from Part 1, our response is to cause a specific outcome rather than pick a specific move.
 * The values are as follows:
 *  ** MOVES **
 * A: Rock, B: Paper, C: Scissors
 * ** Response **
 * X: Lose, Y: Draw, Z: Win
 * ** Scoring **
 * Win: 6, Draw: 3, Loss: 0
 * Rock: 1, Paper: 2, Scissors: 3
 * 
 * Like before I made another struct, but instead of responses I store a game strategy. Each response has denotes an objective to
 * Lose, Draw, or Win. I use a strategy for each move, with the strategy containing an array of responses. The index of the response
 * relates to the index of the move. For example if the opponent choses Paper (B), and my strategy is to Win (Z), I will take the 3rd (Z) index 
 * of the strategies, and the pick the 2nd (B) index of the responses which would the value of 3 for Scissors.
 * 
 * This means I can use array indexing to quickly grab both the game result (Win/Loss/Draw) with the score related to the move picked.
 * 
 * @return int 
 */
int partTwo(const uint32_t input[], uint32_t size)
{
    Strategy strategies[RESPONSE_SIZE] =
    {
         { { 3, 1, 2 }, LOSS },
         { { 1, 2, 3 }, DRAW },
         { { 2, 3, 1 }, WIN }
    };

    int32_t movesCount = size - 1;
    uint32_t score = 0;

    int32_t i = 0;

    while (movesCount > 0)
    {
        Strategy strategy = strategies[input[movesCount--]];
        uint32_t responseScore = strategy.responseScore[input[movesCount--]];


        score += strategy.score + responseScore;
    }
    
    return score;
}

#define ARRAY_SIZE(a) (sizeof((a)) / sizeof((a)[0]))

int main()
{
    stdio_init_all();

    sleep_ms(1000);

    printf("---- Day 2 Part 1 Test  ----\n");
    printf("Result: %d\n", partOne(TEST_INPUT, ARRAY_SIZE(TEST_INPUT)));
    printf("---- Day 2 Part 1  ----\n");
    printf("Result: %d\n", partOne(ACTUAL_INPUT, ARRAY_SIZE(ACTUAL_INPUT)));
    printf("---- Day 2 part 2 Test ----\n");
    printf("Result: %d\n", partTwo(TEST_INPUT, ARRAY_SIZE(TEST_INPUT)));
    printf("---- Day 2 part 2 ----\n");
    printf("Result: %d\n", partTwo(ACTUAL_INPUT, ARRAY_SIZE(ACTUAL_INPUT)));
}
