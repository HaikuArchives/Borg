#ifndef MESSAGEIDS_H_
#define MESSAGEIDS_H_

// these are the message IDs that can be received
// from BOrganizer's menubar, and menuitems
const uint32 MENU_FILE_NEW        = 'MFnw'; 
const uint32 MENU_FILE_OPEN       = 'MFop';
const uint32 MENU_FILE_SAVE       = 'MFsa';
const uint32 MENU_FILE_QUIT       = 'MFqu';
const uint32 MENU_FILE_PRINT      = 'MFpr';

const uint32 MENU_ACTION_EDIT     = 'MAed';
const uint32 MENU_ACTION_DELETE   = 'MAdl';

const uint32 MENU_HELP_ABOUT      = 'MHab';
const uint32 MENU_HELP_LICENSE    = 'MHli';

const uint32 BUTTON_TODAY         = 'BUto';
const uint32 BUTTON_NEXT          = 'BUne';
const uint32 BUTTON_PREV          = 'BUpv';
const uint32 BUTTON_OPEN 		  = 'BUop';
const uint32 BUTTON_SAVE 		  = 'BUsa';
const uint32 BUTTON_NEW  		  = 'BUnw';
const uint32 BUTTON_PRINT 		  = 'BUpr';

const uint32 FILEPANEL_OPEN		  = 'FPop';	
const uint32 FILEPANEL_SAVE       = 'FPsa';
const uint32 EVENT_EDITOR 		  = 'EVed';

/* the following are used by the event editor */
const int32 RECURRING_EVENT_BUTTON_MSG = 'REBu';
const int32 CHECK_BUTTON_MSG = 'ChBu';
const uint32 SAVE_AND_CLOSE = 'SACL';
const uint32 EXIT_EVENT_EDITOR = 'EXEE';


/* currently unused, I may change the implementation int he future */
const uint32 MONTHCELL_SELECTION_CHANGED = 'MCsc';
#endif
