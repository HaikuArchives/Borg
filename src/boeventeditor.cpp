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


#include "boeventeditor.h"
#include "global.h"
#include "calendarview.h"

BOEventEditor::BOEventEditor(CalObject *calendar):BWindow(BRect(100,100,700,550),"Event Editor",
	B_DOCUMENT_WINDOW_LOOK,B_NORMAL_WINDOW_FEEL,B_NOT_RESIZABLE,B_CURRENT_WORKSPACE){
	
	mEvent = 0;
	mCalendar = calendar;
	BRect rect=Bounds();
	BTab *tab;
	rect.top = ICON_BAR_HEIGHT;
	
	tabView = new BTabView(rect,"tab view");
	rect = tabView->Bounds();

	tabView->SetViewColor(255,255,255);
	AddChild(tabView);

	mDetails = new BOEditorDetails(rect);
	mGeneral = new BOEditorGeneralEvent(rect,this);
	mRecurrence = new BOEditorRecurrence(rect);

	tab = new BTab;
	tabView->AddTab(mGeneral,tab);
	tab = new BTab;
	tabView->AddTab(mDetails,tab);
	recurrenceTab = new BTab;
	tabView->Select(0);
	Global::AttachButton(this,tabView,BString("images/save_and_close_up.bmp"),
		BString("images/save_and_close_down.bmp"),
		BRect(0,0,39,ICON_BAR_HEIGHT-1),SAVE_AND_CLOSE);
	Global::AttachButton(this,tabView,BString("images/exit2_up.bmp"),
		BString("images/exit2_down.bmp"),
		BRect(39,0,79,ICON_BAR_HEIGHT-1),EXIT_EVENT_EDITOR);

	Show(); 
	
	mGeneral->resetTargets();
}
BOEventEditor::~BOEventEditor(){

	/* the recurrence tab may or may not be attached */
	tabView->RemoveTab(2);
	delete recurrenceTab;			
}
bool
BOEventEditor::QuitRequested(){
	Quit();
	return true;
}
void
BOEventEditor::SaveAndClose(){
	iCalObject *event = 0;
	CalendarView *cal = (CalendarView *)Global::calendarView();
	BWindow *win = Global::mainWindowHandler();

    if (mEvent) event = mEvent;
    else event = ical_object_new();
	writeEvent(event);
	win->Lock();
	if (mEvent){
		cal->eventChanged(event);
	}else{
		mCalendar->addEvent(event);
		cal->eventChanged(event); /* maybe I should add an eventAdded() method? */
	}
	win->Unlock();

	if (mEvent)
		calendar_object_changed(mCalendar->gnomeCalendar(),event,CHANGE_ALL);
	cal->setModified(true);
	Quit();
}
void
BOEventEditor::editEvent(iCalObject *event,time_t date){
	mEvent = event;
	Lock();
	readEvent(event);
	Unlock();
}
void
BOEventEditor::readEvent(iCalObject *event){
	mDetails->readEvent(event);
	mGeneral->readEvent(event);
}
void
BOEventEditor::writeEvent(iCalObject *event){
	mDetails->writeEvent(event);
	mGeneral->writeEvent(event);
}
void
BOEventEditor::MessageReceived(BMessage *msg){
	switch(msg->what){
	case SAVE_AND_CLOSE:
		SaveAndClose();
	break;
	break;
	case RECURRING_EVENT_BUTTON_MSG:
	{
		// need to request button state
		BCheckBox *recurbox;
		if (msg->FindPointer("source",(void**)&recurbox)==B_OK){
			if (recurbox->Value()==B_CONTROL_ON){
				tabView->AddTab(mRecurrence,recurrenceTab);
				tabView->Invalidate();
			}else{
				tabView->RemoveTab(2);			
			}
		}
	}
	break;

	case EXIT_EVENT_EDITOR:
	case B_QUIT_REQUESTED:
		Quit();
	break;
	default:
		BWindow::MessageReceived(msg);
	break;
	}
}
void
BOEventEditor::newEvent(time_t from, time_t to, bool allDay){
	mEvent = 0;
	setDefaults(from,to,allDay);

}
void 
BOEventEditor::setDefaults(time_t from, time_t to, bool allDay)
{
	mGeneral->setDefaults(from,to,allDay);
    mDetails->setDefaults();
//  mRecurrence->setDefaults(from,to,allDay);

//  enableRecurrence(false);
}
