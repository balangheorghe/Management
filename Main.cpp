#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <fstream>
#include <cstdlib>
#include <cstring>
HWND windowHandle;

struct copil
{
	long long id;
	char nume[25];
	char prenume[25];
	int varsta;
	char adresa[45];
	char nume_mama[25];
	char nume_tata[25];
}c;

int locuri_ocupate[4];
int verificari[10];

//verifica_adresa, verifica_nume_prenume - pentru a verifica daca prenumele, adresa si numele tuturor sunt caractere alfanumerice specifice

int verifica_adresa(char a[45])
{
	int i = 0;
	for (i = 0;i < strlen(a);i++)
		if (!((a[i] >= 'a' && a[i] <= 'z') || (a[i] >= 'A' && a[i] <= 'Z') || (a[i] >= '0'&&a[i] <= '9') || a[i] == '.' || a[i] == ',' || a[i] == ' ' || a[i] == '-'))
			return 0;
	return 1;
}

int verifica_nume_prenume(char a[45])
{
	int i = 0;
	for (i = 0;i < strlen(a);i++)
		if (!((a[i] >= 'a' && a[i] <= 'z') || (a[i] >= 'A' && a[i] <= 'Z') || a[i] == ' ' || a[i] == '-'))
			return 0;
	return 1;
}

int verificare_varsta(int varsta)
{
	if (varsta < 0 || varsta >= 7)
		return 0;
	return 1;
}

int stabilire_grupa(int varsta)
{
	if (varsta < 2)
		return 1;
	if (varsta < 4)
		return 2;
	return 3;
}

int verificare_disponibilitate_grupa(int n)
{
	if (locuri_ocupate[n]<25)
		return 1;
	return 0;
}

void calculeaza_id(long long &id, char nume[25], char prenume[25], int varsta, char adresa[45], char nume_mama[25], char nume_tata[25])
{
	//initial -1, copilul nu poate fi inscris
	id = -1;
	//fac verificarile de baza
	if (verifica_nume_prenume(nume) == 1)
		verificari[1] = 1;
	if (verifica_nume_prenume(prenume) == 1)
		verificari[2] = 1;
	if (verifica_nume_prenume(nume_mama) == 1)
		verificari[5] = 1;
	if (verifica_nume_prenume(nume_tata) == 1)
		verificari[6] = 1;
	if (verificare_varsta(varsta) == 1)
		verificari[3] = 1;
	if (verifica_adresa(adresa) == 1)
		verificari[4] = 1;
	if (verificare_disponibilitate_grupa(stabilire_grupa(varsta)) == 1)
		verificari[7] = 1;
	int i = 1, ok = 1;
	for (i = 1;i < 8;i++)
		if (verificari[i] == 0)
		{
			ok = 0;
			break;
		}
	//daca totul e ok, pot inscrie copilul, deci creez id
	if (ok == 1)
	{
		verificari[0] = 1;
		id = 0;
		id = id * 100 + nume[0]-'A';
		id = id * 100 + nume[1]-'A';
		id = id * 100 + prenume[0]-'A';
		id = id * 100 + prenume[1]-'A';
		id = id * 10 + varsta;
		id = id * 100 + nume_mama[0] - 'A';
		id = id * 100 + nume_mama[1] - 'A';
		id = id * 100 + nume_tata[0] - 'A';
		id = id * 100 + nume_tata[1] - 'A';
		id = id * 10 + stabilire_grupa(varsta);
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:

		break;
	case WM_COMMAND:
		
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hDC;
		hDC = BeginPaint(hwnd, &ps);
		TextOut(hDC, 10, 10, L"Nume: ", strlen("Nume: "));
		TextOut(hDC, 10, 30, L"Prenume: ", strlen("Prenume: "));
		TextOut(hDC, 10, 50, L"Varsta: ", strlen("Varsta: "));
		TextOut(hDC, 10, 70, L"Adresa: ", strlen("Adresa: "));
		TextOut(hDC, 10, 90, L"Numele mamei: ", strlen("Numele mamei: "));
		TextOut(hDC, 10, 110, L"Numele tatalui: ", strlen("Numele tatalui: "));
		EndPaint(hwnd, &ps);
		return 0;
	}
	break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	/*
	int result = MessageBoxA(0, "Invalid charachter!", "Error!", MB_OK | MB_ICONWARNING);
	switch (result)
	{
	case IDOK:
		exit(0);
		break;
	default: 
		MessageBoxA(0, "I'm happy!", "Error!",  MB_ICONWARNING);
		break;
	}
	return 0;
	*/

	//STEP 1

	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIconSm = 0;

	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = L"inscriere";
	wc.lpszMenuName = 0;
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hIcon = LoadIcon(0, IDI_SHIELD);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	//wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	//STEP2

	RegisterClassEx(&wc);

	//STEP3

	int xPos = (GetSystemMetrics(SM_CXSCREEN) - 400) / 2;
	int yPos = (GetSystemMetrics(SM_CYSCREEN) - 225) / 2;

	windowHandle = CreateWindowEx(WS_EX_ACCEPTFILES, L"inscriere", L"Inscriere Copil",
		/*WS_OVERLAPPEDWINDOW | WS_VISIBLE*/ WS_SYSMENU | WS_MINIMIZEBOX , xPos, yPos, 400, 225, HWND_DESKTOP, 0, hInstance, 0);

	//STEP4

	if (windowHandle == 0)
	{
		MessageBoxA(0, "No window!", "Error!", MB_ICONERROR);
		exit(0);
	}
	//STEP5

	ShowWindow(windowHandle, showCmd);

	//STEP6

	UpdateWindow(windowHandle);

	MSG msg;

	SecureZeroMemory(&msg, sizeof(MSG));
	int returnValue = 0;

	while ((returnValue = GetMessage(&msg, 0, 0, 0)) != 0)
	{
		if (returnValue == -1)
		{
			MessageBoxA(windowHandle, "Get Message Failed!", "Error!", MB_ICONERROR);
			break;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

