BOOL DLG_TAD::ThreadConn(){
    B_CONN.Enable( TRUE );B_CONN.SetOff();
    Sleep( 1 );
    B_CONN.SetOff( NULL );
    char T2T[256] = {0};
    int BRE = 0;
    int L_BRE = 0;
    int N_BRE = 0;
    HWPLOT0.Hold( FALSE );HWPLOT1.Hold( FALSE );HWPLOT2.Hold( FALSE );HWPLOT.Hold( FALSE );
    HWPLOT.SetMaxYLim( 5 );
    HWPLOT0.SetLineColor( LineColors[0] );
    HWPLOT1.SetLineColor( LineColors[0] );
    HWPLOT2.SetLineColor( LineColors[0] );
    HWPLOT0_OUT.SetOnSpect( TRUE ); HWPLOT1_OUT.SetOnSpect( TRUE ); HWPLOT2_OUT.SetOnSpect( TRUE );
    ///      /////////////////////////////////////////////////////////////////////////
    const int TimeMaxPFS = 2000 * TimeMax;
    ///      /////////////////////////////////////////////////////////////////////////
    ///      /////////////////////////////////////////////////////////////////////////
    /// #define WITH_SAME_PL 64
    #ifdef WITH_SAME_PL
        SetWindowTextA( HW_DLG_MAIN, "Solo sirve con Tx->Rx unidos" );
        SerHand.ReadBytesAvailable();
        SerHand.NFramesReaded = 0;
        BYTE* B2Send  = (BYTE*)malloc( 10*1024 );
        unsigned short NCNTT = 0;
    #endif
    ///      /////////////////////////////////////////////////////////////////////////
    ///      /////////////////////////////////////////////////////////////////////////
    ///      /////////////////////////////////////////////////////////////////////////
    /// INICIO DE ADQ;
    MIO_MNG.tic( 0 );
    while( QuitThrConnV==FALSE && BRE>-2 ){
        BRE = SerHand.ReadBytesAvailable();
        /// sprintf( T2T, "%i", BRE );SetWindowText( HW_DLG_MAIN, T2T );
        if( BRE>0 ){
            CallPlot();
            CallDSPProcs();
            MIO_MNG.tic( 0 );
            N_BRE = SerHand.NFramesReaded/2000;
            if( N_BRE>L_BRE ){
                L_BRE = N_BRE;
                sprintf( T2T, " %i Segundos\n adquiridos\n %i%%", N_BRE, (100*N_BRE)/TimeMax );
                SetWindowTextA( GetDlgItem(HW_DLG_MAIN, -124), T2T );
            }
        }
        if( SerHand.NFramesReaded>TimeMaxPFS ){
            break;
        }
        if( MIO_MNG.toc(0)>3 ){
            SerHand.StopRec();
            SerHand.Close();
            MsgBoxErrorF( MB_ICONEXCLAMATION, "\n\nEl dispositivo se ha demorado con la comunicación.\n"
                             "\n"
                             "Compruebe que el puerto es el correcto, desconecte  la "
                             "tarjeta de adquisición y vuelva a conectarla al computador si el problema continúa."
                             "\n\n\n"
                                );
            break;

        }
        #ifdef WITH_SAME_PL
            for( int i=0; i<WITH_SAME_PL; i++ ){
                B2Send[0+i*8] = 123;
                B2Send[1+i*8] = 255&NCNTT;
                B2Send[2+i*8] = HIBYTE(NCNTT);
                B2Send[3+i*8] = 255&NCNTT;
                B2Send[4+i*8] = HIBYTE(NCNTT);
                B2Send[5+i*8] = 255&NCNTT;
                B2Send[6+i*8] = HIBYTE(NCNTT);
                B2Send[7+i*8] = 125;
                NCNTT = 1023&(NCNTT+16);
            }
            SerHand.WriteBuff( B2Send, 8*WITH_SAME_PL );
        #endif // WITH_SAME_PL
        Sleep(10);
    }
    /// B2Send
    #ifdef WITH_SAME_PL
        free( B2Send );
    #endif // WITH_SAME_PL
    /// B2Send
    ///
    ///
    ///
    ///
    if( SerHand.NFramesReaded>TimeMaxPFS ){
        SerHand.NFramesReaded = TimeMaxPFS;
    }
    ///
    /// FIN DE ADQ;
    ///      /////////////////////////////////////////////////////////////////////////
    ///      /////////////////////////////////////////////////////////////////////////
    ///      /////////////////////////////////////////////////////////////////////////
    ///      /////////////////////////////////////////////////////////////////////////
    DWORD IdTh = 0;
    HWFathPgr = HW_DLG_MAIN;
    CreateThread( NULL, 0, Call_Progress, (PVOID)&HWFathPgr, 0, &IdTh );
    SerHand.StopRec();
    SerHand.Close();
    Sleep( 20 );Sleep( 20 );Sleep( 20 );
    ///
    SetWindowText( GetDlgItem( HWFathPgr, 1 ), "Termiando comunicación..." );
    SendMessage( GetDlgItem( HWFathPgr, 2 ), PBM_SETPOS, 80,0);
    Sleep( 300 );
    //B_CONN.Enable( FALSE );
    SetWindowText( GetDlgItem( HWFathPgr, 1 ), "Esperando..." );
    SendMessage( GetDlgItem( HWFathPgr, 2 ), PBM_SETPOS, 33,0);
    while( InPlotThr || InDSPProcsThr ){
        /// SetWindowText( GetDlgItem( HWFathPgr, 1 ), "Esperando ... " );
        if( InPlotThr && InDSPProcsThr ){
            SetWindowText( GetDlgItem( HWFathPgr, 1 ), "Esperando 1..." );
        }else{
            if( InDSPProcsThr ){
                SetWindowText( GetDlgItem( HWFathPgr, 1 ), "Esperando 2..." );
            }
            if( InPlotThr ){
                SetWindowText( GetDlgItem( HWFathPgr, 1 ), "Esperando 3..." );
            }
        }
        Sleep(100);
    }
    if( BRE!=-12345 ){
        sprintf( T2T, "%i", BRE );
        //SetWindowText( HW_DLG_MAIN, T2T );
    }
    ///
    ///
    ///
    /// sprintf( T2T, "%i(%i)", (int)((SerHand.NFramesReaded)/2000), (int)((SerHand.NFramesReaded)/2000)/60 );
    //SetWindowText( HW_DLG_MAIN, T2T );
    ///
    ///
    SetWindowText( GetDlgItem( HWFathPgr, 1 ), "Graficando señales originales..." );
    SendMessage( GetDlgItem( HWFathPgr, 2 ), PBM_SETPOS, 66, 0 );
    ThreadPlot( TRUE );
    SetWindowText( GetDlgItem( HWFathPgr, 1 ), "Realizando procesamiento de las señales..." );
    SendMessage( GetDlgItem( HWFathPgr, 2 ), PBM_SETPOS, 80, 0 );
    ThreadDSPProcs( TRUE );
    ///
    QuitThrConnV = FALSE;
    B_CONN.SetOn();
    Sleep( 500 );
    /// Sleep( 2000 );
    ///
    ///
    //*/
    ///
    sprintf( T2T, " %i Segundos\n adquiridos\n", SerHand.NFramesReaded/2000 );
    SetWindowTextA( GetDlgItem(HW_DLG_MAIN, -124), T2T );
    ///
    ///MIO_MNG.SaveMatFile_Short( "TYU.mat", "TYU", TYU, 4 );
    ///
    ///MIO_MNG.SaveMatFile_Channels_ShortPATH( "SBuff0_2.mat", SerHand.SBuff0, SerHand.SBuff1, SerHand.SBuff2, SerHand.NFramesReaded );
    ///
    SendMessageA( HWFathPgr, WM_CLOSE, 0, 0 );
    while( HWFathPgr!=NULL ){
        Sleep( 50 );
    }
    HWPLOT0_OUT.SetOnSpect( FALSE ); HWPLOT1_OUT.SetOnSpect( FALSE ); HWPLOT2_OUT.SetOnSpect( FALSE );
    B_CONN.Enable( TRUE );
    EnableWindow( GetDlgItem( HW_DLG_MAIN, 2048), TRUE );
    OnThrConn = FALSE;
    return TRUE;
}

