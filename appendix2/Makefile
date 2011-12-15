
all: unicc.dtd.man

clean:
	rm -f unicc.dtd.man

unicc.dtd.man: unicc.dtd
	cat unicc.dtd | sed -e "s/\t/    /g" | fold -s -w 80 >unicc.dtd.man

