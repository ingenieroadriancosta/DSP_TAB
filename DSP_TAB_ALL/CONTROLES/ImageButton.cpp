struct ImageButton{
    HWND                            HWCTRL;
    HWND                            HWTOOLT;
    HBITMAP                         HBMPT;
    HBITMAP                         HBMON;
    HBITMAP                         HBMOFF;
    int                             W_BUT;
    int                             H_BUT;
    COLORREF                        TColor;
    BOOL                            IsVisi;
    int                             AdWSty;
    ///
    ImageButton(){
        HWCTRL      = NULL;
        HWTOOLT     = NULL;
        HBMPT       = NULL;
        HBMON       = NULL;
        HBMOFF      = NULL;
        AdWSty      = 0;
    }
    ///
    ///
    BOOL DestroyW(){
        return DestroyWindow( HWCTRL );
    }
    ///
    ///
    BOOL FreeBitmaps(){
        DeleteObject( HBMPT );
        DeleteObject( HBMON );
        DeleteObject( HBMOFF );
        return TRUE;
    }
    ///
    ///
    BOOL SetAdicWStyle( int WS_ADD ){
        AdWSty = WS_ADD;
        return TRUE;
    }
    ///
    ///
    BOOL Create( HWND HWFath, int XPos, int YPos, int WWW, int HHH, int HMenu ){
        FreeBitmaps();
        DestroyW();
        HWCTRL = CreateWindowEx(    0, "BUTTON", " Start ",
                                    WS_CHILD | WS_VISIBLE |
                                    WS_TABSTOP | BS_BITMAP | AdWSty,
                     XPos, YPos, WWW, HHH, HWFath, (HMENU)(HMenu),
                     GetModuleHandle(NULL), NULL );
        IsVisi = TRUE;
        W_BUT = WWW;
        H_BUT = HHH;
        return TRUE;
    }
    ///
    ///
    BOOL TextColor( int R, int G, int B ){
        TColor = RGB( R, G, B );
        return TRUE;
    }
    ///
    ///
    BOOL SetOn( void ){
        return SetOn( NULL );
    }
    ///
    ///
    BOOL SetOn( LPCSTR LP2B ){
        DeleteObject( HBMPT );
        HDC ODC = GetDC( NULL );
        HBMPT = CreateCompatibleBitmap( ODC , W_BUT, H_BUT );
        ReleaseDC(NULL,ODC);
        DeleteDC( ODC );
        ///
        BITMAP BmpT = {0};
        GetObject( HBMON, sizeof(BITMAP) , (LPSTR)&BmpT );
        ///
        ///
        HDC hdc = CreateCompatibleDC( GetDC( 0 ) );
        HDC memDC = CreateCompatibleDC( GetDC( 0 ) );
        SelectObject( hdc , HBMON );/// HBPLOT
        SelectObject( memDC , HBMPT );/// HB_I
        SetStretchBltMode( memDC , HALFTONE );
        StretchBlt( memDC, 0, 0, W_BUT, H_BUT, hdc, 0, 0, BmpT.bmWidth, BmpT.bmHeight, SRCCOPY );
        ///
        if( LP2B!=NULL ){
            SetTextAlign( memDC, TA_CENTER );
            SetTextColor( memDC, TColor );
            SetBkMode( memDC, TRANSPARENT );
            TextOut( memDC, W_BUT/2, H_BUT - 20, LP2B, strlen(LP2B) );
        }
        ///
        ReleaseDC( NULL, hdc );
        ReleaseDC( NULL, memDC );
        DeleteDC( hdc );
        DeleteDC( memDC );
        ///
        char T2T[256] = {0};
        sprintf( T2T, "%i - %i\n""%i - %i\n", W_BUT, H_BUT, (int)BmpT.bmWidth, (int)BmpT.bmHeight );
        /// MessageBoxA( HWCTRL, T2T, "", 0 );
        ///
        return (BOOL)SendMessageA( HWCTRL, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)HBMPT );
    }
    ///
    ///
    BOOL SetOff( void ){
        return SetOn( NULL );
    }
    BOOL SetOff( LPCSTR LP2B ){
        DeleteObject( HBMPT );
        HDC ODC = GetDC( HWCTRL );
        HBMPT = CreateCompatibleBitmap( ODC , W_BUT, H_BUT );
        ReleaseDC(HWCTRL,ODC);
        DeleteDC( ODC );
        ///
        BITMAP BmpT = {0};
        GetObject( HBMOFF, sizeof(BITMAP) , (LPSTR)&BmpT );
        ///
        ///
        HDC hdc = CreateCompatibleDC( GetDC( 0 ) );
        HDC memDC = CreateCompatibleDC( GetDC( 0 ) );
        SelectObject( hdc , HBMOFF );/// HBPLOT
        SelectObject( memDC , HBMPT );/// HB_I
        SetStretchBltMode( memDC , HALFTONE );
        StretchBlt( memDC, 0, 0, W_BUT, H_BUT, hdc, 0, 0, BmpT.bmWidth, BmpT.bmHeight, SRCCOPY );
        ///
        if( LP2B!=NULL ){
            SetTextAlign( memDC, TA_CENTER );
            SetTextColor( memDC, TColor );
            SetBkMode( memDC, TRANSPARENT );
            TextOut( memDC, W_BUT/2, 1, LP2B, strlen(LP2B) );
        }
        ///
        ReleaseDC( NULL, hdc );
        ReleaseDC( NULL, memDC );
        DeleteDC( hdc );
        DeleteDC( memDC );
        return (BOOL)SendMessageA( HWCTRL, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)HBMPT );
    }
    ///
    ///
    BOOL SetPicOn( int ir0, int ir1 ){
        HBITMAP HBMP = LoadImageR( ir0, MAKEINTRESOURCEA(ir1) );
        if( HBMP!=NULL ){
            HBMON = HBMP;
        }
        return (HBMON!=NULL);
    }
    //
    BOOL SetPicOn( HBITMAP HBMP ){
        if( HBMP!=NULL ){
            HBMON = HBMP;
        }
        return (HBMON!=NULL);
    }
    ///
    ///
    BOOL SetPicOff( int ir0, int ir1 ){
        HBITMAP HBMP = LoadImageR( ir0, MAKEINTRESOURCEA(ir1) );
        if( HBMP!=NULL ){
            HBMOFF = HBMP;
        }
        return (HBMOFF!=NULL);
    }
    //
    BOOL SetPicOff( HBITMAP HBMP ){
        if( HBMP!=NULL ){
            HBMOFF = HBMP;
        }
        return (HBMOFF!=NULL);
    }
    ///
    ///
    BOOL Enable( BOOL SETE ){
        return EnableWindow( HWCTRL, SETE );
    }
    ///
    ///
    BOOL SetToolTip( LPSTR LPTT ){
        RECT Rect = {0};
        TOOLINFO           TInfo = {0};
        DestroyWindow( HWTOOLT );
        HWTOOLT = CreateWindowEx( WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
                                       WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
                                       0, 0, 0, 0, HWCTRL, NULL, NULL, NULL );
        SetWindowPos( HWTOOLT, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
        GetClientRect ( HWCTRL, &Rect );
        TInfo.cbSize = sizeof(TOOLINFO);
        TInfo.uFlags = TTF_SUBCLASS;
        TInfo.hwnd = HWCTRL;
        TInfo.hinst = NULL;
        TInfo.uId = 0;
        TInfo.lpszText = LPTT;
        TInfo.rect.left = Rect.left;
        TInfo.rect.top = Rect.top;
        TInfo.rect.right = Rect.right;
        TInfo.rect.bottom = Rect.bottom;
        SendMessage( HWTOOLT, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &TInfo );
        return TRUE;
    }
    ///
    ///
    ///
    BOOL SetVisible( BOOL IsVisiI ){
        IsVisi = IsVisiI;
        ShowWindow( HWCTRL, (int)IsVisi );
        return IsVisi;
    }
    ///
    ///
    ///
};
/// /////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////////////






