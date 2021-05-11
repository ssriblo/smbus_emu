TARGET = smb_emu
PREFIX ?= /usr/local
SRCS = test.c gpio_utility.c soft_i2c.c api.c
OBJS = $(SRCS:.c=.o)
LIBS=-lgpiod

.PHONY: all clean install uninstall

all: $(TARGET) 
$(TARGET): $(OBJS) 
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS) $(LIBS) 
 
.c.o:
	$(CC) $(CFLAGS)  -c $< -o $@

clean:
	rm -rf $(TARGET) $(OBJS)
install:
	install $(TARGET) $(PREFIX)/bin
uninstall:
	rm -rf $(PREFIX)/bin/$(TARGET)