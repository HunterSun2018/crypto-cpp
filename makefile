
TARGET = demo
CXXFLAG = -std=c++20
LIBS = -l crypto++ -l ssl -l crypto
OBJS = ssl_aes.o

$(TARGET) : $(OBJS)
	g++ -g $(CXXFLAG) -o $(TARGET) $(OBJS) $(LIBS)

%.o : src/%.cpp
	g++ -g $(CXXFLAG) -o $@ -c $<

ssl_aes.so : ssl_aes.hpp

clean :
	rm -r $(TARGET) $(OBJS)
