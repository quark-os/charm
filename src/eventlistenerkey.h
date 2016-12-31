#ifndef EVENTLISTENERKEY_H
#define EVENTLISTENERKEY_H

#include "eventlistener.h"
#include "keyboardevent.h"

class EventListenerKey: public EventListener
{
public:

	EventListenerKey();
	
	virtual void process(KeyboardEvent event);
			
private:
	
};

#endif
