int WINAPI SaveFileAs( char *FileName, LPCSTR Filter, HWND HWFath ){
        char FileNameAux[1024] = { 0 };
    ZeroMemory( &FileNameAux[0], 1023 );
    strcpy( FileNameAux, FileName );
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = HWFath;
    ofn.nFilterIndex = 0;
    ofn.lpstrFileTitle = NULL;
    ///ofn.lpstrInitialDir= "C:";
    ofn.lpstrFilter = Filter;//"BMP Files (*.bmp*)\0*.bmp\0"
                      //"JPG Files (*.jpg*)\0*.jpg\0";
    ofn.lpstrFile = FileNameAux;
    ofn.nMaxFile = 1023;
    ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY |
                                    OFN_OVERWRITEPROMPT;
    ofn.lpstrDefExt = "";
    if( !GetSaveFileNameA(&ofn) ){
        return 0;
    }
    sprintf( FileName, "%s", FileNameAux );
    int LenStr = strlen( FileName )-1;
    if( FileName[LenStr]=='g' || FileName[LenStr]=='G' ){
        return 2;
    }
    return 1;
}

/// ///////////////////////////////////////////////////////////////////////////////////////////
/*
bool OpenFileAs( char *FileName ){
char FileNameAux[256] = { 0 };
ZeroMemory( &FileNameAux[0], 250 );
strcpy( FileNameAux, FileName );
OPENFILENAME  openFileDialogfunction = {0};
ZeroMemory(&openFileDialogfunction, sizeof(openFileDialogfunction));
openFileDialogfunction.lStructSize= sizeof(openFileDialogfunction);
openFileDialogfunction.hwndOwner = VENTANA_PADRE;
openFileDialogfunction.nFilterIndex=0;
openFileDialogfunction.lpstrTitle="Abrir";
openFileDialogfunction.hInstance = INSTANCIA_GLOBAL;
openFileDialogfunction.lpstrFilter = "All images (*.png, *.jpg, *.jpeg, *.gif, *.bmp)\0" "*.png;*.jpg;*.jpeg;*.gif;*.bmp\0"
                                     "BMP Files (*.bmp*)\0*.bmp\0"
                                     "JPG Files (*.jpg, *.jpeg)\0*.jpg;*.jpeg\0"
                                     "PNG Files (*.png*)\0*.png\0"
                                     "GIF Files (*.gif*)\0*.gif\0"
                                     "All Files(*.*)\0*.*\0\0";
openFileDialogfunction.lpstrFile = FileNameAux;
openFileDialogfunction.nMaxFile = 8*MAX_PATH;
openFileDialogfunction.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
openFileDialogfunction.lpstrDefExt = "";
if(!GetOpenFileName(&openFileDialogfunction))
   return false;
sprintf( FileName, "%s", FileNameAux);
return true;
}
//*/
/// ///////////////////////////////////////////////////////////////////////////////////////////
BOOL CharEndsWith( char* StrIn, LPCSTR StrMatch ){
    const int CLenI = strlen( StrIn );
    const int CLenM = strlen( StrMatch );
    if( CLenI<CLenM ){
        return FALSE;
    }
    BOOL YesEW = TRUE;
    for( int i=0; i<CLenM; i++ ){
        YesEW = YesEW && ( StrIn[CLenI-1-i]==StrMatch[CLenM-1-i] );
    }
    return YesEW;
}


/// ///////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////
BOOL CenterWindow( HWND HW_W, int XCorri, int YCorri, RECT* RecOut=NULL ){
    const int CXSCREEN2 = GetSystemMetrics( SM_CXSCREEN );
    const int CYSCREEN2 = GetSystemMetrics( SM_CYSCREEN );
    RECT re = {0};
    memset( &re, 0, sizeof(RECT) );
    GetClientRect( HW_W, &re );
    ///
    const int WDD = re.right;
    const int HDD = re.bottom;
    const int bX = (WDD+CXSCREEN2)/2 - WDD - XCorri;
    const int bY = (HDD+CYSCREEN2)/2 - HDD - YCorri;
    SetWindowPos( HW_W, GetParent(HW_W), bX, bY, WDD, HDD, 1 );
    if( RecOut!=NULL ){
        memcpy( RecOut, &re, sizeof(RECT) );
    }
    return TRUE;
}

/// ///////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////
BOOL CenterWindowDLG( HWND HW_W, int XCorri, int YCorri, RECT* RecOut=NULL ){
    const int CXSCREEN2 = GetSystemMetrics( SM_CXSCREEN );
    const int CYSCREEN2 = GetSystemMetrics( SM_CYSCREEN );
    const HWND HWF = GetParent( HW_W );
    RECT re = {0};
    memset( &re, 0, sizeof(RECT) );
    GetClientRect( HW_W, &re );
    ///
    const int WDD = re.right;
    const int HDD = re.bottom;
    const int bX = (WDD+CXSCREEN2)/2 - WDD - XCorri;
    const int bY = (HDD+CYSCREEN2)/2 - HDD - YCorri;
    SetWindowPos( HW_W, HWF, bX, bY, WDD, HDD, 1 );
    if( RecOut!=NULL ){
        memcpy( RecOut, &re, sizeof(RECT) );
    }
    return TRUE;
}








