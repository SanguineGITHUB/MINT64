#include "Types.h"
#include "Page.h"
#include "ModeSwitch.h"

void kPrintString( int iX, int iY, const char* pcString );
BOOL kInitializeKernel64Area( void );
BOOL kIsMemoryEnough( void );

void Main( void )
{
    DWORD i;
    DWORD dwEAX, dwEBX, dwECX, dwEDX;
    char vcVendorString[ 13 ] = { 0, };

    kPrintString( 0, 3, "C Language Kernel Start.....................[Pass]" );
    
    kPrintString( 0, 4, "Minimum Memory Size Check...................[    ]" );
    if( kIsMemoryEnough() == FALSE )
    {
        kPrintString( 45, 4, "Fail" );
        kPrintString( 0, 5, "Not Enough Memory~!! MINT64 OS Requires Over "
                "64Mbyte Memory~!!" );
        while( 1 ) ;
    }
    else
    {
        kPrintString( 45, 4, "Pass" );
    }

    //IA-32e 모드의 커널 영역을 초기화
    kPrintString( 0, 5, "IA-32e Kernel Area Initialize...............[    ]" );
    if( kInitializeKernel64Area() == FALSE )
    {
        kPrintString( 45, 5, "Fail" );
        kPrintString( 0, 6, "Kernel Area Initialization Fail~!!" );
        while( 1 ) ;
    }
    kPrintString( 45, 5, "Pass" );

    kPrintString( 0, 6, "IA-32e Page Tables Initialize...............[    ]" );
    kInitializePageTables();
    kPrintString( 45, 6, "Pass" );

    // 프로세서 제조사 정보 읽기
    kReadCPUID( 0x00, &dwEAX, &dwEBX, &dwECX, &dwEDX );
    *( DWORD* ) vcVendorString = dwEBX;
    *( ( DWORD* ) vcVendorString + 1 ) = dwEDX;
    *( ( DWORD* ) vcVendorString + 2 ) = dwECX;
    kPrintString( 0, 7, "Processor Vendor String.....................[            ]");
    kPrintString( 45, 7, vcVendorString );

    //64bit 지원여부 체크
    kReadCPUID( 0x80000001, &dwEAX, &dwEBX, &dwECX, &dwEDX );
    kPrintString( 0, 8, "64bit Mode Support Check....................[    ]");
    if( dwEDX & ( 1 << 29 ) )
    {
        kPrintString( 45, 8, "Pass" );
    }
    else
    {
        kPrintString( 45, 8, "Fail" );
        kPrintString( 0, 9, "This processor does not support 64bit mode~!!" );
        while( 1 ) ;
    }

    //IA-32e모드로 전환
    kPrintString( 0, 9, "Switch To IA-32e Mode" );
    //kSwitchAndExecute64bitKernel();
    
    while( 1 ) ;
}


void kPrintString( int iX, int iY, const char* pcString )
{
    CHARACTER* pstScreen = ( CHARACTER* ) 0xB8000;
    int i;
    
    pstScreen += ( iY * 80 ) + iX;
    
    for( i = 0 ; pcString[ i ] != 0 ; i++ )
    {
        pstScreen[ i ].bCharactor = pcString[ i ];
    }
}

BOOL kInitializeKernel64Area( void )
{
    DWORD* pdwCurrentAddress;

    pdwCurrentAddress = ( DWORD* ) 0x100000;

    while( ( DWORD ) pdwCurrentAddress < 0x600000 )
    {
        *pdwCurrentAddress = 0x00;

        if( *pdwCurrentAddress != 0 )
        {
            return FALSE;
        }

        pdwCurrentAddress++;
    }
    return TRUE;
}

BOOL kIsMemoryEnough( void )
{
    DWORD* pdwCurrentAddress;

    pdwCurrentAddress = ( DWORD* ) 0x100000;

    while( ( DWORD ) pdwCurrentAddress < 0x4000000 )
    {
        *pdwCurrentAddress = 0x12345678;

        if( *pdwCurrentAddress != 0x12345678 )
        {
            return FALSE;
        }

        pdwCurrentAddress += ( 0x100000 / 4 );
    }
    return TRUE;
}