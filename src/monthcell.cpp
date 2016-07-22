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

#include <stdio.h>

#include "global.h"
#include "monthcell.h"

EventListItem::EventListItem(BString UID,BString Summary,time_t StartTime):BStringItem(Summary.String()){
	mSummary = Summary;
	mUID = UID;
	mStartTime = StartTime;
	
}
EventListItem::~EventListItem(){

}
void
EventListItem::DrawItem(BView *owner,BRect frame,bool complete){
//	if (((BListView*)owner)->FirstItem() == this)
//		if (IsSelected())
//			Deselect();

	BStringItem::DrawItem(owner,frame,complete);

}
void
EventListItem::Update(BView *owner,const BFont *font){
	BStringItem::Update(owner,font);
}

EventListView::EventListView(int CellNum,BRect frame,const char *name,list_view_type type,
		uint32 resizingMode,uint32 flags):BListView(frame,name,type,resizingMode,flags){
	myCellNum = CellNum;
}
void
EventListView::SelectionChanged(){
	/* we can only allow a single cell out of 42 to be selected 
	 * this version directly calls parent method. I wonder if it would be better to 
	 * simply throw a bmessage at monthview instead of deriving a blistview?? 
	 */

	BaseView *currentView = Global::baseView();
	currentView->selectionChanged(myCellNum);
}

MonthCell::MonthCell(int CellNum,BRect frame,int DayOfMonth,bool isCellUsed):BView(frame,"noname",B_FOLLOW_ALL_SIDES,B_WILL_DRAW|B_FULL_UPDATE_ON_RESIZE|B_FRAME_EVENTS){
	myCellNum = CellNum;
	mDay = DayOfMonth; 
	isToday = false;
	listView = 0;
	isUsed = isCellUsed;
	mCalendarView = (BHandler*)Global::calendarView();
	char buffer[3]; /*contains DayOfMonth in a string*/
	snprintf(buffer,3,"%d",DayOfMonth);
	/* setup the listview */
	BRect listViewRect = Bounds(); 
	listView = new EventListView(myCellNum,listViewRect,"name");
	listView->SetInvocationMessage(new BMessage(EVENT_EDITOR));
	AddChild(listView);
	/* setup the topItem , the reserved first item in the listView, users click on this to create a new event */
	topItem = new EventListItem(BString(""),buffer,0);
	listView->AddItem(topItem,0);
	/* Setup first item in listview */
	SetUsed(isUsed);
	SetDate(DayOfMonth);
	listView->SetTarget((BHandler*)mCalendarView);
	listView->SetFontSize(11);

}
MonthCell::~MonthCell(){
	DeleteEventItems();
	delete topItem;
}
void
MonthCell::AddEvent(BString UID,BString Summary,time_t StartTime){
	EventListItem *listItem = new EventListItem(UID,Summary,StartTime);
	listView->AddItem(listItem);

	listView->SetTarget((BHandler*)mCalendarView);

}
void
MonthCell::SetUsed(bool flag){
	isUsed = flag;
	if (isUsed){
		Global::Show(listView);
		SetViewColor(255,255,255);
	}
	else
	{
		Global::Hide(listView);
		SetViewColor(200,200,200);
	}
	Invalidate();
}
void 
MonthCell::SetDateToday(bool flag){
	isToday = flag;
	Invalidate();
}

void
MonthCell::FrameResized(float w,float h){
	listView->ResizeTo(w,h);
}
void
MonthCell::SetDate(int day){
	if (day <1 || day >31)	{
	}else{
		char buffer[3];
		snprintf(buffer,3,"%d",day);
		topItem->SetText(buffer);
		Invalidate();
	}
}
void
MonthCell::Draw(BRect rect){
}
void 
MonthCell::DeleteEventItems(){
	void *anItem;
	int i;
	/* do not delete/remove the first item */
	int NumItems = listView->CountItems()-1;

	for (i=0;i<NumItems;i++){
		anItem = listView->RemoveItem((int32)1);
		delete anItem;
	}
	listView->SetTarget((BHandler*)mCalendarView);
}
void 
MonthCell::EditEvent(BString UID,BString Summary){
	EventListItem *anItem;
	int i;
	for (i=0;(anItem=(EventListItem*)listView->ItemAt(i));i++){
		if (anItem->UID() == UID){
			anItem->SetSummary(Summary);
			break;
		}
	}
}
void
MonthCell::SetStartTime(time_t dayStart){
	topItem->SetStartTime(dayStart);
}
BString 
MonthCell::UID(){
	int item = listView->CurrentSelection();	
	if (item < 1)
		return BString(""); // return an MT bstring
	else
		return ((EventListItem*)listView->ItemAt(item))->UID();
}
