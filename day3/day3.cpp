
#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "day3.h"
#include <cstring>

/*
    Notes for this solution:
    I have some ineffeciencies that I could tidy up to speed up this code, however I feel I would lose
    readability for performance. An example is that I could unroll my loops for calculating the values based off
    the bit positions of the hashtable. I could use a switch statement with hardcoded values, however that would result in a switch statement
    that would be 52 statements long
    I could also break out of a few of my calculations early when a duplicate is found. As I only need to find a single duplicate for each
    calculation, and my algorithms would actually calculate every duplicate.

    I also looked into the potential to using asm to run assembly to take adventage of some specific ARM operators. Assembly can very easily make
    effeciency worse as the compiler is generally smarter than us. However I do want to try doing the following:
    I considered using assembly to calculate my bit values as I the ARM instruction set has some useful instruction that I thought could possibly
    speed up my bit value calculations. ARM uses the ASR intruction to have bit shift right, and you can add an 's' suffix (ASRS) so it stores condition flags
    based on the operation. I could use the carry condition to derive whether I dropped a 0 or 1 using the bit shift. ARM have special conditions that can be
    added to operations that either perform the operation or perform a no-op based off the condition. For example the 'ADD' operator can be suffixed with the carry
    flag condition 'CS' to make 'ADDCS' which would add values if the carry flag is set. This might be a fun experiment to do when I get time, however I will need
    to spend the time to brush up on my assembly, the gcc assembly parser and also make sure it compiles correctly.
*/

/**
 * @brief A data struct for storing the bit flags for upper and lowercase characters
 */
typedef struct
{
    uint32_t upper;
    uint32_t lower;
} CharacterBits;

/**
 * @brief Converts a string buffer into individual characters into bits
 * Uses two 32bit integers to store both the upper and lowercase characters
 *
 * @param start
 * @param end
 * @return CharacterBits
 */
inline CharacterBits calculateBits(const char *start, const char *end)
{
    CharacterBits result{0, 0};
    while (start < end)
    {
        char value = *(start++);
        result.upper |= 1 << ((value > 64 && value < 91) * (value - 64));
        result.lower |= 1 << ((value > 96 && value < 123) * (value - 96));
    }
    return result;
}

/**
 * @brief Sums all the enabled bits in a 32 bit integer
 * The sum will add their position in the integer
 * Supply a base to offset the sum
 *
 * @param duplicates
 * @param base
 * @return int
 */
inline int sumBitPositions(uint32_t duplicates, int base = 1)
{
    int result = 0;
    while (duplicates > 0)
    {
        result += (duplicates & 0x1) * base++;
        duplicates >>= 1;
    }
    return result;
}

/**
 * @brief Calculates all the duplicates characters within a section of memory
 * Each duplicate will will be assigned a value based off their character.
 * The value be represented by lowercase a-z being assigned 1-26, and uppercase A-Z being assigned 27-52
 * @param start
 * @param end
 * @return int
 */
inline int calculateRucksuckDuplicates(const char *start, const char *end)
{

    const char *mid;
    CharacterBits first, second;
    mid = ((end - start) >> 1) + start;
    first = calculateBits(start, mid);
    second = calculateBits(mid, end);

    return sumBitPositions((first.lower & second.lower) >> 1) +
           sumBitPositions((first.upper & second.upper) >> 1, 27);
}

/**
 * @brief Calculates the single similar character within the next 3 lines of a section of memory, 
 * and will return a value representing that character.
 * The value be represented by lowercase a-z being assigned 1-26, and uppercase A-Z being assigned 27-52
 * @param start 
 * @return int 
 */
inline int calculateGroupBadge(const char **start)
{
    const char *startOne, *endOne, *startTwo, *endTwo, *startThree, *endThree;

    startOne = *start;
    endOne = strchr(startOne, '\n');

    startTwo = endOne + 1;
    endTwo = strchr(startTwo, '\n');

    startThree = endTwo + 1;
    endThree = strchr(startThree, '\n');

    if (endThree == NULL)
    {
        endThree = strchr(startThree, '\0');
    }

    *start = endThree + 1;

    CharacterBits first, second, third;

    first = calculateBits(startOne, endOne);
    second = calculateBits(startTwo, endTwo);
    third = calculateBits(startThree, endThree);

    return sumBitPositions((first.lower & second.lower & third.lower) >> 1) +
           sumBitPositions((first.upper & second.upper & third.upper) >> 1, 27);
}

/**
 * @brief Part 1 of Day 3
 * The problem consists of of an input of strings seperated by newlines
 * Each string represents an Elfs rucksack with each character in the string representing an individual item
 * 
 * The problem for part 1 requires us to split the rucksack into a lower and upper half, and calculate the single
 * duplicate item in each half. A value is then derived from this character and the problem result will be the sum of 
 * all the derived value of the duplicates. The derived value is described by the following:
 * The value be represented by lowercase a-z being assigned 1-26, and uppercase A-Z being assigned 27-52
 * 
 * To solve this problem I used a rough (emphasis on rough) hashtable to quickly calculate duplicates.
 * I "hash" each item by calculating a bit value based off their ascii value. Where both a-z and A-Z will be represented by
 * a 0-26 value, and then their value will be set as a single bit flag in a 32 bit integer. I use two 32 bit integers to 
 * store a "hashtable" of both the upper and lower case characters separately. I can then perform an & operator to compare 
 * the "hastable" of each section of rucksack, which will give me a resulting "hashtable" of all duplicates.
 * I can then calculate the duplicate character by finding the position of the flagged bit, and returning a value based off its position

 * @return int
 */
int partOne(const char input[])
{
    int result = 0;
    const char *start = input, *end;

    while ((end = strchr(start, '\n')) != NULL)
    {
        result += calculateRucksuckDuplicates(start, end);
        start = end + 1;
    }

    result += calculateRucksuckDuplicates(start, strchr(start, '\0'));

    return result;
}

/**
 * @brief Part 2 of Day 3
 * The problem consists of of an input of strings seperated by newlines
 * Each string represents an Elfs rucksack with each character in the string representing an individual item
 * 
 * The problem for part 2 requires us to group 3 rucksacks together and calculate the single duplicate item
 * across all 3. A value is then derived from this character and the problem result will be the sum of 
 * all the derived value of duplicate items in each group of 3. The derived value is described by the following:
 * The value be represented by lowercase a-z being assigned 1-26, and uppercase A-Z being assigned 27-52
 * 
 * To solve Part 2 I employed the same "hashing" algorithm as Part 1, but instead of comparing the each section of a single
 * rucksack I compare 3 entire rucksacks. The solution follows the same algorithms for "hashing" and calculating the resulting value
 * of the duplicate item.
 * @return int
 */
int partTwo(const char input[])
{
    int result = 0;
    const char *start = input, *end;

    while ((end = strchr(start, '\n')) != NULL)
    {
        result += calculateGroupBadge(&start);
    }
    return result;
}

int main()
{
    stdio_init_all();

    sleep_ms(2000);

    printf("---- Day 3 part 1 Test ----\n");
    printf("Result: %d\n", partOne(TEST_INPUT));
    printf("---- Day 3 part 1 ----\n");
    printf("Result: %d\n", partOne(ACTUAL_INPUT));
    printf("---- Day 3 part 2 Test ----\n");
    printf("Result: %d\n", partTwo(TEST_INPUT));
    printf("---- Day 3 part 2 ----\n");
    printf("Result: %d\n", partTwo(ACTUAL_INPUT));
}