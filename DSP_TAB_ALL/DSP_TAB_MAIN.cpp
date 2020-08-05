int IWINDOW = 0;
#if (_WIN32_WINNT < 0x0500)
    #undef _WIN32_WINNT
    #define  _WIN32_WINNT 0x0500
#endif
#include <windows.h>
#include <shlwapi.h>
#include <commctrl.h>
#include <stdio.h>
#include "Many_Funcs.h"
///
double FrqSimul = 0.06; /// 20000 ///
const int TimeMillis = 10;/// 150
///
#include "includes_DSP_TAB.h"
#include "FDESIGN\TYPES_FDESIGN.cpp"
/// ////
/*
CreateStatusWindow(WS_CHILD|WS_VISIBLE, "Texto de prueba", hwnd, ID_STATUS);


        case WM_SIZE:
           SendDlgItemMessage(hwnd, ID_STATUS, WM_SIZE, 0, 0);




        case WM_MENUSELECT:
           if((HIWORD(wParam) & MF_POPUP) && ((HMENU)lParam == GetMenu(hwnd))) desplazamientos[1] = 800;
           else desplazamientos[1] = 802;
           MenuHelp(msg, wParam, lParam, (HMENU)lParam, hInstance, GetDlgItem(hwnd, ID_STATUS), desplazamientos);
           break;
//*/
struct DLG_TAD{
    ///
    BOOL    Dlg_Proc_BIG           (HWND , UINT , WPARAM , LPARAM );
    BOOL    InitDlg                (HWND , UINT , WPARAM , LPARAM );
    BOOL    CmdProcs               (HWND , UINT , WPARAM , LPARAM );
    BOOL    LoadInit               ( void );
    BOOL    ThreadConn             ( void );
    BOOL    ThreadPlot             ( BOOL );
    BOOL    ThreadDSPProcs         ( BOOL );
    BOOL    ThreadDSPProcsN        ( int );
    BOOL    Refact                 ( PLOTEOS*, PLOTEOS* );
    BOOL    DSPProc0               ( fftprepare*, short*, int, short*, PLOTEOS*, int, BOOL );
    BOOL    SaveProc               ( void );
    BOOL    GetChannelProc         ( int, LPSTR, BOOL );
    ///
    ///
    ///
    ///
    int                                     IPtr;
    HWND                                    HW_DLG_MAIN;
    BOOL                                    OnWindow;
    BOOL                                    OnTop;
    HMENU                                   HMeW;
    HFONT                                   HFNT[8];
    BOOL                                    SHADV;
    int                                     W_Win;
    int                                     H_Win;
    HWND                                    HWTABS;
    HWND                                    HWFathPgr;
    HWND                                    HW_T_Info;
    int                                     TabPage;
    SERIAL_HANDLE                           SerHand;
    int                                     TimeMax;
    int                                     Port2Conn;
    ///
    ///
    /// CONEXION:
    ImageButton                             B_PRUEBAS;
    ImageButton                             B_CONN;
    ImageButton                             B_PORTS;
    ImageButton                             B_SAVE_DATAS;
    ///
    BOOL                                    SetedButts;
    ///
    ImageButton                             B_DSP_PROCS[8];
    ///
    HWND                                    HWCONEXION;
    ///
    /// PLOTS:
    PLOTEOS                                 HWPLOT;
    PLOTEOS                                 HWPLOT0;
    PLOTEOS                                 HWPLOT0_OUT;
    PLOTEOS                                 HWPLOT1;
    PLOTEOS                                 HWPLOT1_OUT;
    PLOTEOS                                 HWPLOT2;
    PLOTEOS                                 HWPLOT2_OUT;
    ///
    BOOL                                    OnThrConn;
    BOOL                                    QuitThrConnV;
    ///BOOL                                    ForcePlot;
    BOOL                                    InPlotThr;
    BOOL                                    InDSPProcsThr;
    ///
    BOOL                                    InCmdProc;
    HWND                                    hwndP;
    UINT                                    messageP;
    WPARAM                                  wParamP;
    LPARAM                                  lParamP;
    ///
    ///
    int                                     ChannSel;
    int                                     ButSel[4];
    int                                     DSP_TYPE_PROC[4][8];
    fftprepare                              ftp;
    FDESIGN_CLASS                           FDS[3][8];
    ///
    ///
    DLG_TAD(){
        IPtr = 0;
        IPtr = (int)&IPtr;
        TabPage = 0;
        HW_DLG_MAIN   = NULL;
        OnThrConn     = FALSE;
        QuitThrConnV  = FALSE;
        ///ForcePlot     = FALSE;
        InPlotThr     = FALSE;
        InCmdProc     = FALSE;
        Port2Conn = -1;
        ChannSel  = 0;
        FreeAll();
        SerHand.FreeAll();
        for( int x=0; x<3; x++ ){
            for( int y=0; y<8; y++ ){
                FDS[x][y].FixedFs = TRUE;
            }
        }
        SHADV = TRUE;
        TimeMax = 1;
    }
    /// CMD_CONN
    BOOL IsOnWindow(){ return OnWindow; }
    BOOL Show(){
        OnWindow = TRUE;
        MSG             messages = {0};
        HACCEL hAcelerador2;
        hAcelerador2 = LoadAccelerators( GetModuleHandleA(NULL) , MAKEINTRESOURCE(TAD_aceleradores) );
        HWND HW_Fath = CreateDialogParam( GetModuleHandleA(NULL), MAKEINTRESOURCE(DLG_MAIN), NULL,
                                    (DLGPROC)&DlgMain, 0 );
        ShowWindow( HW_Fath, 1 );
        while( GetMessage( &messages, NULL, 0, 0)==TRUE && OnWindow ){
            if( TranslateAccelerator(HW_Fath, hAcelerador2,&messages) ){
                continue;
            }
            if( !IsDialogMessage(HW_Fath, &messages) ){
                TranslateMessage(&messages);
                DispatchMessage(&messages);
            }
        }
        return TRUE;///(BOOL)DialogBoxParamA( GetModuleHandle(NULL), MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain, 0  );
    }
    ///
    ///
    BOOL DeleteBitmaps( void ){
        return TRUE;
    }
    ///
    ///
    ///
    BOOL FreeAll(){
        HWPLOT.FreeAll();
        HWPLOT0_OUT.FreeAll();
        HWPLOT1.FreeAll();
        HWPLOT1_OUT.FreeAll();
        HWPLOT2.FreeAll();
        HWPLOT2_OUT.FreeAll();
        ftp.Free();
        for( int x=0; x<3; x++ ){
            for( int y=0; y<8; y++ ){
                FDS[x][y].Free();
            }
        }
        SetedButts = FALSE;
        return TRUE;
    }
    ///
    ///
    BOOL QuitThrConn(){
        QuitThrConnV = TRUE;
        return TRUE;
    }
    ///
    BOOL CallPlot( void ){
        if( InPlotThr ){
            return FALSE;
        }
        InPlotThr = TRUE;
        DWORD IdTh = 0;
        CreateThread( NULL, 0, Call_ThreadPlot, (PVOID)&IPtr, 0, &IdTh );
        return TRUE;
    }
    ///
    BOOL CallLoadInit( void ){
        if( InPlotThr ){
            return FALSE;
        }
        InPlotThr = TRUE;
        DWORD IdTh = 0;
        CreateThread( NULL, 0, Call_LoadInit, (PVOID)&IPtr, 0, &IdTh );
        return TRUE;
    }
    ///
    ///
    ///
    ///
    ///
    BOOL CallDSPProcs( void ){
        if( InDSPProcsThr ){
            return FALSE;
        }
        InDSPProcsThr = TRUE;
        DWORD IdTh = 0;
        CreateThread( NULL, 0, Call_ThreadDSPProcs, (PVOID)&IPtr, 0, &IdTh );
        return TRUE;
    }
    ///
    ///
    BOOL SetPicButts(){
        const int Pic_Procs[8] = { PIC_PLOT_PROC,
                                   PIC_FILT_PROC,
                                   PIC_WINDOW_PROC,
                                   PIC_FFT_PROC,
                                   PIC_SPECT_PROC,
                                   PIC_WIGNER_PROC,
                                   PIC_CHOIW_PROC,
                                   PIC_CENTRAR_PROC };
        int ProcAct = 0;
        for( int i=0; i<8; i++ ){
            ProcAct = DSP_TYPE_PROC[ChannSel][i];
            B_DSP_PROCS[i].SetPicOn( Pic_Procs[ProcAct], DSPTABPICS );
            B_DSP_PROCS[i].SetOn();
            if( i==ButSel[ChannSel] ){
                B_DSP_PROCS[i].SetOn( "Seleccionado" );
            }else{
                B_DSP_PROCS[i].SetOn( NULL );
            }
        }
        return TRUE;
    }
    ///
    ///
    ///
    ///
    BOOL SetInfoChannel( void ){
        /*
        ButSel[4];
        DSP_TYPE_PROC[4][8];
        FDS[3][8];
        //*/
        char T2T[1024] = {0};
        switch( DSP_TYPE_PROC[ChannSel][ButSel[ChannSel]] ){
            case 0:
                sprintf( T2T,
                    "Gráfica y procesamiento de canal %i. Módulo seleccionado: %i.     "
                    "Tipo de acción(Gráfica).",
                    TabPage, ButSel[ChannSel]  );
                break;
            case 1:
                if( FDS[ChannSel][ButSel[ChannSel]].IsIRR ){
                    switch( FDS[ChannSel][ButSel[ChannSel]].GetFilType() ){
                        case 0:
                            sprintf( T2T,
                                "Gráfica y procesamiento de canal %i. Módulo seleccionado: %i.     "
                                "Tipo de acción (Filtrado). Tipo: BUTTERWORTH de orden %i pasa bajo fc(%g) Hz",
                                TabPage, ButSel[ChannSel],
                                FDS[ChannSel][ButSel[ChannSel]].GetOrdFil(),
                                    FDS[ChannSel][ButSel[ChannSel]].GetFcH()  );
                            break;
                        case 1:
                            sprintf( T2T,
                                "Gráfica y procesamiento de canal %i. Módulo seleccionado: %i.     "
                                "Tipo de acción (Filtrado). Tipo: BUTTERWORTH de orden %i pasa alto fc(%g) Hz",
                                TabPage, ButSel[ChannSel],
                                FDS[ChannSel][ButSel[ChannSel]].GetOrdFil(),
                                    FDS[ChannSel][ButSel[ChannSel]].GetFcL()  );
                            break;
                        case 2:
                            sprintf( T2T,
                                "Gráfica y procesamiento de canal %i. Módulo seleccionado: %i.     "
                                "Tipo de acción (Filtrado). Tipo: BUTTERWORTH de orden %i pasa banda fc(%g  -  %g) Hz",
                                TabPage, ButSel[ChannSel],
                                FDS[ChannSel][ButSel[ChannSel]].GetOrdFil(),
                                FDS[ChannSel][ButSel[ChannSel]].GetFcL(),
                                    FDS[ChannSel][ButSel[ChannSel]].GetFcH()  );
                            break;
                        case 3:
                            sprintf( T2T,
                                "Gráfica y procesamiento de canal %i. Módulo seleccionado: %i.     "
                                "Tipo de acción (Filtrado). Tipo: BUTTERWORTH de orden %i rechazo de banda fc(%g  -  %g) Hz",
                                TabPage, ButSel[ChannSel],
                                FDS[ChannSel][ButSel[ChannSel]].GetOrdFil(),
                                FDS[ChannSel][ButSel[ChannSel]].GetFcL(),
                                    FDS[ChannSel][ButSel[ChannSel]].GetFcH()  );
                            break;
                    }
                }else{
                    switch( FDS[ChannSel][ButSel[ChannSel]].GetFilType() ){
                        case 0:
                            sprintf( T2T,
                                "Gráfica y procesamiento de canal %i. Módulo seleccionado: %i.     "
                                "Tipo de acción (Filtrado). Tipo: FIR de orden %i pasa bajo fc(%g) Hz",
                                TabPage, ButSel[ChannSel],
                                FDS[ChannSel][ButSel[ChannSel]].GetOrdFil(),
                                    FDS[ChannSel][ButSel[ChannSel]].GetFcH()  );
                            break;
                        case 1:
                            sprintf( T2T,
                                "Gráfica y procesamiento de canal %i. Módulo seleccionado: %i.     "
                                "Tipo de acción (Filtrado). Tipo: FIR de orden %i pasa alto fc(%g) Hz",
                                TabPage, ButSel[ChannSel],
                                FDS[ChannSel][ButSel[ChannSel]].GetOrdFil(),
                                    FDS[ChannSel][ButSel[ChannSel]].GetFcL()  );
                            break;
                        case 2:
                            sprintf( T2T,
                                "Gráfica y procesamiento de canal %i. Módulo seleccionado: %i.     "
                                "Tipo de acción (Filtrado). Tipo: FIR de orden %i pasa banda fc(%g  -  %g) Hz",
                                TabPage, ButSel[ChannSel],
                                FDS[ChannSel][ButSel[ChannSel]].GetOrdFil(),
                                FDS[ChannSel][ButSel[ChannSel]].GetFcL(),
                                    FDS[ChannSel][ButSel[ChannSel]].GetFcH()  );
                            break;
                        case 3:
                            sprintf( T2T,
                                "Gráfica y procesamiento de canal %i. Módulo seleccionado: %i.     "
                                "Tipo de acción (Filtrado). Tipo: FIR de orden %i rechazo de banda fc(%g  -  %g) Hz",
                                TabPage, ButSel[ChannSel],
                                FDS[ChannSel][ButSel[ChannSel]].GetOrdFil(),
                                FDS[ChannSel][ButSel[ChannSel]].GetFcL(),
                                    FDS[ChannSel][ButSel[ChannSel]].GetFcH()  );
                            break;
                    }
                }
                break;
            case 2:
                if( FDS[ChannSel][ButSel[ChannSel]].HaveWindowAlpha() ){
                    sprintf( T2T,
                            "Gráfica y procesamiento de canal %i. Módulo seleccionado: %i.     "
                            "Tipo de acción (Ventana). Tipo: (%s), Valor para Alpha(%g)",
                            TabPage, ButSel[ChannSel],
                                Wind_2_Fir[FDS[ChannSel][ButSel[ChannSel]].GetTypeWindow()],
                            FDS[ChannSel][ButSel[ChannSel]].GetAlphaWindow() );
                }else{
                    sprintf( T2T,
                            "Gráfica y procesamiento de canal %i. Módulo seleccionado: %i.     "
                            "Tipo de acción (Ventana). Tipo: (%s) ",
                            TabPage, ButSel[ChannSel],
                                Wind_2_Fir[FDS[ChannSel][ButSel[ChannSel]].GetTypeWindow()] );
                }
                break;
            case 3:
                sprintf( T2T,
                        "Gráfica y procesamiento de canal %i. Módulo seleccionado: %i.     "
                        "Tipo de acción ( FFT (Transformada Rápida de Fourier) ).",
                        TabPage, ButSel[ChannSel] );
                break;
            case 4:
                if( FDS[ChannSel][ButSel[ChannSel]].SPecto.WINDWN.HaveWindowAlpha() ){
                    sprintf( T2T,
                            "Gráfica y procesamiento de canal %i. Módulo seleccionado: %i.     "
                            "Tipo de acción (Espectrograma). Tipo de ventana aplicada: (%s), Valor para Alpha(%g)",
                            TabPage, ButSel[ChannSel],
                                Wind_2_Fir[FDS[ChannSel][ButSel[ChannSel]].SPecto.WINDWN.GetWinType()],
                            FDS[ChannSel][ButSel[ChannSel]].SPecto.WINDWN.GetAlpha() );
                }else{
                    sprintf( T2T,
                            "Gráfica y procesamiento de canal %i. Módulo seleccionado: %i.     "
                            "Tipo de acción (Espectrograma). Tipo de ventana aplicada: (%s) ",
                            TabPage, ButSel[ChannSel],
                                Wind_2_Fir[FDS[ChannSel][ButSel[ChannSel]].SPecto.WINDWN.GetWinType()] );
                }
                break;
            case 5: ///  Wigner Ville spectrogram
                if( FDS[ChannSel][ButSel[ChannSel]].SPecto.WINDWN.HaveWindowAlpha() ){
                    sprintf( T2T,
                            "Gráfica y procesamiento de canal %i. Módulo seleccionado: %i.     "
                            "Tipo de acción (WIGNER - VILLE). Tipo de ventana aplicada: (%s), Valor para Alpha(%g)",
                            TabPage, ButSel[ChannSel],
                                Wind_2_Fir[FDS[ChannSel][ButSel[ChannSel]].SPecto.WINDWN.GetWinType()],
                            FDS[ChannSel][ButSel[ChannSel]].SPecto.WINDWN.GetAlpha() );
                }else{
                    sprintf( T2T,
                            "Gráfica y procesamiento de canal %i. Módulo seleccionado: %i.     "
                            "Tipo de acción (WIGNER - VILLE). Tipo de ventana aplicada: (%s) ",
                            TabPage, ButSel[ChannSel],
                                Wind_2_Fir[FDS[ChannSel][ButSel[ChannSel]].SPecto.WINDWN.GetWinType()] );
                }
                break;
            case 6:
                if( FDS[ChannSel][ButSel[ChannSel]].SPecto.WINDWN.HaveWindowAlpha() ){
                    sprintf( T2T,
                            "Gráfica y procesamiento de canal %i. Módulo seleccionado: %i.     "
                            "Tipo de acción (CHOI - WILLIAM). Tipo de ventana aplicada: (%s), Valor para Alpha(%g)",
                            TabPage, ButSel[ChannSel],
                                Wind_2_Fir[FDS[ChannSel][ButSel[ChannSel]].SPecto.WINDWN.GetWinType()],
                            FDS[ChannSel][ButSel[ChannSel]].SPecto.WINDWN.GetAlpha() );
                }else{
                    sprintf( T2T,
                            "Gráfica y procesamiento de canal %i. Módulo seleccionado: %i.     "
                            "Tipo de acción (CHOI - WILLIAM). Tipo de ventana aplicada: (%s) ",
                            TabPage, ButSel[ChannSel],
                                Wind_2_Fir[FDS[ChannSel][ButSel[ChannSel]].SPecto.WINDWN.GetWinType()] );
                }
                break;
            case 7:
                sprintf( T2T,
                        "Gráfica y procesamiento de canal %i. Módulo seleccionado: %i.     "
                        "Tipo de acción ( Eliminar Offset ) Valor(%g).",
                        TabPage, ButSel[ChannSel], FDS[ChannSel][ButSel[ChannSel]].GetCentralValue() );
                break;
        }
        SetWindowTextA( GetDlgItem( HW_DLG_MAIN, CMD_STATUS_BLOW), T2T );
        /// FDS[IndChannT][i].GetCentralValue()
        return TRUE;
    }
    ///
    ///
    ///
    ///
    BOOL Conect( void ){
        if( !OnThrConn ){
            OnThrConn = TRUE;
            DWORD IdTh = 0;
            CreateThread( NULL, 0, Call_ThreadConn, (PVOID)&IPtr, 0, &IdTh );
        }
        return TRUE;
    }
    ///
    ///
    BOOL ISDOWNKEY( int Key ){ return bool(GetKeyState( Key )& 0x80); }
    ///
    ///
    BOOL MsgBoxErrorF( int TypeMsg, const char* szTypes, ... ){
        char* Text2 = NULL;
        DWORD LCIn = strlen( &szTypes[0] );
        Text2 = (char*)malloc( 1024 * LCIn + 16 );
        va_list arguments = NULL;
        va_start( arguments, szTypes );
        vsnprintf( &Text2[0], 1024 * LCIn, &szTypes[0], arguments );
        int VRet = MessageBox( HW_DLG_MAIN, &Text2[0], "", TypeMsg );
        free( &Text2[0] );
        return VRet;
    }
    ///
    ///
    ///
    ///
    BOOL MsgBoxF( LPCSTR szTypes, ... ){
        if( szTypes==NULL ){
            return MessageBox( HW_DLG_MAIN, "", " ", 0 );
        }
        char* Text2 = NULL;
        DWORD LCIn = strlen( &szTypes[0] );
        Text2 = (char*)malloc( 1024 * LCIn + 16 );
        va_list arguments = NULL;
        va_start( arguments, szTypes );
        vsnprintf( &Text2[0], 1024 * LCIn, &szTypes[0], arguments );
        int VRet = MessageBox( HW_DLG_MAIN, &Text2[0], " ", 0 );
        free( &Text2[0] );
        return VRet;
    }
    ///
    ///
    ///
    ///
};
#include "Dlg_Proc_BIG.cpp"
#include "InitDlg.cpp"
#include "CmdProcs.cpp"
#include "ThreadConn.cpp"
#include "All_Threads.cpp"
#include "ThreadPlot.cpp"
#include "ThreadDSPProcs.cpp"
#include "ThreadDSPProcsN.cpp"
#include "DSPProc0.cpp"
#include "SAVEPROCS\SaveProc.cpp"
#include "SAVEPROCS\GetChannelProc.cpp"
///
#include "FDESIGN\FDESIGN.cpp"
///
DLG_TAD     Dlg_ADQ;
///
///
BOOL DSP_TAB_MAIN( VOID* VPTR ){
    return Dlg_ADQ.Show();
}
#include "DlgProc_Proc_1D_TAD.cpp"
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){ return Dlg_ADQ.Dlg_Proc_BIG(  hwndDlg,  uMsg,  wParam,  lParam ); }
BOOL CALLBACK DlgPortsSel(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
    static SERIAL_HANDLE* SerHandPtr = NULL;
    static HWND HWLBOX = NULL;
    switch(uMsg){
        case WM_INITDIALOG:{
            SerHandPtr = (SERIAL_HANDLE*)lParam;
            //SerHandPtr->Avail_Ports();
            HWLBOX = GetDlgItem( hwndDlg, 1234 );
            char T2T[512] = {0};
            for( int i=0; i<SerHandPtr->PRT_INFO[0].Ports_Avaliable; i++ ){
                sprintf( T2T, "%s (%s)", SerHandPtr->PRT_INFO[i].NamePorts, SerHandPtr->PRT_INFO[i].ValuePorts );
                SendMessage( HWLBOX, LB_ADDSTRING, 0, (LPARAM)T2T );
            }
            SendMessage( HWLBOX, LB_SETCURSEL, 0, 0 );
        }
        break;
        case WM_CLOSE:{
            EndDialog(hwndDlg, -1);
        }
        return TRUE;
        case WM_COMMAND:{
            if( LOWORD(wParam)==IDCANCEL ){
                EndDialog(hwndDlg, -1);
                break;
            }
            if( LOWORD(wParam)==IDOK ){
                EndDialog( hwndDlg, SendMessage( HWLBOX, LB_GETCURSEL, 0, 0 ) );
            }
            if( LOWORD(wParam)==1234 && HIWORD(wParam)==LBN_DBLCLK ){
                EndDialog( hwndDlg, SendMessage( HWLBOX, LB_GETCURSEL, 0, 0 ) );
            }
        }
        break;
    }
    return FALSE;
}
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK DlgPortsSelSIMUL(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
    static SERIAL_HANDLE* SerHandPtr = NULL;
    static HWND HWLBOX = NULL;
    switch(uMsg){
        case WM_INITDIALOG:{
            SerHandPtr = (SERIAL_HANDLE*)lParam;
            //SerHandPtr->Avail_Ports();
            SetWindowTextA( hwndDlg, "Seleccione el puerto para simular." );
            HWLBOX = GetDlgItem( hwndDlg, 1234 );
            char T2T[512] = {0};
            for( int i=0; i<SerHandPtr->PRT_INFO[0].Ports_Avaliable; i++ ){
                sprintf( T2T, "%s (%s)", SerHandPtr->PRT_INFO[i].NamePorts, SerHandPtr->PRT_INFO[i].ValuePorts );
                SendMessage( HWLBOX, LB_ADDSTRING, 0, (LPARAM)T2T );
            }
            SendMessage( HWLBOX, LB_SETCURSEL, 0, 0 );
        }
        break;
        case WM_CLOSE:{
            EndDialog(hwndDlg, -1);
        }
        return TRUE;
        case WM_COMMAND:{
            if( LOWORD(wParam)==IDCANCEL ){
                EndDialog(hwndDlg, -1);
                break;
            }
            if( LOWORD(wParam)==IDOK ){
                EndDialog( hwndDlg, SendMessage( HWLBOX, LB_GETCURSEL, 0, 0 ) );
            }
            if( LOWORD(wParam)==1234 && HIWORD(wParam)==LBN_DBLCLK ){
                EndDialog( hwndDlg, SendMessage( HWLBOX, LB_GETCURSEL, 0, 0 ) );
            }
        }
        break;
    }
    return FALSE;
}
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////

/// ////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK DlgProgress(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
    static HWND* HWFathPgr = NULL;
    switch(uMsg){
        case WM_INITDIALOG:{
            HWFathPgr = (HWND*)lParam;
            HWFathPgr[0] = hwndDlg;
            SendMessage( GetDlgItem(hwndDlg,2), PBM_SETRANGE, 0, MAKELPARAM(0, 100) );
        }
        break;
        case WM_CLOSE:{
            HWFathPgr[0] = NULL;
            HWFathPgr = NULL;
            EndDialog(hwndDlg, -1);
        }
        return TRUE;
        case WM_COMMAND:{
        }
        break;
    }
    return FALSE;
}
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "DLG_TAD_Save_FRest.cpp"
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "RECSIMUL.cpp"
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////




