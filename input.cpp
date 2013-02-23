#include <Python.h>

#include <irrlicht.h>

#include <vector>

#include "input.h"
#include "cpywrapper.h"

using namespace irr;

InputHook::InputHook(Mod* m,InputType t,unsigned event,PyObject* hook):owner(m),type(t),callback(hook){
	if(t==KEY){
		key=(EKEY_CODE)event;
	}
	else if(t==MOUSE){
		mouse=(EMOUSE_INPUT_EVENT)event;
	}
}

InputHook::InputHook(Mod* m,InputType t,unsigned event,PyCFunction f):owner(m),type(t){
	struct PyMethodDef cdef={"<Anonymous C++ input hook>",f,METH_NOARGS,"An input hook function."};

	callback=PyCFunction_New(&cdef,NULL);

	if(t==KEY){
		key=(EKEY_CODE)event;
	}
	else if(t==MOUSE){
		mouse=(EMOUSE_INPUT_EVENT)event;
	}
}

Input::Input(){
	//Initialize key
	for(unsigned i=0;i<KEY_KEY_CODES_COUNT;++i){
		key[i]=false;
	}

	//Initialize mouse
	mouse.left=mouse.dleft=mouse.tleft=
		mouse.middle=mouse.dmiddle=mouse.tmiddle=
		mouse.right=mouse.dright=mouse.tright=false;
	mouse.wheel=0.0f;
	mouse.x=mouse.y=0;
}

bool Input::OnEvent(const SEvent& event){
	if(event.EventType==EET_MOUSE_INPUT_EVENT){
		switch(event.MouseInput.Event){
			//Mouse click events
			case EMIE_LMOUSE_PRESSED_DOWN:
				mouse.left=true;
				break;
			case EMIE_MMOUSE_PRESSED_DOWN:
				mouse.middle=true;
				break;
			case EMIE_RMOUSE_PRESSED_DOWN:
				mouse.right=true;
				break;
			case EMIE_LMOUSE_LEFT_UP:
				mouse.left=false;
				mouse.dleft=false;
				mouse.tleft=false;
				break;
			case EMIE_MMOUSE_LEFT_UP:
				mouse.middle=false;
				mouse.dmiddle=false;
				mouse.tmiddle=false;
				break;
			case EMIE_RMOUSE_LEFT_UP:
				mouse.right=false;
				mouse.dright=false;
				mouse.tright=false;
				break;
			//Mouse double click events
			case EMIE_LMOUSE_DOUBLE_CLICK:
				mouse.dleft=true;
				break;
			case EMIE_MMOUSE_DOUBLE_CLICK:
				mouse.dmiddle=true;
				break;
			case EMIE_RMOUSE_DOUBLE_CLICK:
				mouse.dright=true;
				break;
			//Mouse triple click events
			case EMIE_LMOUSE_TRIPLE_CLICK:
				mouse.tleft=true;
				break;
			case EMIE_MMOUSE_TRIPLE_CLICK:
				mouse.tmiddle=true;
				break;
			case EMIE_RMOUSE_TRIPLE_CLICK:
				mouse.tright=true;
				break;
			//Other mouse events
			case EMIE_MOUSE_MOVED:
				mouse.x=event.MouseInput.X;
				mouse.y=event.MouseInput.Y;
				break;
			case EMIE_MOUSE_WHEEL:
				mouse.wheel=event.MouseInput.Wheel;
				break;
			default:
				//Added so the compiler doesn't generate a warning
				break;
		}

		for(unsigned i=0;i<hooks.size();++i){
			switch(hooks[i].type){
				case MOUSE:
					if(hooks[i].mouse!=event.MouseInput.Event){
						continue;
					}
				case ALLMOUSE:
				case ALL:
					PyObject_CallObject(hooks[i].callback,NULL);
				default:
					break;
			}
		}
	}
	else if(event.EventType==EET_KEY_INPUT_EVENT){
		key[event.KeyInput.Key]=event.KeyInput.PressedDown;

		for(unsigned i=0;i<hooks.size();++i){
			switch(hooks[i].type){
				case KEY:
					if(hooks[i].key!=event.KeyInput.Key){
						continue;
					}
				case ALLKEY:
				case ALL:
					PyObject_CallObject(hooks[i].callback,NULL);
				default:
					break;
			}
		}
	}
	return false;
}
