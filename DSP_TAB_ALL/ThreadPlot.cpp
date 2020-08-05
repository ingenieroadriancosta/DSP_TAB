BOOL DLG_TAD::ThreadPlot( BOOL ForcePlot ){
    int NFrames2 = SerHand.NFramesReaded;
    Sleep(1);//CallPlot
    //MIO_MNG.tic( 0 );
            /*               !!!!!!!!!!!!!LAST!!!!!!!!!!!!!
            HWPLOT.SetMaxXLim( ( (int)NFrames2 )/2000, ForcePlot );
            HWPLOT0.SetMaxXLim( ( (int)NFrames2 )/2000, ForcePlot );
            HWPLOT1.SetMaxXLim( ( (int)NFrames2 )/2000, ForcePlot );
            HWPLOT2.SetMaxXLim( ( (int)NFrames2 )/2000, ForcePlot );
            HWPLOT.PLOTSHORT3( SerHand.SBuff0, SerHand.SBuff1, SerHand.SBuff2, NFrames2, 0, ForcePlot );
            ///
            HWPLOT0.PLOT_U_SHORT( SerHand.SBuff0, NFrames2, 0, ForcePlot );
            HWPLOT1.PLOT_U_SHORT( SerHand.SBuff1, NFrames2, 0, ForcePlot );
            HWPLOT2.PLOT_U_SHORT( SerHand.SBuff2, NFrames2, 0, ForcePlot );
            //*/
/// ///////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////
    /// TabPage
    double TimeOsc = 10; /// 0.05
    int NFROsci = (int)(TimeOsc * 2000);
    if( TabPage==0 || ForcePlot ){
        if( NFrames2<NFROsci ){
            int TopBreak = NFrames2%(NFROsci);
            if( TopBreak==0 ){
                TopBreak = NFROsci;
            }
            HWPLOT.SetMinXLim( 0, FALSE );
            HWPLOT.SetMaxXLim( (NFROsci)/2000, ForcePlot );
            HWPLOT.PLOTSHORT3( SerHand.SBuff0,
                              SerHand.SBuff1,
                              SerHand.SBuff2,
                              NFROsci,
                              TopBreak,
                              0, ForcePlot );
        }else{
            int NINI = NFrames2 - NFrames2%NFROsci;
            if( NINI<0 ){
                NINI = 0;
            }
            int TopBreak = NFrames2%(NFROsci);
            if( TopBreak==0 ){
                NINI = NFrames2 - NFROsci;
                TopBreak = NFROsci;
            }
            ///
            HWPLOT.SetMinXLim( round( NINI/2000.0 ), FALSE );
            HWPLOT.SetMaxXLim( round( (NINI+NFROsci)/2000.0 ), ForcePlot );
            ///
            HWPLOT.PLOTSHORT3( &SerHand.SBuff0[NINI],
                               &SerHand.SBuff1[NINI],
                               &SerHand.SBuff2[NINI],
                                        NFROsci,
                                        TopBreak, 0, ForcePlot );
        }
    }
/// ///////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////
    if( TabPage==1 || ForcePlot ){
        if( ForcePlot ){
            HWPLOT0.SetMinXLim( 0, FALSE );
            HWPLOT0.SetMaxXLim( ( (int)NFrames2 )/2000, ForcePlot );
            HWPLOT0.PLOT_U_SHORT( SerHand.SBuff0, NFrames2, NFrames2, ForcePlot );
        }else{
            if( NFrames2<NFROsci ){
                int TopBreak = NFrames2%(NFROsci);
                if( TopBreak==0 ){
                    TopBreak = NFROsci;
                }
                HWPLOT0.SetMinXLim( 0, FALSE );
                HWPLOT0.SetMaxXLim( (NFROsci)/2000, ForcePlot );
                HWPLOT0.PLOT_U_SHORT( SerHand.SBuff0,
                                  NFROsci,
                                  TopBreak, ForcePlot );
            }else{
                int NINI = NFrames2 - NFrames2%NFROsci;
                if( NINI<0 ){
                    NINI = 0;
                }
                int TopBreak = NFrames2%(NFROsci);
                if( TopBreak==0 ){
                    NINI = NFrames2 - NFROsci;
                    TopBreak = NFROsci;
                }
                ///
                HWPLOT0.SetMinXLim( round( NINI/2000.0 ), FALSE );
                HWPLOT0.SetMaxXLim( round( (NINI+NFROsci)/2000.0 ), ForcePlot );
                ///
                HWPLOT0.PLOT_U_SHORT( &SerHand.SBuff0[NINI],
                                            NFROsci,
                                            TopBreak, ForcePlot );
            }
        }
    }
/// ///////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////
    if( TabPage==2 || ForcePlot ){
        if( ForcePlot ){
            HWPLOT1.SetMinXLim( 0, FALSE );
            HWPLOT1.SetMaxXLim( ( (int)NFrames2 )/2000, ForcePlot );
            HWPLOT1.PLOT_U_SHORT( SerHand.SBuff1, NFrames2, NFrames2, ForcePlot );
        }else{
            if( NFrames2<NFROsci ){
                int TopBreak = NFrames2%(NFROsci);
                if( TopBreak==0 ){
                    TopBreak = NFROsci;
                }
                HWPLOT1.SetMinXLim( 0, FALSE );
                HWPLOT1.SetMaxXLim( (NFROsci)/2000, ForcePlot );
                HWPLOT1.PLOT_U_SHORT( SerHand.SBuff1,
                                  NFROsci,
                                  TopBreak, ForcePlot );
            }else{
                int NINI = NFrames2 - NFrames2%NFROsci;
                if( NINI<0 ){
                    NINI = 0;
                }
                int TopBreak = NFrames2%(NFROsci);
                if( TopBreak==0 ){
                    NINI = NFrames2 - NFROsci;
                    TopBreak = NFROsci;
                }
                ///
                HWPLOT1.SetMinXLim( round( NINI/2000.0 ), FALSE );
                HWPLOT1.SetMaxXLim( round( (NINI+NFROsci)/2000.0 ), ForcePlot );
                ///
                HWPLOT1.PLOT_U_SHORT( &SerHand.SBuff1[NINI],
                                            NFROsci,
                                            TopBreak, ForcePlot );
            }
        }
    }
/// ///////////////////////////////////////////////////////////////////////////////////////////////
    if( TabPage==3 || ForcePlot ){
        if( ForcePlot ){
            HWPLOT2.SetMinXLim( 0, FALSE );
            HWPLOT2.SetMaxXLim( ( (int)NFrames2 )/2000, ForcePlot );
            HWPLOT2.PLOT_U_SHORT( SerHand.SBuff2, NFrames2, NFrames2, ForcePlot );
        }else{
            if( NFrames2<NFROsci ){
                int TopBreak = NFrames2%(NFROsci);
                if( TopBreak==0 ){
                    TopBreak = NFROsci;
                }
                HWPLOT2.SetMinXLim( 0, FALSE );
                HWPLOT2.SetMaxXLim( (NFROsci)/2000, ForcePlot );
                HWPLOT2.PLOT_U_SHORT( SerHand.SBuff2,
                                  NFROsci,
                                  TopBreak, ForcePlot );
            }else{
                int NINI = NFrames2 - NFrames2%NFROsci;
                if( NINI<0 ){
                    NINI = 0;
                }
                int TopBreak = NFrames2%(NFROsci);
                if( TopBreak==0 ){
                    NINI = NFrames2 - NFROsci;
                    TopBreak = NFROsci;
                }
                ///
                HWPLOT2.SetMinXLim( round( NINI/2000.0 ), FALSE );
                HWPLOT2.SetMaxXLim( round( (NINI+NFROsci)/2000.0 ), ForcePlot );
                ///
                HWPLOT2.PLOT_U_SHORT( &SerHand.SBuff2[NINI],
                                            NFROsci,
                                            TopBreak, ForcePlot );
            }
        }
    }
/// ///////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////
    if( !ForcePlot ){
        Sleep(30);
    }
    InPlotThr = FALSE;
    return TRUE;
}
