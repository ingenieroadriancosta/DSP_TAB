% function pruebas
try %#ok<TRYNC>
    info = [ 123, 0, 0, 0, 0, 0, 0, 125 ];
    fwrite( ser, info )
    fclose( ser );
end

clc;clear all;
BaudRate = 1228800;
ser = serial( 'COM1','BaudRate', BaudRate );
set( ser, 'BaudRate', BaudRate );
set( ser, 'DataBits', 8 );
set( ser, 'Parity', 'none' );
set( ser, 'StopBits', 1 );
set( ser, 'FlowControl', 'none' );
set( ser, 'InputBufferSize', 1024 * 1024 );
set( ser, 'OutputBufferSize', 1024 * 1024 );
fopen( ser );

info = [ 123, 0, 1, 0, 0, 0, 0, 125 ];
fwrite( ser, info )
ind = 1;
Lind = 1;
t2 = 2000 * 300;
datao = zeros( 1, t2 );
while( ind<t2 )
    if ser.BytesAvailable>=8
        byn = ser.BytesAvailable;
        byn = byn - mod( byn, 8 );
        DIn = fread( ser, byn );
        for in=1:8:byn
            datao( ind ) = DIn(in+1) + 256 * DIn(in+2);
            ind = ind + 1;
        end
        
        
        disp( [ind ser.BytesAvailable] )
        if (ind - Lind)>2000
            Lind = ind;
            plot( datao(1:ind-1) )
            drawnow
        end
    end
    pause(0.01)
end
pause(1)


info = [ 123, 0, 0, 0, 0, 0, 0, 125 ];
fwrite( ser, info )









