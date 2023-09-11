$(CC) = gcc
CFLAGS = -I -lpthread -lrt

rbs_app: main.o sequences.o workload.o otw.o rbs_lib.o
	$(CC) $(CFLAGS) main.o sequences.o workload.o otw.o rbs_lib.o -o rbs_app -lpthread -lrt

main.o: main.c sequences.h
	$(CC) $(CFLAGS) -c main.c

sequences.o: sequences.c workload.h rbs_lib.h
	$(CC) $(CFLAGS) -c sequences.c

rbs_lib.o: rbs_lib.c rbs_lib.h
	$(CC) $(CFLAGS) -c rbs_lib.c

workload.o: workload.c workload.h otw.h
	$(CC) -c workload.c

otw.o: otw.c otw.h
	$(CC) -c otw.c

clean:
	rm *.o rbs_app

#gcc -o rbs_app main.c rbs_lib.c workload.c sequences.c otw.c -lpthread -lrt