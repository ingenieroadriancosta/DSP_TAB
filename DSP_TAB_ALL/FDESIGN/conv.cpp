int conv( double* P0, int N0, double* P1, int N1, double* Pout ){
    static double PoutT[1024];
    ZeroMemory( PoutT, (N0+N1) * sizeof(double) );
    int yin = 0;
    for( int n=0; n<N0; n++ ){
        for( int k=0; k<N1; k++ ){
            yin = (k) + (n);
            PoutT[yin] = PoutT[yin] + P0[n] * P1[k];
        }
    }
    memcpy( Pout, PoutT, (N0 + N1 - 1)* sizeof(double) );
    return (N0 + N1 - 1);
}
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
double sum( double* Vect, int Len, bool setabs = false ){
    double S2r = 0;
    if( setabs ){
        for( int i=0; i<Len; i++ ){
            S2r = S2r + abs(Vect[i]);
        }
    }else{
        for( int i=0; i<Len; i++ ){
            S2r = S2r + Vect[i];
        }
    }
    return S2r;
}
