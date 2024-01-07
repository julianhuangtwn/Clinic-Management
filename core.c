/*/////////////////////////////////////////////////////////////////////////
                        Assignment 1 - Milestone 3
Full Name  : Julian Huang
Student ID#: 134161231
Email      : jhuang279@myseneca.ca
Section    : NEE

Authenticity Declaration:
I declare this submission is the result of my own work and has not been
shared with any other student or 3rd party content provider. This submitted
piece of work is entirely of my own creation.
/////////////////////////////////////////////////////////////////////////*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "core.h"
#include <string.h>
#define MAX_SIZE 100
#define CURR_YEAR 2023

//////////////////////////////////////
// USER INTERFACE FUNCTIONS
//////////////////////////////////////

// Clear the standard input buffer
void clearInputBuffer(void)
{
    // Discard all remaining char's from the standard input buffer:
    while (getchar() != '\n')
    {
        ; // do nothing!
    }
}

// Wait for user to input the "enter" key to continue
void suspend(void)
{
    printf("<ENTER> to continue...");
    clearInputBuffer();
    putchar('\n');
}


//////////////////////////////////////
// USER INPUT FUNCTIONS
//////////////////////////////////////

//
// Copy your work done from Milestone #2 (core.c) into this file
// - Organize your functions in the same order as they are listed in the core.h file
//

//Checks if user input is a whole number, else prompts re-entry
int inputInt(void)
{
    char newLine = 'X';
    int wholeNum;

    while (newLine != '\n')
    {
        scanf("%d%c", &wholeNum, &newLine);
        if (newLine != '\n')
        {
            clearInputBuffer(); //When incorrect type is inputted, such as char instead of int, input is left in the buffer and will infinitely loop into the next scanf, hence need to clear the buffer when input is invalid
            printf("Error! Input a whole number: ");
        }
    }
    return wholeNum;
}

//Checks if user input is a positive integer number, else prompts re-entry
int inputIntPositive(void)
{
    char newLine = 'x';
    int posNum;
    while (newLine != '\n' || posNum < 0)
    {
        scanf("%d%c", &posNum, &newLine);

        if (newLine != '\n')
        {
            clearInputBuffer();
            printf("Error! Input a whole number: ");
        }
        else if (posNum < 0)
        {
            printf("ERROR! Value must be > 0: ");
            newLine = 'x';
        }
    }
    return posNum;
}

//Checks if user input is an integer number within the min and max range, else prompts re-entry
int inputIntRange(int min, int max)
{
    char newLine = 'x';
    int rangeNum;
    do
    {
        scanf("%d%c", &rangeNum, &newLine); 
        if (newLine != '\n')
        {
            printf("Error! Input a whole number: ");
            clearInputBuffer();
        }
        else if (rangeNum < min || rangeNum > max)
        {
            printf("ERROR! Value must be between %d and %d inclusive: ", min, max);
            newLine = 'x';
        }
    } while (rangeNum < min || rangeNum > max);
    return rangeNum;
}

//Checks if user input is a character that is in the charList, case sensitive. Else prompts re-entry
char inputCharOption(const char* charList)
{
    char charInList, newLine = 'x';
    int finInput = 0;

    do
    {
        scanf(" %c%c", &charInList, &newLine); //Accounts if user puts more than one char together. Since %c only reads one character, you need to immediately check next char if it's \n or not to make sure user input is correct

        if (strchr(charList, charInList) == NULL || newLine != '\n')
        {
            printf("ERROR: Character must be one of [%s]: ", charList);
            clearInputBuffer();
            newLine = 'x';
        }
        else
        {
            finInput = 1;
        }
    } while (finInput == 0);
    return *strchr(charList, charInList);
}

//Checks if user input is a positive integer number, else prompts re-entry
void inputCString(char* string, const int minCharRange, const int maxCharRange)
{
    int strLength = 0, finInput = 0;
    char stringInRange[MAX_SIZE] = { '\0' };

    while (finInput == 0)
    {
        scanf(" %[^\n]", stringInRange); //This means scan until a \n character is met, usually it's when any whitespace is met, this is to account for input with spaces. * with a conversion specifier means will keep reading and not store the info
        strLength = strlen(stringInRange); //Upgraded to count length of string
        if (minCharRange == maxCharRange && strLength != maxCharRange)
        {
            printf("ERROR: String length must be exactly %d chars: ", maxCharRange);
        }
        else if (strLength > maxCharRange)
        {
            printf("ERROR: String length must be no more than %d chars: ", maxCharRange);
        }
        else if (strLength < minCharRange)
        {
            printf("ERROR: String length must be between %d and %d chars: ", minCharRange, maxCharRange);
        }
        else
        {
            finInput = 1;
        }
        strLength = 0;
    }
    strcpy(string, stringInRange); //Upgraded to use strcpy to change string to user input
}

//Prompts user to input year, month, day, and returns valye via argument
void inputDate(int* year, int* month, int* day)
{
    int monthDays;

    printf("Year        : ");
    *year = inputIntRange(CURR_YEAR, CURR_YEAR + 50);

    printf("Month (1-12): ");
    *month = inputIntRange(1, 12);
    findMonthDays(*year, *month, &monthDays);
    printf("Day (1-%d)  : ", monthDays);
    *day = inputIntRange(1, monthDays);
}


//////////////////////////////////////
// UTILITY FUNCTIONS
//////////////////////////////////////

//Checks if the passed in parameter phoneNumber is exactly 10 digits integer long. Returns the same phoneNumber, else returns a blank
void displayFormattedPhone(const char* phoneNumber)
{
    int index, wrongFormat = 0, length = 0;

    length = strlen(phoneNumber); //Upgraded to strlen to count length of user input
    if (phoneNumber != NULL)
    {
        for (index = 0; phoneNumber[index] != '\0'; index++)
        {
            if (phoneNumber[index] < '0' || phoneNumber[index] > '9') //Directly compares the char ASCII value, so that if the argument passed in includes character it will also detect it
            {
                wrongFormat = 1;
            }
        }
    }

    if (wrongFormat == 1 || length != 10)
    {
        printf("(___)___-____");
    }
    else
    {
        printf("(");
        for (index = 0; index <= 2; index++)
        {
            printf("%c", phoneNumber[index]);
        }
        printf(")");
        for (; index <= 5; index++) //Since index isn't reset, continues from the previous index of 3, hence 4th ~ 6th digit of the number
        {
            printf("%c", phoneNumber[index]);
        }
        printf("-");
        for (; index <= 9; index++)
        {
            printf("%c", phoneNumber[index]);
        }
    }
}

//Returns the number of days in the given year and month. Function will account for leap years and return 29 if it's also February 
void findMonthDays(int year, int month, int* monthDays)
{
    int daysOfMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if ((year - 2000) % 4 == 0 && month == 2)
    {
        *monthDays = 29;
    }
    else
    {
        *monthDays = daysOfMonth[month - 1];
    }
}