#ifndef MONTHCELL_H_
#define MONTHCELL_H_

#include <String.h>
#include <ListView.h>
#include <ListItem.h>
#include <StringView.h>
#include "messageids.h"
extern "C"{
#include "gnome/calobj.h"
}
class EventListItem:public BStringItem{
public:
	EventListItem(BString UID,BString Summary,time_t StartTime);
	virtual ~EventListItem();
	BString UID()const{return mUID;}
	void SetSummary(BString Summary){mSummary=Summary; SetText(Summary.String());}
	time_t StartTime()const{return mStartTime;}
	void SetStartTime(time_t startTime){mStartTime=startTime;}
	virtual void DrawItem(BView *owner,BRect frame,bool complete=false);
	virtual void Update(BView *owner,const BFont *font);

protected:
	BString mUID,mSummary;
	time_t mStartTime;
};
class EventListView:public BListView{
public:
	EventListView(int CellNum,BRect frame,const char *name,list_view_type type = 
		B_SINGLE_SELECTION_LIST,uint32 resizingMode=B_FOLLOW_LEFT|B_FOLLOW_TOP,
		uint32 flags = B_WILL_DRAW|B_NAVIGABLE|B_FRAME_EVENTS);
	virtual void SelectionChanged();
private:
	int myCellNum; /* the cell I exist in, 0-42 */
};

class MonthCell;

class MonthCell:public BView{
public:
	MonthCell(int CellNum,BRect frame,int Day,bool isUsed);
	virtual ~MonthCell();
	virtual void FrameResized(float w,float h);
	virtual void Draw(BRect); 
	void SetUsed(bool flag);
	void SetDate(int day);
	void SetStartTime(time_t startTime);
	void SetDateToday(bool flag);
	int Day()const { return mDay; } 
	void DeleteEventItems();
	void AddEvent(BString UID,BString Summary,time_t StartTime); 
	void EditEvent(BString UID,BString Summary);
	void DeselectAll(){listView->DeselectAll();}
	BString UID(); /* returns to UID of the selected event */
protected:
	enum {DATE_BAR_HEIGHT = 15};
private:
	EventListItem *topItem; /* topitem is special, it belongs to the listView, but never holds an 
						  * event, instead, the user that select this object to create a new item */
	EventListView *listView;
	BHandler *mCalendarView; /* a pointer to the main message handler */	

	int mDay; 
	bool isUsed; 
	bool isToday; /* true if this cell is today's date */
	int myCellNum;  /* the cell # assigned to this cell */

};
#endif

