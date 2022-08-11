// TITLE: Oculus Killer
// VERSION: 3.1.0
// CREATED BY: Owen Sullivan
// PURPOSE: This program forces the termination of Oculus software processes that run in the background after the Oculus software is exited.

// COMPILE: Oculus Killer is compiled using Visual Studio 2022. Open the .sln project in Visual Studio and build against the Release x64 target.
// DEPENDENCIES: Oculus Killer depends on the use of the windows-kill-library library from: https://github.com/ElyDotDev/windows-kill/tree/master/windows-kill-library

// SOURCES: https://github.com/ElyDotDev/windows-kill/tree/master/windows-kill-library
//          https://stackoverflow.com/questions/865152/how-can-i-get-a-process-handle-by-its-name-in-c
//          https://stackoverflow.com/questions/57066929/how-would-i-link-a-lib-library-from-a-different-solution-in-visual-studio-201
//          https://stackoverflow.com/questions/246806/i-want-to-convert-stdstring-into-a-const-wchar-t
//          https://www.unknowncheats.me/forum/c-and-c-/267237-std-vector-std-wstring-usage.html
//          https://stackoverflow.com/questions/44985451/how-to-convert-wstring-to-wchar-t-c
//          https://stackoverflow.com/questions/7956519/how-to-kill-processes-by-name-win32-api
//          https://docs.microsoft.com/en-us/cpp/build/reference/manifestuac-embeds-uac-information-in-manifest?view=msvc-170
//          https://stackoverflow.com/questions/6418791/requesting-administrator-privileges-at-run-time
//          https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-messagebox
//          https://stackoverflow.com/questions/6705751/c-win32-api-message-box-button-clicked
//          https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-postquitmessage
//          https://stackoverflow.com/questions/62616855/how-to-specify-an-application-icon-for-c-visual-studio-2019
//          https://stackoverflow.com/questions/8788057/how-to-initialize-and-print-a-stdwstring
//          https://stackoverflow.com/questions/4053918/how-to-portably-write-stdwstring-to-file
//          https://stackoverflow.com/questions/3884124/convert-a-console-app-to-a-windows-app
//          https://stackoverflow.com/questions/495795/how-do-i-use-a-third-party-dll-file-in-visual-studio-c

// Includes
#include <vector> // For std::vector
#include <cstdio> // For I/O
#include <windows.h> // For winapi
#include <tlhelp32.h> // Additional winapi
#include <stdexcept> // For exception handling
#include <WinUser.h> // For MessageBox()
#include <fstream> // For file I/O

// Custom headers
#include "dependencies/windows-kill-library.h" // windows-kill-library

// Link windows-kill-library library
#pragma comment(lib, "dependencies/windows-kill-library.lib")

// windows-kill-library macros
using WindowsKillLibrary::sendSignal;
using WindowsKillLibrary::SIGNAL_TYPE_CTRL_C;
using WindowsKillLibrary::SIGNAL_TYPE_CTRL_BREAK;

// Function prototypes
int displayMessageBox(const bool &anyProcessesFound, const bool &processKillFailure); // Display winapi MessageBox based on existence and/or success of process termination

// Main
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    // Vector to hold process names to be iterated through and killed
    std::vector<std::wstring> processesToKill = { L"OVRServiceLauncher.exe", L"OVRRedir.exe", L"OVRServer_x64.exe" }; // Formatted as wstring for use with szExeFile

    // Boolean to hold status of whether or not any processes were found running (used in MessageBox)
    bool anyProcessesFound = false; // False by default

    // Boolean to hold status of any process termination failures
    bool processKillFailure = false; // False by default

    // Open "lastRun.log" in program directory for output logs of last program run
    std::wofstream lastRunLog;
    lastRunLog.open(L"lastRun.log", std::ios::out | std::ios::trunc); // Truncate existing logs
    
    // Loop through each process and terminate
    for (int numProcess = 0; numProcess < processesToKill.size(); numProcess++) {

        // Boolean to hold status of whether or not processes are found running
        bool processFound = false;
        
        // Create child process to iterate through list of running processes
        PROCESSENTRY32 processEntry;
        processEntry.dwSize = sizeof(PROCESSENTRY32);

        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

        // If child process is still open (not closed from within loop)
        if (Process32First(snapshot, &processEntry) == TRUE) {

            // If child process is still iterating through running processes
            while (Process32Next(snapshot, &processEntry) == TRUE) {

                // If process name matches currently iterated process in running process list
                if (wcscmp(processEntry.szExeFile, processesToKill.at(numProcess).c_str()) == 0) {

                    // Set anyProcessesFound status to true
                    anyProcessesFound = true;
                    
                    // Set processFound status to true
                    processFound = true;
                    
                    // Create a child process with which to find the PID of the matching process
                    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processEntry.th32ProcessID);

                    // Try to terminate the process by PID
                    try {
                        
                        // Log status to "lastRun.log" file
                        lastRunLog << "INFO: Found process matching " << processesToKill.at(numProcess).c_str() << " with PID " << processEntry.th32ProcessID << "." << std::endl;

                        // Use windows-kill-library function sendSignal() to send POSIX-like exit signal
                        sendSignal(processEntry.th32ProcessID, SIGNAL_TYPE_CTRL_C);

                        // Log status to "lastRun.log" file
                        lastRunLog << "INFO: Terminated process with PID " << processEntry.th32ProcessID << "." << std::endl;
                        
                    } catch (const std::invalid_argument &exception) { // Catch error when an argument for sendSignal() is invalid
                        
                        if (strcmp(exception.what(), "ESRCH") == 0) { // If PID does not exist
                            
                            // Log status to "lastRun.log" file
                            lastRunLog << "ERROR: Invalid PID; specified process ID does not exist." << std::endl;

                            // Set processKillFailure status to true
                            processKillFailure = true;

                        } else if (strcmp(exception.what(), "EINVAL") == 0) { // If signal type is invalid
                            
                            // Log status to "lastRun.log" file
                            lastRunLog << "ERROR: Invalid windows-kill signal type." << std::endl;
                        
                            // Set processKillFailure status to true
                            processKillFailure = true;

                        } else { // All other errors
                            
                            // Log status to "lastRun.log" file
                            lastRunLog << "ERROR: InvalidArgument: windows-kill-library:" << exception.what() << std::endl;
                        
                            // Set processKillFailure status to true
                            processKillFailure = true;

                        }

                    } catch (const std::system_error &exception) { // Catch system errors
                        
                        // Log status to "lastRun.log" file
                        lastRunLog << "ERROR: SystemError " << exception.code() << ": " << exception.what() << std::endl;
                    
                        // Set processKillFailure status to true
                        processKillFailure = true;

                    } catch (const std::runtime_error &exception) { // Catch runtime errors
                        
                        if (strcmp(exception.what(), "EPERM") == 0) { // If permissions are not elevated (somehow)
                            
                            // Log status to "lastRun.log" file
                            lastRunLog << "ERROR: Missing required permissions to send specified signal." << std::endl;
                        
                            // Set processKillFailure status to true
                            processKillFailure = true;

                        } else { // All other errors
                            
                            // Log status to "lastRun.log" file
                            lastRunLog << "ERROR: RuntimeError: windows-kill-library:" << exception.what() << std::endl;
                        
                            // Set processKillFailure status to true
                            processKillFailure = true;

                        }
                    
                    } catch (const std::exception &exception) { // Catch all other exceptions
                        
                        // Log status to "lastRun.log" file
                        lastRunLog << "ERROR: windows-kill-library:" << exception.what() << std::endl;
                    
                        // Set processKillFailure status to true
                        processKillFailure = true;

                    }

                    // Close process used to get PID of matching process
                    CloseHandle(hProcess);

                }

            }

        }

        // Close child process
        CloseHandle(snapshot);

        // If a process has not been found matching the specified name
        if (!(processFound)) {

            // Log status to "lastRun.log" file
            lastRunLog << "INFO: No process found matching " << processesToKill.at(numProcess).c_str() << "." << std::endl;

        }

    }

    // Display MessageBox based on whether or not any processes were found
    if (anyProcessesFound) { // If any processes were found

        // Display MessageBox based on whether or not there were failures terminating processes
        if (processKillFailure) { // If some processes failed to terminate

            displayMessageBox(true, true);

        } else { // If all processes terminated successfully

            displayMessageBox(true, false);

        }

    } else { // If no processes were found

        displayMessageBox(false, false);

    }

    // Close "lastRun.log" file
    lastRunLog.close();

}

// Display winapi MessageBox based on success of process termination
int displayMessageBox(const bool &anyProcessesFound, const bool &processKillFailure) {

    if (anyProcessesFound) { // If any processes were found running

        if (processKillFailure) { // If there were termination failures

            // Create MessageBox with error message and error icon
            int msgboxID = MessageBox(NULL, (LPCWSTR)L"Oculus Killer failed to kill one or more process.\nCheck the \"lastrun.log\" file for more information.", (LPCWSTR)L"Oculus Killer", MB_ICONERROR | MB_OK | MB_DEFBUTTON1);

            switch (msgboxID) {

                case IDOK:
                    PostQuitMessage(0); // Close program when "OK" button is clicked
                    return 0;

            }

            return msgboxID; // Generate MessageBox

        } else { // If there were no termination failures

            // Create MessageBox with success message and informational icon
            int msgboxID = MessageBox(NULL, (LPCWSTR)L"Oculus Killer terminated all running Oculus processes successfully.", (LPCWSTR)L"Oculus Killer", MB_ICONINFORMATION | MB_OK | MB_DEFBUTTON1);

            switch (msgboxID) {

                case IDOK:
                    PostQuitMessage(0); // Close program when "OK" button is clicked
                    return 0;

            }

            return msgboxID; // Generate MessageBox

        }

    } else { // If no processes were found running

        // Create MessageBox with message and informational icon
        int msgboxID = MessageBox(NULL, (LPCWSTR)L"Oculus Killer did not find any running Oculus processes.", (LPCWSTR)L"Oculus Killer", MB_ICONINFORMATION | MB_OK | MB_DEFBUTTON1);

        switch (msgboxID) {

            case IDOK:
                PostQuitMessage(0); // Close program when "OK" button is clicked
                return 0;

        }

        return msgboxID; // Generate MessageBox

    }
    
}
