#include "insertpanel.h"

BEGIN_EVENT_TABLE(InsertPanel, wxPanel)
  EVT_TEXT_ENTER(wxID_ANY, InsertPanel::OnEnter)
  EVT_LISTBOX_DCLICK(wxID_ANY, InsertPanel::OnDoubleClick) 
  EVT_LISTBOX(wxID_ANY, InsertPanel::OnListSelection)
  EVT_SIZE(InsertPanel::OnSize)
END_EVENT_TABLE()

InsertPanel::InsertPanel(
  wxWindow *parentWindowParameter,
  int id,
  int typeParameter) :
  wxPanel(parentWindowParameter, id), type(typeParameter), edit(0), list(0)
{
  parentWindow = (MyFrame *)parentWindowParameter;
  doc = lastDoc = NULL;
  int width = 150;
  SetSize(wxSize(width, -1));

  sizer = new wxBoxSizer(wxVERTICAL);
  SetSizer(sizer);
  
  edit = new wxTextCtrl(
    this,
    wxID_ANY,
    wxEmptyString,
    wxDefaultPosition,
    wxDefaultSize,
    wxTE_PROCESS_ENTER);

	wxFont normalFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	wxFont boldFont = normalFont;
	boldFont.SetWeight(wxFONTWEIGHT_BOLD);
  edit->SetFont(boldFont);

  list = new wxListBox(
    this,
    wxID_ANY,
    wxDefaultPosition,
    wxDefaultSize,
    0,
    NULL,
    wxLB_SORT | wxLB_HSCROLL);
  
  sizer->Add(edit, 0, wxGROW | wxTOP, 0);
  sizer->Add(list, 0, wxGROW | wxTOP, 0);
  sizer->Layout();
  list->Show(false);
}

void InsertPanel::update(
  XmlDoc *docParameter,
  const wxString& parentParameter,
  const wxString& grandparentParameter)
{
  doc = docParameter;
  parent = parentParameter;
  grandparent = grandparentParameter;
  
  if (!doc)
  {
    if (lastDoc)
    {
      lastDoc = NULL;
      edit->SetValue(wxEmptyString);
      list->Clear();
      list->Show(false);
    }
    return;
  }
  
  bool refreshEntities = false;
  if (doc != lastDoc)
  {
    refreshEntities = true;
    lastDoc = doc;
  }
  
  if (type == INSERT_PANEL_TYPE_ENTITY && refreshEntities)
  {
    list->Clear();
    lastDoc = doc;
    std::set<std::string> entitySet = doc->getEntitySet();
    entitySet.insert("apos");
    entitySet.insert("gt");
    entitySet.insert("lt");
    entitySet.insert("quot");
    std::set<std::string>::iterator it;
    for (it = entitySet.begin(); it != entitySet.end(); it++)
      list->Append(wxString(it->c_str(), wxConvUTF8, it->size()));
    list->Show(true);

    wxSize clientSize = GetClientSize();
    wxSize editSize = edit->GetSize();
    wxSize listSize = 
      wxSize(clientSize.GetWidth(), clientSize.GetHeight() - editSize.GetHeight());
    list->SetSize(listSize);
    list->Update();

    //sizer->Layout();
    return; 
  }

  if (parent == lastParent)
    return;
  lastParent = parent;


  if (type == INSERT_PANEL_TYPE_CHILD) // ignore for entity/sibling
  {
    doc->toggleLineBackground();
  }

  edit->SetValue(wxEmptyString);
  list->Clear();
  if (parent.empty() || ((type == INSERT_PANEL_TYPE_SIBLING) && grandparent.empty()))
  {
    list->Show(false);
    return;
  }

  std::set<wxString> elementSet;
  elementSet = doc->getChildren((type == INSERT_PANEL_TYPE_SIBLING) ? grandparent : parent);
  if (elementSet.empty())
  {
    list->Show(false);
    return;
  }
  std::set<wxString>::iterator it;
  for (it = elementSet.begin(); it != elementSet.end(); it++)
    list->Append(*it);
  list->Show(true);

  wxSize clientSize = GetClientSize();
  wxSize editSize = edit->GetSize();
  wxSize listSize = 
    wxSize(clientSize.GetWidth(), clientSize.GetHeight() - editSize.GetHeight());

  if (clientSize.IsFullySpecified() && editSize.IsFullySpecified())
    list->SetSize(listSize);
  list->Update();
}

void InsertPanel::OnEnter(wxCommandEvent& event)
{
  if (!doc)
    return;
    
  wxString choice = edit->GetValue();
  if (choice.empty())
    doc->SetFocus();
  else
    handleChoice(choice);
}

void InsertPanel::OnDoubleClick(wxCommandEvent& event)
{
  if (!doc)
    return;
  
  if (!doc)
    return;
    
  wxString choice = list->GetStringSelection();
  handleChoice(choice);
}

void InsertPanel::handleChoice(const wxString& choice)
{
  if (!doc || choice.empty())
    return;
    
  if (parentWindow)
    parentWindow->closePane();

  switch (type)
  {
    case INSERT_PANEL_TYPE_SIBLING:
      if (!parent.empty())
      {
        if (!doc->insertSibling(choice, parent) && parentWindow)
        {
          wxString msg;
          msg.Printf(_T("Cannot insert sibling '%s'"), choice.c_str());
          parentWindow->messagePane(msg, CONST_STOP);
        }
      }
      break;
    case INSERT_PANEL_TYPE_CHILD:
      if (!doc->insertChild(choice) && parentWindow)
      {
        wxString msg;
        msg.Printf(_T("Cannot insert child '%s'"), choice.c_str());
        parentWindow->messagePane(msg, CONST_STOP);
      }
      break;
    case INSERT_PANEL_TYPE_ENTITY:
      if (!doc->insertEntity(choice) && parentWindow)
      {
        wxString msg;
        msg.Printf(_T("Cannot insert entity '%s'"), choice.c_str());
        parentWindow->messagePane(msg, CONST_STOP); 
      }
      break;
    default:
      break;
  }
  doc->setValidationRequired(true);
  doc->SetFocus();
}

void InsertPanel::setEditFocus()
{
  if (!edit)
    return;
  edit->SetFocus();
}

void InsertPanel::OnListSelection(wxCommandEvent& event)
{
  edit->SetValue(list->GetStringSelection());
}

void InsertPanel::OnSize(wxSizeEvent& e)
{
  adjustSize();
  e.Skip();
}

void InsertPanel::adjustSize()
{
  if (!list || !edit)
    return;

  wxSize clientSize = GetClientSize();
  wxSize editSize = edit->GetSize();

  if (!clientSize.IsFullySpecified() || !editSize.IsFullySpecified())
    return;
  
  wxSize listSize = 
    wxSize(clientSize.GetWidth(), clientSize.GetHeight() - editSize.GetHeight());
  if (listSize.IsFullySpecified())
    list->SetSizeHints(listSize, listSize, listSize);
}