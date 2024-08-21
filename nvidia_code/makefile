build:
	g++ main.cpp Message.cpp DataProcess.cpp SendMessage.cpp ReceiveMessage.cpp File.cpp mess.pb.cc -o main -lprotobuf -pthread
clean:
	rm -rf main *.o main2 *.txt
build_nvidia:
	aarch64-linux-gnu-g++ -I/home/tuantnt/Desktop/protobuf_arm/protobuf_set/include -std=c++11 -c Message.cpp -o Message.o
	aarch64-linux-gnu-g++ -I/home/tuantnt/Desktop/protobuf_arm/protobuf_set/include -std=c++11 -c ReceiveMessage.cpp -o ReceiveMessage.o
	aarch64-linux-gnu-g++ -I/home/tuantnt/Desktop/protobuf_arm/protobuf_set/include -std=c++11 -c SendMessage.cpp -o SendMessage.o
	aarch64-linux-gnu-g++ -I/home/tuantnt/Desktop/protobuf_arm/protobuf_set/include -std=c++11 -c DataProcess.cpp -o DataProcess.o
	aarch64-linux-gnu-g++ -std=c++11 -c File.cpp -o File.o
	aarch64-linux-gnu-g++ -I/home/tuantnt/Desktop/protobuf_arm/protobuf_set/include -std=c++11 -c mess.pb.cc -o mess.pb.o
	aarch64-linux-gnu-g++ -I/home/tuantnt/Desktop/protobuf_arm/protobuf_set/include -std=c++11 -c main.cpp -o main.o
	aarch64-linux-gnu-g++ Message.o ReceiveMessage.o SendMessage.o DataProcess.o File.o main.o  mess.pb.o -o main2 -L/home/tuantnt/Desktop/protobuf_arm/protobuf_set/lib -lprotobuf -pthread
scp:
	scp ./main2 nx@192.168.144.15:/home/nx/Desktop/test




