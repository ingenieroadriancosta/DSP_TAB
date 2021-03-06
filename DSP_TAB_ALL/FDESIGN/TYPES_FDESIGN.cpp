#include "FILTERS_FUNCS.cpp"

/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK FDESIGN_Dlg( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK FWINDOWING_Dlg( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
const char Wind_2_Fir[][32] = {     "Rectangular",       "Gauss",                  "Nuttall",    "Hann",
                                    "Blackman",          "Blackman-Harris",        "Flat top",   "Bartlett",
                                    "Triangular",        "Hamming",                "Kaiser",     "Riesz",
                                    "Riemann",           "Parzen",                 "Bohman",     "Tukey",
                                    "Poisson y Cauchy",  "Dolph-Chebyshev" };
                                                    ///  "Parzen(Vall�e-Poussin)"
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
const char Wind_2_Fir_name[][32] = {", rectwin",           ", gausswin",          ", nuttallwin", ", hann",
                                    ", blackman",          ", blackmanharris",    ", flattopwin", ", bartlett",
                                    ", triang",            ", hamming",           ", kaiser",     "",
                                    "",                    ", parzenwin",         ", bohmanwin",  ", tukeywin",
                                    "",                    ", chebwin" };
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
const char Wind_2_Fir_nameFUNC[][32] = {"rectwin",           "gausswin",          "nuttallwin", ", hann",
                                        "blackman",          "blackmanharris",    "flattopwin", ", bartlett",
                                        "triang",            "hamming",           "kaiser",     "",
                                        "",                  "parzenwin",         "bohmanwin",  "tukeywin",
                                        "",                  "chebwin" };
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
struct WINDOWING{
    private:
    int                     IPtr;
    int                     IndWind;
    double                  Alpha[32];
    public:
    WINDOWING(){
        IPtr = 0;
        ZeroMemory( &IPtr, sizeof(WINDOWING) );
    }
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///
    BOOL ExistOnMatlab(){
        return ( !(IndWind==11 || IndWind==12 || IndWind==16) );
    }
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int GetWinType(){ return IndWind; }
    BOOL SetWinType( int WinType ){
        IndWind = WinType;
        return TRUE;
    }
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    double GetAlpha(){ return Alpha[IndWind]; }
    BOOL SetAlpha( double AlphaV ){
        Alpha[IndWind] = AlphaV;
        return TRUE;
    }
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BOOL HaveWindowAlpha(){
        return ( ( IndWind==1 || IndWind==10 || IndWind==16 || IndWind==15 || IndWind==17) );
    }
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BOOL Apply_Window( int NFrames2, double* InputV ){
        Window_F( NFrames2, InputV, IndWind, Alpha[IndWind] );
        return TRUE;
    }
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
};
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////

///
///
///
///
///
///
///
struct Spectogramprepare{
    private:
    int                 IPtr;
    int                 NFrames;
    int                 Fs;
    BYTE*               Spect_Out;
    double              TimeSeg;
    double              MaxVal;
    int                 xs;
    int                 ys;
    int                 x_ys;
    public:
    WINDOWING           WINDWN;
    private:
    int                 DivFrames;
    int                 Tao;
    unsigned char       UCTTEMP;
    public:
    ~Spectogramprepare(){
        Free();
    }
    Spectogramprepare(){
        IPtr = 0;
        ZeroMemory( &IPtr, sizeof(Spectogramprepare) );
        NFrames = 0;
        Fs      = 0;
        TimeSeg = 1;
        SetTimeSeg(1);
        MaxVal  = -1e100;
        Tao = 1;
    }
    ///
    int Get_xs(){ return xs; }
    int Get_ys(){ return ys; }
    BYTE* Get_Spect_Out(){ return Spect_Out; }
    ///
    BOOL Free(){
        if( Spect_Out!=NULL ){
            free( Spect_Out );
            Spect_Out = NULL;
        }
        return TRUE;
    }
    ///
    double GetTimeSeg(){ return TimeSeg; }
    BOOL SetTimeSeg( double TMSeg ){
        TimeSeg = TMSeg;
        DivFrames = (int)(TimeSeg * Fs);
        Free();
        return TRUE;
    }
    ///
    BOOL Prepare( int FsI, int NFramesI ){
        if( Fs==FsI && NFrames==NFramesI && Spect_Out!=NULL ){
            return FALSE;
        }
        Free();
        Fs      = FsI;
        NFrames = NFramesI;
        xs = (int)ceil( ((double)NFrames/TimeSeg)/Fs );
        ys = Fs;
        x_ys = xs * ys;
        Spect_Out = (BYTE*)malloc( x_ys + 128 );
        ZeroMemory( Spect_Out, x_ys + 128 );
        DivFrames = (int)(TimeSeg * Fs);
        return TRUE;
    }
    ///
    ///
    ///
    BOOL SetWindowing( int NFrames2, double* InputV ){
        return WINDWN.Apply_Window( NFrames2, InputV );
    }
    ///
    ///
    ///
    BOOL Spectogram_TAD( int NFrames2, fftprepare* fftpIn ){
        SetTimeSeg( 1 );
        Prepare( 2000, NFrames2 );/// MaxVal
        /// char T2T[128] = {0};sprintf( T2T, "%i", DivFrames );MessageBoxA( GetActiveWindow(), T2T, "", 0 );
        double* DPtr = NULL;
        int N2FR = 0;
        /// asd0
        int PPP = 0;
        int y = 0;
        int x=0;
        int ys_21 = ys/2 - 1;
        const int FNyq = Fs/2;
        for( x=0; x<xs; x++ ){
            N2FR = x * ys;
            DPtr = &fftpIn->InputV[N2FR];
            SetWindowing( Fs, DPtr );
            ZeroMemory( fftpIn->RealV, Fs * sizeof(double) );
            ZeroMemory( fftpIn->ImagV, Fs * sizeof(double) );
            MIO_MNG.FFT( Fs, DPtr,  fftpIn->RealV,  fftpIn->ImagV,  FALSE );
            ///

            for( y = 0; y<ys; y++ ){
                fftpIn->RealV[y] = _hypot( fftpIn->RealV[y], fftpIn->ImagV[y] );
            }
            for( y = 0; y<ys/2; y++ ){
                PPP = (xs * y) + x;
                Spect_Out[PPP] = (BYTE)round( 255 * fftpIn->RealV[ys_21 - y]/FNyq );
            }
        }
        ///
        ///
        ///
        return TRUE;
    }
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    /// MIO_MNG
    ///
    BOOL Wigner_TAD( int NFrames2, fftprepare* fftpIn ){
        Fs = 2000;
        NFrames2 = NFrames2 - NFrames2%Fs;
        Fs = 2000;
        SetTimeSeg( 1 );
        Prepare( Fs, NFrames2 );
        int n = 0;
        int k = 0;
        ///
        ///
        double* DBF = (double*)malloc( Fs * sizeof(double) + 1024 );
        ZeroMemory( DBF, Fs * sizeof(double) );
        ///
        int PPP = 0;
        int x = 0;
        const int FNyq = Fs/2;/// FNyq      Fs
        ///
        ///
        for( x=0; x<xs; x++ ){
            n = x * Fs;
            for( k = 0; k<Fs; k++ ){
                if( (n-k)<0 ){
                    DBF[k] = 0;
                    continue;
                }
                DBF[k] = fftpIn->InputV[n+k] * fftpIn->InputV[n-k];
            }
            ///
            ZeroMemory( fftpIn->RealV, Fs * sizeof(double) );
            ZeroMemory( fftpIn->ImagV, Fs * sizeof(double) );
            SetWindowing( Fs, DBF );
            MIO_MNG.FFT( Fs, DBF,  fftpIn->RealV,  fftpIn->ImagV, FALSE );
            ///
            for( k = 0; k<Fs; k++ ){
                PPP = (xs * (Fs - k - 1) ) + x;
                fftpIn->RealV[k] = _hypot( fftpIn->RealV[k], fftpIn->ImagV[k] );
                fftpIn->RealV[k] = round( 255 * fftpIn->RealV[k]/FNyq );
                Spect_Out[PPP] = ParceBYTE( fftpIn->RealV[k] );
            }
            ///
        }
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        BYTE* BPTR = (BYTE*)fftpIn->RealV;
        memcpy( BPTR, Spect_Out, x_ys + 128 );
        for( int y = 0; y<Fs/2; y++ ){
            for( int x = 0; x<xs; x++ ){
                Spect_Out[y*xs+x] = BPTR[ (Fs/2 - y - 1)*xs+x ];
            }
        }
        //*/
        ///
        free( DBF );
        return TRUE;
    }
    ///
    ///
    ///
    ///
    ///

    /// MIO_MNG
    ///
    BOOL Wigner_TAD_Compl( int NFrames2, fftprepare* fftpIn ){
        Fs = 2000;
        NFrames2 = NFrames2 - NFrames2%Fs;
        Fs = 2000;
        SetTimeSeg( 1 );
        Prepare( Fs, NFrames2 );
        int n = 0;
        int k = 0;
        ///
        ///
        double* DBF = (double*)malloc( NFrames2 * sizeof(double) + 1024 );
        ZeroMemory( DBF, NFrames2 * sizeof(double) );
        ///
        int PPP = 0;
        int x = 0;
        const int FNyq = Fs/2;/// FNyq      Fs
        ///
        ///
        for( x=0; x<xs; x++ ){
            n = x * Fs;
            ZeroMemory( DBF, Fs * sizeof(double) );
            for( k = 0; k<Fs; k++ ){
                if( (n-k)<0 ){
                    continue;
                }
                DBF[k] = fftpIn->InputV[n+k] * fftpIn->InputV[n+FNyq-k-1];
            }
            ///
            ZeroMemory( fftpIn->RealV, Fs * sizeof(double) );
            ZeroMemory( fftpIn->ImagV, Fs * sizeof(double) );
            MIO_MNG.FFT( Fs, DBF,  fftpIn->RealV,  fftpIn->ImagV, FALSE );
            ///
            ///
            ///
            for( k = 0; k<Fs; k++ ){
                PPP = (xs * (Fs - k - 1) ) + x;
                fftpIn->RealV[k] = _hypot( fftpIn->RealV[k], fftpIn->ImagV[k] );
                fftpIn->RealV[k] = round( 255 * fftpIn->RealV[k]/FNyq );
                /*
                if( fftpIn->RealV[k]>255 ){
                    fftpIn->RealV[k] = 255;
                }
                //*/
                Spect_Out[PPP] = (BYTE)fftpIn->RealV[k];
            }
            ///
        }
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        BYTE* BPTR = (BYTE*)fftpIn->RealV;
        memcpy( BPTR, Spect_Out, x_ys + 128 );
        for( int y = 0; y<Fs/2; y++ ){
            for( int x = 0; x<xs; x++ ){
                Spect_Out[y*xs+x] = BPTR[ (Fs/2 - y - 1)*xs+x ];
            }
        }
        ///
        free( DBF );
        return TRUE;
    }
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    BOOL ChoiW_TAD( int NFrames2, fftprepare* fftpIn, double sigma ){
        Fs = 2000;
        NFrames2 = NFrames2 - NFrames2%Fs;
        Fs = 2000;
        SetTimeSeg( 1 );
        Prepare( Fs, NFrames2 );
        int n = 0;
        int k = 0;
        ///
        ///
        double* DBF = (double*)malloc( Fs * sizeof(double) + 1024 );
        ZeroMemory( DBF, Fs * sizeof(double) );
        ///
        int PPP = 0;
        int x = 0;
        const int FNyq = Fs/2;/// FNyq      Fs
        const int FRet = Fs;/// FNyq      Fs
        if( FRet!=Fs ){
            Fs = Fs;
        }
        ///
        ///
        for( x=1; x<xs; x++ ){
            n = x * Fs;
            ZeroMemory( DBF, Fs * sizeof(double) );
            for( k = 0; k<Fs/2; k++ ){
                DBF[k+Fs/4] = fftpIn->InputV[n+k+1] * fftpIn->InputV[n-k-1];
            }
            ///
            ZeroMemory( fftpIn->RealV, Fs * sizeof(double) );
            ZeroMemory( fftpIn->ImagV, Fs * sizeof(double) );
            SetWindowing( Fs, DBF );
            MIO_MNG.FFT( Fs, DBF,  fftpIn->RealV,  fftpIn->ImagV, FALSE );
            ///
            for( k = 0; k<Fs; k++ ){
                // fftpIn->RealV[k] = _hypot( fftpIn->RealV[k], fftpIn->ImagV[k] );
                fftpIn->RealV[k] = sqrt( fftpIn->RealV[k] * fftpIn->RealV[k] + fftpIn->ImagV[k] * fftpIn->ImagV[k] );
            }
            for( k = 0; k<Fs; k++ ){
                PPP = (xs * (Fs - k - 1) ) + x;
                fftpIn->RealV[k] = round( 255 * fftpIn->RealV[k]/FNyq );
                /*
                if( fftpIn->RealV[k]>255 ){
                    fftpIn->RealV[k] = 255;
                }
                //*/
                Spect_Out[PPP] = (BYTE)fftpIn->RealV[k];
            }
            ///
        }
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        BYTE* BPTR = (BYTE*)fftpIn->RealV;
        memcpy( BPTR, Spect_Out, x_ys + 128 );
        for( int y = 0; y<Fs/2; y++ ){
            for( int x = 0; x<xs; x++ ){
                Spect_Out[y*xs+x] = BPTR[ (Fs/2 - y - 1)*xs+x ];
            }
        }
        ///
        free( DBF );
        return TRUE;
    }
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///
};
///
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
struct FDESIGN_CLASS{
    int                         IPtr;
    BOOL                        IsIRR;
    BOOL                        FixedFs;
    private:
    int                         Fs[2];
    int                         FNyq[2];
    int                         OrdFil[2];
    int                         TypeFil[2];
    double                      FcLH_L[2];
    double                      FcLH_H[2];
    double                      FcL[2];
    double                      FcH[2];
    double                      A[1024];
    double                      BA[1024];
    double                      B[2048];
    double                      Alpha[20];
    int                         WType;
    private:
    double                      Max_Of_FFT[2];
    double                      Min_Of_FFT[2];
    int                         Len_Of_FFT;
    ///
    double*                     FFT_OUT;
    public:
    WINDOWING                   WINDWN;
    private:
    public:
    VOID*                       Ploter;
    BOOL                        Performing_sweep;
    BOOL                        Sweep_Cancel;
    public:
    Spectogramprepare           SPecto;
    short*                      ShortTemp;
    private:
    BYTE                        TYPE_PROC;
    BYTE                        BInput;
    BOOL                        BIsOpen;
    double                      CentralValue;
    char                        CTMP;
    /// ////////////////////////////////////////////////////////////////////////////////////////////
    public:
    ///
    ~FDESIGN_CLASS(){ Free(); }
    BOOL Free( void ){
        if( FFT_OUT!=NULL ){
            free( FFT_OUT );
            FFT_OUT = NULL;
        }
        Len_Of_FFT = 0;
        SPecto.Free();
        return TRUE;
    }
    ///
    FDESIGN_CLASS(){
        IPtr = 0;
        ZeroMemory( &IPtr, sizeof(FDESIGN_CLASS) );
        Fs[0]       = 2000;
        Fs[1]       = 2000;
        FNyq[0]     = Fs[0]/2;
        FNyq[1]     = Fs[1]/2;
        OrdFil[0]   = 2;
        OrdFil[1]   = 2;
        FcL[0]      = 200;
        FcL[1]      = 200;
        ///
        FcLH_L[0]      = 500;
        FcLH_L[1]      = 500;
        FcLH_H[0]      = 500;
        FcLH_H[1]      = 500;
        ///
        FcH[0]      = 800;
        FcH[1]      = 800;
        IsIRR       = FALSE;
        FFT_OUT     = NULL;
        Ploter = NULL;
        Performing_sweep     = FALSE;
        Sweep_Cancel         = FALSE;
        CentralValue = 0.5;
        SPecto.SetTimeSeg( 1 );
    }
    /// ////////////////////////////////////////////////////////////////////////////////////////////
    private:
    BOOL SetBuff( int NPos ){
        Free();
        FFT_OUT = (double*)malloc( NPos * sizeof(double) + 1024 );
        ZeroMemory( FFT_OUT, NPos * sizeof(double) + 1024 );
        ///
        ///
        Len_Of_FFT = NPos;
        return (FFT_OUT!=NULL);
    }
    public:
    ///
    ///
    ///
    double GetCentralValue( void ){
        return CentralValue;
    }
    BOOL SetCentralValue( double V_In ){
        if( V_In>1 ){
            return FALSE;
        }
        CentralValue = V_In;
        return TRUE;
    }
    ///
    ///
    ///
    BOOL SetOpen( BOOL B_IN ){
        BIsOpen = B_IN;
        return BIsOpen;
    }
    ///
    BOOL IsOpen( void ){
        return BIsOpen;
    }
    ///
    ///
    ///
    BYTE SetTYPE_PROC( BYTE BINP ){ TYPE_PROC = BINP; return TYPE_PROC; }
    BYTE GetTYPE_PROC(){ return TYPE_PROC; }
    ///
    BYTE SetBInput( BYTE BINP ){ BInput = BINP; return BInput; }
    BYTE GetBInput(){ return BInput; }
    ///
    ///
    int  GetTypeWindow(){ return WINDWN.GetWinType(); }
    BOOL SetTypeWindow( int WTypeI ){ return WINDWN.SetWinType( WTypeI ); }
    double  GetAlphaWindow(){ return WINDWN.GetAlpha(); }
    BOOL SetAlphaWindow( double AlphaV ){ return WINDWN.SetAlpha( AlphaV ); }
    ///
    BOOL SetWindowing( int NFrames2, double* InputV ){
        return WINDWN.Apply_Window( NFrames2, InputV );
    }
    ///
    BOOL HaveWindowAlpha(){
        return WINDWN.HaveWindowAlpha();
    }
    ///
    ///
    ///
    ///
    ///
    double GetMaxFFTV( void ){
        const int IndT = 1&((int)IsIRR);
        return Max_Of_FFT[IndT];
    }
    ///
    int GetFilType( void ){
        const int IndT = 1&((int)IsIRR);
        return TypeFil[IndT];
    }
    ///
    ///
    BOOL SetFilType( int FTyp ){
        const int IndT = 1&((int)IsIRR);
        if( FTyp<0 || FTyp>3 ){
            return FALSE;
        }
        TypeFil[IndT] = FTyp;
        return TRUE;
    }
    ///
    /// FcLH_H
    double GetFcL( void ){
        const int IndT = 1&((int)IsIRR);
        if( TypeFil[IndT]<2 ){
            return FcLH_H[IndT];
        }else{
            return FcL[IndT];
        }
    }
    BOOL SetFcL( double FcLI ){
        const int IndT = 1&((int)IsIRR);
        if( TypeFil[IndT]<2 ){
            if( FcLI>=FNyq[IndT] || FcLI<1e-8 ){
                return FALSE;
            }
            FcLH_H[IndT] = FcLI;
        }else{
            if( FcLI>=FNyq[IndT] || FcLI<1e-8 || FcLI>=FcH[IndT] ){
                return FALSE;
            }
            FcL[IndT] = FcLI;
        }
        return TRUE;
    }
    ///
    ///
    double GetFcH( void ){
        const int IndT = 1&((int)IsIRR);
        if( TypeFil[IndT]<2 ){
            return FcLH_L[IndT];
        }else{
            return FcH[IndT];
        }
    }
    BOOL SetFcH( double FcHI ){
        const int IndT = 1&((int)IsIRR);
        if( TypeFil[IndT]<2 ){
            if( FcHI>=FNyq[IndT] || FcHI<1e-8 ){
                return FALSE;
            }
            FcLH_L[IndT] = FcHI;
        }else{
            if( FcHI>=FNyq[IndT] || FcHI<1e-8 || FcHI<=FcL[IndT] ){
                return FALSE;
            }
            FcH[IndT] = FcHI;
        }
        return TRUE;
    }
    ///
    ///
    int  GetWType( void ){ return WType; }
    BOOL SetWType( int WTyp ){
        if( WTyp>17 ){
            WType = 17;
            return FALSE;
        }else{
            WType = WTyp;
        }
        return TRUE;
    }
    ///
    ///
    BOOL HaveAlpha(){
        return ( ( WType==1 || WType==10 || WType==16 || WType==15 || WType==17) );
    }
    ///
    BOOL ExistOnMatlab(){
        return ( !(WType==11 || WType==12 || WType==16) );
    }
    ///
    ///
    double GetAlpha( void ){ return Alpha[WType]; }
    BOOL SetAlpha( double AlphaV ){
        if( AlphaV<0 ){
            AlphaV = 0;
        }
        Alpha[WType] = AlphaV;
        return TRUE;
    }
    ///
    ///
    ///
    ///
    BOOL GetFcHigh(){ const int IndT = 1&((int)IsIRR);return FcH[IndT]; }
    BOOL SetFcHigh( double FcHigh ){
        const int IndT = 1&((int)IsIRR);
        if( FcHigh>=FNyq[IndT] || FcHigh<=FcL[IndT] || FcHigh<=0 ){
            return FALSE;
        }
        FcH[IndT] = FcHigh;
        return TRUE;
    }
    ///
    double GetFcLow(){ const int IndT = 1&((int)IsIRR);return FcL[IndT]; }
    BOOL SetFcLow( double FcLow ){
        const int IndT = 1&((int)IsIRR);
        if( FcLow>=FNyq[IndT] || FcLow>=FcH[IndT] || FcLow<=0 ){
            /// MessageBoxA( NULL, "", "", 0 );
            return FALSE;
        }
        FcL[IndT] = FcLow;
        return TRUE;
    }
    ///
    ///
    int GetOrdFil(){
        const int IndT = 1&((int)IsIRR);
        return OrdFil[IndT];
    }
    ///
    ///
    BOOL SetOrdFil( int OrdF ){
        const int IndT = 1&((int)IsIRR);
        if( IsIRR ){
            if( OrdF>500 ){
                OrdFil[IndT] = 500;
                return FALSE;
            }
            if( OrdF<1 ){
                OrdFil[IndT] = 1;
                return FALSE;
            }
            OrdFil[IndT] = OrdF;
            return TRUE;
        }else{
            if( TypeFil[IndT]==1 || TypeFil[IndT]==3 ){
                if( (OrdF%2)==1 ){
                    OrdFil[IndT] = OrdF + 1;
                    return FALSE;
                }
            }
            ///
            if( OrdF<2 || ( OrdF>500 && IsIRR) || OrdF>512 ){
                if( !IsIRR && OrdF>512 ){
                    OrdFil[IndT] = 512;
                }
                return FALSE;
            }
            OrdFil[IndT] = OrdF;
            ///
        }
        return TRUE;
    }
    ///
    ///
    int GetFs( void ){
        const int IndT = 1&((int)IsIRR);
        return Fs[IndT];
    }
    ///
    ///
    int GetFNyq( void ){
        const int IndT = 1&((int)IsIRR);
        return FNyq[IndT];
    }
    ///
    ///
    int GetNyq( void ){
        const int IndT = 1&((int)IsIRR);
        return FNyq[IndT];
    }
    ///
    BOOL SetFs( int FsI ){
        const int IndT = 1&((int)IsIRR);
        int FNYT = FsI/2;
        if( FsI>192000 ){
            FsI = 192000;
            Fs[IndT] = FsI;
            FNyq[IndT] = Fs[IndT]/2;
            return FALSE;
        }
        if( FNYT<=FcL[IndT] || FNYT<=FcH[IndT] || FNYT<=FcLH_H[IndT] || FNYT<=FcLH_L[IndT] ){
            double FPRC = ((double)FsI)/Fs[IndT];
            FcL[IndT] = FcL[IndT] * FPRC;
            FcH[IndT] = FcH[IndT] * FPRC;
            FcLH_H[IndT] = FcLH_H[IndT] * FPRC;
            FcLH_L[IndT] = FcLH_L[IndT] * FPRC;
            if( (FsI%2)!=0 ){
                FsI = FsI + 2*( (int)(FsI<2) );
                Fs[IndT] = FsI - (FsI%2);
            }else{
                Fs[IndT] = FsI;
            }
            FNyq[IndT] = Fs[IndT]/2;
            return FALSE;
        }
        if( FsI<2 ){
            FsI = 2;
            Fs[IndT] = FsI;
            FNyq[IndT] = Fs[IndT]/2;
            FcL[IndT] = 0.1;
            FcH[IndT] = 0.8;
            FcLH_H[IndT] = 0.5;
            FcLH_L[IndT] = 0.5;
            return FALSE;
        }
        if( (FsI%2)!=0 ){
            FsI = FsI + 2*( (int)(FsI<2) );
            Fs[IndT] = FsI - (FsI%2);
            FNyq[IndT] = Fs[IndT]/2;
            return FALSE;
        }
        Fs[IndT] = FsI;
        FNyq[IndT] = Fs[IndT]/2;
        return TRUE;
    }
    ///
    ///
    ///
    double* GetBuff(){ return FFT_OUT; }
    int GetMaxBuff(){ return Len_Of_FFT; }
    ///
    ///
    ///
    ///
    BOOL Fir1Set_Coef( void ){
        ZeroMemory( B, 2048*sizeof(double) );
        double WnV[2] = { FcL[0]/FNyq[0], FcH[0]/FNyq[0] };
        if( TypeFil[0]<2 ){
            if( TypeFil[0]==0 ){ ///
                WnV[0] = FcLH_L[0]/FNyq[0];
                WnV[1] = FcLH_L[0]/FNyq[0];
            }else{
                WnV[0] = FcLH_H[0]/FNyq[0];
                WnV[1] = FcLH_H[0]/FNyq[0];
            }
        }
        fir1( OrdFil[0], WnV, TypeFil[0], WType, Alpha[WType], B );
        return TRUE;
    }
    ///
    ///
    BOOL Fir1Set( void ){
        ZeroMemory( B, 2048*sizeof(double) );
        double WnV[2] = { FcL[0]/FNyq[0], FcH[0]/FNyq[0] };
        if( TypeFil[0]<2 ){
            if( TypeFil[0]==0 ){ ///
                WnV[0] = FcLH_L[0]/FNyq[0];
                WnV[1] = FcLH_L[0]/FNyq[0];
            }else{
                WnV[0] = FcLH_H[0]/FNyq[0];
                WnV[1] = FcLH_H[0]/FNyq[0];
            }
        }
        fir1( OrdFil[0], WnV, TypeFil[0], WType, Alpha[WType], B );
        SetFFT_FIR();
        return TRUE;
    }
    ///
    ///
    ///
    ///
    ///
    BOOL IIRSet_Coef( void ){
        char Iir_Types[4][8];
        strcpy( Iir_Types[0], "low" );      strcpy( Iir_Types[1], "high" );
        strcpy( Iir_Types[2], "bandpass" ); strcpy( Iir_Types[3], "stop" );
        ZeroMemory( BA, 1024*sizeof(double) );
        ZeroMemory( A,  1024*sizeof(double) );
        double WnV[2] = { FcL[1]/FNyq[1], FcH[1]/FNyq[1] };
        if( TypeFil[1]<2 ){
            if( TypeFil[1]==0 ){ ///
                WnV[0] = FcLH_L[1]/FNyq[1];
                WnV[1] = FcLH_L[1]/FNyq[1];
            }else{
                WnV[0] = FcLH_H[1]/FNyq[1];
                WnV[1] = FcLH_H[1]/FNyq[1];
            }
        }
        butter( OrdFil[1], WnV, Iir_Types[TypeFil[1]], BA, A );
        return TRUE;
    }
    BOOL IIRSet( void ){
        char Iir_Types[4][16];
        strcpy( Iir_Types[0], "low" );      strcpy( Iir_Types[1], "high" );
        strcpy( Iir_Types[2], "bandpass" ); strcpy( Iir_Types[3], "stop" );
        ZeroMemory( BA, 1024*sizeof(double) );
        ZeroMemory( A,  1024*sizeof(double) );
        double WnV[2] = { FcL[1]/FNyq[1], FcH[1]/FNyq[1] };
        if( TypeFil[1]<2 ){
            if( TypeFil[1]==0 ){ ///
                WnV[0] = FcLH_L[1]/FNyq[1];
                WnV[1] = FcLH_L[1]/FNyq[1];
            }else{
                WnV[0] = FcLH_H[1]/FNyq[1];
                WnV[1] = FcLH_H[1]/FNyq[1];
            }
        }
        butter( OrdFil[1], WnV, Iir_Types[TypeFil[1]], BA, A );
        SetFFT_IIR();
        return TRUE;
    }
    ///
    BOOL SetFFT_IIR( void ){
        ///
        int FsL = Fs[1];
        while( FsL<1000 ){
            FsL = 10*FsL;
        }
        double* B_FFT = (double*)malloc( FsL * sizeof(double) + 1024 );
        ZeroMemory( B_FFT, FsL * sizeof(double) );
        double* RealV = (double*)malloc( FsL * sizeof(double) + 1024 );
        ZeroMemory( RealV, FsL * sizeof(double) );
        double* ImagV = (double*)malloc( FsL * sizeof(double) + 1024 );
        ZeroMemory( ImagV, FsL * sizeof(double) );
        SetBuff( FsL );
        ///
        memcpy( B_FFT, BA, 1024 * sizeof(double) );
        memcpy( FFT_OUT, A, 1024 * sizeof(double) );
        ///
        MIO_MNG.FFT( Len_Of_FFT, B_FFT, RealV, ImagV, FALSE );
        for( int i=0; i<Len_Of_FFT; i++ ){
            B_FFT[i] = sqrt( RealV[i] * RealV[i] + ImagV[i] * ImagV[i] );
        }
        ///
        MIO_MNG.FFT( Len_Of_FFT, FFT_OUT, RealV, ImagV, FALSE );
        Max_Of_FFT[0] = -1;
        Min_Of_FFT[0] = 1e100;
        for( int i=0; i<Len_Of_FFT; i++ ){
            FFT_OUT[i] = sqrt( RealV[i] * RealV[i] + ImagV[i] * ImagV[i] );
            //*
            if( isinf(FFT_OUT[i]) || FFT_OUT[i]<1e-100 ){
                FFT_OUT[i] = 0;
            }else{
                FFT_OUT[i] = B_FFT[i]/FFT_OUT[i];
            }
            //*/
            //FFT_OUT[i] = B_FFT[i]/FFT_OUT[i];
            Max_Of_FFT[1] = fmax( FFT_OUT[i], Max_Of_FFT[1] );
            Min_Of_FFT[1] = fmin( FFT_OUT[i], Min_Of_FFT[1] );
        }
        ///
        ///
        ///
        ///
        ///Min_Of_FFT[0] = 1e100;
        for( int i=0; i<Len_Of_FFT; i++ ){
            ///FFT_OUT[i] = FFT_OUT[i]/Max_Of_FFT[0];
            ///Min_Of_FFT[0] = fmin( Min_Of_FFT[0], FFT_OUT[i] );
        }
        Max_Of_FFT[0] = 1;
        ///
        ///
        free( B_FFT );
        free( RealV );
        free( ImagV );
        return TRUE;
    }
    ///
    ///
    ///
    ///
    ///
    BOOL SetFFT_FIR( void ){
        int FsL = Fs[0];
        while( FsL<1000 ){
            FsL = 10*FsL;
        }
        double* RealV = (double*)malloc( FsL * sizeof(double) + 1024 );
        ZeroMemory( RealV, FsL * sizeof(double) );
        double* ImagV = (double*)malloc( FsL * sizeof(double) + 1024 );
        ZeroMemory( ImagV, FsL * sizeof(double) );
        SetBuff( FsL );
        ///
        memcpy( FFT_OUT, B, 512 * sizeof(double) );
        //
        /// FFT_OUT[0] = 0.5;FFT_OUT[1] = 0.5;
        MIO_MNG.FFT( Len_Of_FFT, FFT_OUT, RealV, ImagV, FALSE );
        ///
        Max_Of_FFT[0] = -1;
        Min_Of_FFT[0] = 1e100;
        for( int i=0; i<Len_Of_FFT; i++ ){
            FFT_OUT[i] = sqrt( RealV[i] * RealV[i] + ImagV[i] * ImagV[i] );
            Max_Of_FFT[0] = fmax( Max_Of_FFT[0], FFT_OUT[i] );
            Min_Of_FFT[0] = fmin( Min_Of_FFT[0], FFT_OUT[i] );
        }
        /// char T2T[128] = {0};sprintf( T2T, "%i", Len_Of_FFT );MessageBoxA( NULL, T2T, " ", 0 );
        Min_Of_FFT[0] = 1e100;
        for( int i=0; i<Len_Of_FFT; i++ ){
            FFT_OUT[i] = FFT_OUT[i]/Max_Of_FFT[0];
            Min_Of_FFT[0] = fmin( Min_Of_FFT[0], FFT_OUT[i] );
        }
        Max_Of_FFT[0] = 1;
        ///
        ///
        free( RealV );
        free( ImagV );
        return TRUE;
    }
    ///
    ///
    ///
    ///
    ///
    BOOL CpyExampleFir(){
        ///
        static char Fir_Types[4][8];
        strcpy( Fir_Types[0], "low" );      strcpy( Fir_Types[1], "high" );
        strcpy( Fir_Types[2], "DC-0" );     strcpy( Fir_Types[3], "DC-1" );
        /// strcpy( Iir_Types[0], "low" );      strcpy( Iir_Types[1], "high" );
        /// strcpy( Iir_Types[2], "bandpass" ); strcpy( Iir_Types[3], "stop" );
        ///
        int Len_AB = OrdFil[0];
        char TUnit[1024] = {0};
        char* T2T = (char*)malloc( 1024 * 1024 );
        if( T2T==NULL ){
            return FALSE;
        }
        // sprintf(T2T, "%i", Len_AB );MessageBox( HWInfoFrq, T2T, "", 0 );
        /// B Coef
        sprintf( T2T, "clc;\n\n%%Coeficientes del programa.\nb = [ " );
        for( int i=0; i<Len_AB-1; i++ ){
            ///sprintf( TUnit, "%5.19f, ", b_iir[i] );
            sprintf( TUnit, "%5.19e, ", B[i] );
            strcat( T2T, TUnit );
        }
        //sprintf( TUnit, "%5.19f ]; \n", b_iir[Len_AB-1] );
        sprintf( TUnit, "%5.19e ]; \n", B[Len_AB-1] );
        strcat( T2T, TUnit );
        ///
        ///
        ///
        if( TypeFil[0]<2 ){
            if( TypeFil[0]==0 ){
                sprintf( TUnit, "Fs = %i;\t\t\t\t\t\t\t%%Frecuencia de muestreo.\n"
                                "FNyq = Fs/2;\t\t\t\t\t\t%%Frecuencia de Nyquist.\n"
                                "[Hfir1 f] = freqz( b, 1, FNyq, Fs ); %%Respuesta en frecuencia del filtro(fft(b)).\n"
                                "Hfir1 = abs(Hfir1);\t\t\t\t\t%%Magnitud de la repuesta.\n"
                                "\n\n\nn = %i;\t\t\t\t\t\t\t\t%%Orden del filtro a implementar.\n"
                                "fc = %g;\t\t\t\t\t\t\t%%Frecuencia de corte.\n",
                        Fs[0], Len_AB,
                        FcLH_H[0] );
            }else{
                sprintf( TUnit, "Fs = %i;\t\t\t\t\t\t\t%%Frecuencia de muestreo.\n"
                                "FNyq = Fs/2;\t\t\t\t\t\t%%Frecuencia de Nyquist.\n"
                                "[Hfir1 f] = freqz( b, 1, FNyq, Fs ); %%Respuesta en frecuencia del filtro(fft(b)).\n"
                                "Hfir1 = abs(Hfir1);\t\t\t\t\t%%Magnitud de la repuesta.\n"
                                "\n\n\nn = %i;\t\t\t\t\t\t\t\t%%Orden del filtro a implementar.\n"
                                "fc = %g;\t\t\t\t\t\t\t%%Frecuencia de corte.\n",
                        Fs[0], Len_AB,
                        FcLH_L[0] );
            }
            strcat( T2T, TUnit );
        }else{
            sprintf( TUnit, "Fs = %i;\t\t\t\t\t\t\t%%Frecuencia de muestreo.\n"
                            "FNyq = Fs/2;\t\t\t\t\t\t%%Frecuencia de Nyquist.\n"
                            "[Hfir1 f] = freqz( b, 1, FNyq, Fs ); %%Respuesta en frecuencia del filtro(fft(b)).\n"
                            "Hfir1 = abs(Hfir1);\t\t\t\t\t%%Magnitud de la repuesta.\n"
                            "\n\n\nn = %i;\t\t\t\t\t\t\t\t%%Orden del filtro a implementar.\n"
                            "fc = [%g %g];\t\t\t\t\t\t%%Frecuencia de corte.\n",
                    Fs[0], Len_AB,
                    FcL[0], FcH[0] );
            strcat( T2T, TUnit );
        }
        ///
        /// gausswin(
        ///  IWind_C, Alpha_C[IWind_C]
        if( WType==0 || (WType>1 && WType<9) || (WType>12 && WType<16) ){
            sprintf( TUnit, "b0 = fir1( n, fc/FNyq, '%s' %s( n + 1 ) );%%Filtro fir\n"
                            "H = freqz( b0, 1, FNyq, Fs );\t\t%%Respuesta en frecuencia del filtro(fft(b0)).\n"
                            "H = abs(H);\t\t\t\t\t\t\t%%Magnitud de la repuesta.\n",
                                    Fir_Types[TypeFil[0]], Wind_2_Fir_name[WType] );
        }else{
            if( WType==1 || WType==10 || WType==17 ){
                sprintf( TUnit, "b0 = fir1( n, fc/FNyq, '%s' %s( n + 1, %g ) );%%Filtro fir\n"
                                "H = freqz( b0, 1, FNyq, Fs );\t\t%%Respuesta en frecuencia del filtro(fft(b0)).\n"
                                "H = abs(H);\t\t\t\t\t\t\t%%Magnitud de la repuesta.\n",
                                        Fir_Types[TypeFil[0]], Wind_2_Fir_name[WType], Alpha[WType] );
            }else{
                sprintf( TUnit, "%%La venatana %s no est� implementada en matlab.\n"
                                "b0 = fir1( n, fc/FNyq, '%s' );%%Filtro fir\n"
                                "H = freqz( b0, 1, FNyq, Fs );\t\t%%Respuesta en frecuencia del filtro(fft(b0)).\n"
                                "H = abs(H);\t\t\t\t\t\t\t%%Magnitud de la repuesta.\n",
                                        Wind_2_Fir[WType], Fir_Types[TypeFil[0]] );
            }
        }
        strcat( T2T, TUnit );
        //
        sprintf( TUnit, "plot( f, H, 'LineWidth', 4 ); %%Gr�fixa del filtro(Matlab)\n"
                        "hold on;\n"
                        "plot( f, Hfir1, 'r', 'LineWidth', 2 );%%Gr�fixa del filtro(TLAB)\n"
                        "hold off;\n"
                        "ylim( [0 max(max(Hfir1), max(H))+0.1] );\n"
                        "xlim( [0 %i] )\n",
                                (int)(FNyq[0])  );
        strcat( T2T, TUnit );
        ///
        ///
        //MessageBox( HWInfoFrq, T2T, "", 0 );
        Copy2ClipBoard2( T2T );
        free( T2T );
        return TRUE;
        ///
    }
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BOOL CpyExampleIIR(){
        static char Iir_Types[4][8];
        strcpy( Iir_Types[0], "low" );      strcpy( Iir_Types[1], "high" );
        strcpy( Iir_Types[2], "bandpass" ); strcpy( Iir_Types[3], "stop" );
        int Len_AB = OrdFil[1] + 1;
        char TUnit[1024] = {0};
        if( TypeFil[1]>1 ){
            Len_AB = 2 * OrdFil[1] + 1;
        }
        char* T2T = (char*)malloc( 1024 * 1024 );
        if( T2T==NULL ){
            return FALSE;
        }
        //sprintf(T2T, "%i", Len_AB );
        //MessageBox( HWInfoFrq, T2T, "", 0 );
        /// B Coef
        sprintf( T2T, "clc;\n\n%%Coeficientes del programa.\nb = [ " );
        for( int i=0; i<Len_AB-1; i++ ){
            ///sprintf( TUnit, "%5.19f, ", b_iir[i] );
            sprintf( TUnit, "%5.19e, ", BA[i] );
            strcat( T2T, TUnit );
        }
        //sprintf( TUnit, "%5.19f ]; \n", b_iir[Len_AB-1] );
        sprintf( TUnit, "%5.19e ]; \n", BA[Len_AB-1] );
        strcat( T2T, TUnit );
        /// A Coef
        sprintf( TUnit, "a = [ " );
        strcat( T2T, TUnit );
        for( int i=0; i<Len_AB-1; i++ ){
            sprintf( TUnit, "%5.22f, ", A[i] );
            strcat( T2T, TUnit );
        }
        sprintf( TUnit, "%5.22f ]; \n\n", A[Len_AB-1] );
        strcat( T2T, TUnit );
        ///
        ///
        ///
        if( TypeFil[1]<2 ){
            if( TypeFil[1]==0 ){
                sprintf( TUnit, "Fs = %i;\t\t\t\t\t\t\t%%Frecuencia de muestreo.\n"
                                "FNyq = Fs/2;\t\t\t\t\t\t%%Frecuencia de Nyquist.\n"
                                "[Hbut f] = freqz( b, a, FNyq, Fs ); %%Respuesta en frecuencia del filtro(fft(b)/fft(a)).\n"
                                "Hbut = abs(Hbut);\t\t\t\t\t%%Magnitud de la repuesta.\n"
                                "\n\n\nn = %i;\t\t\t\t\t\t\t\t%%Orden del filtro a implementar.\n"
                                "fc = %g;\t\t\t\t\t\t\t%%Frecuencia de corte.\n",
                        Fs[1], OrdFil[1],
                        FcLH_L[1] );
            }else{
                sprintf( TUnit, "Fs = %i;\t\t\t\t\t\t\t%%Frecuencia de muestreo.\n"
                                "FNyq = Fs/2;\t\t\t\t\t\t%%Frecuencia de Nyquist.\n"
                                "[Hbut f] = freqz( b, a, FNyq, Fs ); %%Respuesta en frecuencia del filtro(fft(b)/fft(a)).\n"
                                "Hbut = abs(Hbut);\t\t\t\t\t%%Magnitud de la repuesta.\n"
                                "\n\n\nn = %i;\t\t\t\t\t\t\t\t%%Orden del filtro a implementar.\n"
                                "fc = %g;\t\t\t\t\t\t\t%%Frecuencia de corte.\n",
                        Fs[1], OrdFil[1],
                        FcLH_H[1] );
            }
            /// MessageBoxA( NULL, TUnit, "", 0 );
            strcat( T2T, TUnit );
        }else{
            sprintf( TUnit, "Fs = %i;\t\t\t\t\t\t\t%%Frecuencia de muestreo.\n"
                            "FNyq = Fs/2;\t\t\t\t\t\t%%Frecuencia de Nyquist.\n"
                            "[Hbut f] = freqz( b, a, FNyq, Fs ); %%Respuesta en frecuencia del filtro(fft(b)/fft(a)).\n"
                            "Hbut = abs(Hbut);\t\t\t\t\t%%Magnitud de la repuesta.\n"
                            "\n\n\nn = %i;\t\t\t\t\t\t\t\t%%Orden del filtro a implementar.\n"
                            "fc = [%g %g];\t\t\t\t\t\t%%Frecuencia de corte.\n",
                    Fs[1], OrdFil[1],
                    FcL[1], FcH[1] );
            strcat( T2T, TUnit );
        }
        sprintf( TUnit, "[b0 a0] = butter( n, fc/FNyq, '%s' );%%Filtro Butterworth\n"
                        "H = freqz( b0, a0, FNyq, Fs );\t\t%%Respuesta en frecuencia del filtro(fft(b0)/fft(a0)).\n"
                        "H = abs(H);\t\t\t\t\t\t\t%%Magnitud de la repuesta.\n",
                                Iir_Types[TypeFil[1]] );
        strcat( T2T, TUnit );
        //
        sprintf( TUnit, "plot( f, H, 'LineWidth', 4 ); %%Gr�fixa del filtro(Matlab)\n"
                        "hold on;\n"
                        "plot( f, Hbut, 'r', 'LineWidth', 2 );%%Gr�fixa del filtro(TLAB)\n"
                        "hold off;\n"
                        "ylim( [0 max(max(Hbut), max(H))+0.1] );\n"
                        "xlim( [0 %i] )\n",
                                (int)(FNyq[1])  );
        strcat( T2T, TUnit );
        ///
        ///
        //MessageBox( HWInfoFrq, T2T, "", 0 );
        Copy2ClipBoard2( T2T );
        Sleep( 1 );
        /// MessageBoxA( NULL, T2T, "", 0 );
        free( T2T );
        return TRUE;
    }
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BOOL CpyExample(){
        if( IsIRR ){
            return CpyExampleIIR();
        }else{
            return CpyExampleFir();
        }
        return TRUE;
    }
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BOOL Apply_Filter( int NFrames2, fftprepare* fftpIn ){
        /// MessageBoxA( NULL, "", "", 0 );
        if( NFrames2< 2048 ){ NFrames2 = 2048; }
        if( (NFrames2%2)!=0 ){ NFrames2 = NFrames2 + 1; }
        fftprepare fftpB;
        fftprepare fftpA;
        double DValA = 0;
        double DValB = 0;
        if( IsIRR ){
            //*
            fftpA.Prepare( NFrames2 );
            fftpB.Prepare( NFrames2 );
            ///
            memcpy( fftpA.InputV,  A, 1024 * sizeof(double) );
            memcpy( fftpB.InputV, BA, 1024 * sizeof(double) );
            ///
            MIO_MNG.FFT( NFrames2, fftpIn->InputV,  fftpIn->RealV,  fftpIn->ImagV,  FALSE );
            MIO_MNG.FFT( NFrames2, fftpA.InputV,    fftpA.RealV,    fftpA.ImagV,    FALSE );
            MIO_MNG.FFT( NFrames2, fftpB.InputV,    fftpB.RealV,    fftpB.ImagV,    FALSE );
            ///
            for( int i=0; i<NFrames2; i++ ){
                DValA = sqrt( fftpA.RealV[i] * fftpA.RealV[i] + fftpA.ImagV[i] * fftpA.ImagV[i] );
                DValB = sqrt( fftpB.RealV[i] * fftpB.RealV[i] + fftpB.ImagV[i] * fftpB.ImagV[i] );
                DValB = DValB/DValA;
                fftpIn->RealV[i] = fftpIn->RealV[i] * DValB;
                fftpIn->ImagV[i] = fftpIn->ImagV[i] * DValB;
                if( isinf(fftpIn->RealV[i]) || isinf(fftpIn->ImagV[i]) ){
                    fftpIn->RealV[i] = 0;
                    fftpIn->ImagV[i] = 0;
                }
            }
            ///
            ZeroMemory( fftpIn->InputV, NFrames2 * sizeof(double)+8 );
            MIO_MNG.IFFT( NFrames2, fftpIn->RealV, fftpIn->ImagV, fftpIn->InputV );
            ///
            //*/
        }else{
            fftpB.Prepare( NFrames2 );
            memcpy( fftpB.InputV, B, 2048 * sizeof(double) );
            MIO_MNG.FFT( NFrames2, fftpB.InputV,    fftpB.RealV,    fftpB.ImagV,    FALSE );
            MIO_MNG.FFT( NFrames2, fftpIn->InputV,  fftpIn->RealV,  fftpIn->ImagV,  FALSE );
            for( int i=0; i<NFrames2; i++ ){
                DValB = sqrt( fftpB.RealV[i] *fftpB.RealV[i] + fftpB.ImagV[i] * fftpB.ImagV[i] );
                fftpIn->RealV[i] = fftpIn->RealV[i] * DValB;
                fftpIn->ImagV[i] = fftpIn->ImagV[i] * DValB;
            }
            MIO_MNG.IFFT( NFrames2, fftpIn->RealV, fftpIn->ImagV, fftpIn->InputV );
        }
        fftpB.Free();
        fftpA.Free();
        return TRUE;
    }
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ///
    ///
    ///
    ///
};

