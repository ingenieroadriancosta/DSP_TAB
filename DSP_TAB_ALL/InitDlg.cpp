BOOL DLG_TAD::InitDlg( HWND hwnd, UINT  message, WPARAM wParam, LPARAM lParam ){
    CreateStatusWindow( WS_CHILD|WS_VISIBLE, "Texto de prueba", hwnd, CMD_STATUS_BLOW );
    HMeW = LoadMenuA( GetModuleHandleA(NULL), MAKEINTRESOURCEA(TAD_WMenuMain));
    SetMenu( hwnd, HMeW );
    SerHand.Reset();
    RECT re;
    memset( &re, 0, sizeof(RECT) );
    GetClientRect(hwnd, &re);
    W_Win = re.right;
    H_Win = re.bottom;
    ///
    ///
    HW_T_Info = GetDlgItem(hwnd,12345);
    ///
    MoveWindow( GetDlgItem(hwnd,123456), 1, 1, 898, 47, TRUE );
    ShowWindow( GetDlgItem( hwnd, 12345), 0 );
    #ifdef MAKEDSPTAD
    if( strcmp( getenv( "USERNAME" ), "ADRIAN COSTA" )==0 ){
        ShowWindow( GetDlgItem( hwnd, 12345), 1 );
    }
    #endif // MAKEDSPTAD
    memset( &re, 0, sizeof(RECT) );
    /// GetClientRect( GetDlgItem(hwnd,123456), &re );
    /// ShowWindow( hwnd, 1); MsgBoxF( "%ix%i", re.right, re.right );
    ///
    ///
    HICON hIcon = LoadIcon( GetModuleHandle(NULL) , MAKEINTRESOURCEA(DLG_ICON) );
    SendMessage( hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon );
    SendMessage( hwnd, WM_SETICON, ICON_BIG  , (LPARAM)hIcon );
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    float DTY = 1.2;
    B_CONN.Create( hwnd, 1, 1, (int)(DTY*80), (int)(DTY*39), CMD_CONN );
    B_CONN.SetToolTip( (LPSTR)"Inicia o termina la conexión con el dispositivo." );
    B_CONN.SetPicOn( PIC_CONN, DSPTABPICS );
    B_CONN.SetPicOff( PIC_DES_CONN, DSPTABPICS );
    B_CONN.TextColor( 255, 255, 255 );
    B_CONN.SetOn();
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    CreateWindowEx( 0, "EDIT", "1",
                   WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE |
                        ES_CENTER | ES_NUMBER | WS_TABSTOP,
                  181,  2,
                   80,  45,
                    hwnd, (HMENU)2048, GetModuleHandle(NULL), NULL );
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    B_SAVE_DATAS.Create( hwnd, W_Win - 75 - 48, 1, 46, 46, CMD_SAVE_DATAS );
    B_SAVE_DATAS.SetToolTip( (LPSTR)"Guardar datos de la aplicación." );
    B_SAVE_DATAS.SetPicOn( PIC_SAVEDATAS, DSPTABPICS );
    B_SAVE_DATAS.TextColor( 255, 255, 255 );
    B_SAVE_DATAS.SetOn();
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    ///MsgBoxF( "%i, %i", 126, (int)(DTY*39) );
    ///B_PORTS.Create( hwnd, B_CONN.W_BUT + 10, 1, 126, 46, CMD_PORTS );
    ///B_PORTS.Create( hwnd, W_Win - 129, 1, 126, 46, CMD_PORTS );
    B_PORTS.Create( hwnd, W_Win - 75, 1, 74, 46, CMD_PORTS );
    B_PORTS.SetToolTip( (LPSTR)"Selección del puerto serial." );
    B_PORTS.SetPicOn( PIC_PORTS, DSPTABPICS );
    B_PORTS.TextColor( 255, 255, 255 );
    B_PORTS.SetOn();
    /// B_SAVE_DATAS.SetVisible( FALSE );B_PORTS.SetVisible( FALSE );
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    B_PRUEBAS.Create( hwnd, 400, 1, (int)(DTY*80), (int)(DTY*39), CMD_CONN-30 );
    B_PRUEBAS.SetToolTip( (LPSTR)"Inicia o termina la conexión con el dispositivo." );
    B_PRUEBAS.SetPicOff( NULL );
    B_PRUEBAS.TextColor( 255, 255, 255 );
    B_PRUEBAS.SetOn( "Pruebas" );
    B_PRUEBAS.SetVisible( FALSE );
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    RECT Rect = {0};
    GetClientRect( hwnd, &Rect );
    W_Win = Rect.right;
    H_Win = Rect.bottom;
    HWND hTab = CreateWindowEx(
        0,                      // extended style
        WC_TABCONTROL,          // tab control constant
        "",                     // text/caption
        WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | TCS_FIXEDWIDTH,  // is a child control, and visible   | 128
        1,                      // X position - device units from left
        48,                      // Y position - device units from top
        W_Win - 1,                    // Width - in device units
        H_Win - 70,                    // Height - in device units
        hwnd,            // parent window
        NULL,                   // no menu
        GetModuleHandle(NULL),                 // instance
        NULL                    // no extra junk
        );
    ///
    SetWindowTextA( GetDlgItem( hwnd, CMD_STATUS_BLOW), "Gráfica de las tres entradas de la adquisición." );
    ///
    HWTABS = hTab;
    ///
    TCITEM tie = {0};  // tab item structure
    //tie.iImage = (int)HBMP_CON;
    tie.mask = TCIF_TEXT | TCIF_IMAGE;  // we're only displaying text in the tabs
    ///
    tie.pszText = (LPSTR)"Entradas";  // the tab's text/caption
    TabCtrl_InsertItem(hTab, 0, &tie);
    ///
    tie.pszText = (LPSTR)"Canal 1";  // the tab's text/caption
    TabCtrl_InsertItem(hTab, 1, &tie);
    ///
    tie.pszText = (LPSTR)"Canal 2";  // the tab's text/caption
    TabCtrl_InsertItem(hTab, 2, &tie);
    ///
    tie.pszText = (LPSTR)"Canal 3";  // the tab's text/caption
    TabCtrl_InsertItem(hTab, 3, &tie);
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    int WTAB = W_Win - 1;
    int HTAB = H_Win - 50 - 40 - 20;
    ///int WPLT = WTAB - 15;
    ///int HPLT = (HTAB)/3;
    ///
    ///
    ///
    ///
    HFNT[0] = CreateFont( 32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                                OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
                                    DEFAULT_PITCH | FF_ROMAN, "Times" );
    SendMessage( GetDlgItem( hwnd, 2048), WM_SETFONT, (WPARAM)HFNT[0], MAKELPARAM(TRUE, 0) );
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
    HWPLOT.PLOTEOS_INIT();
    HWPLOT.SetBackG( 255 );
    HWPLOT.SetFather( HWTABS );
    HWPLOT.SetGridStyle( 1 );
    HWPLOT.ActGridStyle( TRUE );
    HWPLOT.SetVisible( TRUE );
    HWPLOT.SetLocationCon( 4, 30 );
    HWPLOT.SetSizeCon( WTAB - 12, HTAB );
    HWPLOT.FillGrid( TRUE );
    HWPLOT.SetTitle( " Señales de entrada " );
    HWPLOT.Text2ShowF( " Tiempo(Segundos) " );
    HWPLOT.SetMaxYLim( 65535 );
    HWPLOT.SetMaxYLim( 5 );
    HWPLOT.SetMinYLim( 0 );
    ///
    ///
    ///
    ///  TAB 0
    HWPLOT0.PLOTEOS_INIT();
    HWPLOT0.SetBackG( 255 );
    HWPLOT0.SetFather( HWTABS );
    HWPLOT0.SetGridStyle( 1 );
    HWPLOT0.ActGridStyle( TRUE );
    HWPLOT0.SetVisible( FALSE );
    HWPLOT0.SetLocationCon( 6, 30 );
    HWPLOT0.SetSizeCon( WTAB/2 - 10, (int)(HTAB/2) );
    HWPLOT0.FillGrid( TRUE );
    HWPLOT0.SetTitle( " Señal de entrada " );
    HWPLOT0.Text2ShowF( " Tiempo(Segundos) " );
    ///
    HWPLOT0_OUT.PLOTEOS_INIT();
    HWPLOT0_OUT.SetBackG( 255 );
    HWPLOT0_OUT.SetFather( HWTABS );
    HWPLOT0_OUT.SetGridStyle( 1 );
    HWPLOT0_OUT.ActGridStyle( TRUE );
    HWPLOT0_OUT.SetVisible( FALSE );
    HWPLOT0_OUT.SetLocationCon( WTAB/2 + 6 - 4, 30  );
    HWPLOT0_OUT.SetSizeCon( WTAB/2 - 10, (int)(HTAB/2) );
    HWPLOT0_OUT.FillGrid( TRUE );
    HWPLOT0_OUT.SetTitle( " Señal procesada " );
    HWPLOT0_OUT.Text2ShowF( " Tiempo(Segundos) " );
    ///
    ///
    ///
    ///
    ///
    ///
    /// TAB 1
    HWPLOT1.PLOTEOS_INIT();
    HWPLOT1.SetBackG( 255 );
    HWPLOT1.SetFather( HWTABS );
    HWPLOT1.SetGridStyle( 1 );
    HWPLOT1.ActGridStyle( TRUE );
    HWPLOT1.SetVisible( FALSE );
    HWPLOT1.SetLocationCon( 6, 30 );
    HWPLOT1.SetSizeCon( WTAB/2 - 10, (int)(HTAB/2) );
    HWPLOT1.FillGrid( TRUE );
    HWPLOT1.SetTitle( " Señal de entrada " );
    HWPLOT1.Text2ShowF( " Tiempo(Segundos) " );
    ///
    HWPLOT1_OUT.PLOTEOS_INIT();
    HWPLOT1_OUT.SetBackG( 255 );
    HWPLOT1_OUT.SetFather( HWTABS );
    HWPLOT1_OUT.SetGridStyle( 1 );
    HWPLOT1_OUT.ActGridStyle( TRUE );
    HWPLOT1_OUT.SetVisible( FALSE );
    HWPLOT1_OUT.SetLocationCon( WTAB/2 + 6 - 4, 30  );
    HWPLOT1_OUT.SetSizeCon( WTAB/2 - 10, (int)(HTAB/2) );
    HWPLOT1_OUT.FillGrid( TRUE );
    HWPLOT1_OUT.SetTitle( " Señal procesada " );
    HWPLOT1_OUT.Text2ShowF( " Tiempo(Segundos) " );
    ///
    ///
    ///
    ///
    ///
    ///
    HWPLOT2.PLOTEOS_INIT();
    HWPLOT2.SetBackG( 255 );
    HWPLOT2.SetFather( HWTABS );
    HWPLOT2.SetGridStyle( 1 );
    HWPLOT2.ActGridStyle( TRUE );
    HWPLOT2.SetVisible( FALSE );
    HWPLOT2.SetLocationCon( 6, 30 );
    HWPLOT2.SetSizeCon( WTAB/2 - 10, (int)(HTAB/2) );
    HWPLOT2.FillGrid( TRUE );
    HWPLOT2.SetTitle( " Señal de entrada " );
    HWPLOT2.Text2ShowF( " Tiempo(Segundos) " );
    ///
    HWPLOT2_OUT.PLOTEOS_INIT();
    HWPLOT2_OUT.SetBackG( 255 );
    HWPLOT2_OUT.SetFather( HWTABS );
    HWPLOT2_OUT.SetGridStyle( 1 );
    HWPLOT2_OUT.ActGridStyle( TRUE );
    HWPLOT2_OUT.SetVisible( FALSE );
    HWPLOT2_OUT.SetLocationCon( WTAB/2 + 6 - 4, 30  );
    HWPLOT2_OUT.SetSizeCon( WTAB/2 - 10, (int)(HTAB/2) );
    HWPLOT2_OUT.FillGrid( TRUE );
    HWPLOT2_OUT.SetTitle( " Señal procesada " );
    HWPLOT2_OUT.Text2ShowF( " Tiempo(Segundos) " );
    ///
    ///
    ///
    ///
    ///
    ///
    ///                 HWPLOT0_OUT.SetLocationCon( 6, (int)(HTAB/2) + 34  );
    ///
    ///
    /// HWPLOT.Create( HWTABS, 6, 30,                      WTAB - 12, HTAB, 0 );
    ///
    /// HWPLOT0.SetFatherA( HWTABS, 6, 30,                      WPLT, HPLT, 0 );
    /// HWPLOT0.SetFatherA( HWTABS, 6, 30,                      WPLT, HPLT, 0 );
    ///
    /// HWPLOT1.SetFatherA( HWTABS, 6, (HTAB)/2 - HPLT/2 + 30,           WPLT, HPLT, 0 );
    /// HWPLOT2.SetFatherA( HWTABS, 6, HTAB - HPLT + 30,         WPLT, HPLT, 0 );
    //HWPLOT2.Create( HWTABS, 1, (2*(H_Win-40))/3,    W_Win - 10, ((H_Win-10))/3, 0 );
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    HWPLOT0.SetVisible( FALSE );
    HWPLOT1.SetVisible( FALSE );
    HWPLOT2.SetVisible( FALSE );
    ///
    HWPLOT0_OUT.SetMaxYLim( 1 );HWPLOT0_OUT.SetMinYLim( -1 );
    HWPLOT1_OUT.SetMaxYLim( 1 );HWPLOT1_OUT.SetMinYLim( -1 );
    HWPLOT2_OUT.SetMaxYLim( 1 );HWPLOT2_OUT.SetMinYLim( -1 );
    ///
    Refact( &HWPLOT0, &HWPLOT0_OUT );
    Refact( &HWPLOT1, &HWPLOT1_OUT );
    Refact( &HWPLOT2, &HWPLOT2_OUT );
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    CallLoadInit();
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    return TRUE;
}
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL DLG_TAD::Refact( PLOTEOS* Plt0, PLOTEOS* Plt1 ){
    int WTAB = W_Win - 1;
    int HTAB = H_Win - 50 - 40;
    ///
    Plt0->SetLocationCon( 6, 30 );
    Plt0->SetSizeCon( WTAB/2 - 10, (int)(HTAB/2) + 117 );
    ///
    Plt1->SetLocationCon( WTAB/2 + 6 - 4, 30  );
    Plt1->SetSizeCon( WTAB/2 - 10, (int)(HTAB/2) + 117 );
    Plt1->SetVisible( FALSE );
    ///
    Plt0->FillGrid( TRUE );
    Plt1->FillGrid( TRUE );
    ///
    ///
    if( SetedButts ){
        return FALSE;
    }
    SetedButts = TRUE;
    ///
    int W_B_SDSP = WTAB - 17;
    W_B_SDSP = (W_B_SDSP - W_B_SDSP%8)/8;
    /// MsgBoxF( "W_B_SDSP = %i", W_B_SDSP );
    for( int i=0; i<8; i++ ){
        B_DSP_PROCS[i].SetAdicWStyle( BS_NOTIFY );
        B_DSP_PROCS[i].Create( HW_DLG_MAIN, 7 + (i*(W_B_SDSP+1) ),
                               H_Win - W_B_SDSP - 7 - 20,
                               W_B_SDSP, W_B_SDSP, CMD_DSP_PROCS+i );
        B_DSP_PROCS[i].SetPicOn( PIC_NONE_PROC, DSPTABPICS );
        B_DSP_PROCS[i].SetOn();
        B_DSP_PROCS[i].SetVisible( FALSE );
        B_DSP_PROCS[i].SetToolTip( (LPSTR)"Procesado de la señal. Doble Click ó Ctr + Click para modificar." );
        ///B_DSP_PROCS[i].Enable( i==0 );
    }
    return TRUE;
}
/// /////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL DLG_TAD::LoadInit(){
    ShowWindow( HW_DLG_MAIN, 1 );
    Sleep( 50 );
    SerHand.FreeAll();
    SerHand.Reset();
    HWFathPgr = HW_DLG_MAIN;
    DWORD IdTh = 0;CreateThread( NULL, 0, Call_Progress, (PVOID)&HWFathPgr, 0, &IdTh );
    Sleep( 100 );
    SetWindowText( GetDlgItem( HWFathPgr, 1 ), "Cargando datos de la ultima sesión..." );
    MIO_MNG.HWActiWin = HWFathPgr;
    if( MIO_MNG.LoadRestFileTAD( this, HWFathPgr )==TRUE ){
        Sleep( 100 );
        ///
        char T2TT[8] = {0};
        sprintf( T2TT, "%i", TimeMax );
        SetWindowTextA( GetDlgItem( HW_DLG_MAIN, 2048), T2TT );
        ///
        HWPLOT0.Hold( FALSE );HWPLOT1.Hold( FALSE );HWPLOT2.Hold( FALSE );HWPLOT.Hold( FALSE );
        HWPLOT.SetMaxYLim( 5 );
        HWPLOT0.SetLineColor( LineColors[0] );
        HWPLOT1.SetLineColor( LineColors[0] );
        HWPLOT2.SetLineColor( LineColors[0] );
        ///ShowWindow( HW_DLG_MAIN, 1 );
        ///MsgBoxF( " " );
        ///
        SetWindowText( GetDlgItem( HWFathPgr, 1 ), "Graficando señales originales..." );
        ///SendMessage( GetDlgItem( HWFathPgr, 2 ), PBM_SETPOS, 66,0);
        ThreadPlot( TRUE );
        SendMessage( GetDlgItem( HWFathPgr, 2 ), PBM_SETPOS, 99,0);
        SetWindowText( GetDlgItem( HWFathPgr, 1 ), "Realizando procesamiento de las señales..." );
        ///SendMessage( GetDlgItem( HWFathPgr, 2 ), PBM_SETPOS, 80,0);
        ThreadDSPProcs( TRUE );
        char T2T[128] = {0};
        sprintf( T2T, " %i Segundos\n adquiridos\n", SerHand.NFramesReaded/2000 );
        SetWindowTextA( GetDlgItem(HW_DLG_MAIN, -124), T2T );
        ///
    }
    InPlotThr = FALSE;
    InDSPProcsThr = FALSE;
    SendMessageA( HWFathPgr, WM_CLOSE, 0, 0 );
    return TRUE;
}










