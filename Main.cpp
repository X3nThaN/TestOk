#include"lib.h"
#include"Resources.h"
#include"Methods.h"

int CALLBACK wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow) {

    WNDCLASS MainClass = { sizeof(WNDCLASS) };
    MainClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInstance, LoadIcon(NULL, IDI_APPLICATION), L"MainWndClass", MainWindowProc);

    if (RegisterClass(&MainClass) == 0) {
        printLastErr();
        return EXIT_FAILURE;
    }

    hMainWND = CreateWindow(
        L"MainWndClass",
        L"Life Game",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
        0,
        0,
        ButtonsSize_X + 50 + FieldSize_X * CellSize + 10,
        max((ButtonsSize_Y + 10) * 4, FieldSize_Y * CellSize) + 120,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if ( 
    hMainWND == INVALID_HANDLE_VALUE)
        return EXIT_FAILURE;


    hFieldWND = InitField(hMainWND, hInstance);

    ShowWindow(hMainWND, nCmdShow);
    UpdateWindow(hMainWND);

    MSG message = { 0 };

    while (GetMessage(&message, NULL, NULL, NULL))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return static_cast<int>(message.wParam);
}



LRESULT CALLBACK MainWindowProc(
    _In_ HWND hWnd,
    _In_ UINT message,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam
) 
{
    switch (message)
    {
    case WM_CREATE:
        MainWNDAddMenus(hWnd);
        MainWndAddWidgets(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_COMMAND:

        switch (wParam)
        {
        case OnButtonClickedOneStep:
            SendMessage(hFieldWND, message, OnButtonClickedOneStep, NULL);
            break;
        case OnButtonClickedRun:
            SendMessage(hFieldWND, message, OnButtonClickedRun, NULL);
            break;
        default:
            break;
        }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return NULL;

    
}



WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON hIcon, LPCWSTR Name, WNDPROC Procedure) {
    WNDCLASS  wc = { 0 };

    wc.hbrBackground = BGColor;
    wc.hCursor = Cursor;
    wc.hInstance = hInst;
    wc.hIcon = hIcon;
    wc.lpszClassName = Name;
    wc.lpfnWndProc = Procedure;
    wc.style = CS_HREDRAW | CS_VREDRAW;

    return wc;
}


void printLastErr(){
    wchar_t buff[100];
    _itow_s(static_cast<int>(GetLastError()), buff, 10);
    buff[99] = '/0';
    MessageBox(NULL, buff, L"Error", MB_ICONERROR);
}

//need rework
void MainWNDAddMenus(HWND hWnd) {
    HMENU RootMenu = CreateMenu();

    HMENU SubMenu = CreateMenu();

    //Creating File Submenu points
    AppendMenu(SubMenu, MF_STRING, OnMenuClickedOpenFile, L"Open...");
    AppendMenu(SubMenu, MF_STRING, OnMenuClickedSaveFile, L"Save as...");
    AppendMenu(SubMenu, MF_SEPARATOR, NULL, NULL);
    AppendMenu(SubMenu, MF_STRING, OnMenuClickedExit, L"Exit");

    //Creating File Submenu
    AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenu, L"File");

    SubMenu = CreateMenu();

    //Creating Settings Submenu points
    AppendMenu(SubMenu, MF_STRING, OnMenuClickedChangeSpeed, L"Change speed");
    AppendMenu(SubMenu, MF_SEPARATOR, NULL, NULL);
    AppendMenu(SubMenu, MF_UNCHECKED, OnMenuClickedSaveHistory, L"Save history in file");

    //Creating Settings Submenu
    AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenu, L"Settings"); //MF_POPUP - добавить в режиме дочерней ветки

    SubMenu = CreateMenu();

    //Creating About submenu points
    AppendMenu(SubMenu, MF_STRING, OnMenuClickedAboutAuthor, L"Author...");
    AppendMenu(SubMenu, MF_SEPARATOR, NULL, NULL); //MF_SEPARATOR - разделяющая черта
    AppendMenu(SubMenu, MF_STRING, OnMenuClickedAboutProgram, L"Program...");

    //Creating About submenu
    AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenu, L"About");

    SetMenu(hWnd, RootMenu);
}

void MainWndAddWidgets(HWND hWnd) {
    CreateWindowA("static", "Life Game", WS_VISIBLE | WS_CHILD | ES_CENTER, (ButtonsSize_X + 20 + FieldSize_X * CellSize + 10)/2 -100, 10, 200, 40, hWnd, NULL, NULL, NULL);

    CreateWindow(L"button", L"Run", WS_VISIBLE | WS_CHILD | ES_CENTER | BS_PUSHBUTTON, 10, 50, ButtonsSize_X, ButtonsSize_Y, hWnd, (HMENU)OnButtonClickedRun, NULL, NULL);
    CreateWindow(L"button", L"One step", WS_VISIBLE | WS_CHILD | ES_CENTER | BS_PUSHBUTTON, 10, 50 + ButtonsSize_Y + 10 * 1, ButtonsSize_X, ButtonsSize_Y, hWnd, (HMENU)OnButtonClickedOneStep, NULL, NULL);
    CreateWindow(L"button", L"Stop", WS_VISIBLE | WS_CHILD | ES_CENTER | BS_PUSHBUTTON, 10, 50 + ButtonsSize_Y*2 + 10 * 2, ButtonsSize_X, ButtonsSize_Y, hWnd, (HMENU)OnButtonClickedStop, NULL, NULL);
    CreateWindow(L"button", L"Clean", WS_VISIBLE | WS_CHILD | ES_CENTER | BS_PUSHBUTTON, 10, 50 + ButtonsSize_Y*3 + 10 * 3, ButtonsSize_X, ButtonsSize_Y, hWnd, (HMENU)OnButtonClickedClear, NULL, NULL);

}

HWND InitField(HWND parentWnd, HINSTANCE hInst) {
    WNDCLASS FieldWindow = NewWindowClass(CreateSolidBrush(RGB(255, 20, 147)), LoadCursor(NULL, IDC_CROSS), hInst, LoadIcon(NULL, IDI_QUESTION), L"FieldWindow", FieldProc);

    if (RegisterClass(&FieldWindow) == 0)
        printLastErr();

    HWND field = CreateWindow(L"FieldWindow", L"Field", WS_CHILD | WS_VISIBLE, ButtonsSize_X + 30, 50, FieldSize_X * CellSize, FieldSize_Y * CellSize, parentWnd, NULL, NULL, NULL);
    
    
    if (field == INVALID_HANDLE_VALUE)
        printLastErr();


    return field;
}



LRESULT CALLBACK FieldProc(
    _In_ HWND hWnd,
    _In_ UINT message,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam
) {
    switch (message)
    {
    case WM_LBUTTONUP:
        POINT mousePos;
        mousePos.x = LOWORD(lParam);
        mousePos.y = HIWORD(lParam);
        X = (mousePos.x) / CellSize;
        Y = (mousePos.y) / CellSize;
        if (Y < FieldSize_Y)
            if (X < FieldSize_X) {
                field[Y][X] = !field[Y][X];
               
                cell = { X * CellSize, Y * CellSize, (X+1) * (CellSize), (Y+1) * (CellSize) };
                InvalidateRect(hWnd, &cell, FALSE);
                UpdateWindow(hWnd);
            }
        break;
    case WM_PAINT:
        BeginPaint(hWnd, &ps);
        DrawField(hWnd);
        EndPaint(hWnd, &ps);
        break;
    case WM_CREATE:
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_COMMAND:

        switch (wParam)
        {
        case OnButtonClickedOneStep:
            copyField(&field[0][0], &prevfield[0][0]);
            NextGen(&field[0][0], &prevfield[0][0], hWnd);
            break;
        case OnButtonClickedRun:
            isStoped = false;
            opt = isChangeField(&field[0][0], &prevfield[0][0]);
            while (opt && !isStoped) {
                copyField(&field[0][0], &prevfield[0][0]);
                NextGen(&field[0][0], &prevfield[0][0], hWnd);
                opt = isChangeField(&field[0][0], &prevfield[0][0]);


                //SendMessageA(FieldWindow, WM_PAINT, (WPARAM)11, NULL);


                DWORD k = GetLastError();
                Sleep(SPEED);
            }
        default:
            break;
        }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return NULL;

}

void DrawField(HWND hWnd) {

    SelectObject(ps.hdc, greyPan);

    for (int i = Y; i < (int)cell.bottom / CellSize; i++) {
        for (int j = X; j < (int)cell.right / CellSize; j++) {
            if (field[i][j]) 
                SelectObject(ps.hdc, blackBrush);
            else
                SelectObject(ps.hdc, whiteBtush);


            Rectangle(ps.hdc,
                CellSize * j,			//left
                i * CellSize,			//top
                CellSize * (j + 1),		//right
                (i + 1) * CellSize);	//bottom
        }
    }

}






void NextGen(bool* current, bool* prev, HWND hWnd) {
    int live_nb = 0;	// Кол-во живых соседей

    for (int i = 0; i < FieldSize_Y; i++) {
        for (int j = 0; j < FieldSize_X; j++) {
            bool cells = false;
            X = j; Y = i;
            cells = *((prev + X) + (Y * FieldSize_X));
            live_nb = countLiveNeighbours(prev, X, Y);

            if (!cells) {
                if (live_nb == 3) {
                    *((current + j) + (i * FieldSize_X)) = 1; 
                    cell = { X * CellSize, Y * CellSize, (X + 1) * (CellSize), (Y + 1) * (CellSize) };
                    InvalidateRect(hWnd, &cell, FALSE);
                    UpdateWindow(hWnd);
                }
            }
            else {
                if (live_nb < 2 || live_nb > 3) {
                    *((current + j) + (i * FieldSize_X)) = 0;
                    cell = { X * CellSize, Y * CellSize, (X + 1) * (CellSize), (Y + 1) * (CellSize) };
                    InvalidateRect(hWnd, &cell, FALSE);
                    UpdateWindow(hWnd);
                }
            }
        }
    }
}



int countLiveNeighbours(bool* world, int x, int y) {
    int count = 0;
    int nb[8][2];
    readCellNeighbours(&nb[0][0], x, y);

    for (int i = 0; i < 8; i++) {
        int _x, _y;
        _x = nb[i][0];
        _y = nb[i][1];
        if (_x < 0 || _y < 0 || _x >= FieldSize_X || _y >= FieldSize_Y)
            continue;
        const bool isAlive = *((world + _x) + (_y * FieldSize_X));
        if (isAlive)
            count++;

    }
    //delete world;

    return count;
}



void readCellNeighbours(int* nb, int x, int y) {
    int k = 0;

    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            if (i == x && j == y)
                continue;


            const int _x = i;
            *(nb + k) = _x;
            k++;
            const int _y = j;
            *(nb + k) = _y;
            k++;
        }
    }
}



void copyField(bool* current, bool* prev) {
    for (int i = 0; i < FieldSize_Y; i++) {
        for (int j = 0; j < FieldSize_X; j++) {
            const bool buff = *((current + j) + (i * FieldSize_X));
            *((prev + j) + (i * FieldSize_X)) = buff;
        }
    }
    return;
}


bool isChangeField(bool* current, bool* prev) {
    for (int i = 0; i < FieldSize_Y; i++) {
        for (int j = 0; j < FieldSize_X; j++) {
            if (*((current + j) + i * FieldSize_X) != *((prev + j) + i * FieldSize_X))
                return true;
        }
    }
    return false;
}