BOOL DLG_TAD::SaveProc( void ){
    static char FNAME[1024] = {0};
    static char PATH[1024] = {0};
    char T2T[10*1024] = {0};
    char TFT[128] = {0};
    char TFMAT[128] = {0};
    char TFMATC[128] = {0};
    if( !SaveFileAs( FNAME, "MAT Files (*.m*)\0*.m\0", HW_DLG_MAIN ) ){
        return FALSE;
    }
    if( FNAME[strlen(FNAME)-1]!='m' || FNAME[strlen(FNAME)-2]!='.' ){
        MsgBoxErrorF( 16, "El formato del archivo no es válido." );
        return FALSE;
    }
    strcpy( PATH, FNAME );
    PathRemoveFileSpecA( PATH );
    strcat( PATH, "\\" );
    /// MsgBoxF( PATH );
    ///
    ///
    ///
    ///
    GetFileTitleA( FNAME, TFT, 1023 );
    ///
    ///
    BOOL SetRetu = FALSE;
    for( int i=0; i<(int)strlen(TFT)-2; i++ ){
        if( !( TFT[i]>=97 && TFT[i]<=122 ) && !( TFT[i]>=65 && TFT[i]<=90 ) ){
            if( TFT[i]!='_' || i==0 ){
                SetRetu = TRUE;
                break;
            }
        }
    }
    if( SetRetu ){
        MsgBoxErrorF( 16, "El nombre del archivo no es válido, debe contener letras minúsculas ó mayúsculas." );
        return FALSE;
    }
    ///
    ///
    strcpy( TFMAT, TFT );
    strcpy( TFMATC, FNAME );
    TFMAT[strlen(TFMAT)] = 'a';
    TFMAT[strlen(TFMAT)] = 't';
    TFMAT[strlen(TFMAT)] = 0;
    ///
    TFMATC[strlen(TFMATC)] = 'a';
    TFMATC[strlen(TFMATC)] = 't';
    TFMATC[strlen(TFMATC)] = 0;
    ///
    ///
    ///
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
    sprintf( T2T, T2DLG, TFT, TFMAT );
    SHADV = DialogBoxParam( GetModuleHandle(NULL), MAKEINTRESOURCEA(Dial_SAVE_ADV), HW_DLG_MAIN, DlgProc_Proc_SAVE_ADV, (LPARAM)T2T );
    if( SHADV<1 ){
        return FALSE;
    }
    ///
    ///
    /// /////////////////////////////////////////////////////////////////////////
    DWORD IdTh = 0;
    HWFathPgr = HW_DLG_MAIN;
    CreateThread( NULL, 0, Call_Progress, (PVOID)&HWFathPgr, 0, &IdTh );
    Sleep( 20 );Sleep( 20 );Sleep( 20 );
    SetWindowTextA( HWFathPgr, " Copia de archivos " );
    ///
    ///
    MIO_MNG.Write_Resource( "wigner.m", PATH, MATF_TAD_WIGNER, DSPTABPICS );
    MIO_MNG.Write_Resource( "choi_william.m", PATH, MATF_TAD_CHOI_WILLIAM, DSPTABPICS );
    ///
    ///
    SetWindowText( GetDlgItem( HWFathPgr, 1 ), "Copiando los datos..." );
    SendMessage( GetDlgItem( HWFathPgr, 2 ), PBM_SETPOS, 50,0);
    ///
    ///
    //*
    MIO_MNG.SaveMatFile_Channels_Short( TFMATC,
                                        SerHand.SBuff0,
                                        SerHand.SBuff1,
                                        SerHand.SBuff2,
                                        SerHand.NFramesReaded,
                                        NULL, FALSE );
    //*/
    ///
    SetWindowText( GetDlgItem( HWFathPgr, 1 ), "Copiando funciones..." );
    SendMessage( GetDlgItem( HWFathPgr, 2 ), PBM_SETPOS, 100,0);
    ///
    ///
    ///
    /// ZeroMemory( T2T, 10*1024-1 );
    DWORD written = 0;
    HANDLE HFile = CreateFile( FNAME, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
    if( HFile==NULL ){
        MsgBoxErrorF( 16, "El archivo\n%s\nno puede ser escrito.", FNAME );
        return FALSE;
    }
    ///
    TFT[strlen(TFT)-2] = 0;
    sprintf( T2T, "%%function %s\nclc;\nload %s \nFs = 2000;\nFNyq = Fs/2;\n"
                            "t = (0:%i-1)/Fs;\n\n", TFT, TFT, (int)SerHand.NFramesReaded );
    WriteFile( HFile, &T2T[0], strlen(T2T), &written, NULL );
    ///
    if( (SHADV&(1))==1 ){
        GetChannelProc( 0, T2T, SHADV>=128 );
        WriteFile( HFile, &T2T[0], strlen(T2T), &written, NULL );
    }
    ///
    ///
    if( (SHADV&(2))==2 ){
        GetChannelProc( 1, T2T, SHADV>=128 );
        WriteFile( HFile, &T2T[0], strlen(T2T), &written, NULL );
    }
    ///
    ///
    if( (SHADV&(4))==4 ){
        GetChannelProc( 2, T2T, SHADV>=128 );
        WriteFile( HFile, &T2T[0], strlen(T2T), &written, NULL );
    }
    ///
    CloseHandle( HFile );
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    SetWindowText( GetDlgItem( HWFathPgr, 1 ), "Archivos copiados." );
    MessageBoxA( HWFathPgr, "Archivos copiados.", "", 0 );
    SendMessageA( HWFathPgr, WM_CLOSE, 0, 0 );
    while( HWFathPgr!=NULL ){
        Sleep( 50 );
    }
    ///
    ///
    ///
    ///
    ///
    return TRUE;
}





