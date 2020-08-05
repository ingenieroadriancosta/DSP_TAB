DWORD WINAPI Call_ThreadConn( LPVOID pv=NULL ){ DLG_TAD* DLGT = (DLG_TAD*)pv; DLGT->ThreadConn(); return 0; }
DWORD WINAPI Call_ThreadCmd( LPVOID pv=NULL ){
    DLG_TAD* DLGT = (DLG_TAD*)pv;
    if( DLGT->InCmdProc==TRUE ){
        return 0;
    }
    DLGT->InCmdProc = TRUE;
    DLGT->CmdProcs( DLGT->hwndP, DLGT->messageP, DLGT->wParamP, DLGT->lParamP );
    return 0;
}
DWORD WINAPI Call_ThreadPlot( LPVOID pv=NULL ){ DLG_TAD* DLGT = (DLG_TAD*)pv; DLGT->ThreadPlot(FALSE); return 0; }
DWORD WINAPI Call_ThreadDSPProcs( LPVOID pv=NULL ){ DLG_TAD* DLGT = (DLG_TAD*)pv; DLGT->ThreadDSPProcs(FALSE); return 0; }
DWORD WINAPI Call_LoadInit( LPVOID pv=NULL ){ DLG_TAD* DLGT = (DLG_TAD*)pv; DLGT->LoadInit(); return 0; }
DWORD WINAPI Call_Progress( LPVOID pv=NULL ){
    Sleep( 1 );
    HWND* HWFatPgrs = (HWND*)pv;
    Sleep( 1 );
    ///HWFathPgr = CreateDialogParam( GetModuleHandle(NULL), MAKEINTRESOURCE(Dial_Progress_Procs), HWFatPgrs[0],(DLGPROC)&DlgProgress, 0 );
    DialogBoxParamA( GetModuleHandle(NULL), MAKEINTRESOURCE(Dial_Progress_Procs), HWFatPgrs[0], (DLGPROC)DlgProgress, (LPARAM)HWFatPgrs );
    return 0;
}










