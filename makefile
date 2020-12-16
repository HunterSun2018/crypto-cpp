
TARGET = demo
CXXFLAG = -std=c++20
LIBS = -l ssl -l crypto #-l crypto++ 
OBJS = main.o aes.o

$(TARGET) : $(OBJS)
	g++ $(CXXFLAG) -o $(TARGET) $(OBJS) $(LIBS)

%.o : src/%.cpp
	g++ $(CXXFLAG) -o $@ -c $<

ssl_aes.o : src/ssl_aes.hpp
aes.o : src/aes.hpp
main.o : src/ssl_aes.hpp src/aes.hpp

clean :
	rm -rf $(TARGET) $(OBJS)
