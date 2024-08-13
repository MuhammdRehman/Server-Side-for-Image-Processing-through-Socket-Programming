#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include <iostream>
#include <winsock2.h>
#include <fstream>
#include <list>
#include <string>
#include <ctime>
#include<queue>
using namespace std;
#pragma comment(lib, "ws2_32.lib")
#define PORT 34570
#define threshold  125
struct store {
    SOCKET s;
    int** arr;
    int rows, cols;
};
queue<store>q;
int Flood_fill(int**& arr, int r, int c, int pr, int pc)
{
    // front back

    if (pr - 1 >= 0 && arr[pr - 1][pc] == 0)
    {
        arr[pr - 1][pc] = 255; Flood_fill(arr, r, c, pr - 1, pc);
    }
    if (pr + 1 < r && arr[pr + 1][pc] == 0)
    {
        arr[pr + 1][pc] = 255; Flood_fill(arr, r, c, pr + 1, pc);
    }
    if (pc - 1 >= 0 && arr[pr][pc - 1] == 0)
    {
        arr[pr][pc - 1] = 255; Flood_fill(arr, r, c, pr, pc - 1);
    }
    if (pc + 1 < c && arr[pr][pc + 1] == 0)
    {
        arr[pr][pc + 1] = 255;
        Flood_fill(arr, r, c, pr, pc + 1);
    }
    // Diagonal
    if (pr - 1 >= 0 && pc - 1 >= 0 && arr[pr - 1][pc - 1] == 0)
    {
        arr[pr - 1][pc - 1] = 255; Flood_fill(arr, r, c, pr - 1, pc - 1);
    }
    if (pr + 1 < r && pc + 1 < c && arr[pr + 1][pc + 1] == 0)
    {
        arr[pr + 1][pc + 1] = 255; Flood_fill(arr, r, c, pr + 1, pc + 1);
    }
    if (pr + 1 < r && pc - 1 >= 0 && arr[pr + 1][pc - 1] == 0)
    {
        arr[pr + 1][pc - 1] = 255; Flood_fill(arr, r, c, pr + 1, pc - 1);
    }
    if (pr - 1 >= 0 && pc + 1 < c && arr[pr - 1][pc + 1] == 0)
    {
        arr[pr - 1][pc + 1] = 255; Flood_fill(arr, r, c, pr - 1, pc + 1);
    }
    return 1;
}




void processing_worker()
{
    while (true) {
        if (q.empty())break;
        store st = q.front();
        q.pop();
        int count = 0;
        for (int i = 0; i < st.rows; i++)
        {
            for (int j = 0; j < st.cols; j++)
            {
                if (st.arr[i][j] == 0)
                {
                    st.arr[i][j] = 255;
                    Flood_fill(st.arr, st.rows, st.cols, i, j);
                    count++;
                }
            }
        }
        if (send(st.s, (char*)&count, 4, 0) < 1)
        {
            send(st.s, (char*)&count, 4, 0);
        }
        cout << "Image Count Succesfully sent\n";
    }
}
// handle client function
DWORD WINAPI HandleClient(LPVOID lpParam) {
    SOCKET ClientSocket = (SOCKET)lpParam;
    int rows, cols;
    if (recv(ClientSocket, (char*)&rows, sizeof(int), 0) == SOCKET_ERROR)
    {
        cerr << "Failed to receive Number of rows from client.....(:/)\n";
        closesocket(ClientSocket);
        return 0;
    }
    if (recv(ClientSocket, (char*)&cols, sizeof(int), 0) == SOCKET_ERROR)
    {
        cerr << "Failed to receive Number of cols from client.....(:/)\n";
        closesocket(ClientSocket);    
        return 0;
    }
    int** mtx = new int* [rows];
    for (int r = 0; r < rows; r++)
    {
        mtx[r] = new int[cols];
       
       /* for (int c = 0; c < cols; c++)
        {*/
            if (recv(ClientSocket, (char*)mtx[r], cols*4, 0) < 1)
            {
                cout << "Issue is in receiving data!!!!!!!!!";
                return 3;
            }
      // }
    }
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            if (mtx[r][c] > threshold)
            {
                mtx[r][c] = 255;
            }
            else mtx[r][c] = 0;
        }
    }
  
    store st;
    st.s = ClientSocket; st.rows = rows; st.cols = cols; st.arr = mtx;
    q.push(st); 
    processing_worker();
    return 0;
}

int main() {
    WSADATA _wsData;

    if (WSAStartup(MAKEWORD(2, 2), &_wsData) != 0)
    {
        cerr << "Failed to initialize Winsock " << endl;
        return 1;
    }

    SOCKET ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in ServerAdress;

    if (ServerSocket == INVALID_SOCKET)
    {
        cerr << "Failed to create socket " << endl;
        WSACleanup();
        return 1;
    }

    ServerAdress.sin_family = AF_INET;
    ServerAdress.sin_addr.s_addr = INADDR_ANY;
    ServerAdress.sin_port = htons(34570);

    if (bind(ServerSocket, (struct sockaddr*)&ServerAdress, sizeof(ServerAdress)) == SOCKET_ERROR)
    {
        cerr << "Binding failed " << endl;
        closesocket(ServerSocket);
        WSACleanup();
        return 1;
    }

    do
    {       
        if (listen(ServerSocket, SOMAXCONN) == SOCKET_ERROR)
        {
            cerr << "Listing failed. ....Nothing Found " << endl;
            closesocket(ServerSocket);
            WSACleanup();
            return 1;
        }
        cout << "Server is listening on port " << PORT << endl;
        processing_worker();
        SOCKET CleintSocket = accept(ServerSocket, NULL, NULL);
        if (CleintSocket == INVALID_SOCKET)
        {
            cerr << "Accept failed\n";
            closesocket(ServerSocket);
            WSACleanup();
            return 1;
        }

        DWORD ThreadID;
        int client = 5;
        HANDLE thread_handle = CreateThread(NULL, 0, HandleClient, (LPVOID)CleintSocket, 0, &ThreadID);
        if (thread_handle == NULL)
        {
            cerr << "Failed to create thread for client" << endl;
            closesocket(CleintSocket);
        }
        else
        {
            CloseHandle(thread_handle);
        }
      
    }while (true);


    closesocket(ServerSocket);
    WSACleanup();

    return 0;
}
