function [CD,f,t,CD1,CD2] = cohen(x,fs,type)
% Function to compute several of Cohen's class of time-frequencey distributions
%	
%   Outputs
%		WD  Wigner-Ville Distribution
%		f   Frequency vector for plotting
%		t   Time vector for plotting
%	 Inputs
%		x	 Complex signal
%		fs  Sample frequency
%		type of distribution. Valid arguements are:
%			'choi' (Choi-Williams), 'BJC' (Born-Jorden-Cohen);
%			and 'R_M' (Rihaczek-Margenau)	Default is Wigner-Ville
	
%
sigma = 1;	% Choi-Williams constant
L = 30;		% Determining function size
%
[N, xcol] = size(x);
if N < xcol                 % Make signal a row vector if necessary
   x = x';
   N = xcol;
end  
t = (1:N)/fs;		% Calculate time and frequency vectors
f = (1:N) *(fs/(2*N));
%Compute instantainous autocorrelaton:  Eq. 9-2
CD1 = int_autocorr(x);
if type(1) == 'c'			 	% Get appropriate Determining function
   G = choi(sigma,L);		    % Choi-Williams
elseif type(1) == 'B'	  
   G = BJC(L);					% Born-Jorden-Cohen
elseif type(1) == 'R'
   G = R_M(L);					% Rihaczek-Margenau
else
   G = 1;						% Default Wigner-Ville
end   
    %figure
    %mesh(1:L-1,1:L-1,G);            % Plot Determining function
    %xlabel('N');                    %     and label axis
    %ylabel('N');
    %zlabel('G(,N,N)');
CD2 = conv2(CD1,G);		 		% 2-D convolution
CD2 = CD2(1:N,1:N);				% Truncate extra points produced by convolution
%  Take FFT  Note since CD2 is a matrix, FFT is applied to each column; hence,
%     FFT taken with respect to columns, i.e., time 
CD = fft(CD2);
