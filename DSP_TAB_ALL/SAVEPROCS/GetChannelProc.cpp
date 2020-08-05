BOOL DLG_TAD::GetChannelProc( int ChannProc, LPSTR TOUT, BOOL Set2BSel ){
    /// DSP_TYPE_PROC[4][8]
    int ButSelP = 8;
    if( Set2BSel ){
        ButSelP = ButSel[ChannProc] + 1;
    }
    char* CTemp = (char*)malloc( 10 * 1024 );
    ZeroMemory( CTemp, 10 * 1024 );
    TOUT[0] = 0;
    sprintf( CTemp, "%%Procesamiento del canal %i:(Chann_%i)\n%%\n", ChannProc+1, ChannProc );/// Chann_0
    strcat( TOUT, CTemp );
    ///
    int NNProc = 0;
    ///
    for( int i=0; i<ButSelP; i++ ){
        sprintf( CTemp, "%%Porcesamiento del canal %i módulo %i\n", ChannProc+1, i ); /// Chann_0
        strcat( TOUT, CTemp );
        if( i==0 ){
            sprintf( CTemp, "Var_%i_%i = double(Chann_%i)/32767;\n", ChannProc+1, i, ChannProc );/// Chann_0
            strcat( TOUT, CTemp );
        }else{
            if( FDS[ChannProc][i].GetBInput()>0 ){
                sprintf( CTemp, "Var_%i_%i = double(Chann_%i)/32767;\n", ChannProc+1, i, ChannProc );/// Chann_0
                strcat( TOUT, CTemp );
            }else{
                for( int i_2=i-1; i_2>=0; i_2-- ){
                    NNProc = DSP_TYPE_PROC[ChannProc][i_2];
                    if( i_2==0 ){
                        if( NNProc<3 || NNProc>6 ){
                            sprintf( CTemp, "Var_%i_%i = Var_%i_%i;\n", ChannProc+1, i, ChannProc+1, i_2 );
                            strcat( TOUT, CTemp );
                        }else{
                            sprintf( CTemp, "Var_%i_%i = double(Chann_0)/32767;\n", ChannProc+1, i );
                            strcat( TOUT, CTemp );
                        }
                        break;
                    }
                    if( NNProc<3 || NNProc>6 ){
                        if( i_2==0 ){
                            sprintf( CTemp, "Var_%i_%i = Chann_0;\n", ChannProc+1, i );
                            strcat( TOUT, CTemp );
                        }else{
                            sprintf( CTemp, "Var_%i_%i = Var_%i_%i;\n", ChannProc+1, i, ChannProc+1, i_2 );/// Chann_0
                            strcat( TOUT, CTemp );
                        }
                        break;
                    }
                }
            }
        }
        ///
        ///
        ///
        ///
        //*
        NNProc = DSP_TYPE_PROC[ChannProc][i];
        switch( NNProc ){
            /// /////////////////////////////////////////////////////////////////////////////////////// NEWCASE 0
            case 0:/// PLOT
                sprintf( CTemp, "figure\nplot( Var_%i_%i );\n", ChannProc+1, i );
                strcat( TOUT, CTemp );
                sprintf( CTemp, "title( 'Módulo %i del canal %i' );\n", i, ChannProc+1 );
                strcat( TOUT, CTemp );
                break;
            /// /////////////////////////////////////////////////////////////////////////////////////// NEWCASE 1
            case 1:/// FILTER
                if( FDS[ChannProc][i].IsIRR ){
                    switch( FDS[ChannProc][i].GetFilType() ){
                        case 0:
                            sprintf( CTemp, "[b, a] = butter( %i, %g/FNyq, 'low' );\n",
                                            FDS[ChannProc][i].GetOrdFil(),
                                                FDS[ChannProc][i].GetFcH() );
                            break;
                        case 1:
                            sprintf( CTemp, "[b, a] = butter( %i, %g/FNyq, 'high' );\n",
                                            FDS[ChannProc][i].GetOrdFil(),
                                                FDS[ChannProc][i].GetFcL() );
                            break;
                        case 2:
                            sprintf( CTemp, "[b, a] = butter( %i, [%g, %g]/FNyq, 'bandpass' );\n",
                                            FDS[ChannProc][i].GetOrdFil(),
                                                FDS[ChannProc][i].GetFcL(), FDS[ChannProc][i].GetFcH() );
                            break;
                        case 3:
                            sprintf( CTemp, "[b, a] = butter( %i, [%g, %g]/FNyq, 'stop' );\n",
                                            FDS[ChannProc][i].GetOrdFil(),
                                                FDS[ChannProc][i].GetFcL(), FDS[ChannProc][i].GetFcH() );
                            break;
                    }
                    strcat( TOUT, CTemp );
                    sprintf( CTemp, "H = fft( b, length(Var_%i_%i) )./fft( a, length(Var_%i_%i) );\n",
                                    ChannProc+1, i, ChannProc+1, i );
                    strcat( TOUT, CTemp );
                    sprintf( CTemp, "Y = fft( Var_%i_%i, length(Var_%i_%i) );\n",
                                    ChannProc+1, i, ChannProc+1, i );
                    strcat( TOUT, CTemp );
                    sprintf( CTemp, "Var_%i_%i = real( ifft( Y .* H ) );\n",
                                    ChannProc+1, i );
                    strcat( TOUT, CTemp );
                }else{
                    switch( FDS[ChannProc][i].GetFilType() ){
                        case 0:
                            if( FDS[ChannProc][i].ExistOnMatlab() ){
                                if( FDS[ChannProc][i].HaveAlpha() ){
                                    sprintf( CTemp, "b = fir1( %i, %g/FNyq, 'low'%s( %i, %g ) );\n",
                                                FDS[ChannProc][i].GetOrdFil(),
                                                        FDS[ChannProc][i].GetFcH(),
                                                            &Wind_2_Fir_name[FDS[ChannProc][i].GetWType()][0],
                                                FDS[ChannProc][i].GetOrdFil()+1, FDS[ChannProc][i].GetAlpha() );
                                    strcat( TOUT, CTemp );
                                }else{
                                    sprintf( CTemp, "b = fir1( %i, %g/FNyq, 'low'%s( %i ) );\n",
                                                FDS[ChannProc][i].GetOrdFil(),
                                                        FDS[ChannProc][i].GetFcH(),
                                                            &Wind_2_Fir_name[FDS[ChannProc][i].GetWType()][0],
                                                FDS[ChannProc][i].GetOrdFil()+1 );
                                    strcat( TOUT, CTemp );
                                }
                            }else{
                                sprintf( CTemp, "b = fir1( %i, %g/FNyq, 'low'%s( %i ) );%% La ventana seleccionada no existe en matlab.\n",
                                            FDS[ChannProc][i].GetOrdFil(),
                                                    FDS[ChannProc][i].GetFcH(),
                                                        &Wind_2_Fir_name[0][0],
                                            FDS[ChannProc][i].GetOrdFil()+1 );
                                strcat( TOUT, CTemp );
                            }
                            break;
                        case 1:
                            if( FDS[ChannProc][i].ExistOnMatlab() ){
                                if( FDS[ChannProc][i].HaveAlpha() ){
                                    sprintf( CTemp, "b = fir1( %i, %g/FNyq, 'high'%s( %i, %g ) );\n",
                                                FDS[ChannProc][i].GetOrdFil(),
                                                        FDS[ChannProc][i].GetFcL(),
                                                            &Wind_2_Fir_name[FDS[ChannProc][i].GetWType()][0],
                                                FDS[ChannProc][i].GetOrdFil()+1, FDS[ChannProc][i].GetAlpha() );
                                    strcat( TOUT, CTemp );
                                }else{
                                    sprintf( CTemp, "b = fir1( %i, %g/FNyq, 'high'%s( %i ) );\n",
                                                FDS[ChannProc][i].GetOrdFil(),
                                                        FDS[ChannProc][i].GetFcL(),
                                                            &Wind_2_Fir_name[FDS[ChannProc][i].GetWType()][0],
                                                FDS[ChannProc][i].GetOrdFil()+1 );
                                    strcat( TOUT, CTemp );
                                }
                            }else{
                                sprintf( CTemp, "b = fir1( %i, %g/FNyq, 'high'%s( %i ) );%% La ventana seleccionada no existe en matlab.\n",
                                            FDS[ChannProc][i].GetOrdFil(),
                                                    FDS[ChannProc][i].GetFcL(),
                                                        &Wind_2_Fir_name[0][0],
                                            FDS[ChannProc][i].GetOrdFil()+1 );
                                strcat( TOUT, CTemp );
                            }
                            break;
                        case 2:
                            if( FDS[ChannProc][i].ExistOnMatlab() ){
                                if( FDS[ChannProc][i].HaveAlpha() ){
                                    sprintf( CTemp, "b = fir1( %i, [%g, %g]/FNyq, 'DC-0'%s( %i, %g ) );\n",
                                                FDS[ChannProc][i].GetOrdFil(),
                                                        FDS[ChannProc][i].GetFcL(), FDS[ChannProc][i].GetFcH(),
                                                            &Wind_2_Fir_name[FDS[ChannProc][i].GetWType()][0],
                                                FDS[ChannProc][i].GetOrdFil()+1, FDS[ChannProc][i].GetAlpha() );
                                    strcat( TOUT, CTemp );
                                }else{
                                    sprintf( CTemp, "b = fir1( %i, [%g, %g]/FNyq, 'DC-0'%s( %i ) );\n",
                                                FDS[ChannProc][i].GetOrdFil(),
                                                        FDS[ChannProc][i].GetFcL(), FDS[ChannProc][i].GetFcH(),
                                                            &Wind_2_Fir_name[FDS[ChannProc][i].GetWType()][0],
                                                FDS[ChannProc][i].GetOrdFil()+1 );
                                    strcat( TOUT, CTemp );
                                }
                            }else{
                                sprintf( CTemp, "b = fir1( %i, [%g, %g]/FNyq, 'DC-0'%s( %i ) );%% La ventana seleccionada no existe en matlab.\n",
                                            FDS[ChannProc][i].GetOrdFil(),
                                                    FDS[ChannProc][i].GetFcL(), FDS[ChannProc][i].GetFcH(),
                                                        &Wind_2_Fir_name[0][0],
                                            FDS[ChannProc][i].GetOrdFil()+1 );
                                strcat( TOUT, CTemp );
                            }
                            break;
                        case 3:
                            if( FDS[ChannProc][i].ExistOnMatlab() ){
                                if( FDS[ChannProc][i].HaveAlpha() ){
                                    sprintf( CTemp, "b = fir1( %i, [%g, %g]/FNyq, 'DC-1'%s( %i, %g ) );\n",
                                                FDS[ChannProc][i].GetOrdFil(),
                                                        FDS[ChannProc][i].GetFcL(), FDS[ChannProc][i].GetFcH(),
                                                            &Wind_2_Fir_name[FDS[ChannProc][i].GetWType()][0],
                                                FDS[ChannProc][i].GetOrdFil()+1, FDS[ChannProc][i].GetAlpha() );
                                    strcat( TOUT, CTemp );
                                }else{
                                    sprintf( CTemp, "b = fir1( %i, [%g, %g]/FNyq, 'DC-1'%s( %i ) );\n",
                                                FDS[ChannProc][i].GetOrdFil(),
                                                        FDS[ChannProc][i].GetFcL(), FDS[ChannProc][i].GetFcH(),
                                                            &Wind_2_Fir_name[FDS[ChannProc][i].GetWType()][0],
                                                FDS[ChannProc][i].GetOrdFil()+1 );
                                    strcat( TOUT, CTemp );
                                }
                            }else{
                                sprintf( CTemp, "b = fir1( %i, [%g, %g]/FNyq, 'DC-1'%s( %i ) );%% La ventana seleccionada no existe en matlab.\n",
                                            FDS[ChannProc][i].GetOrdFil(),
                                                    FDS[ChannProc][i].GetFcL(), FDS[ChannProc][i].GetFcH(),
                                                        &Wind_2_Fir_name[0][0],
                                            FDS[ChannProc][i].GetOrdFil()+1 );
                                strcat( TOUT, CTemp );
                            }
                            break;
                    }
                    sprintf( CTemp, "B = fft( b, length(Var_%i_%i) );\n", ChannProc+1, i );
                    strcat( TOUT, CTemp );
                    sprintf( CTemp, "Y = fft( Var_%i_%i, length(Var_%i_%i) ) .* B;\n", ChannProc+1, i, ChannProc+1, i );
                    strcat( TOUT, CTemp );
                    sprintf( CTemp, "Var_%i_%i = real( ifft( Y, length(Var_%i_%i) ) );\n", ChannProc+1, i, ChannProc+1, i );
                    strcat( TOUT, CTemp );
                }
                break;
            /// ENDNEWCASE 1
            /// /////////////////////////////////////////////////////////////////////////////////////// NEWCASE 2
            case 2:/// WINDOWING
                if( FDS[ChannProc][i].WINDWN.ExistOnMatlab() ){
                    if( FDS[ChannProc][i].WINDWN.HaveWindowAlpha() ){
                        sprintf( CTemp, "Var_%i_%i = Var_%i_%i .* %s( %i, %g )';\n",
                                    ChannProc+1, i,
                                    ChannProc+1, i,
                                        &Wind_2_Fir_nameFUNC[FDS[ChannProc][i].GetTypeWindow()][0],
                                        SerHand.NFramesReaded,
                                        FDS[ChannProc][i].GetAlphaWindow()
                            );
                    }else{
                        sprintf( CTemp, "Var_%i_%i = Var_%i_%i .* %s( %i )';\n",
                                    ChannProc+1, i,
                                    ChannProc+1, i,
                                        &Wind_2_Fir_nameFUNC[FDS[ChannProc][i].GetTypeWindow()][0],
                                        SerHand.NFramesReaded
                            );
                    }
                }else{
                    sprintf( CTemp, "Var_%i_%i = Var_%i_%i .* %s( %i )';%% La función de ventana no existe en matlab.\n",
                                    ChannProc+1, i,
                                    ChannProc+1, i,
                                        &Wind_2_Fir_nameFUNC[0][0],
                                        SerHand.NFramesReaded
                            );
                }
                strcat( TOUT, CTemp );
                break;
            /// ENDNEWCASE 2
            ///
            ///
            /// /////////////////////////////////////////////////////////////////////////////////////// NEWCASE 3
            case 3:/// FFT
                sprintf( CTemp,
                        "F = FNyq * (0:%i/2)/(%i/2);\n", SerHand.NFramesReaded, SerHand.NFramesReaded );
                strcat( TOUT, CTemp );
                sprintf( CTemp,
                        "Var_%i_%i = fft( Var_%i_%i, %i );\n"
                        "Var_%i_%i = abs( Var_%i_%i( 1:length(Var_%i_%i)/2+1 ) );\n",
                                ChannProc+1, i,
                                ChannProc+1, i,
                                SerHand.NFramesReaded,
                                ChannProc+1, i,
                                ChannProc+1, i,
                                ChannProc+1, i
                                );
                strcat( TOUT, CTemp );
                ///
                strcat( TOUT, "figure\n" );
                sprintf( CTemp,
                        "plot( F, Var_%i_%i );\n",
                                ChannProc+1, i
                                );
                strcat( TOUT, CTemp );
                sprintf( CTemp, "title( 'Módulo %i del canal %i' );\n", i, ChannProc+1 );
                strcat( TOUT, CTemp );
                break;
            /// ENDNEWCASE 3
            ///
            ///
            /// /////////////////////////////////////////////////////////////////////////////////////// NEWCASE 4
            case 4:/// SPECTO
                /// spectrogram( Var_1_1, blackman(Fs), 1, 1024, Fs,'yaxis' );
                if( FDS[ChannProc][i].SPecto.WINDWN.ExistOnMatlab() ){
                    if( FDS[ChannProc][i].SPecto.WINDWN.HaveWindowAlpha() ){
                        strcat( TOUT, "figure\n" );
                        sprintf( CTemp, "spectrogram( Var_%i_%i, %s(Fs, %g), 1, 1024, Fs, 'yaxis' );\n",
                                                ChannProc+1, i,
                                                &Wind_2_Fir_nameFUNC[FDS[ChannProc][i].SPecto.WINDWN.GetWinType()][0],
                                                FDS[ChannProc][i].SPecto.WINDWN.GetAlpha()
                                                 );
                        strcat( TOUT, CTemp );
                    }else{
                        strcat( TOUT, "figure\n" );
                        sprintf( CTemp, "spectrogram( Var_%i_%i, %s(Fs), 1, 1024, Fs, 'yaxis' );\n",
                                                ChannProc+1, i,
                                                &Wind_2_Fir_nameFUNC[FDS[ChannProc][i].SPecto.WINDWN.GetWinType()][0]
                                                 );
                        strcat( TOUT, CTemp );
                    }
                }else{
                    strcat( TOUT, "figure\n" );
                    sprintf( CTemp, "spectrogram( Var_%i_%i, %s(Fs), 1, 1024, Fs, 'yaxis' );%%La ventana aplicada no existe en matlab.\n",
                                                ChannProc+1, i,
                                                &Wind_2_Fir_nameFUNC[0][0]
                                                 );
                    strcat( TOUT, CTemp );
                }
                sprintf( CTemp, "title( 'Módulo %i del canal %i' );\n", i, ChannProc+1 );
                strcat( TOUT, CTemp );
                break;
                /// ENDNEWCASE 4
            ///
            ///
            /// /////////////////////////////////////////////////////////////////////////////////////// NEWCASE 5
            case 5:/// WIGNER
                if( FDS[ChannProc][i].SPecto.WINDWN.ExistOnMatlab() ){
                    if( FDS[ChannProc][i].SPecto.WINDWN.HaveWindowAlpha() ){
                        strcat( TOUT, "figure\n" );
                        sprintf( CTemp, "wigner( Var_%i_%i, Fs, %s(Fs, %g) );\n",
                                                ChannProc+1, i,
                                                &Wind_2_Fir_nameFUNC[FDS[ChannProc][i].SPecto.WINDWN.GetWinType()][0],
                                                FDS[ChannProc][i].SPecto.WINDWN.GetAlpha()
                                                 );
                        strcat( TOUT, CTemp );
                    }else{
                        strcat( TOUT, "figure\n" );
                        sprintf( CTemp, "wigner( Var_%i_%i, Fs, %s(Fs) );\n",
                                                ChannProc+1, i,
                                                &Wind_2_Fir_nameFUNC[FDS[ChannProc][i].SPecto.WINDWN.GetWinType()][0]
                                                 );
                        strcat( TOUT, CTemp );
                    }
                }else{
                    strcat( TOUT, "figure\n" );
                    sprintf( CTemp, "wigner( Var_%i_%i, Fs, %s(Fs) );%%La ventana aplicada no existe en matlab.\n",
                                                ChannProc+1, i,
                                                &Wind_2_Fir_nameFUNC[0][0]
                                                 );
                    strcat( TOUT, CTemp );
                }
                sprintf( CTemp, "title( 'Módulo %i del canal %i' );\n", i, ChannProc+1 );
                strcat( TOUT, CTemp );
                break;
                /// ENDNEWCASE 5
            ///
            ///
            /// /////////////////////////////////////////////////////////////////////////////////////// NEWCASE 5
            case 6:/// CHOI - WILLIAM
                if( FDS[ChannProc][i].SPecto.WINDWN.ExistOnMatlab() ){
                    if( FDS[ChannProc][i].SPecto.WINDWN.HaveWindowAlpha() ){
                        strcat( TOUT, "figure\n" );
                        sprintf( CTemp, "choi_william( Var_%i_%i, Fs, %s(Fs, %g) );\n",
                                                ChannProc+1, i,
                                                &Wind_2_Fir_nameFUNC[FDS[ChannProc][i].SPecto.WINDWN.GetWinType()][0],
                                                FDS[ChannProc][i].SPecto.WINDWN.GetAlpha()
                                                 );
                        strcat( TOUT, CTemp );
                    }else{
                        strcat( TOUT, "figure\n" );
                        sprintf( CTemp, "choi_william( Var_%i_%i, Fs, %s(Fs) );\n",
                                                ChannProc+1, i,
                                                &Wind_2_Fir_nameFUNC[FDS[ChannProc][i].SPecto.WINDWN.GetWinType()][0]
                                                 );
                        strcat( TOUT, CTemp );
                    }
                }else{
                    strcat( TOUT, "figure\n" );
                    sprintf( CTemp, "choi_william( Var_%i_%i, Fs, %s(Fs) );%%La ventana aplicada no existe en matlab.\n",
                                                ChannProc+1, i,
                                                &Wind_2_Fir_nameFUNC[0][0]
                                                 );
                    strcat( TOUT, CTemp );
                }
                sprintf( CTemp, "title( 'Módulo %i del canal %i' );\n", i, ChannProc+1 );
                strcat( TOUT, CTemp );
                break;
                /// ENDNEWCASE 5
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            case 7:
                sprintf( CTemp, "Var_%i_%i = 2*( Var_%i_%i - 0.5 );\n", ChannProc+1, i, ChannProc+1, i );/// Chann_0
                strcat( TOUT, CTemp );
                break;

        }
        strcat( TOUT, "\n" );
    }
    ///
    strcat( TOUT, "\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n" );
    strcat( TOUT, "\n\n\n" );
    ///
    free( CTemp );
    return TRUE;
}
