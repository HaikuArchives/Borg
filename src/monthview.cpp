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
#include <ListItem.h>


#include "global.h"
#include "calendarview.h"
#include "monthview.h"
extern "C"{
#include "gnome/timeutil.h"
}


	
MonthView::MonthView(BRect frame,CalObject *cal):BaseView(frame,cal){

	mSelectedCell = -1;
	
	BRect rect = Bounds();
	myCal = cal;
	// add a title bar that displays the current month up top
	rect.bottom = MONTH_TITLE_HEIGHT - 1 ;
	rect.right = CellWidth(Bounds()) * 7 + 8 - 1;
	MonthTitle = new ColorStringView(rect,"monthtitle","October, 3199",B_FOLLOW_LEFT_RIGHT|B_FOLLOW_TOP,B_WILL_DRAW|B_FRAME_EVENTS);
	MonthTitle->SetAlignment(B_ALIGN_CENTER);
	MonthTitle->SetFontSize(18);
	MonthTitle->SetLowColor(0,100,0);
	MonthTitle->SetHighColor(255,255,255);
	MonthTitle->SetViewColor(0,100,0);
	AddChild(MonthTitle);

	int i;
	for (i=0;i<7;++i)
		AddWeekdayTitle(i);

	
	// now lets add the 42 cells
	for (i=0;i<42;++i){
		cells[i] = new MonthCell(i,CellRect(i,Bounds()),i+1,true);
		AddChild(cells[i]);	
		cells[i]->AttachedToWindow();
	}
	today();

}

void
MonthView::AddWeekdayTitle(int Day){
	char * DayName[] = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
	/* is that efficient? probably not */
	BRect frame = WeekDayRect(Day,Bounds());	
	WeekTitle[Day] = new ColorStringView(frame,"untitled",DayName[Day],B_FOLLOW_ALL_SIDES,B_WILL_DRAW|B_FRAME_EVENTS);
	WeekTitle[Day]->SetAlignment(B_ALIGN_CENTER);
	WeekTitle[Day]->SetFontSize(14);

	AddChild(WeekTitle[Day]);
	WeekTitle[Day]->SetViewColor(250,200,150);
	WeekTitle[Day]->SetLowColor(250,200,150);
	WeekTitle[Day]->SetHighColor(0,0,0);
}
void
MonthView::SetDate(time_t date_t){
	
	tm *date_tm = localtime(&date_t);
	month = date_tm->tm_mon;  /* month from 0-11 */
	year = date_tm->tm_year+1900;
	day = date_tm->tm_mday; 
	SetDate(day,month,year);
}

void
MonthView::SetDate(int day,int month,int year){
	char *MonthNames[12]={"JANUARY","FEBRUARY","MARCH","APRIL","MAY","JUNE","JULY",
						  "AUGUST","SEPTEMBER","OCTOBER","NOVEMBER","DECEMBER"};

	dayOffset = DayOffset(year,month);
	/* we need to determine how many days there are in this month , and tell the cells*/		
	daysInMonth = time_days_in_month(year,month); /*note, this func wants month from 0-11*/
	for (int i=0;i<42;++i){
			cells[i]->SetUsed((i-dayOffset) < daysInMonth && i>=dayOffset);
			cells[i]->SetDate(i+1 - dayOffset);
			cells[i]->SetDateToday(false);
	}
	cells[day+dayOffset-1]->SetDateToday(true);
	char buffer[30];
	snprintf(buffer,30,"%s, %d",MonthNames[month],year);
	MonthTitle->SetText(buffer);
	MonthTitle->SetAlignment(B_ALIGN_CENTER);
	MonthTitle->Invalidate();
	updateView();	
}
void
MonthView::Draw(BRect UpdateArea){
	
	/* lets draw black spacing lines between each of the cells */
	BRect frame = Bounds();
	rgb_color black;
	BeginLineArray(7+8);
	black.red = black.green = black.blue = 0;
	int right_border = CellWidth(frame) * 7 + 8 - 1 ;
	int bottom_border = (int)MONTH_TITLE_HEIGHT+(int)WEEK_TITLE_HEIGHT + CellHeight(frame) * 6 + 7 - 1; 
	for (int x=0;x<=right_border;x+=CellWidth(frame)+1)
		AddLine(BPoint(x,bottom_border),BPoint(x,frame.top),black);	
	for (int y=MONTH_TITLE_HEIGHT+WEEK_TITLE_HEIGHT ;y<=bottom_border;y+=CellHeight(frame)+1)
		AddLine(BPoint(0,y),BPoint(right_border,y),black);	
	EndLineArray();
	
	/* the month title needs a border around it, make sure it is aligned with the cells */

	MonthTitle->SetHighColor(0,0,0);
	MonthTitle->StrokeRect(MonthTitle->Bounds());
	MonthTitle->SetHighColor(255,255,255);
}
int
MonthView::CellWidth(BRect frame)const{
	int FrameWidth = frame.IntegerWidth() + 1;
	/* since we must have 8 pixels used as separators between each of the cells,
	 * we subtract 8 before calc 
	 */
	return ((FrameWidth - 8)/7);
}
int 
MonthView::CellHeight(BRect frame)const{
	int FrameHeight = frame.IntegerHeight() + 1 - MONTH_TITLE_HEIGHT - WEEK_TITLE_HEIGHT;
	/* In this case, there are 6 rows, so, we need 7 pixels as spacers */
	return ((FrameHeight - 7)/6);
}
void
MonthView::FrameResized(float w,float h){
	/* I hate the fact that beos api uses right - left as width and not the true width. 
	 * w and h are right - left, so add 1 to get true width and height.
	 */
	int i;
	// resize the individual day cells
	// we are using ints instead of floats to make sure the space between cells
	// is exact, no adding of fractions
	int cell_w = CellWidth(Bounds());
	int cell_h = CellHeight(Bounds());
	int right_border = cell_w * 7 + 8 - 1; /* the last pixel of the rightmost cell is located here */
	for (i=0;i<42;++i){
		cells[i]->ResizeTo(cell_w-1,cell_h-1);
		cells[i]->MoveTo(CellPosition(i,Bounds()));
	}
	// resize the month title stringview and box	
	/* hmmm, do ResizeTo() take the true width , or the silly right - left (-1) width?
	 * looks like its the later.
	 */
	MonthTitle->ResizeTo(right_border,MONTH_TITLE_HEIGHT-1);	
	MonthTitle->Invalidate();
	/* resize the weekday titles */
	for (i=0;i<7;++i){
		WeekTitle[i]->MoveTo((cell_w+1)*i+1,MONTH_TITLE_HEIGHT );
		WeekTitle[i]->ResizeTo(cell_w-1,WEEK_TITLE_HEIGHT-1);
		WeekTitle[i]->Invalidate();
	}
}

int 
MonthView::DayOffset(int year,int month)const{
	time_t month_start_t = time_month_begin(time_from_day(year,month,15));
	tm *month_start_tm = localtime(&month_start_t);
	return (month_start_tm->tm_wday);
}
void 
MonthView::updateView()
{
	tm date;
	memset(&date,0,sizeof(date));
	/* we need to make sure that all the cells have the correct summary listed in them */
	for (int cell=0;cell<42;++cell){
		cells[cell]->DeleteEventItems();
		date.tm_year = year-1900;
		date.tm_mon = month ;
		date.tm_sec = 1;
		date.tm_hour = 0;
		date.tm_min = 0;
		date.tm_isdst = -1; /* compute DST */
		date.tm_mday = cell-dayOffset+1;
		time_t temp = mktime(&date);	
		time_t dayStart = time_day_begin(temp);
		time_t dayEnd = time_day_end(temp);
		GList *	listStart=calendar_get_events_in_range(myCal->gnomeCalendar(),dayStart-1,dayEnd);
		for (GList *list = listStart;list;list=list->next){
	    	 CalendarObject *CO = (CalendarObject*)list->data;	    	
	    	 cells[cell]->AddEvent(BString(CO->ico->uid),BString(CO->ico->summary),CO->ico->dtstart);			
		} 
		cells[cell]->SetStartTime(dayStart);
    	if (listStart)
    		calendar_destroy_event_list(listStart);
	}
	Invalidate();
}

void 
MonthView::changeEventDisplay(iCalObject *, int)
{
  // this should be re-written to be much more efficient, but this
  // quick-and-dirty-hack gets the job done for right now.
  updateView();
}
BPoint
MonthView::CellPosition(int cell,BRect frame)const{
	BRect rect=CellRect(cell,frame);
	BPoint pt(rect.left,rect.top);
	return pt;
}

BRect
MonthView::CellRect(int cell,BRect frame)const{
	/* frame should usually be monthview->bounds() */
	int col = cell % 7;
	int row = cell / 7;
	int cell_w = CellWidth(frame);
	int cell_h = CellHeight(frame);
	BRect r;
	r.top = MONTH_TITLE_HEIGHT + WEEK_TITLE_HEIGHT + 1 + row*cell_h + row;
	r.bottom = r.top + cell_h - 1;
	r.left = col * cell_w + col + 1;
	r.right = r.left + cell_w - 1;
	return r;
}
BRect
MonthView::WeekDayRect(int day,BRect frame){
	int cell_w = CellWidth(frame);
	BRect r;
	r.top = MONTH_TITLE_HEIGHT;
	r.bottom = r.top + WEEK_TITLE_HEIGHT - 1;	
	r.left = day * cell_w + day + 1;
	r.right = r.left + cell_w - 1;
	return r;	
}
void
MonthView::next(){
	if (++month>11) {
		month = 0;
		++year;
	}
	SetDate(0,month,year);
}
void 
MonthView::prev(){
	if (--month<0){ 
		month = 11;
		--year;
	}
	SetDate(0,month,year);
}
void 
MonthView::today(){
	SetDate(time(NULL));
	
}
void
MonthView::HighlightToday(){
	
}
void
MonthView::selectionChanged(int newSelection){
	if (newSelection != mSelectedCell){
		if (mSelectedCell != -1)
			cells[mSelectedCell]->DeselectAll();
		mSelectedCell = newSelection;
	}
}
void
MonthView::deleteEvent(){
	// get the UID of the selected event
	// find * from UID
	BString UID;
	if (mSelectedCell != -1)
		UID = cells[mSelectedCell]->UID();
	if (UID == BString(""))
		return; // there is no event selected
	CalendarView *calView = Global::calendarView();
	calView->deleteEvent(UID);	
}


