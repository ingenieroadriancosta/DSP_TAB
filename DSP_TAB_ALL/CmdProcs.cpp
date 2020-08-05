BOOL DLG_TAD::CmdProcs( HWND hwnd, UINT  message, WPARAM wParam, LPARAM lParam ){
    switch( LOWORD(wParam) ){
        case CMD_CONN-30:{
            //*/
            MIO_MNG.SaveRestFileTAD( this );
            //CallDSPProcs();
            if( !OnThrConn ){
                /// InDSPProcsThr = TRUE;ThreadDSPProcs( TRUE );
            }
            //*/
            break;
        }
            break;
            ///
            ///
        case CMD_SIMUL:{
            DWORD IdTh = 0;
            CreateThread( NULL, 0, RECSIMUL, NULL, 0, &IdTh );
        }
        break;
            ///
            ///
            ///
            ///
        case CMD_SALIR:{
            SendMessageA( hwnd, WM_CLOSE, 0, 0 );
        }
        break;
        ///
        ///
        case CMD_OPEN_TLAB:{
            MIO_MNG.OpenTLAB();
        }
        break;
            ///SaveProc
            ///
        case CMD_ABOUT:{
            DialogBoxParam( GetModuleHandle(NULL), MAKEINTRESOURCEA(Dial_About), hwnd, DlgProc_Proc_ABOUT, 0 );
            /*
            ZeroMemory( SerHand.SBuff0, 3600 * 2000 * sizeof(short) );
            ZeroMemory( SerHand.SBuff1, 3600 * 2000 * sizeof(short) );
            ZeroMemory( SerHand.SBuff2, 3600 * 2000 * sizeof(short) );
            //*/
        }
        break;
            ///
            ///
        case CMD_SAVE_PROC:{
            if( OnThrConn ){
                MsgBoxF( "\n\n\nDetenga la adquisición para realizar este procedimiento.\n\n\n" );
                break;
            }
            SaveProc();
        }
        break;
            ///
        case CMD_INST_DRIVER:{
            if( OnThrConn ){
                MsgBoxF( "\n\n\nDetenga la adquisición para realizar este procedimiento.\n\n\n" );
                break;
            }
            SetWindowPos( hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW );
            EnableWindow( hwnd, FALSE );
            /// ShowWindow( hwnd, 0 );
            if( !MIO_MNG.InstalDriverUSB2SERIAL() ){
                ShowWindow( hwnd, 1 );
                MsgBoxErrorF( 16, "El driver no se instaló correctamente." );
            }
            ShowWindow( hwnd, 1 );
            EnableWindow( hwnd, TRUE );
            if( OnTop ){
                SetWindowPos( hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW );
            }else{
                SetWindowPos( hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW );
            }
        }
        break;
        ///
        case CMD_ON_TOP:{
            OnTop = !OnTop;
            if( OnTop ){
                SetWindowPos( hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW );
            }else{
                SetWindowPos( hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW );
            }
            CheckMenuItem( HMeW, CMD_ON_TOP, 8*int(OnTop) );
        }
        break;
        ///
        ///
        ///
        ///
        case 2048:
            if( HIWORD( wParam )==EN_CHANGE || HIWORD( wParam )==EN_KILLFOCUS ){
                char T2TT[16] = {0};
                GetWindowTextA( GetDlgItem( hwnd, 2048), T2TT, 8 );
                if( HIWORD( wParam )==EN_KILLFOCUS ){
                    if( T2TT[0]==0 ){
                        sprintf( T2TT, "%i", TimeMax );
                        SetWindowTextA( GetDlgItem( hwnd, 2048), T2TT );
                        SendMessage( GetDlgItem( hwnd, 2048), (UINT)EM_SETSEL, 4, 4 );
                    }
                }else{
                    if( T2TT[0]!=0 ){
                        if( atoi(T2TT)>3600 || T2TT[0]=='0' ){
                            if( atoi(T2TT)>3600 ){
                                TimeMax = 3600;
                            }
                            sprintf( T2TT, "%i", TimeMax );
                            SetWindowTextA( GetDlgItem( hwnd, 2048), T2TT );
                            SendMessage( GetDlgItem( hwnd, 2048), (UINT)EM_SETSEL, 4, 4 );
                        }else{
                            TimeMax = atoi(T2TT);
                        }
                    }
                }
            }
            break;
        ///
        ///
        ///
        ///
        ///
        ///
        case CMD_CONN:{
            if( !OnThrConn ){
                if( Port2Conn<0 ){
                    SerHand.Avail_Ports();
                    if( SerHand.PRT_INFO[0].Ports_Avaliable<1 ){
                        MsgBoxErrorF( 16, "No existen puertos disponibles." );
                        B_CONN.Enable( TRUE );
                        break;
                    }
                    int IndPort = DialogBoxParamA( GetModuleHandle(NULL), MAKEINTRESOURCE(DLG_PORTS), hwnd, (DLGPROC)DlgPortsSel, (LPARAM)&SerHand );
                    if( IndPort>=0 ){
                        Port2Conn = IndPort;
                        //MsgBoxF( "Port2Conn = %i", Port2Conn );
                    }else{
                        B_CONN.Enable( TRUE );
                        break;
                    }
                }
                if( SerHand.Connect( Port2Conn ) ){
                    EnableWindow( GetDlgItem( hwnd, 2048), FALSE );
                    SerHand.StartRec();
                    Conect();
                }else{
                    B_CONN.SetOff();
                    B_CONN.Enable( TRUE );
                    OnThrConn = FALSE;
                    InCmdProc = FALSE;
                }
            }else{
                QuitThrConn();
                //B_CONN.Enable( FALSE );
            }
            break;
            SerHand.SetAny();
            char T2T[256] = {0};
            sprintf( T2T, "Puertos existentes: %i", SerHand.Avail_Ports() );
            MessageBox( hwnd, T2T, "", 0 );
            for( int i=0; i<SerHand.PRT_INFO[0].Ports_Avaliable; i++ ){
                sprintf( T2T,
                        "\nPortAV:\t\t%i\n"
                        "NamePorts:\t%s\n"
                        "ValuePorts:\t%s\n\n"
                        "ValidPort:\t%i\n\n",
                        SerHand.PRT_INFO[i].PortAV,
                        SerHand.PRT_INFO[i].NamePorts,
                        SerHand.PRT_INFO[i].ValuePorts,
                        ValidPort( SerHand.PRT_INFO[i].ValuePorts )
                        );
                MessageBox( hwnd, T2T, "", 0 );
            }
            Sleep( 500 );
            //exit( 0 );
            EndDialog( hwnd, TRUE );
        }
        break;
        case CMD_PORTS:{
            if( OnThrConn ){
                MsgBoxF( "\n\n\nDetenga la adquisición para realizar este procedimiento.\n\n\n" );
                break;
            }
            SerHand.Avail_Ports();
            if( SerHand.PRT_INFO[0].Ports_Avaliable<1 ){
                MsgBoxErrorF( 16, "No existen puertos disponibles." );
                break;
            }
            int IndPort = DialogBoxParamA( GetModuleHandle(NULL), MAKEINTRESOURCE(DLG_PORTS), hwnd, (DLGPROC)DlgPortsSel, (LPARAM)&SerHand );
            if( IndPort>=0 ){
                Port2Conn = IndPort;
                /// MsgBoxF( "Port2Conn = %i", Port2Conn );
                break;
            }
            break;
        }
        ///
        ///
        ///
        case CMD_SAVE_DATAS:
            if( OnThrConn ){
                MsgBoxF( "\n\n\nDetenga la adquisición para realizar este procedimiento.\n\n\n" );
                break;
            }
            static char FNAME[1024] = {0};
            if( OnThrConn || SerHand.NFramesReaded==0 ){
                break;
            }
            if( SaveFileAs( FNAME, "MAT Files (*.mat*)\0*.mat\0", HW_DLG_MAIN ) ){
                DWORD IdTh = 0;
                HWND HWFathPgr = hwnd;
                CreateThread( NULL, 0, Call_Progress, (PVOID)&HWFathPgr, 0, &IdTh );
                Sleep( 100 );
                SetWindowTextA( HWFathPgr, "Guardando datos:" );
                MIO_MNG.SaveMatFile_Channels_Short( FNAME,
                                                    SerHand.SBuff0,
                                                    SerHand.SBuff1,
                                                    SerHand.SBuff2,
                                                    SerHand.NFramesReaded, HWFathPgr );
                /// MsgBoxF( "Datos guardados en\n%s", FNAME );
            }
            break;
        ///
        ///
    }
    if( LOWORD(wParam)>=CMD_DSP_PROCS && LOWORD(wParam)<=(CMD_DSP_PROCS+8) ){
        if( HIWORD(wParam)==BN_DBLCLK || ( HIWORD(wParam)==BN_CLICKED && ISDOWNKEY(VK_CONTROL) ) ){
            int ResP = DialogBoxParam( GetModuleHandle(NULL), MAKEINTRESOURCEA(Dial_Change_Proc_1D_TAD),
                            hwnd, DlgProc_Proc_1D_TAD, (LPARAM)&FDS[ChannSel][ButSel[ChannSel]] );
            if( ResP>=0 ){
                ResP = 127&ResP;
                FDS[ChannSel][ButSel[ChannSel]].SetTYPE_PROC( ResP );
                if( ResP==1 ){
                    FDESIGN( HW_DLG_MAIN, &FDS[ChannSel][ButSel[ChannSel]] );
                }
                if( ResP==2 ){
                    FWINDOWING( HW_DLG_MAIN, &FDS[ChannSel][ButSel[ChannSel]] );
                }
                if( ResP==4 || ResP==5 || ResP==6 ){
                    FSPECTOGRAM( HW_DLG_MAIN, &FDS[ChannSel][ButSel[ChannSel]] );
                }
                ///
                if( ResP==7 ){
                    switch( ChannSel ){
                        case 0:
                            FDS[ChannSel][ButSel[ChannSel]].ShortTemp = SerHand.SBuff0;
                            break;
                        case 1:
                            FDS[ChannSel][ButSel[ChannSel]].ShortTemp = SerHand.SBuff1;
                            break;
                        case 2:
                            FDS[ChannSel][ButSel[ChannSel]].ShortTemp = SerHand.SBuff2;
                            break;
                    }
                    SIGNAL_CENTER( HW_DLG_MAIN, &FDS[ChannSel][ButSel[ChannSel]] );
                }
                ///
                DSP_TYPE_PROC[ChannSel][ButSel[ChannSel]] = 127&ResP;
                SetPicButts();
                if( !OnThrConn && SerHand.NFramesReaded>0 ){
                    DWORD IdTh = 0;
                    HWFathPgr = HW_DLG_MAIN;
                    CreateThread( NULL, 0, Call_Progress, (PVOID)&HWFathPgr, 0, &IdTh );
                    Sleep( 100 );
                    ThreadDSPProcsN( TabPage );
                }
            }
            SetInfoChannel();
            InCmdProc = FALSE;
            return TRUE;
            /// MsgBoxF( "BN_DBLCLK\nButSel = %i \nChannSel = %i", ButSel[ChannSel], ChannSel );
        }
        //DSP_TYPE_PROC[ChannSel][ButSel[ChannSel]]
        /// if( HIWORD(wParam)==BN_CLICKED || HIWORD(wParam)==BN_SETFOCUS ){
        /// if( HIWORD(wParam)==BN_CLICKED ){
        if( HIWORD(wParam)==BN_SETFOCUS ){
            int BSL = LOWORD(wParam) - CMD_DSP_PROCS;
            if( BSL!=ButSel[ChannSel] ){
                ButSel[ChannSel] = LOWORD(wParam) - CMD_DSP_PROCS;
                if( !OnThrConn && SerHand.NFramesReaded>0 ){
                    DWORD IdTh = 0;
                    HWFathPgr = HW_DLG_MAIN;
                    CreateThread( NULL, 0, Call_Progress, (PVOID)&HWFathPgr, 0, &IdTh );
                    Sleep( 100 );
                    FDS[ChannSel][ButSel[ChannSel]].SPecto.Free();
                    ThreadDSPProcsN( TabPage );
                }
                for( int i=0; i<8; i++ ){
                    if( i==ButSel[ChannSel] ){
                        B_DSP_PROCS[i].SetOn( "Seleccionado" );
                    }else{
                        B_DSP_PROCS[i].SetOn( NULL );
                    }
                }
                /// MsgBoxF( "%i", ButSel[0] );
            }
            SetInfoChannel();
            ///
            ///
            ///
            ///
        }
    }
    InCmdProc = FALSE;
    return TRUE;
}

