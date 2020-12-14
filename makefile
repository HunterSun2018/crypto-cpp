
TARGET = demo
CXXFLAG = -std=c++20
LIBS = -l ssl -l crypto #-l crypto++ 
OBJS = main.o

$(TARGET) : $(OBJS)
	g++ $(CXXFLAG) -o $(TARGET) $(OBJS) $(LIBS)

%.o : src/%.cpp
	g++ $(CXXFLAG) -o $@ -c $<

ssl_aes.o : src/ssl_aes.hpp

clean :
	rm -rf $(TARGET) $(OBJS)
