BOOL DLG_TAD::ThreadDSPProcs( BOOL ForcePlot ){
    Sleep( 1 );
    /// MIO_MNG.tic( 0 );
    static short* SHTemp = NULL;
    int NFrames2 = SerHand.NFramesReaded;
    if( NFrames2<2000 ){
        InDSPProcsThr = FALSE;
        return FALSE;
    }
    NFrames2 = SerHand.NFramesReaded - SerHand.NFramesReaded%2;
    if( SHTemp==NULL ){
        SHTemp = (short*)malloc( sizeof(short) * MaxFRM + 1024 );
        ZeroMemory( SHTemp, sizeof(short) * MaxFRM + 1024 );
    }
    ftp.Prepare( MaxFRM );
    ///
    if( TabPage==1 || ForcePlot ){
        if( HWFathPgr!=NULL ){ SetWindowText( GetDlgItem( HWFathPgr, 1 ), "Procesando canal 1..." ); }
        DSPProc0( &ftp, SHTemp, NFrames2, SerHand.SBuff0, &HWPLOT0_OUT, 0, ForcePlot );
    }
    ///
    if( TabPage==2 || ForcePlot ){
        if( HWFathPgr!=NULL ){ SetWindowText( GetDlgItem( HWFathPgr, 1 ), "Procesando canal 2..." ); }
        DSPProc0( &ftp, SHTemp, NFrames2, SerHand.SBuff1, &HWPLOT1_OUT, 1, ForcePlot );
    }
    ///
    if( TabPage==3 || ForcePlot ){
        if( HWFathPgr!=NULL ){ SetWindowText( GetDlgItem( HWFathPgr, 1 ), "Procesando canal 3..." ); }
        DSPProc0( &ftp, SHTemp, NFrames2, SerHand.SBuff2, &HWPLOT2_OUT, 2, ForcePlot );
    }
    ///
    ///
    ///
    ///
    /// char T2T[128] = {0};sprintf( T2T, "Tiempo por el proceso:\n%f Segundos.", MIO_MNG.EndToc );
    /// SetWindowText( HW_T_Info, T2T );
    if( ForcePlot ){
        free( SHTemp );
        SHTemp = NULL;
        if( HWFathPgr!=HW_DLG_MAIN ){
            SendMessageA( HWFathPgr, WM_CLOSE, 0, 0 );
        }
    }
    if( !ForcePlot ){
        Sleep( 20 );
    }
    InDSPProcsThr = FALSE;
    return TRUE;
}









