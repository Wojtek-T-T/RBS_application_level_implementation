$(CC) = gcc -O3
CFLAGS = -I -lpthread -lrt

rbs_app: main.o sequences.o workload.o otw.o rbs_lib_app.o
	$(CC) $(CFLAGS) main.o sequences.o workload.o otw.o rbs_lib_app.o -o rbs_app -lpthread -lrt

main.o: main.c sequences.h
	$(CC) $(CFLAGS) -c main.c

sequences.o: sequences.c workload.h ../rbs_lib_app/rbs_lib_app.h
	$(CC) $(CFLAGS) -c sequences.c

rbs_lib_app.o: ../rbs_lib_app/rbs_lib_app.c ../rbs_lib_app/rbs_lib_app.h
	$(CC) $(CFLAGS) -c ../rbs_lib_app/rbs_lib_app.c

workload.o: workload.c workload.h otw.h
	$(CC) -c workload.c

otw.o: otw.c otw.h
	$(CC) -c otw.c

clean:
	rm *.o rbs_app