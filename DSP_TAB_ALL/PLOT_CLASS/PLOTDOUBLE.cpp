BOOL PLOTEOS::PLOTDOUBLE( double* DBuff, int MaxPos, int InitFrames, BOOL ForcePlot ){
    if( MaxPos<=0 ){
        FillGrid( TRUE );
        return FALSE;
    }
    ///
    ///
    double MaxV = -1e100;
    for( int i=0; i<MaxPos; i++ ){
        MaxV = fmax( MaxV, fabs(DBuff[i]) );
    }
    ///
    ///
    if( !BVisible && ForcePlot==FALSE ){
        return FALSE;
    }
    ///
    if( !HoldV ){
        FillGrid( FALSE );
    }
    const int WGR_1 = WidthGraph - 1;
    const int HGR_1 = HeightGraph - 1;
    ///                                                           ///
    int xpos = 0;
    int Nxpos = 0;
    int ypos = 0;
    int Nypos = 0;
    if( MaxPos<500000 || ForcePlot ){/// 2400000
    /// if( MaxPos>0 ){/// 2400000
        for( int i=0; i<MaxPos-1; i++ ){
            xpos = (int)( (i*((double)WGR_1))/MaxPos );
            ypos = (int)( HGR_1 - ( HGR_1 * DBuff[i] )/MaxV );
            setRGB( xpos, ypos, ColorLineG );
            ///
            ///
            Nxpos = (int)( ((i+1)*(double)WGR_1)/MaxPos );
            Nypos = (int)( HGR_1 - ( HGR_1 * DBuff[i+1] )/MaxV );
            if( Nxpos==xpos && Nypos==ypos ){
                continue;
            }
            while( xpos!=Nxpos ){
                xpos++;
                setRGB( xpos, ypos, ColorLineG );
            }
            ///
            while( ypos>Nypos ){
                ypos--;
                setRGB( xpos, ypos, ColorLineG );
            }
            while( ypos!=Nypos ){
                ypos++;
                setRGB( xpos, ypos, ColorLineG );
            }
            ///
        }
    }else{
        for( int i=0; i<MaxPos-1; i++ ){
            xpos = (int)( (i*((double)WGR_1))/MaxPos );
            ypos = (int)( HGR_1 - ( HGR_1 * DBuff[i] )/MaxV );
            setRGB( xpos, ypos, ColorLineG );

        }
    }
    ///
    if( !HoldV ){
        FillBuff();
    }
    return TRUE;
}
