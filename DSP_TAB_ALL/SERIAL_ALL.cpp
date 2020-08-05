#include "Av_Ports.cpp"
DWORD WINAPI Thread_ReadBuff( LPVOID );

#define MaxFRM          7200000 ///7340032
struct SERIAL_HANDLE{
    int                                 IPtr;
    HWND                                HW_Fath;
    HANDLE                              PSER_HAND;
    BOOL                                IsConn;
    BOOL                                OnThr;
    BOOL                                IsRec;
    BOOL                                SetCloseV;
    int                                 IndSerial;
    int                                 NFramesReaded;
    int                                 BRE;
    DWORD                               dErrorFlags;
    PORTS_INFO                          PRT_INFO[MLenAVP];
    short*                              SBuff0;
    short*                              SBuff1;
    short*                              SBuff2;
    unsigned char*                      UCBTemp;
    ~SERIAL_HANDLE(){
        FreeAll();
    }
    SERIAL_HANDLE(){
        IPtr                = 0;
        HW_Fath             = NULL;
        PSER_HAND           = NULL;
        IsConn              = FALSE;
        OnThr               = FALSE;
        IsRec               = FALSE;
        SetCloseV           = FALSE;
        IndSerial           = 0;
        NFramesReaded       = 0;
        BRE                 = 0;
        dErrorFlags = 0;
        Reset();
        MSET0();
    }
    ///
    ///
    BOOL FreeAll(){
        if( SBuff0!=NULL ){
            free( SBuff0 );
            free( SBuff1 );
            free( SBuff2 );
            free( UCBTemp );
            SBuff0 = NULL;
            SBuff1 = NULL;
            SBuff2 = NULL;
            UCBTemp = NULL;
        }
        return TRUE;
    }
    ///
    ///
    ///
    ///
    BOOL Reset(){
        FreeAll();
        SBuff0 = (short*)malloc( MaxFRM * sizeof(short) );
        SBuff1 = (short*)malloc( MaxFRM * sizeof(short) );
        SBuff2 = (short*)malloc( MaxFRM * sizeof(short) );
        UCBTemp = (unsigned char*)malloc( 4 * 1024 * 1024 );
        ZeroMemory( SBuff0,  MaxFRM * sizeof(short) );
        ZeroMemory( SBuff1,  MaxFRM * sizeof(short) );
        ZeroMemory( SBuff2,  MaxFRM * sizeof(short) );
        ZeroMemory( UCBTemp, 1024 * 1024 );
        return TRUE;
    }
    ///
    ///
    BOOL MSET0(){
        NFramesReaded = 0;
        for( int i=0; i<(4 * 1024 * 1024); i++ ){
            UCBTemp[i] = 0;
        }
        for( int i=0; i<MaxFRM; i++ ){
            SBuff0[i] = 0;
            SBuff1[i] = 0;
            SBuff2[i] = 0;
        }
        return TRUE;
    }
    ///
    ///
    BOOL SetClose(){
        SetCloseV = TRUE;
        return TRUE;
    }
    ///
    ///
    BOOL SetAny(){
        if( HW_Fath==NULL || !IsWindow(HW_Fath) ){
            HW_Fath = GetActiveWindow();
        }else{
            MessageBoxA( HW_Fath, "", "", 0 );
        }
        return TRUE;
    }
    ///
    ///
    int Avail_Ports(){
        return Av_Ports(PRT_INFO);
    }
    ///
    ///
    BOOL MsgError( DWORD LErr ){
        if( LErr!=0 ){
            LPSTR messageBuffer = NULL;
            FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                         NULL, LErr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
            MessageBoxA( HW_Fath, messageBuffer, " Error ", MB_ICONERROR | MB_OK );
        }
        return (LErr!=0);
    }
    ///
    ///
    BOOL Close(){
        if( IsRec ){
            SetCloseV = FALSE;
            StopRec();
        }
        ///
        if( PSER_HAND!=NULL ){
            CloseHandle( PSER_HAND );
            PSER_HAND = NULL;
        }
        IsConn = FALSE;
        Sleep( 10 );
        // MessageBoxA( HW_Fath, "", "", 0 );
        return TRUE;
    }
    ///
    ///
    BOOL Connect_Only( int Ind_Serial ){
        Close();
        int LErr = OpenSerial( &PSER_HAND, PRT_INFO[Ind_Serial].NamePorts );
        if( LErr==1 ){
            NFramesReaded = 0;
            IsConn = TRUE;
            IndSerial = Ind_Serial;
            return TRUE;
        }
        return FALSE;
    }
    ///
    ///
    BOOL Connect( int Ind_Serial ){
        Close();
        int LErr = OpenSerial( &PSER_HAND, PRT_INFO[Ind_Serial].NamePorts );
        if( LErr==1 ){
            IndSerial = Ind_Serial;
            if( !IsDevice() ){
                if( MessageBox( HW_Fath,
                            "La conexión se ha establecido pero "
                            "el dispositivo no es reconocido,\n"
                            "Desea continuar?",
                            " ", MB_YESNO )==IDNO ){
                    Close();
                    return FALSE;
                }
            }
            NFramesReaded = 0;
            IsConn = TRUE;
            MSET0();
            /// DWORD IdTh = 0;CreateThread( NULL, 0, Thread_ReadBuff, (PVOID)&IPtr, 0, &IdTh );
            Sleep( 1 );
        }else{
            MsgError( LErr );
            Close();
            return FALSE;
        }
        return TRUE;
    }
    ///
    ///
    BOOL WriteBuff( BYTE* BVal, int LBuff ){
        DWORD LW = 0;
        return ( WriteFile( PSER_HAND, &BVal[0], LBuff, &LW, NULL ) && ((int)LW==LBuff) );
    }
    ///
    ///
    BOOL WriteByte( BYTE BVal ){
        BYTE STREC[2] = {BVal, 0};
        DWORD LW = 0;
        return ( WriteFile( PSER_HAND, &STREC[0], 1, &LW, NULL ) && (LW==1) );
    }
    ///
    ///
    BOOL IsDevice(){
        char STREC[16] = {0};
        DWORD LW = 0;
        STREC[0] = 123;
        STREC[1] = 1;
        STREC[2] = 0;
        ///
        STREC[7] = 125;
        for( int i=0; i<8; i++ ){
            if( !WriteFile( PSER_HAND, &STREC[i], 1, &LW, NULL ) ){
                MsgError( GetLastError() );
                Close();
                break;
            }
            Sleep( 1 );
        }
        Sleep( 50 );
        if( GetBytesAvailable()==8 ){
            ReadFile( PSER_HAND, &STREC[0], 8, &LW, NULL );
            char* TCPTR = (char*)&STREC[0];
            return (strcmp( TCPTR, "TAD_OK" )==0);
        }
        return FALSE;
    }
    ///
    ///
    BOOL StartRec(){
        if( IsConn ){
            char STREC[16] = {0};
            DWORD LW = 0;
            STREC[0] = 123;
            STREC[1] = 0;
            STREC[2] = 1;
            ///
            STREC[7] = 125;
            IsRec = FALSE;
            for( int i=0; i<8; i++ ){
                if( !WriteFile( PSER_HAND, &STREC[i], 1, &LW, NULL ) ){
                    MsgError( GetLastError() );
                    Close();
                    break;
                }
                Sleep( 10 );
            }
            IsRec = TRUE;
            Sleep( 10 );
        }
        return TRUE;
    }
    ///
    ///
    BOOL StopRec(){
        if( IsConn ){
            char STREC[16] = {0};
            DWORD LW = 0;
            STREC[0] = 123;
            STREC[1] = 0;
            STREC[2] = 0;
            ///
            STREC[7] = 125;
            IsRec = FALSE;
            for( int i=0; i<8; i++ ){
                if( !WriteFile( PSER_HAND, &STREC[i], 1, &LW, NULL ) ){
                    MsgError( GetLastError() );
                    Close();
                    break;
                }
                Sleep( 10 );
            }
            IsRec = FALSE;
            if( NFramesReaded<2000 ){
                NFramesReaded = 2000;
            }
            NFramesReaded = NFramesReaded - (NFramesReaded%2000);
            Sleep( 10 );
        }
        return TRUE;
    }
    ///
    ///
    char* GetErrorText( DWORD LError ){
        static char* Text2Error = NULL;
    if( Text2Error!=NULL ){free( Text2Error );}
    Text2Error = (char*)malloc( 256 );
    switch( LError ){
        case 0:
             sprintf( Text2Error, "Sin error" );
             break;
        case CE_BREAK:
             sprintf( Text2Error, "The hardware detected a break condition.(CE_BREAK)" );
             break;
        case CE_FRAME:
             sprintf( Text2Error, "The hardware detected a framing error. (CE_FRAME)" );
             break;
        case CE_OVERRUN:
             sprintf( Text2Error, "A character-buffer overrun has occurred. The next character is lost. (CE_OVERRUN)" );
             break;
        case CE_RXOVER:
             sprintf( Text2Error, "Error Serial, CE_RXOVER" );
             break;
        case CE_RXPARITY:
             sprintf( Text2Error, "Error Serial, CE_RXPARITY"  );
             break;
        case CE_TXFULL:
             sprintf( Text2Error, "Error Serial, CE_TXFULL"  );
             break;
        default:
             sprintf( Text2Error, "Error Serial, Desconocido, %u", (unsigned int)LError  );
             break;
    }
    return Text2Error;
}
    ///
    ///
    int GetBytesAvailable( void ){
        COMSTAT                       ComStat = {0};
        ZeroMemory( &ComStat, sizeof(COMSTAT) );
        /// ClearCommBreak( PSER_HAND );
        ClearCommError( PSER_HAND, &dErrorFlags , &ComStat );
        if( dErrorFlags!=0 ){
            StopRec();
            Close();
            MessageBoxA( HW_Fath, GetErrorText(dErrorFlags), "", 0 );
            return -2;
        }
        return ComStat.cbInQue;
    }
    ///
    ///
    ///
    int ReadBytesAvailable(){
        /// UCBTemp
        int RBA = GetBytesAvailable();
        if( RBA>=16 ){
            RBA = RBA - RBA%8;
            DWORD LW = 0;
            if( !ReadFile( PSER_HAND, UCBTemp, RBA, &LW, NULL ) ){
                Close();
                MsgError( GetLastError() );
                return -2;
            }
            if( ((DWORD)RBA)!=LW ){
                ///MsgError( GetLastError() );
                ///Close();
                MsgError( GetLastError() );
                return -2;
            }
            short* SPTR = NULL;
            double DVal = 0;
            for( int i=0; i<RBA; i = i + 8 ){
                if( NFramesReaded>MaxFRM ){
                    RBA = -12345;
                    break;
                }
                ///
                if( UCBTemp[i]!=123 || UCBTemp[i+7]!=125 ){
                    MsgError( GetLastError() );
                    return -3;
                }
                ///
                //asdf();
                // SPTR = (short*)&UCBTemp[i+1];SBuff0[NFramesReaded] = SPTR[0];
                /// SBuff0[NFramesReaded] = (short)(MAKEWORD( UCBTemp[i+1], UCBTemp[i+2] ));
                SPTR = (short*)&UCBTemp[i+1];
                SBuff0[NFramesReaded] = SPTR[0];
                DVal = ( 32767.00 * (double)SBuff0[NFramesReaded] )/1023.00;
                SBuff0[NFramesReaded] = (short)DVal;
                ///
                SPTR = (short*)&UCBTemp[i+3];
                SBuff1[NFramesReaded] = SPTR[0];
                DVal = ( 32767.00 * (double)SBuff1[NFramesReaded] )/1023.00;
                SBuff1[NFramesReaded] = (short)DVal;
                ///
                SPTR = (short*)&UCBTemp[i+5];
                SBuff2[NFramesReaded] = SPTR[0];
                DVal = ( 32767.00 * (double)SBuff2[NFramesReaded] )/1023.00;
                SBuff2[NFramesReaded] = (short)DVal;
                ///
                NFramesReaded++;
            }
        }
        ///
        return RBA;
    }
    ///
    ///
};
/// ////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/*
DWORD WINAPI Thread_ReadBuff( LPVOID pv=NULL ){
    SERIAL_HANDLE* SERH = (SERIAL_HANDLE*)pv;
    SERH[0].OnThr = TRUE;
    unsigned char* UCBuff = (unsigned char*)malloc( 10 * 1024 * 1024 );
    Sleep( 1 );
    int BAva = 0;
    while( SERH[0].IsConn==TRUE ){
        // SERH[0].BytesAvail();
        BAva = SERH[0].GetBytesAvailable();
        if( BAva==-1 ){
            break;
        }
        sprintf( (char*)UCBuff, "%i", BAva );
        SetWindowText( SERH[0].HW_Fath, (char*)UCBuff );
        Sleep( 10 );
    }
    free( UCBuff );
    SERH[0].OnThr = FALSE;
    if( BAva==-1 ){
        SERH[0].Close();
    }
    /// MessageBoxA( SERH[0].HW_Fath, "SERH[0].OnThr = FALSE;", "", 0 );
    Sleep( 10 );
    return 0;
}
/// ////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////
//*/
