%.pdf : %.dvi
	dvipdf $< $@

%.dvi : %.tex
	latex $< -o $@