#ifdef	__MODESWITCH_H__
#define	__MODESWITCH_H__

#include "Types.h"

void kReadCPUID( DWORD dwEax, DWORD* pdwEAX, DWORD* pdwEBX, DWORD* pdwECX, DWORD* pdwEDX );
void kSwitchAndExecute64bitKernel( void );

#endif