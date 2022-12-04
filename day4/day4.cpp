
#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "day4.h"
#include <cstring>

/** 
 * State of
*/
enum ParserState {
    END_OF_LINE,
    VALUE_SWITCH,
    BOUNDARY_SWITCH,
    READING,
    IDLE
};

enum ValueState {
    FIRST_UPPER,
    FIRST_LOWER,
    SECOND_UPPER,
    SECOND_LOWER,
    VALUE_SIZE
};

/**
 * @brief Calculates whether either of the lines fully encapsulates the other.
 * This is done by checking the two secenarios where a line fits within another.
 * It could be done in less checks with some caching of comparisons, but the performance
 * increase would be negligible, and the readability would suffer greatly.
 * 
 * @param firstLower 
 * @param firstUpper 
 * @param secondLower 
 * @param secondUpper 
 * @return true 
 * @return false 
 */
inline bool calculateFullOverlap(int firstLower, int firstUpper, int secondLower, int secondUpper)
{
    return (
        (firstLower <= secondLower && firstUpper >= secondUpper) ||
        (secondLower <= firstLower && secondUpper >= firstUpper)
    );
}

/**
 * @brief Calculates whether either of the lines partially encapsulate the other
 * This is done by checking the two secenarios where either the start of either point is
 * inside the other. We can get away with only checking the points of a single line as
 * if the start of one line overlaps we can derive that the end of the other line also overlaps.
 * 
 * @param firstLower 
 * @param firstUpper 
 * @param secondLower 
 * @param secondUpper 
 * @return true 
 * @return false 
 */
inline bool calculatePartialOverlap(int firstLower, int firstUpper, int secondLower, int secondUpper)
{
    return (
        (firstLower >= secondLower && firstLower <= secondUpper) ||
        (secondLower >= firstLower && secondLower <= firstUpper)
    );
}

/**
 * @brief Part 1 of Day 4
 * 
 * This problem consists of inputs which denotes pairs of lines. We're required to read each pair and 
 * calculate whether either line fully encapsulates the other.
 * 
 * This day is fairly short as the problem was fairly simple and I could reuse a lot of the code from day 1.
 * Potentially I could look at better ways for handling the state management, however I do like how it makes the code very readable.
 * I don't like how often I need to reset the section and radix on different lines, but I'm not to worried with the implementation
 * 
 * @return int
 */
int partOne(const char input[])
{
    int radix = 1;
    size_t inputLength = strlen(input) - 1;

    ParserState parserState = IDLE;
    ValueState valueState = FIRST_UPPER;

    int count, firstUpper, firstLower,
        secondUpper, secondLower, section;
    

    count = section = 0;

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
        case '\n': 
            parserState = END_OF_LINE;
            break;
        case ',':
            parserState = VALUE_SWITCH;
            break;
        case '-':
            parserState = BOUNDARY_SWITCH;
            break;
        default:
            parserState = READING;
            break;
        }

        /*
        The task to execute based off our state
        */
        switch (parserState)
        {
        case END_OF_LINE: // Process Complete Input and reset for new line
            secondLower = section;
            radix = 1;
            section = 0;
            valueState = FIRST_UPPER;
            count += calculateFullOverlap(firstLower, firstUpper, secondLower, secondUpper);
            break;
        case VALUE_SWITCH: // Switch from First and Second values
            firstLower = section;
            radix = 1;
            section = 0;
            valueState = SECOND_UPPER;
            break;
        case BOUNDARY_SWITCH: // Switch from reading the Upper to Lower Value
            if (valueState == FIRST_UPPER)
            {
                valueState = FIRST_LOWER;
                firstUpper = section;
            }
            else
            {
                valueState = SECOND_LOWER;
                secondUpper = section;
            }
            radix = 1;
            section = 0;
            break;
        case READING: // Read value in
            section += (value - 48) * radix;
            radix *= 10;
            break;
        default:
            break;
        }
    }

    // Process Last Field
    secondLower = section;
    count += calculateFullOverlap(firstLower, firstUpper, secondLower, secondUpper);

    return count;
}

/**
 * @brief Part 2 of Day 4
 * 
 * This problem consists of inputs which denotes pairs of lines. We're required to read each pair and 
 * calculate whether either line partially encapsulates the other.
 * 
 * The same as part 1, however a new comparison function is utlized to only check partial overlaps.
 * 
 * @return int
 */
int partTwo(const char input[])
{
    int radix = 1;
    size_t inputLength = strlen(input) - 1;

    ParserState parserState = IDLE;
    ValueState valueState = FIRST_UPPER;

    int count, firstUpper, firstLower,
        secondUpper, secondLower, section;
    

    count = section = 0;

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
        case '\n': 
            parserState = END_OF_LINE;
            break;
        case ',':
            parserState = VALUE_SWITCH;
            break;
        case '-':
            parserState = BOUNDARY_SWITCH;
            break;
        default:
            parserState = READING;
            break;
        }

        /*
        The task to execute based off our state
        */
        switch (parserState)
        {
        case END_OF_LINE: // Process Complete Input and reset for new line
            secondLower = section;
            radix = 1;
            section = 0;
            valueState = FIRST_UPPER;
            count += calculatePartialOverlap(firstLower, firstUpper, secondLower, secondUpper);
            break;
        case VALUE_SWITCH: // Switch from First and Second values
            firstLower = section;
            radix = 1;
            section = 0;
            valueState = SECOND_UPPER;
            break;
        case BOUNDARY_SWITCH: // Switch from reading the Upper to Lower Value
            if (valueState == FIRST_UPPER)
            {
                valueState = FIRST_LOWER;
                firstUpper = section;
            }
            else
            {
                valueState = SECOND_LOWER;
                secondUpper = section;
            }
            radix = 1;
            section = 0;
            break;
        case READING: // Read value in
            section += (value - 48) * radix;
            radix *= 10;
            break;
        default:
            break;
        }
    }

    // Process Last Field
    secondLower = section;
    count += calculatePartialOverlap(firstLower, firstUpper, secondLower, secondUpper);

    return count;
}

int main()
{
    stdio_init_all();

    sleep_ms(2000);

    printf("---- Day 4 part 1 Test ----\n");
    printf("Result: %d\n", partOne(TEST_INPUT));
    printf("---- Day 4 part 1 ----\n");
    printf("Result: %d\n", partOne(ACTUAL_INPUT));
    printf("---- Day 4 part 2 Test ----\n");
    printf("Result: %d\n", partTwo(TEST_INPUT));
    printf("---- Day 4 part 2 ----\n");
    printf("Result: %d\n", partTwo(ACTUAL_INPUT));
}