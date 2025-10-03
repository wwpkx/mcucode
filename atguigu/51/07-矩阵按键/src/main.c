#include "Int_KeyMatrix.h"
#include "Int_DigitalTube.h"

void main()
{
    u8 key;
    Int_DigitalTube_Init();
    while (1) {
        key = Int_KeyMatrix_CheckSW();
        if (key) {
            // ÊýÂë¹Ü
            Int_DigitalTube_DisplayNum(key);
        }
        Int_DigitalTube_Refresh();
    }
}