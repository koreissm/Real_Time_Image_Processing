main: main.o Filter.o ImageProcessing.o
	g++ main.o Filter.o ImageProcessing.o -o main -lopencv_core -lopencv_highgui -lopencv_imgproc

main.o:
	g++ -c main.C

Filter.o: Filter.C
	g++ -c filter.C

ImageProcessing.o: ImageProcessing.C
	g++ -c ImageProcessing.C

debug: main.C Filter.C ImageProcessing.C
	g++ -g main.C Filter.C ImageProcessing.C -o main -lopencv_core -lopencv_highgui -lopencv_imgproc

clean:
	rm -f *.o main
