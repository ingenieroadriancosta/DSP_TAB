#if (_WIN32_WINNT < 0x0500)
    #undef _WIN32_WINNT
    #define  _WIN32_WINNT 0x0500
#endif
///
#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <direct.h>
#include <sys/stat.h>
#include <time.h>
///
#include "libmatio.h"
/// #include "libmatio.cpp"
/// #include "zlib1.cpp"
///
///
#include "matio.h"
#include "fftw3.h"
///
///
struct fftprepare{
    int                 NPosP;
    double*             InputV;
    double*             RealV;
    double*             ImagV;
    double              MaxVal;
    fftprepare(){
        NPosP = 0;
        ZeroMemory( &NPosP, sizeof(fftprepare) );
        MaxVal = -1e100;
        Free();
    }
    BOOL Free(){
        if( RealV!=NULL ){
            free( RealV );
            RealV = NULL;
        }
        if( ImagV!=NULL ){
            free( ImagV );
            ImagV = NULL;
        }
        if( InputV!=NULL ){
            free( InputV );
            InputV = NULL;
        }
        NPosP = 0;
        return TRUE;
    }
    BOOL Prepare( int NPos ){
        if( NPos<1 ){
            return FALSE;
        }
        if( NPosP==NPos ){
            /*
            if( RealV ){
                ZeroMemory( RealV, sizeof(double) * NPos + 1024 );
            }
            if( ImagV ){
                ZeroMemory( ImagV, sizeof(double) * NPos + 1024 );
            }
            if( InputV ){
                ZeroMemory( InputV, sizeof(double) * NPos + 1024 );
            }
            //*/
            return TRUE;
        }
        Free();
        RealV = (double*)malloc( sizeof(double) * NPos + 1024 );
        ImagV = (double*)malloc( sizeof(double) * NPos + 1024 );
        InputV = (double*)malloc( sizeof(double) * NPos + 1024 );
        if( RealV ){
            ZeroMemory( RealV, sizeof(double) * NPos + 1024 );
        }
        if( ImagV ){
            ZeroMemory( ImagV, sizeof(double) * NPos + 1024 );
        }
        if( InputV ){
            ZeroMemory( InputV, sizeof(double) * NPos + 1024 );
        }
        NPosP = NPos;
        return ( RealV!=NULL && ImagV!=NULL && InputV!=NULL );
    }
    ///
    ///
};
///
///
///
///
///
///
struct VarsInfo{
    int                         IPtr;
    int                         LenOfV;
    char*                       VName;
    ~VarsInfo(){
        Free();
    }
    VarsInfo(){
        IPtr        = 0;
        LenOfV      = 0;
        VName = NULL;
        MallocC(1);
    }
    BOOL Free(){
        if( VName!=NULL ){
            free( VName );
            VName = NULL;
        }
        return TRUE;
    }
    BOOL MallocC( int NBytes ){
        Free();
        VName = (char*)malloc( NBytes+1 );
        ZeroMemory( VName, NBytes );
        return (VName!=NULL);
    }
    BOOL CpyStr( LPCSTR StrIn ){
        MallocC( strlen(StrIn) );
        strcpy( VName, StrIn );
        return TRUE;
    }

};
///
///
///
typedef LPVOID (__stdcall * DLLMATIO)( ... );
BOOL CALLBACK Dlg_Proc_Select_Mat_Var( HWND, UINT, WPARAM, LPARAM );
struct mat_io{
    int                             StrPtr;
    HWND                            HWCon;
    HWND                            HWActiWin;
    private:
    char                            PATH[MAX_PATH];
    char                            UName[MAX_PATH];
    HMODULE                         hMod_libmatio;
    HMODULE                         hMod_fft;
    ///
    double                          TIME_SERIES_CTIC[1024];//VARIABLES GLOBALES DE TIEMPO.
    double                          TIME_SERIES_CTOC[1024];//VARIABLES GLOBALES DE TIEMPO.
    public:
    double                          EndToc;
    ///private:
    ///
    DLLMATIO                        Mat_Open;
    DLLMATIO                        Mat_VarWrite;
    DLLMATIO                        Mat_VarCreate;
    DLLMATIO                        Mat_VarFree;
    DLLMATIO                        Mat_VarReadNextInfo;
    DLLMATIO                        Mat_VarReadInfo;
    DLLMATIO                        Mat_VarRead;
    DLLMATIO                        Mat_Close;
    ///
    DLLMATIO                        fftw_malloc;
    DLLMATIO                        fftw_plan_dft_1d;
    DLLMATIO                        fftw_execute;
    DLLMATIO                        fftw_free;
    DLLMATIO                        fftw_destroy_plan;
    ///
    DLLMATIO                        FFT_Dll;
    DLLMATIO                        IFFT_Dll;
    ///
    public:
        DLLMATIO                        FFT_FFT;
    ///
    ~mat_io(){
        FreeLibrary( hMod_libmatio );
        FreeLibrary( hMod_fft );
    }
    mat_io(){
        AllocConsole();
        HWCon = GetConsoleWindow();
        ShowWindow( HWCon, 0 );
        char TempChar[1024] = {0};
        static BOOL CreatedLibAll = FALSE;
        DWORD PDW = 0;
        GetUserNameA( UName, &PDW );
        sprintf( PATH, "%s\\TLAB\\", getenv( "USERPROFILE" )  );
        mkdir( PATH );
        SetCurrentDirectoryA( PATH );
        sprintf( TempChar, "CD \"%s\"", PATH );
        system( TempChar );
        Write_Resource( "msvcr100.dll", DSP_MSVCR100, RCFILE );
        if( CreatedLibAll==FALSE ){
            CreatedLibAll = TRUE;
            ///
            ///
            HWCon = GetConsoleWindow();
            GetUserNameA( UName, &PDW );
            sprintf( PATH, "%s\\TLAB\\", getenv( "USERPROFILE" )  );
            /// ShowError( PATH, 0 );
            mkdir( PATH );
            SetCurrentDirectoryA( PATH );
            Write_Resource( "AFILES.zip", AFILES_ZIP, RCFILE  );
            Write_Resource( "test.dll", LIBUNZIP_DLL, RCFILE   );
            /// Write_fzlib1();
        }
        StrPtr = 0;
        HWActiWin = NULL;
        sprintf( PATH, "%s\\TLAB\\", getenv( "USERPROFILE" ) );
        ///
        ///
        /// MessageBoxA( NULL, PATH, " ", 0 );
        ///
        PDW = MAX_PATH;
        GetUserNameA( UName, &PDW );
        mkdir( PATH );
        HWCon = GetConsoleWindow();
        SetCurrentDirectory( PATH );
        sprintf( TempChar, "%stest.dll" , PATH );
        HMODULE hMod = LoadLibrary( TempChar );
        DLLMATIO UnZip       = LoadAnyFunc( hMod, "UnZip" );
        sprintf( TempChar, "%sAFILES.zip" , PATH );
        UnZip( TempChar, PATH );
        FreeLibrary( hMod );
        DeleteFileA( TempChar );
        ///
        ///
        /*
        sprintf( TempChar, "%s%s" , PATH, "FFT.dll" );
        hMod_fft = LoadLibrary( TempChar );
        FFT_FFT       = LoadAnyFunc( hMod_fft, "FFT" );
        ParseErrNull( (VOID*)FFT_FFT, -1, "FFT_FFT" );
        return;
        //*/
        ///
        ///
        sprintf( TempChar, "%slibmatio.dll" , PATH );
        /// ShowError( TempChar, 0 );
        hMod_libmatio = LoadLibrary( TempChar );
        ParseErrNull( (VOID*)hMod_libmatio, -1, "LoadLibrary load" );
        Mat_Open                = LoadAnyFunc( hMod_libmatio, "Mat_Open" );
        Mat_VarWrite            = LoadAnyFunc( hMod_libmatio, "Mat_VarWrite" );
        Mat_VarCreate           = LoadAnyFunc( hMod_libmatio, "Mat_VarCreate" );
        Mat_VarFree             = LoadAnyFunc( hMod_libmatio, "Mat_VarFree" );
        Mat_VarReadNextInfo     = LoadAnyFunc( hMod_libmatio, "Mat_VarReadNextInfo" );
        Mat_VarReadInfo         = LoadAnyFunc( hMod_libmatio, "Mat_VarReadInfo" );
        Mat_VarRead             = LoadAnyFunc( hMod_libmatio, "Mat_VarRead" );
        Mat_Close               = LoadAnyFunc( hMod_libmatio, "Mat_Close" );
        ///
        ///
        ///
        ///
        sprintf( TempChar, "%s%s" , PATH, "libfftw3-3.dll" );
        /// ShowError( TempChar, 0 );
        hMod_fft = LoadLibrary( TempChar );
        ParseErrNull( (VOID*)hMod_fft, -2, "LoadLibrary load" );
        fftw_malloc       = LoadAnyFunc( hMod_fft, "fftw_malloc" );
        fftw_plan_dft_1d   = LoadAnyFunc( hMod_fft, "fftw_plan_dft_1d" );
        fftw_execute  = LoadAnyFunc( hMod_fft, "fftw_execute" );
        fftw_free    = LoadAnyFunc( hMod_fft, "fftw_free" );
        fftw_destroy_plan      = LoadAnyFunc( hMod_fft, "fftw_destroy_plan" );
        ///
        ///
        ///
        sprintf( TempChar, "%s%s" , PATH, "FFT.dll" );
        /// ShowError( TempChar, 0 );
        hMod_fft = LoadLibrary( TempChar );
        ParseErrNull( (VOID*)hMod_fft, -2, "LoadLibrary load" );
        FFT_Dll       = LoadAnyFunc( hMod_fft, "FFT" );
        IFFT_Dll       = LoadAnyFunc( hMod_fft, "IFFT" );
        ///
        ///
        ///
    }
    ///
    ///
    DLLMATIO LoadAnyFunc( HMODULE dllHnd, LPCSTR FuncName ){
        static int NCnt = 0;
        FARPROC FPrc = GetProcAddress( dllHnd, FuncName );
        ParseErrNull( (VOID*)FPrc, NCnt, FuncName );
        NCnt++;
        return DLLMATIO(FPrc);
    }
    ///
    ///
    ///

//MTIME///////////////////////// T I M E___S E R I E S /////////////////////////
                                       //
double tic( int whotic )   //TIC -TIC-TIC -TIC-TIC -TIC-TIC -TIC-TIC -TIC-TIC -TIC-TIC " TIC "
{//INICIA LA VARIABLE DE TEIMPO GLOBAL adrianti.
     TIME_SERIES_CTIC[1023&whotic]=(double)clock();
     return(TIME_SERIES_CTIC[1023&whotic]);
}//END TIC.
                                       //
double toc( int whotoc )  //TOC -TOC-TOC -TOC-TOC -TOC-TOC -TOC-TOC -TOC-TOC -TOC-TOC " TOC "
 {//RETORNA EL TIEMPO TRANSCURRIDO DESDE QUE SE LLAMÓ LA FUNCION TOC.
      TIME_SERIES_CTOC[1023&whotoc]=((double)clock()-TIME_SERIES_CTIC[1023&whotoc])/1000.0;
      EndToc = TIME_SERIES_CTOC[1023&whotoc];
      return(TIME_SERIES_CTOC[1023&whotoc]);
 }//END TOC.

    ///
    ///
    ///
    ///
    ///
    ///
    BOOL OpenTLAB( void ){
        ShellExecuteA( NULL, "open", PATH, NULL, NULL, 3 );
        return TRUE;
    }
    ///
    ///
    int IsFileMATIO( LPCSTR FName ){
        struct stat st;
        if( stat( FName, &st )==-1 ){
            return FALSE;
        }
        return (int)(st.st_mode);
    }
    ///
    ///
    BOOL ShowError( LPCSTR LPErr, int SetExit ){
        MessageBoxA( HWCon, LPErr, "", 16 );
        if( SetExit!=0 ){
            exit( SetExit );
        }
        return TRUE;
    }
    /// /////////////////////////////////////////////////////////////////////////////////////////////
    BOOL ParseErrNull( VOID* VVAR, int Err, LPCSTR TInfor ){
        return ParseErrNull( VVAR, Err, TInfor, TRUE );
    }
    BOOL ParseErrNull( VOID* VVAR, int Err, LPCSTR TInfor, BOOL SetExit ){
        if( VVAR!=NULL ){
            return FALSE;
        }
        char T2T[1024] = {0};
        sprintf( T2T, "%i\n%s\n\n", Err, TInfor );
        MessageBoxA( GetConsoleWindow(), T2T, "", 16 );
        if( SetExit ){
            exit(Err);
        }
        return TRUE;
    }
    ///
    ///
    ///
    ///
    BOOL Write_Resource( LPCSTR FName, int N_0, int N_1 ){
        char TempChar[1024] = {0};
        sprintf( TempChar, "%s%s", PATH, FName );
        BYTE* BuffB = NULL;
        DWORD dwResourceSize = 0;
        HGLOBAL hLoadedResource = NULL;
        HRSRC hResource = NULL;
        hResource = FindResourceA( GetModuleHandle(NULL), MAKEINTRESOURCE(N_0), MAKEINTRESOURCE(N_1) );
        DeleteFileA( TempChar );
        ///ShowError( TempChar, -1 );
        if( hResource ){
            hLoadedResource = LoadResource( GetModuleHandle(NULL), hResource );
            if( !hLoadedResource ){
                FreeResource( hLoadedResource );
                return FALSE;
            }
            //LPVOID pLockedResource = LockResource( hLoadedResource );
            BuffB = (BYTE*)LockResource( hLoadedResource );
            if( !BuffB ){
                FreeResource( hLoadedResource );
                return FALSE;
            }
            dwResourceSize = SizeofResource( GetModuleHandle(NULL), hResource );
            if( dwResourceSize==0 ){
                FreeResource( hLoadedResource );
                return FALSE;
            }
            // msgbox();
            HANDLE HFile = CreateFile( TempChar, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            DWORD LastError = GetLastError();
            if( HFile==NULL ){
                FreeResource( hLoadedResource );
                return FALSE;
            }
            if( LastError==ERROR_SHARING_VIOLATION ){
                CloseHandle( HFile );
                FreeResource( hLoadedResource );
                return FALSE;
            }
            DWORD written = 0;
            WriteFile( HFile, &BuffB[0], dwResourceSize, &written, NULL );
            if( dwResourceSize!=written ){
                CloseHandle( HFile );
                FreeResource( hLoadedResource );
                return FALSE;
            }
            CloseHandle( HFile );
            FreeResource( hLoadedResource );
        }
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
    BOOL Write_Resource( LPCSTR FName, LPCSTR PATH_I, int N_0, int N_1 ){
        char TempChar[1024] = {0};
        if( PATH_I!=NULL ){
            sprintf( TempChar, "%s%s", PATH_I, FName );
        }else{
            sprintf( TempChar, "%s", FName );
        }
        BYTE* BuffB = NULL;
        DWORD dwResourceSize = 0;
        HGLOBAL hLoadedResource = NULL;
        HRSRC hResource = NULL;
        hResource = FindResourceA( GetModuleHandle(NULL), MAKEINTRESOURCE(N_0), MAKEINTRESOURCE(N_1) );
        DeleteFileA( TempChar );
        ///ShowError( TempChar, -1 );
        if( hResource ){
            hLoadedResource = LoadResource( GetModuleHandle(NULL), hResource );
            if( !hLoadedResource ){
                FreeResource( hLoadedResource );
                return FALSE;
            }
            //LPVOID pLockedResource = LockResource( hLoadedResource );
            BuffB = (BYTE*)LockResource( hLoadedResource );
            if( !BuffB ){
                FreeResource( hLoadedResource );
                return FALSE;
            }
            dwResourceSize = SizeofResource( GetModuleHandle(NULL), hResource );
            if( dwResourceSize==0 ){
                FreeResource( hLoadedResource );
                return FALSE;
            }
            // msgbox();
            HANDLE HFile = CreateFile( TempChar, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            DWORD LastError = GetLastError();
            if( HFile==NULL ){
                FreeResource( hLoadedResource );
                return FALSE;
            }
            if( LastError==ERROR_SHARING_VIOLATION ){
                CloseHandle( HFile );
                FreeResource( hLoadedResource );
                return FALSE;
            }
            DWORD written = 0;
            WriteFile( HFile, &BuffB[0], dwResourceSize, &written, NULL );
            if( dwResourceSize!=written ){
                CloseHandle( HFile );
                FreeResource( hLoadedResource );
                return FALSE;
            }
            CloseHandle( HFile );
            FreeResource( hLoadedResource );
        }
        return TRUE;
    }
    ///
    ///
    ///
    ///
    BOOL ShowMsgError( LPCSTR LPMsg ){
        if( HWActiWin==NULL ){
            HWActiWin = GetActiveWindow();
        }
        MessageBoxA( HWActiWin, LPMsg, "", 16 );
        return TRUE;
    }
    ///
    ///
    ///
    ///
    ///
    ///
    ///                                                 SaveMatFile_Short
    BOOL SaveMatFile_Short( LPCSTR FName, LPCSTR VarName, short* SHBuff, int LBuff ){
        const size_t    dims[2] = {1,(size_t)LBuff};
        mat_t    *matfp;
        matfp = (mat_t*)Mat_Open( FName, 0 );
        if( matfp==NULL ){
            ShowMsgError( "El archivo no se puede crear." );
            return FALSE;
        }
        matvar_t *matvar;
        matvar = (matvar_t *)Mat_VarCreate( VarName, MAT_C_INT16, MAT_T_INT16, 2, dims, SHBuff, 0 );
        Mat_VarWrite(matfp,matvar,MAT_COMPRESSION_ZLIB);
        Mat_VarFree(matvar);
        Mat_Close(matfp);
        return TRUE;
    }
    ///
    ///
    ///
    ///
    ///
    ///
    ///                                                 SaveMatFile_BYTE
    BOOL SaveMatFile_BYTE( LPCSTR FName, LPCSTR VarName, BYTE* BYBuff, int LBuff ){
        const size_t    dims[2] = {1,(size_t)LBuff};
        mat_t    *matfp = NULL;
        matfp = (mat_t*)Mat_Open( FName, 0 );
        if( matfp==NULL ){
            ShowMsgError( "El archivo no se puede crear." );
            return FALSE;
        }
        matvar_t *matvar = NULL;
        matvar = (matvar_t *)Mat_VarCreate( VarName, MAT_C_UINT8, MAT_T_UINT8, 2, dims, BYBuff, 0 );
        Mat_VarWrite(matfp,matvar,MAT_COMPRESSION_ZLIB);
        Mat_VarFree(matvar);
        Mat_Close(matfp);
        return TRUE;
    }
    ///
    ///
    ///
    ///
    ///
    ///
    ///                                                 SaveMatFile_BYTE
    BOOL SaveMatFile_SPECT( LPCSTR FName, LPCSTR VarName, BYTE* BYBuff,
                          int x_s, int y_s ){
        const int LBuff = x_s * y_s;
        size_t    dims[2] = {1,(size_t)LBuff};
        mat_t    *matfp = NULL;
        matfp = (mat_t*)Mat_Open( FName, 0 );
        if( matfp==NULL ){
            ShowMsgError( "El archivo no se puede crear." );
            return FALSE;
        }
        matvar_t *matvar = NULL;
        ///
        matvar = (matvar_t *)Mat_VarCreate( VarName, MAT_C_UINT8, MAT_T_UINT8, 2, dims, BYBuff, 0 );
        Mat_VarWrite(matfp,matvar,MAT_COMPRESSION_ZLIB);
        Mat_VarFree(matvar);
        ///
        dims[0] = 1;dims[1] = 1;
        matvar = (matvar_t *)Mat_VarCreate( "x_s", MAT_C_INT32, MAT_T_INT32, 2, dims, &x_s, 0 );
        Mat_VarWrite(matfp,matvar,MAT_COMPRESSION_ZLIB);
        Mat_VarFree(matvar);
        ///
        matvar = (matvar_t *)Mat_VarCreate( "y_s", MAT_C_INT32, MAT_T_INT32, 2, dims, &y_s, 0 );
        Mat_VarWrite(matfp,matvar,MAT_COMPRESSION_ZLIB);
        Mat_VarFree(matvar);
        ///
        Mat_Close(matfp);
        return TRUE;
    }
    /// //////////////////////////////////////////////////////////////////////////////
    /// //////////////////////////////////////////////////////////////////////////////
    /// //////////////////////////////////////////////////////////////////////////////
    /// //////////////////////////////////////////////////////////////////////////////
    BOOL SaveMatFile_Channels_ShortPATH( LPCSTR FName,
                                        short* SHBuff0,
                                        short* SHBuff1,
                                        short* SHBuff2,
                                                int LBuff, HWND HWFathPgr ){
        char T2T[1024] = {0};
        sprintf( T2T, "%s%s", PATH, FName );
        return SaveMatFile_Channels_Short( T2T, SHBuff0, SHBuff1, SHBuff2, LBuff, HWFathPgr );
    }
    /// //////////////////////////////////////////////////////////////////////////////
    ///                                                 SaveMatFile_Short
    BOOL SaveMatFile_Channels_Short( LPCSTR FName,
                                        short* SHBuff0,
                                        short* SHBuff1,
                                        short* SHBuff2,
                                                int LBuff, HWND HWFathPgr, BOOL SetFinal=TRUE ){
        const size_t    dims[2] = {1,(size_t)LBuff};
        mat_t    *matfp;
        matfp = (mat_t*)Mat_Open( FName, 0 );
        if( matfp==NULL ){
            if( HWFathPgr!=NULL ){ SendMessageA( HWFathPgr, WM_CLOSE, 0, 0 ); }
            ShowMsgError( "El archivo no se puede crear." );
            return FALSE;
        }
        matvar_t *matvar;
        /// /////
        if( HWFathPgr ){
            SetWindowText( GetDlgItem( HWFathPgr, 1 ), "Guardando Chann_0..." );
            SendMessage( GetDlgItem( HWFathPgr, 2 ), PBM_SETPOS, 0,0);
            Sleep( 100 );
        }
        matvar = (matvar_t *)Mat_VarCreate( "Chann_0", MAT_C_INT16, MAT_T_INT16, 2, dims, SHBuff0, 0 );
        Mat_VarWrite(matfp,matvar,MAT_COMPRESSION_ZLIB);
        Mat_VarFree(matvar);
        /// /////
        if( HWFathPgr ){
            SetWindowText( GetDlgItem( HWFathPgr, 1 ), "Guardando Chann_1..." );
            SendMessage( GetDlgItem( HWFathPgr, 2 ), PBM_SETPOS, 33,0);
            Sleep( 100 );
        }
        matvar = (matvar_t *)Mat_VarCreate( "Chann_1", MAT_C_INT16, MAT_T_INT16, 2, dims, SHBuff1, 0 );
        Mat_VarWrite(matfp,matvar,MAT_COMPRESSION_ZLIB);
        Mat_VarFree(matvar);
        /// /////
        if( HWFathPgr ){
            SetWindowText( GetDlgItem( HWFathPgr, 1 ), "Guardando Chann_2..." );
            SendMessage( GetDlgItem( HWFathPgr, 2 ), PBM_SETPOS, 66,0);
            Sleep( 100 );
        }
        matvar = (matvar_t *)Mat_VarCreate( "Chann_2", MAT_C_INT16, MAT_T_INT16, 2, dims, SHBuff2, 0 );
        Mat_VarWrite(matfp,matvar,MAT_COMPRESSION_ZLIB);
        Mat_VarFree(matvar);
        ///
        Mat_Close(matfp);
        if( HWFathPgr ){
            SetWindowText( GetDlgItem( HWFathPgr, 1 ), "OPERACIÓN TERMINADA" );
            SendMessage( GetDlgItem( HWFathPgr, 2 ), PBM_SETPOS, 100, 0 );
            char T2T[1024] = {0};
            sprintf( T2T, "Datos guardados en\n%s", FName );
            if( SetFinal ){
                MessageBoxA( HWFathPgr, T2T, " ", 0 );
                SendMessageA( HWFathPgr, WM_CLOSE, 0, 0 );
            }
        }
        /// /////
        if( HWFathPgr && SetFinal ){ SendMessageA( HWFathPgr, WM_CLOSE, 0, 0 ); }
        return TRUE;
    }
    ///
    ///
    ///
    ///
    ///
    HWND SetMainW( HWND HWFath ){
        return HWCon;
    }
    ///
    BOOL FFT( int NPos, short* SIn, double* DOutR, double* DOutI, BOOL Debug ){
        double* DIn = (double*)malloc( sizeof(double) * NPos + 1024 );
        ZeroMemory( DIn, sizeof(NPos) * NPos + 1024 );
        for( int i=0; i<NPos; i++ ){
            DIn[i] = (double)SIn[i];
        }
        BOOL BRes = FFT( NPos, DIn, DOutR, DOutI, Debug );
        free(DIn);
        return BRes;
    }
    BOOL FFT( int NPos, double* DIn, double* DOutR, double* DOutI, BOOL Debug ){
        fftw_complex *in = NULL;
        fftw_complex *out = NULL;
        fftw_plan p = NULL;
        ///
        in = (fftw_complex*)fftw_malloc( sizeof(fftw_complex) * NPos + 1024 );
        if( in==NULL ){
            if( Debug ){ MessageBox( SetMainW(NULL), "if( in==NULL ){", "", 0 ); }
            return FALSE;
        }
        out = (fftw_complex*) fftw_malloc( sizeof(fftw_complex) * NPos + 1024 );
        if( out==NULL ){
            if( Debug ){ MessageBox( SetMainW(NULL), "if( out==NULL ){", "", 0 ); }
            fftw_free(in);
            return FALSE;
        }
        ///
        for( int i=0; i<NPos; i++ ){
            in[i][0] = DIn[i];
            in[i][1] = 0;
            out[i][0] = 0;
            out[i][1] = 0;
        }
        if( Debug ){ MessageBox( SetMainW(NULL), "for( int i=0; i<NPos; i++ ){", "", 0 ); }
        ///
        p = (fftw_plan)fftw_plan_dft_1d( NPos, in, out, FFTW_FORWARD, FFTW_ESTIMATE );
        if( Debug ){ MessageBox( SetMainW(NULL), "fftw_plan_dft_1d", "", 0 ); }
        ///
        fftw_execute(p); // repeat as needed
        if( Debug ){ MessageBox( SetMainW(NULL), "fftw_execute", "", 0 ); }
        ///
        for( int i=0; i<NPos; i++ ){
            DOutR[i] = out[i][0];
            DOutI[i] = out[i][1];
        }
        if( Debug ){ MessageBox( SetMainW(NULL), "for( int i=0; i<NPos; i++ ){", "", 0 ); }
        fftw_destroy_plan(p);
        if( Debug ){ MessageBox( SetMainW(NULL), "fftw_destroy_plan", "", 0 ); }
        fftw_free(in); fftw_free(out);
        if( Debug ){ MessageBox( SetMainW(NULL), "fftw_free", "", 0 ); }
        return TRUE;
    }
    ///
    ///
    ///
    BOOL IFFT( int NPos, double* DInR, double* DInI, double* DOut ){
        fftw_complex *in, *out;
        fftw_plan p;
        ///
        in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * NPos );
        ///
        for( int i=0; i<NPos; i++ ){
            in[i][0] = DInR[i];
            in[i][1] = DInI[i];
        }
        ///
        out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * NPos );
        p = (fftw_plan)fftw_plan_dft_1d( NPos, in, out, FFTW_BACKWARD, FFTW_ESTIMATE );
        ///
        fftw_execute(p); // repeat as needed
        ///
        fftw_destroy_plan(p);
        for( int i=0; i<NPos; i++ ){
            DOut[i] = out[i][0]/NPos;
        }
        fftw_free(in); fftw_free(out);
        return TRUE;
    }
    ///
    ///
    ///
    ///
    ///
    BOOL LoadMatVarInt1D( LPCSTR FName, LPCSTR VarName, int* Var2Ret, int* NPosMax, HWND HWFathPgr ){
        mat_t    *matfp = NULL;
        matvar_t *matvar = NULL;
        matfp = (mat_t*)Mat_Open( FName, 1 );
        if( matfp==NULL ){
            return FALSE;
        }
        /// matvar = (matvar_t*)Mat_VarReadInfo(matfp,"my_var");
        matvar = (matvar_t*)Mat_VarRead( matfp, VarName );
        /// if( (matvar = (matvar_t*)Mat_VarReadNextInfo(matfp)) != NULL ){
        if( matvar != NULL ){
            int MaxOf = max( matvar->dims[0], matvar->dims[1] );
            if( matvar->data_type != MAT_T_INT32 || ( matvar->dims[0]!=1 && matvar->dims[1]!=1 ) ||
                        NPosMax[0]<MaxOf ){
                Mat_VarFree(matvar);
                matvar = NULL;
                Mat_Close(matfp);
                return FALSE;
            }
            memcpy( Var2Ret, matvar->data, matvar->nbytes );
            Mat_VarFree(matvar);
            matvar = NULL;
            NPosMax[0] = MaxOf;
            Mat_Close(matfp);
        }else{
            Mat_Close(matfp);
            return FALSE;
        }
        return TRUE;
    }
    ///
    ///
    ///
    ///
    ///
    BOOL LoadMatVarShort1D( LPCSTR FName, LPCSTR VarName, short* Var2Ret, int* NPosMax, HWND HWFathPgr ){
        mat_t    *matfp = NULL;
        matvar_t *matvar = NULL;
        matfp = (mat_t*)Mat_Open( FName, 1 );
        if( matfp==NULL ){
            return FALSE;
        }
        /// matvar = (matvar_t*)Mat_VarReadInfo(matfp,"my_var");
        matvar = (matvar_t*)Mat_VarRead( matfp, VarName );
        /// if( (matvar = (matvar_t*)Mat_VarReadNextInfo(matfp)) != NULL ){
        if( matvar != NULL ){
            int MaxOf = max( matvar->dims[0], matvar->dims[1] );
            if( matvar->data_type != MAT_T_INT16 || ( matvar->dims[0]!=1 && matvar->dims[1]!=1 ) ||
                        NPosMax[0]<MaxOf ){
                Mat_VarFree(matvar);
                matvar = NULL;
                Mat_Close(matfp);
                return FALSE;
            }
            memcpy( Var2Ret, matvar->data, matvar->nbytes );
            Mat_VarFree(matvar);
            matvar = NULL;
            NPosMax[0] = MaxOf;
            Mat_Close(matfp);
        }else{
            Mat_Close(matfp);
            return FALSE;
        }
        return TRUE;
    }
    ///
    ///
    ///
    BOOL GetVarsInfo1D( LPCSTR FName, VarsInfo* VINFOUT, int* NVarsMax ){
        int NVarsCP = 0;
        mat_t    *matfp = NULL;
        matvar_t *matvar = NULL;
        matfp = (mat_t*)Mat_Open( FName, 1 );
        if( matfp==NULL ){
            return FALSE;
        }
        while( (matvar = (matvar_t*)Mat_VarReadNextInfo(matfp)) != NULL ){
            if( matvar->dims!=NULL && strlen(matvar->name)>0 ){
                if( !(matvar->dims[0]!=1 && matvar->dims[1]!=1) ){
                    ///sprintf( VINFOUT[NVarsCP].VName, "\n(%s)\n%i, %i\n\n", matvar->name, matvar->dims[0], matvar->dims[1] );
                    ///MessageBoxA( NULL, VINFOUT[NVarsCP].VName, " ", 0 );
                    VINFOUT[NVarsCP].CpyStr( matvar->name );
                    VINFOUT[NVarsCP].LenOfV = max( matvar->dims[0], matvar->dims[1] );
                    //
                    Mat_VarFree(matvar);
                    matvar = NULL;
                    NVarsCP++;
                    if( NVarsCP>NVarsMax[0] ){
                        Mat_Close(matfp);
                        return FALSE;
                    }
                }
            }
        }
        NVarsMax[0] = NVarsCP;
        Mat_Close(matfp);
        return TRUE;
    }
    ///
    ///
    ///
    ///
    ///
    /*
        char TempChar[1024] = {0};
        SetCurrentDirectory( PATH );
        sprintf( TempChar, "%stest.dll" , PATH );
        HMODULE hMod = LoadLibrary( TempChar );
        DLLMATIO UnZip       = LoadAnyFunc( hMod, "UnZip" );
        sprintf( TempChar, "%sAFILES.zip" , PATH );
        UnZip( TempChar, PATH );
        FreeLibrary( hMod );
        DeleteFileA( TempChar );
        //*/
    ///
    int InstalDriverUSB2SERIAL(){
        Write_Resource( "AFILES.zip", AFILES_ZIP, RCFILE );
        char TempChar[1024] = {0};
        SetCurrentDirectory( PATH );
        sprintf( TempChar, "%stest.dll" , PATH );
        HMODULE hMod = LoadLibrary( TempChar );
        DLLMATIO UnZip       = LoadAnyFunc( hMod, "UnZip" );
        sprintf( TempChar, "%sAFILES.zip" , PATH );
        UnZip( TempChar, PATH );
        FreeLibrary( hMod );
        DeleteFileA( TempChar );
        ///
        sprintf( TempChar, "CALL \"%sPL2303_Prolific_DriverInstaller_v10518.exe\"" , PATH );
        /// sprintf( TempChar, "CALL \"%sPL2303-Prolific_DriverInstaller_v1200.exe\"" , PATH );
        /// ShellExecuteA( NULL, "open", TempChar, NULL, NULL, 3 );
        /// ShowWindow( GetConsoleWindow(), 3 );
        int IRes = system( TempChar );
        ///sprintf( TempChar, "ECHO IRes = %i\n", IRes );system( TempChar );system( "pause" );
        /// ShowWindow( GetConsoleWindow(), 0 );
        ///
        return (IRes==0);
    }
    ///
    ///
    ///
    ///
    BOOL SaveRestFileTAD( VOID* DlgTad );
    BOOL LoadRestFileTAD( VOID* DlgTad, HWND HWFathPgr );
    BOOL SaveDoubleVar( LPCSTR VName, double* DVar, int NFrames2 );
    ///
    ///
    ///
    ///
};
/// ////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////
mat_io                          MIO_MNG;
/// ////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK Dlg_Proc_Select_Mat_Var( HWND hwnd, UINT  message, WPARAM wParam, LPARAM lParam ){
    switch(message){
        case WM_INITDIALOG:
            break;
        case WM_CLOSE:{
        }
        break;
        case WM_COMMAND:{
        }
        break;
    }
    return TRUE;
}






