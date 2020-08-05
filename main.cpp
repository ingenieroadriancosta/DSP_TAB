/// driverquery | more >asdfert.txt
/// #define MAKEDSPTAD 0
#include "DSP_TAB_ALL\DSP_TAB_MAIN.cpp"

#include "PruebVari.cpp"
#include "PLOTMAIN.cpp"
/// ConstBaudRate
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd ){
    if( lpCmdLine!=NULL ){
        char TT2[1024] = {0};
        strcpy( TT2, lpCmdLine );
        if( atoi(TT2)==-1 ){
            return -20;
        }
    }
    /// InitCommonControls();
    ShowWindow( GetConsoleWindow(), 0 );
    HWND HWL = FindWindowA( NULL, " DSP TAD " );
    if( HWL!=NULL ){
        ShowWindow( HWL, 0 );
        Sleep( 500 );
        ShowWindow( HWL, SW_SHOWMINIMIZED );
        Sleep( 500 );
        ShowWindow( HWL, 3 );
        Sleep( 500 );
        ShowWindow( HWL, SW_RESTORE );
        return 0;
    }
    SetWindowTextA( GetConsoleWindow(), " CON DSP TAD " );
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////
    ///
    #ifdef MAKEDSPTAD
        /// DWORD IdTh = 0;
    /// PruebVari();
    ///
    //CreateThread( NULL, 0, PruebVari, NULL, 0, &IdTh );
    if( strcmp( getenv( "USERNAME" ), "ADRIAN COSTA" )==0 ){
        ///CreateThread( NULL, 0, RECSIMUL, NULL, 0, &IdTh );
    }
    /// Dlg_ADQ.MsgBoxF( "%i", sizeof(FDESIGN_CLASS)/1024 );
    ///Dlg_ADQ.Show();Dlg_ADQ.MsgBoxF( "sds" );
    ///Dlg_ADQ.Show();Dlg_ADQ.MsgBoxF( "sds" );
    ///
    ///
    /// Spectogramprepare SPFP;SPFP.Prepare( 192000, 10 * 1024 * 1024, 1 );Dlg_ADQ.MsgBoxF( "%i", SPFP.x_y/(1024*1024) );exit( 0 );
    ///
    /*
    PLOTEOS PltT;
    CreateThread( NULL, 0, PLOTMAIN, (PVOID)&PltT, 0, &IdTh );
    Sleep( 1000 );
    const int MAxP = 16 * 1024;
    double* DBL = (double*)malloc( MAxP * sizeof(double) + 1024 );
    fftprepare ftp;
    ftp.Prepare( MAxP );
    for( int i=0; i<MAxP; i++ ){
        DBL[i] = ftp.InputV[i] = Heart[1023&i]/1023.0;
    }
    /// MIO_MNG.FFT( 1024, ftp.InputV, ftp.RealV, ftp.ImagV, FALSE );
    MIO_MNG.FFT_Dll( MAxP, ftp.InputV, ftp.RealV, ftp.ImagV, FALSE );
    for( int i=0; i<MAxP; i++ ){
        ftp.RealV[i] = sqrt( ftp.RealV[i] * ftp.RealV[i] + ftp.ImagV[i] * ftp.ImagV[i] );
    }
    PltT.PLOTDOUBLE( ftp.RealV, MAxP, 0, FALSE );
    MIO_MNG.SaveDoubleVar( "DFTN", ftp.RealV, MAxP );
    MIO_MNG.SaveDoubleVar( "ORIGEN", DBL, MAxP );
    Sleep( 3000 );
    exit( 0 );
    //*/
    ///
    ///
    ///
    ///
    /*
    PLOTEOS PltT;
    CreateThread( NULL, 0, PLOTMAIN, (PVOID)&PltT, 0, &IdTh );
    Sleep( 100 );
    ///
    int N = MaxFRM;
    double* DBL = (double*)malloc( N * sizeof(double) + 1024 );
    for( int i=0; i<MaxFRM; i++ ){
        DBL[i] = sin(  100 * 2 * pi_dbl * ((double)i)/1024.0  );
        DBL[i] = ( DBL[i] + 1 )/2;
    }
    PltT.PLOTDOUBLE( DBL, MaxFRM, 0, FALSE );
    ///         N, b,   WinType, alphaV )
    char T2Temp[1024] = {0};
    char T2T[4*1024] = {0};
    for( int iW=0; iW<17; iW++ ){
        MIO_MNG.tic( 0 );
        Window_F(   N, DBL,       iW,   3 );
        for( int i=0; i<MaxFRM; i++ ){
            DBL[i] = ( DBL[i] + 1 )/2;
        }
        MIO_MNG.toc( 0 );
        sprintf( T2Temp, " %i  --  %f   \n", iW, MIO_MNG.EndToc );
        strcat( T2T, T2Temp );
        PltT.PLOTDOUBLE( DBL, MaxFRM, 0, FALSE );
        for( int i=0; i<MaxFRM; i++ ){
            DBL[i] = sin(  100 * 2 * pi_dbl * ((double)i)/1024.0  );
        }
        // MessageBoxA( PltT.GetHandleGraph(), T2T, " ", 0 );
        /// MessageBoxA( NULL, T2T, " ", 0 );
    }
    MessageBoxA( PltT.GetHandleGraph(), T2T, " ", 0 );
    free( DBL );
    exit( 0 );
    //*/
    ///
    ///
    ///
    ///
    ///
    /// Dlg_ADQ.MsgBoxF( " " );
    FDESIGN_CLASS FDST;
    FDST.IsIRR = TRUE;
    FDST.FixedFs = FALSE;
    FDST.SetWType( 0 );
    FDST.SetFilType( 2 );
    FDST.SetOrdFil( 7 );
    FDST.SetFs( 2000 );
    FDST.SetFcHigh( 0.4*FDST.GetFs() );
    FDST.SetFcLow(  0.1*FDST.GetFs() );
    ///
    ///
    ///
    SIGNAL_CENTER( Dlg_ADQ.HW_DLG_MAIN, &FDST );exit( 0 );
    ///
    /// Dlg_ADQ.MsgBoxF( "%i", FDST.GetNyq() );
    ///
    // FDST.IsIRR = TRUE;
    /// FDESIGN( Dlg_ADQ.HW_DLG_MAIN, &FDST );exit( 0 );
    ///
    /// FSPECTOGRAM( Dlg_ADQ.HW_DLG_MAIN, &FDST );exit( 0 );
    ///
    /// FWINDOWING( Dlg_ADQ.HW_DLG_MAIN, &FDST );exit( 0 );
    ///
    ///
    ///
    /// int ResP = DialogBoxParam( GetModuleHandle(NULL), MAKEINTRESOURCEA(Dial_Change_Proc_1D_TAD), NULL, DlgProc_Proc_1D_TAD, (LPARAM)&FDST );Dlg_ADQ.MsgBoxF( "\nResP = %i\n", ResP );exit( 0 );
    ///
    ///
    ///
    /*
    double nbd = 0;
    MIO_MNG.tic( 3 );
    for( int n=0; n<10*1024*1024; n++ ){
        /// nbd = nbd + _hypot( n, n );
        nbd = nbd + sqrt( n*n + n * n );
    }
    MIO_MNG.toc(3);
    Dlg_ADQ.MsgBoxF( "%f\n%f Seg.", nbd, MIO_MNG.EndToc );exit( 0 );
    //*/
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    // DialogBoxParam( GetModuleHandle(NULL), MAKEINTRESOURCEA(Dial_About), NULL, DlgProc_Proc_ABOUT, 0 );exit( 0 );
    /// Dlg_ADQ.MsgBoxF( "%i", 9<<3 );exit(0);
    ///
    ///
    /// Dlg_ADQ.SaveProc();exit( 0 );
    //*/
    ///
    ///
    ///
    ///
    ///
    /*
    const char* T2DLG =
                    "Advetencia:\n"
                    "Se escribirá el código de procesamiento de cada canal en lenguaje de programación MATLAB "
                    " y otros archivos de funciones con extensión (.m) y datos de extensión (.mat) adicionales, "
                    "elija una carpeta vacia para que no pierda trabajos anteriores.\n\n"
                    "Los archivos que se crearán son:\n"
                    ///
                    "%s\n"
                    "%s\n"
                    "wigner.p\n"
                    "choiwilliam.p\n"
                    "ApplyFilter.p\n"
                    ///
                    "\n\n\nSeleccione los canales a procesar.";
    DialogBoxParam( GetModuleHandle(NULL), MAKEINTRESOURCEA(Dial_SAVE_ADV),
                           NULL, DlgProc_Proc_SAVE_ADV, (LPARAM)T2DLG );
    exit( 0 );
    //*/
    ///
    ///
    ///
    ///
    #endif // MAKEDSPTAD
    /// Dlg_ADQ.Show();Dlg_ADQ.MsgBoxF( " " );
    return !Dlg_ADQ.Show();
    /// return DSP_TAB_MAIN(NULL);
}
