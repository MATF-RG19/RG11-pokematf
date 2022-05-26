FROM gcc:latest

COPY . /usr/src/pokemon

WORKDIR /usr/src/pokemon

RUN apt-get update && apt-get -y install cmake protobuf-compiler

RUN apt-get install mesa-utils -y

RUN apt-get update && apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev -y

RUN apt-get install binutils-gold -y

RUN cmake CMakeLists.txt

RUN make

CMD ["./Pokemon.exe"]
