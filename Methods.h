WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON hIcon, LPCWSTR Name, WNDPROC Procedure);

void printLastErr();

void MainWNDAddMenus(HWND hWnd);
void MainWndAddWidgets(HWND hWnd);
void DrawField(HWND hWnd);


//gameMethods
void NextGen(bool* current, bool* prev, HWND);
int countLiveNeighbours(bool* world, int x, int y);
void readCellNeighbours(int* nb, int x, int y);
void copyField(bool* current, bool* prev);
bool isChangeField(bool* current, bool* prev);

HWND InitField(HWND parentWnd, HINSTANCE hInst);




LRESULT CALLBACK FieldProc(
	_In_ HWND hWnd,
	_In_ UINT message,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
);
LRESULT CALLBACK MainWindowProc(
	_In_ HWND hWnd,
	_In_ UINT message,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
);



