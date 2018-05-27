CC = g++
CFLAGS = -Isrc -O3 -Wall -D_GLIBCXX_ISE_CXX11_ABI=1
LDFLAGS = -Lsrc -lDetailPlace -lGlobalPlace -lLegalizer -lPlacement -lParser -lPlaceCommon
CPPS = $(wildcard src/*.cpp)
OBJECTS = $(addprefix obj/,$(notdir $(CPPS:.cpp=.o)))
EXECUTABLE = place

all: obj $(EXECUTABLE)
	
obj:
	mkdir $@

$(EXECUTABLE): $(OBJECTS)
	$(CC) $^ $(LDFLAGS) -o $@

obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)



