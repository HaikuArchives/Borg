/* I'm not sure I like this class. It encapsulates the C struct Calendar
 * but provides an extra layer of abstraction that I do not think is really
 * necessary - Riegger
 */

#ifndef CALOBJECT_H_
#define CALOBJECT_H_

#include <String.h>

extern "C"{
#include "gnome/calendar.h"
}

class CalObject{
public:
	CalObject();
	virtual ~CalObject();
   /** loads a calendar on disk in vCalendar format into the current calendar.
    * any information already present is lost. Returns TRUE if successful,
    * else returns FALSE.
    * @param fileName the name of the calendar on disk.
    */ 
	bool load(const BString &filename);
   /** writes out the calendar to disk in vCalendar format. Returns nonzero
    * on save error.
    * @param fileName the name of the file
    */
    int save(const BString &fileName);
   /** clears out the current calendar, freeing all used memory etc. etc. */
    void close();

 	void addEvent(iCalObject *event);
    
    Calendar * gnomeCalendar(){return gCalendar;} /* ugly, ugly, ugly , hey!*/
     
protected:
	void MakeLegal(); /* cleans up the gnome calendar */
	Calendar *gCalendar;

 	void newCalendar();
};
#endif
