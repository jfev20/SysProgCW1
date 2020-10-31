#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "date.h"

typedef struct date {
    int dd, mm, yy;
} Date;

int valid_date(int d, int m, int y); // declaring date validation method 
int leap_year(int year); // declaring leap year validation method 

int main() {    // test function - remove when submitting

    char datestr1[11] = "28/02/2019";
    char datestr2[11] = "16/07/2018";

    printf("-------- TEST DATE CREATION --------\n");
    Date *outdate1 = date_create(datestr1);
    //if (outdate1 == NULL) {
    //    printf("Date 1 could not be created\n");
    //} else {
    //    printf("Date 1 created: %d/%d/%d\n", outdate1->dd, outdate1->mm, outdate1->yy);
    //}
    outdate1 == NULL ? printf("Date 1 could not be created\n") : printf("Date 1 created: %d/%d/%d\n", outdate1->dd, outdate1->mm, outdate1->yy);

    Date *outdate2 = date_create(datestr2);
    outdate2 == NULL ? printf("Date 2 could not be created\n") : printf("Date 2 created: %d/%d/%d\n", outdate2->dd, outdate2->mm, outdate2->yy);

    printf("-------- TEST DATE DUPLICATION --------\n");
    Date *outdate3 = date_duplicate(outdate1);
    outdate3 == NULL ? printf("Date 1 could not be duplicated\n") : printf("Date 3 created: %d/%d/%d\n", outdate3->dd, outdate3->mm, outdate3->yy);

    printf("------- TEST DATE COMPARISON ---------\n");
    int compResult = date_compare(outdate1, outdate2);
    printf("%d/%d/%d is sooner than %d/%d/%d, the result should be 1: %d\n", outdate1->dd, outdate1->mm, outdate1->yy, outdate2->dd, outdate2->mm, outdate2->yy, compResult);
    compResult = date_compare(outdate1, outdate3);
    printf("%d/%d/%d is the same as %d/%d/%d, the result should be 0: %d\n", outdate1->dd, outdate1->mm, outdate1->yy, outdate3->dd, outdate3->mm, outdate3->yy, compResult);
    compResult = date_compare(outdate2, outdate1);
    printf("%d/%d/%d is older than %d/%d/%d, the result should be -1: %d\n", outdate2->dd, outdate2->mm, outdate2->yy, outdate1->dd, outdate1->mm, outdate1->yy, compResult);

    printf("-------- TEST DATE DESTRUCTION --------\n");
    date_destroy(outdate1);
    printf("end of program\n");
}

Date *date_create(char *datestr){
    /**
     * QUESTIONS
     * Should the position of the separators always be the same (pos 2 & 5)?
     * Is there a limit of the years we should accept? (e.g. 1990 to 2020)
     * does date_duplication have any validation?
     * 
    */

    char* sep1;
    char* sep2;
    int index = 0;

    sep1 = strchr(datestr, '/');    // search for first occurance of / starting from beginning of string

    if (sep1 == NULL) {             // if / not found
        printf("Invalid date format: first / missing\n");
        return NULL;                // return null and quit
    }

    index = (int)(sep1 - datestr);  // store position of first /
    printf("Position of first / is %d\n", index);

    sep2 = strchr(sep1 + 1, '/');   // search for second occurance of / starting from char after first /

    if (sep2 == NULL) {             // if second / not found
        printf("Invalid date format: second / missing\n");
        return NULL;                // return null and quit
    } 

    index = (int)(sep2 - datestr);  // store position of second /
    printf("Position of second / is %d\n", index);

    int day, month, year = 0;       // initialise days, months and years
    char* endptr = NULL;
    
    day = strtol(datestr, &endptr, 10); // convert first part of string (days) to int

    if (endptr != sep1) {           // if the end points to anything other than the position of the first /
        printf("Invalid characters before first /\n");
        return NULL;
    }
    printf("Day: %d\n", day);

    month = strtol(sep1 + 1, &endptr, 10); // convert after first / (months) to int

    if (endptr != sep2) {           // if the end points to anything other than the position of the second /
        printf("Invalid characters before second /\n");
        return NULL;
    }
    printf("Month: %d\n", month);

    year = strtol(sep2 + 1, &endptr, 10); // convert last part of string (years) to int

    if (*endptr != '\0') {      // if the end points to anything other than a terminator
        printf("Invalid characters at end of date\n");
        return NULL;
    }
    printf("Year: %d\n", year);
    
    // check if final result is a valid date (e.g. 29th Feb only on leap years/no 33rd of months/max 12 months etc)
    if (!(valid_date(day, month, year))) {              // if date validation fails
        printf("Invalid date\n");     
        return NULL;                // return null and quit
    }

    printf("Date valid\n");

	Date* gooddate = malloc(sizeof(Date));      // allocate memory for date structure

    if (gooddate == NULL) {                     // if mem allocation fails
        printf("Memory allocation failure\n");
        return NULL;                            // return null and quit
    }

    gooddate->dd = day;                         // store day, month, year variables in newly created date structure
    gooddate->mm = month;
    gooddate->yy = year;

    return gooddate;                            // return address of date structure
}

Date *date_duplicate(Date *d){
    Date* dupdate = malloc(sizeof(*d));       // allocate memory for duplicate date structure

    if (dupdate != NULL) {                      // if mem allocation doesn't fail
        *dupdate = *d;                          // duplicate date to new address
    }

    return dupdate;                             // return new duplicate date
}

/*
 * date_compare compares two dates, returning <0, 0, >0 if
 * date1<date2, date1==date2, date1>date2, respectively
 */
int date_compare(Date *date1, Date *date2){ // finish this: only returns 0
    
    if (date1->yy > date2->yy) return 1;

    if (date1->yy < date2->yy) return -1;

    if (date1->mm > date2->mm) return 1;

    if (date1->mm < date2->mm) return -1;

    if (date1->dd > date2->dd) return 1;

    if (date1->dd < date2->dd) return -1;

    return 0;

    /*int day1 = date1->dd;
    int month1 = date1->mm;
    int year1 = date1->yy;                // initialise new variables for date1 values
    printf("date1 - day: %d, month: %d, year: %d\n", day1, month1, year1);


    int day2 = date2->dd;
    int month2 = date2->mm;
    int year2 = date2->yy;                // initialise new variables for date2 values
    printf("date2 - day: %d, month: %d, year: %d\n", day2, month2, year2);

    int result = -2;

    if (year1 < year2) {            // if the first year is older than the second year then return -1
        result = -1;
    } else if (year1 > year2) {     // if the first year is more recent than the second year return 1
        result = 1;
    } else if (year1 == year2) {    // if the years are the same...
        if (month1 < month2) {      // ...check if the first month is older than the second month then return -1
            result = -1;
        } else if (month1 > month2) { // if the first month is more recent that the second month return 1
            result = 1;
        } else if (month1 == month2) {  // if the months are the same...
            if (day1 < day2) {          // ...check if the first day is older that the second day, return -1
                result = -1;
            } else if (day1 > day2) {   // if the first day is more recent that the second day, return 1
                result = 1;
            } else {
                result = 0;               // the dates are exactly the same, return 0
            }
        }
    }
    return result;*/
}

void date_destroy(Date *d){
	printf("About to free storage\n");
	free(d);                                    // free up memory at pointer location
    d = NULL;
	printf("Storage freed\n");
}

int valid_date(int d, int m, int y) {
	int febdays;
	int leap;

	if ((y < 1850) || (y > 9999))               // check years are within bounds of accepted range
	{
		printf("Invalid years entered\n");
		return 0;
	}

	/*   Day validation      */

	if (d < 1)                              // make sure days value > 0
	{
		printf("Invalid days entered\n");
		return 0;
	}

	leap = leap_year(y);                    // check to see if year is leap year

	if (leap == 1)                          // if year is leap year...
		febdays = 29;                       // ...set max days in feb to 29...
	else
		febdays = 28;                       // ... otherwise max feb days to 28

	switch (m)                              // check if days is not greater than days allowed in month depending on month
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		if (d > 31)                         // check if day is within 31 
		{
			printf("Invalid days entered\n");
			return 0;
		}
		break;

	case 4:
	case 6:
	case 9:
	case 11:
		if (d > 30)                         // check if day is within 30 
		{
			printf("Invalid days entered\n");
			return 0;
		}
		break;

	case 2:
		if (d > febdays)                    // check if day is within max days allowed in feb (see above)
		{
			printf("Invalid days entered\n");
			return 0;
		}
		break;

		/*   Month validation      */
	default:
		printf("Invalid months entered\n");
		return 0;
	}

	return 1;                               // if all checks are passed without error, return 1 (true)
}

int leap_year(int year) {
	/*   Leap year calculation      */
	int leap = 0;
	if (year % 4 == 0)
		leap = 1;
	if (year % 100 == 0)
		leap = 0;
	if (year % 400 == 0)
		leap = 1;
	if (leap)
		printf("Leap year entered\n");
	return leap;
}