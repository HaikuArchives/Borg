#include "calobject.h"
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


CalObject::CalObject(){
	gCalendar = calendar_new("Empty Calendar",CALENDAR_INIT_NIL);

}
CalObject::~CalObject(){
	close();
}
bool
CalObject::load(const BString &filename){
	close();
	char *temp = new char[filename.Length()+1];
	filename.CopyInto(temp,0,filename.Length());
	temp[filename.Length()] = '\0';
	char *ErrorString = calendar_load(gCalendar,temp);
	
	delete [] temp;
	
	if (ErrorString == NULL){
		MakeLegal();
		return true;
	}
	else
		return false; /* print error string somewhere ?? - Riegger */
		
}
int
CalObject::save(const BString & filename){

	if (!gCalendar->filename)
		gCalendar->filename = g_strdup("");

	char *temp = new char[filename.Length()+1];
	filename.CopyInto(temp,0,filename.Length());
	temp[filename.Length()] = '\0';

	calendar_save(gCalendar,temp);
	delete [] temp;

	return 0;
}
void
CalObject::close(){
	if (gCalendar){
		calendar_destroy(gCalendar);
		gCalendar = 0;
	}
	gCalendar = calendar_new("Empty Calendar",CALENDAR_INIT_NIL);
}
/* MakeLegal() fixes a problem with get_events_in_range when only
 * the dtstart or dtend fields exist in the .vcs file.
 */
void
CalObject::MakeLegal(){
	/* for all events */
	iCalObject *obj;
	GList *list = gCalendar->events;
	for (;list;list=list->next){
		obj = (iCalObject*)list->data;
		if (obj->dtend==0){
			obj->dtend = obj->dtstart;
		}		
		if (obj->dtstart==0){
			obj->dtstart = obj->dtend;
		}		
	
	}
}
void
CalObject::addEvent(iCalObject *event){
	calendar_add_object(gCalendar,event);	
}
void
CalObject::newCalendar(){
	gCalendar = calendar_new("Empty Calendar",CALENDAR_INIT_NIL);

}
