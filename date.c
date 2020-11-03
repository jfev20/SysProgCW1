#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "date.h"

typedef struct date {
    int dd, mm, yy; // initialising days, months, & years
} Date;

/**
 * |========[ORDER OF FUNCTIONS]========|
 * |    HELPER METHOD INITIALISATION    |
 * |    DATE FUNCTIONS                  |
 * |    DATE VALIDATION FUNCTIONS       |
 * |====================================|
 */

//------------------------------------[HELPER METHOD INITIALISATION]-------------------------------------------------

int valid_date(int d, int m, int y);
int valid_day(int day, int max);
int leap_year_validation(int year);

//------------------------------------------[DATE FUNCTIONS]---------------------------------------------------------

/*
* date_create creates a Date structure from `datestr`
* `datestr' is expected to be of the form "dd/mm/yyyy"
* returns pointer to Date structure if successful,
* NULL if not (syntax error)
*/
Date *date_create(char *datestr){

    char* sep1;
    char* sep2;
    sep1 = strchr(datestr, '/');                            // search for first occurance of / starting from beginning of string
    if (sep1 == NULL) {                                     // if / not found
        //printf("Invalid date format: first / missing\n");
        return NULL;                                        // return null and quit
    }
    sep2 = strchr(sep1 + 1, '/');                           // search for second occurance of / starting from char after first /
    if (sep2 == NULL) {                                     // if second / not found
        //printf("Invalid date format: second / missing\n");
        return NULL;                                        // return null and quit
    } 

    int day, month, year = 0;                               // initialise days, months and years to 0
    char* endptr = NULL;

    day = strtol(datestr, &endptr, 10);                     // convert first part of string (days) to int
    if (endptr != sep1) {                                   // if the end points to anything other than the position of the first /
        //printf("Invalid characters before first /\n");
        return NULL;
    }

    month = strtol(sep1 + 1, &endptr, 10);                  // convert after first / (months) to int
    if (endptr != sep2) {                                   // if the end points to anything other than the position of the second /
        //printf("Invalid characters before second /\n");
        return NULL;
    }

    year = strtol(sep2 + 1, &endptr, 10);                   // convert last part of string (years) to int
    if (*endptr != '\0') {                                  // if the end points to anything other than a terminator
        //printf("Invalid characters at end of date\n");
        return NULL;
    }
    
    // check if final result is a valid date (e.g. 29th Feb only on leap years/no 33rd of months/max 12 months etc)
    if (!(valid_date(day, month, year))) {                  // if date validation fails
        //printf("Invalid date\n");     
        return NULL;                                        // return null and quit
    }

	Date* gooddate = malloc(sizeof(Date));                  // allocate memory for date structure
    if (gooddate == NULL) {                                 // if mem allocation fails
        //printf("Memory allocation failure\n");
        return NULL;                                        // return null and quit
    }

    // store day, month, year variables in newly created date structure
    gooddate->dd = day;                         
    gooddate->mm = month;
    gooddate->yy = year;

    return gooddate;                
}

/*
* date_duplicate creates a duplicate of `d'
* returns pointer to new Date structure if successful,
* NULL if not (memory allocation failure)
*/
Date *date_duplicate(Date *d){

    Date* dupdate = malloc(sizeof(*d));                     // allocate memory for duplicate date structure
    if (dupdate != NULL) {                                  // if mem allocation doesn't fail
        *dupdate = *d;                                      // duplicate date to new address
    }
    return dupdate;
}

/*
 * date_compare compares two dates, returning <0, 0, >0 if
 * date1<date2, date1==date2, date1>date2, respectively
 */
int date_compare(Date *date1, Date *date2){
    
    if (date1->yy > date2->yy) return 1;

    if (date1->yy < date2->yy) return -1;

    if (date1->mm > date2->mm) return 1;

    if (date1->mm < date2->mm) return -1;

    if (date1->dd > date2->dd) return 1;

    if (date1->dd < date2->dd) return -1;

    return 0;
}

/*
* date_destroy returns any storage associated with `d' to the system
*/
void date_destroy(Date *d){
	if (d != NULL) free(d);             // if date is not null free up memory at pointer location
    d = NULL;
}

//---------------------------------------[DATE VALIDATION FUNCTIONS]------------------------------------------------------

/**
 * helper method to check if date is valid
 * 
 * returns 1 if valid, 0 if invalid
 */
int valid_date(int day, int month, int year) {
	int days_in_feb;

    // check years are within bounds of accepted range
	if ((year < 1850) || (year > 9999))
		return 0; 

    // if year is leap year set max days in feb to 29, otherwise max feb days to 28
	days_in_feb = (leap_year_validation(year))? 29 : 28;   

    // check if days is not greater than days allowed in month depending on month
	switch (month) {                              
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
        if (!valid_day(day, 31)) return 0;                      // check if day is within 31
		break;
	case 4:
	case 6:
	case 9:
	case 11:                        
		if (!valid_day(day, 30)) return 0;                      // check if day is within 30 
		break;
	case 2:
        if (!valid_day(day, days_in_feb)) return 0;             // check if day is within max days allowed in feb (see above)
		break;
	default:
		return 0;
	}
	return 1;                                                   // if all checks are passed without error, return 1 (true)
}

/**
 * checks if days are valid/not illogical (e.g. below 0 or above 31)
 * 
 * returns 1 is day is valid, 0 is invalid
 */
int valid_day(int day, int max) {
    return (!(day < 1) || !(day > max));
}

/**
 * checks if given year is a leap year
 * 
 * returns 1 is year is leap year, 0 if not
 */
int leap_year_validation(int year) {
	int leap = 0;
	if (year % 4 == 0) leap = 1;

	if (year % 100 == 0) leap = 0;

    if (year % 400 == 0) leap = 1;

    return leap;
}