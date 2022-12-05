
#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h>
#include "pico/stdlib.h"
#include "day5.h"
#include <cstring>
#include <vector>

using namespace std;


/**
 * @brief A struct for representing each box in our Stack
 * 
 */
typedef struct Stack_t {
    char value;
    Stack_t* below;
} Stack;

/**
 * @brief Struct for representing each instruction to be performed on the stacks
 * 
 */
typedef struct {
    int count, from, to;
} Instruction;

/**
 * @brief State for managing which part of the insutruction is being read.
 * 
 */
enum ReadLineState {
    TO,
    FROM,
    COUNT
};

/**
 * @brief Load the stack input
 * Follows a few rules for loading the input.
 * The total number of stacks can be derived from the line length. 
 * Each Stack is 3 characters in width, with another 1 character for the space between.
 * We can't just divide the line length by 4 (3 for width, 1 for space) to get the count as
 * the last element ends without a space so there is actually n-1 spaces per line.
 * So the calculation for the number of stacks is n = (length + 1) / 4
 * After we then read line by line and position our index to read the character ([) before 
 * the stack value. If this character exists we can read the next character as our input.
 * When we hit a newline as an input we can end our parsing as we had read all of the stack input.
 * 
 * We read our stacks from top to bottom, and we keep a reference to the top of each stack.
 * We do this as we want a FILO queue for processing our challenge.
 *  
 * @param input The AOC input
 * @param stacks A pointer to a two dimensional stacks array that will contain our data.
 * @param numberOfColumns A pointer to an integer to store the number of columns
 * @return int An index pointing to a position after the stack input
 */
int loadStacks(const char input[], Stack*** stacks, int *numberOfColumns)
{
    char* endOfLine = strchr(input, '\n');

    if (endOfLine == NULL)
    {
        return -1;
    }

    int lineLength, lineEnd, column, index;
    size_t stacksSize;

    // Calculating Line length and column count
    lineEnd = (endOfLine - input) + 1;
    lineLength = lineEnd;
    *numberOfColumns = lineLength >> 2;

    // Calculating and allocating memory
    stacksSize = sizeof(Stack_t*) * (*numberOfColumns);
    *stacks = (Stack**) malloc(stacksSize);
    
    // Temp pointers for filling out start
    // These will point to the bottom of each stack
    Stack *builders[*numberOfColumns];

    // Null out data
    memset(builders, 0, stacksSize);

    index = 0;

    while(input[index] != '\n')
    {
        column = 0;
        // Read entire line
        for (;index < lineLength;)
        {
            // A box exists on this stack
            if (input[index] == '[')
            {
                char value = input[++index];
                // Either allocate a new stack start, or add to a previous stack
                builders[column] = builders[column] == NULL ? 
                    (*stacks)[column] = (Stack*) malloc(sizeof(Stack_t*)) : 
                    builders[column]->below = (Stack*) malloc(sizeof(Stack_t*));
                
                *builders[column] = { value, NULL };
                // We already incremented once, so we only need to move 3 more
                index += 3;
            }
            else
            {
                // Move 4 to next stack input
                index += 4;
            }
            column++;
        }
        // Incrememnt our line end position
        lineLength += (lineEnd + 1);
    }

    return index += 2;
}

/**
 * @brief Handles a sequential instruction.
 * Performs each instruction each time per count
 * 
 * @param instruction 
 * @param stacks 
 */
inline void parseSequentialInstruction(Instruction instruction, Stack **stacks)
{
    int count = instruction.count;
    Stack *to, *from, *temp;

    to = stacks[instruction.to];
    from = stacks[instruction.from];
    for (size_t i = 0; i < count; i++)
    {
        temp = from->below;
        from->below = to;
        to = from;
    }

    stacks[instruction.to] = to;
    stacks[instruction.from] = from;
}

/**
 * @brief Handles batched insutrctions
 * Performs a single instruction by moving a batch of batches
 * by the size of the count.
 * 
 * @param instruction 
 * @param stacks 
 */
inline void parseBatchedInstruction(Instruction instruction, Stack **stacks)
{
    int count = instruction.count;
    Stack *bottom = NULL, *top;


    top = stacks[instruction.from];
    for (size_t i = 0; i < count; i++)
    {
        bottom = bottom == NULL ? top : bottom->below;
    }

    stacks[instruction.from] = bottom->below;
    bottom->below = stacks[instruction.to];
    stacks[instruction.to] = top;
}

/**
 * @brief Reads an instruction line.
 * Reads the line from back to front to make multi digit numbers easier to parse.
 * Reads until it finds a newline
 * 
 * @param position The position of an end of the line
 * @return Instruction 
 */
inline Instruction readInstruction(const char *position)
{
    char value;
    int radix = 1, current = 0;
    Instruction Instruction;

    ReadLineState state = TO;

    while((value = *--position) != '\n')
    {
        if (value >= 48 && value <= 57)
        {
            current += (value - 48) * radix;
            radix *= 10;
        }
        else
        {
            switch (state)
            {
            case TO:
                Instruction.to = current - 1;
                state = FROM;
                position -= 3;
                break;
            case FROM:
                Instruction.from = current - 1;
                state = COUNT;
                position -= 5;
                break;
            default:
                Instruction.count = current;
                position -= 4;
                break;
            }
            radix = 1;
            current = 0;
        }
    }
    return Instruction;
}

void freeStacks(Stack** stacks, int stackCount)
{
    for (size_t i = 0; i < stackCount; i++)
    {
        Stack* stack = stacks[i], *temp;
        while (stack != NULL)
        {
            temp = stack;
            stack = stack->below;
            free(temp);
        }
    }

    free(stacks);    
}

/**
 * @brief Part 1 of Day 5
 * The problem consists of two parts within the input. The first part describes an orientation of a stacks of boxes.
 * The second are a set of instructions for moving boxes. The problem is fairly easy to describe as we need to import the
 * initial orientation of the stacks, and then perform each operation on these stacks. Each box has an assigned letter, and 
 * our solution is printing out the letter from the top box of each stack.
 * 
 * I solved this one by utilizing linked lists to act as a FILO queue, where I can load the stacks in each queue. I build the number of 
 * queues by calculating the number of stacks from the line length. Afterwards I then perform each action sequentially by
 * moving each box one by one from 1 queue to another. When loading in the initial state I load it in from the top to the bottom.
 * I did this to make it easier to read as I could just read top the bottom. However I did need to keep two lots of pointers,
 * one for the top, and one for the bottom.
 * 
 * @return int
 */
int partOne(const char input[])
{
    Stack** stacks;
    int numberOfColumns, start;

    start = loadStacks(input, &stacks, &numberOfColumns);

    size_t inputLength = strlen(input) - 1;

    const char* endOfLine = input + start;


    while ((endOfLine = strchr(endOfLine, '\n')) != NULL)
    {
        Instruction instruction = readInstruction(endOfLine++);
        parseSequentialInstruction(instruction, stacks);

    }

    Instruction instruction = readInstruction(input + inputLength + 1);
    parseSequentialInstruction(instruction, stacks);

    for (size_t i = 0; i < numberOfColumns; i++)
    {
        Stack* stack = stacks[i];
        printf("%c", stacks[i]->value);
    }
    printf("\n");

    freeStacks(stacks, numberOfColumns);

    return 0;
}

/**
 * @brief Part 12of Day 5
 * The problem consists of two parts within the input. The first part describes an orientation of a stacks of boxes.
 * The second are a set of instructions for moving boxes. The problem is fairly easy to describe as we need to import the
 * initial orientation of the stacks, and then perform each operation on these stacks. Each box has an assigned letter, and 
 * our solution is printing out the letter from the top box of each stack. Part 2 dictates that the instructions are applied
 * in batches rather than 1 by 1.
 * 
 * Part two is the exact same as part 1, however I just needed to change the instruction parser from a 1 by 1 operation to
 * a single batched operation. 
 * 
 * @return int
 */
int partTwo(const char input[])
{
    Stack** stacks;
    int numberOfColumns, start;

    start = loadStacks(input, &stacks, &numberOfColumns);

    size_t inputLength = strlen(input) - 1;

    const char* endOfLine = input + start;


    while ((endOfLine = strchr(endOfLine, '\n')) != NULL)
    {
        Instruction instruction = readInstruction(endOfLine++);
        parseBatchedInstruction(instruction, stacks);

    }

    Instruction instruction = readInstruction(input + inputLength + 1);
    parseBatchedInstruction(instruction, stacks);

    for (size_t i = 0; i < numberOfColumns; i++)
    {
        Stack* stack = stacks[i];
        printf("%c", stacks[i]->value);
    }
    printf("\n");

    freeStacks(stacks, numberOfColumns);

    return 0;
}

int main()
{
    stdio_init_all();

    sleep_ms(2000);

    printf("\n\n---- Day 5 part 1 Test ----\n");
    printf("Result: %d\n", partOne(TEST_INPUT));
    printf("---- Day 5 part 1 ----\n");
    printf("Result: %d\n", partOne(ACTUAL_INPUT));
    printf("---- Day 5 part 2 Test ----\n");
    printf("Result: %d\n", partTwo(TEST_INPUT));
    printf("---- Day 5 part 2 ----\n");
    printf("Result: %d\n", partTwo(ACTUAL_INPUT));
}