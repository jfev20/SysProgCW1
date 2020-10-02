#include <string.h>
#include <date.h>

struct date {
    int dd, mm, yy;
};

int main() {

}

Date *date_create(char *datestr){
    Date date;
    char dmy[3][5]; // day month year matrix
    //char dayStr[3];
    //char monthStr[3];
    //char yearStr[5];
    int index, section, i;

    index = 0; section = 0;
    for (i = 0; i <= (strlen(datestr)); i++) {          // is *datestr value or datestr?
        if (datestr[i] == '/' || datestr[i] == '\0') {  // if / or null found... 
            dmy[section][index]='\0';                   // ...assign null to datestr
            section++;                                  // next section of date string
            index = 0;                                  // reset index
        }
        else
        {
            dmy[section][index] = datestr[i];
            index++;
        }
        
    }

    //strncpy(dayStr, *datestr, 2);
    //dayStr[2] = '\0';
}

void date_destroy(Date *d){
    free(d->dd);
    free(d->mm);
    free(d->yy);
    free(d);
}