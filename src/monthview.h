#ifndef MONTHVIEW_H_
#define MONTHVIEW_H_

#include <time.h>
#include "baseview.h"
#include "StringView.h"
#include "colorstringview.h"
#include "monthcell.h"
#include "calobject.h"

class MonthView:public BaseView{
public:
	MonthView(BRect frame,CalObject *);
	virtual void Draw(BRect);
    virtual	void FrameResized(float w,float h);
	void SetDate(time_t date);
	void SetDate(int day,int month,int year);
	virtual void updateView();
  	virtual void changeEventDisplay(iCalObject *, int);
	int day2index(int day); /* returns the index 0-41 of the specified date */
	virtual void next();
	virtual void today();
	virtual void prev();
	virtual void selectionChanged(int newSelection);
	virtual void deleteEvent();

protected:
	BPoint CellPosition(int cell,BRect frame)const;
	BRect CellRect(int cell,BRect frame)const;
	int CellWidth(BRect frame)const;
	int CellHeight(BRect frame)const;
	BRect WeekDayRect(int day,BRect frame);
	void AddWeekdayTitle(int Day);
	void HighlightToday(); 
private:
	ColorStringView *MonthTitle;
	MonthCell *cells[42];
	ColorStringView *WeekTitle[7];
	int mSelectedCell; /* we can only select 1 item in all of 42 cells , 0-42, -1 for no cell selected */
	
	enum{MONTH_TITLE_HEIGHT = 25,WEEK_TITLE_HEIGHT=20};
	
	// state data.
	CalObject *myCal;
	int month; /* the month we are displaying */
	int year;  /* the year we are displaying */
	int day;   /* we want to highlight today's date */
	int dayOffset; /* months dont always start on sunday, add this offset */
	int daysInMonth;
	
	int DayOffset(int year,int month)const;
	BHandler *mHandler;
};

#endif
