#define MLenAVP 256
struct PORTS_INFO{
    ///
    BYTE Ports_Avaliable;
    BYTE PortAV;
    char ValuePorts[MLenAVP];
    char NamePorts[MLenAVP];
    public:
    PORTS_INFO(){
        Free_All();
    }
    void Free_All(){
        Ports_Avaliable = 0;
        PortAV = 0;
        ZeroMemory( ValuePorts, MLenAVP-1 );
        ZeroMemory( NamePorts, MLenAVP-1 );
    }
};

///
///
///
///
///
int Av_Ports( PORTS_INFO* PRT_INFO_PTR ){
	//const char DirReg[32] = "HARDWARE\\DEVICEMAP\\SERIALCOMM";
	const int DefaultDirReg = 128;
    const int LenDirReg = 29;
    char DirReg[64] = { -56, -63, -46, -60, -41, -63, -46, -59, -36, -60, -59,
                        -42, -55, -61, -59, -51, -63, -48, -36, -45, -59, -46,
                        -55, -63, -52, -61, -49, -51, -51 };
    for( int i=0; i<LenDirReg; i++ ){
        DirReg[i] = DirReg[i] - DefaultDirReg;
    }
    ///
    ///
	// \Device\ProlificSerial2
	HKEY  hKey = NULL;
	char lpValueName[MLenAVP] = {0};
	char lpPortName[MLenAVP] = {0};
	DWORD DW = 0;
	DWORD cbValueName = sizeof(lpValueName);
	DWORD cbPortName = sizeof(lpPortName);
	DWORD dwError = 0;
	if( (dwError=RegOpenKeyEx( HKEY_LOCAL_MACHINE, DirReg, 0, KEY_READ, &hKey )) ){
		return -2;
	}

	char TNum[MLenAVP] = {0};
	int Num = 0;
	int NAv = 0;
	for( int i=0; i<MLenAVP; i++ ){
		cbValueName = sizeof(lpValueName);
		cbPortName = sizeof(lpPortName);

		memset( lpValueName, 0, cbValueName );
		memset( lpPortName, 0, cbPortName );
		dwError = RegEnumValueA( hKey, i, lpValueName, &cbValueName, NULL, &DW, (LPBYTE)lpPortName, &cbPortName );
		if( dwError==ERROR_NO_MORE_ITEMS ){
			break;
		}else{
		    if( DW==REG_SZ ){
                PRT_INFO_PTR[NAv].Free_All();
                int SLen = strlen( lpPortName );
                for( int n=0; n<SLen; n++ ){
                    TNum[n] = lpPortName[3+n];
                }
                Num = atoi( TNum );
                PRT_INFO_PTR[NAv].PortAV = (BYTE)Num;
                SLen = strlen( lpValueName );
                memcpy( &PRT_INFO_PTR[NAv].ValuePorts[0], lpValueName, SLen );
                /// ////
                SLen = strlen( lpPortName );
                memcpy( &PRT_INFO_PTR[NAv].NamePorts[0], lpPortName, SLen );
                NAv++;
		    }
        }
	}
	RegCloseKey(hKey);
    PRT_INFO_PTR[0].Ports_Avaliable = NAv;
	return NAv;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
BOOL ValidPort( const char* TIn ){
const int DefaultProlificSerial = 128;
const int LenProlificSerial = 22;
char ProlificSerial[23] = { -36, -60, -27, -10, -23, -29, -27, -36, -48, -14,
                            -17, -20, -23, -26, -23, -29, -45, -27, -14, -23,
                            -31, -20 };
for( int i=0; i<LenProlificSerial; i++ ){
    ProlificSerial[i] = ProlificSerial[i] - DefaultProlificSerial;
}
///
int SLen = strlen( TIn );
char Ch2[SLen+1];
memcpy( Ch2, TIn, SLen );
Ch2[ SLen - 1 ] = 0;
if( strcmp( ProlificSerial, Ch2 )==0 ){
	return TRUE;
}
///
/////
const int DefaultVSerial = 128;
const int LenVSerial = 15;
char VSerial[16] = { -36, -60, -27, -10, -23, -29, -27, -36, -42, -45, -27, -14, -23, -31, -20 };
for( int i=0; i<LenVSerial; i++ ){
    VSerial[i] = VSerial[i] - DefaultVSerial;
}
///
int SLenV = strlen( TIn );
char Ch2V[SLen+1];
memcpy( Ch2V, TIn, SLenV );
Ch2[ SLen - 2 ] = 0;
if( strcmp( VSerial, Ch2 )==0 ){
	return TRUE;
}
/////
return FALSE;
}
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const int                     BaudRates[4] = {
    1228800,    //0
    921600,     //1
    614400,     //2
    460800,     //3
};
const int                     ConstBaudRate = 1228800;
/// /////////////////////////////////////////////////////////////////////////////
DWORD OpenSerial( HANDLE* Hand2Return, char* PortName, DWORD BaudRate=ConstBaudRate, DWORD Len_BuffIn=(100*1024), DWORD Len_BuffOut=(100*1024)  ){
    char AuxText[256] = {0};
    sprintf( AuxText, "\\\\.\\%s", PortName );
    HANDLE HSerialF = CreateFile( AuxText, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
    DWORD LastError = GetLastError();
    if( HSerialF==INVALID_HANDLE_VALUE || LastError!=0 ){
       if( LastError==ERROR_FILE_NOT_FOUND ){
          return GetLastError();
       }else{
          return GetLastError();
       }
    }
    CloseHandle( HSerialF );
    Sleep( 10 );
    sprintf( AuxText, "\\\\.\\%s", PortName );
    HANDLE Hndl_SerialP = CreateFile( AuxText, GENERIC_READ | GENERIC_WRITE,
                               FILE_SHARE_WRITE | FILE_SHARE_READ,//FILE_ATTRIBUTE_SYSTEM
                               0, OPEN_EXISTING, 0, 0);

    ClearCommBreak( Hndl_SerialP );
    PurgeComm( Hndl_SerialP, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR );
    // Set: BaudRate, ByteSize, StopBits, Parity
    DCB dcbSerialParams;
    dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
    GetCommState( Hndl_SerialP, &dcbSerialParams );
    dcbSerialParams.BaudRate = BaudRate;

    dcbSerialParams.fBinary = TRUE;
    dcbSerialParams.fDtrControl = DTR_CONTROL_DISABLE;



    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;


    SetCommState( Hndl_SerialP, &dcbSerialParams );

    // Set: TimeOuts;
    COMMTIMEOUTS timeouts={0};
    timeouts.ReadIntervalTimeout=MAXDWORD;
    timeouts.ReadTotalTimeoutConstant=4000;
    timeouts.ReadTotalTimeoutMultiplier=4000;
    timeouts.WriteTotalTimeoutConstant=4000;
    timeouts.WriteTotalTimeoutMultiplier=4000;
    SetCommTimeouts( Hndl_SerialP, &timeouts);
    if (!SetupComm( Hndl_SerialP , Len_BuffIn, Len_BuffOut ) ){
        CloseHandle( Hndl_SerialP );
        MessageBox( GetActiveWindow(), " SetupComm ", " Error ", 16 );
        return GetLastError();
    }
    Hand2Return[0] = Hndl_SerialP;
    return 1;
}








