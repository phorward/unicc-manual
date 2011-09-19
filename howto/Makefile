
all: unicc.out conflicts.out Makefile

clean:
	rm -f unicc.out
	rm -f conflicts.out

unicc.out: dates.par
	echo "$$ unicc -v -s dates.par" >$@
	unicc -v -s dates.par 2>>$@
	echo "$$ " >>$@
	rm -f dates.c dates.h

conflicts.out: conflicts.par
	echo "$$ unicc -w conflicts.par" >$@
	unicc -w conflicts.par 2>>$@
	echo "$$ " >>$@
	rm -f conflicts.c conflicts.h
	
