struct PlotCtrl{
    HWND                            HWCTRL;
    HWND                            HWTOOLT;
    HBITMAP                         HBMPLOT;
    int                             W_BUT;
    int                             H_BUT;
    RGBQUAD*                        RgbQ;
    RGBQUAD*                        RgbGrid;
    ///
    ~PlotCtrl(){
        if( RgbQ!=NULL ){
            free( RgbQ );
        }
        if( RgbGrid!=NULL ){
            free( RgbGrid );
        }
        RgbQ = NULL;
        RgbGrid = NULL;
    }
    PlotCtrl(){
        HWCTRL      = NULL;
        HWTOOLT     = NULL;
        RgbQ        = NULL;
        RgbGrid     = NULL;
    }
    ///
    BOOL Create( HWND HWFath, int XPos, int YPos, int WWW, int HHH, int HMenu ){
        HWCTRL = CreateWindowEx(    0, "STATIC", NULL,
                                    WS_CHILD | WS_VISIBLE | (0*WS_BORDER) | SS_BITMAP,
                     XPos, YPos, WWW, HHH, HWFath, (HMENU)(HMenu),
                     GetModuleHandle(NULL), NULL );
        W_BUT = WWW;
        H_BUT = HHH;
        CreateImage();
        ResetGrid();
        SetImage();
        return TRUE;
    }
    ///
    ///
    BOOL ResetGrid(){
        if( RgbGrid!=NULL ){
            free( RgbGrid );
        }
        RgbGrid = (RGBQUAD*)malloc( 4 * W_BUT * H_BUT + 1024 );
        memset( RgbGrid, 255, 4 * W_BUT * H_BUT );
        int Ind = 0;
        ///
        /*
        int TOP_W = W_BUT - 1;
        int TOP_H = H_BUT - 1;
        int W10 = W_BUT/10;
        int H10 = H_BUT/10;
        for( int y = 0; y<H_BUT; y++ ){
            for( int x = 0; x<W_BUT; x++ ){
                if( ( (x%W10)==0 || (y%H10)==0 ) ||
                                ( x==TOP_W ) || ( y==TOP_H ) || ( x==0 ) || ( y==0 )
                     ){
                    Ind = ( W_BUT * y + x );
                    RgbGrid[Ind].rgbRed = RgbGrid[Ind].rgbGreen = RgbGrid[Ind].rgbBlue = 0;
                }
            }
        }
        //*/
        int W_Grid = W_BUT;
        int H_Grid = H_BUT;
	int IndDivY = 0;
	Ind = ( W_BUT * (H_BUT/2) );
	memset( &RgbGrid[Ind], 0, 4 * W_BUT );
	for( int y=0; y<H_Grid; y++ ){
		int IndDivX = 0;
		for( int x=0; x<W_Grid; x++ ){
			if( (IndDivY*H_Grid)/10==y ){
                IndDivY++;
                Ind = ( W_BUT * y + x );
				RgbGrid[Ind].rgbRed = RgbGrid[Ind].rgbGreen = RgbGrid[Ind].rgbBlue = 0;
			}else{
			    if( (IndDivX*W_Grid)/10==x ){
                    Ind = ( W_BUT * y + x );
                    RgbGrid[Ind].rgbRed = RgbGrid[Ind].rgbGreen = RgbGrid[Ind].rgbBlue = 0;
                    IndDivX++;
			    }else{
                    if( x==W_Grid-1 || y==H_Grid-1 ){
                        Ind = ( W_BUT * y + x );
                        RgbGrid[Ind].rgbRed = RgbGrid[Ind].rgbGreen = RgbGrid[Ind].rgbBlue = 0;
                    }else{
                        if( y==0 ){
                            Ind = ( W_BUT * y + x );
                            RgbGrid[Ind].rgbRed = RgbGrid[Ind].rgbGreen = RgbGrid[Ind].rgbBlue = 0;
                        }
                    }
			    }
            }
            if ((IndDivY*H_Grid)/10==y) {
               /// IndDivY++;
            }
		}
    }
        ///
        return (RgbGrid!=NULL);
    }
    ///
    ///
    BOOL FillGrid(){
        memcpy( RgbQ, RgbGrid, 4 * W_BUT * H_BUT );
        return TRUE;
    }
    ///
    ///
    BOOL Show( BOOL SBB ){
        return ShowWindow( HWCTRL, 1&((int)SBB) );
    }
    ///
    ///
    BOOL SetToolTip( LPSTR LPTT ){
        RECT Rect = {0};
        TOOLINFO           TInfo = {0};
        DestroyWindow( HWTOOLT );
        HWTOOLT = CreateWindowEx( WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
                                       WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
                                       0, 0, 0, 0, HWCTRL, NULL, NULL, NULL );
        SetWindowPos( HWTOOLT, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
        GetClientRect ( HWCTRL, &Rect );
        TInfo.cbSize = sizeof(TOOLINFO);
        TInfo.uFlags = TTF_SUBCLASS;
        TInfo.hwnd = HWCTRL;
        TInfo.hinst = NULL;
        TInfo.uId = 0;
        TInfo.lpszText = LPTT;
        TInfo.rect.left = Rect.left;
        TInfo.rect.top = Rect.top;
        TInfo.rect.right = Rect.right;
        TInfo.rect.bottom = Rect.bottom;
        SendMessage( HWTOOLT, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &TInfo );
        return TRUE;
    }
    ///
    ///
    BOOL CreateImage( void ){
        DeleteObject( HBMPLOT );
        HDC ODC = GetDC( HWCTRL );
        HBMPLOT = CreateCompatibleBitmap( ODC , W_BUT, H_BUT );
        ReleaseDC(HWCTRL,ODC);
        DeleteDC( ODC );
        return (HBMPLOT!=NULL);
    }
    ///
    ///
    BOOL SetImage( void ){
        SetBitmapBits( HBMPLOT, 4 * W_BUT * H_BUT, RgbGrid );
        return (BOOL)SendMessageA( HWCTRL, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)HBMPLOT );
    }
    ///
    ///
    ///
};
/// /////////////////////////
/// MIRAR LOS PLOTS ORGANIZARLOS;
/// /////////////////////////


