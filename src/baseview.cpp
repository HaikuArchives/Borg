#include "baseview.h"
#include "global.h"
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

BaseView::BaseView(BRect frame,CalObject *cal):BView(frame,"baseview",B_FOLLOW_ALL_SIDES,B_WILL_DRAW|B_FRAME_EVENTS|B_FULL_UPDATE_ON_RESIZE){
}