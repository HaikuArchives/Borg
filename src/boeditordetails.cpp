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


#include "boeditordetails.h"


BOEditorDetails::BOEditorDetails(BRect frame):BView(frame,"Details",B_FOLLOW_LEFT,B_WILL_DRAW){

}
BOEditorDetails::~BOEditorDetails(){

}
void
BOEditorDetails::readEvent(iCalObject *event){

}
void
BOEditorDetails::writeEvent(iCalObject *event){

}
void
BOEditorDetails::setDefaults(){

}