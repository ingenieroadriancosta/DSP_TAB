BOOL DLG_TAD::Dlg_Proc_BIG( HWND hwnd, UINT  message, WPARAM wParam, LPARAM lParam ){
    switch(message){
        case WM_INITDIALOG:{
            HW_DLG_MAIN = hwnd;
            IWINDOW = (int)HW_DLG_MAIN;
            SerHand.HW_Fath = hwnd;
            return InitDlg( hwnd, message, wParam, lParam );
        }
        return TRUE;
        ///
        ///

        case WM_SIZE:
           SendDlgItemMessage( hwnd, CMD_STATUS_BLOW, WM_SIZE, 0, 0 );
           break;

        ///
        ///
        case WM_CTLCOLORSTATIC:{
            if( GetDlgItem(HW_DLG_MAIN, -124)!=( (HWND)lParam ) ){
                return FALSE;
            }
            #define fndsttc2 250
            static HBRUSH pincel = NULL;
            if( pincel==NULL ){
                pincel = CreateSolidBrush( RGB( fndsttc2, fndsttc2, fndsttc2) );
            }
            HDC hdcStatic = (HDC)wParam; // or obtain the static handle in some other way
            SetTextColor( hdcStatic, RGB( 0, 0, 0)); // text color
            SetBkColor( hdcStatic, RGB( fndsttc2, fndsttc2, fndsttc2 ) );
            /// return (LRESULT)GetStockObject( NULL_BRUSH );
            return (LRESULT)pincel;
        }
        return TRUE;
        ///
        case WM_NOTIFY:{
            LPNMHDR lpnmhdr = (LPNMHDR)lParam;
            if( hwnd==lpnmhdr->hwndFrom ){
                MessageBox( hwnd, "hMainWindow==lpnmhdr.hwndFrom", "Tab Example", MB_OK );
            }
            // MessageBox( hwnd, "Button 100", "Tab Example", MB_OK );
            if( lpnmhdr->idFrom==100 ){
                // MessageBox( hwnd, "lpnmhdr->hwndFrom==100", "Tab Example", MB_OK );
            }
            ///
            if (lpnmhdr->code == TCN_SELCHANGE){
                TabPage = SendMessage( HWTABS, TCM_GETCURSEL, 0, 0 );///TabCtrl_GetCurSel(hTab);
                ///
                ///
                ///
                /// GRAFICAS:
                HWPLOT.SetVisible( TabPage==0 );
                ///
                HWPLOT0.SetVisible( TabPage==1 );
                HWPLOT0_OUT.SetVisible( TabPage==1 );
                ///
                HWPLOT1.SetVisible( TabPage==2 );
                HWPLOT1_OUT.SetVisible( TabPage==2 );
                ///
                HWPLOT2.SetVisible( TabPage==3 );
                HWPLOT2_OUT.SetVisible( TabPage==3 );
                ///
                ///
                for( int i=0; i<8; i++ ){
                    B_DSP_PROCS[i].SetVisible( TabPage!=0 );
                }
                if( TabPage>0 ){
                    ChannSel = TabPage - 1;
                    SetPicButts();
                }
                switch( TabPage ){
                    case 0:
                        SetWindowTextA( GetDlgItem( hwnd, CMD_STATUS_BLOW),
                                       "Gráfica de las tres entradas de la adquisición."
                                       "    (Últimos 10 segundos)." );
                        break;
                    default:
                        SetInfoChannel();
                        break;
                }
                ///
                ///
            }
        }
        break;





        case WM_CLOSE:{
            if( SerHand.IsConn ){
                QuitThrConn();
                B_CONN.Enable( FALSE );
                Sleep( 100 );//EndDialog(hwnd, 0);
                return TRUE;
            }
            B_PRUEBAS.FreeBitmaps();        B_PRUEBAS.DestroyW();
            B_CONN.FreeBitmaps();           B_CONN.DestroyW();
            B_PORTS.FreeBitmaps();          B_PORTS.DestroyW();
            B_SAVE_DATAS.FreeBitmaps();     B_SAVE_DATAS.DestroyW();
            for( int i=0; i<8; i++ ){
                B_DSP_PROCS[i].FreeBitmaps();
                B_DSP_PROCS[i].DestroyW();
            }
            ShowWindow( hwnd, 0 );
            if( SerHand.NFramesReaded>0 ){
                for( int x=0; x<3; x++ ){
                    for( int y=0; y<8; y++ ){
                        FDS[x][y].Free();
                    }
                }
                MIO_MNG.SaveRestFileTAD( this );
            }
            DeleteBitmaps();
            FreeAll();
            SerHand.FreeAll();
            HW_DLG_MAIN = NULL;
            OnWindow = FALSE;
            EndDialog(hwnd, 0);
        }
        return TRUE;
        case WM_COMMAND:{
            if( LOWORD(wParam)<1 ){
                break;
            }
            ///return CmdProcs( hwnd, message, wParam, lParam );
            hwndP = hwnd;
            messageP = message;
            wParamP = wParam;
            lParamP = lParam;
            DWORD IdTh = 0;
            CreateThread( NULL, 0, Call_ThreadCmd, (PVOID)&IPtr, 0, &IdTh );

        }
    }
    return FALSE;
}

