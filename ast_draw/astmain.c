#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Utmp.h"

extern	_syminfo	_symbols[];
static int			semantics	= 0;
static int			types		= 0;
static int			begin_at	= 0;
static char*		typenames[]	=
	{
		"",
		"TERM:Character class",
		"TERM:String",
		"TERM:Regular expression"
		"TERM:System"
	};

void print_doc( _pcb* pcb, _syntree* start )
{
	_syntree* node;

	if( !start )
		return;
	
	for( node = start; node; node = node->next )
	{			
		if( start->parent )
			printf( "[ " );

		printf( "%s ", node->symbol.symbol->name );
		print_doc( pcb, node->child );

		if( start->parent )			
			printf( "] " );
		else
			printf( "\n" );
	}
}

void print_xml( _pcb* pcb, _syntree* start )
{
	_syntree* node;
	char*	begin;
	char*	end;

	if( !start )
		return;
		
	for( node = start; node; node = node->next )
	{
		printf( "<g>" );
	
		printf( "<text>" );

		if( types && node->symbol.symbol->type )
			printf( "(%s) ",
				typenames[ node->symbol.symbol->type ] );
		
		begin = "";
		end = "";
		switch( node->symbol.symbol->type )
		{
			case 1:
				begin = "'";
				end = "'";
				break;

			case 2:
				begin = "@";
				break;

			default:
				break;
		}

		printf( "%s%s%s", begin, node->symbol.symbol->name, end );

		if( semantics && node->token && *node->token )
			printf( " [%s]\n", node->token );

		printf( "</text>" );
		printf( "<line/>" );

		print_xml( pcb, node->child );
		printf( "</g>" );
	}
}

int main( int argc, char** argv )
{
	_pcb	pcb;
	char*	opt;
	int		i;
	int		doc		= 0;

	for( i = 1; i < argc; i++ )
	{
		if( *(argv[i]) == '-' )
		{
			opt = argv[i] + 1;
			if( *opt == '-' )
				opt++;

			if( !strcmp( opt, "semantics" ) )
				semantics = 1;
			else if( !strcmp( opt, "types" ) )
				types = 1;
			else if( !strcmp( opt, "doc" ) )
				doc = 1;
			else
				break;
		}
	}

	memset( &pcb, 0, sizeof( _pcb ) );
	pcb.eof = EOF;
	_parse( &pcb );

	if( doc == 0 )
		print_xml( &pcb, pcb.syntax_tree );
	else
		print_doc( &pcb, pcb.syntax_tree );

	_syntree_free( pcb.syntax_tree );
	
	return 0;
}
