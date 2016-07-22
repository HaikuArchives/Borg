#ifndef COLORSTRINGVIEW_H_
#define COLORSTRINGVIEW_H_

/* EXACTLY like a BStringView, except that attachedtowindow is overridden
 * so it doesnt switch to the windows color scheme
 */
 

#include <StringView.h>
class ColorStringView:public BStringView{
public:
	ColorStringView(BRect frame,const char *name,const char *text,uint32 resizingMode
			=B_FOLLOW_LEFT|B_FOLLOW_TOP,uint32 flags=B_WILL_DRAW); 
	virtual void AttachedToWindow();
};

#endif
