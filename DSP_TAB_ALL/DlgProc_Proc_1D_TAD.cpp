BOOL CALLBACK DlgProc_Proc_1D_TAD(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam){
    static HWND HW_DP_TI = NULL;
    static HWND HW_LB_PRC = NULL;
    static FDESIGN_CLASS* FDSP = NULL;
    static int ITemp = 0;
    /// char CTINFO[1024] = {0};
    const char ProcInfo[8][32] = { "Gráfica de la señal",
                                   "Aplica un filtro a la señal",
                                   "Aplica una ventana a la señal",
                                   "Aplica la fft a la señal",
                                   "Análisis por espectrograma",
                                   "Análisis Wigner - Ville",
                                   "Análisis Choi - Williams",
                                   "Eliminar el offset"
                                     };
    switch(msg){
        case WM_INITDIALOG:{
            FDSP = (FDESIGN_CLASS*)lParam;
            HW_DP_TI = GetDlgItem( hDlg, 10 );
            ///
            HW_LB_PRC = GetDlgItem( hDlg, 20 );
            SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"Gráfica");
            ///SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"Convolución");
            ///SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"Correlación");
            SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"Filtrado");
            SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"Ventanas");
            SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"Analisis espectral(fft)");
            SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"Espectrograma");
            SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"Wigner - Ville");
            SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"Choi - Williams");
            SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"Eliminar offset");
            SendMessage( HW_LB_PRC, LB_SETCURSEL, (WPARAM)FDSP->GetTYPE_PROC(), 0 );
            SetWindowTextA( HW_DP_TI, ProcInfo[SendMessage( HW_LB_PRC, LB_GETCURSEL, 0, 0 )] );
            ///
            ///
            SendDlgItemMessage( hDlg, 30, BM_SETCHECK, FDSP->GetBInput(), 0 );
            ///
            /// CenterWindow( hDlg, 0, 60 );
            ///
            ///
            ///
            }
            return TRUE;
        case WM_COMMAND:
            if( FDSP==NULL ){
                break;
            }
            switch( LOWORD(wParam) ){
                case 20:
                    switch( HIWORD(wParam) ){
                        case LBN_SELCHANGE:
                            //sprintf( CTINFO, "%s", ProcInfo[SendMessage( HW_LB_PRC, LB_GETCURSEL, 0, 0 )] );
                            SetWindowTextA( HW_DP_TI, ProcInfo[SendMessage( HW_LB_PRC, LB_GETCURSEL, 0, 0 )] );
                            break;
                        case LBN_DBLCLK:
                            FDSP->SetBInput( SendDlgItemMessage(hDlg, 30, BM_GETCHECK, 0, 0) == BST_CHECKED );
                            FDSP->SetTYPE_PROC( SendMessage( HW_LB_PRC, LB_GETCURSEL, 0, 0 ) );
                            ITemp = 128*FDSP->GetBInput() + FDSP->GetTYPE_PROC();
                            FDSP = NULL;
                            /// EndDialog( hDlg, SendMessage( HW_LB_PRC, LB_GETCURSEL, 0, 0 ) );
                            EndDialog( hDlg, ITemp );
                            break;
                    }
                    break;
                case IDOK:
                    FDSP->SetTYPE_PROC( SendMessage( HW_LB_PRC, LB_GETCURSEL, 0, 0 ) );
                    FDSP->SetBInput( SendDlgItemMessage(hDlg, 30, BM_GETCHECK, 0, 0) == BST_CHECKED );
                    ITemp = 128*FDSP->GetBInput() + FDSP->GetTYPE_PROC();
                    FDSP = NULL;
                    /// EndDialog( hDlg, SendMessage( HW_LB_PRC, LB_GETCURSEL, 0, 0 ) );
                    EndDialog( hDlg, ITemp );
                    break;
                case IDCANCEL:
                    FDSP = NULL;
                    EndDialog( hDlg, -2 );
                    break;
            }
            return TRUE;
    }
        return FALSE;
}
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK DlgProc_Proc_ABOUT(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam){
    ///
    static HBITMAP HBMPA = NULL;
    HICON hIcon = NULL;
    switch(msg){
        case WM_INITDIALOG:{
            HBMPA = LoadImageR( PIC_DSP_TAD_ABOUT, MAKEINTRESOURCEA(DSPTABPICS) );
            BITMAP BMP = {0};
            GetObjectA( HBMPA, sizeof(BITMAP), &BMP );
            SendMessage( GetDlgItem(hDlg, 2), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)HBMPA );
            MoveWindow( GetDlgItem(hDlg, 2), 1, 1, BMP.bmWidth+2, BMP.bmHeight+2, TRUE );
            ///
            const int CXSCREEN2 = GetSystemMetrics( SM_CXSCREEN );
            const int CYSCREEN2 = GetSystemMetrics( SM_CYSCREEN );
            ///
            const int WDD = BMP.bmWidth + 9;
            const int HDD = BMP.bmHeight + 32;
            const int bX = (WDD+CXSCREEN2)/2 - WDD;
            /// const int bY = (HDD+CYSCREEN2)/2 - HDD - 80;
            const int bY = (HDD+CYSCREEN2)/2 - HDD;
            /// SetWindowPos( hDlg, HWND_TOPMOST, bX, bY, WDD, HDD, 1 );
            MoveWindow( hDlg, bX, bY, WDD, HDD, TRUE );
            ///
            hIcon = LoadIcon( GetModuleHandle(NULL) , MAKEINTRESOURCEA(DLG_ICON) );
            SendMessage( hDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon );
            SendMessage( hDlg, WM_SETICON, ICON_BIG  , (LPARAM)hIcon );
            ///
        }
        break;
        case WM_CLOSE:
            DeleteObject( HBMPA );
            DeleteObject( hIcon );
            HBMPA = NULL;
            hIcon = NULL;
            EndDialog( hDlg, 0 );
            break;
    }
    ///
    return FALSE;
}
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK DlgProc_Proc_SAVE_ADV( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam ){
    static HWND HWGF = NULL;
    static int I2Ret = 0;
    switch(msg){
        case WM_INITDIALOG:{
            I2Ret = 0;   /// MoveWindow( HW_W, bX, bY, WDD, HDD, TRUE );                    737.717
            if( HWGF!=NULL ){
                HWGF = GetFocus();
            }
            HWGF = GetFocus();
            RECT re2ST = {0};
            CenterWindowDLG( hDlg, 0, 60, &re2ST );
            MoveWindow( GetDlgItem(hDlg, -1), 1, 1, re2ST.right-2, re2ST.bottom-2, TRUE );
            SetWindowTextA( GetDlgItem(hDlg, -1), (LPCSTR)lParam );
            MoveWindow( GetDlgItem(hDlg, 1), re2ST.right - 84-70, re2ST.bottom-43,            74, 37, TRUE );
            MoveWindow( GetDlgItem(hDlg, 2), re2ST.right - 78, re2ST.bottom-43,            74, 37, TRUE );
            ///
            ///
            MoveWindow( GetDlgItem(hDlg, 3),                3, re2ST.bottom-43, 63, 36, TRUE );
            MoveWindow( GetDlgItem(hDlg, 4),                3+66, re2ST.bottom-43, 63, 36, TRUE );
            MoveWindow( GetDlgItem(hDlg, 5),                3+131, re2ST.bottom-43, 63, 36, TRUE );
            #define HSPR_M 34
            MoveWindow( GetDlgItem(hDlg, 6),                3, re2ST.bottom-66-HSPR_M, re2ST.right-6, HSPR_M, TRUE );
            ///
            ///
            SendDlgItemMessage( hDlg, 3, BM_SETCHECK, BST_CHECKED, 0 );
            SendDlgItemMessage( hDlg, 4, BM_SETCHECK, BST_CHECKED, 0 );
            SendDlgItemMessage( hDlg, 5, BM_SETCHECK, BST_CHECKED, 0 );
            // SendDlgItemMessage( hDlg, 6, BM_SETCHECK, BST_CHECKED, 0 );
            SetFocus( GetDlgItem(hDlg, 1) );
        }
        break;
        case WM_COMMAND:{
            switch( LOWORD(wParam) ){
                case 1:
                    I2Ret = 1*SendDlgItemMessage( hDlg, 3, BM_GETCHECK, 0, 0 ) +
                            2*SendDlgItemMessage( hDlg, 4, BM_GETCHECK, 0, 0 ) +
                            4*SendDlgItemMessage( hDlg, 5, BM_GETCHECK, 0, 0 ) +
                            128*SendDlgItemMessage( hDlg, 6, BM_GETCHECK, 0, 0 );
                    if( I2Ret==0 ){
                        MessageBoxA( hDlg, "Debe seleccionar al menos un canal.", " ", 16 );
                        return FALSE;
                    }
                    EndDialog( hDlg, I2Ret );
                    break;
                case 2:
                    EndDialog( hDlg, -1 );
                    break;
                case 3:
                    I2Ret = SendDlgItemMessage( hDlg, 3, BM_GETCHECK, 0, 0 );
                    /// sprintf( T2T, "%i", I2Ret );
                    /// MessageBoxA( hDlg, T2T, "", 0 );
                    /// SetFocus( hDlg );
                    break;
            }
        }
    }
    return FALSE;
}






