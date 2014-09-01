PANDOC=pandoc
HTML_FLAGS=--standalone --to=html5
DOCX_FLAGS=--to=docx
PDF_FLAGS=--to=latex
FLAGS=--from=markdown --smart
DOC=exercise2
SOURCES=$(DOC).md 
INCLUDES=kbi_interrupt.c kbi_interrupt.asm
PDF=$(SOURCES:.md=.pdf)
HTML=$(SOURCES:.md=.html)
WORD=$(SOURCES:.md=.docx)
TARGETS=$(HTML) $(PDF) $(WORD)

all: $(TARGETS)

$(DOC).html: $(DOC).md $(INCLUDES)
	$(PANDOC) $(HTML_FLAGS) $(FLAGS) $< -o $@

$(DOC).pdf: $(DOC).md $(INCLUDES)
	$(PANDOC) $(PDF_FLAGS) $(FLAGS) $< -o $@

$(DOC).docx: $(DOC).md $(INCLUDES)
	$(PANDOC) $(DOCX_FLAGS) $(FLAGS) $< -o $@


