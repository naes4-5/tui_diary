#include "../includes/includes.h"
#include <time.h>

const char *days[7] = {"Sunday",   "Monday", "Tuesday", "Wednesday",
                       "Thursday", "Friday", "Saturday"};

const char *months[] = {"1.January/",  "2.February/",  "3.March/",
                               "4.April/",    "5.May/",       "6.June/",
                               "7.July/",     "8.August/",    "9.September/",
                               "10.October/", "11.November/", "12.December/"};

int getmonth() {
    time_t fill = time(NULL);
    struct tm *now = localtime(&fill);
    return now->tm_mon;
}

int getday() {
    time_t fill = time(NULL);
    struct tm *now = localtime(&fill);
    return now->tm_mday;
}

char *dailyheader() {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    char *header = malloc(256);
    strftime(header, 256, "Made on %d-%m-%y, %H:%M", t);
    return header;
}

FILE *mknote(const char *title, const char *path) {
    if (!strcmp("", title)) {
        fprintf(stderr, "Note must have a title\n");
        return NULL;
    }

    const char *header = dailyheader();
    if (header == NULL) {
        fprintf(stderr, "Error in creating header\n");
        return NULL;
    }
    printf("Header: %s\n", header);

    char notedir[PATH_MAX];
    int written = snprintf(notedir, sizeof(notedir), "%s", path);
    if (written < 0 || written >= sizeof(notedir)) {
        free((void *)header);
        return NULL;
    }

    written += snprintf(notedir + written, sizeof(notedir) - written, "%s", title);
    printf("path to note is %s\n", notedir);

    FILE *f = fopen(notedir, "w");
    if (f == NULL) {
        perror("Error opening file from mknote");
        free((void *)header);
        return NULL;
    }
    if (header) {
        fprintf(f, "%s\n\n", header);
    }
    free((void *)header);
    return f;
}
