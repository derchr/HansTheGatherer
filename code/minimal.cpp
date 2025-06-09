#include <stdio.h>
#include <Hall/Hall.h>

int main()
{
    Hall::Clear(1);
    while (Hall::GetIsGPUBusy())
    {
    }
    Hall::SetCommandSwapBuffers();

    Hall::Clear(1);
    while (Hall::GetIsGPUBusy())
    {
    }
    Hall::SetCommandSwapBuffers();

    // printf("Hallo Welt!\n");

    while (1)
    {
    }

    return 0;
}