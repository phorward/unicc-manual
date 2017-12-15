
all: debug.out 

clean:
	rm -f debug.out

debug.out: debug.par
	echo "$$ unicc -w debug.par" >$@
	unicc -w debug.par >>$@
	echo "$$ cc -o debug -DUNICC_DEBUG=1 -DUNICC_STACKDEBUG=1 debug.c" >>$@
	cc -o debug -DUNICC_DEBUG=1 -DUNICC_STACKDEBUG=1 debug.c >>$@
	echo "$$ echo -n \"1+2*3\" | ./debug -s" >>$@
	echo -n "1+2*3" | ./debug -s >>$@ 2>&1
	echo "$$ " >>$@
	rm -f debug debug.c debug.h

