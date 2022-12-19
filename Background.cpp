#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <Windows.h>
#include <Commdlg.h>

using namespace std;

// Function to change the desktop background to a random image from a given list of file paths
void ChangeBackground(const vector<string>& filePaths)
{
    // Create a random number generator
    mt19937 rng;
    rng.seed(random_device()());
    uniform_int_distribution<int> dist(0, filePaths.size() - 1);

    // Pick a random image from the list
    int index = dist(rng);
    string filePath = filePaths[index];

    // Set the desktop background to the selected image
    SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (LPVOID)filePath.c_str(), SPIF_UPDATEINIFILE);
}

// Function to display a file open dialog and allow the user to select multiple images
vector<string> SelectImages()
{
    vector<string> filePaths;

    // Set up the file open dialog
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = NULL;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = "All Files\0*.*\0Images\0*.jpg;*.jpeg;*.png\0";
    ofn.nFilterIndex = 2;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT;

    // Show the file open dialog and get the selected file paths
    if (GetOpenFileName(&ofn))
    {
        char filePath[MAX_PATH];
        strcpy_s(filePath, ofn.lpstrFile);

        // If multiple files are selected, split the file paths into separate strings
        char* context = NULL;
        char* token = strtok_s(filePath, "\\", &context);
        while (token != NULL)
        {
            filePaths.push_back(token);
            token = strtok_s(NULL, "\\", &context);
        }
    }

    return filePaths;
}

int main()
{
    // List of file paths to the images selected by the user
    vector<string> filePaths;

    // Display a file open dialog to allow the user to select multiple images
    filePaths = SelectImages();

    // If the user selected at least one image, register a hotkey (Ctrl + Shift + B) to trigger the background change
    if (!filePaths.empty())
    {
        if (!RegisterHotKey(NULL, 1, MOD_CONTROL | MOD_SHIFT, 0x42)) // 0x
