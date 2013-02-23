#ifndef SANDBLOX_INPUT_H
#define SANDBLOX_INPUT_H

#include <Python.h>

#include <irrlicht.h>

#include <vector>

using namespace irr;

/**
 * Types of events an InputHook can hook.
**/
enum InputType{
	ALL=0,
	ALLKEY,
	KEY,
	MOUSE,
	ALLMOUSE
};

/**
 * Represents a hook to a certain input.
**/
struct InputHook{
	/**
	 * The type of event to hook.
	**/
	InputType type;

	/**
	 * The key to watch for if it's a keyboard hook.
	**/
	EKEY_CODE key;
	/**
	 * The mouse event to watch for if it's a mouse hook.
	**/
	EMOUSE_INPUT_EVENT mouse;

	/**
	 * The function to call when the event happens.
	**/
	PyObject* callback;

	/**
	 * The mod that owns this hook.
	**/
	Mod* owner;

	/**
	 * Create an event hook.
	 *
	 * @param t The type of event to hook.
	 * @param event The specific event to hook (ignored for ALL, ALLKEY, and ALLMOUSE).
	 * @param hook The C function to call when the event happens.
	**/
	InputHook(Mod*,InputType,unsigned,void(*hook)());

	/**
	 * Create an event hook.
	 *
	 * @param t The type of event to hook.
	 * @param event The specific event to hook (ignored for ALL, ALLKEY, and ALLMOUSE).
	 * @param hook The function to call when the event happens.
	**/
	InputHook(Mod*,InputType,unsigned,PyObject*);
};

/**
 * Handles all user input.
**/
struct Input:public IEventReceiver{
	/**
	 * Input callbacks
	**/
	std::vector<InputHook> hooks;

	/**
	 * An array for accessing key states.
	**/
	bool key[KEY_KEY_CODES_COUNT];

	/**
	 * A structure representing the mouse state.
	**/
	struct{
		//Button states
		/**
		 * Whether or not the left mouse button is pressed.
		**/
		bool left;
		/**
		 * Whether or not the middle mouse button is pressed.
		**/
		bool middle;
		/**
		 * Whether or not the right mouse button is pressed.
		**/
		bool right;

		//Button double-click states
		/**
		 * Whether or not the left mouse button has been double clicked.
		**/
		bool dleft;
		/**
		 * Whether or not the middle mouse button has been double clicked.
		**/
		bool dmiddle;
		/**
		 * Whether or not the right mouse button has been double clicked.
		**/
		bool dright;

		//Button triple-click states
		/**
		 * Whether or not the left mouse button has been triple clicked.
		**/
		bool tleft;
		/**
		 * Whether or not the middle mouse button has been triple clicked.
		**/
		bool tmiddle;
		/**
		 * Whether or not the right mouse button has been triple clicked.
		**/
		bool tright;

		/**
		 * The rotation delta of the wheel
		**/
		f32 wheel;

		/**
		 * The x and y position components of the mouse cursor.
		**/
		s32 x,y;
	}mouse;

	Input();

	//IEventReceiver overload
	bool OnEvent(const SEvent& event);
};

#endif
