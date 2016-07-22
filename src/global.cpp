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


#include "global.h"

BWindow * Global::mMainWindowHandler=0;
CalendarView * Global::mCalendarViewHandler=0;
BaseView * Global::mBaseView = 0;


int
Global::AttachButton(BWindow *window,BView *view,BString unclickedImageFile,BString clickedImageFile,BRect frame,int32 Message){
	
	BBitmap *clickedImage,*unclickedImage;
	BPicture clickedPicture,unclickedPicture;

	unclickedImage = BTranslationUtils::GetBitmapFile(unclickedImageFile.String());

	/* need at least the unclicked image */
	if (unclickedImage==NULL){
		return 0;
	}
	/* convert images into a BPicture */
	view->BeginPicture(&unclickedPicture);
	view->DrawBitmap(unclickedImage);
	view->EndPicture();

	clickedImage = BTranslationUtils::GetBitmapFile(clickedImageFile.String());
	if (clickedImage!=NULL){
		view->BeginPicture(&clickedPicture);
		view->DrawBitmap(clickedImage);
		view->EndPicture();
		window->AddChild(new BPictureButton(frame,"icon name",&unclickedPicture,&clickedPicture,new BMessage(Message),B_ONE_STATE_BUTTON));
	}	
	else
		window->AddChild(new BPictureButton(frame,"icon name",&unclickedPicture,&unclickedPicture,new BMessage(Message),B_ONE_STATE_BUTTON));
	
	// free BBitmaps
	delete unclickedImage;	
	delete clickedImage;
	return 1;
}
int
Global::Hide(BView *view){
	int i;
	for (i=0;i<100;++i){
		if (!view->IsHidden())
			view->Hide();
		else
			break;	
	}	
	return i;
}
int
Global::Show(BView *view){
	int i;
	for ( i=0;i<100;++i){
		if (view->IsHidden())
			view->Show();
		else
			break;	
	}	
	return i;
}