
#include "CAppConfig.h"
#include "CGameProfile.h"

extern "C" {
#include "puzzles.h"
}

#include <bautils.h>
#include <s32file.h>    // CFileStore & CPermanentFileStore

#include "_puzzles.inc.h"


CAppConfig::CAppConfig() {
}

CAppConfig::~CAppConfig() {
    Close();
    iFsSession.Close();
}

void CAppConfig::ConstructL() {
    User::LeaveIfError(iFsSession.Connect());
    
    TFileName configFile(KConfigFile);
    TFileName folder(BaflUtils::DriveAndPathFromFullName(configFile));
    
    if (!BaflUtils::FolderExists(iFsSession, folder)) {
        User::LeaveIfError(iFsSession.MkDirAll(folder));
    }
    
    // attempt to open DB
    TRAPD(error, OpenL(configFile));
    
    // if it fails, create and open again
    if (error != KErrNone) {
        Close();
        CreateL(configFile);
        OpenL(configFile);
    }
    
    CheckConfigTableL();
    CheckAutosaveTableL();
}

void CAppConfig::OpenL(const TFileName &aConfigFile) {
    iFileStore = CPermanentFileStore::OpenL(
        iFsSession,
        aConfigFile, 
        EFileRead | EFileWrite
    );
    iFileStore->SetTypeL(iFileStore->Layout());/* Set file store type*/
    iDb.OpenL(iFileStore, iFileStore->Root());
}

void CAppConfig::CreateL(const TFileName &aConfigFile) {
    iFileStore = CPermanentFileStore::ReplaceL(
        iFsSession,
        aConfigFile, 
        EFileRead | EFileWrite
    );
    iFileStore->SetTypeL(iFileStore->Layout());// Set file store type
    TStreamId id = iDb.CreateL(iFileStore);// Create stream object
    iFileStore->SetRootL(id);// Keep database id as root of store
    iFileStore->CommitL();// Complete creation by commiting
    Close();
}

void CAppConfig::Close() {
    iDb.Close();
    if (iFileStore) {
        delete iFileStore;
        iFileStore = NULL;
    }
}

/**********************************************************
 * Config Table
 *      key|value
 */

enum {
    KConfigKeyMaxLength = 20,
    KConfigValueMaxLength = 20,
};
_LIT(KConfigTable, "config");
_LIT(KConfigIndex, "configKeyIdx");
_LIT(KConfigKeyCol, "key");
_LIT(KConfigValueCol, "value");

void CAppConfig::CheckConfigTableL() {
    CDbColSet *colSet = NULL;
    TInt error;
    
    TRAP(error, colSet = iDb.ColSetL(KConfigTable));
    
    if (colSet != NULL) {
        TBool drop =
                colSet->Count() != 2
                || colSet->ColNo(KConfigKeyCol) == KDbNullColNo
                || colSet->ColNo(KConfigValueCol) == KDbNullColNo
        ;
        
        delete colSet;
        
        if (drop) {
            User::LeaveIfError(iDb.DropTable(KConfigTable));
            colSet = NULL;
        }
    }
    
    if (colSet == NULL) {
        // Specify columns for config table
        TDbCol keyCol(KConfigKeyCol, EDbColText, KConfigKeyMaxLength);
        TDbCol valueCol(KConfigValueCol, EDbColText, KConfigValueMaxLength);
        keyCol.iAttributes = TDbCol::ENotNull;
        valueCol.iAttributes = TDbCol::ENotNull;
        
        // Add the columns to column set
        colSet = CDbColSet::NewLC();
        colSet->AddL(keyCol);
        colSet->AddL(valueCol);
        
        // Create the Books table
        User::LeaveIfError(iDb.CreateTable(KConfigTable, *colSet));
        CleanupStack::PopAndDestroy(colSet);
    }
    
    CDbKey *idx = NULL;
    TRAP(error, idx = iDb.KeyL(KConfigIndex, KConfigTable));
    if (idx != NULL) {
        if (!idx->IsUnique()) {
            iDb.DropIndex(KConfigIndex, KConfigTable);
            delete idx;
            idx = NULL;
        } else {
            delete idx;
        }
    }
    if (idx == NULL) {
        TDbKeyCol keyCol(KConfigKeyCol);
        idx = CDbKey::NewLC();
        idx->MakeUnique();
        idx->AddL(keyCol);
        User::LeaveIfError(iDb.CreateIndex(KConfigIndex, KConfigTable, *idx));
        CleanupStack::PopAndDestroy(idx);
    }
}

void CAppConfig::GetConfig(const TDesC &aKey, const TConfig &aConfig) {
    TRAPD(error, GetConfigL(aKey, aConfig));
    User::LeaveIfError(error);
}
void CAppConfig::GetConfigL(const TDesC &aKey, const TConfig &aConfig) {
    RDbTable table;
    TDbSeekKey seekKey(aKey);
    
    User::LeaveIfError(table.Open(iDb, KConfigTable, table.EReadOnly));

    CleanupClosePushL(table);

    User::LeaveIfError(table.SetIndex(KConfigIndex));
    
    CDbColSet* colSet = table.ColSetL();
    CleanupStack::PushL(colSet);
    
    TInt valueColNo = colSet->ColNo(KConfigValueCol);
    
    if (table.SeekL(seekKey)) {
        table.GetL();
        TPtrC value = table.ColDes(valueColNo);
        
        switch (aConfig.iType) {
            case EConfigBool:
                *aConfig.iValue.iBool = value.Compare(KBoolTrue) == 0 ? ETrue : EFalse;
                break;
                
            case EConfigInt: {
                TLex lex(value);
                TInt lvalue;
                if (lex.Val(lvalue) == KErrNone) {
                    *aConfig.iValue.iInt = lvalue;
                }
                break;
            }
            
            case EConfigText:
                if (aConfig.iValue.iText->MaxLength() > value.Length()) {
                    aConfig.iValue.iText->Copy(value.Left(aConfig.iValue.iText->MaxLength()));
                } else {
                    aConfig.iValue.iText->Copy(value);
                }
                break;
                
            case EConfigText8:
                if (aConfig.iValue.iText8->MaxLength() > value.Length()) {
                    aConfig.iValue.iText8->Copy(value.Left(aConfig.iValue.iText8->MaxLength()));
                } else {
                    aConfig.iValue.iText8->Copy(value);
                }
                break;
        }
    }
    
    CleanupStack::PopAndDestroy(colSet);
    CleanupStack::PopAndDestroy(&table);
}

void CAppConfig::SetConfig(const TDesC &aKey, const TConfig &aConfig) {
    TRAPD(error, SetConfigL(aKey, aConfig));
    User::LeaveIfError(error);
}
void CAppConfig::SetConfigL(const TDesC &aKey, const TConfig &aConfig) {
    TBuf<KConfigValueMaxLength> value;
    switch (aConfig.iType) {
        case EConfigBool:
            if (*aConfig.iValue.iBool) {
                value.Copy(KBoolTrue);
            } else {
                value.Copy(KBoolFalse);
            }
            break;
        case EConfigInt:
            value.Num(*aConfig.iValue.iInt);
            break;
        case EConfigText:
            value.Copy(*aConfig.iValue.iText);
            break;
        case EConfigText8:
            value.Copy(*aConfig.iValue.iText8);
            break;
    }
    
    RDbTable table;
    TDbSeekKey seekKey(aKey);
    
    User::LeaveIfError(table.Open(iDb, KConfigTable, table.EUpdatable));
    
    CleanupClosePushL(table);
    
    User::LeaveIfError(table.SetIndex(KConfigIndex));
    
    CDbColSet* colSet = table.ColSetL();
    CleanupStack::PushL(colSet);
    
    TInt keyColNo = colSet->ColNo(KConfigKeyCol);
    TInt valueColNo = colSet->ColNo(KConfigValueCol);
    
    if (table.SeekL(seekKey)) {
        table.GetL();
        table.UpdateL();
        table.SetColL(valueColNo, value);
    } else {
        table.InsertL();
        table.SetColL(keyColNo, aKey);
        table.SetColL(valueColNo, value);
    }
    table.PutL();
    
    CleanupStack::PopAndDestroy(colSet);
    CleanupStack::PopAndDestroy(&table);
}

/********************************************************
 * Get/Set configuration options
 */
_LIT(KCfgScreenMode, "ScreenMode");
_LIT(KCfgScreenOrientation, "ScreenOrientation");
_LIT(KCfgLastSelectedGame, "LastSelectedGame");

TInt CAppConfig::GetScreenMode(TInt aDefaultScreenMode) {
    TInt screenMode = aDefaultScreenMode;
    GetConfig(KCfgScreenMode, TConfig(EConfigInt, &screenMode));
    return screenMode;
}
void CAppConfig::SetScreenMode(TInt aScreenMode) {
    SetConfig(KCfgScreenMode, TConfig(EConfigInt, &aScreenMode));
}

TInt CAppConfig::GetScreenOrientation(TInt aDefaultScreenOrientation) {
    TInt screenMode = aDefaultScreenOrientation;
    GetConfig(KCfgScreenOrientation, TConfig(EConfigInt, &screenMode));
    return screenMode;
}
void CAppConfig::SetScreenOrientation(TInt aScreenOrientation) {
    SetConfig(KCfgScreenOrientation, TConfig(EConfigInt, &aScreenOrientation));
}

void CAppConfig::GetLastSelectedGame(TDes8 &aGameName) {
    aGameName.Zero();
    GetConfig(KCfgLastSelectedGame, TConfig(EConfigText8, &aGameName));
}
void CAppConfig::SetLastSelectedGame(const char *aGameName) {
    TPtrC8 name((const TUint8*)aGameName);
    SetConfig(KCfgLastSelectedGame, TConfig(EConfigText8, &name));
}


/********************************************************
 * Autosave
 *      name|save|date
 */

enum {
    KAutosaveNameMaxLength = 20,
};
_LIT(KAutosaveTable, "autosave");
_LIT(KAutosaveIndex, "autosaveNameIdx");
_LIT(KAutosaveNameCol, "name");
_LIT(KAutosaveSaveCol, "save");
_LIT(KAutosaveDateCol, "date");

void CAppConfig::CheckAutosaveTableL() {
    CDbColSet *colSet = NULL;
    TInt error;
    
    TRAP(error, colSet = iDb.ColSetL(KAutosaveTable));
    
    if (colSet != NULL) {
        TBool drop =
                colSet->Count() != 3
                || colSet->ColNo(KAutosaveNameCol) == KDbNullColNo
                || colSet->ColNo(KAutosaveSaveCol) == KDbNullColNo
                || colSet->ColNo(KAutosaveDateCol) == KDbNullColNo
        ;
        
        delete colSet;
        
        if (drop) {
            User::LeaveIfError(iDb.DropTable(KAutosaveTable));
            colSet = NULL;
        }
    }
    
    if (colSet == NULL) {
        // Specify columns for config table
        TDbCol nameCol(KAutosaveNameCol, EDbColText8, KAutosaveNameMaxLength);
        TDbCol saveCol(KAutosaveSaveCol, EDbColLongText8);
        TDbCol dateCol(KAutosaveDateCol, EDbColDateTime);
        
        nameCol.iAttributes = TDbCol::ENotNull;
        saveCol.iAttributes = TDbCol::ENotNull;
        dateCol.iAttributes = TDbCol::ENotNull;
        
        // Add the columns to column set
        colSet = CDbColSet::NewLC();
        colSet->AddL(nameCol);
        colSet->AddL(saveCol);
        colSet->AddL(dateCol);
        
        // Create the Books table
        TInt _err = iDb.CreateTable(KAutosaveTable, *colSet);
        User::LeaveIfError(_err);
        CleanupStack::PopAndDestroy(colSet);
    }
    
    CDbKey *idx = NULL;
    TRAP(error, idx = iDb.KeyL(KAutosaveIndex, KAutosaveTable));
    if (idx != NULL) {
        if (!idx->IsUnique()) {
            iDb.DropIndex(KAutosaveIndex, KAutosaveTable);
            delete idx;
            idx = NULL;
        } else {
            delete idx;
        }
    }
    if (idx == NULL) {
        TDbKeyCol keyCol(KAutosaveNameCol);
        idx = CDbKey::NewLC();
        idx->MakeUnique();
        idx->AddL(keyCol);
        User::LeaveIfError(iDb.CreateIndex(KAutosaveIndex, KAutosaveTable, *idx));
        CleanupStack::PopAndDestroy(idx);
    }
}

static void save_game(void *ctx, void *buf, int len) {
    RDbColWriteStream *s = (RDbColWriteStream*)ctx;
    TPtrC8 p((TUint8*)buf, len);
    TRAPD(error, s->WriteL(p));
}
void CAppConfig::SaveGame(midend *aME, const game *aGame) {
    TRAPD(error, SaveGameL(aME, aGame));
    if (error != KErrNone) {
        RDebug::Printf("Error saving game %s [%d]", aGame->name, error);
    }
}
void CAppConfig::SaveGameL(midend *aME, const game *aGame) {
    TPtrC8 name((const TUint8*)aGame->name);
    RDbTable table;
    TDbSeekKey seekKey(name);
    
    User::LeaveIfError(table.Open(iDb, KAutosaveTable, table.EUpdatable));
    
    CleanupClosePushL(table);
    
    User::LeaveIfError(table.SetIndex(KAutosaveIndex));
    
    CDbColSet* colSet = table.ColSetL();
    CleanupStack::PushL(colSet);
    
    TInt nameColNo = colSet->ColNo(KAutosaveNameCol);
    TInt saveColNo = colSet->ColNo(KAutosaveSaveCol);
    TInt dateColNo = colSet->ColNo(KAutosaveDateCol);
    
    if (table.SeekL(seekKey)) {
        table.GetL();
        table.UpdateL();
    } else {
        table.InsertL();
        table.SetColL(nameColNo, name);
    }
    TTime now;
    now.HomeTime();
    table.SetColL(dateColNo, now);
    
    RDbColWriteStream stream;
    stream.OpenL(table, saveColNo);
    CleanupClosePushL(stream);
    
    midend_serialise(aME, save_game, &stream);
    CleanupStack::PopAndDestroy(&stream);
    
    table.PutL();
    
    CleanupStack::PopAndDestroy(colSet);
    CleanupStack::PopAndDestroy(&table);
    
    iDb.Compact();
}


static int read_game(void *ctx, void *buf, int len) {
    RDbColReadStream *s = (RDbColReadStream*)ctx;
    TPtr8 p((TUint8*)buf, len);
    TRAPD(error, s->ReadL(p));
    return error == KErrNone;
}
char* CAppConfig::LoadGame(midend *aME, const game *aGame) {
    char *errmsg = "";
    TRAPD(error, errmsg = LoadGameL(aME, aGame));
    if (error != KErrNone) {
        RDebug::Printf("Error loading game %s [%d]", aGame->name, error);
    }
    return errmsg;
}
char* CAppConfig::LoadGameL(midend *aME, const game *aGame) {
    TPtrC8 name((const TUint8*)aGame->name);
    RDbTable table;
    TDbSeekKey seekKey(name);
    char *errmsg = "";
    
    User::LeaveIfError(table.Open(iDb, KAutosaveTable, table.EReadOnly));

    CleanupClosePushL(table);

    User::LeaveIfError(table.SetIndex(KAutosaveIndex));
    
    CDbColSet* colSet = table.ColSetL();
    CleanupStack::PushL(colSet);
    
    TInt saveColNo = colSet->ColNo(KAutosaveSaveCol);
    
    if (table.SeekL(seekKey)) {
        table.GetL();
        RDbColReadStream stream;
        stream.OpenLC(table,saveColNo);
        
        errmsg = midend_deserialise(aME, read_game, &stream);
        
        CleanupStack::PopAndDestroy(&stream);
    }
    
    CleanupStack::PopAndDestroy(colSet);
    CleanupStack::PopAndDestroy(&table);
    
    return errmsg;
}


