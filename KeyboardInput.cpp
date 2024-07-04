#include "KeyboardInput.h"

// Define the keyboard input handling function
void handleKeyboardInput()
{
    if (GetAsyncKeyState('W') & 0x8000) {
        writeRealValues[0] += 0.1f; // Increment the first real value by 0.1
        std::cout << "Incrementing linear velocity to " << writeRealValues[0] << std::endl;
    }
    if (GetAsyncKeyState('X') & 0x8000) { 
        writeRealValues[0] -= 0.1f; // Decrement the first real value by 0.1
        std::cout << "Decrementing linear velocity to " << writeRealValues[0] << std::endl;
    }
    if (GetAsyncKeyState('S') & 0x8000) {
        writeRealValues[0] = 0.0f; // Set the first real value to 0
        writeRealValues[1] = 0.0f; // Set the second real value to 0
        std::cout << "Setting linear velocity and angular velocity to 0.0" << std::endl;
    }

    if (GetAsyncKeyState('A') & 0x8000) {
        writeRealValues[1] += 0.1f; // Increment the second real value by 0.1
        std::cout << "Incrementing angular velocity to " << writeRealValues[1] << std::endl;
    }

    if (GetAsyncKeyState('D') & 0x8000) {
        writeRealValues[1] -= 0.1f; // Decrement the second real value by 0.1
        std::cout << "Decrementing angular velocity to " << writeRealValues[1] << std::endl;
    }

    if (GetAsyncKeyState('Q') & 0x8000) {
        writeRealValues[0] += 0.1f; // Increment the first real value by 0.1
        writeRealValues[1] += 0.1f; // Increment the second real value by 0.1
        std::cout << "Incrementing linear and angular velocity to " << writeRealValues[0] << ", " << writeRealValues[1] << std::endl;
    }

    if (GetAsyncKeyState('E') & 0x8000) {
        writeRealValues[0] += 0.1f; // Increment the first real value by 0.1
        writeRealValues[1] -= 0.1f; // Decrement the second real value by 0.1
        std::cout << "Incrementing linear velocity and decrementing angular velocity to " << writeRealValues[0] << ", " << writeRealValues[1] << std::endl;
    }

    if (GetAsyncKeyState('Z') & 0x8000) {
        writeRealValues[0] -= 0.1f; // Decrement the first real value by 0.1
        writeRealValues[1] += 0.1f; // Increment the second real value by 0.1
        std::cout << "Decrementing linear velocity and incrementing angular velocity to " << writeRealValues[0] << ", " << writeRealValues[1] << std::endl;
    }

    if (GetAsyncKeyState('C') & 0x8000) {
        writeRealValues[0] -= 0.1f; // Decrement the first real value by 0.1
        writeRealValues[1] -= 0.1f; // Decrement the second real value by 0.1
        std::cout << "Decrementing linear and angular velocity to " << writeRealValues[0] << ", " << writeRealValues[1] << std::endl;
    }
}
