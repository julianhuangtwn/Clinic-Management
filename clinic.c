#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include "core.h"
#include "clinic.h"



//////////////////////////////////////
// DISPLAY FUNCTIONS
//////////////////////////////////////

// Display's the patient table header (table format)
void displayPatientTableHeader(void)
{
    printf("Pat.# Name            Phone#\n"
           "----- --------------- --------------------\n");
}

// Displays a single patient record in FMT_FORM | FMT_TABLE format
void displayPatientData(const struct Patient* patient, int format)
{
    if (format == FMT_FORM)
    {
        printf("Name  : %s\n"
               "Number: %05d\n"
               "Phone : ", patient->name, patient->patientNumber);
        displayFormattedPhone(patient->phone.number);
        printf(" (%s)\n", patient->phone.description);
    }
    else
    {
        printf("%05d %-15s ", patient->patientNumber,
               patient->name);
        displayFormattedPhone(patient->phone.number);
        printf(" (%s)\n", patient->phone.description);
    }
}

// Display's appointment schedule headers (date-specific or all records)
void displayScheduleTableHeader(const struct Date* date, int isAllRecords)
{
    printf("Clinic Appointments for the Date: ");

    if (isAllRecords)
    {
        printf("<ALL>\n\n");
        printf("Date       Time  Pat.# Name            Phone#\n"
               "---------- ----- ----- --------------- --------------------\n");
    }
    else
    {
        printf("%04d-%02d-%02d\n\n", date->year, date->month, date->day);
        printf("Time  Pat.# Name            Phone#\n"
               "----- ----- --------------- --------------------\n");
    }
}

// Display a single appointment record with patient info in tabular format
void displayScheduleData(const struct Patient* patient,
                         const struct Appointment* appoint,
                         int includeDateField)
{
    if (includeDateField)
    {
        printf("%04d-%02d-%02d ", appoint->date.year, appoint->date.month,
               appoint->date.day);
    }
    printf("%02d:%02d %05d %-15s ", appoint->time.hour, appoint->time.min,
           patient->patientNumber, patient->name);

    displayFormattedPhone(patient->phone.number);

    printf(" (%s)\n", patient->phone.description);
}


//////////////////////////////////////
// MENU & ITEM SELECTION FUNCTIONS
//////////////////////////////////////

// Displays menu with choice
void menuMain(struct ClinicData* data)
{
    int selection;

    do {
        printf("Veterinary Clinic System\n"
               "=========================\n"
               "1) PATIENT     Management\n"
               "2) APPOINTMENT Management\n"
               "-------------------------\n"
               "0) Exit System\n"
               "-------------------------\n"
               "Selection: ");
        selection = inputIntRange(0, 2);
        putchar('\n');
        switch (selection)
        {
        case 0:
            printf("Are you sure you want to exit? (y|n): ");
            selection = !(inputCharOption("yn") == 'y');
            putchar('\n');
            if (!selection)
            {
                printf("Exiting system... Goodbye.\n\n");
            }
            break;
        case 1:
            menuPatient(data->patients, data->maxPatient);
            break;
        case 2:
            menuAppointment(data);
            break;
        }
    } while (selection);
}

// Menu for patient management
void menuPatient(struct Patient patient[], int max)
{
    int selection;

    do {
        printf("Patient Management\n"
               "=========================\n"
               "1) VIEW   Patient Data\n"
               "2) SEARCH Patients\n"
               "3) ADD    Patient\n"
               "4) EDIT   Patient\n"
               "5) REMOVE Patient\n"
               "-------------------------\n"
               "0) Previous menu\n"
               "-------------------------\n"
               "Selection: ");
        selection = inputIntRange(0, 5);
        putchar('\n');
        switch (selection)
        {
        case 1:
            displayAllPatients(patient, max, FMT_TABLE);
            suspend();
            break;
        case 2:
            searchPatientData(patient, max);
            break;
        case 3:
            addPatient(patient, max);
            suspend();
            break;
        case 4:
            editPatient(patient, max);
            break;
        case 5:
            removePatient(patient, max);
            suspend();
            break;
        }
    } while (selection);
}

// Menu for patient edit
void menuPatientEdit(struct Patient* patient)
{
    int selection;

    do {
        printf("Edit Patient (%05d)\n"
               "=========================\n"
               "1) NAME : %s\n"
               "2) PHONE: ", patient->patientNumber, patient->name);
        
        displayFormattedPhone(patient->phone.number);
        
        printf("\n"
               "-------------------------\n"
               "0) Previous menu\n"
               "-------------------------\n"
               "Selection: ");
        selection = inputIntRange(0, 2);
        putchar('\n');

        if (selection == 1)
        {
            printf("Name  : ");
            inputCString(patient->name, 1, NAME_LEN);
            putchar('\n');
            printf("Patient record updated!\n\n");
        }
        else if (selection == 2)
        {
            inputPhoneData(&patient->phone);
            printf("Patient record updated!\n\n");
        }

    } while (selection);
}

// Menu for appointment management
void menuAppointment(struct ClinicData* data)
{
    int selection;

    do {
        printf("Appointment Management\n"
               "==============================\n"
               "1) VIEW   ALL Appointments\n"
               "2) VIEW   Appointments by DATE\n"
               "3) ADD    Appointment\n"
               "4) REMOVE Appointment\n"
               "------------------------------\n"
               "0) Previous menu\n"
               "------------------------------\n"
               "Selection: ");
        selection = inputIntRange(0, 4);
        putchar('\n');
        switch (selection)
        {
        case 1:
            viewAllAppointments(data);  
            suspend();
            break;
        case 2:
            viewAppointmentSchedule(data); 
            suspend();
            break;
        case 3:
            addAppointment(data->appointments, data->maxAppointments, data->patients, data->maxPatient); 
            suspend();
            break;
        case 4:
            removeAppointment(data->appointments, data->maxAppointments, data->patients, data->maxPatient);  
            suspend();
            break;
        }
    } while (selection);
}


// Display's all patient data in the FMT_FORM | FMT_TABLE format
void displayAllPatients(const struct Patient patient[], int max, int fmt)
{
    int patients, dataFound = 0;

    displayPatientTableHeader();

    for (patients = 0; patients < max; patients++)
    {
        if (patient[patients].patientNumber != 0)
        {
            displayPatientData(&patient[patients], FMT_TABLE);
            dataFound = 1;
        }
    }
    if (dataFound != 1)
    {
        printf("\n*** No Record Found ***\n");
    }
    printf("\n");
}


// Search for a patient record based on patient number or phone number
void searchPatientData(const struct Patient patient[], int max)
{
    int selection, prevMenuSelected = 0;

    while (prevMenuSelected == 0)
    {
        printf("Search Options\n"
            "==========================\n"
            "1) By patient number\n"
            "2) By phone number\n"
            "..........................\n"
            "0) Previous menu\n"
            "..........................\n"
            "Selection: ");
        selection = inputIntRange(0, 2);
        printf("\n");
        switch (selection)
        {
        case 1:
            searchPatientByPatientNumber(patient, max);
            suspend();
            break;

        case 2:
            searchPatientByPhoneNumber(patient, max);
            suspend();
            break;

        case 0:
            prevMenuSelected = 1;
            break;

        default:
            printf("*** No records found ***\n\n");
        }
    }
}


// Add a new patient record to the patient array
void addPatient(struct Patient patient[], int max)
{
    int patients, listFull = 1, emptyIndex = 0;

    for (patients = 0; patients < max; patients++, emptyIndex++)
    {
        if (patient[patients].patientNumber == 0) //If found a space where patientNumber is 0, that means list isn't full yet.
        {
            listFull = 0;
            patients = max; // Force loop to break early
        }
    }

    if (listFull == 1)
    {
        printf("ERROR: Patient listing is FULL!\n\n");
    }
    else
    {
        patient[emptyIndex - 1].patientNumber = nextPatientNumber(patient, max) + 1; //Since adding emptyIndex in for loop makes it go up by one extra, the actual index is one less
        inputPatient(&patient[emptyIndex - 1]);
        printf("*** New patient record added ***\n\n");
    }
}


// Edit a patient record from the patient array
void editPatient(struct Patient patient[], int max)
{
    int patientNum, searchResultIndex;

    printf("Enter the patient number: ");
    patientNum = inputIntPositive();
    printf("\n");
    searchResultIndex = findPatientIndexByPatientNum(patientNum, patient, max);

    if (searchResultIndex != -1)
    {
        menuPatientEdit(&patient[searchResultIndex]);
    }
    else
    {
        printf("\nERROR: Patient record not found!\n\n");
    }
}


// Remove a patient record from the patient array
void removePatient(struct Patient patient[], int max)
{
    int patientNum, searchResultIndex, index = 0;
    char confirmation, confirmationChoice[3] = { 'y', 'n' };

    printf("Enter the patient number: ");
    patientNum = inputIntPositive();
    printf("\n");

    searchResultIndex = findPatientIndexByPatientNum(patientNum, patient, max);
    if (searchResultIndex != -1)
    {
        displayPatientData(&patient[searchResultIndex], FMT_FORM);
        printf("\nAre you sure you want to remove this patient record? (y/n): ");
        confirmation = inputCharOption(confirmationChoice); //User is only allowed to input y or n, hence the predefined char array of confirmationChoice
        if (confirmation == 'y') //Set everything to 0 or \0
        {
            patient[searchResultIndex].patientNumber = 0;
            for (index = 0; index < NAME_LEN; index++)
            {
                patient[searchResultIndex].name[index] = '\0';
            }
            for (index = 0; index < PHONE_DESC_LEN; index++)
            {
                patient[searchResultIndex].phone.description[index] = '\0';
            }
            for (index = 0; index < PHONE_LEN; index++)
            {
                patient[searchResultIndex].phone.number[index] = '\0';
            }
            printf("Patient record has been removed!\n\n");
        }
        else
        {
            printf("Operation aborted.\n\n");
        }
         //\n character is left over from previous input, this will cause suspend() to take the character as an input and skips an empty line
    }
    else
    {
        printf("ERROR: Patient record not found!\n\n");
    }
}

// View ALL scheduled appointments
void viewAllAppointments(struct ClinicData* data)
{
    int numOfAppoints = countAppoints(data->appointments), appoint = 0, matchedPatient = 0;

    bubbleSortAppoint(data->appointments, numOfAppoints);
    displayScheduleTableHeader(NULL, 1);
    for (appoint = 0; appoint < numOfAppoints; appoint++)
    {
        matchedPatient = findPatientIndexByPatientNum(data->appointments[appoint].patientNumber, data->patients, data->maxPatient);
        displayScheduleData(&data->patients[matchedPatient], &data->appointments[appoint], 1);
    }
    printf("\n");
}


// View appointment schedule for the user input date
void viewAppointmentSchedule(const struct ClinicData* data)
{
    int year, month, day, appoint, matchedPatient, headerDisplayed = 0, recordFound = 0, 
        numOfAppoints = countAppoints(data->appointments);

    bubbleSortAppoint(data->appointments, numOfAppoints);
    inputDate(&year, &month, &day);
    
    for (appoint = 0; appoint < data->maxAppointments && data->appointments[appoint].patientNumber != 0; appoint++)
    {
        if (data->appointments[appoint].date.year == year && data->appointments[appoint].date.month == month
            && data->appointments[appoint].date.day == day)
        {
            recordFound = 1;
            matchedPatient = findPatientIndexByPatientNum(data->appointments[appoint].patientNumber, data->patients, data->maxPatient);
            if (headerDisplayed == 0)
            {
                printf("\n");
                displayScheduleTableHeader(&data->appointments[appoint].date, 0);
                headerDisplayed = 1;
            }
            displayScheduleData(&data->patients[matchedPatient], &data->appointments[appoint], 0);
        }
    }
    if (recordFound != 1)
    {
        printf("\n*** No records found ***\n");
    }
    printf("\n");
}


// Add an appointment record to the appointment array
void addAppointment(struct Appointment appoint[], int maxAppointments,
    struct Patient patients[], int maxPatient)
{
    int emptySpot = 0, emptySpotIndex, patientNum, patientFound = 0, 
        addYear, addMonth, addDay, addHour, addMin,
        noDuplicate = 0, duplicateFound = 0, appAdded = 0, app;

    for (app = 0; app < maxAppointments; app++) //See if there is an empty spot in the appointment array. Condition for the actual appointment addition to proceed
    {
        if (appoint[app].patientNumber == 0)
        {
            emptySpot = 1;
            emptySpotIndex = app;
            app = maxAppointments;
        }
    }
    
    if (emptySpot == 1)
    {
        printf("Patient Number: ");
        patientNum = inputIntPositive();
        patientFound = patientExists(patients, patientNum, maxPatient);

        if (patientFound == 1)
        {
            while (appAdded == 0)
            {
                if (noDuplicate == 0)
                {
                    inputDate(&addYear, &addMonth, &addDay);
                }
                printf("Hour (0-23)  : ");
                addHour = inputIntRange(0, 23);
                printf("Minute (0-59): ");
                addMin = inputIntRange(0, 59);

                if (noDuplicate == 0)
                {
                    for (app = 0; app < maxAppointments && appoint[app].patientNumber != 0; app++) //If there is an exact duplicate appoint timing, appointment request is rejected
                    {
                        if (addYear == appoint[app].date.year && addMonth == appoint[app].date.month
                            && addDay == appoint[app].date.day && addHour == appoint[app].time.hour && addMin == appoint[app].time.min)
                        {
                            duplicateFound = 1;
                        }
                    }
                }
                
                if (duplicateFound != 1)
                {
                    noDuplicate = 1;
                    if (addHour < OPEN_HOUR || addHour > CLOSE_HOUR || addMin < OPEN_MIN || addMin > CLOSE_MIN || addMin % APP_INTERVAL != 0)
                    {
                        printf("ERROR: Time must be between %02d:%02d and %02d:%02d in %02d minute intervals.\n\n", OPEN_HOUR, OPEN_MIN, CLOSE_HOUR, CLOSE_MIN, APP_INTERVAL);
                    }

                    else
                    {
                        appoint[emptySpotIndex].patientNumber = patientNum; //Adds the requested appointment to the list
                        appoint[emptySpotIndex].date.year = addYear;
                        appoint[emptySpotIndex].date.month = addMonth;
                        appoint[emptySpotIndex].date.day = addDay;
                        appoint[emptySpotIndex].time.hour = addHour;
                        appoint[emptySpotIndex].time.min = addMin;
                        printf("\n*** Appointment scheduled! ***\n\n");
                        appAdded = 1;
                    }
                }
                else
                {
                    printf("\nERROR: Appointment timeslot is not available!\n\n");
                    duplicateFound = 0;
                    noDuplicate = 0;
                }
            }
        } 
        else
        {
            printf("\n*** No records found ***\n\n");
        }
    }
}


// Remove an appointment record from the appointment array
void removeAppointment(struct Appointment appoint[], int maxAppointments,
    struct Patient patients[], int maxPatient)
{
    int patientNum, patientFound = 0, year, month, day, app, matchingPatient, numOfAppoints;
    char choice, options[3] = { 'y', 'n' };
    struct Appointment temp;

    printf("Patient Number: ");
    patientNum = inputIntPositive();
    patientFound = patientExists(patients, patientNum, maxPatient);
    if (patientFound == 1)
    {
        inputDate(&year, &month, &day);
        for (app = 0; app < maxAppointments && appoint[app].patientNumber != 0; app++)
        {
            if (appoint[app].date.year == year && appoint[app].date.month == month
                && appoint[app].date.day == day && appoint[app].patientNumber == patientNum)
            {
                matchingPatient = findPatientIndexByPatientNum(patientNum, patients, maxPatient);
                printf("\n");
                displayPatientData(&patients[matchingPatient], FMT_FORM);
                printf("Are you sure you want to remove this appointment (y,n): ");
                choice = inputCharOption(options);
                if (choice == 'y')
                {
                    numOfAppoints = countAppoints(appoint);
                    appoint[app].patientNumber = 0; //Resets all data to 0 for the requested appointment
                    appoint[app].date.year = 0;
                    appoint[app].date.month = 0;
                    appoint[app].date.day = 0;
                    appoint[app].time.hour = 0;
                    appoint[app].time.min = 0;
                    temp = appoint[app]; //Switch the empty index to the end of the array so that when the list is sorted, the 0 data index won't be sorted to the front
                    appoint[app] = appoint[numOfAppoints - 1];
                    appoint[numOfAppoints - 1] = temp;
                    numOfAppoints--;
                    printf("\nAppointment record has been removed!\n\n");
                }
                else
                {
                    printf("Operation aborted.\n\n");;
                }
            }
        }
    }
    else
    {
        printf("ERROR: Patient record not found!\n\n");
    }
}


//////////////////////////////////////
// UTILITY FUNCTIONS
//////////////////////////////////////

// Search and display patient record by patient number (form)
void searchPatientByPatientNumber(const struct Patient patient[], int max)
{
    int patientNum, patients, dataFound = 0;

    printf("Search by patient number: ");
    patientNum = inputIntPositive();
    printf("\n");
    for (patients = 0; patients < max; patients++)
    {
        if (patient[patients].patientNumber == patientNum)
        {
            displayPatientData(&patient[patients], FMT_FORM);
            printf("\n");
            patients = max; //To break the for loop early
            dataFound = 1;
        }
    }

    if (dataFound != 1)
    {
        printf("*** No records found ***\n\n");
    }
}


// Search and display patient records by phone number (tabular)
void searchPatientByPhoneNumber(const struct Patient patient[], int max)
{
    int patients, dataFound = 0;
    char patientPhone[PHONE_LEN + 1] = { 0 };

    printf("Search by phone number: ");
    inputCString(patientPhone, PHONE_LEN, PHONE_LEN);
    printf("\n");
    displayPatientTableHeader();
    for (patients = 0; patients < max; patients++)
    {
        if (strcmp(patientPhone, patient[patients].phone.number) == 0)
        {
            displayPatientData(&patient[patients], FMT_TABLE);
            dataFound = 1;
        }
    }

    if (dataFound != 1)
    {
        printf("\n*** No records found ***\n");
    }
    printf("\n");
    clearInputBuffer(); //\n character is left over from previous input, this will cause suspend() to take the character as an input and skips an empty line
}


// Get the next highest patient number
int nextPatientNumber(const struct Patient patient[], int max)
{
    int patients, maxPatientNum = 0;

    for (patients = 0; patients < max; patients++)
    {
        if (patient[patients].patientNumber > maxPatientNum)
        {
            maxPatientNum = patient[patients].patientNumber; //Replace maxPatientNum with the currently found even bigger one
        }
    }
    return maxPatientNum;
}


// Find the patient array index by patient number (returns -1 if not found)
int findPatientIndexByPatientNum(int patientNumber, const struct Patient patient[], int max)
{
    int searchIndex = 0, patients, dataFound = 0;

    for (patients = 0; patients < max; patients++, searchIndex++)
    {
        if (patientNumber == patient[patients].patientNumber)
        {
            patients = max; //Break for loop early
            dataFound = 1;
        }
    }

    if (dataFound != 1)
    {
        searchIndex = 0; //Set to 0 so function can return -1 for not found result
    }

    return (searchIndex - 1); //-1 because for loop will have an extra ++ as it's iterated there, then breaks loop
}


// Counts and returns the current number of appointments that exists
int countAppoints(const struct Appointment appoint[])
{
    int numOfAppoints;

    for (numOfAppoints = 0; appoint[numOfAppoints].patientNumber != 0; numOfAppoints++);
    return numOfAppoints;
}


// Sorts the appointment list in ascending chronological order
//////////THIS NEEDS CLEANING UP///////////////
void bubbleSortAppoint(struct Appointment appoint[], int numOfAppoints)
{
    int currCompIndex = 0, swapNeeded = 0, exitSearch = 0;
    struct Appointment temp;

    for (; numOfAppoints >= 0; numOfAppoints--)
    {
        for (currCompIndex = 0; currCompIndex < numOfAppoints; currCompIndex++)
        {
            while (exitSearch == 0)
            {
                if (appoint[currCompIndex].date.year > appoint[currCompIndex + 1].date.year)
                {
                    swapNeeded = 1;
                    exitSearch = 1;
                }
                else if (appoint[currCompIndex].date.year == appoint[currCompIndex + 1].date.year)
                {
                    if (appoint[currCompIndex].date.month > appoint[currCompIndex + 1].date.month)
                    {
                        swapNeeded = 1;
                        exitSearch = 1;
                    }
                    else if (appoint[currCompIndex].date.month == appoint[currCompIndex + 1].date.month)
                    {
                        if (appoint[currCompIndex].date.day > appoint[currCompIndex + 1].date.day)
                        {
                            swapNeeded = 1;
                            exitSearch = 1;
                        }
                        else if (appoint[currCompIndex].date.day == appoint[currCompIndex + 1].date.day)
                        {
                            if (appoint[currCompIndex].time.hour > appoint[currCompIndex + 1].time.hour)
                            {
                                swapNeeded = 1;
                                exitSearch = 1;
                            }
                            else if (appoint[currCompIndex].time.hour == appoint[currCompIndex + 1].time.hour)
                            {
                                if (appoint[currCompIndex].time.min > appoint[currCompIndex + 1].time.min)
                                {
                                    swapNeeded = 1;
                                    exitSearch = 1;
                                }
                                else
                                {
                                    exitSearch = 1;
                                }
                            }
                            else
                            {
                                currCompIndex++;
                            }
                        }
                        else
                        {
                            currCompIndex++;
                        }
                    }
                    else
                    {
                        currCompIndex++;
                    }
                }
                else
                {
                    currCompIndex++;
                }
            }
            if (swapNeeded == 1 && appoint[currCompIndex + 1].patientNumber != 0)
            {
                temp = appoint[currCompIndex];
                appoint[currCompIndex] = appoint[currCompIndex + 1];
                appoint[currCompIndex + 1] = temp;
            }
            swapNeeded = 0;
            exitSearch = 0;
        }
    }
}


// Finds if patient exists in the current list of patients.
int patientExists(struct Patient patients[], int patientNum, int maxPatient)
{
    int patient, patientFound = 0;

    for (patient = 0; patient < maxPatient && patients[patient].patientNumber != 0; patient++)
    {
        if (patients[patient].patientNumber == patientNum)
        {
            patientFound = 1;
            patient = maxPatient;
        }
    }
    return patientFound;
}



//////////////////////////////////////
// USER INPUT FUNCTIONS
//////////////////////////////////////

// Get user input for a new patient record
void inputPatient(struct Patient* patient)
{
    printf("Patient Data Input\n"
        "------------------\n"
        "Number: %05d\n"
        "Name  : ", patient->patientNumber); //Since we're accessing a structure pointer we use ->
    inputCString(patient->name, 1, NAME_LEN);
    printf("\n");

    inputPhoneData(&patient->phone);
}


// Get user input for phone contact information
void inputPhoneData(struct Phone* phone)
{
    int selection, index, finInput = 0, strLength;

    printf("Phone Information\n");
    printf("-----------------\n");
    printf("How will the patient like to be contacted?\n");
    printf("1. Cell\n");
    printf("2. Home\n");
    printf("3. Work\n");
    printf("4. TBD\n");
    printf("Selection: ");

    selection = inputIntRange(1, 4);
    switch (selection)
    {
    case 1:
        strcpy(phone->description, "CELL"); //Using strcpy to replace the description string member with a new string
        break;

    case 2:
        strcpy(phone->description, "HOME");
        break;

    case 3:
        strcpy(phone->description, "WORK");
        break;

    case 4:
        strcpy(phone->description, "TBD");
        for (index = 0; index < PHONE_LEN; index++) //TBD doesn't need a phone number, so choosing this option would clear the record
        {
            phone->number[index] = '\0';
        }
        break;
    }

    if (selection != 4) //Ask user for phone number input unless user chose TBD
    {
        printf("\nContact: %s\n", phone->description);
        printf("Number : ");
        while (finInput == 0)
        {
            
            scanf(" %[^\n]", phone->number); //This means scan until a \n character is met, usually it's when any whitespace is met, this is to account for input with spaces. * with a conversion specifier means will keep reading and not store the info
            strLength = strlen(phone->number); //Upgraded to count length of string
            if (strLength != PHONE_LEN)
            {
                printf("Invalid %d-digit number! Number: ", PHONE_LEN);
            }
            else
            {
                finInput = 1;
            }
        }
        clearInputBuffer();
    }
    printf("\n");
}


//////////////////////////////////////
// FILE FUNCTIONS
//////////////////////////////////////

// Import patient data from file into a Patient array (returns # of records read)
int importPatients(const char* datafile, struct Patient patients[], int max)
{
    int row;
    FILE* filePointer = NULL;
    filePointer = fopen(datafile, "r");

    if (filePointer != NULL)
    {
        for (row = 0; row < max && feof(filePointer) != 1; row++)
        {
            fscanf(filePointer, "%d|%[^|]|%[^|]|%[^\n]", &patients[row].patientNumber, patients[row].name,
                patients[row].phone.description, patients[row].phone.number);

            if (feof(filePointer) == 1) //Checks if the next line is empty before scanning for it, avoids trailing empty space at the end of the txt file
            {
                row--;
            }
        }
    }
    else
    {
        printf("Error reading file\n");
    }
    fclose(filePointer);
    filePointer = NULL;

    return row;
}


// Import appointment data from file into an Appointment array (returns # of records read)
int importAppointments(const char* datafile, struct Appointment appoints[], int max)
{
    int row;
    FILE* filePointer = NULL;
    filePointer = fopen(datafile, "r");

    if (filePointer != NULL)
    {
        for (row = 0; row < max && feof(filePointer) != 1; row++)
        {
            fscanf(filePointer, "%d,%d,%d,%d,%d,%d", &appoints[row].patientNumber, &appoints[row].date.year, 
                &appoints[row].date.month, &appoints[row].date.day, &appoints[row].time.hour, &appoints[row].time.min);

            if (feof(filePointer) == 1)//Checks if the next line is empty before scanning for it, avoids trailing empty space at the end of the txt file
            {
                row--;
            }
        }
    }
    else
    {
        printf("Error reading file\n");
    }
    fclose(filePointer);
    filePointer = NULL;

    return row;
}