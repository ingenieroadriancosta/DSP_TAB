DWORD WINAPI PruebVari( LPVOID pv=NULL ){
    Sleep( 1000 );
    /// short TYU[4] = { 0, 1, 2, 3 };MIO_MNG.SaveMatFile_Short( "TYU.mat", "TYU", TYU, 4 );
    ///
    /*
        if( matvar->rank==2 ){
        sprintf( T2T, "matvar->name: %s\n"
                 "matvar->rank: %i\n"
                 "matvar->dims: %i\n"
                 "matvar->dims: %i\n"
                 "",
                    matvar->name,
                    matvar->rank,
                    matvar->dims[0],
                    matvar->dims[1]
                     );
        }
        //*/
    ///
    char T2T[1024] = "C:\\Users\\ADRIAN COSTA\\Desktop\\Aaa.mat";
    int Var2Ret[1024] = {0};
    int MaxNPos = 1024;
    if( MIO_MNG.LoadMatVarInt1D( T2T,
                               "BaudRate", Var2Ret, &MaxNPos, NULL ) ){
        ///
        //Dlg_ADQ.MsgBoxF( "MaxNPos = %i\n%i", MaxNPos, Var2Ret[0] );
    }
    MaxNPos = 1024;
    VarsInfo VINFOUT[1024];
    MIO_MNG.GetVarsInfo1D( T2T, VINFOUT, &MaxNPos );
    for( int i=0; i<MaxNPos; i++ ){
        Dlg_ADQ.MsgBoxF( "LenOfV = %i\n%s", VINFOUT[i].LenOfV, VINFOUT[i].VName );
    }
    /*
    mat_t    *matfp;
    matvar_t *matvar;
    matfp = (mat_t*)MIO_MNG.Mat_Open( "C:\\Users\\ADRIAN COSTA\\Desktop\\Aaa.mat", 1 );
    while( (matvar = (matvar_t*)MIO_MNG.Mat_VarReadNextInfo(matfp)) != NULL ){
        strcat( T2T, matvar->name );
        strcat( T2T, "\n" );
        ///                                 BaudRate                    ///
        ///
        MIO_MNG.Mat_VarFree(matvar);
        matvar = NULL;
        //break;
    }
    MIO_MNG.Mat_Close(matfp);
    Dlg_ADQ.MsgBoxF( T2T );
    //*/
    return TRUE;
}
