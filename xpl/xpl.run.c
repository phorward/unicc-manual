#include "xpl.h"

static int xpl_push( xpl_runtime* rt, xpl_value* val )
{
	if( ( rt->stack_cnt % XPL_MALLOCSTEP ) == 0 )
	{
		rt->stack = (xpl_value*)xpl_malloc(
							(char*)rt->stack,
								( rt->stack_cnt + XPL_MALLOCSTEP )
									* sizeof( xpl_value ) );
	}
		
	prog->stack[ rt->stack_cnt ] = val;
	return rt->stack_cnt++;
}

static xpl_value* xpl_pop( xpl_runtime* rt )
{
	if( !rt->stack_cnt )
		return (xpl_value*)NULL;
		
	return rt->stack[ --rt->stack_cnt ];
}

void xpl_program_run( xpl_program* prog )
{
	xpl_runtime	rt;
	xpl_value*	val;
	int			i;
	
	/* Initialize runtime */
	memset( &rt, 0, sizeof( xpl_runtime ) );
	rt.variables = (xpl_value**)xpl_malloc( (char*)NULL,
						prog->variables_cnt * sizeof( xpl_value ) );

	rt.ip = prog->program;
	
	while( rt.ip < prog->program + prog->program_cnt )
	{
		switch( rt.ip->op )
		{
			case XPL_NOP:
				/* No nothing */
				break;

			case XPL_CAL:
				/* Calling build-in functions */
				{
					int		param_cnt;
					
					/* Last stack item contains the number of parameters */
					val = xpl_pop( rt );
					param_cnt = xpl_value_get_integer( val );
					xpl_value_free( val );
					
					/* Call the function */
					val = (*prog->functions[ rt.ip->param ].fn)(
							param_cnt, prog->stack +
											prog->stack_cnt - param_cnt );
					
					/* If no value is returned, create a default value */
					if( !val )
						val = xpl_value_create_integer( 0 );
						
					/* Discard the parameters from stack */
					while( param_cnt > 0 )
					{
						xpl_value_free( xpl_pop( rt ) );
						param_cnt--;
					}
					
					/* Push the return value */
					xpl_push( rt, val );
				}
				break;

			case XPL_LIT:
				/* Load literal and push duplicate */
				xpl_push( rt,
					xpl_value_dup( &( prog->literals[ rt.ip->param ] ) ) );
				break;

			case XPL_LOD:
				/* Load value from variable and push duplicate */
				xpl_push( rt,
					xpl_value_dup( &( prog->variables[ rt.ip->param ] ) ) );
				break;

			case XPL_STO:
				/* Store value to variable */
				if( rt->variables[ rt.ip->param ] )
					xpl_value_free( rt->variables[ rt.ip->param ] );
				
				rt->variables[ rt.ip->param ] = xpl_pop( rt );
				break;

			case XPL_JMP:
				/* Jump to address */
				ip = rt.ip->param;
				continue;

			case XPL_JPC:
				/* Jump to address only if stacked value is nonzero */
				if( xpl_value_get_integer( ( val = xpl_pop( rt ) ) ) )
				{
					xpl_value_free( val );
					ip = rt.ip->param;
					continue;
				}
				
				xpl_value_free( val );
				break;
								
			default:
				{
					xpl_datatype	prefer;
					xpl_value*		op	[ 2 ];
					
					/* Pop operands off the stack */
					op[1] = xpl_pop( rt );
					op[0] = xpl_pop( rt );
					
					/* 
					 * Get best matching type for operation from both operands 
					 */
					if( op[0]->type == XPL_STRINGVAL ||
							op[1]->type == XPL_STRINGVAL )
						prefer = XPL_STRINGVAL;
					else if( op[0]->type == XPL_FLOATVAL ||
								op[1]->type == XPL_FLOATVAL )
						prefer = XPL_FLOATVAL;
					else
						prefer = XPL_INTEGERVAL;
					
					switch( rt.ip->op )
					{
						case XPL_ADD:
							/* Addition, or with Strings, concatenation */
							if( prefer == XPL_STRINGVAL )
							{
								char*	str;
								
								str = xpl_malloc( (char*)NULL,
									( strlen( xpl_value_get_string( op[0] ) )
									+ strlen( xpl_value_get_string( op[1] ) ) 
									+ 1 ) * sizeof( char ) );
								
																
								sprintf( str, "%s%s",
									xpl_value_get_string( op[0] ),
									xpl_value_get_string( op[1] ) );
									
								val = xpl_value_create_string( str, 0 );
							}
							else if( prefer == XPL_FLOATVAL )
							{
								val = xpl_value_create_float(
										xpl_value_get_float( op[0] ) +
											xpl_value_get_float( op[1] ) );
							}
							else
							{
								val = xpl_value_create_integer(
										xpl_value_get_integer( op[0] ) +
											xpl_value_get_integer( op[1] ) );
							}
							break;

						case XPL_SUB:
							/* Substraction */
							if( prefer == XPL_FLOATVAL )
							{
								val = xpl_value_create_float(
										xpl_value_get_float( op[0] ) -
											xpl_value_get_float( op[1] ) );
							}
							else
							{
								val = xpl_value_create_integer(
										xpl_value_get_integer( op[0] ) -
											xpl_value_get_integer( op[1] ) );
							}
							break;

						case XPL_MUL:
							/* Multiplication */
							if( prefer == XPL_FLOATVAL )
							{
								val = xpl_value_create_float(
										xpl_value_get_float( op[0] ) *
											xpl_value_get_float( op[1] ) );
							}
							else
							{
								val = xpl_value_create_integer(
										xpl_value_get_integer( op[0] ) *
											xpl_value_get_integer( op[1] ) );
							}
							break;

						case XPL_DIV:
							/* Division */
							if( prefer == XPL_FLOATVAL )
							{
								val = xpl_value_create_float(
										xpl_value_get_float( op[0] ) /
											xpl_value_get_float( op[1] ) );
							}
							else
							{
								val = xpl_value_create_integer(
										xpl_value_get_integer( op[0] ) /
											xpl_value_get_integer( op[1] ) );
							}
							break;
							
						default:
							{
								float	res;
								
								/* 
								 * Compare by substracting the left operand
								 * from the right operand, or with the string
								 * comparion function strcmp, resulting in:
								 * 
								 * res == 0					equal
								 * res != 0 				not equal
								 * res < 0					lower than
								 * res <= 0					lower-equal
								 * res > 0					greater-than
								 * res >=0					greater-equal
								 */
								if( prefer == XPL_STRINGVAL )
								{
									res = (float)strcmp( 
											xpl_value_get_string( op[0] ),
											xpl_value_get_string( op[1] ) );
								}
								else if( prefer == XPL_FLOATVAL )
								{
									res = xpl_value_get_float( op[0] ) 
											- xpl_value_get_float( op[1] );
								}
								else
								{
									res = (float)xpl_value_get_integer( op[0] )
											- xpl_value_get_integer( op[1] );
								}
								
								/* Switch comparison */
								switch( rt.ip->op )
								{
									case XPL_EQU:
										val = xpl_value_create_integer(
												!res ? 1 : 0 );
										break;
									case XPL_NEQ:
										val = xpl_value_create_integer(
												res ? 1 : 0 );
										break;
									case XPL_LOT:
										val = xpl_value_create_integer(
												res < 0 ? 1 : 0 );
										break;
									case XPL_LEQ:
										val = xpl_value_create_integer(
												res <= 0 ? 1 : 0 );
										break;
									case XPL_GRT:
										val = xpl_value_create_integer(
												res > 0 ? 1 : 0 );
										break;
									case XPL_GEQ:
										val = xpl_value_create_integer(
												res >= 0 ? 1 : 0 );
										break;
								}
							}
						}
						
						/* Free the operands */
						xpl_value_free( op[0] );
						xpl_value_free( op[1] );
						
						/* Push the operation or comparison result */
						xpl_push( rt, val );
				}
				break;
		}
	}
	
	/* Clear stack */
	for( i = 0; i < rt->stack_cnt; i++ )
		xpl_value_free( rt->stack[ i ] );
		
	xpl_free( rt->stack );
	
	/* Clear variables */
	for( i = 0; i < prog->variables_cnt; i++ )
		xpl_value_free( rt->variables[ i ] );
	
	xpl_free( rt->variables );
}