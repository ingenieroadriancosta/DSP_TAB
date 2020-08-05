DWORD WINAPI PLOTMAIN( LPVOID pv=NULL ){
    PLOTEOS* PltT = (PLOTEOS*)pv;
    Sleep( 1 );
    PltT->PLOTEOS_INIT();
    PltT->SetBackG( 255 );
    PltT->SetFatherPOP( NULL );
    PltT->SetGridStyle( 1 );
    PltT->ActGridStyle( TRUE );
    PltT->SetVisible( TRUE );
    PltT->SetLocationCon( 762, 10 );
    PltT->SetSizeCon( 600, 360 );
    PltT->FillGrid( TRUE );
    PltT->SetTitle( " Señales de entrada " );
    PltT->Text2ShowF( " Tiempo(Segundos) " );
    PltT->SetMaxYLim( 65535 );
    PltT->SetMaxYLim( 5 );
    PltT->SetMinYLim( 0 );
    PltT->SetVisible( TRUE );
    PltT->SetLineColor( LineColors[0] );
    MSG messages;
    while( true ){
        GetMessage (&messages, NULL, 0, 0);
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }
    return 0;
}
///
