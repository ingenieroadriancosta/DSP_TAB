const int                     BaudRates[4] = {
1228800,//0
921600, //1
614400, //2
460800, //3
};
const int                     ConstBaudRate = 1228800;
/*
1228800
921600
614400
460800
230400
115200
57600
38400
28800
19200
14400
9600
7200
4800
3600
2400
1800
1200
600
300
150
75
//*/





BYTE IsPortAvailable( char*, BOOL );
char                          TextDevice[128];
class TagSerial{
private:
HANDLE                        Hndl_SerialP;
DWORD                         BaudRateP;
DWORD                         LBufferInP;
DWORD                         LBufferOutP;

char                          PortNameP[256];
char                          AuxText[1024];
BOOL                          IsOpen;
DWORD                         dErrorFlags;
COMSTAT                       ComStat;
DWORD                         dwBytesWrite;
DWORD                         dwBytesRead;
BYTE                          ByteArray[16];
DWORD                         LError;
char*                         Text2Error;

public:
TagSerial(){
memset( TextDevice, 0, 128 );
memset( PortNameP, 0, 256 );
memset( AuxText, 0, 1024 );
memset( &ComStat, 0, sizeof(ComStat) );
memset( ByteArray, 0, 16 );
IsOpen = FALSE;
LError = 0;
dErrorFlags = 0;
dwBytesWrite = 0;
dwBytesRead = 0;
Hndl_SerialP = NULL;
BaudRateP = ConstBaudRate;
LBufferInP = 10*(1024*1024);
LBufferOutP = 10*(1024*1024);
Text2Error = NULL;
}
////////////////////////////////////////////////////////////////////////////////
BOOL IsPortOpen( void ){return IsOpen;}
////////////////////////////////////////////////////////////////////////////////
BYTE OpenSerial( char* PortName, DWORD BaudRate=ConstBaudRate, DWORD Len_BuffIn=(100*1024), DWORD Len_BuffOut=(100*1024)  ){
    IF IsOpen THEN
       //msgbox( "Cierre la conexión actual antes de establecer una nueva.", "Error", 16 );
       return 128;
    ENDIF;
    sprintf( AuxText, "\\\\.\\%s", PortName );
    HANDLE HSerialF = CreateFile( AuxText, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
    DWORD LastError = GetLastError();
    IF HSerialF==INVALID_HANDLE_VALUE || LastError!=0 THEN
       IF LastError==ERROR_FILE_NOT_FOUND THEN
          return 0;
       ELSE
          return 255;
       ENDIF;
    ENDIF;
    CloseHandle( HSerialF );
    Sleep( 10 );
    // FILE_SHARE_READ

    sprintf( AuxText, "\\\\.\\%s", PortName );
    sprintf( PortNameP, "%s", PortName );
    BaudRateP = BaudRate;
    LBufferInP = Len_BuffIn;
    LBufferOutP = Len_BuffOut;
    Hndl_SerialP = CreateFile( AuxText, GENERIC_READ | GENERIC_WRITE,
                               FILE_SHARE_WRITE | FILE_SHARE_READ,//FILE_ATTRIBUTE_SYSTEM
                               0, OPEN_EXISTING, 0, 0);

    ClearCommBreak( Hndl_SerialP );
    PurgeComm( Hndl_SerialP, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
    // Set: BaudRate, ByteSize, StopBits, Parity
    DCB dcbSerialParams;
    dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
    GetCommState( Hndl_SerialP, &dcbSerialParams );
    dcbSerialParams.BaudRate = BaudRateP;

    dcbSerialParams.fBinary = TRUE;
    dcbSerialParams.fDtrControl = DTR_CONTROL_DISABLE;



    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;


    SetCommState( Hndl_SerialP, &dcbSerialParams );

    // Set: TimeOuts;
    COMMTIMEOUTS timeouts={0};
    timeouts.ReadIntervalTimeout=MAXDWORD;
    timeouts.ReadTotalTimeoutConstant=1000;
    timeouts.ReadTotalTimeoutMultiplier=1000;
    timeouts.WriteTotalTimeoutConstant=1000;
    timeouts.WriteTotalTimeoutMultiplier=1000;
    SetCommTimeouts( Hndl_SerialP, &timeouts);


    // Set Len Buffers.
    //
    //LBufferInP = 10*MBs;
    //LBufferOutP = 10*MBs;
    if (!SetupComm( Hndl_SerialP , LBufferInP, LBufferOutP ) ){
       MessageBox( NULL, " SetupComm ", " Error ", 16 );
    }


    //SetupComm( Hndl_SerialP , LBufferInP, LBufferOutP );

    IsOpen = true;

    return 1;

}

////////////////////////////////////////////////////////////////////////////////
BOOL Close( void ){
    CloseHandle( Hndl_SerialP );
    Hndl_SerialP = NULL;
    IsOpen = FALSE;
    return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
DWORD ClearError( void ){
    LError = 0;
    return LError;
}
////////////////////////////////////////////////////////////////////////////////
DWORD GetError( void ){
    return LError;
}
////////////////////////////////////////////////////////////////////////////////
char* GetErrorText( void ){
    if( Text2Error!=NULL ){free( Text2Error );}
    Text2Error = (char*)malloc( 256 );
    switch( LError ){
        case 0:
             sprintf( Text2Error, "Sin error" );
             break;
        case CE_FRAME:
             sprintf( Text2Error, "Error Serial, CE_FRAME" );
             break;
        case CE_OVERRUN:
             sprintf( Text2Error, "Error Serial, CE_OVERRUN" );
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
////////////////////////////////////////////////////////////////////////////////
DWORD GetBytesAvailable( void ){

    ClearCommError( Hndl_SerialP, &dErrorFlags , &ComStat );
    IF dErrorFlags!=0 THEN
       LError = dErrorFlags;
    ENDIF;
    IF dErrorFlags==CE_FRAME THEN
       //msgbox( "dErrorFlags==CE_FRAME", "", 16 );
       // MessageBeep( 16 );
       printf( "Error Serial, CE_FRAME\t\t\t\t\t\t\n" );
    ENDIF;

    IF dErrorFlags==CE_OVERRUN THEN
       //msgbox( "dErrorFlags==CE_OVERRUN", "", 16 );
       // MessageBeep( 16 );
       printf( "Error Serial, CE_OVERRUN\t\t\t\t\n" );
    ENDIF;

    IF dErrorFlags==CE_RXOVER THEN
       //msgbox( "dErrorFlags==CE_RXOVER", "", 16 );
       // MessageBeep( 16 );
       printf( "Error Serial, CE_RXOVER\t\t\t\t\n" );
    ENDIF;

    IF dErrorFlags==CE_RXPARITY THEN
       //msgbox( "dErrorFlags==CE_RXPARITY", "", 16 );
       // MessageBeep( 16 );
       printf( "Error Serial, CE_RXPARITY\t\t\t\n" );
    ENDIF;

    IF dErrorFlags==CE_TXFULL THEN
       //msgbox( "dErrorFlags==CE_TXFULL", "", 16 );
       // MessageBeep( 16 );
       printf( "Error Serial, CE_TXFULL\t\t\t\n" );
    ENDIF;

    return ComStat.cbInQue;
}
////////////////////////////////////////////////////////////////////////////////
BOOL SendByte( BYTE Byte2send )
{
IF !IsOpen THEN
   printf( "El puerto serial está cerrado."  );
   return FALSE;
ENDIF;
ByteArray[0] = Byte2send;
WriteFile( Hndl_SerialP, &ByteArray[0], 1, &dwBytesWrite, NULL );
if( dwBytesWrite!=1  ){
    printf( "Error en la función:\nbool SendByte( BYTE Byte2send )\n\n"  );
    return FALSE;
}
return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
BYTE GetByte( void )
{
IF !IsOpen THEN
   printf( "El puerto serial está cerrado."  );
   return FALSE;
ENDIF;
ByteArray[0] = 0;
ReadFile( Hndl_SerialP, &ByteArray[0], 1, &dwBytesRead, NULL );
if( dwBytesRead!=1  ){
    printf( "Error en la función:\nBYTE GetByte( void )\n\n"  );
    return 0;
}
return ByteArray[0];
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DWORD SendBuffer( BYTE* BBuffer, DWORD NBytes=16 ){
IF !IsOpen THEN
   printf( "El puerto serial está cerrado."  );
   return FALSE;
ENDIF;
WriteFile( Hndl_SerialP, &BBuffer[0], NBytes, &dwBytesWrite, NULL );
if( dwBytesWrite!=NBytes  ){
    printf( "Error en la función:\nDWORD SendBuffer( BYTE* BBuffer, DWORD NBytes=16 )\n\n"  );
    return 0;
}
return dwBytesWrite;
}
////////////////////////////////////////////////////////////////////////////////
DWORD Read2Buffer( BYTE* BBuffer, DWORD NBytes=16 ){
IF !IsOpen THEN
   printf( "El puerto serial está cerrado."  );
   return FALSE;
ENDIF;
ReadFile( Hndl_SerialP, &BBuffer[0], NBytes, &dwBytesRead, NULL );
if( dwBytesRead!=NBytes  ){
    printf( "Error en la función:\nDWORD Read2Buffer( BYTE* BBuffer, DWORD NBytes=16 )\n\n"  );
    return 0;
}
return dwBytesRead;
}


HANDLE GetHandleS( void ){ return Hndl_SerialP; }


};
TagSerial  HSerial;
TagSerial  HSerial_Temp;















////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
char                              DevTad[16][128];
char                              DevTadNew[128];
char                              DevPass[128];
WORD                              DevDivisor = 255;
BYTE IsPortAvailable( char* TPort, BOOL Check=TRUE ){
DevPass[0] = 0;
char TextoTemp[256];

/*
CreateFile(L"COM1", ...);
CreateFile(L"\\\\.\\COM1", ...);
CreateFile(L"\\\\.\\Global\COM1", ...);
//*/


sprintf( TextoTemp, "\\\\.\\Global\\%s", TPort );
sprintf( TextoTemp, "\\\\.\\%s", TPort );
//DeleteFileA( TextoTemp );
HANDLE HSerialFAV = NULL;
DWORD LastError = 0;
// HSerialFAV = CreateFile( TextoTemp, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
//HSerialFAV = CreateFile( TextoTemp, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0 );

HSerialFAV = CreateFile( TextoTemp, GENERIC_READ | GENERIC_WRITE,
                               FILE_SHARE_WRITE | FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0 );

LastError = GetLastError();




IF HSerialFAV==INVALID_HANDLE_VALUE || LastError!=0 THEN
   IF LastError==ERROR_FILE_NOT_FOUND THEN
      CloseHandle( HSerialFAV );
      return 0;
   ELSE
      CloseHandle( HSerialFAV );
      return 255;
   ENDIF;
ELSE
   PurgeComm( HSerialFAV, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR );
   IF !Check THEN
      CloseHandle( HSerialFAV );
      return 2;
   ENDIF;
   DCB dcbSerialParams;
   dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
   GetCommState( HSerialFAV, &dcbSerialParams );
   dcbSerialParams.BaudRate = ConstBaudRate;
   dcbSerialParams.fBinary = TRUE;
   dcbSerialParams.fDtrControl = DTR_CONTROL_DISABLE;
   dcbSerialParams.ByteSize = 8;
   dcbSerialParams.StopBits = ONESTOPBIT;
   dcbSerialParams.Parity = NOPARITY;
   SetCommState( HSerialFAV, &dcbSerialParams );

   // Set: TimeOuts;
   COMMTIMEOUTS timeouts={0};
   timeouts.ReadIntervalTimeout=MAXDWORD;
   timeouts.ReadTotalTimeoutConstant=1000;
   timeouts.ReadTotalTimeoutMultiplier=1000;
   timeouts.WriteTotalTimeoutConstant=1000;
   timeouts.WriteTotalTimeoutMultiplier=1000;
   SetCommTimeouts( HSerialFAV, &timeouts );
   if (!SetupComm( HSerialFAV , 1024, 1024 ) ){
      MessageBox( NULL, " IsPortAvailable SetupComm ", " Error ", 16 );
      exit( 0 );
   }


   //msgbox();
   //msgbox( "CloseHandle( HSerialF );" );
   Sleep( 1 );
   BYTE ComBuff[MaxNBTx+16];
   ComBuff[0] = '{';
   ComBuff[1] = 128;
   ComBuff[MaxNBTx-1] = '}';

   FOR int i=0; i<MaxNBTx; i++ LOOP
       WriteFile( HSerialFAV, &ComBuff[i], 1, &LastError, NULL );
       Sleep( 1 );
   ENDLOOP;
   ///msgbox();
   int BAvailable = 0;
   Sleep( 100 );
   COMSTAT                       ComStatFAV;
   DWORD dErrorFlagsFAV = 0;
   ClearCommError( HSerialFAV, &dErrorFlagsFAV , &ComStatFAV );
   BAvailable = ComStatFAV.cbInQue;
   IF BAvailable>=1 THEN
      // char CMsg[256];
      Sleep( 100 );
      //HSerial_Temp.Read2Buffer( (BYTE*)&DevPass[0], 16 );
      //HSerial_Temp.Close();
      memset( &DevPass[0], 0, 32 );
      ReadFile( HSerialFAV, &DevPass[0], 31&BAvailable, &LastError, NULL );
      // msgbox( DevPass );
      CloseHandle( HSerialFAV );
      FOR int i=0; i<8; i++ LOOP
          //msgbox( &DevTad[i][0] );
          IF strcmp( &DevTad[i][0], DevPass )==0 THEN
             DevDivisor = 255;
             return 1;
          ENDIF;
      ENDLOOP;
      ///
      /*
      IF strcmp( DevTadNew, DevPass )==0 THEN
         DevDivisor = 255;
         return 1;
      ENDIF;
      //*/
      return 2;
   ELSE
      CloseHandle( HSerialFAV );
      return 2;
   ENDIF;
ENDIF;
return 0;
}
////////////////////////////////////////////////////////////////////////////////
//*/
