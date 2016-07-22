#ifndef BOEVENTEDITOR_H_
#define BOEVENTEDITOR_H_

#include <Window.h>
#include <TabView.h>
#include "calobject.h"

#include "boeditordetails.h"
#include "boeditorgeneralevent.h"
#include "boeditorrecurrence.h"
#include "global.h"
#include "messageids.h"


class BOEventEditor:public BWindow{
public:
	BOEventEditor(CalObject *calendar);
	virtual ~BOEventEditor();
	
    /** Clear eventwin for new event, and preset the dates and times with hint */
    void newEvent(time_t from, time_t to, bool allDay = FALSE );
	
	/** Edit an existing event. */
	void editEvent(iCalObject *,time_t date);
	
	/* read an event, setup ahem, widgets, accordingly */
	void readEvent(iCalObject *);
	void writeEvent(iCalObject *);
	virtual void MessageReceived(BMessage *);
	void SaveAndClose(); /* korganizer uses slotapply, huh? */
protected:
	
	virtual bool QuitRequested();
	void setDefaults(time_t from,time_t to,bool allDay);
private:
	enum {ICON_BAR_HEIGHT = 45};
	CalObject *mCalendar;
	iCalObject *mEvent;
	BTabView *tabView;
	BTab *recurrenceTab;
	
	BOEditorDetails *mDetails;
	BOEditorGeneralEvent *mGeneral;
	BOEditorRecurrence *mRecurrence;
};

#endif
