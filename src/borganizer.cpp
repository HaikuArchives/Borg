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


#include "borganizer.h"
#include <View.h>

#include "global.h"
#include "boeventeditor.h"



BOrganizer::BOrganizer(BRect frame):BWindow(frame,"BORG - The Be Organizer",B_DOCUMENT_WINDOW,B_OUTLINE_RESIZE){
	OpenFilePanel = 0; SaveFilePanel = 0;
	Global::SetMainWindowHandler(this);
	
	BMenuBar *MenuBar;
	BMenu *menu;
	float MenuBarHeight;
	BRect Rect,WindowBounds = Bounds();
	Rect = Bounds();
	Rect.top = 0;
	Rect.bottom = 60;// only needed if BMenubar, resizetofit=false
	MenuBar = new BMenuBar(Rect,"menu bar");
	AddChild(MenuBar);
	
	// add file option to menubar
	menu = new BMenu("File");
	menu->AddItem(new BMenuItem("New",new BMessage(MENU_FILE_NEW)));
	menu->AddItem(new BMenuItem("Open",new BMessage(MENU_FILE_OPEN)));
	menu->AddItem(new BMenuItem("Save",new BMessage(MENU_FILE_SAVE)));
	menu->AddItem(new BMenuItem("Print",new BMessage(MENU_FILE_PRINT)));
	menu->AddItem(new BMenuItem("Quit",new BMessage(MENU_FILE_QUIT)));
	MenuBar->AddItem(menu);
	
	// add edit option to menubar
	menu = new BMenu("Edit");
	MenuBar->AddItem(menu);
	// add view option to menubar
	menu = new BMenu("View");
	MenuBar->AddItem(menu);
	// add actions option to menubar
	menu = new BMenu("Actions");
	menu->AddItem(new BMenuItem("Edit Appointment",new BMessage(MENU_ACTION_EDIT)));
	menu->AddItem(new BMenuItem("Delete Appointment",new BMessage(MENU_ACTION_DELETE)));
	MenuBar->AddItem(menu);
	

	// add options option to menubar
	menu = new BMenu("Options");
	MenuBar->AddItem(menu);
	
	// add help option to menubar
	menu = new BMenu("Help");
	menu->AddItem(new BMenuItem("About",new BMessage(MENU_HELP_ABOUT)));
	menu->AddItem(new BMenuItem("License",new BMessage(MENU_HELP_LICENSE)));

	MenuBar->AddItem(menu);

	Rect = MenuBar->Bounds();
	MenuBarHeight = Rect.Height() +1; 
	// add the mcalendarView view
	WindowBounds.top = MenuBarHeight+ICON_BAR_HEIGHT ; 
	// which we have not created yet
	mcalendarView = new CalendarView(WindowBounds);
	AddChild(mcalendarView);

	Global::AttachButton(this,mcalendarView,BString("images/new_up.bmp"),BString("images/new_down.bmp"),BRect(0,MenuBarHeight,39,MenuBarHeight+ICON_BAR_HEIGHT-1),BUTTON_NEW);
	Global::AttachButton(this,mcalendarView,BString("images/open_up.bmp"),BString("images/open_down.bmp"),BRect(40,MenuBarHeight,79,MenuBarHeight+ICON_BAR_HEIGHT-1),BUTTON_OPEN);
	Global::AttachButton(this,mcalendarView,BString("images/save_up.bmp"),BString("images/save_down.bmp"),BRect(80,MenuBarHeight,119,MenuBarHeight+ICON_BAR_HEIGHT-1),BUTTON_SAVE);
	Global::AttachButton(this,mcalendarView,BString("images/print_up.bmp"),BString("images/print_down.bmp"),BRect(120,MenuBarHeight,159,MenuBarHeight+ICON_BAR_HEIGHT-1),BUTTON_PRINT);
	Global::AttachButton(this,mcalendarView,BString("images/prev_generic_up.bmp"),BString("images/prev_generic_down.bmp"),BRect(160,MenuBarHeight,199,MenuBarHeight+ICON_BAR_HEIGHT-1),BUTTON_PREV);
//	Global::AttachButton(this,mcalendarView,BString("images/prev_month_up.bmp"),BString("images/prev_month_down.bmp"),BRect(160,MenuBarHeight,199,MenuBarHeight+ICON_BAR_HEIGHT-1),BUTTON_PREV);
	Global::AttachButton(this,mcalendarView,BString("images/today_up.bmp"),BString("images/today_down.bmp"),BRect(200,MenuBarHeight,239,MenuBarHeight+ICON_BAR_HEIGHT-1),BUTTON_TODAY);
	Global::AttachButton(this,mcalendarView,BString("images/next_generic_up.bmp"),BString("images/next_generic_down.bmp"),BRect(240,MenuBarHeight,279,MenuBarHeight+ICON_BAR_HEIGHT-1),BUTTON_NEXT);
//	Global::AttachButton(this,mcalendarView,BString("images/next_month_up.bmp"),BString("images/next_month_down.bmp"),BRect(240,MenuBarHeight,279,MenuBarHeight+ICON_BAR_HEIGHT-1),BUTTON_NEXT);


	file_new();
	Show();

}
void
BOrganizer::MessageReceived(BMessage *message){
	switch(message->what){
		case MENU_FILE_NEW:
		case BUTTON_NEW:
			file_new();
		break;
		case MENU_FILE_OPEN:
		case BUTTON_OPEN:
			if (!OpenFilePanel){
				OpenFilePanel = new BFilePanel(B_OPEN_PANEL,NULL,NULL,0,false);
				OpenFilePanel->SetTarget(BMessenger(this));
				OpenFilePanel->SetPanelDirectory("~");
				OpenFilePanel->SetMessage(new BMessage(FILEPANEL_OPEN));	
			}
			OpenFilePanel->Show();
		break;
		case MENU_FILE_SAVE:
		case BUTTON_SAVE:
			if (!SaveFilePanel){
				SaveFilePanel = new BFilePanel(B_SAVE_PANEL,NULL,NULL,0,false);
				SaveFilePanel->SetTarget(BMessenger(this));
				SaveFilePanel->SetMessage(new BMessage(FILEPANEL_SAVE));
				SaveFilePanel->SetPanelDirectory("~");
			}
			SaveFilePanel->Show();	
		break;
		case MENU_FILE_PRINT:
		case BUTTON_PRINT:
			DisplayFeatureNotPresent();
		break;\
		case MENU_FILE_QUIT:
			file_quit();
		break;
		case MENU_ACTION_EDIT:
			DisplayFeatureNotPresent();
		break;
		case MENU_ACTION_DELETE:
			mcalendarView->deleteEvent(); 
		break;
		case MENU_HELP_ABOUT:
			DisplayAboutMessage();
		break;
		case MENU_HELP_LICENSE:
			DisplayLicense();
		break;
		case FILEPANEL_OPEN:
		{
			entry_ref ref;
			message->FindRef("refs",0,&ref);
			BEntry entry(&ref);
			BPath path(&entry);		
			file_open(path);					
		}
		break;
		case BUTTON_NEXT:
			mcalendarView->next();
		break;
		case BUTTON_PREV:
			mcalendarView->prev();
		break;
		case BUTTON_TODAY:
			mcalendarView->today();
		break;
	
		case FILEPANEL_SAVE:
		{
			entry_ref ref;
			message->FindRef("directory",0,&ref);			
			BEntry directory(&ref);
			BString name;
			BPath path(&directory);
			message->FindString("name",&name);
			path.Append(name.String());
			file_save(path);
		}
		break;
		default:
			BWindow::MessageReceived(message);
		break;
	}
}
bool
BOrganizer::QuitRequested(){
	file_quit();
	return true;
}
bool
BOrganizer::SavePrompt(){
	if (mcalendarView->isModified()){
		BAlert *alert = new BAlert("Save calendar?","Calendar has been modified, do you wish to save first ?",
			"Cancel","Save",NULL,B_WIDTH_AS_USUAL,B_OFFSET_SPACING,B_WARNING_ALERT);
		if(alert->Go()==0)
			return false;
		else
			return true;
	}
	return true;
}
void
BOrganizer::file_new(){
    mcalendarView->newCalendar();
}
void 
BOrganizer::file_open(BPath path){
	mcalendarView->initCalendar(BString(path.Path()));
}
void
BOrganizer::file_save(BPath path){
	mcalendarView->saveCalendar(BString(path.Path()));
}
void 
BOrganizer::file_quit(){
	if (OpenFilePanel){
		delete OpenFilePanel;
		OpenFilePanel=0;
	}
	if (SaveFilePanel){
		delete SaveFilePanel;
		SaveFilePanel=0;
	}

	be_app->PostMessage(B_QUIT_REQUESTED);

}


void
BOrganizer::switchto_monthview(){

}
void 
BOrganizer::switchto_yearview(){

}
void
BOrganizer::FrameResized(float w,float h){
}
void 
BOrganizer::DisplayAboutMessage(){
	BString message = "BORG - The Be Organizer.\n\n";
	message += "License: GNU General Public License (GPL).\n";
	message += "Based upon KOrganizer.\n\n";
	message += "Contains code from: \n Gnome Calendar(www.gnome.org)\n and KOrganizer(devel-home.kde.org/~korganiz).\n\n";
	message += "Uses the BeOS port of glib (www.gtk.org).\n\n";
	message += "BORG was written by Michael Riegger.\n";
	message += "riegger@chaoticevil.com\n\n";
	message += "BORG icons were created by Mark Magennis.\n";
	message += "mark_magennis@fundy.net\n\n";
	message += "Source code available from:\n";
	message += "http://sourceforge.net/projects/borg.\n";
	message += "\n\"We only seek to better the lives of other species.\" - Locutus of Borg.";
	BAlert *aboutMsg = new BAlert("About the BORG",message.String(),"ok",NULL,NULL,B_WIDTH_AS_USUAL,
		B_INFO_ALERT);
	aboutMsg->SetShortcut(0, B_ESCAPE);
	aboutMsg->Go();
	fprintf(stdout,"ABOUT - %s\n\n",message.String());
}
void
BOrganizer::DisplayLicense(){
	system("StyledEdit borg/COPYING");
}	
void
BOrganizer::DisplayFeatureNotPresent(){
	BString message = "This feature has not been implemented yet!";
	BAlert *msg = new BAlert("SORRY!",message.String(),"ok",NULL,NULL,B_WIDTH_AS_USUAL,B_STOP_ALERT);
	msg->SetShortcut(0, B_ESCAPE);
	msg->Go();	
}

