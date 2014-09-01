PANDOC=pandoc
HTML_FLAGS=--standalone --to=html5
DOCX_FLAGS=--to=docx
PDF_FLAGS=--to=latex
FLAGS=--from=markdown --smart
DOC=exercise2
SOURCES=$(DOC).md 
INCLUDES=kbi_interrupt.c kbi_interrupt.asm
PDF=$(SOURCES:.md=.pdf) $(INCLUDES)
HTML=$(SOURCES:.md=.html) $(INCLUDES)
WORD=$(SOURCES:.md=.docx) $(INCLUDES)
TARGETS=$(HTML) $(PDF) $(WORD)

all: $(TARGETS)

$(DOC).html: $(DOC).md $(INCLUDES)
	$(PANDOC) $(HTML_FLAGS) $(FLAGS) $(DOC).md -o $@

$(DOC).pdf: $(DOC).md $(INCLUDES)
	$(PANDOC) $(PDF_FLAGS) $(FLAGS) $(DOC).md -o $@

$(DOC).docx: $(DOC).md $(INCLUDES)
	$(PANDOC) $(DOCX_FLAGS) $(FLAGS) $(DOC).md -o $@


