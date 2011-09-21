
all: anomaly.out simple.xml simple.c w_sensitive.out no_lexeme.out lexeme.out error.out

clean:
	rm -f anomaly.out
	rm -f simple.xml
	rm -f simple.c
	rm -f simple.h
	rm -f w_sensitive.out
	rm -f no_lexeme.out
	rm -f lexeme.out
	rm -f error.out

anomaly.out: anomaly.par
	unicc -w -X anomaly.par 2>$@
	rm -f anomaly.xml

simple.xml: simple.par
	unicc -s -X simple.par
	cat simple.xml | sed -e "s/\t/    /g" | fold -s -w 80 >simple1.xml
	rm simple.xml
	mv simple1.xml simple.xml

simple.c: simple.par
	unicc -s simple.par
	cat simple.c | sed -e "s/\t/    /g" | fold -s -w 80 >simple1.c
	cat simple.h | sed -e "s/\t/    /g" | fold -s -w 80 >simple1.h
	rm simple.c simple.h
	mv simple1.c simple.c
	mv simple1.h simple.h

w_sensitive.out: w_sensitive.par
	echo "$$ unicc -G w_sensitive.par" >$@
	unicc -G w_sensitive.par >>$@
	echo "$$ " >>$@
	rm -f w_sensitive.c w_sensitive.h

no_lexeme.out: no_lexeme.par
	unicc -G no_lexeme.par >$@
	rm -f no_lexeme.c no_lexeme.h

lexeme.out: lexeme.par
	unicc -G lexeme.par >$@
	rm -f lexeme.c lexeme.h

error.out: error.par
	echo "$$ unicc -w error.par" >$@
	unicc -w error.par >>$@
	echo "$$ cc -o error error.c" >>$@
	cc -o error error.c >>$@
	echo "$$ echo -n \"2*5+x\" | ./error -s" >>$@
	echo -n "2*5+x" | ./error -s >>$@
	echo "$$ " >>$@
	rm -f error error.c error.h
