#ifndef _DATE_H_INCLUDED_
#define _DATE_H_INCLUDED_

typedef struct date Date;

/*
 * date_create creates a Date structure from `datestr`
 * `datestr' is expected to be of the form "dd/mm/yyyy"
 * returns pointer to Date structure if successful,
 *         NULL if not (syntax error)
 * 
 * The constructor for this ADT is date_create(); it converts 
 * a datestring in the format “dd/mm/yyyy” to a Date structure.
 * You will have to use malloc()to allocate this Date structure 
 * to return to the user. 
 */
Date *date_create(char *datestr);

/*
 * date_duplicate creates a duplicate of `d'
 * returns pointer to new Date structure if successful,
 *         NULL if not (memory allocation failure)
 * 
 * date_duplicate()is known as a copy constructor; 
 * it duplicates the Date argument on the heap (using malloc()) 
 * and returns it to the user.
 */
Date *date_duplicate(Date *d);

/*
 * date_compare compares two dates, returning <0, 0, >0 if
 * date1<date2, date1==date2, date1>date2, respectively
 */
int date_compare(Date *date1, Date *date2);

/*
 * date_destroy returns any storage associated with `d' 
 * to the system
 */
void date_destroy(Date *d);

#endif /* _DATE_H_INCLUDED_ */
