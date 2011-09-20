BEGIN		{	FS="#"	
				img_count = 1
				tab_count = 1
				ast_count = 1
				
				pwd=""
			}

/GENAST#/	{
				astfile = "ast" ast_count
				ast_count++

				astpath = pwd "/" astfile

				system( "ast_draw/unicc2ast " $2 " \"" $3 "\" " astpath " " $4 )

				figtext = ""
				html = ""
				html = html "<center>"

				if( $5 != "" )
				{
					figtext = "Fig. " img_count ": " $5
					img_count++
				}

				html = html img( astfile ".png", figtext )

				if( figtext )
				{
					html = html "\n<br />" figtext "\n"
				}
				
				html = html "</center>"

				print html
				next
			}

/AST#/		{
				gsub( "AST", "", $1 )

				astfile = "ast" ast_count
				ast_count++

				astpath = pwd "/" astfile

				system( "ast_draw/mkast " astpath " " $2 )
				#print "ast_draw/mkast " astpath " " $2

				figtext = ""
				html = ""
				html = html "<center>"

				if( $3 != "" )
				{
					figtext = "Fig. " img_count ": " $3
					img_count++
				}

				html = html img( astfile ".png", figtext )

				if( figtext )
				{
					html = html "\n<br />" figtext "\n"
				}
				
				html = html "</center>"

				print $1 html $4
				next
			}

/IMAGE#/	{
				figtext = ""

				if( $3 != "" )
				{
					figtext = "Fig. " img_count ": " $3
					img_count++
				}

				html = "<center>"
				html = img( $2, figtext ) "<br />\n"

				if( figtext )
					html = html figtext "\n"
				html = html "</center>\n"

				print html
				next
			}

/FIGURE#/	{
				print "Fig. " img_count ": " $2 "\n"
				img_count++
				next
			}
			
/TABLE#/	{
				print "<center>"
				print "Table " tab_count ": " $3 "\n"
				print "</center>"

				tab_count++
				next
			}

/\/CENTER/	{
				print "</center>"
				next
			}

/CENTER/	{
				print "<center>"
				next
			}

			{	print

				if( pwd == "" )
					pwd = get_cmd( "dirname " FILENAME )
			}

function img( filename, figtext )
{
	filepath = pwd "/" filename
	#width = get_cmd( "convert " filepath " -verbose /dev/null | cut -d\\\  -f3 | cut -dx f1" )
	#height = get_cmd( "convert " filepath " -verbose /dev/null | cut -d\\\  -f3 | cut -dx f2" )
	width = get_cmd( "identify " filepath " | cut -d\\  -f3" )
	width = get_cmd( "echo " width " | cut -dx -f1" )
	height = get_cmd( "identify " filepath " | cut -d\\  -f3" )
	height = get_cmd( "echo " height " | cut -dx -f2" )

	if( figtext != "" )
		alt="This is an image showing " figtext

	return "<img src=\"" filename "\" title=\"" \
				figtext "\" alt=\"" alt "\" " \
					" width=\"" width "\" " \
						"height=\"" height "\" />"
}

function get_cmd( cmd )
{
	ret = ""
	cmd | getline ret
	close( cmd )
	return ret
}

