
#include "CGameProfile.h"

extern "C" {
#include "puzzles.h"
}

#include "_puzzles.inc.h"


CGameProfile::CGameProfile() {
    iName = NULL;
    iParams = NULL;
}

CGameProfile::~CGameProfile() {
    sfree(iName);
    sfree(iParams);
}

void CGameProfile::ConstructL(char *aName, char *aParams) {
    ConstructL(-1, aName, aParams);
}

void CGameProfile::ConstructL(TInt aId, char *aName, char *aParams) {
    sfree(iName);
    sfree(iParams);
    
    iId = aId;
    iName = aName;
    iParams = aParams;
}
