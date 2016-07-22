#ifndef DATETIMEVIEW_H_
#define DATETIMEVIEW_H_

#include <TextView.h>

class DateView:public BTextView{
public:
	DateView(BRect frame,const char *name,BRect textRect,uint32 resizingMode,uint32 flags);
	void SetDate(time_t date);
	bool Date(time_t *time)const;
protected:
private:
};
class TimeView:public BTextView{
public:
	TimeView(BRect frame,const char *name,BRect textRect,uint32 resizingMode,uint32 flags);
	void SetTime(time_t time);
	bool Time(int *minutes)const;
protected:
private:

};


#endif