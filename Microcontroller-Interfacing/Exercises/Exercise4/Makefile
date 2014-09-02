PANDOC=pandoc
HTML_FLAGS=--standalone --to=html5 \
					 --css=../../../stylesheets/stylesheet.css \
					 --template=../../../templates/html5.html
DOCX_FLAGS=--to=docx
PDF_FLAGS=--to=latex
FLAGS=--from=markdown --smart --filter includes.hs \
			-T="EG-252 Microcontroller Interfacing: Microcontroller Laboratory"

DOC=exercise4
SOURCES=$(DOC).md 
INCLUDES=tpm_motor.c tpm_timer.c
PDF=$(SOURCES:.md=.pdf)
HTML=$(SOURCES:.md=.html)
WORD=$(SOURCES:.md=.docx)
TARGETS=$(HTML) $(PDF) $(WORD)

all: $(TARGETS)

$(DOC).html: $(SOURCES) $(INCLUDES)
	$(PANDOC) $(HTML_FLAGS) $(FLAGS) $< -o $@

$(DOC).pdf: $(SOURCES) $(INCLUDES)
	$(PANDOC) $(PDF_FLAGS) $(FLAGS) $< -o $@

$(DOC).docx: $(SOURCES) $(INCLUDES)
	$(PANDOC) $(DOCX_FLAGS) $(FLAGS) $< -o $@


