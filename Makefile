CC=gcc # c language
RM?=rm -f
MKDIR?=mkdir -p
RMDIR?=rm -rf

#LDFLAGS := -L"../mylibdir" -lccv $(LDFLAGS)
#CFLAGS := -O3 -Wall -I"../mylibdir" $(CFLAGS)
CFLAGS := -std=c99 -fPIC -g -O3 -Wall $(CFLAGS) 

TARGET := test   #the binary file name.
OBJS := test.o convert.o stack.o utils.o parser.o commands.o equations.o funct1.o

all: $(TARGET)

#link
$(TARGET): $(OBJS)
	$(CC)  -o $@ $(OBJS) $(LDFLAGS) 
	#$(CC) -o $@ $< $(LDFLAGS) 

#compile
$(OBJS): %.o: %.c
	$(CC) -o $@ -c $(CFLAGS) $< 

clean:
	rm -f *.o $(TARGET) *.so
	
so: $(OBJS)
	gcc $(CFLAGS)  -shared -o libc.so $(OBJS) 

.PHONY: all  clean so
