#include <iostream>
#include <cstring>
#include <pthread.h>
#include <cstdlib> // Để sử dụng rand() và srand()
#include <ctime>   // Để sử dụng time()
#include <unistd.h>

using namespace std;


int main(){
    char buff[6] = "01234";
    cout << buff<< "     " << strlen(buff) <<endl;
    return 0;
}
