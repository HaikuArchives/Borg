/*
	BORG - The Be Organizer. A calendar / organizer based on KDE's KOrganizer.
	Copyright (c) 2000 Michael Riegger (riegger@chaoticevil.com)
	
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by 
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.
	
	This program is distributed in the hope that it will be useful, but 
	WITHOUT ANY WARRANTY; without even the implied warranty of 
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	General Public License for more details.
	
	Your should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software Foundation, 
	Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include "boeditorgeneralevent.h"
#include <String.h>
#include "global.h"
extern "C"{
#include "timeutil.h"
}

/* right from gnome calendar , what is the value of these fields ??*/
static char *class_names [] = { "PUBLIC", "PRIVATE", "CONFIDENTIAL" };



BOEditorGeneralEvent::BOEditorGeneralEvent(BRect frame,BHandler *handler)
	:BView(frame,"General",B_FOLLOW_LEFT,B_WILL_DRAW){
	
	
	SetViewColor(200,200,200);
	BBox *box,*innerBox;
	BRect r,innerRect;

	/* Setup a summary editor */
	r = Bounds(); r.InsetBy(4,0); r.top = 0; r.bottom = 40;	
	box = new BBox(r);
	box->SetLabel("Summary");
	r = box->Bounds();
	r.InsetBy(3,3);  r.top += 10; 
	summaryEdit = new BTextView(r,"",r,B_FOLLOW_LEFT,B_WILL_DRAW);
	box->AddChild(summaryEdit);
	AddChild(box);
	Global::Show(summaryEdit);
	summaryEdit->SetWordWrap(false);
	
	/* set up the appointment time fields */
	r = Bounds(); r.InsetBy(4,0);r.top = 45; r.bottom = 130;
	box =  new BBox(r);
	box->SetLabel("Appointment Time");

	/* (Inside the appt time bbox) - setup the date fields */
	r = BRect(110,20,210,45);
	innerBox = new BBox(r);
	innerRect = innerBox->Bounds(); innerRect.InsetBy(3,3);
	startDateEdit = new DateView(innerRect,"",innerRect,B_FOLLOW_LEFT,B_WILL_DRAW);
	innerBox->AddChild(startDateEdit);
	box->AddChild(innerBox);
	Global::Show(startDateEdit);

	r = BRect(110,50,210,75);
	innerBox = new BBox(r);
	innerRect = innerBox->Bounds(); innerRect.InsetBy(3,3);
	endDateEdit = new DateView(innerRect,"",innerRect,B_FOLLOW_LEFT,B_WILL_DRAW);
	innerBox->AddChild(endDateEdit);
	box->AddChild(innerBox);
	Global::Show(endDateEdit);
	apptBox = box;


	/* (Inside the appt time bbox) - setup the time fields */
	r = BRect(290,20,390,45);
	innerBox = new BBox(r);
	innerRect = innerBox->Bounds(); innerRect.InsetBy(3,3);
	startTimeEdit = new TimeView(innerRect,"",innerRect,B_FOLLOW_LEFT,B_WILL_DRAW);
	innerBox->AddChild(startTimeEdit);
	box->AddChild(innerBox);
	Global::Show(startTimeEdit);

	r = BRect(290,50,390,75);
	innerBox = new BBox(r);
	innerRect = innerBox->Bounds(); innerRect.InsetBy(3,3);
	endTimeEdit = new TimeView(innerRect,"",innerRect,B_FOLLOW_LEFT,B_WILL_DRAW);
	innerBox->AddChild(endTimeEdit);
	box->AddChild(innerBox);
	Global::Show(endTimeEdit);
	/* (inside the appt time box) - add "no time associated" check box */

	noTimeButton = new BCheckBox(BRect(400,20,590,45),"","No time associated",new BMessage(CHECK_BUTTON_MSG));
	box->AddChild(noTimeButton);
	
	/* (inside the appt time box) - add the recursion on/off button */
	
	recursButton = new BCheckBox(BRect(400,50,590,75),"","Recurring event",new BMessage(RECURRING_EVENT_BUTTON_MSG));
	recursButton->SetTarget(handler);
	box->AddChild(recursButton);
	
	/* finally add the encapsulating appointment box */
	AddChild(box);	

	/* set up the dessciption edit view */
	r = Bounds(); r.InsetBy(4,0); r.top = 200; r.bottom = 300;
	box = new BBox(r);
	r = box->Bounds();
	r.InsetBy(2,2); r.top += 10;
	descriptionEdit = new BTextView(r,"",r,B_FOLLOW_LEFT,B_WILL_DRAW);
	box->AddChild(descriptionEdit);
	box->SetLabel("Description");
	AddChild(box);
	Global::Show(descriptionEdit);

	/* set up the classification buttons */
	r = BRect(150,310,420,360);
	box = new BBox(r);
	box->SetLabel("Classification");
	r = BRect(15,20,15+56-1,40);
	/* width 56, height 17 */
	box->AddChild(classButton[0]=new BRadioButton(r,"",class_names[0],new BMessage(12345)));
	classButton[0]->SetValue(1); /* default to PUBLIC */
	r = BRect(15+56+10,20,15+56+10+63-1,40);
	/* width 63, height 17 */
	box->AddChild(classButton[1]=new BRadioButton(r,"",class_names[1],new BMessage(12345)));

	r = BRect(15+20+56+63,20,15+20+56+63+93-1,40);
	/* width 93,height 17 */
	box->AddChild(classButton[2]=new BRadioButton(r,"",class_names[2],new BMessage(12345)));

	AddChild(box);
}
BOEditorGeneralEvent::~BOEditorGeneralEvent(){

}
void
BOEditorGeneralEvent::resetTargets(){
	noTimeButton->SetTarget(this);
	 
}
void
BOEditorGeneralEvent::readEvent(iCalObject *event){
	for (int i=0;i<3;++i){
		if (g_strcasecmp(event->classtype,class_names[i])==0){
			classButton[i]->SetValue(1);
			break;
		}
	}
	summaryEdit->SetText(event->summary);
	descriptionEdit->SetText(event->comment);

	startDateEdit->SetDate(event->dtstart);
	endDateEdit->SetDate(event->dtend);

	startTimeEdit->SetTime(event->dtstart);
	endTimeEdit->SetTime(event->dtend);
}
void
BOEditorGeneralEvent::writeEvent(iCalObject *event){
	g_free(event->summary);
	event->summary = g_strdup (summaryEdit->Text());

	g_free(event->comment);
	event->comment = g_strdup (descriptionEdit->Text());

/* the following fields have no intelligence behind them , please fix */
	g_free(event->status);
	event->status = g_strdup ("NEEDS ACTION");

	g_free(event->classtype);
	for (int i=0;i<3;++i){
		if (classButton[i]->Value()==B_CONTROL_ON){
			event->classtype = g_strdup (class_names[i]);
			break;
		}
	}

	g_free(event->location);
	event->location = g_strdup ("DUMMY_LOCATION_FIELD_FIX!!!");

	g_free(event->organizer);
	event->organizer = g_strdup ("");

	g_free(event->rstatus);
	event->rstatus = g_strdup ("DUMMY_RSTATUS_FIELD_FIX!!!");

	if (!startDateEdit->Date(&event->dtstart))
		event->dtstart = currStartDateTime;

	if (!endDateEdit->Date(&event->dtend))
		event->dtend = currEndDateTime;

	if (noTimeButton->Value()==B_CONTROL_ON){
			
	}else{
		int minutes;
		if (startTimeEdit->Time(&minutes))
			event->dtstart = time_add_minutes(event->dtstart,minutes);
						
		if (endTimeEdit->Time(&minutes))
			event->dtend = time_add_minutes(event->dtend,minutes);
	}
	
}
void
BOEditorGeneralEvent::Draw(BRect updateRect){
	Global::Show(summaryEdit);			
	Global::Show(descriptionEdit);			
	Global::Show(startDateEdit);
	Global::Show(endDateEdit);
	
	apptBox->DrawString("Start Time:",BPoint(35,37));
	apptBox->DrawString("  End Time:",BPoint(35,68));

}
void
BOEditorGeneralEvent::MessageReceived(BMessage *msg){
	switch(msg->what)
	{
	case CHECK_BUTTON_MSG:
		if (noTimeButton->Value()==B_CONTROL_ON){
			Global::Hide(startTimeEdit);
			Global::Hide(endTimeEdit);
		}else{
			Global::Show(startTimeEdit);
			Global::Show(endTimeEdit);
		}
	break;
	default:
		BView::MessageReceived(msg);
	break;
	}
}
void 
BOEditorGeneralEvent::setDefaults(time_t from,time_t to,bool allDay)
{
//  ownerLabel->setText(i18n("Owner: ") + KOPrefs::instance()->mName);

	BFont niceFont;
	niceFont.SetSize(16);
	summaryEdit->SetFont(&niceFont);


	if (allDay)
 		noTimeButton->SetValue(B_CONTROL_ON);
	else
 		noTimeButton->SetValue(B_CONTROL_OFF);

  //timeStuffDisable(allDay);
//  alarmStuffDisable(allDay);

    setDateTimes(from,to);
//  recursButton->setChecked(false);
/* no alarms in borganizer just yet
  QString alarmText(QString::number(KOPrefs::instance()->mAlarmTime));
  int pos = alarmText.find(' ');
  if (pos >= 0)
    alarmText.truncate(pos);
  alarmTimeEdit->setText(alarmText.data());
  alarmStuffEnable(false);
  */
}


void 
BOEditorGeneralEvent::setDateTimes(time_t start, time_t end)
{
//  qDebug("KOEditorGeneralEvent::setDateTimes(): Start DateTime: %s",
//         start.toString().latin1());
/* FIX!!!
  startDateEdit->setDate(start.date());
  startTimeEdit->setTime(start.time());
  endDateEdit->setDate(end.date());
  endTimeEdit->setTime(end.time());
*/
	startDateEdit->SetDate(start);
	endDateEdit->SetDate(end);	
	
	startTimeEdit->SetTime(0);
	endTimeEdit->SetTime(0);

    currStartDateTime = start;
    currEndDateTime = end;
}


