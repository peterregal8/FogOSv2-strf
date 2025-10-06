#ifndef TIME_H
#define TIME_H

struct tm {
	int sec;
	int min;
	int hour;
	int mday;	// day of the month
	int month;
	int year;
	int wday;	// day of the week (sunday - saturday i.e. sunday=0)
	int yday;	// day of the year (0-365)
};

//converts a Unix timestamp to the struct tm
void unix_to_time(long ts, struct tm *t);

//formats the actual time as user wants
int strftime(char *buf, int max, const char *fmt, const struct tm *t);

#endif
