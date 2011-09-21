#include "xpl.h"

xpl_fn	xpl_buildin_functions[] =
{
	{	"print",	1,		-1,		XPL_print	}
};

int xpl_get_function( char* name )
{
	int		i;
	
	/* Try to find function */	
	for( i = 0; i < sizeof( xpl_buildin_functions ) / sizeof( xpl_fn ); i++ )
		if( strcmp( prog->functions[ i ].name, name ) == 0 )
			return i;
			
	return -1;
}

/* Build-in functions follow */

xpl_value* XPL_print( int argc, xpl_value** argv )
{
	int		i;
	for( i = 0; i < argc; i++ )
		printf( "%s\n", argv[ i ];
		
	return (xpl_value*)NULL;
}


