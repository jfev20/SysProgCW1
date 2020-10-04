#include <string.h>
#include <date.h>

typedef struct date {
    int dd, mm, yy;
} Date;

int main() {    // test function - remove when submitting

    char datestr[11] = "28/02/2019";

    printf("-------- TEST DATE CREATION --------\n");
    Date *outdate = date_create(datestr);
    if (outdate == NULL) {
        prinf("Date could not be created\n");
    } else {
        printf("Date created: %d/%d/%d\n", outdate->dd, outdate->mm, outdate->yy);
    }

    printf("-------- TEST DATE DESTRUCTION --------\n");
    date_destory(outdate);
    printf("end of program");
}

Date *date_create(char *datestr){
    /**
     * QUESTIONS
     * should we only accept / as the separator?
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
        printf("Invalid date format: first / missing");
        return NULL;                // return null and quit
    }

    index = (int)(sep2 - datestr);  // store position of first /
    printf("Position of first / is %d\n", index);

    sep2 = strchr(sep1 + 1, '/');   // search for second occurance of / starting from char after first /

    if (sep2 == NULL) {             // if second / not found
        printf("Invalid date format: second / missing\n");
        return NULL;                // return null and quit
    } 

    index = (int)(sep2 - datestr);  // store position of second /
    printf("Position of second / is %d", index);

    int day, month, year = 0;       // initialise days, months and years
    char* endptr;
    
    day = strtol(datestr, &endptr, 10); // convert first part of string (days) to int

    if (endptr != sep1) {           // if the end points to anything other than the position of the first /
        printf("Invalid characters before first /\n");
        return NULL;
    }
    printf("Day: %d", day);

    month = strtol(sep1 + 1, &endptr, 10); // convert after first / (months) to int

    if (endptr != sep2) {           // if the end points to anything other than the position of the second /
        printf("Invalid characters before second /\n");
        return NULL;
    }
    printf("Month: %d", month);

    year = strtol(sep2 + 1, &endptr, 10); // convert last part of string (years) to int

    if (*endptr != '\0') {      // if the end points to anything other than a terminator
        printf("Invalid characters at end of date\n");
        return NULL;
    }
    printf("Year: %d", year);
    
    int result = valid_date(day, month, year);  // check if final result is a valid date (e.g. 29th Feb only on leap years/no 33rd of months/max 12 months etc)

    if (result == 0) {              // if date validation fails
        printf("Invalid date");     
        return NULL;                // return null and quit
    }

    printf("Date valid");

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
    Date* dupdate = malloc(sizeof(Date));       // allocate memory for duplicate date structure

    if (dupdate == NULL) {                      // if mem allocation fails
        printf("Memory allocation failure\n");
        return NULL;                            // return null and quit
    }

    int dupday, dupmonth, dupyear = 0;          // initialise new values for duplicate date
    d->dd = dupday;
    d->mm = dupmonth;
    d->yy = dupyear;

    dupdate->dd = dupday;                       // set new values to duplicate date structure
    dupdate->mm = dupmonth;
    dupdate->yy = dupyear;

    return dupdate;                             // return new duplicate date
}

void date_destroy(Date *d){
	printf("About to free storage\n");
	free(d);                                    // free up memory at pointer location
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