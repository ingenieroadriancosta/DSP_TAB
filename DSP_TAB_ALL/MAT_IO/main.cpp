#include "Mat_Io_Lib\Mat_Io_Lib.cpp"


int main(){
    short Buff[16] = {0,1,2,3,4,5,6,7,7,6,5,4,3,2,1,0};
    MIO_MNG.SaveMatFile_Short( "Hola.mat", "Buff", Buff, 16 );
    return 0;
}
