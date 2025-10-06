#include "kernel/types.h"
#include "user/user.h"
#include "user/time.h"
#include "kernel/stat.h"

int is_leap(int year){
	return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

void unix_to_time(long ts, struct tm *t){
	t->sec = ts % 60;
	t->min = (ts / 60) % 60;
	t->hour = (ts / 3600) % 24;
	long days = ts / 86400;

	// finding the year, check if leap year. If so then subtract 366 days from total days,
	// and add one to t->year. Otherwise subtract 365 and t->year++. Do this until days is
	// less then the days in the year, this means we are in the current year.
	t->year = 1970;
	while(1){
		int leap = is_leap(t->year);
		int days_in_year = leap ? 366 : 365;
		if (days >= days_in_year){
			days -= days_in_year;
			t->year++;
		} else {
			break;
		}
	}

	t->yday = days; // days in the year

	// loops through each month and subtracts the days from the days left after finding the
	// year. Once the days left is less then the days in the month, break out we found our momth.
	// Also accounts for leap year by checking if we are checking february (i = 1) and if leap, 
	// then adds one day to february's dim
	int mdays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; //days per month
	int month;
	int leap = is_leap(t->year);
	for (month = 0; month < 12; month++){
		int dim = mdays[month];		// dim = days in month
		if (month == 1 && leap){
			dim = 29;
		}
		if (days >= dim){
			days -= dim;
		} else {
			break;
		}
	}
	t->month = month;
	
	// our days is from 0 (index) so we need to add one to get the actual days
	t->mday = days + 1;

	// adding 4 to days_total because unix epoch (Jan 1, 1970) was a thursday (day 4)
	// modulo 7 wraps around 0-6. 
	t->wday = (4 + days) % 7;
}

// needed a helper function to append numbers to buffers. had to create this, had internet help with this
void append_number(char *buf, int *pos, int value, int width){
	char tmp[16];
	int len = 0;

	do {
		tmp[len++] = '0' + (value %10);
		value /= 10;
	}while (value > 0);

	while (len < width) {
		tmp[len++] = '0';
	}

	for (int i = len - 1; i >= 0; i--){
		buf[(*pos)++] = tmp[i];
	}
}

// month lookup table
static const char *month_names[12] = {
	"January","February","March","April","May","June",
    "July","August","September","October","November","December"
};

// ALSO BORROWED
void itoa(int value, char *str, int width){
	int i = 0;

	if (value == 0){
		str[i++] = '0';
	} else {
		while (value > 0 && i < width){
			str[i++] = (value % 10) + '0';
			value /= 10;
		}
	}

	while (i < width) {
		str[i++] = '0';
	}

	str[i] = '\0';

	for (int j = 0, k = i - 1; j < k; j++, k--) {
		char tmp = str[j];
		str[j] = str[k];
		str[k] = tmp;
	}
}

// originally wanted to give user ability to choose what format but we have very strict
// availability in XV6 so had to resort to this style only. 
int strftime(char *buf, int max, const char *fmt,  const struct tm *t){
	//int pos = 0;
	//int len = 0;
	

	char temp[32];
	buf[0] = '\0';
	// TODO finish the switch case and add the new function signature to the time.h file
	for(const char *p = fmt; *p != '\0'; p++){
		if (*p == '%' && *(p + 1)) {
			p++;
			switch(*p){
				case 'Y': //YEAR
					itoa(t->year, temp, 4);
					break;
				case 'm': //MONTh
					itoa(t->month + 1, temp, 2);
					break;
				case 'd': //DAY
					itoa(t->mday, temp, 2);
					break;
				case 'H': //HOUR
					itoa(t->hour, temp, 2);
					break;
				case 'M': //MINUTE
					itoa(t->min, temp, 2);
					break;
				case 'S': //SECOND
					itoa(t->sec, temp, 2);
					break;
				case 'j': //DAY OF THE YEAR, i.e. day 46 of 365
					itoa(t->yday, temp, 2);
					break;
				case 'w': //MONTH IN WORD FORMAT
					strcpy(temp, month_names[t->month]);
					break;
				default:
					temp[0] = '%';
					temp[1] = *p;
					temp[2] = '\0';
			}

			if(strlen(buf) + strlen(temp) < max - 1){
				strcat(buf, temp);
			} else {
				break;
			}
		} else {
			// not a '%' followed by a char. Instead just copy said char
			int len = strlen(buf);
			if (len < max - 1) {
				buf[len] = *p;
				buf[len + 1] = '\0';
			} else {
				break;
			}
		}
	}
	
	return strlen(buf);

	//const char *m = month_names[t->month];
	//strcpy(buf, m);
	//pos += strlen(m);
	//buf[pos++] = ' ';

	// day
	//append_number(buf, &pos, t->mday, 2);
	//buf[pos++] = ',';
	//buf[pos++] = ' ';

	//year
	//append_number(buf, &pos, t->year, 4);
	//buf[pos++] = ' ';
	//buf[pos++] = ' ';

	// time
	//append_number(buf, &pos, t->hour, 2);
	//buf[pos++] = ':';
	//append_number(buf, &pos, t->min, 2);
	//buf[pos++] = ':';
	//append_number(buf, &pos, t->sec, 2);

	//buf[pos] = '\0';
	//return pos;
}

//int main(int argc, char *argv[]){
//	uint64 ts = time();
//	struct tm t;
//	unix_to_time((long)ts, &t);
	
//	char buf[128];
//	strftime(buf, 128, "%m-%d-%Y %H:%M:%S", &t);

	//printf("time and date is: %s\n", buf);
//	printf("%s\n", buf);
//	exit(0);
//}
