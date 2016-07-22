#ifndef BASEVIEW_H_
#define BASEVIEW_H_

#include <View.h>
#include "calobject.h"

class BaseView:public BView{
public:
	BaseView(BRect frame,CalObject *);
	  /**
       * Updates the current display to reflect changes that may have happened
       * in the calendar since the last display refresh.
       */
    virtual void updateView() = 0;

	/**
 	 * Updates the current display to reflect the changes to one particular event.
   	 */
  	virtual void changeEventDisplay(iCalObject *, int) = 0;
	virtual void selectionChanged(int newSelection)=0;	
	virtual void deleteEvent()=0;
	virtual void next()=0;
	virtual void prev()=0;
	virtual void today()=0;

protected:
};
#endif
