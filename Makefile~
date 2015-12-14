CPPFLAGS+= -I/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux
LDFLAGS+= -lfreetype -lz
LDFLAGS+=-L/opt/vc/lib/ -lGLESv2 -lEGL -lopenmaxil -lbcm_host -lvcos -lvchiq_arm -lpthread -lrt -lm -lshapes

all: osd

%.o: %.c
	gcc -c -o $@ $< $(CPPFLAGS)
 

osd: main.o frsky.o render.o telemetry.o
	gcc -o $@ $^ $(LDFLAGS)


clean:
	rm -f osd *.o *~

