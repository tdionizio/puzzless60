
#ifndef __PUZZLESCAPPCONFIG_H__
#define __PUZZLESCAPPCONFIG_H__

#include <e32base.h>
#include <d32dbms.h>    // RDbStoreDatabase
#include <f32file.h>    // RFs

class CFileStore;

typedef struct midend midend;
typedef struct game game;

/**
 * Application Config
 * - screen mode [OK]
 * - auto save last game?
 * - screen orientation
 * ->last game [OK]
 * 
 * Last saved games [OK]
 * - gamename|gamesave|time
 * 
 * Profiles
 * - gamename|name|params
 * 
 * Save games
 * - gamename|gamesave|time
 * 
 */

class CAppConfig : CBase {

public:
    CAppConfig();
    ~CAppConfig();
    void ConstructL();
    
    TInt GetScreenMode(TInt aDefaultScreenMode);
    void SetScreenMode(TInt aScreenMode);
    
    TInt GetScreenOrientation(TInt aDefaultScreenOrientation);
    void SetScreenOrientation(TInt aScreenOrientation);
    
    void GetLastSelectedGame(TDes8 &aGameName);
    void SetLastSelectedGame(const char *aGameName);
    
    void SaveGame(midend *aME, const game *aGame);
    char* LoadGame(midend *aME, const game *aGame);
    
private:
    RFs iFsSession;
    RDbStoreDatabase iDb;
    CFileStore *iFileStore;
    
    void OpenL(const TFileName &aConfigFile);
    void Close();
    void CreateL(const TFileName &aConfigFile);
    void CheckConfigTableL();
    void CheckAutosaveTableL();
    
    typedef enum {
        EConfigBool,
        EConfigInt,
        EConfigText,
        EConfigText8
    } TConfigType;
    typedef union {
        TBool *iBool;
        TInt *iInt;
        TDes *iText;
        TDes8 *iText8;
        TAny *iPtr;
    } TConfigValue;
    
    class TConfig {
    public:
        TConfigType iType;
        TConfigValue iValue;
        
        TConfig(TConfigType aType, TAny *aValue) {
            iType = aType;
            iValue.iPtr = aValue;
        }
    };
    
    void GetConfig(const TDesC &aKey, const TConfig &aConfig);
    void SetConfig(const TDesC &aKey, const TConfig &aConfig);
    
    void GetConfigL(const TDesC &aKey, const TConfig &aConfig);
    void SetConfigL(const TDesC &aKey, const TConfig &aConfig);
    

    void SaveGameL(midend *aME, const game *aGame);
    char* LoadGameL(midend *aME, const game *aGame);
};


#endif /* __PUZZLESCAPPCONFIG_H__ */
