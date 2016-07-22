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
#include "calendarview.h"
#include <String.h>
#include <time.h>
#include "timeutil.h"
#include "boeventeditor.h"
#include "monthcell.h" 

CalendarView::CalendarView(BRect frame):BView(frame,"viewname",B_FOLLOW_ALL_SIDES,B_WILL_DRAW|B_FRAME_EVENTS){
	mModified = false;
	Global::SetCalendarViewHandler(this);
	mCalendar = new CalObject();
	monthView = 0;
	/* lets setup a new calendar with today's date */
	monthView = new MonthView(Bounds(),mCalendar);
	
	AddChild(monthView);
	changeView(monthView);
}

CalendarView::~CalendarView(){
	mCalendar->close();
	delete mCalendar;
	mCalendar = 0;
	Global::SetBaseView(0);
}
void
CalendarView::newCalendar(){
	mCalendar->close();
	currentView->updateView();
	setModified(false);
}
bool
CalendarView::initCalendar(BString filename){
	setModified(false);

	if (!(mCalendar->load(filename))){
		BAlert *alert = new BAlert("Error loading file","Could not open the file","ok");
		alert->Go();
		mCalendar->close();
		return false;
	}	
	currentView->updateView();
	return true;
}
void
CalendarView::editEvent(iCalObject *anEvent){
	BOEventEditor *eventWin = new BOEventEditor(mCalendar);
	eventWin->Show();
	eventWin->editEvent(anEvent,0);
}
bool
CalendarView::isModified(){
	return mModified;
}
void
CalendarView::setModified(bool modified){
	mModified = modified;
	BWindow *win = Global::mainWindowHandler();
	BString title("BORG");
	if (modified){
		title << " (modified)";
		win->SetTitle(title.String());
	}else
		win->SetTitle(title.String());
	
	// the korganizer version of this uses the keyword
	// emit, ???
}
void
CalendarView::changeView(BaseView *view){
	Global::SetBaseView(view);
	currentView = view;
}

void
CalendarView::FrameResized(float w,float h){
//	currentView->ResizeTo();	
}
void
CalendarView::MessageReceived(BMessage *msg){
	switch(msg->what){
	case EVENT_EDITOR:
	{
		BListView *listView;
		if (msg->FindPointer("source",(void**)&listView)==B_OK){
			int32 index;
			if (msg->FindInt32("index",&index)==B_OK ) { /* remember the 0 index is reserved for the top bar*/
				if (index == 0){
					time_t startTime = ((EventListItem*)listView->FirstItem())->StartTime();
					newEvent(startTime,startTime+1);	
				}else{
					EventListItem *item = (EventListItem*)listView->ItemAt(index);
					if (item != NULL){
						iCalObject *event = calendar_object_find_event(mCalendar->gnomeCalendar(),item->UID().String());
						if (event) editEvent(event);
					}
				}
			}
		}
	}
	break;
	default:
		BView::MessageReceived(msg);
	break;
	}
}
void 
CalendarView::changeEventDisplay(iCalObject *which, int action){
	//  dateNavigator->updateView();
	/* we have a search dialog? 
	 * if (searchDlg)
     *    searchDlg->updateView();
	 */

  if (which) {
    // If there is an event view visible update the display
    if (currentView) currentView->changeEventDisplay(which,action);
  /* No todos yet...
     if (which->getTodoStatus()) {
      if (!currentView) todoView->updateView();
      todoList->updateView();
    }
  } else {*/
  
    if (currentView) currentView->updateView();
  }
	/* i dont think I like this function at all in its current implementation, someone clean it up */
    if (currentView) currentView->updateView();
	
}
void
CalendarView::eventAdded(iCalObject *event){
	changeEventDisplay(event,EVENTADDED);
}
void
CalendarView::eventChanged(iCalObject *event){
	changeEventDisplay(event,EVENTEDITED);
	setModified(true);
}
void
CalendarView::eventToBeDeleted(iCalObject *event){
	fprintf(stderr,"eventtobedeleted() not implemented.\n");
}
void
CalendarView::eventDeleted(){
	changeEventDisplay(0,EVENTDELETED);
}
void
CalendarView::goToday()
{
// QDateList tmpList;
//  QDate today(QDate::currentDate());
//  tmpList.append(&today);
//  dateNavigator->selectDates(tmpList);
//  saveSingleDate = QDate::currentDate();
//  updateView(dateNavigator->getSelected());
}

void 
CalendarView::goNext()
{
  // adapt this to work for other views
 // agendaView->slotNextDates();
  // this *appears* to work fine...
//  updateView(dateNavigator->getSelected());
}

void
CalendarView::goPrevious()
{
  // adapt this to work for other views
//  agendaView->slotPrevDates();
  // this *appears* to work fine...
  //updateView(dateNavigator->getSelected());
}

void 
CalendarView::newEvent(time_t fromHint, time_t toHint){
    // create empty event win
	BOEventEditor *eventWin = new BOEventEditor(mCalendar);
    // put in date hint
	eventWin->Lock();
	eventWin->newEvent(fromHint, toHint);
    // show win
	eventWin->Show();
	eventWin->Unlock();
}
bool
CalendarView::saveCalendar(BString filename){
	mCalendar->save(filename);
    // We should check for errors here.
    setModified(false);
    return true;
}
void
CalendarView::deleteEvent(BString UID){
	iCalObject *obj = calendar_object_find_event(mCalendar->gnomeCalendar(),UID.String());
	if (obj){
		calendar_remove_object(mCalendar->gnomeCalendar(),obj);
		eventDeleted();
	}
}

