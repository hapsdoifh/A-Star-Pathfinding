
// A-Star-GUIDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "A-Star-GUI.h"
#include "A-Star-GUIDlg.h"
#include "afxdialogex.h"
#include "aStarSearch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CAStarGUIDlg dialog



CAStarGUIDlg::CAStarGUIDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ASTARGUI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAStarGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAStarGUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CAStarGUIDlg::OnBnClickedOk)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON1, &CAStarGUIDlg::OnBnClickedSwitch)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CAStarGUIDlg message handlers

BOOL CAStarGUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAStarGUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework



POINT mousePos;
POINT UpdateRectSize;
int state = 0,firstTime = 1;
int stateBEnter = 0;
void CAStarGUIDlg::OnPaint()
{
	CPaintDC MyPaint(this);
	CPen myPen(PS_SOLID, 1, RGB(50, 50, 50));
	MyPaint.SelectObject(myPen);
	

	
	static grid board[BOARDSIZE][BOARDSIZE];
	pos BoardSize;
	BoardSize.x = BOARDSIZE;
	BoardSize.y = BOARDSIZE;
	static pos StartPos, DestPos, OriginPos;
	static pos CurPathPos;
	static pos MouseBlockPos;
	RECT ClientSize;
	GetClientRect(&ClientSize);

	UpdateRectSize.x = ClientSize.right / BoardSize.x*2;
	UpdateRectSize.y = ClientSize.bottom / BoardSize.y*2;

	if (firstTime == 1) {
		memset(&StartPos, 0, sizeof(StartPos));
		memset(&DestPos, 0, sizeof(DestPos));
		memset(&OriginPos, 0, sizeof(OriginPos));
		memset(board, 0, sizeof(board));
		firstTime = 0;
	}
	MouseBlockPos = CoordBlock(mousePos, ClientSize, BoardSize);
	if (state == 0) { //means to place walls
		board[MouseBlockPos.y][MouseBlockPos.x].NaviState = WALL;
	}
	else if (state == 1) {
		StartPos.x = MouseBlockPos.x;
		StartPos.y = MouseBlockPos.y;
		board[StartPos.y][StartPos.x].NaviState = START;
		OriginPos.x = StartPos.x;
		OriginPos.y = StartPos.y;
	}
	else if (state == 2 && stateBEnter == 0) {
		DestPos.x = MouseBlockPos.x;
		DestPos.y = MouseBlockPos.y;
		board[DestPos.y][DestPos.x].NaviState = END;
		stateBEnter = 1;
	}
	else {
		CurPathPos.x = 0;
		CurPathPos.y = 0;
		while (StartPos.x != DestPos.x || StartPos.y != DestPos.y) {
			if (StartPos.y == 7) {
				StartPos.y = 7;
			}
			MarkAvail(board, StartPos, DestPos, CurPathPos);
			CurPathPos.x = StartPos.x;
			CurPathPos.y = StartPos.y;
			board[StartPos.y][StartPos.x].NaviState = MARKED;
			StartPos.y = *(findFmin(board));
			StartPos.x = *(findFmin(board) + 1);
			board[StartPos.y][StartPos.x].NaviState = CURSEL;
			RECT PixelRect;
			POINT BlockPlace;
			CBrush mybrush;
			CPen BoardPen;
			for (int x = 0; x < BOARDSIZE; x++) {
				for (int y = 0; y < BOARDSIZE; y++) {
					BlockPlace.x = x;
					BlockPlace.y = y;
					BlockCoord(ClientSize, BoardSize.x, BoardSize.y, BlockPlace, &PixelRect);
					//BlockPlace.x++;
					//BlockPlace.y++;
					//BlockCoord(ClientSize, BoardSize.x, BoardSize.y, BlockPlace, &PixelCoordEnd);
					COLORREF BlockColor = BoardColor(board[y][x]);
					mybrush.CreateSolidBrush(BlockColor);
					BoardPen.CreatePen(PS_SOLID, 1, RGB(50, 50, 50));
					MyPaint.SelectObject(&mybrush);
					MyPaint.SelectObject(&BoardPen);
					Rectangle(MyPaint, PixelRect.left, PixelRect.top, PixelRect.right, PixelRect.bottom);
					mybrush.DeleteObject();
					BoardPen.DeleteObject();
					CString Str;
					RECT textRect;
					textRect.top = PixelRect.top;
					textRect.bottom = PixelRect.bottom;
					textRect.left = PixelRect.left;
					textRect.right = PixelRect.right;
					if (board[y][x].NaviState != EMPTY && board[y][x].NaviState != WALL) {
						RECT PRrect;
						CPen myPen;
						myPen.CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
						MyPaint.SelectObject(&myPen);
						POINT CurCenter, PrCenter;
						BlockPlace.x = board[y][x].parentX;
						BlockPlace.y = board[y][x].parentY;
						BlockCoord(ClientSize, BoardSize.x, BoardSize.y, BlockPlace, &PRrect);
						CurCenter.x = (PixelRect.right + PixelRect.left) / 2;
						CurCenter.y = (PixelRect.top + PixelRect.bottom) / 2;
						MoveToEx(MyPaint, CurCenter.x, CurCenter.y, 0);
						PrCenter.x = (PRrect.left + PRrect.right) / 2;
						PrCenter.y = (PRrect.top + PRrect.bottom) / 2;
						MyPaint.LineTo((CurCenter.x+PrCenter.x)/2, (CurCenter.y + PrCenter.y) / 2);
						Str.Format(_T("Dst:%3f"), board[y][x].Hval+ board[y][x].Gval);
						textRect.top = PixelRect.top + 20;
						//DrawText(MyPaint, Str, 7, &textRect, DT_BOTTOM);
						myPen.DeleteObject();
					}
				}
			}
			Sleep(100);
		}
		int tpx, tpy;
		while (StartPos.x != OriginPos.x || StartPos.y != OriginPos.y) {
			board[StartPos.y][StartPos.x].NaviState = SUCCESS;
			tpx = StartPos.x;
			tpy = StartPos.y;
			StartPos.x = board[tpy][tpx].parentX;
			StartPos.y = board[tpy][tpx].parentY; 
			
		}
	}

	RECT PixelRect;
	POINT BlockPlace;
	CBrush mybrush;
	CPen BoardPen;
	for (int x = 0; x < BOARDSIZE; x++) {
		for (int y = 0; y < BOARDSIZE; y++) {
			BlockPlace.x = x;
			BlockPlace.y = y;
			BlockCoord(ClientSize, BoardSize.x, BoardSize.y, BlockPlace, &PixelRect);
			//BlockPlace.x++;
			//BlockPlace.y++;
			//BlockCoord(ClientSize, BoardSize.x, BoardSize.y, BlockPlace, &PixelCoordEnd);
			COLORREF BlockColor = BoardColor(board[y][x]);
			mybrush.CreateSolidBrush(BlockColor);
			BoardPen.CreatePen(PS_SOLID, 1, RGB(50, 50, 50));
			MyPaint.SelectObject(&mybrush);
			MyPaint.SelectObject(&BoardPen);
			Rectangle(MyPaint, PixelRect.left, PixelRect.top, PixelRect.right, PixelRect.bottom);
			mybrush.DeleteObject();
			BoardPen.DeleteObject();
			CString Str;
			RECT textRect;
			textRect.top = PixelRect.top;
			textRect.bottom = PixelRect.bottom;
			textRect.left = PixelRect.left;
			textRect.right = PixelRect.right;
			if (board[y][x].NaviState != EMPTY && board[y][x].NaviState != WALL) {
				RECT PRrect;
				CPen myPen;
				myPen.CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
				MyPaint.SelectObject(&myPen);
				POINT CurCenter, PrCenter;
				BlockPlace.x = board[y][x].parentX;
				BlockPlace.y = board[y][x].parentY;
				BlockCoord(ClientSize, BoardSize.x, BoardSize.y, BlockPlace, &PRrect);
				CurCenter.x = (PixelRect.right + PixelRect.left) / 2;
				CurCenter.y = (PixelRect.top + PixelRect.bottom) / 2;
				MoveToEx(MyPaint, CurCenter.x, CurCenter.y, 0);
				PrCenter.x = (PRrect.left + PRrect.right) / 2;
				PrCenter.y = (PRrect.top + PRrect.bottom) / 2;
				MyPaint.LineTo((CurCenter.x + PrCenter.x) / 2, (CurCenter.y + PrCenter.y) / 2);
				Str.Format(_T("Dst:%3f"), board[y][x].Hval + board[y][x].Gval);
				textRect.top = PixelRect.top + 20;
				//DrawText(MyPaint, Str, 7, &textRect, DT_BOTTOM);
				myPen.DeleteObject();
			}
		}
	}
	Sleep(100);

	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAStarGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CAStarGUIDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}

int ldown = 0;

void CAStarGUIDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	RECT UpdateRect;
	GetCursorPos(&mousePos);
	ScreenToClient(&mousePos);
	ldown = 1;
	UpdateRect.top = mousePos.y - UpdateRectSize.y / 2;
	UpdateRect.left = mousePos.x - UpdateRectSize.x / 2;
	UpdateRect.bottom = mousePos.y + UpdateRectSize.y / 2;
	UpdateRect.right = mousePos.x + UpdateRectSize.x / 2;
	this->Invalidate();
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CAStarGUIDlg::OnBnClickedSwitch()
{
	if (state < 3) {
		state++;
	}
	// TODO: Add your control notification handler code here
}


void CAStarGUIDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (ldown == 1 && state ==0) {
		GetCursorPos(&mousePos);
		ScreenToClient(&mousePos);
		RECT UpdateRect;
		UpdateRect.top = mousePos.y - UpdateRectSize.y / 2;
		UpdateRect.left = mousePos.x - UpdateRectSize.x / 2;
		UpdateRect.bottom = mousePos.y + UpdateRectSize.y / 2;
		UpdateRect.right = mousePos.x + UpdateRectSize.x / 2;
		this->InvalidateRect(&UpdateRect);
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CAStarGUIDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	ldown = 0;
	CDialogEx::OnLButtonUp(nFlags, point);
}
