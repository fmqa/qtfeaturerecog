CXXFLAGS = -std=c++11 -Wall -g -I../src/include -I/home/fadimk/Anwendungen/gtest/include -fsanitize=address -fno-omit-frame-pointer
LDFLAGS = -L/home/fadimk/Anwendungen/gtest/lib64 -lgtest -lgtest_main -lasan

all: CirclesTest LineUtilsTest RasterCoreTest RasterFilterTest

CirclesTest: CirclesTest.o
	$(CXX) -o CirclesTest CirclesTest.o circles.o $(LDFLAGS)

CirclesTest.o: CirclesTest.cc 
	$(CXX) $(CXXFLAGS) -c CirclesTest.cc ../src/include/util/circles.cc

LineUtilsTest: LineUtilsTest.o
	$(CXX) -o LineUtilsTest LineUtilsTest.o $(LDFLAGS)

LineUtilsTest.o: LineUtilsTest.cc
	$(CXX) $(CXXFLAGS) -c LineUtilsTest.cc

RasterCoreTest: RasterCoreTest.o
	$(CXX) -o RasterCoreTest RasterCoreTest.o $(LDFLAGS)

RasterCoreTest.o: RasterCoreTest.cc
	$(CXX) $(CXXFLAGS) -c RasterCoreTest.cc

RasterFilterTest: RasterFilterTest.o
	$(CXX) -o RasterFilterTest RasterFilterTest.o sobel.o gaussian.o $(LDFLAGS)

RasterFilterTest.o: RasterFilterTest.cc
	$(CXX) $(CXXFLAGS) -c RasterFilterTest.cc ../src/include/raster/flt/sobel.cc ../src/include/raster/flt/gaussian.cc

clean:
	$(RM) CirclesTest LineUtilsTest RasterCoreTest RasterFilterTest
	$(RM) CirclesTest.o LineUtilsTest.o RasterCoreTest.o RasterFilterTest.o
	$(RM) circles.o sobel.o gaussian.o

