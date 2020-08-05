% function pruebas
clc;clear all;
Fs = 2000;
FNyq = Fs/2;
nSeg = 50;
n = 0:nSeg * Fs - 1;
t = n/Fs;
f = 10;
f = 200;y = sin( 2 * pi * f * t );
% y = sin( 2 * pi * f * t .* t );

load AAA;
% y = double(Chann_0(1:nSeg * Fs))/32767;y = 2*( y - 0.5 );

cla;drawnow;
% wigner( y, Fs, rectwin(Fs) );asd
% cohendist( y, 1 ); asd
choi_william( y, Fs, rectwin(Fs) );
% Fs, window_in, sigmavalue
sinp = y;
window_in = rectwin(Fs);
% % % % % % % % % % % % % % % % % % % % % % % % % % % 
asdasdasd
sinp = sinp(:)';
window_in = window_in(:)';
l = length(sinp);
l = l - mod( l, Fs );
lf = l/Fs;
yact = zeros( 1, Fs );
wig = zeros( Fs/2+1, (lf) );
thetaa = (0:size( wig, 1 ) - 1)/size( wig, 1 );
taoa = (0:size( wig, 2 ) - 1)/size( wig, 2 );
for x = 0:size( wig, 2 )-1
    n = Fs * (x);
    for y = 0:size( wig, 1 )-1
        if( (n-y+1)<1 )
            yact(y+1) = 0;
            continue;
        end
        yact(y+1) =  sinp( n-y+1 ) * sinp( n+y+1 );
    end
    t_o = fft(yact .* window_in, Fs);
    t_o = t_o(1:Fs/2+1);
    wig( :, x+1 ) = 2*abs( t_o )/Fs;
end
% 
phi = zeros( size(wig) );
sigma = sin( pi * thetaa );
for x = 0:size( wig, 2 )-1
    n = Fs * (x);
    for y = 0:size( wig, 1 )-1
        phi( y+1, x+1 ) = exp( -( thetaa(y+1)^2 * taoa(x+1) )/sigma(y+1) );
    end
end
wig = wig .* phi;
wig = wig/max( max(wig) );
t = 0:length(sinp)/Fs - 1;
f = 0:Fs/2;
surf( t, f, wig, 'EdgeColor', 'none' );   
% colorbar;
axis xy; axis tight; colormap(jet); view(0,90);
xlabel( 'Time (s)' );
ylabel( 'Frequency (Hz)' );
% imshow( 10*imresize(wig, [400 1000], 'bilinear' ) )












