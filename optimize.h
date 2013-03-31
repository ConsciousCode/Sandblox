#ifndef SANDBLOX_OPTIMIZE_H
#define SANDBLOX_OPTIMIZE_H

#include <Python.h>

const unsigned char
	SETUP_FINALLY=122,
	STORE_NAME=90,
	DELETE_GLOBAL=98,
	SETUP_EXCEPT=121,
	LOAD_GLOBAL=116,
	BUILD_SLICE=133,
	RAISE_VARARGS=130,
	INPLACE_OR=79,
	BINARY_LSHIFT=62,
	BUILD_TUPLE=102,
	SET_ADD=146,
	MAP_ADD=147,
	DELETE_NAME=91,
	INPLACE_MULTIPLY=57,
	RETURN_VALUE=83,
	STORE_MAP=54,
	DELETE_SUBSCR=61,
	BUILD_MAP=105,
	DUP_TOP_TWO=5,
	EXTENDED_ARG=144,
	INPLACE_RSHIFT=76,
	DELETE_ATTR=96,
	INPLACE_AND=77,
	LOAD_BUILD_CLASS=71,
	IMPORT_STAR=84,
	BINARY_TRUE_DIVIDE=27,
	MAKE_FUNCTION=132,
	BREAK_LOOP=80,
	YIELD_VALUE=86,
	YIELD_FROM=72,
	BINARY_FLOOR_DIVIDE=26,
	LOAD_DEREF= 136,
	DUP_TOP=4,
	UNPACK_SEQUENCE=92,
	COMPARE_OP=107,
	CALL_FUNCTION_VAR_KW=142,
	NOP=9,
	CONTINUE_LOOP=119,
	SETUP_LOOP=120,
	BINARY_MULTIPLY= 20,
	STORE_SUBSCR=60,
	INPLACE_XOR=78,
	WITH_CLEANUP=81,
	LOAD_CLOSURE=135,
	UNARY_POSITIVE=10,
	BUILD_SET=104,
	BINARY_MODULO=22,
	BINARY_AND=64,
	UNARY_INVERT=15,
	POP_JUMP_IF_FALSE=114,
	IMPORT_NAME=108,
	STORE_DEREF=137,
	LOAD_CONST=100,
	UNPACK_EX=94,
	POP_TOP=1,
	INPLACE_FLOOR_DIVIDE=28,
	POP_JUMP_IF_TRUE=115,
	ROT_THREE=3,
	MAKE_CLOSURE=134,
	LOAD_NAME=101,
	JUMP_IF_TRUE_OR_POP=112,
	INPLACE_POWER=67,
	STORE_FAST=125,
	LOAD_FAST=124,
	CALL_FUNCTION_VAR=140,
	BINARY_RSHIFT=63,
	GET_ITER=68,
	BINARY_OR=66,
	BUILD_LIST=103,
	FOR_ITER=93,
	PRINT_EXPR=70,
	INPLACE_SUBTRACT=56,
	POP_EXCEPT=89,
	CALL_FUNCTION_KW=141,
	STORE_LOCALS=69,
	BINARY_SUBTRACT=24,
	INPLACE_TRUE_DIVIDE=29,
	JUMP_IF_FALSE_OR_POP=111,
	UNARY_NEGATIVE=11,
	INPLACE_MODULO=59,
	UNARY_NOT=12,
	JUMP_FORWARD=110,
	LIST_APPEND=145,
	BINARY_POWER=19,
	ROT_TWO=2,
	JUMP_ABSOLUTE=113,
	BINARY_XOR=65,
	INPLACE_ADD=55,
	LOAD_ATTR=106,
	SETUP_WITH=143,
	BINARY_SUBSCR=25,
	BINARY_ADD=23,
	INPLACE_LSHIFT=75,
	CALL_FUNCTION=131,
	END_FINALLY=88,
	STORE_ATTR=95,
	DELETE_FAST=126,
	POP_BLOCK=87,
	STORE_GLOBAL=97,
	DELETE_DEREF=138,
	IMPORT_FROM=109;

/**
 * Perform peephole optimization on the given code object and return a new, optimized code object.
 *
 * @param code TThe code to optimize
**/
PyObject* peephole(PyObject* code);

#endif