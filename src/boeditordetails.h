#ifndef BOEDITORDETAILS_H_
#define BOEDITORDETAILS_H_

#include <View.h>
#include "calobj.h"

class BOEditorDetails:public BView{
public:
	BOEditorDetails(BRect frame);
	virtual ~BOEditorDetails();
	void readEvent(iCalObject *event);
	void writeEvent(iCalObject *event);

	void setDefaults();

protected:
private:
};
#endif
