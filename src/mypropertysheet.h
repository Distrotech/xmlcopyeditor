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

#ifndef MYPROPERTYSHEET_H
#define MYPROPERTYSHEET_H

#include <wx/wx.h>
#include <wx/generic/propdlg.h>
#include <wx/filename.h>
#include "xmlctrl.h"

enum
{
	APPLICATION_DIR_BROWSE,
	BROWSER_COMMAND_BROWSE,
	FONT_BROWSE
};

enum langIndex
{
    INDEX_CATALAN = 0,
	INDEX_CHINESE_SIMPLIFIED,
	INDEX_CHINESE_TRADITIONAL,
	INDEX_DUTCH,
	INDEX_ENGLISH_US,
	INDEX_FRENCH,
	INDEX_GERMAN,
	INDEX_ITALIAN,
	INDEX_RUSSIAN,
	INDEX_SLOVAK,
	INDEX_SPANISH,
	INDEX_SWEDISH,
	INDEX_UKRAINIAN
};

class MyPropertySheet : public wxPropertySheetDialog
{
	public:
		MyPropertySheet (
		    wxWindow *parent,
		    XmlCtrlProperties& propertiesParameter,
		    wxString& applicationDirParameter,
		    wxString& browserCommandParameter,
		    bool rememberOpenTabsParameter,
		    bool libxmlNetAccess,
		    bool singleInstanceCheck,
		    bool saveBom,
		    bool unlimitedUndo,
		    bool restoreLayout,
		    bool expandInternalEntities,
		    bool showFullPathOnFrame,
		    int lang,
		    wxWindowID id = wxID_ANY,
		    wxString title = _T ( "" ),
		    const wxPoint& position = wxDefaultPosition,
		    const wxSize& size = wxDefaultSize,
		    long style = wxDEFAULT_DIALOG_STYLE );
		~MyPropertySheet();
		void OnOk ( wxCommandEvent& e );
		void OnBrowserCommandBrowse ( wxCommandEvent& e );
		void OnApplicationDirBrowse ( wxCommandEvent& e );
		XmlCtrlProperties getProperties();
		wxString getApplicationDir();
		wxString getBrowserCommand();
		bool getSingleInstanceCheck();
		bool getRememberOpenTabs();
		bool getLibxmlNetAccess();
		bool getSaveBom();
		bool getUnlimitedUndo();
		bool getRestoreLayout();
		bool getExpandInternalEntities();
		bool getShowFullPathOnFrame();
		int getLang();
	private:
		int lang;
		wxCheckBox *completionBox,
		*currentLineBox,
		*foldBox,
		*numberBox,
		*whitespaceVisibleBox,
		*indentLinesBox,
		*deleteWholeTagBox,
		*rememberOpenTabsBox,
		*libxmlNetAccessBox,
		*singleInstanceCheckBox,
		*saveBomBox,
		*protectHiddenElementsBox,
		*toggleLineBackgroundBox,
		*validateAsYouTypeBox,
		*fullPathBox,
		*highlightSyntaxBox,
		*unlimitedUndoBox,
		*restoreLayoutBox,
		*expandInternalEntitiesBox,
		*insertCloseTagBox;
		wxChoice *languageBox, *fontBox;
		wxTextCtrl *applicationDirEdit, *browserCommandEdit;
		XmlCtrlProperties properties;
		wxString applicationDir, browserCommand;
		bool singleInstanceCheck,
		rememberOpenTabs,
		libxmlNetAccess,
		saveBom,
		unlimitedUndo,
		restoreLayout,
		expandInternalEntities,
		showFullPathOnFrame,
		insertCloseTag;
		DECLARE_EVENT_TABLE()
};

#endif
