#ifndef BOEDITORGENERALEVENT_H_
#define BOEDITORGENERALEVENT_H_

#include <View.h>
#include <Box.h>
#include <CheckBox.h>
#include <RadioButton.h>
#include "calobj.h"
#include "messageids.h"
#include "datetimeview.h"

class BOEditorGeneralEvent:public BView{
public:
	BOEditorGeneralEvent(BRect frame,BHandler *handler);
	virtual ~BOEditorGeneralEvent();
	virtual void Draw(BRect);
	void readEvent(iCalObject *event);
	void writeEvent(iCalObject *event);
	virtual void MessageReceived(BMessage *msg);
	void setDefaults(time_t from,time_t to,bool allDay);
	void resetTargets();
protected:
    void setDateTimes(time_t start, time_t end);
	
private:
	BTextView *summaryEdit;
	BTextView *descriptionEdit;
	BBox *appointmentBox;
	DateView *startDateEdit;
	DateView *endDateEdit;
	TimeView *startTimeEdit;
	TimeView *endTimeEdit;
	BCheckBox *noTimeButton;	
	BCheckBox *recursButton;
	BBox *apptBox;
	BRadioButton *classButton[3];
	
	bool isNoTimeButtonChecked;

    // current start and end date and time
   	time_t currStartDateTime;
    time_t currEndDateTime;
};
#endif
