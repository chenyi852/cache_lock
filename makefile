

default:
	echo "this is default"
kernel_module:
	echo "hello"
#make -f $@/Makefile all 
	cd $@ && make all
	echo `pwd`


tt:
	echo "test"

clean:
	cd kernel_module && make clean

.PHONY: default clean  kernel_module
