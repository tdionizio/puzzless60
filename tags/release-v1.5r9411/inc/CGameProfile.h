
#ifndef __PUZZLESCGAMEPROFILE_H__
#define __PUZZLESCGAMEPROFILE_H__

#include <e32base.h>

class CAppConfig;

class CGameProfile : CBase {
    friend class CAppConfig;
public:
    CGameProfile();
    ~CGameProfile();

    void ConstructL(char *aName, char *aParams);
    void ConstructL(TInt aId, char *aName, char *aParams);
    
    TInt Id() { return iId; }
    char *Name() { return iName; }
    char *Params() { return iParams; }
    
private:
    TInt iId;
    char *iName;
    char *iParams;
};


#endif /* __PUZZLESCGAMEPROFILE_H__ */
