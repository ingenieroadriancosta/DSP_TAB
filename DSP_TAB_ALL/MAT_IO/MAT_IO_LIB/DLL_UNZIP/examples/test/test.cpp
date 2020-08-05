#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "../../zip.h"
#include "../../unzip.h"
///
#define MATHLIBRARY_API __declspec(dllexport)
extern "C"{
MATHLIBRARY_API int Add(int a, int b);
MATHLIBRARY_API int UnZip( const TCHAR* , const TCHAR*  );
MATHLIBRARY_API int UnZipWin( const TCHAR* , const TCHAR*, HWND  );

}


	HZIP hz = NULL; 
	ZIPENTRY ze = {0}; 
	
  
	char* BFile = (char*)malloc( 1024 );
	char* BDir = (char*)malloc( 1024 );
	HWND HWCW = GetConsoleWindow();

	int UnZip2( const TCHAR* ZipFile, const TCHAR* FoldName );
int main( int narg, char* args[] ){
	if( narg>2 ){
		sprintf( BFile, "%s\n%s", args[1], args[2] );
		//MessageBox( HWCW, BFile, "", 0 );
		return UnZip2( args[1], args[2] );
	}
	return 0;
}





int UnZip2( const TCHAR* ZipFile, const TCHAR* FoldName ){

		/// kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib;%(AdditionalDependencies)
		
	strcpy( BFile, ZipFile );	
	
	strcpy( BDir, FoldName );	


/// /////////////////////////////////////////////////////////////////////////////////////
            hz = OpenZip(((const TCHAR *)BFile),0);
			
  if( hz==NULL ){
	  free( BFile );free( BDir );
	  return -1;
  }
  if( SetUnzipBaseDir(hz,(const TCHAR *)BDir)!=0 ){
	  return -2;
  }
	 if( GetZipItem(hz,-1,&ze) ){
		 return -3;
	 }
	 
 int numitems=ze.index;
  for (int zi=0; zi<numitems; zi++){
	  if( GetZipItem(hz,zi,&ze)!=0 ){
		  return -4;
	  }
		if( UnzipItem(hz,zi,ze.name)!=0 ){
			return -5;
		}
  }

  CloseZip(hz);
/// /////////////////////////////////////////////////////////////////////////////////////
		return 1;
	}


















extern "C"{
	MATHLIBRARY_API int Add(int a, int b){
		return ( a+b);
	}

	

	MATHLIBRARY_API int UnZip( const TCHAR* ZipFile, const TCHAR* FoldName ){

		/// kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib;%(AdditionalDependencies)
		
	strcpy( BFile, ZipFile );	
	
	strcpy( BDir, FoldName );	


/// /////////////////////////////////////////////////////////////////////////////////////
            hz = OpenZip(((const TCHAR *)BFile),0);
			
  if( hz==NULL ){
	  free( BFile );free( BDir );
	  return -1;
  }
  if( SetUnzipBaseDir(hz,(const TCHAR *)BDir)!=0 ){
	  return -2;
  }
	 if( GetZipItem(hz,-1,&ze) ){
		 return -3;
	 }
	 
 int numitems=ze.index;
  for (int zi=0; zi<numitems; zi++){
	  if( GetZipItem(hz,zi,&ze)!=0 ){
		  return -4;
	  }
		if( UnzipItem(hz,zi,ze.name)!=0 ){
			///return -5;
		}
  }

  CloseZip(hz);
			  //MessageBox( HWCW, "", "", 0 );
/// /////////////////////////////////////////////////////////////////////////////////////
		return 1;
	}
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
	
	#define PBM_SETPOS	(WM_USER+2)
	MATHLIBRARY_API int UnZipWin( const TCHAR* ZipFile, const TCHAR* FoldName, HWND HWPGR ){

		/// kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib;%(AdditionalDependencies)
		
	strcpy( BFile, ZipFile );	
	
	strcpy( BDir, FoldName );	


/// /////////////////////////////////////////////////////////////////////////////////////
            hz = OpenZip(((const TCHAR *)BFile),0);
			
  if( hz==NULL ){
	  free( BFile );free( BDir );
	  return -1;
  }
  if( SetUnzipBaseDir(hz,(const TCHAR *)BDir)!=0 ){
	  return -2;
  }
	 if( GetZipItem(hz,-1,&ze) ){
		 return -3;
	 }
	 
 int numitems=ze.index;
  for (int zi=0; zi<numitems; zi++){
		if( GetZipItem(hz,zi,&ze)!=0 ){
			return -4;
		}
		if( UnzipItem(hz,zi,ze.name)!=0 ){
			///return -5;
		}
		SendMessage( HWPGR, PBM_SETPOS, (WPARAM)( ( 100*(zi+1) )/numitems), 0 );
  }

  CloseZip(hz);
			  //MessageBox( HWCW, "", "", 0 );
/// /////////////////////////////////////////////////////////////////////////////////////
		return 1;
	}
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
}




