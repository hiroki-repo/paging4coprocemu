// dllmain.cpp : DLL アプリケーションのエントリ ポイントを定義します。
#include "pch.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

UINT32 pagetableptr;
UINT32 tlb[1048576];
UINT32(*memaccess)(UINT32, UINT32, UINT32) = 0;
void(**machandler)(UINT32);
UINT32 v4ret = 0;
UINT32 vfromret = 0;
UINT32 addr4pf = 0;
UINT32 resonofpf = 0;
bool v4retset = false;

#ifdef __cplusplus
extern "C" {
#endif
    __declspec(dllexport) void setmemaccesshandler_p4cop(UINT32(*prm_0)(UINT32, UINT32, UINT32), void(**prm_1)(UINT32)) { memaccess = prm_0; machandler = prm_1; v4retset = false; return; }
    __declspec(dllexport) UINT32 mainmemaccess_p4cop(UINT32 prm_0, UINT32 prm_1, UINT32 prm_2) {
        switch (prm_2 & 1) {
        case 0:
            switch (prm_0 & 0xFF) {
            case 0x00:
            case 0x01:
            case 0x02:
            case 0x03:
                pagetableptr = (pagetableptr & ~(0xFF << ((prm_0 & 3) * 8))) | ((prm_1 & 0xFF) << ((prm_0 & 3) * 8));
                break;
            case 0x04:
            case 0x05:
            case 0x06:
            case 0x07:
                for (int cnt = 0; cnt < 1048576; cnt++) {
                    UINT32 tlbfst = ((memaccess(pagetableptr + (4 * ((cnt >> 10) & 0x3FF)) + 0, 0, 1) & 0xFF) << (8 * 0)) | ((memaccess(pagetableptr + (4 * ((cnt >> 10) & 0x3FF)) + 1, 0, 1) & 0xFF) << (8 * 1)) | ((memaccess(pagetableptr + (4 * ((cnt >> 10) & 0x3FF)) + 2, 0, 1) & 0xFF) << (8 * 2)) | ((memaccess(pagetableptr + (4 * ((cnt >> 10) & 0x3FF)) + 3, 0, 1) & 0xFF) << (8 * 3));
                    if (tlbfst & 1) {
                        tlb[cnt] = ((memaccess((tlbfst & 0xFFFFF000) + (4 * ((cnt >> 0) & 0x3FF)) + 0, 0, 1) & 0xFF) << (8 * 0)) | ((memaccess((tlbfst & 0xFFFFF000) + (4 * ((cnt >> 0) & 0x3FF)) + 1, 0, 1) & 0xFF) << (8 * 1)) | ((memaccess((tlbfst & 0xFFFFF000) + (4 * ((cnt >> 0) & 0x3FF)) + 2, 0, 1) & 0xFF) << (8 * 2)) | ((memaccess((tlbfst & 0xFFFFF000) + (4 * ((cnt >> 0) & 0x3FF)) + 3, 0, 1) & 0xFF) << (8 * 3));
                    }
                }
                v4retset = true;
                break;
            case 0x08:
            case 0x09:
            case 0x0a:
            case 0x0b:
                v4ret = (v4ret & ~(0xFF << ((prm_0 & 3) * 8))) | ((prm_1 & 0xFF) << ((prm_0 & 3) * 8));
                break;
            }
            break;
        case 1:
            switch (prm_0 & 0xFF) {
            case 0x00:
            case 0x01:
            case 0x02:
            case 0x03:
                return (pagetableptr >> ((prm_0 & 3) * 8)) & 0xFF;
                break;
            case 0x04:
            case 0x05:
            case 0x06:
            case 0x07:
                return (addr4pf >> ((prm_0 & 3) * 8)) & 0xFF;
                break;
            case 0x08:
            case 0x09:
            case 0x0a:
            case 0x0b:
                return (vfromret >> ((prm_0 & 3) * 8)) & 0xFF;
                break;
            case 0x0c:
            case 0x0d:
            case 0x0e:
            case 0x0f:
                return (resonofpf >> ((prm_0 & 3) * 8)) & 0xFF;
                break;
            }
            break;
        }
        return 0xFF;
    }
    __declspec(dllexport) UINT32 copmemaccess_p4cop(UINT32 prm_0, UINT32 prm_1, UINT32 prm_2) {
        switch (prm_2 & 3) {
        case 0:
            if ((tlb[(prm_0 >> 12) & 0xFFFFF] & 1) && (tlb[(prm_0 >> 12) & 0xFFFFF] & 4)) { return memaccess((tlb[(prm_0 >> 12) & 0xFFFFF] & 0xFFFFF000) + (prm_0 & 0xFFF), prm_1, prm_2); }
            break;
        case 1:
            if ((tlb[(prm_0 >> 12) & 0xFFFFF] & 1) && (tlb[(prm_0 >> 12) & 0xFFFFF] & 2)) { return memaccess((tlb[(prm_0 >> 12) & 0xFFFFF] & 0xFFFFF000) + (prm_0 & 0xFFF), prm_1, prm_2); }
            break;
        }
        resonofpf = prm_2; vfromret = prm_1; addr4pf = prm_0; v4retset = false; machandler[0](0); v4retset = false; return v4ret;
    }
    __declspec(dllexport) bool isv4retset_p4cop() { return v4retset; }
#ifdef __cplusplus
}
#endif

