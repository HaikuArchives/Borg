
#include <time.h>
#include <String.h>
#include <stdio.h>
#include "datetimeview.h"

DateView::DateView(BRect frame,const char *name,BRect textRect,uint32 resizingMode,uint32 flags)
	:BTextView(frame,name,textRect,resizingMode,flags){
	
}
void
DateView::SetDate(time_t date){
	tm *tmTime = localtime(&date);
	int month = tmTime->tm_mon + 1;
	int year = tmTime->tm_year + 1900;
	int day = tmTime->tm_mday;	
	BString dateString ;
	dateString << " " << month << " / " << day << " / " << year;

	SetText(dateString.String());
}
bool  
DateView::Date(time_t *date_t)const{
	const char * text = Text();
	int month,year,day;
	if(sscanf(text,"%d / %d / %d",&month,&day,&year)!=3){
		return false;
	}else{
		tm date_tm;
		memset(&date_tm,0,sizeof(tm));
		date_tm.tm_mday = day;
		date_tm.tm_year = year - 1900;
		date_tm.tm_mon = month - 1;	
		date_tm.tm_isdst = -1;
		*date_t = mktime(&date_tm);
		return true;
	}
}

TimeView::TimeView(BRect frame,const char *name,BRect textRect,uint32 resizingMode,uint32 flags)
	:BTextView(frame,name,textRect,resizingMode,flags){
	
}

void
TimeView::SetTime(time_t time){
	tm *tmTime = localtime(&time);
	int hour = tmTime->tm_hour;
	int min  = tmTime->tm_min;
	BString ampm;
	/* here goes */
	if (hour == 0) {
		hour = 12; 
		ampm = "AM";
	}
	else if (hour < 12){
		ampm = "AM";
	}
	else if (hour == 12){
		ampm = "PM";	
	}
	else{
		hour -= 12;
		ampm = "PM";	
	}	
	char buffer[10];
	if (min <10)
		snprintf(buffer,10,"0%d",min);
	else
		snprintf(buffer,10,"%d",min);
	BString timeString;
	timeString << hour << " : " << buffer << " " << ampm; 
	
	SetText(timeString.String());
}
bool
TimeView::Time(int *minutes)const{
	const char * text = Text();
	int hour=-1,min=-1;
	unsigned char c;
	int NumScanned = sscanf(text,"%d : %d %c",&hour,&min,&c);
	if(NumScanned != 2 && NumScanned != 3){
		return false;
	}
	/* general bounds checking */
	if (hour < 1 || hour > 23)
		return false;
	if (min < 0 || min > 59 )
		return false;
	if (hour > 12){ /* military time */
		*minutes = hour*60 + min;
		return true;	
	}else{ /* need AM or PM */
		if (hour == 12) hour = 0;
		if (c == 'p' || c == 'P')
			hour += 12;	
		*minutes = hour*60 + min;
		return true;
	}	 	
}
