CC = g++

#network static lib
NETWORK_STATICLIB ?= lib/linux/network.a

# phony
.PHNOY : all clean

all : \
	$(NETWORK_STATICLIB)

$(NETWORK_STATICLIB) :
	cd base/network/ && $(MAKE) CC='$(CC) -std=gnu99'

clean :
	cd base/network/ && $(MAKE) clean 
