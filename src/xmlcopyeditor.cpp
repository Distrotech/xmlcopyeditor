#include <iostream>
#include <fstream>
#include <string>
#include <wx/aboutdlg.h>
#include "xmlcopyeditor.h"
#include "readfile.h"
#include "xmldoc.h"
#include "xmlctrl.h"
#include "wraplibxml.h"
#include "xmlschemalocator.h"
#include "xsllocator.h"
#include "xmlutf8reader.h"
#include "xmlpromptgenerator.h"
#include "xmlencodingspy.h"
#include "styledialog.h"
#include "mypropertysheet.h"
#include "wraptempfilename.h"
#include "globalreplacedialog.h"
#include "replace.h"
#include "associatedialog.h"
#include "xmlassociatexsd.h"
#include "xmlassociatexsl.h"
#include "xmlassociatedtd.h"
#include "aboutdialog.h"
#include "pathresolver.h"
#include "locationpanel.h"
#include "insertpanel.h"
#include "xmlwordcount.h"
#include "mynotebook.h"
#include "getlinuxappdir.h"
#include "commandpanel.h"
#include "binaryfile.h"
#include <wx/aui/auibook.h>
#include <wx/richtext/richtextsymboldlg.h>
#include <wx/textctrl.h>
#include <iconv.h>

#define ngettext wxGetTranslation

#ifdef NEWFINDREPLACE
#include "findreplacepanel.h"
#endif

#ifdef __WXMSW__
#include <wx/msw/uxtheme.h>
#else
#include "wrapxerces.h"
#include "xpm/appicon.xpm"
#endif

BEGIN_EVENT_TABLE ( MyFrame, wxFrame )
EVT_ACTIVATE_APP ( MyFrame::OnActivateApp )
EVT_CLOSE ( MyFrame::OnFrameClose )
EVT_KEY_DOWN ( MyFrame::OnKeyPressed )
EVT_MENU ( wxID_ABOUT, MyFrame::OnAbout )
EVT_MENU ( wxID_CLOSE, MyFrame::OnClose )
EVT_MENU ( wxID_CLOSE_ALL, MyFrame::OnCloseAll )
EVT_MENU ( wxID_CUT, MyFrame::OnCut )
EVT_MENU ( wxID_COPY, MyFrame::OnCopy )
EVT_MENU ( wxID_HELP, MyFrame::OnHelp )
EVT_MENU ( wxID_PASTE, MyFrame::OnPaste )
EVT_MENU ( ID_PASTE_NEW_DOCUMENT, MyFrame::OnPasteNewDocument )
EVT_MENU ( wxID_EXIT, MyFrame::OnQuit )
EVT_MENU ( wxID_NEW, MyFrame::OnNew )
EVT_MENU ( wxID_OPEN, MyFrame::OnOpen )
EVT_MENU ( wxID_SAVE, MyFrame::OnSave )
EVT_MENU ( wxID_SAVEAS, MyFrame::OnSaveAs )
EVT_MENU ( wxID_UNDO, MyFrame::OnUndo )
EVT_MENU ( wxID_REDO, MyFrame::OnRedo )
EVT_MENU ( wxID_REVERT, MyFrame::OnRevert )
EVT_MENU ( ID_INSERT_CHILD, MyFrame::OnInsertChild )
EVT_MENU ( ID_INSERT_SIBLING, MyFrame::OnInsertSibling )
EVT_MENU ( ID_INSERT_ENTITY, MyFrame::OnInsertEntity )
EVT_MENU ( ID_INSERT_TWIN, MyFrame::OnInsertTwin )
EVT_MENU ( ID_INSERT_SYMBOL, MyFrame::OnInsertSymbol )
EVT_MENU ( ID_TOGGLE_FOLD, MyFrame::OnToggleFold )
EVT_MENU ( ID_FOLD_ALL, MyFrame::OnFoldAll )
EVT_MENU ( ID_UNFOLD_ALL, MyFrame::OnUnfoldAll )
EVT_MENU ( ID_OPEN_LARGE_FILE, MyFrame::OnOpen )
EVT_MENU ( ID_PRINT_PREVIEW, MyFrame::OnPrintPreview )
EVT_MENU ( ID_PRINT_SETUP, MyFrame::OnPrintSetup )
EVT_MENU ( ID_PRINT, MyFrame::OnPrint )
EVT_MENU ( ID_WORD_COUNT, MyFrame::OnWordCount )
EVT_MENU ( ID_IMPORT_MSWORD, MyFrame::OnImportMSWord )
EVT_MENU ( ID_EXPORT_MSWORD, MyFrame::OnExportMSWord )
EVT_MENU ( ID_HIDE_PANE, MyFrame::OnClosePane )
EVT_MENU ( ID_COMMAND, MyFrame::OnCommand )
EVT_MENU ( ID_FIND, MyFrame::OnFind )
EVT_MENU ( ID_FIND_AGAIN, MyFrame::OnFindAgain )
EVT_MENU ( ID_GOTO, MyFrame::OnGoto )
EVT_MENU ( ID_FEEDBACK, MyFrame::OnFeedback )
EVT_MENU ( ID_PREVIOUS_DOCUMENT, MyFrame::OnPreviousDocument )
EVT_MENU ( ID_NEXT_DOCUMENT, MyFrame::OnNextDocument )
EVT_MENU ( ID_BROWSER, MyFrame::OnBrowser )
EVT_MENU ( ID_REPLACE, MyFrame::OnFindReplace )
EVT_MENU ( ID_GLOBAL_REPLACE, MyFrame::OnGlobalReplace )
EVT_MENU ( ID_CHECK_WELLFORMED, MyFrame::OnCheckWellformedness )
EVT_MENU ( ID_VALIDATE_DTD, MyFrame::OnValidateDTD )
EVT_MENU ( ID_VALIDATE_RELAX_NG, MyFrame::OnValidateRelaxNG )
EVT_MENU ( ID_VALIDATE_W3C_SCHEMA, MyFrame::OnValidateSchema )
EVT_MENU ( ID_XPATH, MyFrame::OnXPath )
EVT_MENU_RANGE ( ID_XSLT, ID_XSLT_WORDML_DOCBOOK, MyFrame::OnXslt )
EVT_MENU ( ID_PRETTYPRINT, MyFrame::OnPrettyPrint )
EVT_MENU ( ID_ENCODING, MyFrame::OnEncoding )
EVT_MENU ( ID_SPELL, MyFrame::OnSpelling )
EVT_MENU ( ID_FONT_SMALLER, MyFrame::OnFontSmaller )
EVT_MENU ( ID_FONT_NORMAL, MyFrame::OnFontMedium )
EVT_MENU ( ID_FONT_LARGER, MyFrame::OnFontLarger )
EVT_MENU ( ID_OPTIONS, MyFrame::OnOptions )
EVT_MENU ( ID_HOME, MyFrame::OnHome )
EVT_MENU ( ID_DOWNLOAD_SOURCE, MyFrame::OnDownloadSource )
EVT_MENU ( ID_TOOLBAR_VISIBLE, MyFrame::OnToolbarVisible )
EVT_MENU ( ID_LOCATION_PANE_VISIBLE, MyFrame::OnLocationPaneVisible )
EVT_MENU ( ID_PROTECT_TAGS, MyFrame::OnProtectTags )
EVT_MENU ( ID_WRAP_WORDS, MyFrame::OnWrapWords )
EVT_MENU_RANGE ( ID_SHOW_TAGS, ID_HIDE_TAGS, MyFrame::OnVisibilityState )
EVT_MENU_RANGE ( ID_ASSOCIATE_DTD_PUBLIC, ID_ASSOCIATE_XSL, MyFrame::OnAssociate )
EVT_MENU_RANGE ( wxID_FILE1, wxID_FILE9, MyFrame::OnHistoryFile )
EVT_MENU_RANGE (
    ID_VALIDATE_PRESET1, ID_VALIDATE_PRESET9, MyFrame::OnValidatePreset )
EVT_MENU_RANGE (
    ID_COLOR_SCHEME_DEFAULT,
    ID_COLOR_SCHEME_NONE,
    MyFrame::OnColorScheme )
EVT_UPDATE_UI_RANGE ( ID_REPLACE, ID_GLOBAL_REPLACE, MyFrame::OnUpdateReplaceRange )
EVT_FIND ( wxID_ANY, MyFrame::OnDialogFind )
EVT_FIND_NEXT ( wxID_ANY, MyFrame::OnDialogFind )
EVT_FIND_REPLACE ( wxID_ANY, MyFrame::OnDialogReplace )
EVT_FIND_REPLACE_ALL ( wxID_ANY, MyFrame::OnDialogReplaceAll )
EVT_ICONIZE ( MyFrame::OnIconize )
EVT_UPDATE_UI ( ID_LOCATION_PANE_VISIBLE, MyFrame::OnUpdateLocationPaneVisible )
EVT_UPDATE_UI ( wxID_CLOSE, MyFrame::OnUpdateDocRange )
EVT_UPDATE_UI ( wxID_SAVEAS, MyFrame::OnUpdateDocRange )
EVT_UPDATE_UI ( wxID_CLOSE_ALL, MyFrame::OnUpdateCloseAll )
EVT_UPDATE_UI ( wxID_REVERT, MyFrame::OnUpdateSaveUndo )
EVT_UPDATE_UI ( wxID_SAVE, MyFrame::OnUpdateDocRange ) // always allow save if doc present
EVT_UPDATE_UI ( wxID_UNDO, MyFrame::OnUpdateSaveUndo )
EVT_UPDATE_UI ( wxID_REDO, MyFrame::OnUpdateRedo )
EVT_UPDATE_UI ( wxID_PASTE, MyFrame::OnUpdatePaste )
EVT_UPDATE_UI ( wxID_CUT, MyFrame::OnUpdateCutCopy )
EVT_UPDATE_UI ( wxID_COPY, MyFrame::OnUpdateCutCopy )
EVT_UPDATE_UI ( ID_FIND_AGAIN, MyFrame::OnUpdateFindAgain )
EVT_UPDATE_UI_RANGE ( ID_FIND, ID_EXPORT_MSWORD, MyFrame::OnUpdateDocRange )
EVT_UPDATE_UI ( ID_PREVIOUS_DOCUMENT, MyFrame::OnUpdatePreviousDocument )
EVT_UPDATE_UI ( ID_NEXT_DOCUMENT, MyFrame::OnUpdateNextDocument )
EVT_UPDATE_UI ( ID_HIDE_PANE, MyFrame::OnUpdateClosePane )
EVT_IDLE ( MyFrame::OnIdle )
EVT_AUINOTEBOOK_PAGE_CLOSE ( wxID_ANY, MyFrame::OnPageClosing )
#ifdef __WXMSW__
EVT_DROP_FILES ( MyFrame::OnDropFiles )
#endif
END_EVENT_TABLE()

IMPLEMENT_APP ( MyApp )

MyApp::MyApp() : checker ( NULL ), server ( NULL ), connection ( NULL ),
#ifdef __WXMSW__
        config ( new wxConfig ( _T ( "SourceForge Project\\XML Copy Editor" ) ) )
#else
        config ( new wxConfig ( _T ( "xmlcopyeditor" ) ) )
#endif
{
    lang = 0;

#ifdef __WXGTK__
    int fdnull = open ( "/dev/null", O_WRONLY, 0 );
    dup2 ( fdnull, STDERR_FILENO );
#endif
    myLocale.Init();
    int systemLocale = myLocale.GetSystemLanguage();
    switch ( systemLocale )
    {
    case wxLANGUAGE_GERMAN:
    case wxLANGUAGE_GERMAN_AUSTRIAN:
    case wxLANGUAGE_GERMAN_BELGIUM:
    case wxLANGUAGE_GERMAN_LIECHTENSTEIN:
    case wxLANGUAGE_GERMAN_LUXEMBOURG:
    case wxLANGUAGE_GERMAN_SWISS:
        systemLocale = wxLANGUAGE_GERMAN;
        break;
    case wxLANGUAGE_CHINESE_SIMPLIFIED:
        systemLocale = wxLANGUAGE_CHINESE_SIMPLIFIED;
        break;
    case wxLANGUAGE_CHINESE_TRADITIONAL:
        systemLocale = wxLANGUAGE_CHINESE_TRADITIONAL;
        break;
    case wxLANGUAGE_SPANISH:
    case wxLANGUAGE_SPANISH_ARGENTINA:
    case wxLANGUAGE_SPANISH_BOLIVIA:
    case wxLANGUAGE_SPANISH_CHILE:
    case wxLANGUAGE_SPANISH_COLOMBIA:
    case wxLANGUAGE_SPANISH_COSTA_RICA:
    case wxLANGUAGE_SPANISH_DOMINICAN_REPUBLIC:
    case wxLANGUAGE_SPANISH_ECUADOR:
    case wxLANGUAGE_SPANISH_EL_SALVADOR:
    case wxLANGUAGE_SPANISH_GUATEMALA:
    case wxLANGUAGE_SPANISH_HONDURAS:
    case wxLANGUAGE_SPANISH_MEXICAN:
    case wxLANGUAGE_SPANISH_MODERN:
    case wxLANGUAGE_SPANISH_NICARAGUA:
    case wxLANGUAGE_SPANISH_PANAMA:
    case wxLANGUAGE_SPANISH_PARAGUAY:
    case wxLANGUAGE_SPANISH_PERU:
    case wxLANGUAGE_SPANISH_PUERTO_RICO:
    case wxLANGUAGE_SPANISH_URUGUAY:
    case wxLANGUAGE_SPANISH_US:
    case wxLANGUAGE_SPANISH_VENEZUELA:
        systemLocale = wxLANGUAGE_SPANISH;
        break;
    case wxLANGUAGE_SLOVAK:
        systemLocale = wxLANGUAGE_SLOVAK;
        break;
    case wxLANGUAGE_SWEDISH:
        systemLocale = wxLANGUAGE_SWEDISH;
        break;
    case wxLANGUAGE_FRENCH:
        systemLocale = wxLANGUAGE_FRENCH;
        break;
    case wxLANGUAGE_UKRAINIAN:
        systemLocale = wxLANGUAGE_UKRAINIAN;
        break;
    default:
        systemLocale = wxLANGUAGE_ENGLISH_US;
        break;
    }

    if ( config.get() )
    {
#ifdef __WXMSW__
        singleInstanceCheck = config->Read ( _T ( "singleInstanceCheck" ), true );
#else
        long longFalse = 0;
        singleInstanceCheck = config->Read ( _T ( "singleInstanceCheck" ), longFalse );
#endif
        lang = config->Read ( _T ( "lang" ), systemLocale );
    }
    else
    {
        lang = systemLocale;
#ifdef __WXMSW__
        singleInstanceCheck = true;
#else
        singleInstanceCheck = false;
#endif
    }

    myLocale.Init ( lang, wxLOCALE_LOAD_DEFAULT );
    wxLocale::AddCatalogLookupPathPrefix ( wxT ( "." ) );
    wxLocale::AddCatalogLookupPathPrefix ( wxT ( ".." ) );

#ifdef __LINUX__
    wxString poDir = GetLinuxAppDir::run() + wxFileName::GetPathSeparator() + _T ( "po" );
    wxLocale::AddCatalogLookupPathPrefix ( poDir );
#endif

    if ( !myLocale.AddCatalog ( _T ( "messages" ) ) )
      ;

#ifdef __LINUX__
    {
        wxLogNull noLog;
        myLocale.AddCatalog ( _T ( "fileutils" ) );
    }
#endif
}

MyApp::~MyApp()
{
    delete checker;
    delete server;
    delete connection;
}

bool MyApp::OnInit()
{

    wxString name, service, hostName;
    name.Printf ( _T ( "xmlcopyeditor-%s" ), wxGetUserId().c_str() );
    service = IPC_SERVICE;
    hostName = _T ( "localhost" );

    if ( singleInstanceCheck )
    {
        checker = new wxSingleInstanceChecker ( name );
        while ( checker->IsAnotherRunning() )
        {
            // attempt calling server
            client = new MyClient();
            connection = ( MyClientConnection * )
                         client->MakeConnection ( hostName, service, IPC_TOPIC );
            if ( !connection || !connection->StartAdvise ( IPC_ADVISE_NAME ) )
                break;
            else
            {
                wxString argument;
                if ( this->argc > 1 )
                {
                    for ( int i = 1; i < this->argc; i++ )
                    {
                        argument = ( wxString ) this->argv[i];
                        argument = PathResolver::run ( argument );
                        connection->Poke ( argument, _T ( "Data" ) );
                    }
                }
                else
                {
                    argument = ( wxString ) IPC_NO_FILE;
                    connection->Poke ( argument, _T ( "Data" ) );
                }
                return false;
            }
        }
    }

    server = new MyServer;
    server->Create ( service );

    MyFrame *frame;
    try
    {
        wxImage::AddHandler ( new wxPNGHandler );
        wxSystemOptions::SetOption ( _T ( "msw.remap" ), 0 );
        frame = new MyFrame (
                    _ ( "XML Copy Editor" ),
                    config.get(),
                    myLocale,
                    singleInstanceCheck,
                    lang );
        frame->Show ( true );
        if ( frame->getHandleCommandLineFlag() )
            frame->handleCommandLine();
    }
    catch ( exception &e )
    {
        const char *what;
        what = e.what();
        wxString wideWhat, errorString;
        wideWhat = wxString ( what, wxConvLocal, strlen ( what ) );

        if ( wideWhat.empty() )
            wideWhat = _ ( "(unknown error)" );

        errorString = _ ( "XML Copy Editor has encountered the following error and needs to close: " );
        errorString += wideWhat;
        errorString += _T ( "." );
#ifdef __WXMSW__
        ::MessageBox (
            NULL,
            errorString,
            _ ( "Error" ),
            MB_ICONERROR | MB_TASKMODAL );
#else
        wxMessageBox ( errorString, _ ( "Error" ), wxICON_ERROR );
#endif
        exit ( EXIT_FAILURE );
    }
    catch ( ... )
    {
        exit ( EXIT_FAILURE );
    }
    return true;
}

void MyApp::OnUnhandledException()
{
#ifdef __WXMSW__
    ::MessageBox (
        NULL,
        _ ( "XML Copy Editor has encountered an error and needs to close." ),
        _ ( "Error" ),
        MB_ICONERROR | MB_TASKMODAL );
#else
    wxMessageBox (
        _ ( "XML Copy Editor has encountered an error and needs to close." ),
        _ ( "Error" ),
        wxICON_ERROR );
#endif
    exit ( EXIT_FAILURE );
}

bool MyApp::OnExceptionInMainLoop()
{
    try
    {
        throw;
    }
#ifdef __WXMSW__
    catch ( bad_alloc& )
    {
        ::MessageBox (
            NULL,
            _ ( "The operating system has turned down a request for additional memory" ),
            _ ( "Out of memory" ),
            MB_ICONERROR );
        return true;
    }
#endif
    catch ( exception &e )
    {
        const char *what;
        what = e.what();
        wxString wideWhat, errorString;
        wideWhat = wxString ( what, wxConvLocal, strlen ( what ) );

        if ( wideWhat.empty() )
            _ ( "(unknown error)" );

        errorString = _ ( "The following error has occurred: " );
        errorString += wideWhat;
        errorString += _ ( ".\n\nSelect \"Abort\" to exit, \"Retry\" to close this window and \"Ignore\" to continue." );
#ifdef __WXMSW__
        int ret = ::MessageBox (
                      NULL,
                      errorString,
                      _ ( "Error" ),
                      MB_ABORTRETRYIGNORE |
                      MB_ICONERROR |
                      MB_TASKMODAL );
        switch ( ret )
        {
        case IDABORT:
            exit ( EXIT_FAILURE );
            break;
        case IDRETRY:
            return false;
        case IDIGNORE:
            return true;
        default:
            throw;
        }
#else
        // wxGTK does not reach this point; see HandleEvent below
        wxMessageBox (
            errorString,
            _ ( "Error" ),
            wxICON_ERROR );
        return false;
#endif
    }
    catch ( ... )
    {
        wxString otherError ( _ ( "XML Copy Editor has encountered an error and needs to close." ) );
#ifdef __WXMSW__
        ::MessageBox (
            NULL,
            otherError,
            _ ( "Error" ),
            MB_ICONERROR );
        return false;
#else
        wxMessageBox (
            otherError,
            _ ( "Error" ),
            wxICON_ERROR );
        return false;
#endif
    }
    return false;
}

#ifndef __WXMSW__
void MyApp::HandleEvent ( wxEvtHandler *handler, wxEventFunction func, wxEvent& event ) const
{
    try
    {
        wxApp::HandleEvent ( handler, func, event );
    }
    catch ( std::bad_alloc& )
    {
        wxMessageBox (
            _ ( "The operating system has turned down a request for additional memory" ),
            _ ( "Out of memory" ),
            wxICON_ERROR );
        return;
    }
    catch ( std::exception& e )
    {
        std::string s ( e.what() );
        wxString ws = wxString ( s.c_str(), wxConvUTF8, s.size() );
        wxMessageBox (
            ws,
            _ ( "Error" ),
            wxICON_ERROR );
        return;
    }
    catch ( ... )
    {
        throw;
    }
}
#endif

MyFrame::MyFrame (
    const wxString& title,
    wxConfig *configParameter,
    wxLocale& locale,
    bool singleInstanceCheckParameter,
    int langParameter ) :
        wxFrame ( NULL, wxID_ANY, title ),
        config ( configParameter ),
        myLocale ( locale ),
        singleInstanceCheck ( singleInstanceCheckParameter ),
        lang ( langParameter ),
        htmlPrinting ( new wxHtmlEasyPrinting (
                           wxEmptyString,
                           this ) ),
        findDialog ( 0 ),
#ifndef __WXMSW__
        helpController ( new wxHtmlHelpController() ),
#endif
        menuBar ( 0 ),
        toolBar ( 0 ),
        xmlMenu ( 0 ),
        mainBook ( 0 ),
        restoreFocusToNotebook ( false )
{
    manager.SetManagedWindow ( this );

    lastPos = 0;
    htmlReport = NULL;
    lastDoc = NULL;

    wxString defaultFont =
#ifdef __WXMSW__
        _T ( "Arial" );
#else
        _T ( "Bitstream Vera Sans" );
#endif

#ifdef __WXMSW__
    coolBar = NULL;
#endif

    bool findMatchCase;

    // fetch configuration
    if ( config ) // config found
    {
        history.Load ( *config );
        properties.insertCloseTag =
            config->Read ( _T ( "insertCloseTag" ), true );
        properties.completion =
            config->Read ( _T ( "completion" ), true );
        properties.number =
            config->Read ( _T ( "number" ), true );
        properties.fold =
            config->Read ( _T ( "fold" ), true );
        properties.currentLine =
            config->Read ( _T ( "currentLine" ), true );
        properties.highlightSyntax =
            config->Read ( _T ( "highlightSyntax" ), true );
        properties.whitespaceVisible =
            config->Read ( _T ( "whitespaceVisible" ), true );
        properties.indentLines =
            config->Read ( _T ( "indentLines" ), true );
        properties.toggleLineBackground =
            config->Read ( _T ( "toggleLineBackground" ), true );
        properties.protectHiddenElements =
            config->Read ( _T ( "protectHiddenElements" ), true );
        properties.deleteWholeTag =
            config->Read ( _T ( "deleteWholeTag" ), true );
        properties.validateAsYouType =
            config->Read ( _T ( "validateAsYouType" ), true );
        properties.font =
            config->Read ( _T ( "font" ), defaultFont );
        findRegex =
            config->Read ( _T ( "findRegex" ), true );
        xpathExpression =
            config->Read ( _T ( "xpathExpression" ), wxEmptyString );
        lastXslStylesheet =
            config->Read ( _T ( "lastXslStylesheet" ), wxEmptyString );
        lastRelaxNGSchema =
            config->Read ( _T ( "lastRelaxNGSchema" ), wxEmptyString );

        lastXslStylesheet.Replace ( _T ( " " ), _T ( "%20" ), true );
        lastRelaxNGSchema.Replace ( _T ( " " ), _T ( "%20" ), true );

        applicationDir =
            config->Read ( _T ( "applicationDir" ), wxEmptyString );
        if ( applicationDir.empty() )
        {
#ifdef __WXMSW__
            applicationDir =
                config->Read ( _T ( "InstallPath" ), wxGetCwd() );
#else
            applicationDir = GetLinuxAppDir::run();
#endif
        }
        browserCommand =
            config->Read ( _T ( "browserCommand" ), wxEmptyString );

        // if default value != true, type as long int
        long valZoom, longFalse;
        longFalse = false;
        valZoom = 0;
        frameWidth = frameHeight = framePosX = framePosY = 0;

        properties.wrap =
            config->Read ( _T ( "wrap" ), longFalse );

        properties.zoom =
            config->Read ( _T ( "zoom" ), valZoom );

        properties.colorScheme = config->Read ( _T ( "colorScheme" ), COLOR_SCHEME_DEFAULT );

        globalReplaceAllDocuments =
            config->Read ( _T ( "globalReplaceAllDocuments" ), longFalse );
        showFullPathOnFrame =
            config->Read ( _T ( "showFullPathOnFrame" ), longFalse );
        findMatchCase =
            config->Read ( _T ( "findMatchCase" ), longFalse );

        commandSync = config->Read ( _T ( "commandSync" ), longFalse );
        commandOutput = config->Read ( _T ( "commandOutput" ), ID_COMMAND_OUTPUT_IGNORE );
        commandString = config->Read ( _T ( "commandString" ), wxEmptyString );

        ruleSetPreset =
            config->Read ( _T ( "ruleSetPreset" ), _ ( "Default dictionary and style" ) );
        filterPreset =
            config->Read ( _T ( "filterPreset" ), _ ( "(No filter)" ) );
        findData.SetFindString ( config->Read ( _T ( "findReplaceFind" ), _T ( "" ) ) );
        findData.SetReplaceString ( config->Read ( _T ( "findReplaceReplace" ), _T ( "" ) ) );

        toolbarVisible =
            config->Read ( _T ( "toolbarVisible" ), true );
        protectTags = config->Read ( _T ( "protectTags" ), longFalse );
        visibilityState = config->Read ( _T ( "visibilityState" ), ID_SHOW_TAGS );

        framePosX = config->Read ( _T ( "framePosX" ), framePosX );
        framePosY = config->Read ( _T ( "framePosY" ), framePosY );
        frameWidth = config->Read ( _T ( "frameWidth" ), frameWidth );
        frameHeight = config->Read ( _T ( "frameHeight" ), frameHeight );
        rememberOpenTabs = config->Read ( _T ( "rememberOpenTabs" ), true );
        libxmlNetAccess = config->Read ( _T ( "libxmlNetAccess" ), longFalse );
        openTabsOnClose = config->Read ( _T ( "openTabsOnClose" ), _T ( "" ) );
        notebookStyle = config->Read ( _T ( "notebookStyle" ), ID_NOTEBOOK_STYLE_VC8_COLOR );
        saveBom = config->Read ( _T ( "saveBom" ), true );
        unlimitedUndo = config->Read ( _T ( "unlimitedUndo" ), true );
        layout = config->Read ( _T ( "layout" ), wxEmptyString );
        restoreLayout = config->Read ( _T ( "restoreLayout" ), true );
        showLocationPane = config->Read ( _T ( "showLocationPane" ), true );
        showInsertChildPane = config->Read ( _T ( "showInsertChildPane" ), true );
        showInsertSiblingPane = config->Read ( _T ( "showInsertSiblingPane" ), true );
        showInsertEntityPane = config->Read ( _T ( "showInsertEntityPane" ), true );
        expandInternalEntities = config->Read ( _T ( "expandInternalEntities" ), true );

#ifdef __WXMSW__
        useCoolBar = config->Read ( _T ( "useCoolBar" ), true );
#endif
    }
    else // config not found
    {
        properties.insertCloseTag =
            properties.completion =
                properties.currentLine =
                    properties.indentLines =
                        properties.protectHiddenElements =
                            properties.toggleLineBackground =
                                properties.deleteWholeTag =
                                    properties.highlightSyntax = true;
        properties.font = defaultFont;
        properties.wrap = properties.whitespaceVisible = false;
        properties.zoom = 0;
        properties.colorScheme = COLOR_SCHEME_DEFAULT;
#ifdef __WXMSW__
        applicationDir = wxGetCwd();
#else
        applicationDir = GetLinuxAppDir::run();//getLinuxApplicationDir();
#endif
        ruleSetPreset = _ ( "Default dictionary and style" );
        filterPreset = _ ( "No filter" );
        xpathExpression = lastXslStylesheet = lastRelaxNGSchema = wxEmptyString;
        findRegex = true;
        findMatchCase = globalReplaceAllDocuments =
                            showFullPathOnFrame = false;
        toolbarVisible = true;
        protectTags = false;
        visibilityState = SHOW_TAGS;
        framePosX = framePosY = frameWidth = frameHeight = 0;
        rememberOpenTabs = true;
        libxmlNetAccess = false;
        openTabsOnClose = wxEmptyString;
        browserCommand = wxEmptyString;
        notebookStyle = ID_NOTEBOOK_STYLE_VC8_COLOR;
        saveBom = unlimitedUndo = true;
        layout = wxEmptyString;
        restoreLayout = true;
        showLocationPane = true;
        showInsertChildPane = true;
        showInsertSiblingPane = true;
        showInsertEntityPane = true;
        expandInternalEntities = true;
        properties.validateAsYouType = true;

        commandSync = false;
        commandOutput = ID_COMMAND_OUTPUT_IGNORE;
        commandString = wxEmptyString;

#ifdef __WXMSW__
        useCoolBar = true;
#endif
    }

    largeFileProperties.completion = false;
    largeFileProperties.fold = false;
    largeFileProperties.whitespaceVisible = false;
    largeFileProperties.wrap = false;
    largeFileProperties.indentLines = false;
    largeFileProperties.protectHiddenElements = false;
    largeFileProperties.toggleLineBackground = false;
    largeFileProperties.toggleLineBackground = false;
    largeFileProperties.insertCloseTag = false;
    largeFileProperties.deleteWholeTag = false;
    largeFileProperties.highlightSyntax = false;
    largeFileProperties.validateAsYouType = false;
    largeFileProperties.number = properties.number;
    largeFileProperties.currentLine = properties.currentLine;
    largeFileProperties.font = properties.font;
    largeFileProperties.zoom = 0;
    largeFileProperties.colorScheme = COLOR_SCHEME_NONE;

    updatePaths();
    loadBitmaps();

    size_t findFlags = 0;
    findFlags |= wxFR_DOWN;

    if ( findMatchCase )
        findFlags |= wxFR_MATCHCASE;

    findData.SetFlags ( findFlags );

    if ( browserCommand.empty() )
    {
#ifdef __WXMSW__
        browserCommand = binDir + _T ( "navigate.exe" );
#else
        browserCommand = getLinuxBrowser();
#endif
    }

    // initialise document count for tab labels
    documentCount = 1;

    SetIcon ( wxICON ( appicon ) );

    CreateStatusBar();
    wxStatusBar *status = GetStatusBar();
    int widths[] = { -24, -6, -6, -6, -8 };
    status->SetFieldsCount ( 5 );
    status->SetStatusWidths ( 5, widths );

    if ( !frameWidth ||
            !frameHeight ||
            frameWidth < 0 ||
            frameHeight < 0 ||
            framePosX < 0 ||
            framePosY < 0 )
    {
#ifdef __WXMSW__
        Maximize();
#else
        SetSize ( 50, 50, 640, 480 );
#endif
    }
    else
    {
        SetSize ( framePosX, framePosY, frameWidth, frameHeight );
    }

    stylePosition = aboutPosition = wxDefaultPosition;
    styleSize = wxSize ( 720, 540 );

#ifdef __WXMSW__
    useCoolBarOnStart = useCoolBar;
#endif

    showTopBars ( toolbarVisible );

    long style = wxAUI_NB_TOP |
                 wxAUI_NB_TAB_SPLIT |
                 wxAUI_NB_TAB_MOVE |
                 wxAUI_NB_WINDOWLIST_BUTTON |
                 wxAUI_NB_CLOSE_ON_ALL_TABS |
                 wxNO_BORDER;

    mainBook = new MyNotebook (
                   this,
                   ID_NOTEBOOK,
                   wxDefaultPosition,
                   wxDefaultSize,
                   style );

    manager.AddPane ( mainBook, wxAuiPaneInfo().CenterPane()
                      .PaneBorder ( false ).Name ( _T ( "documentPane" ) ) );
    manager.GetPane ( mainBook ).dock_proportion = 10;

    // add insert child panes
    locationPanel = new LocationPanel ( this, ID_LOCATION_PANEL );
    insertChildPanel = new InsertPanel ( this, ID_INSERT_CHILD_PANEL,
                                         INSERT_PANEL_TYPE_CHILD );
    insertSiblingPanel = new InsertPanel ( this, ID_INSERT_SIBLING_PANEL,
                                           INSERT_PANEL_TYPE_SIBLING );
    insertEntityPanel = new InsertPanel ( this, ID_INSERT_ENTITY_PANEL,
                                          INSERT_PANEL_TYPE_ENTITY );

#ifdef __WXMSW__
    manager.AddPane ( ( wxWindow * ) locationPanel, wxRIGHT, _ ( "Current Element" ) );
    manager.AddPane ( ( wxWindow * ) insertChildPanel, wxRIGHT, _ ( "Insert Element" ) );
    manager.AddPane ( ( wxWindow * ) insertSiblingPanel, wxRIGHT, _ ( "Insert Sibling" ) );
    manager.AddPane ( ( wxWindow * ) insertEntityPanel, wxRIGHT, _ ( "Insert Entity" ) );
#else
    manager.AddPane ( ( wxWindow * ) insertEntityPanel, wxRIGHT, _ ( "Insert Entity" ) );
    manager.AddPane ( ( wxWindow * ) insertSiblingPanel, wxRIGHT, _ ( "Insert Sibling" ) );
    manager.AddPane ( ( wxWindow * ) insertChildPanel, wxRIGHT, _ ( "Insert Element" ) );
    manager.AddPane ( ( wxWindow * ) locationPanel, wxRIGHT, _ ( "Current Element" ) );
#endif

    manager.GetPane ( locationPanel ).Name ( _T ( "locationPane" ) ).Show (
        ( restoreLayout ) ? showLocationPane : true ).DestroyOnClose ( false ).PinButton ( true );
    manager.GetPane ( locationPanel ).dock_proportion = 1;

    manager.GetPane ( insertChildPanel ).Name ( _T ( "insertChildPane" ) ).Show (
        ( restoreLayout ) ? showInsertChildPane : true ).DestroyOnClose ( false ).PinButton ( true );
    manager.GetPane ( insertChildPanel ).dock_proportion = 1;

    manager.GetPane ( insertSiblingPanel ).Name ( _T ( "insertSiblingPane" ) ).Show (
        ( restoreLayout ) ? showInsertSiblingPane : true ).DestroyOnClose ( false ).PinButton ( true );
    manager.GetPane ( insertSiblingPanel ).dock_proportion = 1;

    manager.GetPane ( insertEntityPanel ).Name ( _T ( "insertEntityPane" ) ).Show (
        ( restoreLayout ) ? showInsertEntityPane : true ).DestroyOnClose ( false ).PinButton ( true );
    manager.GetPane ( insertEntityPanel ).dock_proportion = 1;

    // add (hidden) message pane
    htmlReport = new MyHtmlPane (
                     this,
                     ID_VALIDATION_PANE,
                     wxDefaultPosition,
                     wxSize ( -1, 48 ) );
#ifndef __WXMSW__
    const int sizeArray[] =
        {
            8, 9, 10, 11, 12, 13, 14
        };
    htmlReport->SetFonts ( wxEmptyString, wxEmptyString, sizeArray );
#endif
    htmlReport->SetBorders ( 0 );
    manager.AddPane ( htmlReport, wxAuiPaneInfo().Movable().Bottom()
                      .Hide().Name ( _T ( "messagePane" ) )
                      .DestroyOnClose ( false ).Layer ( 1 ) );
    manager.GetPane ( htmlReport ).dock_proportion = 1;

#ifdef NEWFINDREPLACE
    findReplacePanel = new FindReplacePanel (
                           this,
                           ID_FIND_REPLACE_PANEL,
                           &findData,
                           true,
                           findRegex );

    manager.AddPane (
        ( wxWindow * ) findReplacePanel,
        wxAuiPaneInfo().Bottom().Hide().Caption ( wxEmptyString ).
        DestroyOnClose ( false ).Layer ( 2 ) );
#endif

    commandPanel = new CommandPanel (
                       this,
                       wxID_ANY,
                       commandString, // tbd
                       commandSync, // tbd
                       commandOutput // tbd
                   );
    manager.AddPane (
        ( wxWindow * ) commandPanel,
        wxAuiPaneInfo().Bottom().Hide().Caption ( _T ( "Command" ) ).DestroyOnClose ( false ).Layer ( 3 ) );

    if ( !wxFileName::DirExists ( applicationDir ) )
        GetStatusBar()->SetStatusText ( _ ( "Cannot open application directory: see Tools, Options..., General" ) );

    // handle command line and, on Windows, MS Word integration
    handleCommandLineFlag = ( wxTheApp->argc > 1 ) ? true : false;

    if ( rememberOpenTabs && !openTabsOnClose.empty() )
        openRememberedTabs();
    else
    {
        if ( !handleCommandLineFlag )
            newDocument ( wxEmptyString );
    }

#ifdef __WXMSW__
    DragAcceptFiles ( true ); // currently Windows only
#endif

    XmlDoc *doc = getActiveDocument();
    insertEntityPanel->update ( doc ); // NULL is ok

    manager.Update();

    /*
      defaultLayout = manager.SavePerspective();

      // restore layout if req'd
      if (restoreLayout && !layout.empty())
      {
        if (!manager.LoadPerspective(layout, true))
          manager.LoadPerspective(defaultLayout, true);
      }
    */
}

MyFrame::~MyFrame()
{
    std::vector<wxString>::iterator it;
    for ( it = tempFileVector.begin(); it != tempFileVector.end(); it++ )
        wxRemoveFile ( *it );

    layout = manager.SavePerspective();
    if ( !config )
        return;
    history.Save ( *config );
    config->Write ( _T ( "insertCloseTag" ), properties.insertCloseTag );
    config->Write ( _T ( "completion" ), properties.completion );
    config->Write ( _T ( "number" ), properties.number );
    config->Write ( _T ( "fold" ), properties.fold );
    config->Write ( _T ( "currentLine" ), properties.currentLine );
    config->Write ( _T ( "whitespaceVisible" ), properties.whitespaceVisible );
    config->Write ( _T ( "wrap" ), properties.wrap );
    config->Write ( _T ( "indentLines" ), properties.indentLines );
    config->Write ( _T ( "zoom" ), properties.zoom );
    config->Write ( _T ( "colorScheme" ), properties.colorScheme );
    config->Write ( _T ( "protectHiddenElements" ), properties.protectHiddenElements );
    config->Write ( _T ( "toggleLineBackground" ), properties.toggleLineBackground );
    config->Write ( _T ( "deleteWholeTag" ), properties.deleteWholeTag );
    config->Write ( _T ( "validateAsYouType" ), properties.validateAsYouType );
    config->Write ( _T ( "font" ), properties.font );
    config->Write ( _T ( "highlightSyntax" ), properties.highlightSyntax );
    config->Write ( _T ( "applicationDir" ), applicationDir );
    config->Write ( _T ( "ruleSetPreset" ), ruleSetPreset );
    config->Write ( _T ( "filterPreset" ), filterPreset );
    config->Write ( _T ( "xpathExpression" ), xpathExpression );
    config->Write ( _T ( "findReplaceFind" ), findData.GetFindString() );
    config->Write ( _T ( "findReplaceReplace" ), findData.GetReplaceString() );
    config->Write ( _T ( "globalReplaceAllDocuments" ), globalReplaceAllDocuments );
    config->Write ( _T ( "showFullPathOnFrame" ), showFullPathOnFrame );
    config->Write ( _T ( "toolbarVisible" ), toolbarVisible );
    config->Write ( _T ( "protectTags" ), protectTags );
    config->Write ( _T ( "visibilityState" ), visibilityState );
    config->Write ( _T ( "browserCommand" ), browserCommand );
    config->Write ( _T ( "layout" ), layout );
    config->Write ( _T ( "showLocationPane" ), manager.GetPane ( locationPanel ).IsShown() );
    config->Write ( _T ( "showInsertChildPane" ), manager.GetPane ( insertChildPanel ).IsShown() );
    config->Write ( _T ( "showInsertSiblingPane" ), manager.GetPane ( insertSiblingPanel ).IsShown() );
    config->Write ( _T ( "showInsertEntityPane" ), manager.GetPane ( insertEntityPanel ).IsShown() );
    config->Write ( _T ( "expandInternalEntities" ), expandInternalEntities );
    config->Write ( _T ( "findRegex" ), findReplacePanel->getRegex() );
    config->Write ( _T ( "findMatchCase" ), ( findData.GetFlags() ) & wxFR_MATCHCASE );
    config->Write ( _T ( "commandSync" ), commandPanel->getSync() );
    config->Write ( _T ( "commandOutput" ), commandPanel->getOutput() );
    config->Write ( _T ( "commandString" ), commandPanel->getCommand() );

#ifdef __WXMSW__
    config->Write ( _T ( "useCoolBar" ), useCoolBar );
#endif

    config->Write ( _T ( "restoreLayout" ), restoreLayout );


    config->Write ( _T ( "lastXslStylesheet" ), lastXslStylesheet );
    config->Write ( _T ( "lastRelaxNGSchema" ), lastRelaxNGSchema );

    GetPosition ( &framePosX, &framePosY );
    config->Write ( _T ( "framePosX" ), framePosX );
    config->Write ( _T ( "framePosY" ), framePosY );
    GetSize ( &frameWidth, &frameHeight );
    config->Write ( _T ( "frameWidth" ), frameWidth );
    config->Write ( _T ( "frameHeight" ), frameHeight );

    config->Write ( _T ( "rememberOpenTabs" ), rememberOpenTabs );
    config->Write ( _T ( "openTabsOnClose" ), openTabsOnClose );
    config->Write ( _T ( "libxmlNetAccess" ), libxmlNetAccess );

    config->Write ( _T ( "singleInstanceCheck" ), singleInstanceCheck );
    config->Write ( _T ( "lang" ), lang );
    config->Write ( _T ( "notebookStyle" ), notebookStyle );
    config->Write ( _T ( "saveBom" ), saveBom );
    config->Write ( _T ( "unlimitedUndo" ), unlimitedUndo );
    manager.UnInit();
}

wxString MyFrame::getLinuxBrowser()
{
    wxString s;
    const int stringArrayLen = 9;
    wxString stringArray[stringArrayLen];
    stringArray[0] = _T ( "/usr/bin/firefox" );
    stringArray[1] = _T ( "/usr/bin/mozilla" );
    stringArray[2] = _T ( "/usr/bin/opera" );
    stringArray[3] = _T ( "/usr/bin/dillo" );
    stringArray[4] = _T ( "/opt/gnome/bin/epiphany" );
    stringArray[5] = _T ( "/opt/gnome/bin/galeon" );
    stringArray[6] = _T ( "/opt/kde/bin/konqueror" );
    stringArray[7] = _T ( "/opt/mozilla/bin/firefox" );
    stringArray[8] = wxEmptyString; // empty option is safe

    for ( int i = 0; i < stringArrayLen; i++ )
    {
        s = stringArray[i];
        if ( wxFileName::FileExists ( s ) )
            break;
    }
    return s;
}

void MyFrame::showTopBars ( bool b )
{
#ifdef __WXMSW__
    if ( useCoolBarOnStart )
    {
        if ( coolBar )
        {
            manager.DetachPane ( coolBar );
            manager.Update();
            coolBar->ShowBand ( 1, b );
            manager.AddPane ( coolBar, wxAuiPaneInfo().Top().CaptionVisible ( false ).Name ( _T ( "coolBar" ) ) );
        }
        else
        {
            toolBar = getToolBar();
            SetToolBar ( NULL );
            if ( toolBar && protectTags )
                toolBar->ToggleTool ( ID_PROTECT_TAGS, protectTags );
            menuBar = getMenuBar();
            coolBar = new wxCoolBar ( this, -1 );
            coolBar->AddBand ( menuBar, false, wxEmptyString, true );
            coolBar->AddBand ( toolBar, true, wxEmptyString, true );
            coolBar->ShowBand ( 1, b );
            manager.AddPane ( coolBar, wxAuiPaneInfo().Top().CaptionVisible ( false ).Name ( _T ( "coolBar" ) ) );
        }
        return;
    }
#endif
    if ( !menuBar )
    {
        SetToolBar ( NULL );
        menuBar = getMenuBar();
        SetMenuBar ( menuBar );
    }
    if ( b )
    {
        if ( !toolBar )
            toolBar = getToolBar();
        SetToolBar ( toolBar );
    }
    else
    {
        SetToolBar ( NULL );
        delete toolBar;
        toolBar = NULL;
    }
}

void MyFrame::handleCommandLine()
{
    bool wordFlag, styleFlag;
    wordFlag = styleFlag = false;
    wxChar c;

    int m_argc = wxTheApp->argc;
    wxChar **m_argv = wxTheApp->argv;

    while ( ( --m_argc > 0 && ( *++m_argv ) [0] == L'-' ) != 0 )
    {
        wxString wideVersion ( ABOUT_VERSION );
        std::string version = ( const char * ) wideVersion.mb_str ( wxConvUTF8 );
        while ( ( c = *++m_argv[0] ) != 0 )
        {
            switch ( c )
            {
            case L'w':
                wordFlag = true;
                break;
            case L's':
                styleFlag = true;
                break;
            case L'-':
                if ( *++m_argv[0] == L'v' )
                {
                    std::cout << version.c_str() << std::endl;
                }
                else
                {
                    std::cout << "Usage: xmlcopyeditor [--version --help -ws] [<file>] [<file2>]" << std::endl << "Options -w (import Microsoft Word document) and -s (open Spelling and style check) are provided for integration with Microsoft Office and only available on Windows" << std::endl;
                }
                exit ( 0 );
            default:
                messagePane ( _ ( "Unknown command line switch (expecting 'w', 's', --version or --help)" ),
                              CONST_STOP );
                return;
            }
        }
    }

    if ( ! ( *m_argv ) )
    {
        messagePane ( _ ( "Command line processing incomplete: no file specified" ),
                      CONST_STOP );
        return;
    }

    wxString fileName;

    // no flags specified or not Windows
#ifdef __WXMSW__
    if ( !styleFlag && !wordFlag )
#endif
    {
        for ( ; *m_argv; ++m_argv )
        {
            fileName = wxString ( *m_argv, wxConvLocal, wcslen ( *m_argv ) );
            fileName = PathResolver::run ( fileName );
            if ( isOpen ( fileName ) )
                continue;
            else if ( !openFile ( fileName ) )
                break;
        }
        return;
    }

    // options only available on Windows
    fileName = wxString ( *m_argv, wxConvLocal, wcslen ( *m_argv ) );

    // fetch as many parameters as possible
    for ( ;; )
    {
        ++m_argv;
        if ( ! ( *m_argv ) )
            break;
        ruleSetPreset = wxString ( *m_argv, wxConvLocal, wcslen ( *m_argv ) );

        ++m_argv;
        if ( ! ( *m_argv ) )
            break;
        filterPreset = wxString ( *m_argv, wxConvLocal, wcslen ( *m_argv ) );

        ++m_argv;
        if ( ! ( *m_argv ) )
            break;
        applicationDir = wxString ( *m_argv, wxConvLocal, wcslen ( *m_argv ) );
        updatePaths();

        break;
    }
    if ( wordFlag )
        importMSWord ( fileName );
    else
        openFile ( fileName );

    if ( styleFlag && !ruleSetPreset.empty() && !filterPreset.empty() )
    {
        wxCommandEvent e;
        OnSpelling ( e );
    }
}

bool MyFrame::isOpen ( const wxString& fileName )
{
    return ( openFileSet.find ( fileName ) != openFileSet.end() );
}

void MyFrame::activateTab ( const wxString& fileName )
{
    int pageCount = mainBook->GetPageCount();
    XmlDoc *currentDoc;
    for ( int i = 0; i < pageCount; ++i )
    {
        currentDoc = ( XmlDoc * ) mainBook->GetPage ( i );
        if ( !currentDoc )
            break;
        if ( currentDoc->getFullFileName() == fileName )
        {
            mainBook->SetSelection ( i );
            break;
        }
    }
}

void MyFrame::OnAbout ( wxCommandEvent& WXUNUSED ( event ) )
{
    wxAboutDialogInfo info;
    info.SetName ( _ ( "XML Copy Editor" ) );
    info.SetWebSite ( _T ( "http://xml-copy-editor.sourceforge.net" ) );
    info.SetVersion ( ABOUT_VERSION );
    info.SetCopyright ( ABOUT_COPYRIGHT );
    info.AddDeveloper ( _ ( "Gerald Schmidt (development) <gnschmidt@users.sourceforge.net>" ) );
    info.AddDeveloper ( _ ( "Matt Smigielski (testing) <alectrus@users.sourceforge.net>" ) );
    info.AddTranslator ( _ ( "Viliam Búr (Slovak) <viliam@bur.sk>" ) );
    info.AddTranslator ( _ ( "David Håsäther (Swedish) <hasather@gmail.com>" ) );
    info.AddTranslator ( _ ( "François Badier (French) <frabad@gmail.com>" ) );
    info.AddTranslator ( _ ( "Thomas Wenzel (German) <thowen@users.sourceforge.net>" ) );
    info.AddTranslator ( _ ( "SHiNE CsyFeK (Chinese Simplified) <csyfek@gmail.com>" ) );
    info.AddTranslator ( _ ( "HSU PICHAN, YANG SHUFUN, CHENG PAULIAN, CHUANG KUO-PING, Marcus Bingenheimer (Chinese Traditional)" ) );
    info.AddTranslator ( _ ( "Serhij Dubyk (Ukrainian) <dubyk@library.lviv.ua>" ) );
    info.SetLicense ( ABOUT_LICENSE );
    info.SetDescription ( ABOUT_DESCRIPTION );
    wxAboutBox ( info );
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;
    doc->SetFocus();
}

void MyFrame::OnCheckWellformedness ( wxCommandEvent& event )
{
    statusProgress ( wxEmptyString );
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;

    std::string utf8Buffer;
    getRawText ( doc, utf8Buffer );
    if ( utf8Buffer.empty() )
        return;

    // handle unusual encodings
    if ( !XmlEncodingHandler::setUtf8 ( utf8Buffer ) )
    {
        encodingMessage();
        return;
    }

    doc->clearErrorIndicators();
    statusProgress ( _ ( "Parse in progress..." ) );

    // check for well-formedness
    auto_ptr<WrapExpat> we ( new WrapExpat() );
    if ( !we->parse ( utf8Buffer.c_str() ) )
    {
        std::string error = we->getLastError();
        wxString werror = wxString ( error.c_str(), wxConvUTF8, error.size() );
        statusProgress ( wxEmptyString );
        messagePane ( werror, CONST_WARNING );
        std::pair<int, int> posPair = we->getErrorPosition();
        -- ( posPair.first );
        int cursorPos =
            doc->PositionFromLine ( posPair.first );
        doc->SetSelection ( cursorPos, cursorPos );

        doc->setErrorIndicator ( posPair.first, posPair.second );
        return;
    }

    statusProgress ( wxEmptyString );
    documentOk ( _ ( "well-formed" ) );
}

void MyFrame::OnPageClosing ( wxAuiNotebookEvent& event ) //wxNotebookEvent& event)//wxFlatNotebookEvent& event)
{
    deletePageVetoed = false;

    if ( insertChildPanel && insertSiblingPanel && locationPanel )
    {
        insertChildPanel->update ( NULL, wxEmptyString );
        insertSiblingPanel->update ( NULL, wxEmptyString );
        locationPanel->update();
        manager.Update();
    }

    XmlDoc *doc;
    doc = ( XmlDoc * ) mainBook->GetPage ( event.GetSelection() );
    if ( !doc )
        return;

    statusProgress ( wxEmptyString );
    closePane();

    if ( doc->GetModify() ) //CanUndo())
    {
        int selection;
        wxString fileName;
        if ( ( selection = mainBook->GetSelection() ) != -1 )
            fileName = doc->getShortFileName();

        int answer = wxMessageBox (
                         _ ( "Do you want to save the changes to " ) + fileName + _T ( "?" ),
                         _ ( "XML Copy Editor" ),
                         wxYES_NO | wxCANCEL | wxICON_QUESTION,
                         this );

        if ( answer == wxCANCEL )
        {
            event.Veto();
            deletePageVetoed = true;
            return;
        }
        else if ( answer == wxYES )
        {
            wxCommandEvent event;
            OnSave ( event );
        }
    }
    statusProgress ( wxEmptyString );

    openFileSet.erase ( doc->getFullFileName() );
    event.Skip();
}

void MyFrame::OnClose ( wxCommandEvent& WXUNUSED ( event ) )
{
    closeActiveDocument();
}

void MyFrame::OnCloseAll ( wxCommandEvent& WXUNUSED ( event ) )
{
    if ( !mainBook )
        return;
    openTabsOnClose = wxEmptyString;

    // retain tab order
    if ( rememberOpenTabs && !openFileSet.empty() )
    {
        XmlDoc *doc;
        wxString fullPath;
        size_t maxTabs = mainBook->GetPageCount();
        for ( size_t i = 0; i < maxTabs; ++i )
        {
            doc = ( XmlDoc * ) mainBook->GetPage ( i );
            if ( doc )
            {
                fullPath = doc->getFullFileName();
                if ( !fullPath.empty() )
                {
                    openTabsOnClose.Append ( fullPath );
                    openTabsOnClose.Append ( _T ( "|" ) );
                }
            }
        }
    }

    while ( closeActiveDocument() )
        ;
}

void MyFrame::OnClosePane ( wxCommandEvent& WXUNUSED ( event ) )
{
    closePane();
    //closeFindReplacePane();
    //closeCommandPane();

    XmlDoc *doc = getActiveDocument();
    if ( doc )
        doc->SetFocus();
}

void MyFrame::closePane()
{
    if ( !htmlReport )
        return;
    manager.GetPane ( htmlReport ).Hide();
    manager.Update();

    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;
    doc->SetFocus();
}

void MyFrame::closeFindReplacePane()
{
    if ( manager.GetPane ( findReplacePanel ).IsShown() )
        manager.GetPane ( findReplacePanel ).Hide();
    manager.Update();
}

void MyFrame::closeCommandPane()
{
    if ( manager.GetPane ( commandPanel ).IsShown() )
        manager.GetPane ( commandPanel ).Hide();
    manager.Update();
}

bool MyFrame::panelHasFocus()
{
    XmlDoc *doc = getActiveDocument();
    return ( !doc || ( FindFocus() != ( wxWindow * ) doc ) );
}

void MyFrame::OnCut ( wxCommandEvent& event )
{
    if ( panelHasFocus() )
    {
        event.Skip();
        return;
    }

    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;

    if ( protectTags )
        doc->adjustSelection();

    doc->Cut();
    doc->setValidationRequired ( true );
}

void MyFrame::OnCopy ( wxCommandEvent& event )
{
    if ( panelHasFocus() )
    {
        event.Skip();
        return;
    }

    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;
    doc->Copy();
}

void MyFrame::OnPaste ( wxCommandEvent& event )
{
    if ( panelHasFocus() )
    {
        event.Skip();
        return;
    }
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;

    // this has to be handled here to override Scintilla's default Ctrl+V support
    if ( protectTags )
    {
        if ( !wxTheClipboard->Open() || !wxTheClipboard->IsSupported ( wxDF_TEXT ) )
            return;
        wxTextDataObject data;
        wxTheClipboard->GetData ( data );
        wxString buffer = data.GetText();
        xmliseWideTextNode ( buffer );
        doc->adjustCursor();
        doc->AddText ( buffer );
    }
    else
        doc->Paste();

    /*
    XmlDoc *doc;
    doc = getActiveDocument();
    if (doc && protectTags)
      doc->adjustCursor();

    doc->setValidationRequired(true);
    event.Skip(); // new
    */
}

void MyFrame::OnIdle ( wxIdleEvent& event )
{
    wxStatusBar *status = GetStatusBar();
    if ( !status )
        return;

    // update attributes hidden field even if no document loaded
    wxString currentHiddenStatus = status->GetStatusText ( STATUS_HIDDEN );
    if ( visibilityState == HIDE_ATTRIBUTES )
    {
        if ( currentHiddenStatus != _ ( "Attributes hidden" ) )
            status->SetStatusText (
                _ ( "Attributes hidden" ),
                STATUS_HIDDEN );
    }
    else if ( visibilityState == HIDE_TAGS )
    {
        if ( currentHiddenStatus != _ ( "Tags hidden" ) )
            status->SetStatusText (
                _ ( "Tags hidden" ),
                STATUS_HIDDEN );
    }
    else
    {
        if ( !currentHiddenStatus.empty() )
            status->SetStatusText ( wxEmptyString, STATUS_HIDDEN );
    }

    // update protected field even if no document loaded
    wxString currentProtectedStatus = status->GetStatusText ( STATUS_PROTECTED );
    if ( protectTags )
    {
        if ( currentProtectedStatus != _ ( "Tags locked" ) )
            status->SetStatusText (
                _ ( "Tags locked" ),
                STATUS_PROTECTED );
    }
    else
    {
        if ( !currentProtectedStatus.empty() )
            status->SetStatusText ( wxEmptyString, STATUS_PROTECTED );
    }

    // check if document loaded
    wxString frameTitle = GetTitle();

    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
    {
        if ( lastDoc != NULL )
        {
            lastDoc = NULL;
            status->SetStatusText ( wxEmptyString, STATUS_MODIFIED );
            status->SetStatusText ( wxEmptyString, STATUS_POSITION );
            locationPanel->update ( NULL, wxEmptyString );
            insertChildPanel->update ( NULL, wxEmptyString );
            insertSiblingPanel->update ( NULL, wxEmptyString );
            insertEntityPanel->update ( NULL, wxEmptyString );
            wxString minimal = _ ( "XML Copy Editor" );
            if ( frameTitle != minimal )
                SetTitle ( minimal );

            closeFindReplacePane();

            event.Skip();
            manager.Update();
        }
        return;
    }

    if ( restoreFocusToNotebook )
    {
        doc->SetFocus();
        restoreFocusToNotebook = false;
    }

    wxString docTitle;
    if ( doc->getFullFileName().empty() || !showFullPathOnFrame )
        docTitle = doc->getShortFileName();
    else
        docTitle = doc->getFullFileName();

    docTitle += _T ( " - " );
    docTitle += _ ( "XML Copy Editor" );

    if ( frameTitle != docTitle )
        SetTitle ( docTitle );

    // update modified field
    if ( !mainBook )
        return;
    int index = mainBook->GetSelection();

    wxString currentModifiedStatus = status->GetStatusText ( STATUS_MODIFIED );
    wxString currentTabLabel = mainBook->GetPageText ( index );
    if ( doc->GetModify() )
    {
        if ( currentModifiedStatus != _ ( "Modified" ) )
        {
            status->SetStatusText ( _ ( "Modified" ), STATUS_MODIFIED );

            if ( ! ( currentTabLabel.Mid ( 0, 1 ) == _T ( "*" ) ) )
            {
                currentTabLabel.Prepend ( _T ( "*" ) );
                mainBook->SetPageText ( index, currentTabLabel );
            }
        }
    }
    else
    {
        if ( !currentModifiedStatus.empty() )
        {
            status->SetStatusText ( _T ( "" ), STATUS_MODIFIED );

            if ( currentTabLabel.Mid ( 0, 1 ) == _T ( "*" ) )
            {
                currentTabLabel.Remove ( 0, 1 );
                mainBook->SetPageText ( index, currentTabLabel );
            }
        }
    }

    // update coordinates field
    std::pair<int, int> myControlCoordinates;
    int current = doc->GetCurrentPos();
    myControlCoordinates.first = doc->LineFromPosition ( current ) + 1;
    myControlCoordinates.second = doc->GetColumn ( current ) + 1;

    if ( myControlCoordinates != controlCoordinates )
    {
        wxString coordinates;
        coordinates.Printf (
            _ ( "Ln %i Col %i" ),
            myControlCoordinates.first,
            myControlCoordinates.second );
        GetStatusBar()->SetStatusText ( coordinates, STATUS_POSITION );
        controlCoordinates = myControlCoordinates;
    }

    // update parent element field
    wxString parent, grandparent;
    if ( current == lastPos && doc == lastDoc )
        return;

    lastPos = current;
    lastDoc = doc;


    // don't try to find parent if pane is not shown
    if ( !manager.GetPane ( insertChildPanel ).IsShown() && !properties.validateAsYouType )
        return;

    int parentCloseAngleBracket = -1;
    if ( !doc->canInsertAt ( current ) )
        parent = grandparent = wxEmptyString;
    else
    {
        parentCloseAngleBracket = doc->getParentCloseAngleBracket ( current );
        parent = doc->getLastElementName ( parentCloseAngleBracket );
    }

    if ( !parent.empty() && properties.validateAsYouType && doc->getValidationRequired() )
    {
        // tbd: limit to parent element
        doc->shallowValidate ( doc->LineFromPosition ( current ), true );
    }


    if ( parent == lastParent )
        return;
    lastParent = parent;

    bool mustUpdate = false;
    if ( locationPanel && insertChildPanel && insertEntityPanel )
    {
        locationPanel->update ( doc, parent );
        insertChildPanel->update ( doc, parent );
        insertEntityPanel->update ( doc );
        mustUpdate = true;
    }

    if ( parent.empty() )
    {
        if ( insertSiblingPanel )
            insertSiblingPanel->update ( doc, wxEmptyString );
        if ( mustUpdate )
            manager.Update();
        return;
    }

    if ( !manager.GetPane ( insertSiblingPanel ).IsShown() )
    {
        if ( mustUpdate )
            manager.Update();
        return;
    }

    // try to fetch grandparent if necessary/possible
    if ( !parent.empty() && parentCloseAngleBracket != -1 )
    {
        int grandParentCloseAngleBracket;
        grandParentCloseAngleBracket =
            doc->getParentCloseAngleBracket (
                doc->getTagStartPos ( parentCloseAngleBracket ) );
        grandparent = doc->getLastElementName ( grandParentCloseAngleBracket );

        if ( insertSiblingPanel )
            insertSiblingPanel->update ( doc, parent, grandparent );
        if ( grandparent != lastGrandparent )
        {
            mustUpdate = true;
            lastGrandparent = grandparent;
        }

    }
    if ( mustUpdate )
        manager.Update();
}

void MyFrame::OnInsertChild ( wxCommandEvent& event )
{
    if ( !insertChildPanel )
        return;

    wxAuiPaneInfo info = manager.GetPane ( insertChildPanel );
    if ( !info.IsOk() )
    {
        return;
    }
    if ( !info.IsShown() )
    {
        manager.GetPane ( insertChildPanel ).Show ( true );
        manager.Update();
    }
    insertChildPanel->setEditFocus();
}

void MyFrame::OnInsertSibling ( wxCommandEvent& event )
{
    if ( !insertSiblingPanel )
        return;

    wxAuiPaneInfo info = manager.GetPane ( insertSiblingPanel );
    if ( !info.IsOk() )
    {
        return;
    }

    if ( !info.IsShown() )
    {
        manager.GetPane ( insertSiblingPanel ).Show ( true );
        manager.Update();
    }
    insertSiblingPanel->setEditFocus();
}

void MyFrame::OnInsertEntity ( wxCommandEvent& event )
{
    if ( !insertEntityPanel )
        return;

    wxAuiPaneInfo info = manager.GetPane ( insertEntityPanel );
    if ( !info.IsOk() )
    {
        return;
    }

    if ( !info.IsShown() )
    {
        manager.GetPane ( insertEntityPanel ).Show ( true );
        manager.Update();
    }
    insertEntityPanel->setEditFocus();
}

void MyFrame::OnInsertSymbol ( wxCommandEvent& event )
{
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;
    wxSymbolPickerDialog dlg ( _T ( "*" ), wxEmptyString, properties.font, this );

    if ( dlg.ShowModal() == wxID_OK )
    {
        if ( dlg.HasSelection() )
        {
            doc->AddText ( dlg.GetSymbol() );
        }
    }
}

void MyFrame::OnInsertTwin ( wxCommandEvent& event )
{
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;

    wxString parent = doc->getParent();
    if ( !doc->insertSibling ( parent, parent ) )
    {
        wxString msg;
        msg.Printf ( _T ( "Cannot insert twin '%s'" ), parent.c_str() );
        messagePane ( msg, CONST_STOP );
    }
    doc->setValidationRequired ( true );
    doc->SetFocus();
}

void MyFrame::OnPasteNewDocument ( wxCommandEvent& event )
{
    if ( !wxTheClipboard->Open() )
    {
        messagePane ( _ ( "Cannot open clipboard" ), CONST_STOP );
        return;
    }
    if ( !wxTheClipboard->IsSupported ( wxDF_TEXT ) )
    {
        messagePane ( _ ( "Cannot paste as new document: no text on clipboard" ), CONST_STOP );
        return;
    }
    wxTextDataObject data;
    wxTheClipboard->GetData ( data );

    wxString buffer = data.GetText();
    xmliseWideTextNode ( buffer );

    buffer.Prepend ( _T ( "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<root>" ) );
    buffer.Append ( _T ( "</root>\n" ) );

    newDocument ( buffer );
}

void MyFrame::OnDialogFind ( wxFindDialogEvent& event )
{
    findAgain ( event.GetFindString(), event.GetFlags() );
}

void MyFrame::OnDialogReplace ( wxFindDialogEvent& event )
{
    statusProgress ( wxEmptyString );
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;

    int regexWidth = 0;
    if ( findReplacePanel->getRegex() )
    {
        regexWidth = doc->ReplaceTargetRE ( event.GetReplaceString() );
        //doc->SetTargetStart(newLocation + regexWidth);
    }
    else
    {
        doc->ReplaceTarget ( event.GetReplaceString() );
        //doc->SetTargetStart(newLocation + event.GetReplaceString().size());
    }
    /*
    if (doc->GetSelectionStart() != doc->GetSelectionEnd())
      doc->ReplaceSelection(event.GetReplaceString());
    */
    OnDialogFind ( event );
}

void MyFrame::OnDialogReplaceAll ( wxFindDialogEvent& event )
{
    statusProgress ( wxEmptyString );
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;

    int flags = 0;
    if ( event.GetFlags() & wxFR_WHOLEWORD )
        flags |= wxSTC_FIND_WHOLEWORD;
    if ( event.GetFlags() & wxFR_MATCHCASE )
        flags |= wxSTC_FIND_MATCHCASE;
    if ( findReplacePanel->getRegex() )
        flags |= wxSTC_FIND_REGEXP;

    doc->SetTargetStart ( 0 );
    doc->SetTargetEnd ( doc->GetLength() );
    doc->SetSearchFlags ( flags );

    int newLocation, replacementCount, regexWidth;
    newLocation = replacementCount = regexWidth = 0;

    while ( ( newLocation = doc->SearchInTarget ( event.GetFindString() ) ) != -1 )
    {
        if ( findReplacePanel->getRegex() )
        {
            regexWidth = doc->ReplaceTargetRE ( event.GetReplaceString() );
            doc->SetTargetStart ( newLocation + regexWidth );
        }
        else
        {
            doc->ReplaceTarget ( event.GetReplaceString() );
            doc->SetTargetStart ( newLocation + event.GetReplaceString().size() );
        }
        doc->SetTargetEnd ( doc->GetLength() );
        ++replacementCount;
    }
    wxString msg;

    msg.Printf (
        ngettext ( L"%i replacement made", L"%i replacements made", replacementCount ),
        replacementCount );
    statusProgress ( msg );
}

void MyFrame::OnPrintSetup ( wxCommandEvent &WXUNUSED ( event ) )
{
    if ( !htmlPrinting.get() )
        return;
    htmlPrinting->PageSetup();
}

void MyFrame::OnPrintPreview ( wxCommandEvent &WXUNUSED ( event ) )
{
    XmlDoc *doc;
    if ( !htmlPrinting.get() || ( doc = getActiveDocument() ) == NULL )
        return;
    wxString shortFileName, header;
    shortFileName = doc->getShortFileName();
    if ( !shortFileName.empty() )
        header = shortFileName + _T ( " " );
    header += _T ( "(@PAGENUM@/@PAGESCNT@)<hr>" );

    htmlPrinting->SetHeader (
        header,
        wxPAGE_ALL );
    statusProgress ( _ ( "Preparing Print Preview..." ) );
    wxString htmlBuffer = getHtmlBuffer();
    statusProgress ( wxEmptyString );
    if ( ! ( htmlPrinting->PreviewText ( htmlBuffer ) ) )
        ;
}

void MyFrame::OnPrint ( wxCommandEvent &WXUNUSED ( event ) )
{
    XmlDoc *doc;
    if ( !htmlPrinting.get() || ( doc = getActiveDocument() ) == NULL )
        return;
    wxString shortFileName, header;
    shortFileName = doc->getShortFileName();
    if ( !shortFileName.empty() )
        header = shortFileName + _T ( " " );
    header += _T ( "(@PAGENUM@/@PAGESCNT@)<hr>" );

    htmlPrinting->SetHeader (
        header,
        wxPAGE_ALL );
    statusProgress ( _ ( "Preparing to print..." ) );
    wxString htmlBuffer = getHtmlBuffer();
    statusProgress ( wxEmptyString );
    if ( ! ( htmlPrinting->PrintText ( htmlBuffer ) ) )
        ;
}

wxString MyFrame::getHtmlBuffer()
{
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return _T ( "<html><head></head><body></body></html>" );
    wxString buffer, htmlBuffer;
    buffer = doc->GetText();
    size_t size = buffer.size();
    htmlBuffer.reserve ( size * 2 );

    htmlBuffer = _T ( "<html><body><p>" );
    bool startOfLine = true;
    for ( size_t i = 0; i < size; ++i )
    {
        wchar_t c = buffer[i];
        switch ( c )
        {
        case L' ':
            htmlBuffer += ( startOfLine ) ? _T ( "&nbsp;" ) : _T ( " " );
            break;
        case L'\t':
            htmlBuffer += _T ( "&nbsp;&nbsp;" );
            break;
        case L'<':
            htmlBuffer += _T ( "&lt;" );
            startOfLine = false;
            break;
        case L'>':
            htmlBuffer += _T ( "&gt;" );
            startOfLine = false;
            break;
        case L'\n':
            htmlBuffer += _T ( "<br>" );
            startOfLine = true;
            break;
        case L'&':
            htmlBuffer + _T ( "&amp" );
            startOfLine = false;
            break;
        default:
            htmlBuffer += c;
            startOfLine = false;
            break;
        }
    }
    htmlBuffer += _T ( "</p></body></html>" );
    return htmlBuffer;
}

void MyFrame::OnFind ( wxCommandEvent& WXUNUSED ( event ) )
{
#ifdef NEWFINDREPLACE
    manager.GetPane ( findReplacePanel ).Caption ( _ ( "Find" ) );
    bool visible = manager.GetPane ( findReplacePanel ).IsShown();
    if ( !visible )
    {
        manager.GetPane ( findReplacePanel ).Show();
    }
    manager.Update();
    findReplacePanel->refresh();
    findReplacePanel->setReplaceVisible ( false );
    findReplacePanel->focusOnFind();
    return;
#endif

    if ( findDialog.get() )
    {
        findDialog = std::auto_ptr<wxFindReplaceDialog> ( 0 );
    }
    findDialog = ( std::auto_ptr<wxFindReplaceDialog> ( new wxFindReplaceDialog (
                       this,
                       &findData,
                       _ ( "Find" ) ) ) );
    findDialog->Show();
}

void MyFrame::OnImportMSWord ( wxCommandEvent& event )
{
#ifndef __WXMSW__
    messagePane ( _ ( "This functionality requires Microsoft Windows" ) );
    return;
#endif

    std::auto_ptr<wxFileDialog> fd ( new wxFileDialog (
                                         this,
                                         _ ( "Import Microsoft Word Document" ),
                                         _T ( "" ),
                                         _T ( "" ),
                                         _T ( "Microsoft Word (*.doc)|*.doc" ),
                                         wxOPEN | wxFILE_MUST_EXIST | wxCHANGE_DIR
                                         /*
                                         #ifdef __WXMSW__
                                             | wxHIDE_READONLY
                                         #endif
                                         */
                                     ) );
    if ( fd->ShowModal() == wxID_CANCEL )
        return;

    wxString path = fd->GetPath();

    if ( path == _T ( "" ) )
        return;

    importMSWord ( path );
}

void MyFrame::importMSWord ( const wxString& path )
{
#ifndef __WXMSW__
    messagePane ( _ ( "This functionality requires Microsoft Windows" ) );
    return;
#endif

    WrapTempFileName tempFileName ( _T ( "" ) ), swapFileName ( _T ( "" ) );
    wxString completeSwapFileName = swapFileName.wideName() + _T ( ".doc" );
    if ( !wxCopyFile ( path, completeSwapFileName, true ) )
    {
        wxString message;
        message.Printf ( _ ( "Cannot open %s for import" ), path.c_str() );
        messagePane ( message, CONST_STOP );
        return;
    }

    wxString cmd = binDir +
                   _T ( "doc2xml.exe \"" ) +
                   completeSwapFileName + _T ( "\" \"" ) +
                   tempFileName.wideName() + _T ( "\"" );

    statusProgress ( _ ( "Import in progress..." ) );
    int result = wxExecute ( cmd, wxEXEC_SYNC );

    wxRemoveFile ( completeSwapFileName ); // necessary because .doc extension added

    statusProgress ( wxEmptyString );
    wxString message;
    wxString versionMessage (
        _ ( "(lossless conversion requires version 2003 or later)" ) );

    switch ( result )
    {
    case 0:
        break;
    case 1:
        messagePane ( _ ( "Cannot start Microsoft Word" ), CONST_STOP );
        return;
    case 2:
        messagePane (
            _ ( "A more recent version of Microsoft Word is required" ), CONST_STOP );
        return;
    case 3:
        message.Printf ( _T ( "Microsoft Word cannot open %s" ), path.c_str() );
        messagePane ( message + path, CONST_STOP );
        return;
    case 4:
        message.Printf ( _ ( "Microsoft Word cannot save %s as XML" ), path.c_str() );
        messagePane ( message, CONST_STOP );
        return;
    case 5:
        messagePane (
            _ ( "Microsoft Word cannot save this document as WordprocessingML " ) +
            versionMessage,
            CONST_INFO );
        break;
    default:
        break;
    }

    statusProgress ( _ ( "Opening imported file..." ) );
    std::string buffer;

    wxString displayBuffer;

    if ( result != 5 ) // Word 2003 or later
    {
        std::auto_ptr<WrapLibxml> prettyPrinter ( new WrapLibxml ( libxmlNetAccess ) );
        prettyPrinter->parse ( tempFileName.name(), true );
        buffer = prettyPrinter->getOutput();
        displayBuffer = wxString ( buffer.c_str(), wxConvUTF8, buffer.size() );
    }
    else // earlier versions
    {
        if ( !ReadFile::run ( tempFileName.name(), buffer ) )
        {
            statusProgress ( wxEmptyString );
            messagePane ( _ ( "Cannot open imported file" ), CONST_STOP );
            return;
        }
        displayBuffer = wxString ( buffer.c_str(), wxConvUTF8, buffer.size() );
        displayBuffer.Remove ( 0, 1 ); // remove byte order mark
        xmliseWideTextNode ( displayBuffer );

        displayBuffer.Prepend (
            _T ( "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<root>" ) );
        displayBuffer.Append ( _T ( "</root>\n" ) );
    }

    newDocument ( displayBuffer, tempFileName.wideName() );
    statusProgress ( wxEmptyString );
}

void MyFrame::OnExportMSWord ( wxCommandEvent& event )
{
#ifndef __WXMSW__
    messagePane ( _ ( "This functionality requires Microsoft Windows" ) );
    return;
#endif
    statusProgress ( wxEmptyString );

    // fetch document contents
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;

    WrapTempFileName wtfn ( _T ( "" ) );
    wxString sourceFileName = doc->getFullFileName();

    if ( sourceFileName.empty() )
    {
        sourceFileName = wtfn.wideName();
        std::fstream ofs ( wtfn.name().c_str() );
        if ( !ofs )
            return;

        std::string utf8Buffer;
        getRawText ( doc, utf8Buffer );
        ofs << utf8Buffer;
        ofs.close();
    }
    else if ( doc->GetModify() ) //CanUndo())
    {
        modifiedMessage();
        return;
    }

    std::auto_ptr<wxFileDialog> fd ( new wxFileDialog (
                                         this,
                                         _ ( "Export Microsoft Word Document" ),
                                         _T ( "" ),
                                         _T ( "" ),
                                         _T ( "Microsoft Word (*.doc)|*.doc" ),
                                         wxSAVE | wxOVERWRITE_PROMPT ) );
    fd->ShowModal();

    wxString path = fd->GetPath();

    if ( path == _T ( "" ) )
        return;

    wxString cmd = binDir +
                   _T ( "xml2doc.exe -v \"" ) +
                   sourceFileName + _T ( "\" \"" ) +
                   path + _T ( "\"" );

    statusProgress ( _ ( "Export in progress..." ) );
    int result = wxExecute ( cmd, wxEXEC_SYNC );
    statusProgress ( wxEmptyString );
    wxString message;
    switch ( result )
    {
    case 1:
        messagePane ( _ ( "Cannot start Microsoft Word" ), CONST_STOP );
        return;
    case 2:
        messagePane (
            _ ( "A more recent version of Microsoft Word is required" ), CONST_STOP );
        return;
    case 3:
        message.Printf ( _ ( "Microsoft Word cannot save %s" ), path.c_str() );
        messagePane ( message, CONST_STOP );
        return;
    case 0:
        break;
    default:
        break;
    }
}

void MyFrame::OnBrowser ( wxCommandEvent& WXUNUSED ( event ) )
{
    statusProgress ( wxEmptyString );

    // fetch document contents
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;

    wxString sourceFileName = doc->getFullFileName();
    WrapTempFileName wtfn ( sourceFileName, _T ( ".html" ) );

    if ( sourceFileName.empty() || doc->GetModify() )
    {
        sourceFileName = wtfn.wideName();

        std::ofstream ofs ( ( const char * ) wtfn.name().c_str() );
        if ( !ofs )
        {
            messagePane ( _ ( "Cannot save temporary file" ), CONST_STOP );
            return;
        }

        std::string utf8Buffer;
        getRawText ( doc, utf8Buffer );
        ofs << utf8Buffer;
        ofs.close();

        // keep files until application closes
        tempFileVector.push_back ( sourceFileName );
        tempFileVector.push_back ( wtfn.originalWideName() );
        wtfn.setKeepFiles ( true );
    }

    navigate ( sourceFileName );
}

void MyFrame::OnHelp ( wxCommandEvent& event )
{
#ifdef __WXMSW__
    wxString cmd = _T ( "hh.exe \"" ) + helpDir + _T ( "xmlcopyeditor.chm\"" );
    wxExecute ( cmd );
#else
    wxString helpFileName = helpDir + _T ( "xmlcopyeditor.hhp" );
    helpController->AddBook ( wxFileName ( helpFileName ) );
    helpController->DisplayContents();
#endif
}

void MyFrame::OnColorScheme ( wxCommandEvent& event )
{
    int id = event.GetId();
    switch ( id )
    {
    case ID_COLOR_SCHEME_DEFAULT:
        properties.colorScheme = COLOR_SCHEME_DEFAULT;
        break;
    case ID_COLOR_SCHEME_DEFAULT_BACKGROUND:
        properties.colorScheme = COLOR_SCHEME_DEFAULT_BACKGROUND;
        break;
    case ID_COLOR_SCHEME_REDUCED_GLARE:
        properties.colorScheme = COLOR_SCHEME_REDUCED_GLARE;
        break;
    case ID_COLOR_SCHEME_NONE:
        properties.colorScheme = COLOR_SCHEME_NONE;
        break;
    default:
        return;
    }
    colorSchemeMenu->Check ( id, true );

    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;

    properties.zoom = doc->GetZoom(); // ensure temp changes to font size are kept

    applyEditorProperties ( false );
    doc->SetFocus();
}

void MyFrame::OnFontSmaller ( wxCommandEvent& event )
{
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;
    doc->ZoomOut();
    properties.zoom = doc->GetZoom();
    applyEditorProperties ( true );
    doc->SetFocus();
}

void MyFrame::OnFontMedium ( wxCommandEvent& event )
{
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;
    doc->SetZoom ( 0 );
    properties.zoom = doc->GetZoom();
    applyEditorProperties ( true );
    doc->SetFocus();
}

void MyFrame::OnFontLarger ( wxCommandEvent& event )
{
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;
    doc->ZoomIn();
    properties.zoom = doc->GetZoom();
    applyEditorProperties ( true );
    doc->SetFocus();
}

void MyFrame::OnOptions ( wxCommandEvent& WXUNUSED ( event ) )
{
    // ensure font size does not change after
    XmlDoc *doc = getActiveDocument();
    if ( doc )
    {
        properties.zoom = doc->GetZoom();
    }

    wxString title ( _ ( "Options" ) );
    std::auto_ptr<MyPropertySheet> mpsd ( new MyPropertySheet (
                                              this,
                                              properties,
                                              applicationDir,
                                              browserCommand,
                                              rememberOpenTabs,
                                              libxmlNetAccess,
                                              singleInstanceCheck,
                                              saveBom,
                                              unlimitedUndo,
                                              restoreLayout,
                                              expandInternalEntities,
                                              showFullPathOnFrame,
                                              lang,
#ifdef __WXMSW__
                                              useCoolBar,
#endif
                                              wxID_ANY,
                                              title ) );
    if ( mpsd->ShowModal() == wxID_OK )
    {
        properties = mpsd->getProperties();
        applyEditorProperties();
        applicationDir = mpsd->getApplicationDir();
        browserCommand = mpsd->getBrowserCommand();
        rememberOpenTabs = mpsd->getRememberOpenTabs();
        libxmlNetAccess = mpsd->getLibxmlNetAccess();
        singleInstanceCheck = mpsd->getSingleInstanceCheck();
        saveBom = mpsd->getSaveBom();
        unlimitedUndo = mpsd->getUnlimitedUndo();
        restoreLayout = mpsd->getRestoreLayout();
        expandInternalEntities = mpsd->getExpandInternalEntities();
        showFullPathOnFrame = mpsd->getShowFullPathOnFrame();
        lang = mpsd->getLang();
#ifdef __WXMSW__
        useCoolBar = mpsd->getUseCoolBar();
#endif
        updatePaths();
    }
    if ( doc )
        doc->SetFocus();
}

void MyFrame::OnHistoryFile ( wxCommandEvent& event )
{
    wxString f ( history.GetHistoryFile ( event.GetId() - wxID_FILE1 ) );
    if ( !f.empty() )
        openFile ( f );
}

void MyFrame::OnGoto ( wxCommandEvent& WXUNUSED ( event ) )
{
    statusProgress ( wxEmptyString );

    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;

    wxTextEntryDialog *dlg = new wxTextEntryDialog (
                                 this,
                                 _ ( "Enter line number:" ),
                                 _ ( "Go To" ) );
    int ret = dlg->ShowModal();
    if ( ret == wxID_CANCEL )
        return;
    wxString val = dlg->GetValue();
    long line;
    if ( !val.ToLong ( &line ) || line < 1 )
    {
        wxString msg;
        msg.Printf ( _ ( "'%s' is not a valid line number" ), val.c_str() );
        messagePane ( msg, CONST_STOP );
        return;
    }
    --line;
    doc->GotoLine ( ( int ) line );
    doc->SetFocus();
}

void MyFrame::OnFindAgain ( wxCommandEvent& event )
{
    //findAgain(findData.GetFindString(), findData.GetFlags());
    findReplacePanel->OnFindNext ( event );
}

void MyFrame::OnCommand ( wxCommandEvent& WXUNUSED ( event ) )
{
    bool visible = manager.GetPane ( commandPanel ).IsShown();
    if ( !visible )
    {
        manager.GetPane ( commandPanel ).Show();
    }
    manager.Update();
    commandPanel->focusOnCommand();
}

void MyFrame::OnFindReplace ( wxCommandEvent& WXUNUSED ( event ) )
{
#ifdef NEWFINDREPLACE
    manager.GetPane ( findReplacePanel ).Caption ( _ ( "Replace" ) );
    bool visible = manager.GetPane ( findReplacePanel ).IsShown();
    if ( !visible )
    {
        manager.GetPane ( findReplacePanel ).Show();
    }
    manager.Update();
    findReplacePanel->refresh();
    findReplacePanel->setReplaceVisible ( true );
    findReplacePanel->focusOnFind();
    return;
#endif


    if ( findDialog.get() )
    {
        findDialog = std::auto_ptr<wxFindReplaceDialog> ( 0 );
    }
    findDialog = std::auto_ptr<wxFindReplaceDialog> ( new wxFindReplaceDialog (
                     this,
                     &findData,
                     _ ( "Find and Replace" ),
                     wxFR_REPLACEDIALOG ) );
    findDialog->Show();
}

void MyFrame::OnGlobalReplace ( wxCommandEvent& event )
{
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;

    size_t flags = findData.GetFlags();
    std::auto_ptr<GlobalReplaceDialog> grd ( new GlobalReplaceDialog (
                this,
                findData.GetFindString(),
                findData.GetReplaceString(),
                flags & wxFR_MATCHCASE,
                globalReplaceAllDocuments,
                findRegex ) );
    int res = grd->ShowModal();

    flags = 0;
    flags |= wxFR_DOWN;
    if ( grd->getMatchCase() )
        flags |= wxFR_MATCHCASE;
    findRegex = grd->getRegex();
    globalReplaceAllDocuments = grd->getAllDocuments();

    findData.SetFindString ( grd->getFindString() );
    findData.SetReplaceString ( grd->getReplaceString() );
    findData.SetFlags ( flags );
    findReplacePanel->setRegex ( findRegex );
    findReplacePanel->setMatchCase ( flags & wxFR_MATCHCASE );
    findReplacePanel->refresh();

    if ( res != wxID_OK )
    {
        return;
    }

    int globalMatchCount, pageCount;
    globalMatchCount = 0;
    pageCount = mainBook->GetPageCount();
    XmlDoc *currentDoc = getActiveDocument();
    if ( !currentDoc )
        return;

    for ( int i = 0; i < pageCount; ++i )
    {
        std::string bufferUtf8;
        if ( !globalReplaceAllDocuments )
        {
            getRawText ( currentDoc, bufferUtf8 );
        }
        else
        {
            currentDoc = ( XmlDoc * ) mainBook->GetPage ( i );
            if ( !currentDoc )
                return;
            getRawText ( currentDoc, bufferUtf8 );
        }

        size_t flags = findData.GetFlags();
        if ( !findRegex )
        {
            std::string findUtf8, replaceUtf8;
            findUtf8 =findData.GetFindString().mb_str ( wxConvUTF8 );
            replaceUtf8 = findData.GetReplaceString().mb_str ( wxConvUTF8 );
            globalMatchCount += Replace::run (
                                    bufferUtf8,
                                    findUtf8,
                                    replaceUtf8,
                                    flags & wxFR_MATCHCASE );
            currentDoc->SetTextRaw ( bufferUtf8.c_str() );
        }
        else
        {
            try
            {
                std::auto_ptr<WrapRegex> wr ( new WrapRegex (
                                                  ( const char * ) findData.GetFindString().mb_str ( wxConvUTF8 ),
                                                  flags & wxFR_MATCHCASE,
                                                  ( const char * ) findData.GetReplaceString().mb_str ( wxConvUTF8 ) ) );

                int matchCount;
                std::string outputBuffer = wr->replaceGlobal ( bufferUtf8, &matchCount );
                globalMatchCount += matchCount;
                currentDoc->SetTextRaw ( outputBuffer.c_str() );
            }
            catch ( std::exception& e )
            {
                wxString wideError = wxString ( e.what(), wxConvUTF8, strlen ( e.what() ) );
                messagePane ( _ ( "Cannot replace: " ) + wideError, CONST_STOP );
                return;
            }
        }
        if ( !globalReplaceAllDocuments )
            break;
    }
    wxString msg;

    msg.Printf (
        ngettext ( L"%i replacement made", L"%i replacements made", globalMatchCount ),
        globalMatchCount );

    statusProgress ( msg );
}

void MyFrame::OnFrameClose ( wxCloseEvent& event )
{
    wxCommandEvent e;
    OnCloseAll ( e );
    if ( mainBook->GetPageCount() )
    {
        event.Veto();
        return;
    }
    event.Skip();
}

void MyFrame::OnNew ( wxCommandEvent& WXUNUSED ( event ) )
{
    wxString defaultSelection, typeSelection, templateFile;
    defaultSelection = _ ( "XML document (*.xml)" );
    wxArrayString templateArray;
    if ( wxFileName::DirExists ( templateDir ) )
    {
        wxString templateMask, name, extension, entry;
        templateMask = templateDir + wxFileName::GetPathSeparator() + _T ( "*.*" );
        templateFile = wxFindFirstFile ( templateMask, wxFILE );
        wxFileName fn;

        if ( !templateFile.empty() )
        {
            fn.Assign ( templateFile );
            name = fn.GetName();
            extension = fn.GetExt();

            entry.Printf ( _T ( "%s (*.%s)" ), name.c_str(), extension.c_str() );
            templateArray.Add ( entry );

            for ( ;; )
            {
                templateFile = wxFindNextFile();
                if ( templateFile.empty() )
                    break;
                fn.Assign ( templateFile );
                name = fn.GetName();
                extension = fn.GetExt();

                entry.Printf ( _T ( "%s (*.%s)" ), name.c_str(), extension.c_str() );
                templateArray.Add ( entry );
            }
        }
        templateArray.Sort();
        templateArray.Insert ( defaultSelection, 0 );

        const int arraySize = templateArray.GetCount();

        wxString choiceArray[arraySize + 1];
        for ( int i = 0; i < arraySize; ++i )
            * ( choiceArray + i ) = templateArray.Item ( i );

        wxSingleChoiceDialog scd (
            this, _ ( "Choose a document type:" ), _ ( "New Document" ), arraySize, choiceArray );
        if ( scd.ShowModal() == wxID_CANCEL )
        {
            XmlDoc *doc = getActiveDocument();
            if ( doc )
                doc->SetFocus();
            return;
        }
        typeSelection = scd.GetStringSelection();
    }

    if ( typeSelection == defaultSelection )
    {
        wxString emptyString ( _T ( "" ) );
        newDocument ( emptyString );
        return;
    }

    typeSelection.Replace ( _T ( " (*" ), wxEmptyString );
    typeSelection.Replace ( _T ( ")" ), wxEmptyString );
    templateFile = templateDir + typeSelection;
    std::string templateFileLocal, buffer;
    templateFileLocal = templateFile.mb_str ( wxConvLocal );
    ReadFile::run ( templateFileLocal, buffer );
    wxString documentContents = wxString ( buffer.c_str(), wxConvUTF8, buffer.size() );

    newDocument ( documentContents,
                  wxString ( templateFileLocal.c_str(), wxConvUTF8, templateFileLocal.size() ) );
}

void MyFrame::newDocument ( const wxString& s, const wxString& path, bool canSave )
{
    std::string bufferUtf8 = ( const char * ) s.mb_str ( wxConvUTF8 );
    std::string pathUtf8 = ( const char * ) path.mb_str ( wxConvUTF8 );
    newDocument ( bufferUtf8, pathUtf8, canSave );
}

void MyFrame::newDocument ( const std::string& s, const std::string& path, bool canSave )
{
    XmlDoc *doc;

    wxString documentLabel;
    documentLabel.Printf ( _ ( "Document%i" ), documentCount++ );

    std::string auxPath = getAuxPath ( path );

    Freeze();
    doc = ( s.empty() ) ?
          new XmlDoc (
              mainBook,
              properties,
              &protectTags,
              visibilityState,
              FILE_TYPE_XML,
              wxID_ANY,
              NULL, 0 // new: NULL pointer leads to default document
          )
          : new XmlDoc (
              mainBook,
              properties,
              &protectTags,
              visibilityState,
              FILE_TYPE_XML,
              wxID_ANY,
              s.c_str(), // modified
              s.size(), // new
              catalogPath,
              path,
              auxPath );
    mainBook->AddPage ( ( wxWindow * ) doc, documentLabel, true );
    Thaw();

    mainBook->Refresh();
    if ( properties.completion )
        doc->updatePromptMaps();
    doc->setShortFileName ( documentLabel );
    doc->SetFocus();
    manager.Update();
    locationPanel->update ( doc, wxEmptyString );
    insertChildPanel->update ( doc, wxEmptyString );
    insertSiblingPanel->update ( doc, wxEmptyString );
    insertEntityPanel->update ( doc );
    if ( properties.validateAsYouType )
        doc->shallowValidate();
}

void MyFrame::OnOpen ( wxCommandEvent& event )
{
    bool largeFile;
    largeFile = ( event.GetId() == ID_OPEN_LARGE_FILE );

    wxString defaultFile, defaultDir;
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) != NULL )
    {
        defaultFile = doc->getFullFileName();
        if ( !defaultFile.empty() )
        {
            wxFileName fn ( defaultFile );
            defaultDir = fn.GetPath();
        }
    }

    wxFileDialog *fd = new wxFileDialog (
                           this,
                           ( largeFile ) ? _ ( "Open Large Document" ) : _ ( "Open" ),
                           defaultDir,
                           wxEmptyString,
                           FILE_FILTER,
                           wxOPEN | wxMULTIPLE | wxFILE_MUST_EXIST | wxCHANGE_DIR
                       );


    if ( fd->ShowModal() == wxID_CANCEL )
        return;

    wxArrayString paths;
    fd->GetPaths ( paths );
    size_t count = paths.Count();
    if ( !count )
        return;
    for ( size_t i = 0; i < count; ++i )
        if ( !openFile ( paths[i], largeFile ) )
            break;
}

bool MyFrame::openFile ( wxString& fileName, bool largeFile )
{
#ifndef __WXMSW__
    // truncate string up to file:/ portion added by GNOME
    wxString filePrefix = _T ( "file:" );
    int index = fileName.Find ( filePrefix.c_str() );
    if ( index != -1 )
    {
        fileName = fileName.Mid ( ( size_t ) index + filePrefix.Length() );
    }
#endif

    if ( !wxFileName::FileExists ( fileName ) )
    {
        wxString message;
        message.Printf ( _ ( "Cannot open %s" ), fileName.c_str() );
        messagePane ( message, CONST_STOP );
        return false;
    }

    if ( openFileSet.count ( fileName ) )
    {
        wxString message;
        message.Printf ( _ ( "%s is already open" ), fileName.c_str() );
        statusProgress ( message );
        activateTab ( fileName );
        return false;
    }

    wxString directory, name, extension;
    wxFileName::SplitPath ( fileName, NULL, &directory, &name, &extension );

    if ( !extension.empty() )
    {
        name += _T ( "." );
        name += extension;
    }

    wxString wideError;
    std::string buffer;
    pair<int, int> posPair;
    XmlDoc *doc;

    int type = getFileType ( fileName );
    std::string auxPath = getAuxPath ( ( const char * ) fileName.mb_str ( wxConvLocal ) );

    char *docBuffer = 0;
    size_t docBufferLen = 0;
    bool fileEmpty = false;

    statusProgress ( _T ( "Opening file..." ) );
    //wxMemoryMappedFile *memorymap = NULL;
    BinaryFile *binaryfile = NULL;

    try
    {
        binaryfile = new BinaryFile ( ( const char * ) fileName.mb_str ( wxConvLocal ) );
        /*
            memorymap = new wxMemoryMappedFile(
              fileName,
              true, // readOnly
              true // fread
            );
        */

    }
    /*
      catch (wxMemoryMappedFileEmptyException&)
      {
        fileEmpty = true;
      }
    */
    catch ( ... )
    {
        wxString message;
        message.Printf ( _ ( "Cannot open %s" ), fileName.c_str() );
        messagePane ( message, CONST_STOP );
        statusProgress ( wxEmptyString );
        return false;
    }

    bool isUtf8 = false;

    if ( !fileEmpty )
    {
        docBuffer = ( char * ) binaryfile->getData();//memorymap->GetStream();
        docBufferLen = binaryfile->getDataLen();//memorymap->GetMapSize();
    }
    else
    {
        docBuffer = NULL;
        docBufferLen = 0;
        isUtf8 = true;
    }

    statusProgress ( wxEmptyString );

    char *iconvBuffer = 0;
    size_t iconvBufferLen = 0;

    char *finalBuffer;
    size_t finalBufferLen;

    // adjust for UTF-8 BOM
    if ( docBuffer &&
            ( unsigned char ) docBuffer[0] == 0xEF &&
            ( unsigned char ) docBuffer[1] == 0xBB &&
            ( unsigned char ) docBuffer[2] == 0xBF )
    {
        docBuffer += 3;
        docBufferLen -= 3;
        isUtf8 = true;
    }

    // no UTF-8 BOM found
    std::string encoding;
    if ( !isUtf8 || !binaryfile->getDataLen() )
    {
        XmlEncodingSpy es;
        es.parse ( docBuffer, docBufferLen );
        encoding = es.getEncoding();
        if ( encoding == "UTF-8" || encoding == "US-ASCII" ) // US-ASCII is a subset of UTF-8
            isUtf8 = true;
    }

    // convert buffer if not UTF-8
    if ( isUtf8 )
    {
        finalBuffer = docBuffer;
        finalBufferLen = docBufferLen;
    }
    else
    {
        // clear any other BOMs

        if ( docBuffer && // UTF-32 BE
                ( unsigned char ) docBuffer[0] == 0x00 &&
                ( unsigned char ) docBuffer[1] == 0x00 &&
                ( unsigned char ) docBuffer[2] == 0xFE &&
                ( unsigned char ) docBuffer[3] == 0xFF )
        {
            docBuffer += 4;
            docBufferLen -= 4;
        }
        if ( docBuffer && // UTF-32 LE
                ( unsigned char ) docBuffer[0] == 0xFF &&
                ( unsigned char ) docBuffer[1] == 0xFE &&
                ( unsigned char ) docBuffer[2] == 0x00 &&
                ( unsigned char ) docBuffer[3] == 0x00 )
        {
            docBuffer += 4;
            docBufferLen -= 4;
        }

        if ( docBuffer && //UTF-16 BE
                ( unsigned char ) docBuffer[0] == 0xFE &&
                ( unsigned char ) docBuffer[1] == 0xFF )
        {
            docBuffer += 2;
            docBufferLen -= 2;
        }
        if ( docBuffer && //UTF-16 LE
                ( unsigned char ) docBuffer[0] == 0xFF &&
                ( unsigned char ) docBuffer[1] == 0xFE )
        {
            docBuffer += 2;
            docBufferLen -= 2;
        }

        if ( !encoding.size() ) // Expat couldn't parse file (e.g. UTF-32)
        {
            encoding = getApproximateEncoding ( docBuffer, docBufferLen );
        }

        wxString wideEncoding = wxString (
                                    encoding.c_str(),
                                    wxConvLocal,
                                    encoding.size() );
        iconv_t cd = iconv_open ( "UTF-8", encoding.c_str() );
        if ( cd == ( iconv_t )-1 )
        {
            wxString message;
            message.Printf ( _ ( "Cannot open %s: unknown encoding %s" ),
                             fileName.c_str(),
                             wideEncoding.c_str() );
            messagePane ( message, CONST_STOP );
            delete binaryfile;//memorymap;
            return false;
        };

        int iconvLenMultiplier = 4; // worst case scenario
        if ( encoding == "ISO-8859-1" ||
                encoding == "UTF-16" ||
                encoding == "UTF-16BE" ||
                encoding == "UTF-16LE" )
        {
            iconvLenMultiplier = 2;
        }
        else if ( encoding == "UTF-32" ||
                  encoding == "UTF-32BE" ||
                  encoding == "UTF-32LE" )
        {
            iconvLenMultiplier = 1;
        }

        size_t iconvBufferLeft, docBufferLeft;
        iconvBufferLen = iconvBufferLeft = docBufferLen * iconvLenMultiplier + 1;
        docBufferLeft = docBufferLen;
        iconvBuffer = new char[iconvBufferLen];
        finalBuffer = iconvBuffer; // iconvBuffer will be incremented by iconv
        size_t nconv;

        nconv = iconv (
                    cd,
#ifdef __WXMSW__
                    ( const char ** )
#endif
                    &docBuffer,
                    &docBufferLeft,
                    &iconvBuffer,
                    &iconvBufferLeft );

        *iconvBuffer = '\0';

        iconv_close ( cd );

        if ( nconv == ( size_t )-1 )
        {
            wxString message;
            message.Printf ( _ ( "Cannot open %s: conversion from encoding %s failed" ),
                             fileName.c_str(),
                             wideEncoding.c_str() );
            messagePane ( message, CONST_STOP );
            delete[] finalBuffer;
            delete binaryfile; //delete memorymap;
            return false;
        }
        finalBufferLen = iconvBufferLen - iconvBufferLeft;
    }

    statusProgress ( _ ( "Creating document view..." ) );
    Freeze();
    doc = new XmlDoc (
              mainBook,
              ( largeFile ) ? largeFileProperties: properties,
              &protectTags,
              visibilityState,
              ( !binaryfile->getDataLen() ) ? FILE_TYPE_XML : type,
              wxID_ANY,
              finalBuffer,
              finalBufferLen,
              catalogPath,
              ( const char * ) fileName.mb_str ( wxConvLocal ),
              auxPath );
#ifdef __WXMSW__
    doc->SetUndoCollection ( false );
    doc->SetUndoCollection ( true );
#endif

    doc->setFullFileName ( fileName );
    doc->setShortFileName ( name );
    doc->setDirectory ( directory );
    openFileSet.insert ( fileName );
    history.AddFileToHistory ( fileName );
    updateFileMenu();
    wxFileName ofn ( fileName );
    doc->setLastModified ( ofn.GetModificationTime() );

    mainBook->AddPage ( ( wxWindow * ) doc, name, _T ( "" ) );
    Thaw();
    statusProgress ( wxEmptyString );

    mainBook->Refresh();

    wxFileName fn ( fileName );
    doc->setLastModified ( fn.GetModificationTime() );
    doc->SetFocus();

    if ( type != FILE_TYPE_XML || !binaryfile->getDataLen() )
    {
        delete binaryfile;//memorymap;
        return true;
    }

    // NOW parse the document, but don't create a UTF-8 copy
    statusProgress ( _T ( "Parsing document..." ) );
    std::auto_ptr<WrapExpat> we ( new WrapExpat() );

    bool optimisedParseSuccess = false;

    // omit XML declaration
    if ( !isUtf8 && finalBufferLen &&
            finalBuffer[0] == '<' &&
            finalBuffer[1] == '?' &&
            finalBuffer[2] == 'x' &&
            finalBuffer[3] == 'm' &&
            finalBuffer[4] == 'l' )
    {
        for ( ; *finalBuffer && finalBufferLen; finalBuffer++ && finalBufferLen-- )
        {
            if ( *finalBuffer == '>' )
            {
                finalBuffer++;
                finalBufferLen--;
                break;
            }
        }
    }

    if ( finalBuffer )
    {
        optimisedParseSuccess = we->parse ( finalBuffer, finalBufferLen );
        statusProgress ( wxEmptyString );
    }

    // NOW update prompt maps if necessary
    if ( !largeFile && ( properties.completion || properties.validateAsYouType ) )
    {
        statusProgress ( _T ( "Compiling autocompletion lists..." ) );
        doc->updatePromptMaps ( finalBuffer, finalBufferLen );
        statusProgress ( wxEmptyString );
    }

    if ( !largeFile && ( properties.validateAsYouType && doc->getDtdFound() ) )
    {
        statusProgress ( _T ( "Validating document..." ) );
        doc->shallowValidate ( finalBuffer, finalBufferLen );
        statusProgress ( wxEmptyString );
    }

    if ( !optimisedParseSuccess )
    {
        std::string error = we->getLastError();
        wideError = wxString ( error.c_str(), wxConvUTF8, error.size() );
        posPair = we->getErrorPosition();
        -- ( posPair.first );
        messagePane ( wideError, CONST_WARNING );

        int newPosition = doc->PositionFromLine ( posPair.first );
        doc->SetSelection ( newPosition, newPosition );
        doc->SetFocus();
    }
    else
    {
        closePane();
    }
    delete binaryfile; //delete memorymap;
    return true;
}

std::string MyFrame::getApproximateEncoding ( char *docBuffer,
        size_t docBufferLen )
{
    std::string line, encoding;
    char *it;
    size_t i;

    // grab first line
    for (
        i = 0, it = docBuffer;
        i < docBufferLen && *it != '\n' && i < BUFSIZ;
        i++, it++ )
    {
        if ( *it )
            line += *it;
    }

    std::pair<int, int> limits = XmlEncodingHandler::getEncodingValueLimits ( line );

    if ( limits.first == -1 || limits.second == -1 )
        return "";

    return line.substr ( limits.first, limits.second );
}

void MyFrame::OnToggleFold ( wxCommandEvent& WXUNUSED ( event ) )
{
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;
    doc->toggleFold();
}

void MyFrame::OnFoldAll ( wxCommandEvent& WXUNUSED ( event ) )
{
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;
    doc->foldAll();
    doc->SetFocus();
}

void MyFrame::OnUnfoldAll ( wxCommandEvent& WXUNUSED ( event ) )
{
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;
    doc->unfoldAll();
    doc->SetFocus();
}


void MyFrame::OnQuit ( wxCommandEvent& WXUNUSED ( event ) )
{
    Close ( true );
}

void MyFrame::OnUndo ( wxCommandEvent& WXUNUSED ( event ) )
{
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;
    doc->Undo();
    doc->setValidationRequired ( true );
    doc->SetFocus();
}

void MyFrame::OnRedo ( wxCommandEvent& WXUNUSED ( event ) )
{
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;
    doc->Redo();
    doc->setValidationRequired ( true );
    doc->SetFocus();
}

void MyFrame::OnRevert ( wxCommandEvent& WXUNUSED ( event ) )
{
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;

    while ( doc->GetModify() )
    {
        if ( !doc->CanUndo() )
            return;
        doc->Undo();
        doc->setValidationRequired ( true );
    }
    doc->SetFocus();
}

void MyFrame::OnSpelling ( wxCommandEvent& WXUNUSED ( event ) )
{
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;

    statusProgress ( wxEmptyString );
    closePane();

#ifdef __WXMSW__
    doc->SetUndoCollection ( false );
    doc->SetUndoCollection ( true );
#endif


    std::string rawBufferUtf8;
    getRawText ( doc, rawBufferUtf8 );

    // handle unusual encodings
    if ( !XmlEncodingHandler::setUtf8 ( rawBufferUtf8 ) )
    {
        encodingMessage();
        return;
    }

    WrapTempFileName tempFileName ( doc->getFullFileName() );

    ofstream rawBufferStream ( tempFileName.name().c_str() );
    if ( !rawBufferStream )
        return;

    rawBufferStream << rawBufferUtf8;
    rawBufferStream.close();

    auto_ptr<WrapLibxml> wl ( new WrapLibxml ( libxmlNetAccess, catalogPath ) );

    bool success = wl->parse ( tempFileName.name(), true );

    std::string bufferParameterUtf8;

    if ( !success )
    {
        statusProgress ( wxEmptyString );
        std::string error = wl->getLastError();
        wxString wideError = wxString ( error.c_str(), wxConvUTF8, error.size() );
        wideError.Prepend ( _ ( "Opening spelling and style check in read-only mode: " ) );
        messagePane ( wideError, CONST_WARNING );

        if ( !ReadFile::run ( tempFileName.name(), bufferParameterUtf8 ) )
            return;
        std::auto_ptr<WrapExpat> we ( new WrapExpat() );
        bufferParameterUtf8 = we->xmliseTextNode ( bufferParameterUtf8 );
        bufferParameterUtf8.insert ( 0, "<root>" );
        bufferParameterUtf8 += "</root>";
    }
    else
    {
        bufferParameterUtf8 = wl->getOutput();
    }

    auto_ptr<StyleDialog> sd ( new StyleDialog (
                                   this,
                                   wxICON ( appicon ),
                                   bufferParameterUtf8,
                                   doc->getShortFileName(),
                                   ruleSetDir,
                                   filterDir,
                                   browserCommand,
                                   ruleSetPreset,
                                   filterPreset,
                                   ( success ) ? false : true,
                                   stylePosition,
                                   styleSize ) );

    if ( sd->ShowModal() == wxID_OK )
    {
        std::string bufferUtf8 = sd->getEditedString();
        if ( bufferUtf8.empty() )
            messagePane ( _ ( "Edited document empty" ), CONST_STOP );
        else
            doc->SetTextRaw ( bufferUtf8.c_str() );
    }
    // update presets if report has been created (even if followed by cancel)
    ruleSetPreset = sd->getRuleSetPreset();
    filterPreset = sd->getFilterPreset();

#ifdef __WXMSW__
    stylePosition = sd->getPosition();
    styleSize = sd->getSize();
#endif
}

void MyFrame::OnPreviousDocument ( wxCommandEvent& WXUNUSED ( event ) )
{
    if ( !getActiveDocument() )
        return;

    statusProgress ( wxEmptyString );
    closePane();

    int currentSelection = mainBook->GetSelection();
    if ( currentSelection < 1 )
        return;
    mainBook->SetSelection ( --currentSelection );
    XmlDoc *doc = getActiveDocument();
    if ( doc )
        doc->SetFocus();
}

void MyFrame::OnNextDocument ( wxCommandEvent& WXUNUSED ( event ) )
{
    if ( !getActiveDocument() )
        return;

    statusProgress ( wxEmptyString );
    closePane();

    int currentSelection = mainBook->GetSelection();
    int maxSelection = mainBook->GetPageCount();
    if ( currentSelection >= ( maxSelection - 1 ) )
        return;
    mainBook->SetSelection ( ++currentSelection );
    XmlDoc *doc = getActiveDocument();
    if ( doc )
        doc->SetFocus();
}

void MyFrame::OnSave ( wxCommandEvent& event )
{
    save();
}

void MyFrame::save()
{
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;

    /*
      if (!doc->GetModify())
        return;
    */

    wxString fileName;
    if ( ( fileName = doc->getFullFileName() ) == _T ( "" ) )
    {
        wxCommandEvent event;
        OnSaveAs ( event );
        return;
    }

    if ( !saveFile ( doc, fileName, true ) )
        ; // handle messages in saveFile
}

void MyFrame::OnSaveAs ( wxCommandEvent& event )
{
    saveAs();
}

void MyFrame::saveAs()
{
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;

    wxString defaultFile, defaultDir;
    defaultFile = doc->getFullFileName();
    if ( !defaultFile.empty() )
    {
        wxFileName fn ( defaultFile );
        defaultDir = fn.GetPath();
    }
    auto_ptr<wxFileDialog> fd ( new wxFileDialog (
                                    this,
                                    _ ( "Save As" ),
                                    defaultDir,
                                    defaultFile,
                                    FILE_FILTER,
                                    wxSAVE | wxOVERWRITE_PROMPT ) );
    if ( fd->ShowModal() == wxID_CANCEL )
        return;

    wxString path = fd->GetPath();

    if (
        openFileSet.count ( path ) &&
        path != doc->getFullFileName() )
    {
        wxString message;
        message.Printf ( _ ( "%s is already open" ), path.c_str() );
        messagePane ( message, CONST_STOP );
        return;
    }

    wxString name = fd->GetFilename();
    wxString directory;
    wxFileName::SplitPath ( path, &directory, NULL, NULL );
    if ( path == _T ( "" ) )
        return;

    if ( !saveFile ( doc, path, false ) )
        return;

    // if already named, remove from set of open files
    openFileSet.erase ( doc->getFullFileName() );

    doc->setFullFileName ( path );
    doc->setShortFileName ( name );
    doc->setDirectory ( directory );

    history.AddFileToHistory ( path ); // update history
    updateFileMenu();

    int selection;
    if ( ( selection = mainBook->GetSelection() ) == -1 )
        return;
    mainBook->SetPageText ( selection, name );
}

void MyFrame::OnUpdateCloseAll ( wxUpdateUIEvent& event )
{
    event.Enable ( mainBook->GetPageCount() > 1 );
}

void MyFrame::OnUpdateCutCopy ( wxUpdateUIEvent& event )
{
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
    {
        event.Enable ( false );
        return;
    }
    event.Enable ( true );
}

void MyFrame::OnUpdateLocationPaneVisible ( wxUpdateUIEvent& event )
{
    if ( !viewMenu )
        return;
    wxAuiPaneInfo info = manager.GetPane ( locationPanel );
    event.Check ( info.IsShown() );
}

void MyFrame::OnUpdateSavedOnly ( wxUpdateUIEvent& event )
{
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
    {
        event.Enable ( false );
        return;
    }
    event.Enable (
        ( doc->getFullFileName().empty() ) ? false : true );
}

void MyFrame::OnUpdateDocRange ( wxUpdateUIEvent& event )
{
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
    {
        event.Enable ( false );
        return;
    }
    event.Enable ( true );
}

void MyFrame::OnUpdateReplaceRange ( wxUpdateUIEvent& event )
{
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL ) // || protectTags)
    {
        event.Enable ( false );
        return;
    }
    event.Enable ( true );
}

void MyFrame::OnUpdateFindAgain ( wxUpdateUIEvent& event )
{
    if ( !getActiveDocument() || findData.GetFindString().empty() )
    {
        event.Enable ( false );
        return;
    }
    event.Enable ( true );
}

void MyFrame::OnUpdateSaveUndo ( wxUpdateUIEvent& event )
{
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
    {
        event.Enable ( false );
        return;
    }
    //event.Enable((doc->CanUndo()) ? true : false);
    event.Enable ( ( doc->GetModify() ) ? true : false );
}

void MyFrame::OnUpdateRedo ( wxUpdateUIEvent& event )
{
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
    {
        event.Enable ( false );
        return;
    }
    event.Enable ( doc->CanRedo() );
}

void MyFrame::OnUpdatePaste ( wxUpdateUIEvent& event )
{
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
    {
        event.Enable ( false );
        return;
    }
    event.Enable ( true );
}

void MyFrame::OnUpdatePreviousDocument ( wxUpdateUIEvent& event )
{
    if ( !getActiveDocument() )
    {
        event.Enable ( false );
        return;
    }
    int currentDocument = mainBook->GetSelection();
    event.Enable ( ( currentDocument < 1 ) ? false : true );
}

void MyFrame::OnUpdateNextDocument ( wxUpdateUIEvent& event )
{
    if ( !getActiveDocument() )
    {
        event.Enable ( false );
        return;
    }
    int currentDocument = mainBook->GetSelection();
    int maxDocument = mainBook->GetPageCount();
    event.Enable ( ( currentDocument >= ( maxDocument - 1 ) ) ? false : true );
}

void MyFrame::OnUpdateClosePane ( wxUpdateUIEvent& event )
{
    wxAuiPaneInfo i1, i2, i3;
    i1 = manager.GetPane ( htmlReport );
    i2 = manager.GetPane ( findReplacePanel );
    i3 = manager.GetPane ( commandPanel );
    event.Enable ( i1.IsShown() || i2.IsShown() || i3.IsShown() );
}

void MyFrame::OnValidateDTD ( wxCommandEvent& event )
{
    statusProgress ( wxEmptyString );

    // fetch document contents
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;

    updatePaths(); // needed to ensure catalog is available

    wxString fname = doc->getFullFileName();

    WrapTempFileName wtfn ( fname );
    if ( fname.empty() || doc->GetModify() )
    {
        wxString wideBuffer = doc->GetText();

        std::string buffer = ( const char * ) wideBuffer.mb_str ( wxConvUTF8 );
        if ( !saveRawUtf8 (
                    wtfn.name(),
                    buffer ) )
        {
            messagePane (
                _ ( "Cannot save temporary copy for validation; please save or discard changes" ),
                CONST_STOP );
            return;
        }
        fname = wtfn.wideName();
    }

    std::string fnameLocal = ( const char * ) fname.mb_str ( wxConvLocal );

    doc->clearErrorIndicators();
    statusProgress ( _ ( "DTD validation in progress..." ) );

    auto_ptr<WrapLibxml> wl ( new WrapLibxml ( libxmlNetAccess, catalogPath ) );

    if ( !wl->validate ( fnameLocal ) )
    {
        std::string error = wl->getLastError();
        wxString wideError = wxString ( error.c_str(), wxConvUTF8, error.size() );
        statusProgress ( wxEmptyString );
        messagePane ( wideError, CONST_WARNING );

        std::pair<int, int> posPair = wl->getErrorPosition();
        -- ( posPair.first );
        int cursorPos =
            doc->PositionFromLine ( posPair.first );
        doc->SetSelection ( cursorPos, cursorPos );

        // shallow validate all
        doc->shallowValidate(); // has to come first as it deletes all indicators
        doc->setErrorIndicator ( posPair.first, posPair.second );

        return;
    }
    statusProgress ( wxEmptyString );
    documentOk ( _ ( "valid" ) );
}

void MyFrame::OnValidateRelaxNG ( wxCommandEvent& event )
{
    statusProgress ( wxEmptyString );

    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;

    wxString fileName = doc->getFullFileName();

    wxString defaultFile, defaultDir;
    defaultFile = doc->getFullFileName();
    if ( !defaultFile.empty() )
    {
        wxFileName fn ( defaultFile );
        defaultDir = fn.GetPath();
    }
    AssociateDialog ad (
        this,
        _ ( "Select RELAX NG grammar" ),
        _ ( "Choose a file:" ),
        _ ( "RELAX NG grammar" ),
        _T ( "*.*" ),
        lastRelaxNGSchema );
    if ( ad.ShowModal() != wxID_OK )
        return;

    wxString path = lastRelaxNGSchema = ad.getUrl();

    if ( path.empty() )
    {
        statusProgress ( wxEmptyString );
        return;
    }

    validateRelaxNG ( doc, path, fileName );
}

void MyFrame::validateRelaxNG (
    XmlDoc *doc,
    const wxString& schemaName,
    wxString& fileName ) // not const: may change if empty/document modified
{
    statusProgress ( wxEmptyString );

    if ( !doc )
        return;

    WrapTempFileName wtfn ( fileName );
    if ( fileName.empty() || doc->GetModify() )
    {
        wxString wideBuffer = doc->GetText();

        std::string buffer = ( const char * ) wideBuffer.mb_str ( wxConvUTF8 );
        if ( !saveRawUtf8 (
                    wtfn.name(),
                    buffer ) )
        {
            messagePane (
                _ ( "Cannot save temporary copy for validation; please save or discard changes" ),
                CONST_STOP );
            return;
        }
        fileName = wtfn.wideName();
    }

    doc->clearErrorIndicators();
    statusProgress ( _ ( "RELAX NG validation in progress..." ) );

    auto_ptr<WrapLibxml> wl ( new WrapLibxml ( libxmlNetAccess, catalogPath ) );

    std::string schemaFileNameLocal = ( const char * ) schemaName.mb_str ( wxConvLocal );
    std::string fileNameLocal = ( const char * ) fileName.mb_str ( wxConvLocal );
    if ( !wl->validateRelaxNG ( schemaFileNameLocal, fileNameLocal ) )
    {
        std::string error = wl->getLastError();
        wxString wideError = wxString ( error.c_str(), wxConvUTF8, error.size() );
        statusProgress ( wxEmptyString );

        std::pair<int, int> posPair = wl->getErrorPosition();
        -- ( posPair.first );

        int cursorPos =
            doc->PositionFromLine ( posPair.first );
        doc->SetSelection ( cursorPos, cursorPos );
        doc->setErrorIndicator ( posPair.first, posPair.second );
        messagePane ( wideError, CONST_WARNING );
        doc->SetFocus();
        return;
    }

    statusProgress ( wxEmptyString );
    documentOk ( _ ( "valid" ) );
    doc->SetFocus();
}

void MyFrame::OnValidatePreset ( wxCommandEvent& event )
{
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;

    wxString fileName = doc->getFullFileName();

    int id = event.GetId();
    wxString schemaFileName = validationPresetMap[id];
    if ( schemaFileName.empty() )
        return;
    validateRelaxNG ( doc, schemaFileName, fileName );
}

void MyFrame::OnValidateSchema ( wxCommandEvent& event )
{
    statusProgress ( wxEmptyString );

    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;

    /*
      // schema location required
      std::auto_ptr<XmlSchemaLocator> xsl(new XmlSchemaLocator());
      std::string utf8Buffer;
      getRawText(doc, utf8Buffer);
      XmlEncodingHandler::setUtf8(utf8Buffer, true);
      xsl->parse(utf8Buffer);
      std::string utf8Location = xsl->getSchemaLocation();
      if (utf8Location.empty())
      {
        messagePane(_("No XML Schema found"), CONST_STOP);
        return;
      }
    */

    wxString fileName;
    std::string tempFileNameLocal;
    fileName = doc->getFullFileName();

    WrapTempFileName wtfn ( fileName );
    if ( fileName.empty() || doc->GetModify() )
    {
        wxString wideBuffer = doc->GetText();

        std::string buffer = ( const char * ) wideBuffer.mb_str ( wxConvUTF8 );
        if ( !saveRawUtf8 (
                    wtfn.name(),
                    buffer ) )
        {
            messagePane (
                _ ( "Cannot save temporary copy for validation; please save or discard changes" ),
                CONST_STOP );
            return;
        }
        fileName = wtfn.wideName();
    }

    doc->clearErrorIndicators();

    std::string error;
    wxString wideError;
#ifdef __WXMSW__
    // separate WrapTempFileName for output log
    WrapTempFileName tempFileName ( _T ( "" ) );

    wxString cmd = binDir +
                   _T ( "validate.exe \"" ) +
                   fileName + _T ( "\" \"" ) +
                   tempFileName.wideName() + _T ( "\"" );

    statusProgress ( _ ( "Validation in progress..." ) );
    int result = wxExecute ( cmd, wxEXEC_SYNC );
    statusProgress ( wxEmptyString );

    switch ( result )
    {
    case 0:
        documentOk ( _ ( "valid" ) );
        break;
    case 1:
        messagePane ( _ ( "MSXML validation failed (version 4.0 or later required)" ), CONST_STOP );
        break;
    case 2:
        if ( ReadFile::run ( tempFileName.name(), error ) )
        {
            wideError = wxString ( error.c_str(), wxConvLocal, error.size() );
            messagePane ( wideError, CONST_WARNING );
            // tbd: extract line & column numbers; use error indicators
        }
        else
            messagePane ( _ ( "Validation error" ), CONST_WARNING );
        break;
    default:
        break;
    }
    doc->SetFocus();
    return;
#else
    std::auto_ptr<WrapXerces> validator ( new WrapXerces() );
    std::string fileNameLocal = ( const char * ) fileName.mb_str ( wxConvLocal );
    if ( !validator->validate ( fileNameLocal ) )
    {
        error = validator->getLastError();
        wideError = wxString ( error.c_str(), wxConvUTF8, error.size() );
        messagePane ( wideError, CONST_WARNING );
        std::pair<int, int> posPair = validator->getErrorPosition();

        int cursorPos =
            doc->PositionFromLine ( posPair.first - 1 );
        doc->SetSelection ( cursorPos, cursorPos );
        doc->setErrorIndicator ( posPair.first - 1, posPair.second );
    }
    else
        documentOk ( _ ( "valid" ) );

    /*
    // handle relative locations
    schemaPath = PathResolver::run(schemaPath, fileName);

    statusProgress(_("XML Schema validation in progress..."));

    auto_ptr<WrapLibxml> wl(new WrapLibxml(libxmlNetAccess, catalogPath));
    std::string schemaFileNameLocal = (const char *)schemaPath.mb_str(wxConvLocal);
    std::string fileNameLocal = (const char *)fileName.mb_str(wxConvLocal);
    if (!wl->validateW3CSchema(schemaFileNameLocal, fileNameLocal))
    {
    std::string error = wl->getLastError();
    wxString wideError = wxString(error.c_str(), wxConvUTF8, error.size());
    statusProgress(wxEmptyString);

    std::pair<int, int> posPair = wl->getErrorPosition();
    --(posPair.first);

    int cursorPos =
    doc->PositionFromLine(posPair.first - 1);
    doc->SetSelection(cursorPos, cursorPos);
    doc->setErrorIndicator(posPair.first - 1, posPair.second);

    messagePane(wideError, CONST_WARNING);
    return;
    }

    statusProgress(wxEmptyString);
    documentOk(_("valid"));
    */
#endif
}

void MyFrame::OnXPath ( wxCommandEvent& event )
{
    statusProgress ( wxEmptyString );
    closePane();

    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;

    auto_ptr<wxTextEntryDialog> dlg ( new wxTextEntryDialog (
                                          this,
                                          _ ( "Enter XPath:" ),
                                          _ ( "Evaluate XPath" ),
                                          xpathExpression ) );

    int ret = dlg->ShowModal();
    if ( ret == wxID_CANCEL )
        return;
    xpathExpression = dlg->GetValue();
    std::string valUtf8 = ( const char * ) xpathExpression.mb_str ( wxConvUTF8 );

    // fetch document contents
    std::string rawBufferUtf8;
    getRawText ( doc, rawBufferUtf8 );
    if ( !XmlEncodingHandler::setUtf8 ( rawBufferUtf8 ) )
    {
        encodingMessage();
        return;
    }

    WrapTempFileName tempFileName ( doc->getFullFileName() );

    ofstream rawBufferStream ( tempFileName.name().c_str() );
    if ( !rawBufferStream )
        return;
    rawBufferStream << rawBufferUtf8;
    rawBufferStream.close();

    auto_ptr<WrapLibxml> wl ( new WrapLibxml ( libxmlNetAccess, catalogPath ) );

    bool success = wl->xpath ( valUtf8, tempFileName.name() );

    if ( !success )
    {
        std::string error = wl->getLastError();
        wxString wideError = wxString ( error.c_str(), wxConvUTF8, error.size() );
        if ( !wideError.empty() )
            wideError.Prepend ( _T ( ": " ) );
        wideError.Prepend ( _ ( "Cannot evaluate XPath" ) );

        messagePane ( wideError, CONST_WARNING );
        return;
    }

    statusProgress ( wxEmptyString );
    std::string buffer = wl->getOutput();

    if ( buffer.empty() )
    {
        messagePane ( _ ( "No matching nodes found" ), CONST_WARNING );
        return;
    }
    newDocument ( buffer );
    statusProgress ( wxEmptyString );
}

void MyFrame::OnXslt ( wxCommandEvent& event )
{
    statusProgress ( wxEmptyString );
    closePane();

    // fetch document contents
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;
    std::string rawBufferUtf8;
    getRawText ( doc, rawBufferUtf8 );
    if ( !XmlEncodingHandler::setUtf8 ( rawBufferUtf8 ) )
    {
        encodingMessage();
        return;
    }

    WrapTempFileName tempFileName ( doc->getFullFileName() );

    ofstream rawBufferStream ( tempFileName.name().c_str() );
    if ( !rawBufferStream )
        return;
    rawBufferStream << rawBufferUtf8;
    rawBufferStream.close();

    wxString path;

    int id = event.GetId();
    if ( id == ID_XSLT )
    {
        XslLocator xl;
        xl.parse ( rawBufferUtf8 );
        std::string location = xl.getXslLocation();

        path = wxString ( location.c_str(), wxConvUTF8, location.size() );
        path.Replace ( _T ( "%20" ), _T ( " " ), true );

        path = PathResolver::run ( path, doc->getFullFileName() );

        if ( !wxFileName::FileExists ( path ) )
        {
            if ( !path.empty() )
            {
                wxString message;
                message.Printf ( _ ( "Cannot open stylesheet %s" ), path.c_str() );
                messagePane ( message, CONST_WARNING );
            }

            wxString defaultFile, defaultDir;
            defaultFile = doc->getFullFileName();
            if ( !defaultFile.empty() )
            {
                wxFileName fn ( defaultFile );
                defaultDir = fn.GetPath();
            }
            AssociateDialog ad (
                this,
                _ ( "Select stylesheet" ),
                _ ( "Choose a file:" ),
                _ ( "XSLT stylesheet" ),
                _T ( "*.xsl;*.xslt" ),
                lastXslStylesheet );
            if ( ad.ShowModal() != wxID_OK )
                return;

            path = lastXslStylesheet = ad.getUrl();


            if ( path.empty() ) // Cancel selected
            {
                statusProgress ( wxEmptyString );
                return;
            }
        }
    }
    else
    {
        wxString sep;
        sep.Append ( wxFileName::GetPathSeparator() );
        switch ( id )
        {
        case ID_XSLT_TEI_FO:
            path = applicationDir + sep + _T ( "tei" ) + sep + _T ( "fo" ) + sep +
                   _T ( "tei.xsl" );
            break;
        case ID_XSLT_TEI_HTML:
            path = applicationDir + sep + _T ( "tei" ) + sep + _T ( "html" ) + sep +
                   _T ( "tei.xsl" );
            break;
        case ID_XSLT_TEI_XHTML:
            path = applicationDir + sep + _T ( "tei" ) + sep + _T ( "xhtml" ) + sep +
                   _T ( "tei.xsl" );
            break;
        case ID_XSLT_TEI_LATEX:
            path = applicationDir + sep + _T ( "tei" ) + sep + _T ( "latex" ) + sep +
                   _T ( "tei.xsl" );
            break;
        case ID_XSLT_DOCBOOK_FO:
            path = applicationDir + sep + _T ( "docbook" ) + sep + _T ( "fo" ) + sep +
                   _T ( "docbook.xsl" );
            break;
        case ID_XSLT_DOCBOOK_HTML:
            path = applicationDir + sep + _T ( "docbook" ) + sep + _T ( "html" ) + sep +
                   _T ( "docbook.xsl" );
            break;
        case ID_XSLT_DOCBOOK_XHTML:
            path = applicationDir + sep + _T ( "docbook" ) + sep + _T ( "xhtml" ) + sep +
                   _T ( "docbook.xsl" );
            break;
        default:
            break;
        }
    }
    statusProgress ( _ ( "XSL transformation in progress..." ) );

    std::string stylefnameLocal = ( const char * ) path.mb_str ( wxConvLocal );

    auto_ptr<WrapLibxml> wl ( new WrapLibxml ( libxmlNetAccess, catalogPath ) );
    if ( !wl->xslt ( stylefnameLocal, tempFileName.name() ) )
    {
        std::string error = wl->getLastError();
        wxString wideError = wxString ( error.c_str(), wxConvUTF8, error.size() );
        wideError.Prepend ( _ ( "Cannot transform: " ) );
        statusProgress ( wxEmptyString );
        messagePane ( wideError, CONST_WARNING );
        return;
    }
    std::string buffer = wl->getOutput();
    if ( buffer.empty() )
    {
        messagePane ( _ ( "Output document empty" ), CONST_WARNING );
        return;
    }
    statusProgress ( wxEmptyString );
    newDocument ( buffer );
}

void MyFrame::OnPrettyPrint ( wxCommandEvent& event )
{
    statusProgress ( wxEmptyString );
    closePane();

    // fetch document contents
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;

    int line = doc->LineFromPosition ( doc->GetCurrentPos() );

    std::string rawBufferUtf8;
    getRawText ( doc, rawBufferUtf8 );

    std::string encoding = XmlEncodingHandler::get ( rawBufferUtf8 );

    if ( !XmlEncodingHandler::setUtf8 ( rawBufferUtf8, true ) )
    {
        encodingMessage();
        return;
    }

    statusProgress ( _ ( "Pretty-printing in progress..." ) );

    for ( int i = 0; i < 2; i++ ) // perform two iterations
    {
        WrapTempFileName tempFileName ( doc->getFullFileName() );

        ofstream rawBufferStream ( tempFileName.name().c_str() );
        if ( !rawBufferStream )
            return;
        rawBufferStream << rawBufferUtf8;
        rawBufferStream.close();

        auto_ptr<WrapLibxml> wl ( new WrapLibxml ( libxmlNetAccess, catalogPath ) );
        bool success = wl->parse ( tempFileName.name(), true );

        if ( !success )
        {
            std::string error = wl->getLastError();
            wxString wideError = wxString ( error.c_str(), wxConvUTF8, error.size() );
            wideError.Prepend ( _ ( "Cannot pretty-print: " ) );
            statusProgress ( wxEmptyString );
            messagePane ( wideError, CONST_WARNING );
            return;
        }
        rawBufferUtf8 = wl->getOutput();
    }

    statusProgress ( wxEmptyString );
    if ( rawBufferUtf8.empty() )
        messagePane (
            _ ( "Pretty-print unsuccessful: output document empty" ),
            CONST_STOP );
    else
    {
        /*
            if (encoding != "UTF-8")
            {
              std::string output = getEncodedBuffer(rawBufferUtf8, encoding);
              if (!output.empty())
                rawBufferUtf8 = output;
            }
        */
        if ( encoding != "UTF-8" && !encoding.empty() )
        {
            XmlEncodingHandler::set ( rawBufferUtf8, encoding );
        }
        doc->SetTextRaw ( rawBufferUtf8.c_str() );
        statusProgress ( wxEmptyString );
    }

    doc->GotoLine ( line );
    doc->SetFocus();
}

void MyFrame::OnEncoding ( wxCommandEvent& event )
{
    statusProgress ( wxEmptyString );
    closePane();

    // fetch document contents
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;

    std::vector<wxString> encodingVector;
    encodingVector.push_back ( _T ( "UTF-8" ) );
    encodingVector.push_back ( _T ( "UTF-16" ) );
    encodingVector.push_back ( _T ( "UTF-16LE" ) );
    encodingVector.push_back ( _T ( "UTF-16BE" ) );
    encodingVector.push_back ( _T ( "ISO-8859-1" ) );
    encodingVector.push_back ( _T ( "US-ASCII" ) );
    const int vectorSize = encodingVector.size();
    wxString choiceArray[vectorSize + 1];
    for ( int i = 0; i < vectorSize; ++i )
        * ( choiceArray + i ) = encodingVector.at ( i );
    wxSingleChoiceDialog scd (
        this, _ ( "Choose an encoding:" ), _ ( "Encoding" ), vectorSize, choiceArray );

    if ( scd.ShowModal() == wxID_CANCEL )
        return;

    wxString selection;
    std::string selectionUtf8, bufferUtf8;

    selection = scd.GetStringSelection();
    selectionUtf8 = selection.mb_str ( wxConvUTF8 );

    getRawText ( doc, bufferUtf8 );
    XmlEncodingHandler::setUtf8 ( bufferUtf8, true );

    WrapTempFileName tempFileName ( _T ( "" ) );

    std::auto_ptr<WrapLibxml> wl ( new WrapLibxml ( libxmlNetAccess ) );
    int res;

    WrapTempFileName sourceFileName ( doc->getFullFileName() );
    saveRawUtf8 ( sourceFileName.name(), bufferUtf8 );

    res = wl->saveEncodingFromFile ( sourceFileName.name(), tempFileName.name(), selectionUtf8 );
    if ( res == -1 )
    {
        std::string error = wl->getLastError();
        wxString wideError = wxString ( error.c_str(), wxConvUTF8, error.size() );
        wideError.Prepend ( _ ( "Cannot set encoding: " ) );
        messagePane ( wideError, CONST_STOP );
        return;
    }

    std::string newBuffer;
    bool success = ReadFile::run ( tempFileName.name(), newBuffer );
    if ( !success )
    {
        messagePane ( _ ( "Cannot set encoding (cannot open temporary file)" ),
                      CONST_STOP );
        return;
    }

    std::auto_ptr<XmlUtf8Reader> xur ( new XmlUtf8Reader (
                                           false,
                                           expandInternalEntities,
                                           newBuffer.size() ) );
    if ( !xur->parse ( newBuffer ) )
    {
        messagePane ( _ ( "Cannot set encoding (cannot parse temporary file)" ),
                      CONST_STOP );
        return;
    }

    /*
      if (wl->getLastError().empty())
      {
        doc->SetTextRaw(xur->getBuffer().c_str());
      }
      else
      {
        std::string err = wl->getLastError();
        wxString wideErr;
        wideErr = _("Cannot set encoding: ");
        wideErr += wxString(err.c_str(), wxConvUTF8, err.size());

        messagePane(wideErr, CONST_STOP);
      }
    */
    doc->SetTextRaw ( xur->getBuffer().c_str() );
    doc->SetFocus();
}

/*
std::string MyFrame::getEncodedBuffer(
  const std::string& bufferUtf8,
  const std::string& encodingUtf8)
{
  WrapTempFileName tempFileName(_T(""));

  std::auto_ptr<WrapLibxml> wl(new WrapLibxml(libxmlNetAccess));
  int res;

  // change to saveEncodingFromFile?
  res = wl->saveEncoding(bufferUtf8, tempFileName.name(), encodingUtf8);
  if (res == -1)
  {
    std::string error = wl->getLastError();
    wxString wideError = wxString(error.c_str(), wxConvUTF8, error.size());
    wideError.Prepend(_("Cannot set encoding: "));
    messagePane(wideError, CONST_STOP);
    return "";
  }

  std::string newBuffer;
  bool success = ReadFile::run(tempFileName.name(), newBuffer);
  if (!success)
  {
    messagePane(_("Cannot set encoding (cannot open temporary file)"),
      CONST_STOP);
    return "";
  }

  std::auto_ptr<XmlUtf8Reader> xur(new XmlUtf8Reader(
    false,
    expandInternalEntities,
    newBuffer.size()));
  if (!xur->parse(newBuffer))
  {
    messagePane(_("Cannot set encoding (cannot parse temporary file)"),
      CONST_STOP);
    return "";
  }
  return xur->getBuffer().c_str();
}
*/

void MyFrame::OnHome ( wxCommandEvent& event )
{
    navigate ( _T ( "http://xml-copy-editor.sourceforge.net" ) );
}

void MyFrame::OnDownloadSource ( wxCommandEvent& event )
{
    navigate ( _T ( "http://xml-copy-editor.svn.sourceforge.net/viewvc/xml-copy-editor/" ) );
}

void MyFrame::OnToolbarVisible ( wxCommandEvent& event )
{
    if ( !viewMenu )
        return;
#ifdef __WXMSW__
    if ( useCoolBarOnStart )
        toolbarVisible = ( viewMenu->IsChecked ( ID_TOOLBAR_VISIBLE ) ) ? false : true;
    else
        toolbarVisible = ( toolbarVisible ) ? false : true;
#else
    toolbarVisible = ( toolbarVisible ) ? false : true;
#endif
    viewMenu->Check ( ID_TOOLBAR_VISIBLE, toolbarVisible );
    showTopBars ( toolbarVisible );
    manager.Update();
}

void MyFrame::OnWrapWords ( wxCommandEvent& event )
{
    if ( !viewMenu )
        return;

    bool wrapWords;
#ifdef __WXMSW__
    wrapWords = ( viewMenu->IsChecked ( ID_WRAP_WORDS ) ) ? false : true;
#else
    wrapWords = ( properties.wrap ) ? false : true;
#endif

    viewMenu->Check ( ID_WRAP_WORDS, wrapWords );
    properties.wrap = wrapWords;

    // update all documents
    int pageCount = mainBook->GetPageCount();
    XmlDoc *currentDoc;
    for ( int i = 0; i < pageCount; ++i )
    {
        currentDoc = ( XmlDoc * ) mainBook->GetPage ( i );
        if ( !currentDoc )
            break;
        currentDoc->SetWrapMode ( wrapWords );
    }
}

void MyFrame::OnLocationPaneVisible ( wxCommandEvent& event )
{
    wxAuiPaneInfo info = manager.GetPane ( locationPanel );
    bool visible = ( info.IsShown() ) ? false : true;
    manager.GetPane ( locationPanel ).Show ( visible );
    manager.Update();

    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;
    doc->SetFocus();
}

void MyFrame::OnProtectTags ( wxCommandEvent& event )
{
    if ( !xmlMenu )
        return;
#ifdef __WXMSW__
    if ( useCoolBarOnStart )
        protectTags = ( xmlMenu->IsChecked ( ID_PROTECT_TAGS ) ) ? false : true;
    else

        protectTags = ( protectTags ) ? false : true;
#else
    protectTags = ( protectTags ) ? false : true;
#endif
    if ( xmlMenu )
        xmlMenu->Check ( ID_PROTECT_TAGS, protectTags );
    if ( toolBar )
        toolBar->ToggleTool ( ID_PROTECT_TAGS, protectTags );

    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;

    if ( protectTags )
        doc->adjustCursor(); // apply to all open docs?

    doc->SetFocus();
}

void MyFrame::OnVisibilityState ( wxCommandEvent& event )
{

    int id;
    id = event.GetId();
    switch ( id )
    {
    case ID_SHOW_TAGS:
        visibilityState = SHOW_TAGS;
        //GetStatusBar()->SetStatusText(wxEmptyString, STATUS_PARENT);
        break;
    case ID_HIDE_ATTRIBUTES:
        visibilityState = HIDE_ATTRIBUTES;
        //GetStatusBar()->SetStatusText(wxEmptyString, STATUS_PARENT);
        break;
    case ID_HIDE_TAGS:
        visibilityState = HIDE_TAGS;
        break;
    default:
        visibilityState = SHOW_TAGS;
        break;
    }
    if ( viewMenu )
        viewMenu->Check ( id, true );

    // iterate over all open documents
    int pageCount = mainBook->GetPageCount();
    XmlDoc *currentDoc;
    for ( int i = 0; i < pageCount; ++i )
    {
        currentDoc = ( XmlDoc * ) mainBook->GetPage ( i );
        if ( !currentDoc )
            break;
        currentDoc->applyVisibilityState ( visibilityState );
    }

    if ( visibilityState == HIDE_ATTRIBUTES || visibilityState == HIDE_TAGS )
    {
        if ( properties.protectHiddenElements && !protectTags )
        {
            wxCommandEvent e;
            OnProtectTags ( e );
        }
    }

    // fetch current document
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;

    // set focus for current document
    doc->SetFocus();
}

void MyFrame::OnFeedback ( wxCommandEvent& event )
{
    wxString forumUrl =
        _T ( "https://sourceforge.net/forum/forum.php?forum_id=475215" );
    navigate ( forumUrl );
}

void MyFrame::navigate ( const wxString& url )
{
    wxString testString = browserCommand;
    testString.Replace ( _T ( " -remote" ), wxEmptyString, true );
    if ( browserCommand.empty() )
    {
        messagePane (
            _ ( "Cannot open in browser: no browser defined (see Tools, Options..., General)" ),
            CONST_STOP );
        return;
    }
    else if ( !wxFileName::FileExists ( testString ) )
    {
        wxString message;
        message.Printf (
            _ ( "Cannot open in browser: %s not found (see Tools, Options..., General)" ),
            testString.c_str() );
        messagePane ( message, CONST_STOP );
        return;
    }
    wxString cmd;
    cmd = browserCommand + _T ( " \"" ) + url + _T ( "\"" );
    wxExecute ( cmd, wxEXEC_ASYNC ); // make ASYNC an option?
}

void MyFrame::findAgain ( wxString s, int flags )
{
    findReplacePanel->flagNotFound ( false );

    if ( s.empty() )
        return;

    statusProgress ( wxEmptyString );
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;

    // update regex parameter to keep global replace in sync
    findRegex = findReplacePanel->getRegex();

    int newLocation;
    int myFlags = 0;
    if ( flags & wxFR_WHOLEWORD )
        myFlags |= wxSTC_FIND_WHOLEWORD;
    if ( flags & wxFR_MATCHCASE )
        myFlags |= wxSTC_FIND_MATCHCASE;
    if ( findReplacePanel->getRegex() )
        myFlags |= wxSTC_FIND_REGEXP;

    bool incrementalFind =
        ( findReplacePanel->getIncrementalFind() ) ? true : false;

    //doc->SetYCaretPolicy(wxSTC_CARET_SLOP | wxSTC_CARET_STRICT, 10);

    if ( flags & wxFR_DOWN ) // find next
    {
        doc->SetTargetStart ( ( incrementalFind ) ?
                              doc->GetSelectionStart() : doc->GetSelectionEnd() );
        doc->SetTargetEnd ( doc->GetLength() );
        doc->SetSearchFlags ( myFlags );
        newLocation = doc->SearchInTarget ( s );

        // try once more from top
        if ( newLocation == -1 )
        {
            doc->SetTargetStart ( 0 );
            doc->SetTargetEnd ( doc->GetLength() );
            newLocation = doc->SearchInTarget ( s );
        }
    }
    else // find previous
    {
        doc->SetCurrentPos (
            ( doc->GetSelectionStart() ) ? doc->GetSelectionStart() - 1 : 0 );
        doc->SearchAnchor();
        newLocation = doc->SearchPrev ( myFlags, s );
    }

    //doc->SetYCaretPolicy(wxSTC_CARET_SLOP, 0);


    if ( newLocation == -1 )
    {
        findReplacePanel->flagNotFound ( true );
        wxString err;
        err.Printf ( _ ( "Cannot find '%s'" ), s.c_str() );
        doc->SetSelectionEnd ( doc->GetSelectionStart() );

        statusProgress ( err );

        // must clear target to prevent replace affecting whole document
        doc->SetTargetStart ( 0 );
        doc->SetTargetEnd ( 0 );

        return;
    }

    doc->SetSelection ( doc->GetTargetStart(), doc->GetTargetEnd() );
    doc->EnsureCaretVisible();
}

bool MyFrame::closeActiveDocument()
{
    statusProgress ( wxEmptyString );
    closePane();

    int selection = mainBook->GetSelection();
    if ( selection == -1 || !mainBook->GetPageCount() ) // GetPageCount needed for wxAuiNotebook
        return false;

    locationPanel->update ( NULL, wxEmptyString );
    insertChildPanel->update ( NULL, wxEmptyString );
    insertSiblingPanel->update ( NULL, wxEmptyString );

    // workaround -- wxAuiNotebook: send virtual close event? DeletePage doesn't generate one
    wxAuiNotebookEvent e;
    e.SetSelection ( selection );
    OnPageClosing ( e );
    if ( deletePageVetoed )
        return false;

    mainBook->DeletePage ( selection );
    return true;

    // was:
    // mainBook->DeletePage(selection);
    // return (!deletePageVetoed);
}

bool MyFrame::saveFile ( XmlDoc *doc, wxString& fileName, bool checkLastModified )
{
    if ( !doc )
        return false;

    statusProgress ( wxEmptyString );

    if ( checkLastModified )
    {
        wxFileName fn ( fileName );
        if ( fn.IsOk() )
        {
            wxDateTime myLastModified = fn.GetModificationTime();
            if ( !myLastModified.IsEqualTo ( doc->getLastModified() ) )
            {
                int choice = wxMessageBox (
                                 _ ( "File has been modified by another application.\nDo you want to proceed?" ),
                                 _ ( "Confirmation" ),
                                 wxICON_QUESTION | wxYES_NO | wxCANCEL );
                if ( choice != wxYES )
                    return false;
            }
        }
    }

    int bytes = 0;
    std::string utf8Buffer, encoding, fileNameLocal;
    bool isXml = true;
    try
    {
        getRawText ( doc, utf8Buffer );
        XmlEncodingSpy es;
        es.parse ( utf8Buffer );
        encoding = es.getEncoding();

        fileNameLocal = fileName.mb_str ( wxConvLocal );

        closePane();
        bool success;
        success = true;
        if ( getFileType ( fileName ) != FILE_TYPE_XML )
        {
            isXml = false;
        }

        // raw file conditions
        if ( doc->getType() == FILE_TYPE_BINARY )
        {
            success = saveRawUtf8 ( fileNameLocal, utf8Buffer, true, isXml );
            if ( success )
                bytes = utf8Buffer.size();
        }
        else if ( !isXml && encoding.empty() )
        {
            success = saveRawUtf8 ( fileNameLocal, utf8Buffer, true, isXml );
            if ( success )
                bytes = utf8Buffer.size();
        }
        else if ( encoding == "UTF-8" )
        {
            auto_ptr<WrapExpat> we ( new WrapExpat() );

            if ( !we->parse ( utf8Buffer ) )
            {
                std::string error = we->getLastError();
                wxString werror = wxString ( error.c_str(), wxConvUTF8, error.size() );
                if ( we->isEncodingError() )
                    ;
                messagePane ( werror, CONST_WARNING );
            }
            success = saveRawUtf8 ( fileNameLocal, utf8Buffer, true, isXml );
            if ( success )
                bytes = utf8Buffer.size();
        }
        else
        {
            wxString wideEncoding = wxString ( encoding.c_str(), wxConvLocal, encoding.size() );

            // IF Unicode, use iconv to convert buffer
            if ( encoding == "UTF-16" || encoding == "UTF-16LE" ||
                    encoding == "UTF-16BE" || encoding == "UTF-32" || encoding == "UTF-32LE" ||
                    encoding == "UTF-32BE" )
            {
                iconv_t cd = iconv_open ( encoding.c_str(), "UTF-8" );
                if ( cd == ( iconv_t )-1 )
                {
                    success = saveRawUtf8 ( fileNameLocal, utf8Buffer, false, isXml );
                    if ( success )
                    {
                        bytes = utf8Buffer.size();
                        wxString message;
                        message.Printf (
                            _ ( "%s saved in default encoding UTF-8: unknown encoding %s" ),
                            fileName.c_str(),
                            wideEncoding.c_str() );
                        messagePane ( message, CONST_WARNING );
                    }
                    else
                        return false;
                }
                else
                {
                    size_t utf8BufferLen = utf8Buffer.size();

                    size_t iconvBufferLen, iconvBufferLeft, utf8BufferLeft;
                    int iconvLenMultiplier = 4; // worst case scenario
                    if ( encoding == "UTF-16" ||
                            encoding == "UTF-16BE" ||
                            encoding == "UTF-16LE" )
                    {
                        iconvLenMultiplier = 2;
                    }
                    else if ( encoding == "UTF-32" ||
                              encoding == "UTF-32BE" ||
                              encoding == "UTF-32LE" )
                    {
                        iconvLenMultiplier = 4;
                    }

                    iconvBufferLen = iconvBufferLeft =
                                         utf8BufferLen * iconvLenMultiplier + 4; // worst case scenario

                    char *finalBuffer;
                    char *iconvBuffer = new char[iconvBufferLen];

                    utf8BufferLeft = utf8BufferLen;
                    iconvBuffer = new char[iconvBufferLen];
                    finalBuffer = iconvBuffer; // iconvBuffer will be incremented by iconv
                    size_t nconv;

#ifdef __WXMSW_
                    const char *
#else
                    char *
#endif
                    utf8BufferPtr =
                        ( char * )
                        utf8Buffer.c_str();

                    nconv = iconv (
                                cd,
#ifdef __WXMSW__
                                ( const char ** )
#endif
                                &utf8BufferPtr,
                                &utf8BufferLeft,
                                &iconvBuffer,
                                &iconvBufferLeft );

                    iconv_close ( cd );

                    if ( nconv == ( size_t )-1 ) // conversion failed
                    {
                        delete[] finalBuffer;
                        success = saveRawUtf8 ( fileNameLocal, utf8Buffer, false, isXml );
                        if ( success )
                        {
                            bytes = utf8Buffer.size();
                            wxString message;
                            message.Printf (
                                _ ( "%s saved in default encoding UTF-8: conversion to %s failed" ),
                                fileName.c_str(),
                                wideEncoding.c_str() );
                            messagePane ( message, CONST_WARNING );
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else
                    {
                        size_t finalBufferLen = iconvBufferLen - iconvBufferLeft;

                        std::ofstream ofs ( fileNameLocal.c_str(), std::ios::out | std::ios::binary );
                        if ( !ofs )
                        {
                            delete[] finalBuffer;
                            return false;
                        }

                        // iconv adds boms for UTF-16 & UTF-32 automatically

                        ofs.write ( finalBuffer, finalBufferLen );
                        ofs.close();
                        delete[] finalBuffer;
                        bytes = finalBufferLen;
                    }
                }
            }
            else // all other encodings handled by Libxml
            {
                XmlEncodingHandler::setUtf8 ( utf8Buffer );
                auto_ptr<WrapLibxml> wl ( new WrapLibxml ( libxmlNetAccess ) );

                WrapTempFileName sourceFileName ( fileName );
                saveRawUtf8 ( sourceFileName.name(), utf8Buffer );
                int result = wl->saveEncodingFromFile ( sourceFileName.name(), fileNameLocal, encoding );
                if ( result == -1 )
                {
                    success = saveRawUtf8 ( fileNameLocal, utf8Buffer, false, isXml );
                    if ( success )
                    {
                        std::string libxmlError = wl->getLastError();
                        bytes = utf8Buffer.size();
                        wxString msg, wideEncoding, wideError;
                        wideEncoding =
                            wxString ( encoding.c_str(), wxConvUTF8, encoding.size() );
                        wideError = wxString ( libxmlError.c_str(), wxConvUTF8, libxmlError.size() );
                        if ( wideError.empty() )
                            wideError = _ ( "unknown error" );

                        msg.Printf ( _ ( "Cannot save document in %s: %s (saved in default encoding UTF-8)" ),
                                     wideEncoding.c_str(), wideError.c_str() );
                        messagePane ( msg, CONST_INFO );
                    }
                }
                else
                    bytes = result;
            }
        }
    } // try
    catch ( std::bad_alloc& )
    {
        if ( encoding != "UTF-8" )
        {
            int answer = wxMessageBox (
                             _ ( "Out of memory: attempt to save in default encoding UTF-8?" ),
                             _ ( "Out of memory" ),
                             wxYES_NO | wxCANCEL | wxICON_QUESTION,
                             this );
            if ( answer == wxCANCEL || answer == wxNO )
                return false;

            bool success = saveRawUtf8 ( fileNameLocal, utf8Buffer, true, isXml );
            if ( success )
            {
                bytes = utf8Buffer.size();
                wxString message;
                message.Printf (
                    _ ( "%s saved in default encoding UTF-8" ),
                    fileName.c_str() );
                messagePane ( message, CONST_INFO );
            }
            else
            {
                wxString message;
                message.Printf ( _ ( "Cannot save %s" ), fileName.c_str() );
                messagePane ( message, CONST_STOP );
                return false;
            }
        }
    }

    doc->SetFocus();
    doc->SetSavePoint();

    if ( properties.validateAsYouType && isXml )
        doc->shallowValidate ( utf8Buffer.c_str(), utf8Buffer.size() );

    if ( !unlimitedUndo )
        doc->EmptyUndoBuffer();
    wxFileName fn ( fileName );
    if ( fn.IsOk() )
        doc->setLastModified ( fn.GetModificationTime() );
    openFileSet.insert ( fileName );
    displaySavedStatus ( bytes );
    return true;
}

bool MyFrame::saveRawUtf8 (
    const std::string& fileNameLocal,
    std::string& bufferUtf8,
    bool ignoreEncoding,
    bool isXml )
{
    ofstream ofs ( fileNameLocal.c_str(), std::ios::out | std::ios::binary );
    if ( !ofs )
        return false;

    if ( !ignoreEncoding && isXml )
        XmlEncodingHandler::setUtf8 ( bufferUtf8, true );

    if ( saveBom && isXml )
    {
        char bom[4];
        bom[0] = 0xEF;
        bom[1] = 0xBB;
        bom[2] = 0xBF;
        bom[3] = 0;
        ofs.write ( bom, 3 );
    }
    ofs.write ( bufferUtf8.c_str(), bufferUtf8.size() );
    ofs.close();
    return true;
}

void MyFrame::displaySavedStatus ( int bytes )
{
    wxString unit;
    float result = 0;
    if ( bytes > 1000000 )
    {
        result = bytes / 1000000;
        unit = _ ( "MB" );
    }
    else if ( bytes > 1000 )
    {
        result = bytes / 1000;
        unit = _ ( "kB" );
    }
    else if ( bytes >= 0 )
    {
        result = bytes;
        unit = ngettext ( L"byte", L"bytes", bytes );
    }
    else
        return;

    wxString msg;

    msg.Printf (
        _ ( "%g %s saved" ),
        result,
        unit.c_str() );
    statusProgress ( msg );
}

bool MyFrame::xpThemeActive()
{
#ifndef __WXMSW__
    return false;
#else
    return ( wxUxThemeEngine::Get() && wxUxThemeEngine::Get()->IsThemeActive() );
#endif
}

bool MyFrame::getHandleCommandLineFlag()
{
    return handleCommandLineFlag;
}

MyMenuBar *MyFrame::getMenuBar()
{
    fileMenu = new wxMenu; // use class-wide data member
    updateFileMenu ( false );

    // edit menu
    wxMenu *editMenu = new wxMenu;

    wxMenuItem *undoItem =
        new wxMenuItem ( NULL, wxID_UNDO, _ ( "&Undo\tCtrl+Z" ), _ ( "Undo" ) );
    undoItem->SetBitmap ( undo16Bitmap );

    wxMenuItem *redoItem =
        new wxMenuItem ( NULL, wxID_REDO, _ ( "&Redo\tCtrl+Y" ), _ ( "Redo" ) );
    redoItem->SetBitmap ( redo16Bitmap );

    wxMenuItem *cutItem =
        new wxMenuItem ( NULL, wxID_CUT, _ ( "&Cut\tCtrl+X" ), _ ( "Cut" ) );
    cutItem->SetBitmap ( cutBitmap );

    wxMenuItem *copyItem =
        new wxMenuItem ( NULL, wxID_COPY, _ ( "C&opy\tCtrl+C" ), _ ( "Copy" ) );
    copyItem->SetBitmap ( copyBitmap );

    wxMenuItem *pasteItem =
        new wxMenuItem ( NULL, wxID_PASTE, _ ( "&Paste\tCtrl+V" ), _ ( "Paste" ) );
    pasteItem->SetBitmap ( pasteBitmap );

    wxMenuItem *pasteNewDocumentItem =
        new wxMenuItem (
            NULL,
            ID_PASTE_NEW_DOCUMENT,
            _ ( "P&aste As New Document" ),
            _ ( "Paste As New Document" ) );
    pasteNewDocumentItem->SetBitmap ( wxNullBitmap );

    wxMenuItem *findItem =
        new wxMenuItem ( NULL, ID_FIND, _ ( "&Find...\tCtrl+F" ), _ ( "Find..." ) );
    findItem->SetBitmap ( searchBitmap );

    wxMenuItem *findAgainItem =
        new wxMenuItem ( NULL, ID_FIND_AGAIN, _ ( "F&ind Again\tF3" ), _ ( "Find Again" ) );
    findAgainItem->SetBitmap ( wxNullBitmap );

    wxMenuItem *replaceItem =
        new wxMenuItem ( NULL, ID_REPLACE, _ ( "&Replace...\tCtrl+R" ), _ ( "Replace..." ) );
    replaceItem->SetBitmap ( wxNullBitmap );

    wxMenuItem *globalReplaceItem =
        new wxMenuItem (
            NULL,
            ID_GLOBAL_REPLACE,
            _ ( "&Global Replace...\tCtrl+Shift+R" ),
            _ ( "Global Replace..." ) );
    globalReplaceItem->SetBitmap ( wxNullBitmap );

    wxMenuItem *gotoItem =
        new wxMenuItem ( NULL, ID_GOTO, _ ( "G&o To...\tCtrl+G" ), _ ( "Go To..." ) );
    gotoItem->SetBitmap ( wxNullBitmap );

    editMenu->Append ( undoItem );
    editMenu->Append ( redoItem );
    editMenu->AppendSeparator();
    editMenu->Append ( cutItem );
    editMenu->Append ( copyItem );
    editMenu->Append ( pasteItem );
    editMenu->Append ( pasteNewDocumentItem );
    editMenu->AppendSeparator();
    editMenu->Append ( findItem );
    editMenu->Append ( findAgainItem );
    editMenu->Append ( replaceItem );
    editMenu->Append ( globalReplaceItem );
    editMenu->AppendSeparator();
    editMenu->Append ( gotoItem );

    // font size menu
    wxMenu *fontSizeMenu = new wxMenu;
    fontSizeMenu->Append (
        ID_FONT_LARGER, _ ( "Increase\tCtrl+U" ), _ ( "Increase" ) );
    fontSizeMenu->Append (
        ID_FONT_SMALLER, _ ( "Decrease\tCtrl+D" ), _ ( "Decrease" ) );
    fontSizeMenu->AppendSeparator();
    fontSizeMenu->Append ( ID_FONT_NORMAL, _ ( "Normal\tCtrl+0" ), _ ( "Normal" ) );

    // color scheme menu
    colorSchemeMenu = new wxMenu;
    colorSchemeMenu->AppendRadioItem (
        ID_COLOR_SCHEME_DEFAULT, _ ( "&Default" ), _ ( "Default" ) );
    colorSchemeMenu->AppendRadioItem (
        ID_COLOR_SCHEME_REDUCED_GLARE,
        _ ( "&Blue background, white text" ),
        _ ( "Blue background, white text" ) );
    colorSchemeMenu->AppendRadioItem (
        ID_COLOR_SCHEME_DEFAULT_BACKGROUND,
        _ ( "&Light" ),
        _ ( "Light" ) );
    colorSchemeMenu->AppendRadioItem (
        ID_COLOR_SCHEME_NONE,
        _ ( "&None" ),
        _ ( "None" ) );

    switch ( properties.colorScheme )
    {
    case COLOR_SCHEME_DEFAULT:
        colorSchemeMenu->Check ( ID_COLOR_SCHEME_DEFAULT, true );
        break;
    case COLOR_SCHEME_DEFAULT_BACKGROUND:
        colorSchemeMenu->Check ( ID_COLOR_SCHEME_DEFAULT_BACKGROUND, true );
        break;
    case COLOR_SCHEME_REDUCED_GLARE:
        colorSchemeMenu->Check ( ID_COLOR_SCHEME_REDUCED_GLARE, true );
        break;
    case COLOR_SCHEME_NONE:
        colorSchemeMenu->Check ( ID_COLOR_SCHEME_NONE, true );
        break;
    default:
        break;
    }

    viewMenu = new wxMenu; // use class-wide data member
    viewMenu->Append ( ID_PREVIOUS_DOCUMENT, _ ( "&Previous Document\tCtrl+PgUp" ), _ ( "Previous Document" ) );
    viewMenu->Append ( ID_NEXT_DOCUMENT, _ ( "&Next Document\tCtrl+PgDn" ), _ ( "Next Document" ) );
    viewMenu->Append ( ID_BROWSER, _ ( "&Browser\tCtrl+B" ), _ ( "Browser" ) );
    viewMenu->AppendSeparator();
    viewMenu->AppendRadioItem (
        ID_SHOW_TAGS,
        _ ( "&Show Tags and Attributes\tCtrl+T" ), _ ( "Show Tags and Attributes" ) );
    viewMenu->AppendRadioItem (
        ID_HIDE_ATTRIBUTES,
        _ ( "&Hide Attributes Only\tCtrl+Shift+A" ), _ ( "Hide Attributes Only" ) );
    viewMenu->AppendRadioItem (
        ID_HIDE_TAGS,
        _ ( "H&ide Tags and Attributes\tCtrl+Shift+T" ), _ ( "Hide Tags and Attributes" ) );

    switch ( visibilityState )
    {
    case SHOW_TAGS:
        viewMenu->Check ( ID_SHOW_TAGS, true );
        break;
    case HIDE_TAGS:
        viewMenu->Check ( ID_HIDE_TAGS, true );
        break;
    case HIDE_ATTRIBUTES:
        viewMenu->Check ( ID_HIDE_ATTRIBUTES, true );
        break;
    default:
        viewMenu->Check ( ID_SHOW_TAGS, true );
        break;
    }
    viewMenu->AppendSeparator();
    viewMenu->Append (
        ID_TOGGLE_FOLD, _ ( "&Toggle Fold" ), _ ( "Toggle Fold" ) );
    viewMenu->Append (
        ID_FOLD_ALL, _ ( "&Fold Tags\tCtrl+Shift+F" ), _ ( "Fold Tags" ) );
    viewMenu->Append (
        ID_UNFOLD_ALL, _ ( "&Unfold Tags\tCtrl+Shift+U" ), _T ( "Unfold Tags" ) );
    viewMenu->AppendSeparator();
    viewMenu->AppendCheckItem (
        ID_WRAP_WORDS, _ ( "&Wrap Words\tCtrl+W" ), _T ( "Wrap Words" ) );
    viewMenu->Check ( ID_WRAP_WORDS, properties.wrap );
    viewMenu->Append ( wxID_ANY, _ ( "&Color Scheme" ), colorSchemeMenu );
    viewMenu->Append ( wxID_ANY, _ ( "&Text Size" ), fontSizeMenu );
    viewMenu->AppendSeparator();

    viewMenu->AppendCheckItem (
        ID_LOCATION_PANE_VISIBLE,
        _ ( "S&how Current Element Pane" ),
        _ ( "Show Current ElementPane" ) );
    viewMenu->Check ( ID_LOCATION_PANE_VISIBLE, false );
    viewMenu->AppendCheckItem (
        ID_TOOLBAR_VISIBLE, _ ( "Sh&ow Toolbar" ), _ ( "Show Toolbar" ) );
    viewMenu->Check ( ID_TOOLBAR_VISIBLE, toolbarVisible );
    viewMenu->Append (
        ID_HIDE_PANE, _ ( "C&lose Message Pane\tAlt+C" ), _ ( "Close Message Pane" ) );

    // insert menu
    wxMenu *insertMenu = new wxMenu;
    insertMenu->Append ( ID_INSERT_CHILD, _ ( "&Element...\tCtrl+I" ), _ ( "Element..." ) );
    insertMenu->Append ( ID_INSERT_SIBLING, _ ( "&Sibling...\tCtrl+Shift+I" ), _ ( "Sibling..." ) );
    insertMenu->Append ( ID_INSERT_ENTITY, _ ( "&Entity...\tCtrl+E" ), _ ( "Entity..." ) );
    insertMenu->AppendSeparator();
    insertMenu->Append ( ID_INSERT_TWIN, _ ( "&Twin\tCtrl+Enter" ), _ ( "Twin" ) );
    insertMenu->AppendSeparator();
    insertMenu->Append ( ID_INSERT_SYMBOL, _ ( "S&ymbol..." ), _ ( "Symbol..." ) );

    // validation menu
    wxMenu *validationMenu = new wxMenu;
    validationMenu->Append ( ID_VALIDATE_DTD, _ ( "&DTD\tF4" ), _ ( "DTD" ) );
    validationMenu->Append (
        ID_VALIDATE_W3C_SCHEMA, _ ( "&XML Schema\tF5" ), _ ( "XML Schema" ) );
    validationMenu->AppendSeparator();
    validationMenu->Append (
        ID_VALIDATE_RELAX_NG, _ ( "&RELAX NG...\tF6" ), _ ( "RELAX NG..." ) );

    wxMenu *associateMenu = new wxMenu;
    associateMenu->Append ( ID_ASSOCIATE_DTD_PUBLIC, _ ( "&Public DTD..." ), _ ( "Public DTD..." ) );
    associateMenu->Append ( ID_ASSOCIATE_DTD_SYSTEM, _ ( "&System DTD..." ), _ ( "System DTD..." ) );
    associateMenu->Append ( ID_ASSOCIATE_W3C_SCHEMA, _ ( "&XML Schema..." ), _ ( "XML Schema..." ) );
    associateMenu->Append ( ID_ASSOCIATE_XSL, _ ( "XS&LT stylesheet..." ), _ ( "XSLT stylesheet..." ) );

    if ( wxFileName::DirExists ( rngDir ) )
    {
        wxString rngMask, rngFile, displayName, shortcutString;
        rngMask = rngDir + wxFileName::GetPathSeparator() + _T ( "*.rng" );
        rngFile = wxFindFirstFile ( rngMask, wxFILE );

        int id = ID_VALIDATE_PRESET1;

        if ( !rngFile.empty() )
        {
            validationPresetMap.insert ( make_pair ( id, rngFile ) );
            wxFileName::SplitPath ( rngFile, NULL, NULL, &displayName, NULL );
            displayName.Replace ( _T ( ".rng" ), _T ( "" ) );
            shortcutString.Printf ( _ ( "\tCtrl+%i" ), ( id - ID_VALIDATE_PRESET1 ) + 1 );

            validationMenu->Append ( id, displayName + shortcutString, displayName );

            for ( id = ID_VALIDATE_PRESET2; id <= ID_VALIDATE_PRESET9; ++id )
            {
                rngFile = wxFindNextFile();
                if ( rngFile.empty() )
                    break;
                validationPresetMap.insert ( make_pair ( id, rngFile ) );
                wxFileName::SplitPath ( rngFile, NULL, NULL, &displayName, NULL );
                shortcutString.Printf ( _ ( "\tCtrl+%i" ), ( id - ID_VALIDATE_PRESET1 ) + 1 );
                displayName.Replace ( _T ( ".rng" ), _T ( "" ) );
                validationMenu->Append ( id, displayName + shortcutString, displayName );
            }
        }
    }

    // xsl menu
    wxMenu *xslMenu = new wxMenu;
    xslMenu->Append ( ID_XSLT, _ ( "&XSL Transform...\tF8" ),
                      _ ( "XSL Transform..." ) );
    xslMenu->AppendSeparator();
    xslMenu->Append (
        ID_XSLT_DOCBOOK_HTML,
        _ ( "&DocBook to HTML\tAlt+1" ), _ ( "DocBook to HTML" ) );
    xslMenu->Append (
        ID_XSLT_DOCBOOK_XHTML,
        _ ( "&DocBook to XHTML\tAlt+2" ), _ ( "DocBook to XHTML" ) );
    xslMenu->Append (
        ID_XSLT_DOCBOOK_FO,
        _ ( "D&ocBook to XSL-FO\tAlt+3" ), _ ( "DocBook to XSL-FO" ) );
    xslMenu->Append (
        ID_XSLT_TEI_HTML,
        _ ( "&TEI to HTML\tAlt+4" ), _ ( "TEI to HTML" ) );
    xslMenu->Append (
        ID_XSLT_TEI_LATEX,
        _ ( "T&EI to LaTeX\tAlt+5" ), _ ( "TEI to LaTeX" ) );
    xslMenu->Append (
        ID_XSLT_TEI_XHTML,
        _ ( "TE&I to XHTML\tAlt+6" ), _ ( "TEI to XHTML" ) );
    xslMenu->Append (
        ID_XSLT_TEI_FO,
        _ ( "TEI to &XSL-FO\tAlt+7" ), _ ( "TEI to XSL-FO" ) );

    // xml menu
    xmlMenu = new wxMenu; // use class-wide data member
    xmlMenu->Append (
        ID_CHECK_WELLFORMED,
        _ ( "&Check Well-formedness\tF2" ), _ ( "Check Well-formedness" ) );
    xmlMenu->Append (
        wxID_ANY,
        _ ( "&Validate" ),
        validationMenu );
    xmlMenu->AppendSeparator();
    xmlMenu->Append (
        wxID_ANY,
        _ ( "&Associate" ),
        associateMenu );
    xmlMenu->AppendSeparator();
    xmlMenu->Append ( wxID_ANY, _ ( "&XSLT" ), xslMenu );
    xmlMenu->Append (
        ID_XPATH,
        _ ( "&Evaluate XPath...\tF9" ),
        _ ( "Evaluate XPath..." ) );

    xmlMenu->AppendSeparator();
    xmlMenu->Append (
        ID_PRETTYPRINT,
        _ ( "&Pretty-print\tF11" ), _ ( "Pretty-print" ) );
    xmlMenu->AppendSeparator();
    xmlMenu->AppendCheckItem (
        ID_PROTECT_TAGS,
        _ ( "&Lock Tags\tCtrl+L" ),
        _ ( "Lock Tags" ) );
    xmlMenu->Check ( ID_PROTECT_TAGS, protectTags );
    xmlMenu->AppendSeparator();
    xmlMenu->Append (
        ID_ENCODING,
        _ ( "E&ncoding..." ), _ ( "Encoding..." ) );

    // tools menu
    wxMenu *toolsMenu = new wxMenu;

    wxMenuItem *spellingItem =
        new wxMenuItem (
            NULL,
            ID_SPELL,
            _ ( "&Spelling and Style...\tF7" ),
            _ ( "Spelling and Style..." ) );
    spellingItem->SetBitmap ( spelling16Bitmap );

    wxMenuItem *wordCountItem =
        new wxMenuItem (
            NULL,
            ID_WORD_COUNT,
            _ ( "&Word Count" ),
            _ ( "Word Count" ) );
    wordCountItem->SetBitmap ( wxNullBitmap );

    wxMenuItem *commandItem =
        new wxMenuItem (
            NULL,
            ID_COMMAND,
            _ ( "&Command\tCtrl+Alt+C" ),
            _ ( "Command" ) );
    commandItem->SetBitmap ( wxNullBitmap );

    wxMenuItem *optionsItem =
        new wxMenuItem (
            NULL,
            ID_OPTIONS,
            _ ( "&Options..." ),
            _ ( "Options..." ) );
    optionsItem->SetBitmap ( wxNullBitmap );

    toolsMenu->Append ( spellingItem );
    toolsMenu->Append ( wordCountItem );
    toolsMenu->AppendSeparator();
    toolsMenu->Append ( commandItem );
    toolsMenu->AppendSeparator();
    toolsMenu->Append ( optionsItem );

    // help menu
    wxMenu *helpMenu = new wxMenu;

    wxMenuItem *helpItem =
        new wxMenuItem ( NULL, wxID_HELP,
                         _ ( "&XML Copy Editor Help\tF1" ), _ ( "Help" ) );
    helpItem->SetBitmap ( helpBitmap );
    wxMenuItem *homeItem =
        new wxMenuItem ( NULL, ID_HOME,
                         _ ( "&Home Page" ), _ ( "Home Page" ) );
    homeItem->SetBitmap ( wxNullBitmap );
    wxMenuItem *feedbackItem =
        new wxMenuItem ( NULL, ID_FEEDBACK, _ ( "&Forum" ), _ ( "Forum" ) );
    feedbackItem->SetBitmap ( wxNullBitmap );
    wxMenuItem *aboutItem =
        new wxMenuItem ( NULL, wxID_ABOUT,
                         _ ( "&About XML Copy Editor" ), _ ( "About" ) );
    aboutItem->SetBitmap ( wxNullBitmap );
    wxMenuItem *downloadSourceItem =
        new wxMenuItem ( NULL, ID_DOWNLOAD_SOURCE,
                         _ ( "&Browse Source" ), _ ( "Browse Source" ) );
    downloadSourceItem->SetBitmap ( wxNullBitmap );
    helpMenu->Append ( helpItem );
    helpMenu->AppendSeparator();
    helpMenu->Append ( homeItem );
    helpMenu->Append ( feedbackItem );
    helpMenu->Append ( downloadSourceItem );
    helpMenu->AppendSeparator();
    helpMenu->Append ( aboutItem );

    MyMenuBar *menuBar = new MyMenuBar ( wxMB_DOCKABLE );
    menuBar->Append ( fileMenu, _ ( "&File" ) );
    menuBar->Append ( editMenu, _ ( "&Edit" ) );
    menuBar->Append ( viewMenu, _ ( "&View" ) );
    menuBar->Append ( insertMenu, _ ( "&Insert" ) );
    menuBar->Append ( xmlMenu, _ ( "&XML" ) );
    menuBar->Append ( toolsMenu, _ ( "&Tools" ) );
    menuBar->Append ( helpMenu, _ ( "&Help" ) );
    return menuBar;
}

void MyFrame::updateFileMenu ( bool deleteExisting )
{
    if ( deleteExisting )
    {
        wxMenuItemList list = fileMenu->GetMenuItems();
        size_t count = list.size();
        for ( size_t i = 0; i < count; ++i )
            fileMenu->Delete ( list[i] );
    }

    wxMenuItem *newItem =
        new wxMenuItem ( NULL, wxID_NEW, _ ( "&New...\tCtrl+N" ), _ ( "New..." ) );
    newItem->SetBitmap ( new16Bitmap );
    wxMenuItem *openItem =
        new wxMenuItem ( NULL, wxID_OPEN, _ ( "&Open...\tCtrl+O" ), _ ( "Open..." ) );
    openItem->SetBitmap ( open16Bitmap );
    wxMenuItem *openLargeFileItem =
        new wxMenuItem ( NULL, ID_OPEN_LARGE_FILE,
                         _ ( "O&pen Large Document...\tCtrl+Shift+O" ), _ ( "Open Large Document..." ) );
    openLargeFileItem->SetBitmap ( wxNullBitmap );
    wxMenuItem *closeItem =
        new wxMenuItem ( NULL, wxID_CLOSE, _ ( "&Close\tCtrl+F4" ), _ ( "Close" ) );
    closeItem->SetBitmap ( wxNullBitmap );
    wxMenuItem *closeAllItem =
        new wxMenuItem ( NULL, wxID_CLOSE_ALL, _ ( "C&lose All" ), _ ( "Close All" ) );
    closeAllItem->SetBitmap ( wxNullBitmap );
    wxMenuItem *saveItem =
        new wxMenuItem ( NULL, wxID_SAVE, _ ( "&Save\tCtrl+S" ), _ ( "Save" ) );
    saveItem->SetBitmap ( save16Bitmap );

    wxMenuItem *saveAsItem =
        new wxMenuItem ( NULL, wxID_SAVEAS, _ ( "S&ave As...\tF12" ), _ ( "Save As..." ) );
    saveAsItem->SetBitmap ( wxNullBitmap );
    wxMenuItem *revertItem =
        new wxMenuItem ( NULL, wxID_REVERT, _ ( "&Revert" ), _ ( "Revert" ) );
    revertItem->SetBitmap ( wxNullBitmap );
    wxMenuItem *printSetupItem =
        new wxMenuItem ( NULL, ID_PRINT_SETUP, _ ( "Pa&ge Setup..." ), _ ( "Page Setup..." ) );
    printSetupItem->SetBitmap ( wxNullBitmap );
    wxMenuItem *printPreviewItem =
        new wxMenuItem ( NULL, ID_PRINT_PREVIEW, _ ( "P&rint Preview..." ), _ ( "Print Preview..." ) );
    printPreviewItem->SetBitmap ( printPreviewBitmap );
    wxMenuItem *printItem =
        new wxMenuItem ( NULL, ID_PRINT, _ ( "Pr&int...\tCtrl+P" ), _ ( "Print..." ) );
    printItem->SetBitmap ( print16Bitmap );
    wxMenuItem *importMSWordItem =
        new wxMenuItem (
            NULL, ID_IMPORT_MSWORD, _ ( "I&mport Microsoft Word Document..." ) );
    importMSWordItem->SetBitmap ( wxNullBitmap );
    wxMenuItem *exportMSWordItem =
        new wxMenuItem (
            NULL, ID_EXPORT_MSWORD, _ ( "&Export Microsoft Word Document..." ) );
    exportMSWordItem->SetBitmap ( wxNullBitmap );

    wxMenuItem *exitItem =
        new wxMenuItem ( NULL, wxID_EXIT, _ ( "E&xit" ), _ ( "Exit" ) );
    exitItem->SetBitmap ( wxNullBitmap );

    fileMenu->Append ( newItem );
    fileMenu->Append ( openItem );
    fileMenu->Append ( openLargeFileItem );
    fileMenu->AppendSeparator();
    fileMenu->Append ( closeItem );
    fileMenu->Append ( closeAllItem );
    fileMenu->Append ( saveItem );
    fileMenu->Append ( saveAsItem );
    fileMenu->Append ( revertItem );
    fileMenu->AppendSeparator();
    fileMenu->Append ( printSetupItem );
    fileMenu->Append ( printPreviewItem );
    fileMenu->Append ( printItem );
#ifdef __WXMSW__
    fileMenu->AppendSeparator();
    fileMenu->Append ( importMSWordItem );
    fileMenu->Append ( exportMSWordItem );
#endif
    history.AddFilesToMenu ( fileMenu );
    fileMenu->AppendSeparator();
    fileMenu->Append ( exitItem );
}

MyToolBar *MyFrame::getToolBar()
{
    MyToolBar *myToolBar = new MyToolBar (
                               this,
                               ID_TOOLBAR,
                               wxDefaultPosition,
                               wxDefaultSize,
                               wxTB_FLAT |
                               wxTB_HORIZONTAL |
                               wxTB_DOCKABLE );
    int w, h;
#ifdef __WXMSW__
    w = saveBitmap.GetWidth(), h = saveBitmap.GetHeight();
#else
    w = h = 24;
#endif
    myToolBar->SetToolBitmapSize ( wxSize ( w, h ) );

    myToolBar->AddTool (
        wxID_NEW,
        _ ( "New" ),
        newBitmap,
        _ ( "New" ) );
    myToolBar->AddTool (
        wxID_OPEN,
        _ ( "Open" ),
        openBitmap,
        _ ( "Open" ) );
    myToolBar->AddTool (
        wxID_SAVE,
        _ ( "Save" ),
        saveBitmap,
        wxNullBitmap,
        wxITEM_NORMAL,
        _ ( "Save" ) );
    myToolBar->AddTool (
        ID_PRINT,
        _ ( "Print" ),
        printBitmap,
        wxNullBitmap,
        wxITEM_NORMAL,
        _ ( "Print" ) );
    myToolBar->AddTool (
        ID_BROWSER,
        _ ( "Browser" ),
        internetBitmap,
        wxNullBitmap,
        wxITEM_NORMAL,
        _ ( "Browser" ) );
    myToolBar->AddTool (
        ID_SPELL,
        _ ( "Spelling and Style" ),
        spellingBitmap,
        wxNullBitmap,
        wxITEM_NORMAL,
        _ ( "Spelling and Style" ) );

    myToolBar->AddCheckTool (
        ID_PROTECT_TAGS,
        _ ( "Lock Tags" ),
        hyperlinkBitmap,
        wxNullBitmap,
        _ ( "Lock Tags" ) );
    myToolBar->ToggleTool (
        ID_PROTECT_TAGS, protectTags );

    myToolBar->Realize();
    return myToolBar;
}

XmlDoc *MyFrame::getActiveDocument()
{
    if ( !mainBook->GetPageCount() )
        return NULL;
    return ( XmlDoc * ) mainBook->GetPage ( mainBook->GetSelection() );
}

void MyFrame::addSafeSeparator ( wxToolBar *toolBar )
{
    if ( xpThemeActive() )
    {
        toolBar->AddSeparator();
        return;
    }
    wxStaticText *staticControl = new wxStaticText (
                                      toolBar,
                                      wxID_ANY,
                                      _T ( "  " ) );
    toolBar->AddControl ( staticControl );
}

void MyFrame::statusProgress ( const wxString& s )
{
    wxStatusBar *status = GetStatusBar();
    if ( !status )
        return;
    status->SetStatusText ( s, 0 );
}

void MyFrame::messagePane ( const wxString& s, int iconType )
{
    wxString paneTitle;
    switch ( iconType )
    {
    case ( CONST_INFO ) :
                    if ( s.Length() < 50 ) // magic no. necessary?
            {
                statusProgress ( s );
                return;
            }
        paneTitle = _ ( "Information" );
        break;
    case ( CONST_WARNING ) :
                    paneTitle = _ ( "Warning" );
        break;
    case ( CONST_STOP ) :
                    paneTitle = _ ( "Stopped" );
        break;
    case ( CONST_QUESTION ) :
                    paneTitle = _ ( "Question" );
        break;
    default:
        paneTitle = _ ( "Message" );
        break;
    }

    wxAuiPaneInfo info = manager.GetPane ( htmlReport );
    if ( !info.IsShown() )
{
        manager.GetPane ( htmlReport ).Show ( true );
        manager.Update();
    }

    manager.GetPane ( htmlReport ).Caption ( paneTitle );

    wxString htmlString = s;
    htmlString.Replace ( _T ( "&" ), _T ( "&amp;" ), true );
    htmlString.Replace ( _T ( "<" ), _T ( "&lt;" ), true );
    htmlString.Replace ( _T ( ">" ), _T ( "&gt;" ), true );

    wxString htmlBuffer;
    htmlBuffer += _T ( "<html><body><table><tr><td width=\"5%\"><img src=\"" );
    switch ( iconType )
    {
    case ( CONST_INFO ) :
                    htmlBuffer += pngDir;
        htmlBuffer += _T ( "stock_dialog-info-32.png" );
        break;
    case ( CONST_WARNING ) :
                    htmlBuffer += pngDir;
        htmlBuffer += _T ( "stock_dialog-warning-32.png" );
        break;
    case ( CONST_STOP ) :
                    htmlBuffer += pngDir;
        htmlBuffer += _T ( "stock_dialog-stop-32.png" );
        break;
    case ( CONST_QUESTION ) :
                    htmlBuffer += pngDir;
        htmlBuffer += _T ( "stock_dialog-question-32.png" );
        break;
    default:
        break;
    }
    htmlBuffer += _T ( "\"></td><td width=\"95%\">" );
    htmlBuffer += htmlString;
    htmlBuffer += _T ( "</td></tr></table></body></html>" );

    htmlReport->SetPage ( htmlBuffer );

    manager.Update();
}

void MyFrame::documentOk ( const wxString& status )
{
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;
    wxString message;
    message.Printf ( _ ( "%s is %s" ),
                     doc->getShortFileName().c_str(),
                     status.c_str() );
    messagePane ( message, CONST_INFO );
}

void MyFrame::applyEditorProperties ( bool zoomOnly )
{
    XmlDoc *doc;
    size_t documentCount = mainBook->GetPageCount();
    for ( size_t i = 0; i < documentCount; i++ )
    {
        doc = ( XmlDoc * ) mainBook->GetPage ( i );
        if ( doc )
        {
            doc->applyProperties ( properties, zoomOnly );
            if ( !properties.validateAsYouType )
                doc->clearErrorIndicators();
        }
    }
}

void MyFrame::modifiedMessage()
{
    messagePane (
        _ ( "Document has been modified: save or discard changes" ),
        CONST_STOP );
}

void MyFrame::xmliseWideTextNode ( wxString& s )
{
    s.Replace ( _T ( "&" ), _T ( "&amp;" ), true );
    s.Replace ( _T ( "<" ), _T ( "&lt;" ), true );
    s.Replace ( _T ( ">" ), _T ( "&gt;" ), true );
}

int MyFrame::getFileType ( const wxString& fileName )
{
    std::string fileNameLocal, fileNameLocalLC;
    fileNameLocal = fileName.mb_str ( wxConvLocal );
    fileNameLocalLC = CaseHandler::lowerCase ( fileNameLocal );

    if ( fileNameLocalLC.find ( ".dtd" ) != std::string::npos ||
            fileNameLocalLC.find ( ".ent" ) != std::string::npos )
        return FILE_TYPE_DTD;
    else if ( fileNameLocalLC.find ( ".css" ) != std::string::npos )
        return FILE_TYPE_CSS;
    else if ( fileNameLocalLC.find ( ".php" ) != std::string::npos )
        return FILE_TYPE_PHP;
    else if ( fileNameLocalLC.find ( ".exe" ) != std::string::npos )
        return FILE_TYPE_BINARY;
    else if ( fileNameLocalLC.find ( ".rnc" ) != std::string::npos )
        return FILE_TYPE_RNC;
    return FILE_TYPE_XML;
}

long MyFrame::getNotebookStyleMask()
{
    /*
    if (notebookStyleMenu->IsChecked(ID_NOTEBOOK_STYLE_FLAT))
      return wxFNB_FANCY_TABS | wxFNB_MOUSE_MIDDLE_CLOSES_TABS | wxFNB_X_ON_TAB;
    else if (notebookStyleMenu->IsChecked(ID_NOTEBOOK_STYLE_VC8))
      return wxFNB_BACKGROUND_GRADIENT | wxFNB_VC8 | wxFNB_MOUSE_MIDDLE_CLOSES_TABS |
        wxFNB_X_ON_TAB | wxFNB_DROPDOWN_TABS_LIST | wxFNB_NO_NAV_BUTTONS;
    else
      return wxFNB_BACKGROUND_GRADIENT | wxFNB_VC8 | wxFNB_MOUSE_MIDDLE_CLOSES_TABS | wxFNB_COLORFUL_TABS |
        wxFNB_X_ON_TAB | wxFNB_DROPDOWN_TABS_LIST | wxFNB_NO_NAV_BUTTONS;
      //wxFNB_BACKGROUND_GRADIENT | wxFNB_VC8 | wxFNB_MOUSE_MIDDLE_CLOSES_TABS | wxFNB_COLORFUL_TABS;
    */
    return 0;
}

bool MyFrame::isSpecialFileType ( const wxString& fileName )
{
    std::string fileNameLocal, fileNameLocalLC;
    fileNameLocal = fileName.mb_str ( wxConvLocal );
    fileNameLocalLC = CaseHandler::lowerCase ( fileNameLocal );

    return (
               fileNameLocalLC.find ( ".dtd" ) != std::string::npos ||
               fileNameLocalLC.find ( ".css" ) != std::string::npos ||
               fileNameLocalLC.find ( ".php" ) != std::string::npos );
}

void MyFrame::encodingMessage()
{
    wxString msg = _ ( "Encoding should be one of " );
    msg += ENCODING_INFO;
    messagePane ( msg, CONST_STOP );
}

void MyFrame::updatePaths()
{
    ruleSetDir = applicationDir + wxFileName::GetPathSeparator() + _T ( "rulesets" );
    filterDir = applicationDir + wxFileName::GetPathSeparator() + _T ( "filters" );
    templateDir = applicationDir + wxFileName::GetPathSeparator() + _T ( "templates" ) +
                  wxFileName::GetPathSeparator();
    binDir = applicationDir + wxFileName::GetPathSeparator() + _T ( "bin" ) +
             wxFileName::GetPathSeparator();
    helpDir = applicationDir + wxFileName::GetPathSeparator() + _T ( "help" ) +
              wxFileName::GetPathSeparator();
    rngDir = applicationDir + wxFileName::GetPathSeparator() + _T ( "rng" ) +
             wxFileName::GetPathSeparator();
    htmlDir = applicationDir + wxFileName::GetPathSeparator() + _T ( "html" ) +
              wxFileName::GetPathSeparator();
    pngDir = applicationDir + wxFileName::GetPathSeparator() + _T ( "png" ) +
             wxFileName::GetPathSeparator();
    wxString wideCatalogPath =
        applicationDir + wxFileName::GetPathSeparator() + _T ( "catalog" ) +
        wxFileName::GetPathSeparator() + _T ( "catalog" );
    catalogPath = wideCatalogPath.mb_str ( wxConvLocal );
    wxString wideXslDtdPath =
        applicationDir + wxFileName::GetPathSeparator() + _T ( "dtd" ) +
        wxFileName::GetPathSeparator() + _T ( "xslt10.dtd" );
    xslDtdPath = wideXslDtdPath.mb_str ( wxConvLocal );
    wxString wideRssDtdPath =
        applicationDir + wxFileName::GetPathSeparator() + _T ( "dtd" ) +
        wxFileName::GetPathSeparator() + _T ( "rss2.dtd" );
    rssDtdPath = wideRssDtdPath.mb_str ( wxConvLocal );
    wxString wideXtmDtdPath =
        applicationDir + wxFileName::GetPathSeparator() + _T ( "dtd" ) +
        wxFileName::GetPathSeparator() + _T ( "xtm1.dtd" );
    xtmDtdPath = wideXtmDtdPath.mb_str ( wxConvLocal );
    wxString wideLzxDtdPath =
        applicationDir + wxFileName::GetPathSeparator() + _T ( "dtd" ) +
        wxFileName::GetPathSeparator() + _T ( "lzx.dtd" );
    lzxDtdPath = wideLzxDtdPath.mb_str ( wxConvLocal );
    wxString wideXliffDtdPath =
        applicationDir + wxFileName::GetPathSeparator() + _T ( "dtd" ) +
        wxFileName::GetPathSeparator() + _T ( "xliff.dtd" );
    xliffDtdPath = wideXliffDtdPath.mb_str ( wxConvLocal );
}

void MyFrame::OnAssociate ( wxCommandEvent& event )
{
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;

    wxString title, label, type, extension, path, defaulturl, defaultaux;
    wxString auxiliaryLabel;
    int id = event.GetId();
    switch ( id )
    {
    case ID_ASSOCIATE_DTD_PUBLIC:
        type = _ ( "Public DTD" );
        extension = _T ( "*.dtd" );
        defaulturl = lastDtdPublic;
        defaultaux = lastDtdPublicAux;
        break;
    case ID_ASSOCIATE_DTD_SYSTEM:
        type = _ ( "System DTD" );
        extension = _T ( "*.dtd" );
        defaulturl = lastDtdSystem;
        defaultaux = _T ( "" );
        break;
    case ID_ASSOCIATE_W3C_SCHEMA:
        type = _ ( "XML Schema" );
        extension = _T ( "*.xsd" );
        defaulturl = lastSchema;
        defaultaux = _T ( "" );
        break;
    case ID_ASSOCIATE_XSL:
        type = _ ( "XSLT stylesheet" );
        extension = _T ( "*.xsl;*.xslt" );
        defaulturl = lastXslStylesheet;
        defaultaux = _T ( "" );
        break;
    default:
        return;
    }

    std::string utf8Buffer;
    getRawText ( doc, utf8Buffer );
    std::auto_ptr<WrapExpat> wellformedparser ( new WrapExpat() );
    if ( !wellformedparser->parse ( utf8Buffer ) )
    {
        std::string error = wellformedparser->getLastError();
        wxString wideError = wxString ( error.c_str(), wxConvUTF8, error.size() );
        wxString message;
        message.Printf (
            _ ( "Cannot associate %s: %s" ),
            type.c_str(),
            wideError.c_str() );
        messagePane ( message,
                      CONST_STOP );
        return;
    }

    title.Printf ( _ ( "Associate %s" ), type.c_str() );
    label = _ ( "Choose a file:" );

    bool auxiliaryBox =
        ( id == ID_ASSOCIATE_DTD_PUBLIC );
    //(id == ID_ASSOCIATE_W3C_SCHEMA_NS || id == ID_ASSOCIATE_DTD_PUBLIC);
    if ( auxiliaryBox )
    {
        auxiliaryLabel = _ ( "Choose a public identifier:" );
        /*
              (id == ID_ASSOCIATE_DTD_PUBLIC) ? _("Choose a public identifier:") :
                _("Choose a namespace:");
        */
    }

    AssociateDialog ad (
        this,
        title,
        label,
        type,
        extension,
        defaulturl,
        auxiliaryBox,
        auxiliaryLabel,
        defaultaux );
    if ( ad.ShowModal() != wxID_OK )
        return;

    path = ad.getUrl();

    wxString aux, schemaPathMemory;

    if ( auxiliaryBox )
    {
        aux = ad.getAux();
    }

    std::string utf8Path = ( const char * ) path.mb_str ( wxConvUTF8 );

    std::string modifiedBuffer;

    // remember choice
    switch ( id )
    {
    case ID_ASSOCIATE_W3C_SCHEMA:
        lastSchema = path;
        break;
    case ID_ASSOCIATE_DTD_PUBLIC:
        lastDtdPublic = path;
        lastDtdPublicAux = aux;
        break;
    case ID_ASSOCIATE_DTD_SYSTEM:
        lastDtdSystem = path;
        break;
    case ID_ASSOCIATE_XSL:
        lastXslStylesheet = path;
        break;
    default:
        break;
    }

    if ( id == ID_ASSOCIATE_W3C_SCHEMA )
    {
        std::auto_ptr<XmlAssociateXsd> parser ( new XmlAssociateXsd ( utf8Path ) );
        if ( !parser->parse ( utf8Buffer ) )
            return;
        modifiedBuffer = parser->getBuffer();
    }
    else if ( id == ID_ASSOCIATE_DTD_SYSTEM || id == ID_ASSOCIATE_DTD_PUBLIC )
    {
        std::auto_ptr<XmlAssociateDtd> parser ( new XmlAssociateDtd (
                                                    utf8Path,
                                                    ( auxiliaryBox ) ? ( const char * ) aux.mb_str ( wxConvUTF8 ) : "" ) );
        if ( !parser->parse ( utf8Buffer ) )
            return;
        modifiedBuffer = parser->getBuffer();
    }
    else if ( id == ID_ASSOCIATE_XSL )
    {
        std::auto_ptr<XmlAssociateXsl> parser ( new XmlAssociateXsl (
                                                    utf8Path ) );
        if ( !parser->parse ( utf8Buffer ) )
            return;
        modifiedBuffer = parser->getBuffer();
    }
    else
        return;
    doc->SetTextRaw ( modifiedBuffer.c_str() );
    doc->SetFocus();
}

void MyFrame::openRememberedTabs()
{
    if ( openTabsOnClose.empty() )
        return;
    wchar_t *s, *it;
    s = it = ( wchar_t * ) openTabsOnClose.wc_str();
    std::vector<wxString> v;
    wxString buffer = wxEmptyString;
    for ( ; *it; it++ )
    {
        if ( *it == L'|' )
        {
            if ( !buffer.empty() )
            {
                v.push_back ( buffer );
            }
            buffer = wxEmptyString;
        }
        else
            buffer += *it;
    }
    if ( !buffer.empty() )
    {
        v.push_back ( buffer );
    }

    if ( v.empty() )
        return;

    std::vector<wxString>::iterator vit;
    for ( vit = v.begin(); vit != v.end(); vit++ )
    {
        if ( !openFile ( *vit ) )
            break;
    }
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) != NULL )
        doc->SetFocus();
}

void MyFrame::getRawText ( XmlDoc *doc, std::string& buffer )
{
    if ( !doc )
    {
        buffer = "";
        return;
    }
    //wxString wideBuffer = doc->GetText();
    //buffer = wideBuffer.mb_str(wxConvUTF8);
    buffer = doc->myGetTextRaw();
}

void MyFrame::OnWordCount ( wxCommandEvent& event )
{
    XmlDoc *doc;
    if ( ( doc = getActiveDocument() ) == NULL )
        return;
    wxString wideBuffer;
    std::string buffer;
    wideBuffer = doc->GetText();
    buffer = wideBuffer.mb_str ( wxConvUTF8 );

    auto_ptr<XmlWordCount> xwc ( new XmlWordCount() );
    wxString msg;
    if ( !xwc->parse ( buffer.c_str() ) )
    {
        std::string error = xwc->getLastError();
        wxString werror = wxString ( error.c_str(), wxConvUTF8, error.size() );
        statusProgress ( wxEmptyString );

        msg.Printf ( _ ( "Cannot count words: %s" ), werror.c_str() );
        messagePane ( msg, CONST_STOP );
        return;
    }

    int count = xwc->getWordCount();

    msg.Printf (
        ngettext ( L"%s contains %i word", L"%s contains %i words", count ),
        doc->getShortFileName().c_str(), count );

    messagePane ( msg, CONST_INFO );
    doc->SetFocus();
}

void MyFrame::removeUtf8Bom ( std::string& buffer )
{
    if ( buffer.size() > 3 &&
            ( unsigned char ) buffer[0] == 0xEF &&
            ( unsigned char ) buffer[1] == 0xBB &&
            ( unsigned char ) buffer[2] == 0xBF )
    {
        buffer.erase ( 0, 3 );
    }
}

void MyFrame::loadBitmaps()
{
#ifdef __WXMSW__
    // toolbar icons
    newBitmap = wxBITMAP ( stock_new );
    openBitmap = wxBITMAP ( stock_open );
    saveBitmap = wxBITMAP ( stock_save );
    printBitmap = wxBITMAP ( stock_print );
    spellingBitmap = wxBITMAP ( stock_spellcheck );
    internetBitmap = wxBITMAP ( stock_internet );
    hyperlinkBitmap = wxBITMAP ( stock_hyperlink );
    filtersBitmap = wxBITMAP ( stock_filters );

    // menu icons
    new16Bitmap = wxBITMAP ( stock_new_16 );
    open16Bitmap = wxBITMAP ( stock_open_16 );
    save16Bitmap = wxBITMAP ( stock_save_16 );
    printPreviewBitmap = wxBITMAP ( stock_print_preview_16 );
    print16Bitmap = wxBITMAP ( stock_print_16 );
    undo16Bitmap = wxBITMAP ( stock_undo_16 );
    redo16Bitmap = wxBITMAP ( stock_redo_16 );
    cutBitmap = wxBITMAP ( stock_cut_16 );
    copyBitmap = wxBITMAP ( stock_copy_16 );
    pasteBitmap = wxBITMAP ( stock_paste_16 );
    findBitmap = wxBITMAP ( stock_search_16 );
    spelling16Bitmap = wxBITMAP ( stock_spellcheck_16 );
    helpBitmap = wxBITMAP ( stock_help_16 );
#else
    // toolbar icons
    newBitmap.LoadFile ( pngDir + _T ( "stock_new.png" ), wxBITMAP_TYPE_PNG );
    openBitmap.LoadFile ( pngDir + _T ( "stock_open.png" ), wxBITMAP_TYPE_PNG );
    saveBitmap.LoadFile ( pngDir + _T ( "stock_save.png" ), wxBITMAP_TYPE_PNG );
    printBitmap.LoadFile ( pngDir + _T ( "stock_print.png" ), wxBITMAP_TYPE_PNG );
    spellingBitmap.LoadFile ( pngDir + _T ( "stock_spellcheck.png" ), wxBITMAP_TYPE_PNG );
    internetBitmap.LoadFile ( pngDir + _T ( "stock_internet.png" ), wxBITMAP_TYPE_PNG );
    hyperlinkBitmap.LoadFile ( pngDir + _T ( "stock_hyperlink.png" ), wxBITMAP_TYPE_PNG );
    filtersBitmap.LoadFile ( pngDir + _T ( "stock_filters.png" ), wxBITMAP_TYPE_PNG );

    // menu icons
    new16Bitmap.LoadFile ( pngDir + _T ( "stock_new-16.png" ), wxBITMAP_TYPE_PNG );
    open16Bitmap.LoadFile ( pngDir + _T ( "stock_open-16.png" ), wxBITMAP_TYPE_PNG );
    save16Bitmap.LoadFile ( pngDir + _T ( "stock_save-16.png" ), wxBITMAP_TYPE_PNG );
    printPreviewBitmap.LoadFile ( pngDir + _T ( "stock_print_preview.png" ), wxBITMAP_TYPE_PNG );
    print16Bitmap.LoadFile ( pngDir + _T ( "stock_print-16.png" ), wxBITMAP_TYPE_PNG );
    undo16Bitmap.LoadFile ( pngDir + _T ( "stock_undo-16.png" ), wxBITMAP_TYPE_PNG );
    redo16Bitmap.LoadFile ( pngDir + _T ( "stock_redo-16.png" ), wxBITMAP_TYPE_PNG );
    cutBitmap.LoadFile ( pngDir + _T ( "stock_cut-16.png" ), wxBITMAP_TYPE_PNG );
    copyBitmap.LoadFile ( pngDir + _T ( "stock_copy-16.png" ), wxBITMAP_TYPE_PNG );
    pasteBitmap.LoadFile ( pngDir + _T ( "stock_paste-16.png" ), wxBITMAP_TYPE_PNG );
    findBitmap.LoadFile ( pngDir + _T ( "stock_find-16.png" ), wxBITMAP_TYPE_PNG );
    spelling16Bitmap.LoadFile ( pngDir + _T ( "stock_spellcheck-16.png" ), wxBITMAP_TYPE_PNG );
    helpBitmap.LoadFile ( pngDir + _T ( "stock_help-16.png" ), wxBITMAP_TYPE_PNG );
#endif
}

#ifdef __WXMSW__
void MyFrame::OnDropFiles ( wxDropFilesEvent& event )
{
    int no = event.GetNumberOfFiles();
    wxString *iterator = event.GetFiles();

    if ( !no || !iterator )
        return;

    for ( int i = 0; i < no; i++, iterator++ )
    {
        if ( !openFile ( *iterator ) )
            break;
    }
}
#endif

std::string MyFrame::getAuxPath ( const std::string& fileName )
{
    if ( fileName.find ( ".xsl" ) != std::string::npos ||
            fileName.find ( ".XSL" ) != std::string::npos )
        return xslDtdPath;
    else if ( fileName.find ( ".rss" ) != std::string::npos ||
              fileName.find ( ".RSS" ) != std::string::npos )
        return rssDtdPath;
    else if ( fileName.find ( ".xtm" ) != std::string::npos ||
              fileName.find ( ".xtmm" ) != std::string::npos ||
              fileName.find ( ".XTM" ) != std::string::npos ||
              fileName.find ( ".XTMM" ) != std::string::npos )
        return xtmDtdPath;
    else if ( fileName.find ( ".lzx" ) != std::string::npos ||
              fileName.find ( ".LZX" ) != std::string::npos )
        return lzxDtdPath;
    else if ( fileName.find ( ".xlf" ) != std::string::npos ||
              fileName.find ( ".XLF" ) != std::string::npos )
        return xliffDtdPath;
    return "";
}

void MyFrame::OnActivateApp ( wxActivateEvent& event )
{
    event.Skip();
    if ( !mainBook || !event.GetActive() )
        return;
    restoreFocusToNotebook = true;
}

void MyFrame::OnIconize ( wxIconizeEvent& event )
{
    event.Skip();
    if ( event.Iconized() )
        return;
    restoreFocusToNotebook = true;
}

void MyFrame::OnKeyPressed ( wxKeyEvent& event )
{
    event.Skip();
}

void MyFrame::setStrictScrolling ( bool b )
{
    XmlDoc *doc;
    doc = getActiveDocument();
    if ( !doc )
        return;
    doc->SetYCaretPolicy ( ( b ) ? ( wxSTC_CARET_STRICT | wxSTC_CARET_SLOP ) : wxSTC_CARET_EVEN,
                           ( b ) ? 10 : 0 );
}

