.PHONY: all clean extract

include ../Makefile.toolchain

SUBDIR := STM32F10x_StdPeriph_Lib_V3.5.0

TARGETS := stdperiph.a

CFLAGS += -include configuration.h
CFLAGS += -I$(SUBDIR)/Libraries/STM32F10x_StdPeriph_Driver/inc -I$(SUBDIR)/Libraries/CMSIS/CM3/CoreSupport -I$(SUBDIR)/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x
SRCS := $(wildcard $(SUBDIR)/Libraries/STM32F10x_StdPeriph_Driver/src/*.c)
CFLAGS += -ffunction-sections -fdata-sections

OBJS := $(SRCS:%.c=%.o)

all: $(TARGETS)

clean:
	rm -f $(OBJS) $(TARGETS)

extract:
	rm -fr $(SUBDIR)
	rm -f include system cmsis
	unzip en.stsw-stm32054.zip
	ln -sf $(SUBDIR)/Libraries/STM32F10x_StdPeriph_Driver/inc include
	ln -sf $(SUBDIR)/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x system
	ln -sf $(SUBDIR)/Libraries/CMSIS/CM3/CoreSupport cmsis
	sed -i 's/\(static __INLINE void [_a-zA-Z]\+\)()/\1(void)/g;' $(SUBDIR)/Libraries/CMSIS/CM3/CoreSupport/core_cm3.h

stdperiph.a: $(OBJS)
	@stat $(SUBDIR)
	$(AR) cm $@ $^

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

.s.o:
	$(CC) $(CFLAGS) -c -o $@ $<
