#pragma once
#define FieldSize_X		60	
#define FieldSize_Y		40
#define CellSize		12
#define SPEED 10


#define ButtonsSize_X 120
#define ButtonsSize_Y 30

//Commands menu
#define OnMenuClickedExit			1
#define OnMenuClickedOpenFile		11
#define OnMenuClickedSaveFile		12
#define OnMenuClickedChangeSpeed	21
#define OnMenuClickedSaveHistory	22
#define OnMenuClickedAboutAuthor	31
#define OnMenuClickedAboutProgram	32


//Commands buttons
#define OnButtonClickedRun			201
#define OnButtonClickedOneStep		202
#define OnButtonClickedStop			203
#define OnButtonClickedClear		204



HWND hMainWND;
HWND hFieldWND;
bool field[FieldSize_Y][FieldSize_X];
bool prevfield[FieldSize_Y][FieldSize_X];
PAINTSTRUCT ps;

HBRUSH blackBrush = CreateSolidBrush(RGB(20, 150, 255));
HBRUSH whiteBtush = CreateSolidBrush(RGB(255, 255, 255));
HPEN greyPan = CreatePen(PS_SOLID, 1, RGB(100, 160, 255));


int X = 0, Y = 0;
bool isStoped = false;
bool opt = true;
RECT cell = { 0, 0, FieldSize_X * CellSize, FieldSize_Y * CellSize };