

text := 1.txt \
		2.txt \
		3.txt

DIRS := d1 \
		d2 \
		d3

src := /home/rongtao/1.c \
	   /home/rongtao/test/2.c \
	   /home/3.c

obj = $(patsubst %.c,%.o,$(src))

define func_deps
@echo "----------{"
@echo "$@"
@echo "----------}"
endef

all: d1 
	echo $(wordlist 1,2,$(text))
	echo $(obj)



$(DIRS):
	$(func_deps) 
	
