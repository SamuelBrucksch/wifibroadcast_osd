
CPPFLAGS+= -I/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux -I$(VGFONTPATH) -I/opt/vc/include -I/opt/vc/include/interface/vmcs_host/linux -I/opt/vc/include/interface/vcos/pthreads

LDFLAGS+=-L/opt/vc/lib/ -lGLESv2 -lEGL -lopenmaxil -lbcm_host -lvcos -lvchiq_arm -lpthread -lrt -lm

all: osd

%.o: %.c
	gcc -c -o $@ $< $(CPPFLAGS)
 

osd: main.o
	gcc -o $@ $^ $(LDFLAGS) -lshapes


clean:
	rm -f osd *.o *~


