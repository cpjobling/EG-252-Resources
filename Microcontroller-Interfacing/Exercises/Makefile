PANDOC=pandoc
HTML_FLAGS=--standalone --to=html5
DOCX_FLAGS=--to=docx
PDF_FLAGS=--to=latex
FLAGS=--from=markdown --smart
DOC=overview
SOURCES=$(DOC).md 
PDF=$(SOURCES:.md=.pdf)
HTML=$(SOURCES:.md=.html)
WORD=$(SOURCES:.md=.docx)
TARGETS=$(HTML) $(PDF) $(WORD)

all: $(TARGETS)

$(DOC).html: $(DOC).md
	$(PANDOC) $(HTML_FLAGS) $(FLAGS) $< -o $@

$(DOC).pdf: $(DOC).md
	$(PANDOC) $(PDF_FLAGS) $(FLAGS) $< -o $@

$(DOC).docx: $(DOC).md
	$(PANDOC) $(DOCX_FLAGS) $(FLAGS) $< -o $@


