
clean:
	rm -rf main *.o main2 *.txt
build_nvidia:
	aarch64-linux-gnu-g++ -I/home/tuantnt/Desktop/protobuf_arm/protobuf_set/include -std=c++17 -c Buffer.cpp -o Buffer.o
	aarch64-linux-gnu-g++ -I/home/tuantnt/Desktop/protobuf_arm/protobuf_set/include -std=c++17 -c ReceiveMessage.cpp -o ReceiveMessage.o
	aarch64-linux-gnu-g++ -I/home/tuantnt/Desktop/protobuf_arm/protobuf_set/include -std=c++17 -c SendMessage.cpp -o SendMessage.o
	aarch64-linux-gnu-g++ -I/home/tuantnt/Desktop/protobuf_arm/protobuf_set/include -std=c++17 -c DataProcess.cpp -o DataProcess.o
	aarch64-linux-gnu-g++ -std=c++17 -c File.cpp -o File.o
	aarch64-linux-gnu-g++ -I/home/tuantnt/Desktop/protobuf_arm/protobuf_set/include -std=c++17 -c mess.pb.cc -o mess.pb.o
	aarch64-linux-gnu-g++ -I/home/tuantnt/Desktop/protobuf_arm/protobuf_set/include -std=c++17 -c main.cpp -o main.o
	aarch64-linux-gnu-g++ Buffer.o Message.o ReceiveMessage.o SendMessage.o DataProcess.o File.o main.o  mess.pb.o -o main2 -L/home/tuantnt/Desktop/protobuf_arm/protobuf_set/lib -lprotobuf -pthread

scp:
	scp ./main2 nx@192.168.144.15:/home/nx/Desktop/test




