STR1=string1outer


ifneq ("1","2")
STR=11111
STR1+=add some string to string1
#这个地方就是个宏定义，如果if不成立，这里跟不不会“编译”  
else
STR=22222
endif

all:
	@echo $(STR) $(STR1)
