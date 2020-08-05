#include "VarHeart.cpp"
/// #include "INPUT0.cpp"
DWORD WINAPI RECSIMUL( LPVOID pv ){
    static BOOL OnSimul = FALSE;
    if( OnSimul==TRUE ){
        Dlg_ADQ.MsgBoxF( "\n\n\nYa se encuentra activa la simulación.\n\n\n" );
        return 0;
    }
    OnSimul = TRUE;
    Sleep( 300 );
    SERIAL_HANDLE Ser_TH;
    Ser_TH.HW_Fath = Dlg_ADQ.HW_DLG_MAIN;

    int NPRTAV = Ser_TH.Avail_Ports();
    if( NPRTAV<2 ){
        Dlg_ADQ.MsgBoxErrorF( 16, "No existen puertos disponibles.\n\n\n"
                             "Instale el programa VIRTUAL SERIAL PORT y cree un par virtual.\n\n" );
        OnSimul = FALSE;
        return 0;
    }
    if( Ser_TH.PRT_INFO[0].Ports_Avaliable<1 ){
        Dlg_ADQ.MsgBoxErrorF( 16, "No existen puertos disponibles.\nn\n"
                             "Instale el programa VIRTUAL SERIAL PORT y cree un par virtual.n\n" );
        OnSimul = FALSE;
        return 0;
    }
    int IndPort = DialogBoxParamA( GetModuleHandle(NULL), MAKEINTRESOURCE(DLG_PORTS), Ser_TH.HW_Fath, (DLGPROC)DlgPortsSelSIMUL, (LPARAM)&Ser_TH );
    if( IndPort<0 ){
        /// MsgBoxF( "Port2Conn = %i", Port2Conn );
        OnSimul = FALSE;
        return 0;
    }
    /*
    int PAV = Ser_TH.Avail_Ports();
    if( PAV<2 ){
        return 0;
    }
    //*/
    Ser_TH.Connect_Only( IndPort );
    Sleep( 100 );
    if( Ser_TH.IsConn==FALSE ){
        OnSimul = FALSE;
        return FALSE;
    }
    int Ind = 0;
    double Val2s = 0;
    ///double Val2sAdd = 0;
    double piD = 3.1415926535897932384;
    const double pi2_Fs = (2 * 3.1415926535897932384)/2000;
    BYTE B2S[4*1024] = {0};
    B2S[0] = 123;
    B2S[3] = 0;B2S[4] = 0;
    B2S[5] = 0;B2S[6] = 0;
    B2S[7] = 125;
    int VINT = 0;
    int FrqCnt = 0;
    int CNHRT = 0;
    const double FrqSimul_Init = FrqSimul;
    double FrqSimul0 = FrqSimul_Init;
    const int DBLS = 16;
    double fplus = 0;
    while( true ){
        if( Dlg_ADQ.SerHand.IsConn ){
            for( int i=0; i<DBLS*16; i++ ){
                Ind = (Ind + 1);
                ///
                Val2s = 0.9 * sin( FrqSimul0 * FrqSimul0 * pi2_Fs * Ind ) + 0.1 * sin( 60*pi2_Fs * Ind);
                ///
                Val2s = 1023 * ( Val2s + 1 )/2;
                ///
                ///
                FrqSimul0 = FrqSimul0 + 0.00005;
                ///
                ///
                ///
                ///Val2sAdd = 1023 *(( sin( 800*(8191&FrqCnt)/8192.0 *(2 * piD * Ind)/65536 ) + 1 )/2);
                ///Val2s = (Val2s + Val2sAdd)/2;
                VINT = (int)( Val2s );
                ///
                FrqCnt++;
                ///
                B2S[8*i  ] = 123;
                B2S[8*i+1] = VINT;
                B2S[8*i+2] = VINT/256;
                ///
                ///
                Val2s = 1023 *(( sin( 0.02*FrqSimul*(2 * piD * Ind)/65536 ) + 1 )/2) +
                            1023 *(( sin( 0.02*8*FrqSimul*(2 * piD * Ind)/65536 ) + 1 )/2) +
                            1023 *(( sin( 0.02*16*FrqSimul*(2 * piD * Ind)/65536 ) + 1 )/2) +
                            1023 *(( sin( 0.02*(900000)*FrqSimul*(2 * piD * Ind)/65536 ) + 1 )/2);
                ///
                ///
                /*
                Val2s = 0.1 * sin( fplus * (2 * piD * Ind)/2000 )
                            + 0.9 * sin( 0.0001 * (2 * piD * Ind)/2000 );
                Val2s = ( Val2s + 1 )/2;
                //*/
                ///
                Val2s = 0.1*sin( 400*(2 * piD * Ind)/2000 ) +
                            0.2*sin( 450*(2 * piD * Ind)/2000 ) +
                                0.7 * sin( 0.1*(2 * piD * Ind)/2000 );
                Val2s = ( Val2s + 1 )/2;
                ///
                Val2s = round( 1023 * Val2s );
                fplus = fplus + 0.001;
                if( fplus>=1 ){
                    fplus = 0;
                }
                ///
                ///
                ///
                ///
                VINT = (int)( Val2s );
                B2S[8*i+3] = 255&VINT;
                B2S[8*i+4] = VINT/256;
                FrqSimul = FrqSimul + 0.005;
                ///
                ///
                Val2s = 1023 *(( sin( FrqSimul*(2 * piD * Ind)/65536 ) + 1 )/2) +
                            1023 *(( sin( 2*FrqSimul*(2 * piD * Ind)/65536 ) + 1 )/2) +
                            1023 *(( sin( 4*FrqSimul*(2 * piD * Ind)/65536 ) + 1 )/2);
                VINT = (int)( Val2s/3 );
                ///
                ///
                ///
                ///
                ///
                VINT = (int)Heart[1023&CNHRT];
                ///
                ///
                ///
                Val2s = ( sin( 350 * (2 * piD * Ind)/2000 ) + 1 )/2;
                VINT = (int)round( 1023 * Val2s );//    ESTE!!!!
                ///
                ///
                ///
                CNHRT = 1023&(CNHRT + 1);
                B2S[8*i+5] = VINT;
                B2S[8*i+6] = VINT/256;
                ///
                B2S[8*i+7] = 125;
                ///
                ///
                ///Ser_TH.WriteBuff( B2S, 8 );
                ///memcpy( &Ser_TH.SBuff0[8*i], B2S, 8 );
            }
            Ser_TH.WriteBuff( B2S, DBLS*8*16 );
            ///Ser_TH.WriteBuff( (BYTE*)&Ser_TH.SBuff0[0], 8*16 );
        }else{
            Ind = 0;
            FrqCnt = 0;
            FrqSimul = FrqSimul_Init;
            FrqSimul0 = FrqSimul_Init;
            fplus = 0;
        }
        Sleep(TimeMillis);
    }
    OnSimul = FALSE;
    return 0;
}

