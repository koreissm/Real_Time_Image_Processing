main: main.o Filter.o ImageProcessing.o Bot.o SerialClass.o
	g++ main.o Filter.o ImageProcessing.o Bot.o SerialClass.o -o main -lopencv_core -lopencv_highgui -lopencv_imgproc

main.o:
	g++ -c main.C

SerialClass.o:
	g++ -c SerialClass.C

Bot.o:
	g++ -c Bot.C

Filter.o: Filter.C
	g++ -c filter.C

ImageProcessing.o: ImageProcessing.C
	g++ -c ImageProcessing.C

debug: main.C Filter.C ImageProcessing.C Bot.o
	g++ -g main.C Filter.C ImageProcessing.C Bot.o -o main -lopencv_core -lopencv_highgui -lopencv_imgproc
	sudo gdb ./main

clean:
	rm -f *.o main
