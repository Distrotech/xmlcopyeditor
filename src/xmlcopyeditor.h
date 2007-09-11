/* 
 * Copyright 2005-2007 Gerald Schmidt.
 * 
 * This file is part of Xml Copy Editor.
 * 
 * Xml Copy Editor is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 * 
 * Xml Copy Editor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Xml Copy Editor; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef XMLCOPYEDITOR_H
#define XMLCOPYEDITOR_H

#define NEWFINDREPLACE 1

#include <wx/wx.h>
#include <wx/fdrepdlg.h>
#include <wx/filename.h>
#include <wx/wxhtml.h>
#include <wx/image.h>
#include <wx/imagpng.h>
#include <wx/utils.h>
#include <wx/docview.h>
#include <wx/propdlg.h>
#include <wx/generic/propdlg.h>
#include <wx/sysopt.h>
#include <wx/datetime.h>
#include <wx/log.h>
#include <wx/clipbrd.h>
#include <wx/strconv.h>
#include <wx/html/helpctrl.h>
#include <wx/snglinst.h>
#include <wx/ipc.h>
#include <wx/intl.h>
#include <utility>
#include <string>
#include <set>
#include <map>
#include <memory>
#include <vector>
#include <stdexcept>
#include "xmldoc.h"
#include "myhtmlpane.h"
#include "xmlencodinghandler.h"
#include "myipc.h"
#include "xmlcopyeditorcopy.h"
#include "mytoolbar.h"
#include "mymenubar.h"
#include <wx/aui/framemanager.h>

#ifdef __WXMSW__
#include "wxcoolbar.h"
#endif

enum
{
    STATUS_HIDDEN = 1,
    STATUS_PROTECTED,
    STATUS_MODIFIED,
    STATUS_POSITION,
    ID_TOOLBAR = wxID_HIGHEST + 1,
    ID_XML_TOOLBAR,
    ID_NOTEBOOK,
    ID_LOCATION_PANEL,
    ID_INSERT_CHILD_PANEL,
    ID_INSERT_SIBLING_PANEL,
    ID_INSERT_ENTITY_PANEL,
    ID_FIND_REPLACE_PANEL,
    ID_FIND_PANEL,
    ID_COMMAND,
    ID_VALIDATION_PANE,
    ID_LOCATION_PANE_VISIBLE,
    ID_PREVIOUS_DOCUMENT,
    ID_NEXT_DOCUMENT,
    ID_OPTIONS,
    ID_IMPORT_MSWORD,
    ID_HIDE_PANE,
    ID_HOME,
    ID_PASTE_NEW_DOCUMENT,
    ID_PRINT_SETUP,
    ID_FEEDBACK,
    ID_TOOLBAR_VISIBLE,
    ID_PROTECT_TAGS,
    ID_SHOW_TAGS,
    ID_HIDE_ATTRIBUTES,
    ID_HIDE_TAGS,
    ID_REPLACE,
    ID_GLOBAL_REPLACE,
    ID_COLOR_SCHEME_DEFAULT,
    ID_COLOR_SCHEME_DEFAULT_BACKGROUND,
    ID_COLOR_SCHEME_REDUCED_GLARE,
    ID_COLOR_SCHEME_NONE,
    ID_NOTEBOOK_STYLE_FLAT,
    ID_NOTEBOOK_STYLE_VC8,
    ID_NOTEBOOK_STYLE_VC8_COLOR,
    ID_DOWNLOAD_SOURCE,
    ID_OPEN_LARGE_FILE,
    ID_WRAP_WORDS,
    // IDs to be activated only if a document is open
    ID_FIND,
    ID_FIND_AGAIN,
    ID_GOTO,
    ID_PRINT,
    ID_WORD_COUNT,
    ID_PRINT_PREVIEW,
    ID_INSERT_CHILD,
    ID_INSERT_SIBLING,
    ID_INSERT_TWIN,
    ID_INSERT_ENTITY,
    ID_INSERT_SYMBOL,
    ID_CHECK_WELLFORMED,
    ID_VALIDATE_DTD,
    ID_VALIDATE_RELAX_NG,
    ID_VALIDATE_W3C_SCHEMA,
    ID_XPATH,
    ID_XSLT,
    ID_XSLT_TEI_FO,
    ID_XSLT_TEI_HTML,
    ID_XSLT_TEI_XHTML,
    ID_XSLT_TEI_LATEX,
    ID_XSLT_DOCBOOK_FO,
    ID_XSLT_DOCBOOK_HTML,
    ID_XSLT_DOCBOOK_XHTML,
    ID_XSLT_DOCBOOK_WORDML,
    ID_XSLT_WORDML_DOCBOOK,
    ID_ASSOCIATE_DTD_PUBLIC,
    ID_ASSOCIATE_DTD_SYSTEM,
    ID_ASSOCIATE_W3C_SCHEMA,
    ID_ASSOCIATE_W3C_SCHEMA_NS,
    ID_ASSOCIATE_XSL,
    ID_PRETTYPRINT,
    ID_ENCODING,
    ID_SPELL,
    ID_FONT_SMALLER,
    ID_FONT_NORMAL,
    ID_FONT_LARGER,
    ID_BROWSER,
    ID_TOGGLE_FOLD,
    ID_FOLD_ALL,
    ID_UNFOLD_ALL,
    ID_VALIDATE_PRESET1,
    ID_VALIDATE_PRESET2,
    ID_VALIDATE_PRESET3,
    ID_VALIDATE_PRESET4,
    ID_VALIDATE_PRESET5,
    ID_VALIDATE_PRESET6,
    ID_VALIDATE_PRESET7,
    ID_VALIDATE_PRESET8,
    ID_VALIDATE_PRESET9,
    ID_EXPORT_MSWORD,
    // icon constants
    CONST_WARNING,
    CONST_STOP,
    CONST_INFO,
    CONST_QUESTION
};

class MyApp : public wxApp
{
public:
    MyApp();
    ~MyApp();
    virtual bool OnInit();
    virtual void OnUnhandledException();
    virtual bool OnExceptionInMainLoop();
#ifndef __WXMSW__
    virtual void HandleEvent ( wxEvtHandler *handler, wxEventFunction func, wxEvent& event ) const;
#endif
protected:
    wxLocale myLocale;
private:
    wxSingleInstanceChecker *checker;
    MyServer *server;
    MyClient *client;
    MyClientConnection *connection;
    bool singleInstanceCheck;
    int lang;
    std::auto_ptr<wxConfig> config;
};

// forward declarations
class MyNotebook;
class wxAuiNotebookEvent;
class LocationPanel;
class InsertPanel;
class CommandPanel;

#ifdef NEWFINDREPLACE
class FindReplacePanel;
#endif

class MyFrame : public wxFrame
{
public:
    MyFrame (
        const wxString& title,
        wxConfig *configParameter,
        wxLocale& locale,
        bool singleInstanceCheck,
        int langParameter );
    ~MyFrame();
    void OnActivateApp ( wxActivateEvent& event );
    void OnAbout ( wxCommandEvent& event );
    void OnCheckWellformedness ( wxCommandEvent& event );
    void OnClose ( wxCommandEvent& event );
    void OnCloseAll ( wxCommandEvent& event );
    void OnClosePane ( wxCommandEvent& event );
    void OnCut ( wxCommandEvent& event );
    void OnCopy ( wxCommandEvent& event );
    void OnPaste ( wxCommandEvent& event );
    void OnPasteNewDocument ( wxCommandEvent& event );
    void OnPrintSetup ( wxCommandEvent &event );
    void OnPrintPreview ( wxCommandEvent &event );
    void OnPrint ( wxCommandEvent &event );
    void OnFind ( wxCommandEvent& event );
    void OnFindAgain ( wxCommandEvent& event );
    void OnFindReplace ( wxCommandEvent& event );
    void OnCommand ( wxCommandEvent& event );
    void OnGlobalReplace ( wxCommandEvent& event );
    void OnWordCount ( wxCommandEvent& event );
    void OnFeedback ( wxCommandEvent& event );
    void OnFontSmaller ( wxCommandEvent& event );
    void OnFontMedium ( wxCommandEvent& event );
    void OnFontLarger ( wxCommandEvent& event );
    void OnImportMSWord ( wxCommandEvent& event );
    void OnInsertChild ( wxCommandEvent& event );
    void OnInsertSibling ( wxCommandEvent& event );
    void OnInsertTwin ( wxCommandEvent& event );
    void OnInsertEntity ( wxCommandEvent& event );
    void OnInsertSymbol ( wxCommandEvent& event );
    void OnExportMSWord ( wxCommandEvent& event );
    void OnBrowser ( wxCommandEvent& event );
    void OnHelp ( wxCommandEvent& event );
    void OnGoto ( wxCommandEvent& event );
    void OnIconize ( wxIconizeEvent& event );
    void OnNew ( wxCommandEvent& event );
    void OnOpen ( wxCommandEvent& event );
    void OnPrettyPrint ( wxCommandEvent& event );
    void OnEncoding ( wxCommandEvent& event );
    void OnQuit ( wxCommandEvent& event );
    void OnSave ( wxCommandEvent& event );
    void OnSaveAs ( wxCommandEvent& event );
    void OnUndo ( wxCommandEvent& event );
    void OnRedo ( wxCommandEvent& event );
    void OnSpelling ( wxCommandEvent& event );
    void OnPreviousDocument ( wxCommandEvent& event );
    void OnNextDocument ( wxCommandEvent& event );
    void OnOptions ( wxCommandEvent& event );
    void OnHistoryFile ( wxCommandEvent& event );
    void OnDialogFind ( wxFindDialogEvent& event );
    void OnDialogReplace ( wxFindDialogEvent& event );
    void OnDialogReplaceAll ( wxFindDialogEvent& event );
    void OnFrameClose ( wxCloseEvent& event );
    void OnIdle ( wxIdleEvent& event );
    void OnUpdateClosePane ( wxUpdateUIEvent& event );
    void OnUpdateCloseAll ( wxUpdateUIEvent& event );
    void OnUpdateSaveUndo ( wxUpdateUIEvent& event );
    void OnUpdatePreviousDocument ( wxUpdateUIEvent& event );
    void OnUpdateSavedOnly ( wxUpdateUIEvent& event );
    void OnUpdateNextDocument ( wxUpdateUIEvent& event );
    void OnUpdateRedo ( wxUpdateUIEvent& event );
    void OnUpdateCutCopy ( wxUpdateUIEvent& event );
    void OnUpdateFindAgain ( wxUpdateUIEvent& event );
    void OnUpdatePaste ( wxUpdateUIEvent& event );
    void OnUpdateDocRange ( wxUpdateUIEvent& event );
    void OnUpdateReplaceRange ( wxUpdateUIEvent& event );
    void OnUpdateLocationPaneVisible ( wxUpdateUIEvent& event );
    void OnValidateDTD ( wxCommandEvent& event );
    void OnValidateRelaxNG ( wxCommandEvent& event );
    void OnValidateSchema ( wxCommandEvent& event );
    void OnXPath ( wxCommandEvent& event );
    void OnXslt ( wxCommandEvent& event );
    void OnValidatePreset ( wxCommandEvent& event );
    void OnHome ( wxCommandEvent& event );
    void OnDownloadSource ( wxCommandEvent& event );
    void OnKeyPressed ( wxKeyEvent& event );
    void OnToolbarVisible ( wxCommandEvent& event );
    void OnLocationPaneVisible ( wxCommandEvent& event );
    void OnProtectTags ( wxCommandEvent& event );
    void OnVisibilityState ( wxCommandEvent& event );
    void OnColorScheme ( wxCommandEvent& event );
    void OnAssociate ( wxCommandEvent& event );
    void OnPageClosing ( wxAuiNotebookEvent& event );
    void OnToggleFold ( wxCommandEvent& event );
    void OnFoldAll ( wxCommandEvent& event );
    void OnUnfoldAll ( wxCommandEvent& event );
    void OnRevert ( wxCommandEvent& event );
    void OnWrapWords ( wxCommandEvent& event );
#ifdef __WXMSW__
    void OnDropFiles ( wxDropFilesEvent& event );
#endif
    void setStrictScrolling ( bool b );
    void validateRelaxNG (
        XmlDoc *doc,
        const wxString& schemaName,
        wxString& fileName );
    void navigate ( const wxString& url );
    void closePane();
    void closeFindReplacePane();
    void closeCommandPane();
    bool closeActiveDocument();
    bool getHandleCommandLineFlag();

    // public to allow access outside MyFrame constructor
    void handleCommandLine();
    MyNotebook *getNotebook()
    {
        return mainBook;
    }

    // public to allow access from CommandPanel
    XmlDoc *getActiveDocument();
    void newDocument ( const wxString& s, const wxString& path = wxEmptyString, bool canSave = false );
    void newDocument ( const std::string& s, const std::string& path = "", bool canSave = false );
    void statusProgress ( const wxString& s );

    // public to allow InsertPanel access
    void messagePane ( const wxString& s, int iconType = CONST_INFO );

    // public to allow IPC access
    bool openFile ( wxString& fileName, bool largeFile = false );
    bool isOpen ( const wxString& fileName );
    void activateTab ( const wxString& fileName );
private:
    wxAuiManager manager;
    wxConfig *config; // owned by MyApp
    wxLocale& myLocale;
    bool singleInstanceCheck;
    int lang, lastPos;
    wxLogNull logTarget;
    std::auto_ptr<wxHtmlEasyPrinting> htmlPrinting;
    std::auto_ptr<wxFindReplaceDialog> findDialog;
    std::auto_ptr<wxHtmlHelpController> helpController;

    wxBoxSizer *frameSizer;
    MyMenuBar *menuBar;
    MyToolBar *toolBar;
    LocationPanel *locationPanel;
    InsertPanel *insertChildPanel, *insertSiblingPanel, *insertEntityPanel;

#ifdef NEWFINDREPLACE
    FindReplacePanel *findReplacePanel;
#endif
    CommandPanel *commandPanel;

    XmlDoc *lastDoc;
    wxMenu *fileMenu, *xmlMenu, *viewMenu, *colorSchemeMenu;
    std::vector<wxMenu *> menuVector;
    MyNotebook *mainBook;
    MyHtmlPane *htmlReport;

    std::string catalogPath, xslDtdPath, rssDtdPath, lzxDtdPath, xtmDtdPath,
    xliffDtdPath;
    std::pair<int, int> controlCoordinates;
    std::map<std::string, std::map<std::string, std::set<std::string> > >
        promptMap;
    std::map<int, wxString> validationPresetMap;
    std::set<wxString> openFileSet;
    std::set<wxString> openLargeFileSet;
    std::vector<wxString> tempFileVector;
    int documentCount,
    framePosX,
    framePosY,
    frameWidth,
    frameHeight,
    notebookStyle,
    visibilityState,
    commandOutput;
    wxPoint stylePosition, aboutPosition;
    wxSize styleSize;
    wxString applicationDir,
    ruleSetPreset,
    filterPreset,
    ruleSetDir,
    filterDir,
    binDir,
    templateDir,
    helpDir,
    rngDir,
    htmlDir,
    pngDir,
    xpathExpression,
    lastDtdPublic,
    lastDtdSystem,
    lastSchema,
    lastSchemaNamespace,
    lastXslStylesheet,
    lastSchemaNamespaceAux,
    lastRelaxNGSchema,
    lastDtdPublicAux,
    openTabsOnClose,
    browserCommand,
    layout,
    defaultLayout,
    lastParent,
    lastGrandparent,
    commandString;
    //wxTextCtrl *stderrControl;
    //wxStreamToTextRedirector *redirector;
    bool globalReplaceAllDocuments,
    toolbarVisible,
    protectTags,
    handleCommandLineFlag,
    rememberOpenTabs,
    libxmlNetAccess,
    deletePageVetoed,
    saveBom,
    unlimitedUndo,
#ifdef __WXMSW__
    useCoolBar,
    useCoolBarOnStart,
#endif
    restoreLayout,
    showLocationPane,
    showInsertChildPane,
    showInsertSiblingPane,
    showInsertEntityPane,
    expandInternalEntities,
    validateAsYouType,
    restoreFocusToNotebook,
    showFullPathOnFrame,
    findRegex,
    commandSync;
    wxBitmap newBitmap,
    new16Bitmap,
    openBitmap,
    open16Bitmap,
    searchBitmap,
    saveBitmap,
    saveGrBitmap,
    save16Bitmap,
    printPreviewBitmap,
    printBitmap,
    print16Bitmap,
    undoBitmap,
    undo16Bitmap,
    redoBitmap,
    redo16Bitmap,
    cutBitmap,
    copyBitmap,
    pasteBitmap,
    findBitmap,
    spellingBitmap,
    spellingGrBitmap,
    spelling16Bitmap,
    helpBitmap,
    internetBitmap,
    internetGrBitmap,
    hyperlinkBitmap,
    filtersBitmap;
    wxFileHistory history;
    wxFindReplaceData findData;
    XmlCtrlProperties properties, largeFileProperties;
#ifdef __WXMSW__
    wxCoolBar *coolBar;
#endif

    // member functions
    bool panelHasFocus();
    bool saveFile (
        XmlDoc *doc,
        wxString& fileName,
        bool checkLastModified = true );
    bool xpThemeActive();
    int getFileType ( const wxString& fileName );
    long getNotebookStyleMask();
    wxString getLinuxBrowser();
    bool isSpecialFileType ( const wxString& fileName );
    wxString getHtmlBuffer();
    void encodingMessage();
    void save();
    void saveAs();
    void displaySavedStatus ( int bytes );
    void addSafeSeparator ( wxToolBar *toolBar );
    void findAgain ( wxString s, int flags );
    void updateFileMenu ( bool deleteExisting = true );
    void documentOk ( const wxString& status );
    void applyEditorProperties ( bool zoomOnly = false );
    void xmliseWideTextNode ( wxString& s );
    void updatePaths();
    void importMSWord ( const wxString& path );
    void showTopBars ( bool b );
    void modifiedMessage();
    void loadBitmaps();
    void openRememberedTabs();
    void getRawText ( XmlDoc *doc, std::string& buffer );
    void updateToolbar();
    std::string getApproximateEncoding ( char *docBuffer, size_t docBufferLen );
    bool saveRawUtf8 (
        const std::string& fileNameLocal,
        std::string& bufferUtf8,
        bool ignoreEncoding = false,
        bool isXml = true );
    void removeUtf8Bom ( std::string& buffer );
    std::string getAuxPath ( const std::string& fileName );
    MyMenuBar *getMenuBar();
    MyToolBar *getToolBar();

    DECLARE_EVENT_TABLE()
};

#endif

