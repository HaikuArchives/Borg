#ifndef BORGANIZER_H_
#define BORGANIZER_H_

#include <Application.h>
#include <Window.h>
#include <MenuBar.h>
#include <Menu.h>
#include <MenuItem.h>
#include <FilePanel.h>
#include <Path.h>

#include <Button.h>

#include "messageids.h"
#include "calendarview.h"

class BOrganizer:public BWindow{

public:
	BOrganizer(BRect frame);
	virtual void FrameResized(float w,float h);

protected:
	enum{ICON_BAR_HEIGHT=40};	
	CalendarView *mcalendarView;
	BString mFile; // local name of calendar file
	BFilePanel *OpenFilePanel,*SaveFilePanel;
	
	/* loads and attaches a menubutton , returns 1 on sucess, 0 on failure*/
	
	// start a new borg 
	void file_new();

    /** open a file, load it into the calendar. */
	void file_open(BPath path);
	
	/* save a file */
	void file_save(BPath path);
	
	// exit the program , prompt for save if files are dirty
	void file_quit();

	// switch calendarview to month mode
	void switchto_monthview();
	
	// switch calendarview to year mode
	void switchto_yearview();
	
	// receives and processes menubar choices
	virtual void MessageReceived(BMessage *message);

	// overloaded BWindow::QuitRequested , handles...
	virtual bool QuitRequested();
	
	bool SavePrompt(); /* returns false if the user cancels and doesn't save the file */
	void DisplayAboutMessage();
	void DisplayLicense();
	void DisplayFeatureNotPresent(); /* pops up a msg box saying feature not implemented */
};
#endif