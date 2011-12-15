
UNICC_MAIN	=	unicc.t2t
UNICC_CONT	=	contents.t2t

UNICC_SRC	=	$(UNICC_MAIN) \
				$(UNICC_CONT) \
				intro/intro.t2t \
				howto/howto.t2t \
				reference/reference.t2t \
				stdcparser/stdcparser.t2t \
				appendix1/appendix1.t2t \
				appendix2/appendix2.t2t \
				Makefile \
				markup.awk \
				book.html \
				unicc.css

UNICC_IMG	=	reference/*.png

UNICC_PDF	=	output/pdf/unicc.pdf
UNICC_PDF_H	=	output/pdf/unicc.pdf.html

UNICC_HTML	=	output/html/unicc.html

UNICC_TXT	=	output/text/unicc.txt

#all: $(UNICC_HTML) $(UNICC_PDF) $(UNICC_TXT)
all: $(UNICC_PDF) $(UNICC_TXT)

clean:
	rm -f $(UNICC_HTML)
	rm -f $(UNICC_PDF)
	rm -f $(UNICC_PDF_H)
	rm -f $(UNICC_TXT)

backup:
	tar cvf ../manual.tar ../manual

$(UNICC_PDF): $(UNICC_PDF_H)
	htmldoc -t pdf14 --book --linkstyle plain --linkcolor 000030 \
		--header .t. --no-title --duplex --browserwidth 1024 \
			--toclevels 4 --no-jpeg -f $@.tmp $(UNICC_PDF_H)
	pdftk book/front.pdf $@.tmp book/back.pdf cat output $@
	rm -f $@.tmp

$(UNICC_PDF_H): $(UNICC_SRC)
	txt2tags -o $@.tmp -t html --no-enum-title -H \
		--css-sugar --style unicc.css --css-inside \
			$(UNICC_CONT)
	awk -f markup.awk $@.tmp >$@
	rm -f $@.tmp
	mv $@ $@.tmp
	sed -e "/CONTENT/r $@.tmp" -e "/CONTENT/d" book.html >$@
	rm -f $@.tmp
	cp $(UNICC_IMG) output/pdf

$(UNICC_HTML): $(UNICC_SRC)
	txt2tags -o $@.tmp -t html --toc --toc-level=4 --mask-email \
		--css-sugar --style unicc.css --css-inside $(UNICC_MAIN)
	awk -f markup.awk $@.tmp >$@
	rm -f $@.tmp
	cp $(UNICC_IMG) output/html

$(UNICC_TXT): $(UNICC_SRC)
	txt2tags -o $@ -t art --toc --width 80 --height 25 $(UNICC_MAIN)
