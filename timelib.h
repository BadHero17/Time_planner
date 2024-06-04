#ifndef TIMELIB_H_INCLUDED
#define TIMELIB_H_INCLUDED

#define SEC_IN_MIN 60
#define MIN_IN_HOUR 60
#define HOUR_IN_DAY 24
#define DAY_IN_YEAR 365
#define START_YEAR 1970
#define LONG_MONTH 31
#define SHORT_MONTH 30
#define FEBRUARY_DAYS 28
#define LEAP_FEBRUARY_DAYS 29
#define JAN 1
#define FEB 2
#define MAR 3
#define APR 4
#define MAY 5
#define JUN 6
#define JUL 7
#define AUG 8
#define SEP 9
#define OCT 10
#define NOV 11
#define DEC 12
#define L_Y_GAP 4
#define L_Y_FIX_1 100
#define L_Y_FIX_2 400
#define L_Y_TILL_OFFSET 90
#define L_Y_OFFSET 370
#define UI_SIZE 32
#define DAY_BITS 5
#define MONTH_BITS 4
#define YEAR_BITS 6
// This is the number of hours beetwen 1st of January 1970 and 31st of December 2050,
// necesarry for planner, can't be trivially calculated from the base amounts due to leap years
// so I took it directly from an online calculator :)
#define MAX_HOURS 710039
#define MONTH_COUNT 12
#define MAX_MONTH_WORD_LENGTH 11
#define MAX_TIMEZONE_WORD_LENGTH 6

typedef struct {
    unsigned char hour;
    unsigned char min;
    unsigned char sec;
} TTime;

typedef struct {
    unsigned char day;
    unsigned char month;
    unsigned int year;
} TDate;

typedef struct {
    char name[MAX_TIMEZONE_WORD_LENGTH];
    signed char utc_hour_difference;
} TTimezone;

typedef struct {
    TDate date;
    TTime time;
    TTimezone *tz;
} TDateTimeTZ;

TTime convertUnixTimestampToTime(unsigned int timestamp);
TDate convertUnixTimestampToDateWithoutLeapYears(unsigned int timestamp);
TDate convertUnixTimestampToDate(unsigned int timestamp);
TDateTimeTZ convertUnixTimestampToDateTimeTZ(unsigned int timestamp, TTimezone *timezones, int timezone_index);
unsigned int convertDateTimeTZToUnixTimestamp(TDateTimeTZ);
void printDateTimeTZ(TDateTimeTZ datetimetz);

#endif  // TIMELIB_H_INCLUDED
