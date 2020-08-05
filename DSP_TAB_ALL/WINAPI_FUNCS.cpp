BOOL ParseEditDouble( WPARAM wParam, LPARAM lParam ){
    char T2T[32] = {0};
    char T2T2[32] = {0};
    BOOL IsPSet = FALSE;
    GetWindowTextA( (HWND)lParam, T2T, 16 );
    wParam = T2T[strlen(T2T)-1];
    int SLen = strlen(T2T);
    int IndT = 0;
    BOOL SetTT = FALSE;
    int indsel = strlen(T2T);
    for( int i=0; i<SLen; i++ ){
        wParam = T2T[i];
        if( !((wParam >= '0' && wParam <= '9') || wParam == '.' ///|| wParam == VK_RETURN || wParam == VK_DELETE
                ///|| wParam == VK_BACK
                ) ){
            SetTT = TRUE;
            indsel = i;
        }else{
            if( IsPSet && T2T[i]=='.' ){
                SetTT = TRUE;
                indsel = i;
                continue;
            }else{
                if( T2T[i]=='.' ){
                    IsPSet = TRUE;
                }
                T2T2[IndT] = T2T[i];
                IndT++;
                T2T2[IndT] = 0;
            }
        }
    }
    if( SetTT ){
        SetWindowTextA( (HWND)lParam, T2T2 );
        SendMessage( (HWND)lParam, (UINT)EM_SETSEL, indsel, indsel );
    }
    return TRUE;
}
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL SetToolTip( HWND HWCTRL, LPCSTR LPTT, HWND* HWndT ){
    if( HWndT==NULL ){ return TRUE; }
    RECT Rect = {0};
    TOOLINFO           TInfo = {0};
    HWndT[0] = CreateWindowEx( WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
                                   WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
                                   0, 0, 0, 0, HWCTRL, NULL, NULL, NULL );
    SetWindowPos( HWndT[0], HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    GetClientRect ( HWCTRL, &Rect );
    TInfo.cbSize = sizeof(TOOLINFO);
    TInfo.uFlags = TTF_SUBCLASS;
    TInfo.hwnd = HWCTRL;
    TInfo.hinst = NULL;
    TInfo.uId = 0;
    TInfo.lpszText = (LPSTR)LPTT;
    TInfo.rect.left = Rect.left;
    TInfo.rect.top = Rect.top;
    TInfo.rect.right = Rect.right;
    TInfo.rect.bottom = Rect.bottom;
    SendMessage( HWndT[0], TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &TInfo );
    return TRUE;
}
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL Ch2WCh( LPWCH LPW, LPCSTR LPS, int IMax ){
    for( int i=0; i<IMax; i++ ){
        LPW[i] = LPS[i];
    }
    LPW[IMax] = 0;
    return TRUE;
}
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL SetToolTipW( HWND HWCTRL, LPCWSTR LPTT, HWND* HWndT ){
    if( HWndT==NULL ){ return TRUE; }
    RECT Rect = {0};
    TOOLINFOW           TInfo = {0};
    DestroyWindow( HWndT[0] );
    HWndT[0] = CreateWindowExW( WS_EX_TOPMOST, TOOLTIPS_CLASSW, NULL,
                                   WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
                                   0, 0, 0, 0, HWCTRL, NULL, NULL, NULL );
    SetWindowPos( HWndT[0], HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    GetClientRect ( HWCTRL, &Rect );
    TInfo.cbSize = sizeof(TOOLINFOW);
    TInfo.uFlags = TTF_SUBCLASS;
    TInfo.hwnd = HWCTRL;
    TInfo.hinst = NULL;
    TInfo.uId = 0;
    TInfo.hinst = GetModuleHandleA( NULL );
    TInfo.lpszText = (LPWSTR)LPTT;
    TInfo.rect.left = Rect.left;
    TInfo.rect.top = Rect.top;
    TInfo.rect.right = Rect.right;
    TInfo.rect.bottom = Rect.bottom;
    SendMessageW( HWndT[0], TTM_ADDTOOLW, 0, (LPARAM) (LPTOOLINFOW) &TInfo );
    return TRUE;
}
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
inline int ISDOWNKEY( int Key = VK_SPACE ){ return int(GetKeyState( Key )& 0x80); }
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int Copy2ClipBoard2( char* Texto ){
    size_t len = strlen( Texto );
    HGLOBAL hMem =  GlobalAlloc(GMEM_MOVEABLE, len);
    memcpy( GlobalLock(hMem), Texto, len );
    GlobalUnlock(hMem);
    OpenClipboard(0);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();
    return 0;
}
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
BYTE ParceBYTE( double DVL ){
    if( DVL>255 ){
        return 255;
    }
    return ( (BYTE)DVL );
}




/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
