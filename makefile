obj = $(wildcard *.cpp)

main.exe : $(obj)
	g++ -std=c++11 -o main.exe $(obj)