#ifndef XPL_H
#define XPL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Virtual machine values */
typedef enum
{
	XPL_NULLVAL,							/* Nothing/Undefined */
	XPL_INTEGERVAL,							/* Integer type */
	XPL_FLOATVAL,							/* Float type */
	XPL_STRINGVAL							/* String type */
} xpl_datatype;

typedef struct
{
	xpl_datatype	type;					/* Value type */
	
	union
	{
		int			i;
		float		f;
		char*		s;
	} value;								/* Value storage union */
	
	char*			strval;					/* Temporary string value
												representation pointer */
} xpl_value;

/* Functions */
typedef struct
{
	char*			name;					/* Function name */
	int				min;					/* Minimal parameter count */
	int				max;					/* Maximal parameter count */
	xpl_value*		(*fn)( int, xpl_value** ); /* Function callback pointer */
} xpl_fn;

/* Virtual machine opcodes */
typedef enum
{
	XPL_NOP,								/* No operation */

	XPL_CAL,								/* Call function */
	XPL_LIT,								/* Load literal value */
	XPL_LOD,								/* Load value from variable */
	XPL_STO,								/* Store value into variable */
	XPL_JMP,								/* Jump to address */
	XPL_JPC,								/* Jump to address if true */

	XPL_EQU,								/* Check for equal */
	XPL_NEQ,								/* Check for not-equal */
	XPL_LOT,								/* Check for lower-than */
	XPL_LEQ,								/* Check for lower-equal */
	XPL_GRT,								/* Check for greater-than */
	XPL_GEQ,								/* Check for greater-equal */

	XPL_ADD,								/* Add or join two values */
	XPL_SUB,								/* Substract two values */
	XPL_MUL,								/* Multiply two values */
	XPL_DIV									/* Divide two values */
} xpl_op;

/* Command description */
typedef struct
{
	xpl_op		op;
	int			param;
} xpl_cmd;

/* Program structure */
typedef struct
{


	xpl_cmd*	program;
	int			program_cnt;

	xpl_value**	literals;
	int			literals_cnt;

	char**		variables;
	int			variables_cnt;

	xpl_fn*		functions;
	int			functions_cnt;
} xpl_program;

/* Runtime structure */
typedef struct
{
	xpl_value**	variables;

	xpl_value**	stack;
	int			stack_cnt;
	
	xpl_cmd*	ip;
} xpl_runtime;

/* Some general defines */
#define XPL_MALLOCSTEP						256

/* Import function prototypes */
#include "xpl.proto.h"

#endif
