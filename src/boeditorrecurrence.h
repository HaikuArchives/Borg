#ifndef BOEDITORRECURRENCE_H_
#define BOEDITORRECURRENCE_H_

#include <View.h>
#include "datetimeview.h"
extern "C"{
#include "gnome/calobj.h"
}

class BOEditorRecurrence:public BView{
public:

	BOEditorRecurrence(BRect frame);
	virtual ~BOEditorRecurrence();
	void setDefaults(time_t from,time_t to,bool allday);
	void readEvent(iCalObject *);
	void writeEvent(iCalObject *);
	
protected:
	
private:
	BHandler *editorHandler; 

};

#endif
