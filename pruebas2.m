clc;clear all;
load asd
Fs = 2000;
FNyq = Fs/2;
n = 0:1023;
t = n/Fs;
f1 = 700;f2 = 200;
y = double(Chann_1);%(0*sin( 2 * pi * f1 * t ) + sin( 2 * pi * f2 * t ))/2;
n = 0:length(Chann_1)-1;
%%%
N = 10;
fc = 29;
[b, a] = butter( N, fc/FNyq, 'low' );
A = fft( a, length(n) );
B = fft( b, length(n) );
Y = fft( y, length(n) );
% % % 
f = Fs * (0:length(n)-1)/length(n);
plot( f, abs(Y)/max(abs(Y)), 'LineWidth', 2 );
plot( f, abs(B)./abs(A), 'g', 'LineWidth', 2 );
hold on
%%%
YO = Y.*B + Y.*A;
plot( f, abs(YO)/max(abs(YO)), 'r', 'LineWidth', 2 );
%
YON = Y .* (B./A);
plot( f, abs(YON)/max(abs(YON)), 'y', 'LineWidth', 2 );
hold off
% % % 
% % % 
YR = Y .* ( abs(B)./abs(A) );
plot( f, abs(YR)/max(abs(YR)), 'y', 'LineWidth', 2 );
yn = ifft( YR, length(n) );
plot( n, [y;real(yn)] )

max( real(yn) )
min( real(yn) )
%%
clc;clear all; load a01m
val = val/max( abs(val) );
val = round( 512 * val + 511 );
min( val )
max( val )
plot( val )
ylim( [0 1023] )

val = val( (1:64) + 100 );

valT = zeros( 1, 16*length(val) );
for in=1:length(val)
    for in2=1:16
        valT( 16*( in-1 ) + in2  ) = val(in);
    end
    
end
val = valT;

plot( val )
clc;
fprintf( 'unsigned short Heart[%i] = { ', length( val ) )
for in=1:length( val )-1
    fprintf( '%i, ', val( in ) )
end
fprintf( '%i }; ', val(length( val )) )
fprintf( '\n\n\n' )

V = abs(fft(val-0, length(val)));
V = V( 1:end/2 );
f = 1000 * ( 0:length(V)-1 )/length(V);
plot( f, V )

y = val - 512;
y = y/max( abs(y) );
y = [y y(1:100)];

save('AAA', 'y' )


%%
clc;clear all;
load DFTN
load ORIGEN
load asd
who
Chann_2 = double(Chann_2(1:end));
Chann_2 = Chann_2/32767;
Chann_2 = 2*(Chann_2 - 0.5);

wind = gausswin( length(Chann_2), 30 );
chw = Chann_2 .* wind';
Y = fft( chw );
Y = abs( Y );


Y = Y(1:end/2);
plot( Y )



%%
clc;clear all;
[val, Fs] = audioread( 'Normal Split Second Sound.wav' );
%
val = val( 1:round(Fs/1000):end);
val = val( 200:end-45 );
val( 1:300 ) = 0;
val( end-570:end ) = 0;
%
plot( val )
length(val)
val = val/max(abs(val));
clc;

% val = ( (val + 1)/2 );
val = round(1023 * ( (val + 1)/2 ));
plot(val)
% 
% 
val = val( 1:1024 );
val( end-100:end ) = 512;
val = [ val; val; val; val; val; val; val; val];
size(val)
plot(val)
% asd
% 
clc
fprintf( 'unsigned short Heart[%i] = { ', length( val ) )
for in=1:length( val )-1
    fprintf( '%i, ', val( in ) )
    if mod( in, 512 )==0
        fprintf( '\n' )
    end
end
fprintf( '%i }; ', val(length( val )) )
fprintf( '\n\n\n' )
% 8192


val = val - 512;

wind = gausswin( length(val), 30 );
chw = val .* wind;

Y = fft( chw );
Y = abs( Y );


Y = Y(1:end/2);
% plot( Y )


%%
clc;clear all;
Fs = 8000;
MAXS = ( 10 * 1024 * 1024 );
NFrames = ceil( MAXS/Fs )
TMSeg = 0.1;
% 
x = NFrames/TMSeg
y = Fs;
xy = x * y;
Tam = xy/( 1024 * 1024 )




%%
clc;clear all;
Fs = 2000;
NFrames = 3600 * Fs;
TMSeg = .1;
NPos = ceil( (NFrames/Fs)/TMSeg )
% 
Tam = NPos * Fs;
Tam = Tam/( 1024 * 1024 )
%%
clc;clear all;
for in=2:1024
    y = ones( in, 1 );
    wnd = gausswin( length(y), 4 );
    t = (0:length(y)-1)/( length(y) - 1 );
    plot( t, y.*wnd )
    title( in )
    axis( [0 1 0 1] )
    drawnow
end




















