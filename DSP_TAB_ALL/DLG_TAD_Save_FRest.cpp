BOOL mat_io::LoadRestFileTAD( VOID* DlgTadV, HWND HWFathPgr ){
    DLG_TAD*    DlgTad = (DLG_TAD*)DlgTadV;
    char FName[1024] = {0};
    sprintf( FName, "%sFRES.frs", PATH ); ///
    sprintf( FName, "%sFRES.mat", PATH ); ///
    int         IntPtr[64] = {0};
    matvar_t*   matvar = NULL;
    mat_t*      matfp = NULL;
    matfp = (mat_t*)Mat_Open( FName, 1 );
    if( matfp==NULL ){
        // ShowMsgError( "El archivo de restauración no se encontró." );
        return FALSE;
    }
    ///
    matvar = (matvar_t*)Mat_VarRead( matfp, "NFramesReaded" );
    if( matvar==NULL ){
        ShowMsgError( "NFramesReaded" );
        Mat_Close(matfp);
        return FALSE;
    }
    if( matvar->data_type != MAT_T_INT32 ){
        ShowMsgError( "NFramesReaded" );
        Mat_Close(matfp);
        return FALSE;
    }
    memcpy( IntPtr, matvar->data, matvar->nbytes );
    DlgTad->SerHand.NFramesReaded = IntPtr[0];
    Mat_VarFree( matvar );
    matvar = NULL;
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    matvar = (matvar_t*)Mat_VarRead( matfp, "Chann_1" );
    if( matvar==NULL ){
        ShowMsgError( "Chann_1" );
        DlgTad->SerHand.NFramesReaded = 0;
        Mat_Close(matfp);
        return FALSE;
    }
    if( matvar->data_type != MAT_T_INT16 ){
        ShowMsgError( "Chann_1" );
        DlgTad->SerHand.NFramesReaded = 0;
        Mat_Close(matfp);
        return FALSE;
    }
    ///
    /// DlgTad->MsgBoxF( "%i\n%i", DlgTad->SerHand.NFramesReaded, matvar->nbytes/2 );
    ///
    ///
    memcpy( DlgTad->SerHand.SBuff0, matvar->data, matvar->nbytes );
    Mat_VarFree( matvar );
    matvar = NULL;
    SetWindowText( GetDlgItem( HWFathPgr, 1 ), "Canal 1 cargado..." );
    SendMessage( GetDlgItem( HWFathPgr, 2 ), PBM_SETPOS, 33,0);
    ///
    ///
    ///
    ///
    ///
    ///
    matvar = (matvar_t*)Mat_VarRead( matfp, "Chann_2" );
    if( matvar==NULL ){
        ShowMsgError( "Chann_2" );
        DlgTad->SerHand.NFramesReaded = 0;
        Mat_Close(matfp);
        return FALSE;
    }
    if( matvar->data_type != MAT_T_INT16 ){
        ShowMsgError( "Chann_2" );
        DlgTad->SerHand.NFramesReaded = 0;
        Mat_Close(matfp);
        return FALSE;
    }
    memcpy( DlgTad->SerHand.SBuff1, matvar->data, matvar->nbytes );
    Mat_VarFree( matvar );
    matvar = NULL;
    SetWindowText( GetDlgItem( HWFathPgr, 1 ), "Canal 2 cargado..." );
    SendMessage( GetDlgItem( HWFathPgr, 2 ), PBM_SETPOS, 66,0);
    ///
    ///
    ///
    ///
    ///
    ///
    matvar = (matvar_t*)Mat_VarRead( matfp, "Chann_3" );
    if( matvar==NULL ){
        ShowMsgError( "Chann_3" );
        DlgTad->SerHand.NFramesReaded = 0;
        Mat_Close(matfp);
        return FALSE;
    }
    if( matvar->data_type != MAT_T_INT16 ){
        ShowMsgError( "Chann_3" );
        DlgTad->SerHand.NFramesReaded = 0;
        Mat_Close(matfp);
        return FALSE;
    }
    memcpy( DlgTad->SerHand.SBuff2, matvar->data, matvar->nbytes );
    Mat_VarFree( matvar );
    matvar = NULL;
    SetWindowText( GetDlgItem( HWFathPgr, 1 ), "Canal 3 cargado..." );
    SendMessage( GetDlgItem( HWFathPgr, 2 ), PBM_SETPOS, 80,0);
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    matvar = (matvar_t*)Mat_VarRead( matfp, "ButSel" );
    if( matvar==NULL ){
        ShowMsgError( "ButSel" );
        DlgTad->SerHand.NFramesReaded = 0;
        Mat_Close(matfp);
        return FALSE;
    }
    if( matvar->data_type != MAT_T_INT32 ){
        ShowMsgError( "ButSel" );
        DlgTad->SerHand.NFramesReaded = 0;
        Mat_Close(matfp);
        return FALSE;
    }
    memcpy( DlgTad->ButSel, matvar->data, matvar->nbytes );
    Mat_VarFree( matvar );
    matvar = NULL;
    ///
    ///
    ///
    ///
    ///
    matvar = (matvar_t*)Mat_VarRead( matfp, "DSP_TYPE_PROC" );
    if( matvar==NULL ){
        ShowMsgError( "DSP_TYPE_PROC" );
        DlgTad->SerHand.NFramesReaded = 0;
        Mat_Close(matfp);
        return FALSE;
    }
    if( matvar->data_type != MAT_T_INT32 ){
        ShowMsgError( "DSP_TYPE_PROC" );
        DlgTad->SerHand.NFramesReaded = 0;
        Mat_Close(matfp);
        return FALSE;
    }
    memcpy( IntPtr, matvar->data, matvar->nbytes );
    Mat_VarFree( matvar );
    matvar = NULL;
    int IndT = 0;
    for( int y=0; y<8; y++ ){
        for( int x=0; x<4; x++ ){
            DlgTad->DSP_TYPE_PROC[x][y] = IntPtr[IndT];
            IndT++;
        }
    }
    ///
    ///
    ///
    ///
    ///
    //*
    char T2T[128] = {0};
    for( int x=0; x<3; x++ ){
        for( int y=0; y<8; y++ ){
            sprintf( T2T, "FDS_%i%i", x, y );
            matvar = (matvar_t*)Mat_VarRead( matfp, T2T );
            memcpy( &DlgTad->FDS[x][y], matvar->data, matvar->nbytes );
            Mat_VarFree( matvar );
            matvar = NULL;
            DlgTad->FDS[x][y].IIRSet();
            DlgTad->FDS[x][y].Fir1Set();
        }
    }
    //*/
    SetWindowText( GetDlgItem( HWFathPgr, 1 ), "Configuración de módulos cargada..." );
    SendMessage( GetDlgItem( HWFathPgr, 2 ), PBM_SETPOS, 88,0);
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    matvar = (matvar_t*)Mat_VarRead( matfp, "OnTop" );
    if( matvar==NULL ){
        ShowMsgError( "OnTop" );
        Mat_Close(matfp);
        return FALSE;
    }
    if( matvar->data_type != MAT_T_INT32 ){
        ShowMsgError( "OnTop" );
        Mat_Close(matfp);
        return FALSE;
    }
    memcpy( IntPtr, matvar->data, matvar->nbytes );
    DlgTad->OnTop = IntPtr[0];
    Mat_VarFree( matvar );
    matvar = NULL;
    if( DlgTad->OnTop ){
        SetWindowPos( DlgTad->HW_DLG_MAIN, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW );
    }else{
        SetWindowPos( DlgTad->HW_DLG_MAIN, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW );
    }
    CheckMenuItem( DlgTad->HMeW, 2, 8*int(DlgTad->OnTop) );
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    matvar = (matvar_t*)Mat_VarRead( matfp, "TimeMax" );
    if( matvar==NULL ){
        ShowMsgError( "TimeMax" );
        Mat_Close(matfp);
        return FALSE;
    }
    if( matvar->data_type != MAT_T_INT32 ){
        ShowMsgError( "TimeMax" );
        Mat_Close(matfp);
        return FALSE;
    }
    memcpy( IntPtr, matvar->data, matvar->nbytes );
    DlgTad->TimeMax = IntPtr[0];
    Mat_VarFree( matvar );
    matvar = NULL;
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
    Mat_Close( matfp );
    ///
    return TRUE;
}
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL mat_io::SaveRestFileTAD( VOID* DlgTadV ){
    DLG_TAD*    DlgTad = (DLG_TAD*)DlgTadV;
    char FName[1024] = {0};
    char T2T[1024] = {0};
    sprintf( FName, "%sFRES.frs", PATH );///
    sprintf( FName, "%sFRES.mat", PATH );///
    size_t      dims[2] = {1,1};
    int         IntPtr[64] = {0};
    matvar_t*   matvar = NULL;
    short*      SBuffN = NULL;
    mat_t*      matfp = NULL;
    matfp = (mat_t*)Mat_Open( FName, 0 );
    if( matfp==NULL ){
        ShowMsgError( "El archivo no se puede crear." );
        return FALSE;
    }
    ///
    IntPtr[0] = DlgTad->SerHand.NFramesReaded;
    dims[0] = dims[1] = 1;
    matvar = (matvar_t *)Mat_VarCreate( "NFramesReaded", MAT_C_INT32, MAT_T_INT32, 2, dims, IntPtr, 0 );
    Mat_VarWrite(matfp,matvar,MAT_COMPRESSION_ZLIB);
    Mat_VarFree(matvar);
    ///
    dims[1] = DlgTad->SerHand.NFramesReaded;
    ///
    SBuffN = DlgTad->SerHand.SBuff0;
    matvar = (matvar_t *)Mat_VarCreate( "Chann_1", MAT_C_INT16, MAT_T_INT16, 2, dims, SBuffN, 0 );
    Mat_VarWrite(matfp,matvar,MAT_COMPRESSION_NONE);
    Mat_VarFree(matvar);
    ///
    ///
    SBuffN = DlgTad->SerHand.SBuff1;
    matvar = (matvar_t *)Mat_VarCreate( "Chann_2", MAT_C_INT16, MAT_T_INT16, 2, dims, SBuffN, 0 );
    Mat_VarWrite(matfp,matvar,MAT_COMPRESSION_NONE);
    Mat_VarFree(matvar);
    ///
    SBuffN = DlgTad->SerHand.SBuff2;
    matvar = (matvar_t *)Mat_VarCreate( "Chann_3", MAT_C_INT16, MAT_T_INT16, 2, dims, SBuffN, 0 );
    Mat_VarWrite(matfp,matvar,MAT_COMPRESSION_NONE);
    Mat_VarFree(matvar);
    ///
    ///
    ///
    ///
    IntPtr[0] = DlgTad->ChannSel;
    dims[0] = dims[1] = 1;
    matvar = (matvar_t *)Mat_VarCreate( "NFramesReaded", MAT_C_INT32, MAT_T_INT32, 2, dims, IntPtr, 0 );
    Mat_VarWrite(matfp,matvar,MAT_COMPRESSION_ZLIB);
    Mat_VarFree(matvar);
    ///
    ///
    ///
    memcpy( IntPtr, DlgTad->ButSel, 4 * sizeof(int) );
    dims[0] = 1;
    dims[1] = 4;
    matvar = (matvar_t *)Mat_VarCreate( "ButSel", MAT_C_INT32, MAT_T_INT32, 2, dims, IntPtr, 0 );
    Mat_VarWrite(matfp,matvar,MAT_COMPRESSION_ZLIB);
    Mat_VarFree(matvar);
    ///
    ///
    ///
    dims[0] = 4;
    dims[1] = 8;
    int IndT = 0;
    for( int y=0; y<8; y++ ){
        for( int x=0; x<4; x++ ){
            IntPtr[IndT] = DlgTad->DSP_TYPE_PROC[x][y];
            IndT++;
        }
    }
    matvar = (matvar_t *)Mat_VarCreate( "DSP_TYPE_PROC", MAT_C_INT32, MAT_T_INT32, 2, dims, IntPtr, 0 );
    Mat_VarWrite(matfp,matvar,MAT_COMPRESSION_ZLIB);
    Mat_VarFree(matvar);
    ///
    ///
    ///
    ///
    ///
    BYTE* BPTR = NULL;
    dims[0] = sizeof( FDESIGN_CLASS );
    dims[1] = 1;
    for( int x=0; x<3; x++ ){
        for( int y=0; y<8; y++ ){
            sprintf( T2T, "FDS_%i%i", x, y );
            /// MessageBoxA( NULL, T2T, "", 0 );
            BPTR = (BYTE*)&DlgTad->FDS[x][y];
            matvar = (matvar_t *)Mat_VarCreate( T2T, MAT_C_UINT8, MAT_T_UINT8, 2, dims, BPTR, 0 );
            Mat_VarWrite(matfp,matvar,MAT_COMPRESSION_NONE);
            Mat_VarFree(matvar);
        }
    }
    ///
    ///
    ///
    ///
    ///
    ///
    IntPtr[0] = DlgTad->OnTop;
    dims[0] = dims[1] = 1;
    matvar = (matvar_t *)Mat_VarCreate( "OnTop", MAT_C_INT32, MAT_T_INT32, 2, dims, IntPtr, 0 );
    Mat_VarWrite(matfp,matvar,MAT_COMPRESSION_ZLIB);
    Mat_VarFree(matvar);
    ///
    ///
    ///
    ///
    ///
    ///
    IntPtr[0] = DlgTad->TimeMax;
    dims[0] = dims[1] = 1;
    matvar = (matvar_t *)Mat_VarCreate( "TimeMax", MAT_C_INT32, MAT_T_INT32, 2, dims, IntPtr, 0 );
    Mat_VarWrite(matfp,matvar,MAT_COMPRESSION_ZLIB);
    Mat_VarFree(matvar);
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    Mat_Close( matfp );
    ///
    return TRUE;
}






/// ///////////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL mat_io::SaveDoubleVar( LPCSTR VName, double* DVar, int NFrames2 ){
    char FName[1024] = {0};
    sprintf( FName, "%s%s.frs", PATH, VName );///
    sprintf( FName, "%s%s.mat", PATH, VName );///
    size_t      dims[2] = {1,1};
    matvar_t*   matvar = NULL;
    mat_t*      matfp = NULL;
    matfp = (mat_t*)Mat_Open( FName, 0 );
    if( matfp==NULL ){
        ShowMsgError( "El archivo no se puede crear." );
        return FALSE;
    }
    ///
    dims[0] = 1;
    dims[1] = NFrames2;
    matvar = (matvar_t*)Mat_VarCreate( VName, MAT_C_DOUBLE, MAT_T_DOUBLE, 2, dims, DVar, 0 );
    Mat_VarWrite(matfp,matvar,MAT_COMPRESSION_ZLIB);
    Mat_VarFree(matvar);
    ///
    ///
    ///
    ///
    Mat_Close( matfp );
    ///
    return TRUE;
}
