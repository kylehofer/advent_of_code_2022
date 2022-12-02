
#include <stdio.h>
#include <stdint.h> 
#include "pico/stdlib.h"
#include "day1.h"
#include <algorithm>
#include <cstring>

using namespace std;


enum ParserState {
    NEXT,
    END_OF_LINE,
    READING,
    IDLE
};


/**
 * @brief Part 1 of Day 1
 * The problem consists of an input which contains a list of numeric values seperates by newlines
 * Each group of values represents a single elfs storage of food items, with the values being
 * the calories of each item.
 * 
 * Our goal for part 1 is to calculate which elf has the highest amount of calories in their storage.
 * 
 * To do this I loop through the input reading the input 1 character at a time.
 * I manage a state that will change depending on the values being read in, where a newline
 * will represent a new food item, and two new lines a new elf.
 * To calculate the values I read the input from back to front calculating the numeric representation of the character
 * and adding it to the total. By reading back to front I can increment a power of 10 to correctly position each digit without needing
 * to read the whole input at once.
 * 
 * @return int 
 */
int partOne(const char input[])
{
    int radix = 1;
    int state = IDLE;
    int localMax = 0;
    int current = 0;

    size_t inputLength = strlen(input) - 1;

    /*
    Loop through entire Input from back to front.
    We do this so we can progressively add powers of 10 
    without the need to read the entire line first
    */
    for (int i = inputLength; i >= 0; i--)
    {
        // Read the next character
        char value = input[i];

        /* 
        Update the state based off the read character
        This will dictate what action we take on this loop
        */
        switch (value)
        {
        // A single newline will dictate an end line and to read the next value for the elf
        // Two newlines will dictate a new elf
        case '\n': 
            state = state == END_OF_LINE ? NEXT : END_OF_LINE;
            break;
        default:
            state = READING;
            break;
        }

        /*
        The task to execute based off our state
        */
        switch (state)
        {
        case NEXT: // Next Elf, so we will update our max
            localMax = max(localMax, current);
            current = 0;
            break;
        case READING: // Read the next value into our current total
            current += (value - 48) * radix;
            radix *= 10;
            break;
        case END_OF_LINE: // We reset our radix on a newline
            radix = 1;
            break;
        default:
            break;
        }
    }

    // Last calculation as we won't have a newline at the start if our input;
    return max(localMax, current);
}

#define TOP_SIZE 3

/**
 * @brief Updates the maximum values stored in an array.
 * Maintains a ordered array of maxium values to reduce the number of comparisons required.
 * As we can compare from lowest to highest quickly rejecting any values that won't fit in our max values.
 * If our new value is larger than any of our maximums, we will then loop backwards from the position it fits
 * swapping each value as we reach the end of our array.
 * 
 * @param value
 * @param maximums
 */
inline void checkMax(int value, int maximums[])
{
    int max = TOP_SIZE - 1;
    int i = max;

    while (i >= 0)
    {
        if (maximums[i] > value)
        {
            break;
        }
        i--;
    }

    i++;

    while (i <= max)
    {
        value ^= maximums[i];
        maximums[i] ^= value;
        value ^= maximums[i++];
    }


}

/**
 * @brief Part 2 of Day 1
 * The problem consists of an input which contains a list of numeric values seperates by newlines
 * Each group of values represents a single elfs storage of food items, with the values being
 * the calories of each item.
 * 
 * Our goal for part 2 is to calculate the top 3 elves that have the highest amount of calories in their storage.
 * 
 * Similar to part 1 I loop through the input from back to front reading the input 1 character at a time.
 * The difference is how I calculate the maximum, where I now maintain an ordered array of maximum values and
 * so that we can store the top 3 values.
 * 
 * @return int 
 */
int partTwo(const char input[])
{
    int radix = 1;
    int state = IDLE;
    int localMax[TOP_SIZE];

    memset(localMax, 0, TOP_SIZE * sizeof(int));    

    int current = 0;

    size_t inputLength = strlen(input) - 1;

    /*
    Loop through entire Input from back to front.
    We do this so we can progressively add powers of 10 
    without the need to read the entire line first
    */
    for (int i = inputLength; i >= 0; i--)
    {
        // Read the next character
        char value = input[i];

        /* 
        Update the state based off the read character
        This will dictate what action we take on this loop
        */
        switch (value)
        {
        // A single newline will dictate an end line and to read the next value for the elf
        // Two newlines will dictate a new elf
        case '\n': 
            state = state == END_OF_LINE ? NEXT : END_OF_LINE;
            break;
        default:
            state = READING;
            break;
        }

        /*
        The task to execute based off our state
        */
        switch (state)
        {
        case NEXT: // Next Elf, so we will update our max
            checkMax(current, localMax);
            current = 0;
            break;
        case READING: // Read the next value into our current total
            current += (value - 48) * radix;
            radix *= 10;
            break;
        case END_OF_LINE: // We reset our radix on a newline
            radix = 1;
            break;
        default:
            break;
        }
    }

    // Last calculation as we won't have a newline at the start if our input;
    checkMax(current, localMax);

    int result = 0;

    for (size_t i = 0; i < TOP_SIZE; i++)
    {
        result += localMax[i];
    }   


    return result;
}

int main()
{
    stdio_init_all();

    sleep_ms(5000);

    printf("Beginning Execution\n");


    printf("---- Day 1 part 1 Test ----\n");
    printf("Result: %d\n", partOne(TEST_INPUT));
    printf("---- Day 1 part 1 ----\n");
    printf("Result: %d\n", partOne(ACTUAL_INPUT));
    printf("---- Day 1 part 2 Test ----\n");
    printf("Result: %d\n", partTwo(TEST_INPUT));
    printf("---- Day 1 part 2 ----\n");
    printf("Result: %d\n", partTwo(ACTUAL_INPUT));
}