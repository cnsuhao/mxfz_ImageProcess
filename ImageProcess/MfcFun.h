#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
ANSI 和 UNICODE 的函数对应表 

ANSI        UNICODE     通用
(char.h)    (wchar.h)        (tchar.h)
char         wchar_t          TCHAR
char *      wchar_t *        PTCHAR (PTSTR,LPWSTR,PWSTR,WCHAR)
printf       wprintf          _tprintf
scanf       wscanf          _tscanf
atoi         _wtoi            _ttoi
atol         _wtol            _ttol
itoa         _itow            _itot
ltoa         _ltow            _ltot
atof         _wtof            _tstof
strlen       wcslen          _tcslen
strcat       wcscat          _tcscat
strcpy      wcscpy         _tcscpy
strcmp		wcscmp        _tcscmp
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void set_DropDownSize(CComboBox& box, UINT LinesToDisplay) 
/*-------------------------------------------------------------------------- 
* Purpose: Set the proper number of lines in a drop-down vector or 
* combo box. 
* Description: Resizes the combo box window to fit the proper number 
* of lines. The window must exist before calling this function. 
* This function should be called when the combo box is created, and when 
* the font of the combo box changes. (e.g. WM_SETTINGCHANGE) 
* TestPluging needed: 
* Are there cases where SM_CYBORDER should be used instead of SM_CYEDGE? 
* owner-draw variable height combo box 
* Subclassed combo box with horizontal scroll-bar 
* Returns: nothing 
* Author: KTM 
*--------------------------------------------------------------------------*/ 
{ 
	ASSERT(IsWindow(box)); // Window must exist or SetWindowPos won't work 

	CRect cbSize; // current size of combo box 
	int Height; // new height for drop-down portion of combo box 

	box.GetClientRect(cbSize); 
	Height = box.GetItemHeight(-1); // start with size of the edit-box portion 
	Height += box.GetItemHeight(0) * LinesToDisplay; // add height of lines of text 

	// Note: The use of SM_CYEDGE assumes that we're using Windows '95 
	// Now add on the height of the border of the edit box 
	Height += GetSystemMetrics(SM_CYEDGE) * 2; // top & bottom edges 

	// The height of the border of the drop-down box 
	Height += GetSystemMetrics(SM_CYEDGE) * 2; // top & bottom edges 

	// now set the size of the window 
	box.SetWindowPos(NULL, // not relative to any other windows 
		0, 0, // TopLeft corner doesn't change 
		cbSize.right, Height, // existing width, new height 
		SWP_NOMOVE | SWP_NOZORDER // don't move box or change z-ordering. 
		); 
};

static int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM , LPARAM lpData)   
{
	if(uMsg == BFFM_INITIALIZED) {
		CTreeCtrl	treePath;
		HTREEITEM	hItemSel;
		::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, lpData);
		treePath.SubclassWindow(::GetDlgItem(hWnd, 0x3741));
		hItemSel	= treePath.GetSelectedItem();
		treePath.Expand(hItemSel, TVE_COLLAPSE);
		treePath.UnsubclassWindow();
	} 
	return 0;  
}