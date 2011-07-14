
all: unicc.out Makefile

clean:
	rm -f unicc.out

unicc.out: dates.par
	echo "$$ unicc -v -s dates.par" >$@
	unicc -v -s dates.par 2>>$@
	echo "$$ " >>$@

