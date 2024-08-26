build1:

	g++ main1.cpp mess.pb.cc Buffer.cpp -lprotobuf -pthread -o main1
build2:
	g++ main2.cpp mess.pb.cc -lprotobuf -pthread -o main2
build2344442:
	aarch64-linux-gnu-g++ -I/home/tuantnt/Desktop/protobuf_arm/protobuf_set/include -std=c++11 -c main2.cpp -o main2.o
	aarch64-linux-gnu-g++ -I/home/tuantnt/Desktop/protobuf_arm/protobuf_set/include -std=c++11 -c mess.pb.cc -o mess.pb.o
	aarch64-linux-gnu-g++ main2.o mess.pb.o -o main2 -L/home/tuantnt/Desktop/protobuf_arm/protobuf_set/lib -lprotobuf -pthread
clean:
	sudo rm -rf main2 main1
proto:
	protoc --cpp_out=. mess.proto
