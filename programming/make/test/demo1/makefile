


root=$(PWD)
objdir=$(root)/obj

dir:=$(root)/src1
dir+=$(root)/src2

src:=$(root)/src1/1.c
src+=$(root)/src2/2.c
src+=$(root)/src3/3.c

libs:=-lm
libs+=-lrt

cflags=$(root)/include $(libs) -o0 -g3 -wall

vpath %.c $(dir)

objs=$(addprefix $(objdir)/,$(patsubst %.c,%.o, $(notdir $(src))))

# main: $(objs)
# @echo "$(cflags) -o main $(objs)"

# $(objs)

vpath src1

all: $(src) 
	@echo $(objs)
	@echo $(cflags)
	@echo "---------------------------"
	@echo $@ 
	@echo "---------------------------"
	@echo $^
	@echo "---------------------------"
	@echo $<
	@echo "---------------------------"
	@echo $?
	@echo "---------------------------"
	-echo $(subst .o,.c,test.o test2.o )
	@echo "---------------------------"
#-echo $($(subst .o,.c, test.o, test.o): %.c=%.o)
	@echo "---------------------------"
	@echo $(wildcard *.c)
	@echo "---------------------------"
	@echo $(basename RongTao.c) $~



