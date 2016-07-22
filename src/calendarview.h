#ifndef CALENDARVIEW_H_
#define CALENDARVIEW_H_

#include <View.h>
#include <Alert.h>

#include "baseview.h"
#include "monthview.h"
#include "calobject.h"
#include "messageids.h"


class CalendarView:public BView{
public:

    enum { EVENTADDED, EVENTEDITED, EVENTDELETED };

	CalendarView(BRect frame);
	virtual ~CalendarView();
	void changeView(BaseView *);
	virtual void FrameResized(float w,float h);
	bool initCalendar(BString filename);

    /** Save calendar data to file. */
    bool saveCalendar(BString filename);

	void newCalendar();
	void editEvent(iCalObject *anEvent);
	void newEvent(time_t fromHint,time_t toHint);
	virtual void MessageReceived(BMessage *message);
	
	/** passes on the message that an event has changed to the currently
     * activated view so that it can make appropriate display changes. */
 	void changeEventDisplay(iCalObject *, int);
	void eventAdded(iCalObject *);
  	void eventChanged(iCalObject *);
  	void eventToBeDeleted(iCalObject *);
  	void eventDeleted();
	void next(){currentView->next();}
	void prev(){currentView->prev();}
	void today(){currentView->today();}
	bool isModified();
	void setModified(bool);
	void deleteEvent(){currentView->deleteEvent();}
	void deleteEvent(BString UID);
	
protected:

    /** Move the current view date to today */
    void goToday();

    /** Move to the next date(s) in the current view */
    void goNext();

  	/** Move to the previous date(s) in the current view */
 	void goPrevious();
  

private:
	CalObject *mCalendar;
	BaseView *currentView;
	MonthView *monthView;
	bool mModified;
};
#endif
