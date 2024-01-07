
//Safeguards ensure that only one file is included
#ifndef CLINIC_H
#define CLINIC_H


#define FMT_FORM 1
#define FMT_TABLE 2

#define NAME_LEN 15
#define PHONE_DESC_LEN 4
#define PHONE_LEN 10

#define CURR_YEAR 2023
#define APP_INTERVAL 30
#define OPEN_HOUR 10
#define OPEN_MIN 0
#define CLOSE_HOUR 14
#define CLOSE_MIN 0



//////////////////////////////////////
// Structures
//////////////////////////////////////

struct Phone
{
    char description[PHONE_DESC_LEN + 1];
    char number[PHONE_LEN + 1];
};

struct Patient
{
    int patientNumber;
    char name[NAME_LEN];
    struct Phone phone;
};

struct Time
{
    int hour, min;
};

struct Date
{
    int year, month, day;
};

struct Appointment
{
    int patientNumber;
    struct Date date;
    struct Time time;
};

struct ClinicData
{
    struct Patient* patients;
    int maxPatient;
    struct Appointment* appointments;
    int maxAppointments;
};



//////////////////////////////////////
// DISPLAY FUNCTIONS
//////////////////////////////////////

// Display's the patient table header (table format)
void displayPatientTableHeader(void);

// Displays a single patient record in FMT_FORM or FMT_TABLE format
void displayPatientData(const struct Patient* patient, int format);

// Display's appointment schedule headers (date-specific or all records)
void displayScheduleTableHeader(const struct Date* date, int isAllRecords);

// Display a single appointment record with patient info. in tabular format
void displayScheduleData(const struct Patient* patient,
                         const struct Appointment* appoint,
                         int includeDateField);



//////////////////////////////////////
// MENU & ITEM SELECTION FUNCTIONS
//////////////////////////////////////

// Displays main menu with choice to go to patient or appointment management
void menuMain(struct ClinicData* data);

// Menu for patient management
void menuPatient(struct Patient patient[], int max);

// Menu for patient edit
void menuPatientEdit(struct Patient* patient);

// Menu for appointment management
void menuAppointment(struct ClinicData* data);

// Display's all patient data in the FMT_FORM | FMT_TABLE format
void displayAllPatients(const struct Patient patient[], int max, int format);

// Search for a patient record based on patient number or phone number
void searchPatientData(const struct Patient patient[], int max);

// Add a new patient record to the patient array
void addPatient(struct Patient patient[], int max);

// Edit a patient record from the patient array
void editPatient(struct Patient patient[], int max);

// Remove a patient record from the patient array
void removePatient(struct Patient patient[], int max);

// View ALL scheduled appointments
void viewAllAppointments(struct ClinicData* data);

// View appointment schedule for the user input date
void viewAppointmentSchedule(const struct ClinicData* data);

// Add an appointment record to the appointment array
void addAppointment(struct Appointment appoint[], int maxAppointments,
    struct Patient patients[], int maxPatient);

// Remove an appointment record from the appointment array
void removeAppointment(struct Appointment appoint[], int maxAppointments,
    struct Patient patients[], int maxPatient);



//////////////////////////////////////
// UTILITY FUNCTIONS
//////////////////////////////////////

// Search and display patient record by patient number (form)
void searchPatientByPatientNumber(const struct Patient patient[], int max);

// Search and display patient records by phone number (tabular)
void searchPatientByPhoneNumber(const struct Patient patient[], int max);

// Get the next highest patient number
int nextPatientNumber(const struct Patient patient[], int max);

// Find the patient array index by patient number (returns -1 if not found)
int findPatientIndexByPatientNum(int patientNumber, const struct Patient patient[], int max);

// Counts and returns the current number of appointments that exists in the list. Returns the counted value via return statement
int countAppoints(const struct Appointment appoint[]);

// Sorts the appointment list in ascending chronological order. Comparisons are made on year, month, day, hour, min
void bubbleSortAppoint(struct Appointment appoint[], int numOfAppoints);

//Finds if patient exists in the current list of patients.Returns 1 if true, 0 if false
int patientExists(struct Patient patients[], int patientNum, int maxPatient);



//////////////////////////////////////
// USER INPUT FUNCTIONS
//////////////////////////////////////

// Get user input for a new patient record
void inputPatient(struct Patient* patient);

// Get user input for phone contact information
void inputPhoneData(struct Phone* phone);



//////////////////////////////////////
// FILE FUNCTIONS
//////////////////////////////////////

// Import patient data from file into a Patient array (returns # of records read)
int importPatients(const char* datafile, struct Patient patients[], int max);

// Import appointment data from file into an Appointment array (returns # of records read)
int importAppointments(const char* datafile, struct Appointment appoints[], int max);


#endif // !CLINIC_H