BOOL DLG_TAD::DSPProc0( fftprepare* ftp, short* SHTemp, int NFrames2,
                      short* SBuffN, PLOTEOS* HWPLOTN_OUT, int IndChannT,
                      BOOL ForcePlot ){
    int ProcSel = 0;
    int ButSelP = ButSel[IndChannT];
    double MaxVal = 0;
    BOOL ObligPlot = FALSE;
    /// ////////////////////////////////////////////////////////////////////////////////////
    /// ////////////////////////////////////////////////////////////////////////////////////
    /// ////////////////////////////////////////////////////////////////////////////////////
    /// ////////////////////////////////////////////////////////////////////////////////////
    /// NFrames2 = 1024;
    /// ////////////////////////////////////////////////////////////////////////////////////
    /// ////////////////////////////////////////////////////////////////////////////////////
    /// ////////////////////////////////////////////////////////////////////////////////////
    /// ////////////////////////////////////////////////////////////////////////////////////
    memcpy( SHTemp, SBuffN, NFrames2 * sizeof(short)+4 );
    for( int i=0; i<NFrames2; i++ ){
        ftp->InputV[i] = SHTemp[i]/32767.0;
    }
    for( int i=0; i<=ButSelP; i++ ){
        Sleep(1);
        ProcSel = DSP_TYPE_PROC[IndChannT][i];
        if( FDS[IndChannT][i].GetBInput()>0 ){
            for( int i=0; i<NFrames2; i++ ){
                ftp->InputV[i] = SHTemp[i]/32767.0;
            }
        }
        if( FDS[IndChannT][i].IsOpen() ){
            continue;
        }
        switch( ProcSel ){
            /// //////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////
            case 0:{/// PLOT
                /*
                for( int i=0; i<NFrames2; i++ ){
                    ftp->InputV[i] = SHTemp[i]/32767.0;
                }
                //*/
            }
            break;
            /// //////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////
            case 1:{/// FILTRO
                /// MsgBoxF( "%f", FDS[IndChannT][i].GetFcH() );
                /// FDS[IndChannT][i].IIRSet();
                FDS[IndChannT][i].Apply_Filter( NFrames2, ftp );
            }
            break;
            /// //////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////
            case 2:{/// VENTANA
                FDS[IndChannT][i].SetWindowing( NFrames2, ftp->InputV );
            }
            break;
            /// //////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////
            /// FFT 3
            case 3:{/// FFT
                if( i==ButSelP ){
                    ///ObligPlot = TRUE;
                }else{
                    continue;
                }
                ZeroMemory( ftp->RealV, NFrames2 * sizeof(double) );
                ZeroMemory( ftp->ImagV, NFrames2 * sizeof(double) );
                MIO_MNG.FFT_Dll( NFrames2, ftp->InputV, ftp->RealV, ftp->ImagV, FALSE );
                /// MIO_MNG.FFT( NFrames2, ftp->InputV, ftp->RealV, ftp->ImagV, FALSE );
                MaxVal = 0;
                for( int i2=0; i2<NFrames2; i2++ ){
                    ftp->RealV[i2] = 2*sqrt( ftp->RealV[i2] * ftp->RealV[i2] + ftp->ImagV[i2] * ftp->ImagV[i2] )/2000;
                    if( MaxVal<ftp->RealV[i2] ){
                        MaxVal = ftp->RealV[i2];
                    }
                }
                if( i==ButSelP ){
                    memcpy( ftp->InputV, ftp->RealV, NFrames2 * sizeof(double)+4 );
                }
            }
            break;
            /// //////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////
            /// SPECTOGRAM
            case 4:{/// SPECTOGRAM
                if( i==ButSelP ){
                    ///ObligPlot = TRUE;
                }else{
                    continue;
                }
                FDS[IndChannT][i].SPecto.Spectogram_TAD( NFrames2, ftp );
            }
            break;
            /// //////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////
            /// WIGNER
            case 5:{/// WIGNER
                if( i==ButSelP ){
                    ///ObligPlot = TRUE;
                }else{
                    continue;
                }
                FDS[IndChannT][i].SPecto.Wigner_TAD( NFrames2, ftp );
            }
            break;
            /// //////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////
            /// CHOI_WILLIAM
            case 6:{/// CHOI_WILLIAM
                if( i==ButSelP ){
                    ///ObligPlot = TRUE;
                }else{
                    continue;
                }
                FDS[IndChannT][i].SPecto.ChoiW_TAD( NFrames2, ftp, 1 );
            }
            break;
            /// //////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////
            case 7:{
                const double DCVal = FDS[IndChannT][i].GetCentralValue();
                double MaxVal = -1e100;
                for( int i=0; i<NFrames2; i++ ){
                    ftp->InputV[i] = ( ftp->InputV[i] - DCVal );
                    MaxVal = fmax( fabs(ftp->InputV[i]), MaxVal );
                    /*
                    if( fabs(ftp->InputV[i])>1 ){
                        if( ftp->InputV[i]<0 ){
                            ftp->InputV[i] = -1;
                        }else{
                            ftp->InputV[i] = 1;
                        }
                    }
                    //*/
                }
                ///
                for( int i=0; i<NFrames2; i++ ){
                    ftp->InputV[i] = ftp->InputV[i]/MaxVal;
                }
                ///
            }
            break;
            /// //////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////

            ///
        }
        ///
        #define YTAD_SPECTO 1000
        if( i==ButSelP ){
            ///
            ///
            MaxVal = -1e100;
            for( int n=0; n<NFrames2; n++ ){
                MaxVal = fmax( fabs(ftp->InputV[n]), MaxVal );
            }
            if( MaxVal<1 ){
                MaxVal = 1;
            }
            for( int n=0; n<NFrames2; n++ ){
                SHTemp[n] = (short)( 32767 * (ftp->InputV[n]/MaxVal) );
            }
            ///
            ///
            /// HWPLOTN_OUT->FillGrid( ForcePlot || ObligPlot );
            HWPLOTN_OUT->SetLineColor( LineColors[2] );
            ///
            switch( ProcSel ){
                case 3:
                    HWPLOTN_OUT->SetMaxYLim( 1 );HWPLOTN_OUT->SetMinYLim( 0 );
                    HWPLOTN_OUT->TextMx = "";//"Hz ";
                    HWPLOTN_OUT->TextMy = NULL;
                    HWPLOTN_OUT->SetMaxXLim( 1000, FALSE );
                    HWPLOTN_OUT->Text2ShowF( " Frecuencia (Hz) " );
                    HWPLOTN_OUT->PLOT_U_SHORT_FFT( SHTemp, NFrames2/2, 0, ForcePlot || ObligPlot );
                    break;
                    ///
                case 4:{
                    HWPLOTN_OUT->SetMaxYLim( 1000 );HWPLOTN_OUT->SetMinYLim( 0 );
                    HWPLOTN_OUT->TextMy = " Frecuencia (Hz) ";//"Hz ";
                    HWPLOTN_OUT->SetMaxXLim( (int)(NFrames2/2000), FALSE );
                    HWPLOTN_OUT->Text2ShowF( " Tiempo (Seg) " );
                    HWPLOTN_OUT->PLOT_SPECTOGRAM(   FDS[IndChannT][i].SPecto.Get_Spect_Out(),
                                                    FDS[IndChannT][i].SPecto.Get_xs(),
                                                    FDS[IndChannT][i].SPecto.Get_ys()/2,
                                                        ForcePlot || ObligPlot );
                }
                    break;
                case 5:{
                    HWPLOTN_OUT->SetMaxYLim( YTAD_SPECTO );HWPLOTN_OUT->SetMinYLim( 0 );
                    HWPLOTN_OUT->TextMy = " Frecuencia (Hz) ";//"Hz ";
                    HWPLOTN_OUT->SetMaxXLim( (int)(NFrames2/2000), FALSE );
                    HWPLOTN_OUT->Text2ShowF( " Tiempo (Seg) " );
                    HWPLOTN_OUT->PLOT_SPECTOGRAM(   FDS[IndChannT][i].SPecto.Get_Spect_Out(),
                                                    FDS[IndChannT][i].SPecto.Get_xs(),
                                                    FDS[IndChannT][i].SPecto.Get_ys()/2,
                                                        ForcePlot || ObligPlot );
                }
                    break;
                case 6:{
                    HWPLOTN_OUT->SetMaxYLim( YTAD_SPECTO );HWPLOTN_OUT->SetMinYLim( 0 );
                    HWPLOTN_OUT->TextMy = " Frecuencia (Hz) ";//"Hz ";
                    HWPLOTN_OUT->SetMaxXLim( (int)(NFrames2/2000), FALSE );
                    HWPLOTN_OUT->Text2ShowF( " Tiempo (Seg) " );
                    HWPLOTN_OUT->PLOT_SPECTOGRAM(   FDS[IndChannT][i].SPecto.Get_Spect_Out(),
                                                    FDS[IndChannT][i].SPecto.Get_xs(),
                                                    FDS[IndChannT][i].SPecto.Get_ys()/2,
                                                        ForcePlot || ObligPlot );
                }
                    break;
                default:
                    HWPLOTN_OUT->SetMaxYLim( 1 );HWPLOTN_OUT->SetMinYLim( -1 );
                    HWPLOTN_OUT->TextMx = "";//"Seg ";
                    HWPLOTN_OUT->TextMy = NULL;
                    HWPLOTN_OUT->SetMaxXLim( (int)(NFrames2/2000), FALSE );
                    HWPLOTN_OUT->Text2ShowF( " Tiempo (Seg) " );
                    HWPLOTN_OUT->PLOTSHORT( SHTemp, NFrames2, 0, ForcePlot );
                    /// MsgBoxF( "%i", NFrames2 );

            }
            break;
        }
        ///
    }
    return TRUE;
}






