
#log="echo if [ ! -d $1 ]; then echo $1; fi;"

define log
echo $1 $@
endef

SRCROOT=/home/rongtao/src
DEPROOT=/home/rongtao/deps

SRC = $(SRCROOT)/module1/a.c \
	  $(SRCROOT)/module2/b.c \
	  $(SRCROOT)/module3/src/c.c

DEPS :=  $(subst $(SRCROOT), $(DEPROOT), $(patsubst %.c,%.o,$(SRC)))

define SED_DEPS
echo $(DEPS)
endef

all:/home/rongtao/deps/module1/a.o
	$(call log, rong)
	$(call SED_DEPS)
	
$(DEPS):
	@echo ">>>$@"
	@echo ">>>$(subst $(DEPROOT), $(SRCROOT),$@)"
	@echo ">>> directory $(dir $(SRC))"
