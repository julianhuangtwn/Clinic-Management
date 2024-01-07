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

// SAFE-GUARD: 
// It is good practice to apply safe-guards to header files
// Safe-guard's ensures only 1 copy of the header file is used in the project build
// The macro name should be mirroring the file name with _ for spaces, dots, etc.

// !!! DO NOT DELETE THE BELOW 2 LINES !!!
#ifndef CORE_H
#define CORE_H

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// 
// Copy your work done from Milestone #2 (core.h) into this file
// 
// NOTE:
// - Organize your functions into the below categories
// - Make sure the core.c file also lists these functions in the same order!
// - Be sure to provide a BRIEF comment for each function prototype
// - The comment should also be copied to the core.c function definition
//
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


//////////////////////////////////////
// USER INTERFACE FUNCTIONS
//////////////////////////////////////

// Clear the standard input buffer
void clearInputBuffer(void);

// Wait for user to input the "enter" key to continue
void suspend(void);


//////////////////////////////////////
// USER INPUT FUNCTIONS
//////////////////////////////////////

//Checks if user input is a whole number, else prompts re-entry
int inputInt(void);

//Checks if user input is a positive integer number, else prompts re-entry
int inputIntPositive(void);

//Checks if user input is an integer number within the min and max range, else prompts re-entry
int inputIntRange(int min, int max);

//Checks if user input is a character that is in the charList, case sensitive. Else prompts re-entry
char inputCharOption(const char* charList);

//Checks if user input is a positive integer number, else prompts re-entry
void inputCString(char* string, int minCharRange, int maxCharRange);

//Prompts user to input year, month, day, and returns valye via argument
void inputDate(int* year, int* month, int* day);


//////////////////////////////////////
// UTILITY FUNCTIONS
//////////////////////////////////////

//Checks if the passed in parameter phoneNumber is exactly 10 digits integer long. Returns the same phoneNumber, else returns a blank
void displayFormattedPhone(const char* phoneNumber);

//Returns the number of days in the given year and month. Function will account for leap years and return 29 if it's also February 
void findMonthDays(int year, int month, int* monthDays);

// !!! DO NOT DELETE THE BELOW LINE !!!
#endif // !CORE_H
