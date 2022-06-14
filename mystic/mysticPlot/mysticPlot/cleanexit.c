#ifdef CleanExit
		case ClientMessage:
		{
		Atom wm_protocols = XInternAtom(xg.mydisplay, "WM_PROTOCOLS", True);			
			if (myevent.xclient.message_type == wm_protocols)
			{
				if ((Atom) (myevent.xclient.data.l[0]) == myIcon->wm_delete_window)
				{
					if(myIcon->pClose){
					     (*myIcon->pClose)(myIcon);
					}
				}
			}

		}
		break;
#endif


#ifdef CleanExit
		case ClientMessage:
		{
		Atom wm_protocols = XInternAtom(xg.mydisplay, "WM_PROTOCOLS", True);			
			if (event.xclient.message_type == wm_protocols)
			{
				if ((Atom) (event.xclient.data.l[0]) == myIcon->wm_delete_window)
				{
		    		myIcon->done = -1;
				}
			}

		}
		break;
#endif


#ifdef CleanExit	
     p->wm_delete_window = XInternAtom(xg.mydisplay, "WM_DELETE_WINDOW", False);
    (void) XSetWMProtocols(xg.mydisplay, p->myWindow, &p->wm_delete_window, 1);
#endif	
