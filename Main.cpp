#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <iostream>
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
	//initializez verifica
	int j = 0;
	for (j = 0;j < 8;j++)
		verificari[j] = 0;
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

int verifica_varsta_char(char varsta[4])
{
	int i = 0;
	if (strlen(varsta) > 2)
		return 0;
	for (i = 0;i < strlen(varsta);i++)
		if (!(varsta[i] >= '0'&&varsta[i] <= '9'))
			return 0;
	return 1;
}

HWND hnume, hprenume, hvarsta, hadresa, hnume_mama, hnume_tata;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		hnume = CreateWindow(L"Edit",L"", WS_BORDER | WS_CHILD | WS_VISIBLE, 150, 10, 200, 20, hwnd, 0, 0, 0);
		hprenume = CreateWindow(L"Edit", L"", WS_BORDER | WS_CHILD | WS_VISIBLE, 150, 40, 200, 20, hwnd, 0, 0, 0);
		hvarsta = CreateWindow(L"Edit", L"", WS_BORDER | WS_CHILD | WS_VISIBLE, 150, 70, 200, 20, hwnd, 0, 0, 0);
		hadresa = CreateWindow(L"Edit", L"", WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 150, 100, 200, 20, hwnd, 0, 0, 0);
		hnume_mama = CreateWindow(L"Edit", L"", WS_BORDER | WS_CHILD | WS_VISIBLE, 150, 130, 200, 20, hwnd, 0, 0, 0);
		hnume_tata = CreateWindow(L"Edit", L"", WS_BORDER | WS_CHILD | WS_VISIBLE, 150, 160, 200, 20, hwnd, 0, 0, 0);
		CreateWindow(L"Button", L"Cancel", WS_BORDER | WS_CHILD | WS_VISIBLE, 232, 200, 100, 30, hwnd, (HMENU)1, 0, 0);
		CreateWindow(L"Button", L"Inregistreaza", WS_BORDER | WS_CHILD | WS_VISIBLE, 66, 200, 100, 30, hwnd, (HMENU)2, 0, 0);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			case 1:
				exit(0);
			break;
			case 2:
			{
				char nume[20], prenume[25], adresa[45], nume_mama[25], nume_tata[25],varsta[25];
				int varsta_int;
				int gwtstat1 = 0, gwtstat2 = 0, gwtstat3 = 0, gwtstat4 = 0, gwtstat5 = 0,gwtstat6 = 0;
				gwtstat1 = GetWindowTextA(hnume, nume, 24);
				gwtstat2 = GetWindowTextA(hprenume, prenume, 24);
				gwtstat3 = GetWindowTextA(hvarsta, varsta, 24);
				gwtstat4 = GetWindowTextA(hadresa, adresa, 44);
				gwtstat5 = GetWindowTextA(hnume_mama, nume_mama, 24);
				gwtstat6 = GetWindowTextA(hnume_tata, nume_tata, 24);
				if (gwtstat1 ==0 || gwtstat2==0 || gwtstat3==0 || gwtstat4 == 0 || gwtstat5==0 || gwtstat6 ==0)
					MessageBoxA(0, "Toate campurile trebuiesc completate!", "Eroare!", MB_OK | MB_ICONWARNING);
				else if (verifica_varsta_char(varsta)==1)
				{
					long long id=0;
					varsta_int = atoi(varsta);
					calculeaza_id(id, nume, prenume, varsta_int, adresa, nume_mama, nume_tata);
					if (id==-1)
					{
						//MessageBoxA(0, "'id'", "ID!", MB_OK | MB_ICONWARNING);
					}
					else
					{
						switch (stabilire_grupa(varsta_int))
						{
							case 1:
							{
								MessageBoxA(0, "Copil inregistrat cu succes in grupa 1!", "Inregistrare cu succes!", MB_OK | MB_ICONWARNING);
							}
							break;
							case 2:
							{
								MessageBoxA(0, "Copil inregistrat cu succes in grupa 2!", "Inregistrare cu succes!", MB_OK | MB_ICONWARNING);
							}
							break;
							case 3:
							{
								MessageBoxA(0, "Copil inregistrat cu succes in grupa 3!", "Inregistrare cu succes!", MB_OK | MB_ICONWARNING);
							}
							break;
						}
						
					}
				}
				else MessageBoxA(0, "Varsta trebuie sa contina maxim 2 cifre!", "Eroare!", MB_OK | MB_ICONWARNING);


			}
			break;
		}
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
		TextOut(hDC, 10, 12, L"Nume: ", strlen("Nume: "));
		TextOut(hDC, 10, 42, L"Prenume: ", strlen("Prenume: "));
		TextOut(hDC, 10, 72, L"Varsta: ", strlen("Varsta: "));
		TextOut(hDC, 10, 102, L"Adresa: ", strlen("Adresa: "));
		TextOut(hDC, 10, 132, L"Numele mamei: ", strlen("Numele mamei: "));
		TextOut(hDC, 10, 162, L"Numele tatalui: ", strlen("Numele tatalui: "));
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
		/*WS_OVERLAPPEDWINDOW | WS_VISIBLE*/ WS_SYSMENU | WS_MINIMIZEBOX , xPos, yPos, 400, 300, HWND_DESKTOP, 0, hInstance, 0);

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

