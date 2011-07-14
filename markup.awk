BEGIN		{	FS=":"	
				img_count = 1
				tab_count = 1
			}

/IMAGE:/	{
				figtext = ""

				if( $3 != "" )
				{
					figtext = "Fig. " img_count ": " $3
					img_count++
				}

				html = "<center>"
				html = html "	<img src=\"" $2 "\" title=\"" \
							figtext "\" alt=\"This is an image showing " \
								figtext "\" /><br />\n"

				if( figtext )
					html = html figtext "\n"
				html = html "</center>\n"

				print html
				next
			}

/FIGURE:/	{
				print "Fig. " img_count ": " $2 "\n"
				img_count++
				next
			}
			
/TABLE:/	{
				print "<center>"
				print "Table " tab_count ": " $3 " [TAB_" $2 "]\n"
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
			}
