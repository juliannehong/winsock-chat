#include "PanelContainer.h"



LRESULT CPanelContainer::HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	return LRESULT();
}

bool CPanelContainer::InitializeWindowClass(LPWNDCLASSEX wcx)
{
	return false;
}

bool CPanelContainer::InitializeWindowCreateStruct(LPCREATESTRUCT cs)
{
	return false;
}

CPanelContainer::CPanelContainer()
{
}

CPanelContainer::~CPanelContainer()
{
}

U32 CPanelContainer::GetNumPanels() const
{
	return (U32)panels.size();
}

bool CPanelContainer::IsIndexValid(U32 Index) const
{
	return (Index < panels.size() && Index >= 0);
}

CObjectPtr<CWindow> CPanelContainer::GetPanel(U32 Index) const
{
	if(IsIndexValid(Index))
	{
		return panels[Index];
	}
	return nullptr;
}
