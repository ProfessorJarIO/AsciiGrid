#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <sstream>

#define WIDTH 5
#define HEIGHT 5

struct Position {
	int x;
	int y;
};

struct Boundary {
    int top = 0;        // y val
    int bottom;         // y val
    int left = 0;       // x val
    int right;          // x val
};

// https://learn.microsoft.com/en-us/windows/console/clearing-the-screen
void cls(HANDLE hConsole)
{
    COORD coordScreen = { 0, 0 };    // home for the cursor
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;

    // Get the number of character cells in the current buffer.
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        return;
    }

    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    // Fill the entire screen with blanks.
    if (!FillConsoleOutputCharacter(hConsole,        // Handle to console screen buffer
        (TCHAR)' ',      // Character to write to the buffer
        dwConSize,       // Number of cells to write
        coordScreen,     // Coordinates of first cell
        &cCharsWritten)) // Receive number of characters written
    {
        return;
    }

    // Get the current text attribute.
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        return;
    }

    // Set the buffer's attributes accordingly.
    if (!FillConsoleOutputAttribute(hConsole,         // Handle to console screen buffer
        csbi.wAttributes, // Character attributes to use
        dwConSize,        // Number of cells to set attribute
        coordScreen,      // Coordinates of first cell
        &cCharsWritten))  // Receive number of characters written
    {
        return;
    }

    // Put the cursor at its home coordinates.
    SetConsoleCursorPosition(hConsole, coordScreen);
}

bool VerifyPosition(int top, int bottom, int left, int right, int x, int y) {
    // Check both the x and y coordinates and verify if they fit within map boundaries
    if (x < left || x > right-1) { // We are using -1 because we are going from 0-4, not 0-5 where 5 itself counts (that would be outside boundary)
        return false;
    } 
    if (y < top || y > bottom-1) { // We are using -1 because we are going from 0-4, not 0-5 where 5 itself counts (that would be outside boundary)
        return false;
    }
    return true;
}

int main() {

	// Define initial player position
	Position p1 = { 2, 1 };

    // Define map boundaries
    Boundary b;
    b.bottom = HEIGHT;
    b.right = WIDTH;

    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    std::ostringstream buffer;

	// Continue looping
	while (true) {
        // Capture Key Strokes
        // We use Sleep() to capture individual key stroke, otherwise we would have too many. Maybe a more sophisiticated solution would be to check keydown and then key up.

        // W key
        if (GetAsyncKeyState(0x57)) {
            p1.y -= 1;
            bool pos = VerifyPosition(b.top, b.bottom, b.left, b.right, p1.x, p1.y);
            if (!pos) {
                p1.y += 1; // Return back to original num
            } // If the position is valid, continue...
            Sleep(100);
        }
        // S key
        else if (GetAsyncKeyState(0x53)) {
            p1.y += 1;
            bool pos = VerifyPosition(b.top, b.bottom, b.left, b.right, p1.x, p1.y);
            if (!pos) {
                p1.y -= 1; // Return back to original num
            } // If the position is valid, continue...
            Sleep(100);
        }
        // A key
        else if (GetAsyncKeyState(0x41)) {
            p1.x -= 1;
            bool pos = VerifyPosition(b.top, b.bottom, b.left, b.right, p1.x, p1.y);
            if (!pos) {
                p1.x += 1; // Return back to original num
            } // If the position is valid, continue...
            Sleep(100);
        }
        // D key
        else if (GetAsyncKeyState(0x44)) {
            p1.x += 1;
            bool pos = VerifyPosition(b.top, b.bottom, b.left, b.right, p1.x, p1.y);
            if (!pos) {
                p1.x -= 1; // Return back to original num
            } // If the position is valid, continue...
            Sleep(100);
        }

        std::cout << "Player x: " << p1.x << " y: " << p1.y << "\n";

		// We have to go through each height var first
        for (int height = 0; height < HEIGHT; height++) {
            // Now we have to iterate through width
            for (int width = 0; width < WIDTH; width++) {
                if (p1.x == width && p1.y == height) {
                    buffer << "#";
                }
                else {
                    buffer << ".";
                }
            }
            buffer << "\n";
        }

        std::cout << buffer.str();  // Print buffer
        buffer.str("");             // Clears the stored string
        buffer.clear();             // Resets any error flags
        Sleep(200);

        cls(hStdout);



	}

	return 0;
}
