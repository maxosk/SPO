#include <Windows.h> 
#include <iostream> 
#include <conio.h>

using namespace std;

SYSTEM_INFO SysInfo;

void writeSystemInfo() {

    
    GetSystemInfo(&SysInfo);

    cout << "������ ��������: " << SysInfo.dwPageSize << endl;
    cout << "����������� ����� ����������: " << SysInfo.lpMinimumApplicationAddress << endl;
    cout << "������������ ����� ����������: " << SysInfo.lpMaximumApplicationAddress << endl;
    cout << "�������� ����� ����������: " << SysInfo.dwActiveProcessorMask << endl;
    cout << "���������� ���� ����������: " << SysInfo.dwNumberOfProcessors << endl;
    cout << "��� ����������: " << SysInfo.dwProcessorType << endl;
    cout << "�������������: " << SysInfo.dwAllocationGranularity << endl;
    cout << "������� ����������: " << SysInfo.wProcessorLevel << endl;
    cout << "��������� ����������?: " << SysInfo.wProcessorRevision << endl;
}
void writeMemoryInfo() {

    MEMORYSTATUS MemStat;
    MemStat.dwLength = { sizeof(MemStat) };
    GlobalMemoryStatus(&MemStat);

    cout << "������ ��������: " << MemStat.dwLength << endl;
    cout << "������� ������������� ������: " << MemStat.dwMemoryLoad << endl;
    cout << "������ ���������� ������ � ������: " << MemStat.dwTotalPhys << endl;
    cout << "��������� ������ � ������: " << MemStat.dwAvailPhys << endl;
    cout << "������ ����� ��������: " << MemStat.dwTotalPageFile << endl;
    cout << "��������� ������ ����� ��������: " << MemStat.dwAvailPageFile << endl;
    cout << "�� ����������� ������: " << MemStat.dwTotalVirtual << endl;
    cout << "��������� ����������� ������: " << MemStat.dwAvailVirtual << endl;

}


void workWithHeap() {
    HANDLE SysHeap = GetProcessHeap(); // ������������ ���������� ��������� ������ 

    UINT MAX_ALLOCATIONS = 32; // ������� ���������� ��������� ������ 
    UINT NumOfAllocations; 
    
    for (NumOfAllocations = 0; NumOfAllocations < MAX_ALLOCATIONS; NumOfAllocations++) // ��������� �� ���� 1 �� ������ 
        if (HeapAlloc(SysHeap, 0, 1024) == NULL) break; // ����� ��������������� ��������� � ����������� �� �������� 

    if (NumOfAllocations == 0) 
        cout << "������ �� ���� �� ����������.\n"; 
    else 
        printf("������ �� ���� ���������� %d ���.", NumOfAllocations); 

    cout << endl;
}


void workWithRAM() {
    SIZE_T MEMSIZE = 4096; 
    LPVOID Mem = VirtualAlloc(NULL, MEMSIZE, MEM_COMMIT, PAGE_EXECUTE_READWRITE); 
    
    if (Mem != NULL) { 
        if (VirtualLock(Mem, MEMSIZE)) 
            printf("��������\n"); 
        else 
            printf("������ ��������\n"); 
        
        printf("������� ����� �������, ����� ����������...\n"); 
        _getch(); 
        
        if (VirtualUnlock(Mem, MEMSIZE)) 
            printf("�������� �����\n"); 
        else 
            printf("������ ������ ��������\n"); 

        if (VirtualFree(Mem, 0, MEM_RELEASE)) 
            printf("������ �����������\n"); 
        else 
            printf("������ �� �����������\n"); 
    }

    else printf("������ �� ��������\n");
}

int main() {
    setlocale(LC_ALL, "Russian");

    writeSystemInfo();
    cout << endl;

    writeMemoryInfo();
    cout << endl;

    workWithHeap();
    cout << endl;

    workWithRAM();
    cout << endl;
}