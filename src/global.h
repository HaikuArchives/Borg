#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <Window.h>
#include <Bitmap.h>
#include <PictureButton.h>
#include <String.h>
#include <TranslationUtils.h>
#include "baseview.h"
//#include "calendarview.h"

class CalendarView;

class Global{
public:
	static int AttachButton(BWindow *window,BView *view,BString unclickedImageFile,BString clickedImageFile,BRect frame,int32 Message);
	static void SetMainWindowHandler(BWindow *handler){mMainWindowHandler=handler;}
	static void SetCalendarViewHandler(CalendarView *handler){mCalendarViewHandler=handler;}
	static void SetBaseView(BaseView *baseView){mBaseView=baseView;}
	
	static   BWindow *mainWindowHandler(){return mMainWindowHandler;}
	static   CalendarView  *calendarView(){return mCalendarViewHandler;}
	static   BaseView *baseView() { return mBaseView; }
	
	static   int   Hide(BView *);
	static   int   Show(BView *);
private:
	Global();
	static BWindow *mMainWindowHandler;
	static CalendarView *mCalendarViewHandler;
	static BaseView *mBaseView;
};

#endif
