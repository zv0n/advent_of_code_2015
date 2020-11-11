CXX ?= c++
CXXFLAGS ?= -std=c++11 -Wall -Wextra -pedantic `pkg-config openssl --cflags`
LDFLAGS ?= `pkg-config openssl --libs`
PROJECT = hash

all: ${PROJECT}

${PROJECT}: main.cpp
	${CXX} ${CXXFLAGS} -o $@ $^ ${LDFLAGS}

test: ${PROJECT}
	./${PROJECT}

clean:
	${RM} *.o ${PROJECT}

.PHONY: all clean test
