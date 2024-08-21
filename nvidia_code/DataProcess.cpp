#include "DataProcess.h"

Coordinate DataProcess::CoorProcess(const Coordinate& c, int fd){
    Coordinate coort;
    coort.set_x(c.x() * 2);
    coort.set_y(c.y() * 2);
    coort.set_z(c.z() * 2);

    char *buff = new char [256];
    sprintf(buff, "\n%d %d %d", coort.x(), coort.y(), coort.z());
    int w = write(fd, buff, strlen(buff));
    if (w <=0){
        cout <<"ERROR in Write to File"<<endl;
    }
    
    delete[] buff;
    return coort;
}

Heartbeat DataProcess::HeartProcess(const Heartbeat& h, int fd){
    Heartbeat hb;
    hb.set_panangle(h.panangle() * 2);
    hb.set_tiltangle(h.tiltangle() * 2);
    for (int i = 0; i < 6; i++){
        hb.add_errorcode(h.errorcode(i) * 2);
    }

    char *buff = new char [256];
    int len = sprintf(buff, "\n%.2f %.2f ", hb.panangle(), hb.tiltangle());

    for (int i = 0; i < 24; i+=4){
        sprintf(buff + len + i, "%3d ", hb.errorcode(i/4));
    }

    int w = write(fd, buff, strlen(buff));
    if (w <=0){
        cout <<"ERROR in Write to File"<<endl;
    }
     
    delete[] buff;
    return hb;
}
