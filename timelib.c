// Copyright 2023 - 2024 Ursu Vasile
#include <stdio.h>
#include <stdlib.h>
#include "timelib.h"
TTime convertUnixTimestampToTime(unsigned int timestamp) {
    TTime result;
    result.sec = 0;
    result.min = 0;
    result.hour = 0;
    result.sec = timestamp % SEC_IN_MIN;
    timestamp /= SEC_IN_MIN;
    result.min = timestamp % MIN_IN_HOUR;
    timestamp /= MIN_IN_HOUR;
    result.hour = timestamp % HOUR_IN_DAY;
    return result;
}

TDate convertUnixTimestampToDateWithoutLeapYears(unsigned int timestamp) {
    TDate result;
    result.day = 0;
    result.month = 0;
    result.year = 0;
    int SEC_IN_DAY = SEC_IN_MIN * MIN_IN_HOUR * HOUR_IN_DAY;
    int days_since_epoch = (int)timestamp / SEC_IN_DAY;
    result.year = days_since_epoch / DAY_IN_YEAR + START_YEAR;
    int current_day = days_since_epoch % DAY_IN_YEAR + 1;
    int current = 0;
    if (current_day <= current + LONG_MONTH) {
        result.month = JAN;
        result.day = current_day - current;
        return result;
    } else {
        current += LONG_MONTH;
    }
    if (current_day <= current + FEBRUARY_DAYS && current_day > current) {
        result.month = FEB;
        result.day = current_day - current;
        return result;
    } else {
        current += FEBRUARY_DAYS;
    }
    if (current_day <= current + LONG_MONTH && current_day > current) {
        result.month = MAR;
        result.day = current_day - current;
        return result;
    } else {
        current += LONG_MONTH;
    }
    if (current_day <= current + SHORT_MONTH && current_day > current) {
        result.month = APR;
        result.day = current_day - current;
        return result;
    } else {
        current += SHORT_MONTH;
    }
    if (current_day <= current + LONG_MONTH && current_day > current) {
        result.month = MAY;
        result.day = current_day - current;
        return result;
    } else {
        current += LONG_MONTH;
    }
    if (current_day <= current + SHORT_MONTH && current_day > current) {
        result.month = JUN;
        result.day = current_day - current;
        return result;
    } else {
        current += SHORT_MONTH;
    }
    if (current_day <= current + LONG_MONTH && current_day > current) {
        result.month = JUL;
        result.day = current_day - current;
        return result;
    } else {
        current += LONG_MONTH;
    }
    if (current_day <= current + LONG_MONTH && current_day > current) {
        result.month = AUG;
        result.day = current_day - current;
        return result;
    } else {
        current += LONG_MONTH;
    }
    if (current_day <= current + SHORT_MONTH && current_day > current) {
        result.month = SEP;
        result.day = current_day - current;
        return result;
    } else {
        current += SHORT_MONTH;
    }
    if (current_day <= current + LONG_MONTH && current_day > current) {
        result.month = OCT;
        result.day = current_day - current;
        return result;
    } else {
        current += LONG_MONTH;
    }
    if (current_day <= current + SHORT_MONTH && current_day > current) {
        result.month = NOV;
        result.day = current_day - current;
        return result;
    } else {
        current += SHORT_MONTH;
    }
    if (current_day <= current + LONG_MONTH && current_day > current) {
        result.month = DEC;
        result.day = current_day - current;
        return result;
    } else {
        current += LONG_MONTH;
    }
    return result;
}

TDate convertUnixTimestampToDate(unsigned int timestamp) {
    TDate result;
    result.day = 0;
    result.month = 0;
    result.year = 0;
    int SEC_IN_DAY = SEC_IN_MIN * MIN_IN_HOUR * HOUR_IN_DAY;
    int days_since_epoch = (int)timestamp / SEC_IN_DAY;
    int year_since_epoch = days_since_epoch / DAY_IN_YEAR;
    int year_checker = year_since_epoch + L_Y_OFFSET;
    if ((year_checker % L_Y_GAP == 0 && year_checker % L_Y_FIX_1 != 0) || year_checker % L_Y_FIX_2 == 0) {
        // Check if leap year
        int cnt = year_checker / L_Y_GAP - year_checker / L_Y_FIX_1 + year_checker / L_Y_FIX_2;
        cnt -= L_Y_TILL_OFFSET;
        days_since_epoch -= cnt;
        // If we are at a late enough date, there is a chance we consider a leap_year
        // while we aren't in one, thus we add one to the counter since cnt is smaller in leap years
        if (days_since_epoch / DAY_IN_YEAR != year_since_epoch) {
            days_since_epoch++;
        }
        if (days_since_epoch % DAY_IN_YEAR == LONG_MONTH + LEAP_FEBRUARY_DAYS - 1) {
            // Check if it's the 29th of February
            result.year = days_since_epoch / DAY_IN_YEAR;
            result.month = FEB;
            result.day = LEAP_FEBRUARY_DAYS;
            return result;
        } else if (days_since_epoch % DAY_IN_YEAR > LONG_MONTH + LEAP_FEBRUARY_DAYS - 1) {
            // If the date is beyond the 29th of February, substract one day from counter;
            days_since_epoch--;
        }
    } else {
        int cnt = year_checker / L_Y_GAP - year_checker / L_Y_FIX_1 + year_checker / L_Y_FIX_2;
        cnt -= L_Y_TILL_OFFSET;
        days_since_epoch -= cnt;
        days_since_epoch--;
    }
    result = convertUnixTimestampToDateWithoutLeapYears(days_since_epoch * SEC_IN_DAY);
    return result;
}

TDateTimeTZ convertUnixTimestampToDateTimeTZ(unsigned int timestamp, TTimezone *timezones, int timezone_index) {
    TDateTimeTZ result;
    int SEC_IN_HOUR = SEC_IN_MIN * MIN_IN_HOUR;
    timestamp += timezones[timezone_index].utc_hour_difference * SEC_IN_HOUR;
    result.tz = timezones + timezone_index;
    result.date = convertUnixTimestampToDate(timestamp);
    result.time = convertUnixTimestampToTime(timestamp);
    return result;
}

unsigned int convertDateTimeTZToUnixTimestamp(TDateTimeTZ datetimetz) {
    unsigned int result = 0;
    // Calculate the result ignoring leap years
    // then add those extra days afterwards
    int SEC_IN_DAY = SEC_IN_MIN * MIN_IN_HOUR * HOUR_IN_DAY;
    int SEC_IN_HOUR = SEC_IN_MIN * MIN_IN_HOUR;
    int SEC_IN_YEAR = DAY_IN_YEAR * SEC_IN_DAY;
    result += SEC_IN_YEAR * (datetimetz.date.year - START_YEAR);
    if (datetimetz.date.month > JAN) {
        result += LONG_MONTH * SEC_IN_DAY;
    }
    if (datetimetz.date.month > FEB) {
        result += FEBRUARY_DAYS * SEC_IN_DAY;
    }
    if (datetimetz.date.month > MAR) {
        result += LONG_MONTH * SEC_IN_DAY;
    }
    if (datetimetz.date.month > APR) {
        result += SHORT_MONTH * SEC_IN_DAY;
    }
    if (datetimetz.date.month > MAY) {
        result += LONG_MONTH * SEC_IN_DAY;
    }
    if (datetimetz.date.month > JUN) {
        result += SHORT_MONTH * SEC_IN_DAY;
    }
    if (datetimetz.date.month > JUL) {
        result += LONG_MONTH * SEC_IN_DAY;
    }
    if (datetimetz.date.month > AUG) {
        result += LONG_MONTH * SEC_IN_DAY;
    }
    if (datetimetz.date.month > SEP) {
        result += SHORT_MONTH * SEC_IN_DAY;
    }
    if (datetimetz.date.month > OCT) {
        result += LONG_MONTH * SEC_IN_DAY;
    }
    if (datetimetz.date.month > NOV) {
        result += SHORT_MONTH * SEC_IN_DAY;
    }
    result += SEC_IN_DAY * (datetimetz.date.day - 1);
    result += SEC_IN_HOUR * datetimetz.time.hour;
    result += SEC_IN_MIN * datetimetz.time.min;
    result += datetimetz.time.sec;
    // Calculate the number of 29ths of February ignored then add them to the timestamp

    int year_since_epoch = (int)datetimetz.date.year - START_YEAR;
    int year_checker = year_since_epoch + L_Y_OFFSET;
    int cnt = 0;
    if ((year_checker % L_Y_GAP == 0 && year_checker % L_Y_FIX_1 != 0) || year_checker % L_Y_FIX_2 == 0) {
        // Check if leap year
        cnt = year_checker / L_Y_GAP - year_checker / L_Y_FIX_1 + year_checker / L_Y_FIX_2;
        cnt -= L_Y_TILL_OFFSET;
        if (datetimetz.date.month > FEB) {
            cnt++;
        }
    } else {
        cnt = year_checker / L_Y_GAP - year_checker / L_Y_FIX_1 + year_checker / L_Y_FIX_2;
        cnt -= L_Y_TILL_OFFSET - 1;
    }
    result += cnt * SEC_IN_DAY;
    result += SEC_IN_HOUR * (-datetimetz.tz->utc_hour_difference);
    return result;
}

void printDateTimeTZ(TDateTimeTZ datetimetz) {
    printf("%02d ", datetimetz.date.day);
    if (datetimetz.date.month == JAN) {
        printf("ianuarie ");
    } else if (datetimetz.date.month == FEB) {
        printf("februarie ");
    } else if (datetimetz.date.month == MAR) {
        printf("martie ");
    } else if (datetimetz.date.month == APR) {
        printf("aprilie ");
    } else if (datetimetz.date.month == MAY) {
        printf("mai ");
    } else if (datetimetz.date.month == JUN) {
        printf("iunie ");
    } else if (datetimetz.date.month == JUL) {
        printf("iulie ");
    } else if (datetimetz.date.month == AUG) {
        printf("august ");
    } else if (datetimetz.date.month == SEP) {
        printf("septembrie ");
    } else if (datetimetz.date.month == OCT) {
        printf("octombrie ");
    } else if (datetimetz.date.month == NOV) {
        printf("noiembrie ");
    } else if (datetimetz.date.month == DEC) {
        printf("decembrie ");
    }
    printf("%d, ", datetimetz.date.year);
    printf("%02d:%02d:%02d %s", datetimetz.time.hour, datetimetz.time.min, datetimetz.time.sec, datetimetz.tz->name);
    if (datetimetz.tz->utc_hour_difference >= 0) {
        printf(" (UTC+%d)\n", datetimetz.tz->utc_hour_difference);
    } else {
        printf(" (UTC-%d)\n", -datetimetz.tz->utc_hour_difference);
    }
}
