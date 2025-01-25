# DualSense HID Reader

This project demonstrates how to interact with the Sony DualSense controller (PlayStation 5) using the Windows HID (Human Interface Device) API. The application reads inputs such as button presses, D-Pad directions, and analog stick movements from the DualSense controller.

## Features
- Detect and connect to a Sony DualSense controller via USB.
- Read and display:
  - D-Pad inputs.
  - Button presses (e.g., Square, Cross, Circle, Triangle).
  - Analog stick positions (Left and Right).
  - System buttons (e.g., PS button, Touchpad, Mute).

## Requirements
- **Windows** operating system.
- **Visual Studio** or any compatible C++ compiler.
- DualSense controller connected via USB.

## Build and Run

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/DualSense-HID-Reader.git
   cd DualSense-HID-Reader
   ```

2. Open the project in Visual Studio or your preferred IDE.

3. Make sure to link against the following libraries:
   - `SetupAPI`
   - `Hid` (Human Interface Device library)

4. Build the project and run the executable.
you can run it in terminal using this command:
```bash
   g++ -o program.exe main.cpp -lhid -lsetupapi
  .\program.exe
   ```


6. Connect the DualSense controller to your PC via USB. The application will detect the controller and start reading inputs.

## How It Works
The program uses the Windows HID API to:
- Enumerate connected HID devices.
- Identify the DualSense controller by its vendor ID (`0x054C`) and product ID (`0x0CE6`).
- Continuously read and interpret the controller's input report.

## Example Output
When the application is running and the controller is connected, you might see output like:
```
DualSense connected! Reading button inputs...
Buttons: [DPad Up] [Square]
Left Stick - X: 128, Y: 128
Right Stick - X: 128, Y: 128
```

## Notes
- Ensure that the DualSense controller is connected via USB.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contributions
Contributions are welcome! Feel free to submit issues or pull requests to improve the project.

