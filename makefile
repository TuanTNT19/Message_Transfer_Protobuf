build1:

	g++ main1.cpp mess.pb.cc Buffer.cpp -lprotobuf -pthread -o main1

clean:
	sudo rm -rf  main1
proto:
	protoc --cpp_out=. mess.proto
