#include <Python.h>

#include <cctype>

#include "sandblox.h"
#include "../game.h"
#include "../util.h"

/**
 * Dictionary entry with a name and numeric value
**/
static struct NameValue{
	const char* name;
	const unsigned value;
};

/**
 * Dictionary of key event names and their Irrlicht numeric IDs
**/
static const NameValue key_names[]={
	//Letter keys
	{"A",KEY_KEY_A},{"B",KEY_KEY_B},{"C",KEY_KEY_C},{"D",KEY_KEY_D},{"E",KEY_KEY_E},
	{"F",KEY_KEY_F},{"G",KEY_KEY_G},{"H",KEY_KEY_H},{"I",KEY_KEY_I},{"J",KEY_KEY_J},
	{"K",KEY_KEY_K},{"L",KEY_KEY_L},{"M",KEY_KEY_M},{"N",KEY_KEY_N},{"O",KEY_KEY_O},
	{"P",KEY_KEY_P},{"Q",KEY_KEY_Q},{"R",KEY_KEY_R},{"S",KEY_KEY_S},{"T",KEY_KEY_T},
	{"U",KEY_KEY_U},{"V",KEY_KEY_V},{"W",KEY_KEY_W},{"X",KEY_KEY_X},{"Y",KEY_KEY_Y},
	{"Z",KEY_KEY_Z},

	//Arrow keys
	{"DOWN",KEY_DOWN},{"UP",KEY_UP},{"LEFT",KEY_LEFT},{"RIGHT",KEY_RIGHT},

	//Digit keys (top row)
	{"0",KEY_KEY_0},{"1",KEY_KEY_1},{"2",KEY_KEY_2},{"3",KEY_KEY_3},{"4",KEY_KEY_4},
	{"5",KEY_KEY_5},{"6",KEY_KEY_6},{"7",KEY_KEY_7},{"8",KEY_KEY_8},{"9",KEY_KEY_9},

	//Control keys
	{"LEFT SHIFT",KEY_LSHIFT},{"SHIFT",KEY_SHIFT},{"RIGHT SHIFT",KEY_RSHIFT},
	{"LEFT CTRL",KEY_LCONTROL},{"CTRL",KEY_CONTROL},{"RIGHT CTRL",KEY_RCONTROL},
	{"TAB",KEY_TAB},{"\t",KEY_TAB},
	{"ENTER",KEY_RETURN},{"\n",KEY_RETURN},
	{"SPACE",KEY_SPACE},{" ",KEY_SPACE},

	//Numpad keys
	{"NUMLOCK",KEY_NUMLOCK},{"NUM 0",KEY_NUMPAD0},{"NUM 1",KEY_NUMPAD1},{"NUM 2",KEY_NUMPAD2},
	{"NUM 3",KEY_NUMPAD3},{"NUM 4",KEY_NUMPAD4},{"NUM 5",KEY_NUMPAD5},{"NUM 6",KEY_NUMPAD6},
	{"NUM 7",KEY_NUMPAD7},{"NUM 8",KEY_NUMPAD8},{"NUM 9",KEY_NUMPAD9},

	//Misc keys
	{"BACK",KEY_BACK},{",",KEY_COMMA},{"DEL",KEY_DELETE},
	{"END",KEY_END},{"HOME",KEY_HOME},{"INSERT",KEY_INSERT},
	{"+",KEY_PLUS},{"-",KEY_MINUS},{"*",KEY_MULTIPLY},{"/",KEY_DIVIDE},

	//FN keys
	{"F0",KEY_F0},{"F1",KEY_F1},{"F2",KEY_F2},{"F3",KEY_F3},{"F4",KEY_F4},
	{"F5",KEY_F5},{"F6",KEY_F6},{"F7",KEY_F7},{"F8",KEY_F8},{"F9",KEY_F9},
	{"F10",KEY_F10},{"F11",KEY_F11},{"F12",KEY_F12},{"F13",KEY_F13},{"F14",KEY_F14},
	{"F15",KEY_F15},{"F16",KEY_F16},{"F17",KEY_F17},{"F18",KEY_F18},{"F19",KEY_F19},
	{"F20",KEY_F20},{"F21",KEY_F21},{"F22",KEY_F22},{"F23",KEY_F23},

	//Extra music shortcut keys
	{"PRIOR",KEY_PRIOR},{"PLAY",KEY_PLAY},{"PAUSE",KEY_PAUSE},{"NEXT",KEY_NEXT},
	{NULL}
};

/**
 * Dictionary of mouse event names and their Irrlicht numeric IDs
**/
static const NameValue mouse_names[]={
	{"MOVE",EMIE_MOUSE_MOVED},

	//Left
	{"LEFT DOWN",EMIE_LMOUSE_PRESSED_DOWN},{"LEFT UP",EMIE_LMOUSE_LEFT_UP},
	{"LEFT DOUBLE",EMIE_LMOUSE_DOUBLE_CLICK},{"LEFT TRIPLE",EMIE_LMOUSE_TRIPLE_CLICK},

	//Middle
	{"MIDDLE DOWN",EMIE_MMOUSE_PRESSED_DOWN},{"MIDDLE UP",EMIE_MMOUSE_LEFT_UP},
	{"MIDDLE DOUBLE",EMIE_MMOUSE_DOUBLE_CLICK},{"MIDDLE TRIPLE",EMIE_MMOUSE_TRIPLE_CLICK},
	{"WHEEL",EMIE_MOUSE_WHEEL},

	//Right
	{"RIGHT DOWN",EMIE_RMOUSE_PRESSED_DOWN},{"RIGHT UP",EMIE_RMOUSE_LEFT_UP},
	{"RIGHT DOUBLE",EMIE_RMOUSE_DOUBLE_CLICK},{"RIGHT TRIPLE",EMIE_RMOUSE_TRIPLE_CLICK},
	{NULL}
};

/**
 * Find a dictionary entry and return its value, or -1 on error.
**/
static int find_value(const NameValue* dict,const std::string& name){
	for(unsigned i=0;dict[i].name!=NULL;++i){
		if(name==dict[i].name){
			return dict[i].value;
		}
	}
	return -1;
}

PyObject* init_sandbloxmod(){
	PyObject* m=PyModule_Create2(&Sandblox_mod,PYTHON_API_VERSION);
	if(m==NULL){
		return NULL;
	}

	PyModule_AddObject(m,"version",Version_New(SANDBLOX_VERSION));
	PyModule_AddStringConstant(m,"engine",SANDBLOX_3DENGINE);

	return m;
}
