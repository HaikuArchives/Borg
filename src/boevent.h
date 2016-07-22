#ifndef BOEVENT_H_
#define BOEVENT_H_
/* NOTE-IT APPEARS BOEVENT WILL NOT BE USED, INSTEAD, gnome calendar structures*/
#include <String.h>
#include<ListItem.h>


class CalObject;


class BOEvent:public BListItem{
	friend CalObject;
public:
    /** number of events created */
    static int eventCount;

    /** enumeration for describing how an event recurs, if at all. */
    enum { rNone = 0, rDaily = 0x0001, rWeekly = 0x0002, rMonthlyPos = 0x0003,
     	 rMonthlyDay = 0x0004, rYearlyMonth = 0x0005, rYearlyDay = 0x0006 };
    /** enumeration for describing an event's status. */
    enum { NEEDS_ACTION = 0, ACCEPTED = 1, SENT = 2, TENTATIVE = 3,
   	   CONFIRMED = 4, DECLINED = 5, COMPLETED = 6, DELEGATED = 7 };
    /** enumeration for describing an event's secrecy. */
    enum { PUBLIC = 0, PRIVATE = 1, CONFIDENTIAL = 2 };
    /** structure for RecursMonthlyPos */
    struct rMonthPos {
      bool negative;
      short rPos;
      /*QBitArray rDays; - qbitarray unavail , using char[7] */
      unsigned char rDays[7];
    };
    /** constructs a new event with variables initialized to "sane" values. */
    BOEvent();
    ~BOEvent();

    /** Recreate event. The event is made a new unique event, but already stored
    event information is preserved. Sets uniquie id, creation date, last
    modification date and revision number. */
    void recreate();

protected:
	// data variables
	int id ; // globally unique ID for this event
	
//	PIM_datetime dtStart;  /* start time for the event */
//	PIM_datetime dtEnd;   /* end time for event. Events with only
						   /* ent time or start time take up "no space"  */
	BString description; /* detailed description of the event */
	BString summary;    /* summary of the event */
};

#endif