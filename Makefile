CC = g++

#network static lib
NETWORK_STATICLIB ?= lib/linux/network.a
MARUTIL_STATICLIB ?= lib/linux/marsutil.a

# phony
.PHNOY : all clean

all : \
	$(NETWORK_STATICLIB) $(MARUTIL_STATICLIB)

$(NETWORK_STATICLIB) :
	cd base/network/ && $(MAKE) CC='$(CC) -std=gnu99'

$(MARUTIL_STATICLIB) :
	cd base/marsutil/ && $(MAKE) CC='$(CC) -std=gnu99'

clean :
	cd base/network/ && $(MAKE) clean;
	cd base/marsutil/ && $(MAKE) clean;
