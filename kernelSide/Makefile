INCLUDE=include
SOURCE=src
TARGET=kmodule
OUTPUT=build
EXPORT=package

SOURCES=$(wildcard $(SOURCE)/*.c)

.PHONY: run

# Depends on bin/include bin/*.c and bin/Makefile
all: $(subst $(SOURCE),$(OUTPUT),$(SOURCES)) $(OUTPUT)/Makefile
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD)/$(OUTPUT) modules

# Create a symlink from src to bin
$(OUTPUT)/%: $(SOURCE)/%
	ln -s ../$< $@

# Generate a Makefile with the needed obj-m and mymodule-objs set
$(OUTPUT)/Makefile:
	echo "obj-m += $(TARGET).o" > $@

clean:
	sudo rmmod build/$(TARGET).ko
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rf $(OUTPUT)
	mkdir $(OUTPUT)

run: all
	sudo insmod build/$(TARGET).ko
