// Copyright 2023 - 2024 Ursu Vasile
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timelib.h"

// Holds the information about a person's available interval
typedef struct {
    unsigned int start;
    int duration;
} Tinterval;

// Holds all information about each person
typedef struct {
    char name[MAX_MONTH_WORD_LENGTH];
    int timezone_index;
    int K;
    Tinterval *intervals;
} TPerson;

// Compare function for sorting intervals based on starting point
int compare_intervals(const void* a, const void* b) {
    Tinterval first = *(Tinterval*)a;
    Tinterval second = *(Tinterval*)b;
    return (int)first.start - (int)second.start;
}

// Compare function for sorting persons based on name
int compare_persons(const void* a, const void* b) {
    TPerson first = *(TPerson*)a;
    TPerson second = *(TPerson*)b;
    return strcmp(first.name, second.name);
}

int main() {
    int T = 0;
    scanf("%d", &T);
    // Allocate and read T timezones
    TTimezone *timezones = NULL;
    timezones = malloc(T * sizeof(TTimezone));
    for (int i = 0; i < T; i++) {
        scanf("%s", timezones[i].name);
        scanf("%hhd", &timezones[i].utc_hour_difference);
    }
    int P = 0;
    // Allocate and read P persons
    scanf("%d", &P);
    TPerson *persons = NULL;
    persons = malloc(P * sizeof(TPerson));
    int SEC_IN_HOUR = SEC_IN_MIN * MIN_IN_HOUR;
    for (int i = 0; i < P; i++) {
        scanf("%s", persons[i].name);
        char zone[MAX_TIMEZONE_WORD_LENGTH];
        scanf("%s", zone);
        for (int j = 0; j < T; j++) {
            if (!strcmp(zone, timezones[j].name)) {
                persons[i].timezone_index = j;
                break;
            }
        }
        // Allocate and read all intervals at which each person is available
        scanf("%d", &persons[i].K);
        if (persons[i].K == 0) {
            persons[i].intervals = NULL;
            continue;
        }
        persons[i].intervals = malloc(persons[i].K * sizeof(Tinterval));
        for (int j = 0; j < persons[i].K; j++) {
            TDateTimeTZ current;
            scanf("%d%hhd", &current.date.year, &current.date.month);
            scanf("%hhd%hhd", &current.date.day, &current.time.hour);
            current.time.min = 0;
            current.time.sec = 0;
            current.tz = timezones + persons[i].timezone_index;
            persons[i].intervals[j].start = convertDateTimeTZToUnixTimestamp(current);
            int length = 0;
            scanf("%d", &length);
            persons[i].intervals[j].duration = length * SEC_IN_HOUR;
        }
    }
    // Read the number of requiered attendants for the event and the event length
    int F = 0, L = 0;
    scanf("%d", &F);
    scanf("%d", &L);
    // Sort and concatenate continuous intervals for each person
    for (int i = 0; i < P; i++) {
        qsort(persons[i].intervals, persons[i].K, sizeof(Tinterval), compare_intervals);
    }
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < persons[i].K - 1; j++) {
            Tinterval *curr = persons[i].intervals;
            if (curr[j].start + curr[j].duration == curr[j + 1].start) {
                curr[j].duration += curr[j + 1].duration;
                persons[i].K--;
                for (int k = j + 1; k < persons[i].K; k++) {
                    curr[k].duration = curr[k + 1].duration;
                    curr[k].start = curr[k + 1].start;
                }
                j--;
            }
        }
    }
    // Iterating through all hours, I try to make the event start at each hour,
    // then check how many persons can attend it. If enough people are available,
    // stop, and return a valid starting point for the event, thus it is guaranteed to be the first.
    // If the program reaches the 31 of December 2050 without fitting the event,
    // then it is impossible to do it.
    int flag = 0, available = 0;
    unsigned int iterator = 0;
    int *ind = NULL;
    ind = calloc(P, sizeof(int));
    qsort(persons, P, sizeof(TPerson), compare_persons);
    while (iterator <= MAX_HOURS * SEC_IN_HOUR && !flag) {
        available = 0;
        for (int i = 0; i < P; i++) {
            // The event fits in a person's available interval if it starts at the exact moment or later,
            // and ends before or at the exact time the interval does.
            Tinterval *curr = persons[i].intervals;
            if (curr == NULL) {
                continue;
            }
            if (curr[ind[i]].start <= iterator) {
                if (curr[ind[i]].start + curr[ind[i]].duration >= iterator + L * SEC_IN_HOUR) {
                    available++;
                } else if (curr[ind[i]].start + curr[ind[i]].duration == iterator) {
                    if (ind[i] < persons[i].K - 1) {
                        ind[i]++;
                    }
                }
            }
        }
        if (available >= F) {
            flag = 1;
        } else {
            iterator += SEC_IN_HOUR;
        }
    }
    if (!flag) {
        printf("imposibil\n");
        for (int i = 0; i < P; i++) {
            free(persons[i].intervals);
        }
        free(persons);
        free(timezones);
        free(ind);
    return 0;
        return 0;
    } else {
        for (int i = 0; i < P; i++) {
            Tinterval *curr = persons[i].intervals;
            if (curr == NULL) {
                printf("%s: invalid\n", persons[i].name);
                continue;
            }
            if (curr[ind[i]].start <= iterator) {
                if (curr[ind[i]].start + curr[ind[i]].duration >= iterator + L * SEC_IN_HOUR) {
                    printf("%s: ", persons[i].name);
                    printDateTimeTZ(convertUnixTimestampToDateTimeTZ(iterator, timezones, persons[i].timezone_index));
                } else {
                    printf("%s: invalid\n", persons[i].name);
                }
            } else {
                printf("%s: invalid\n", persons[i].name);
            }
        }
    }
    // After writing the answer, free all allocated memory
    for (int i = 0; i < P; i++) {
        free(persons[i].intervals);
    }
    free(persons);
    free(timezones);
    free(ind);
    return 0;
}
