# Oculus Killer

### Version 2.0.0

*Created by Owen Sullivan, [sulliops.co](https://sulliops.co)*

----

#### What is Oculus Killer?

Oculus Killer is a simple self-contained PowerShell executable designed to kill all Oculus Client processes at once, invalidating the need to open Task Manager and kill each process individually. Oculus Killer will automatically request elevation, as OVRServiceLauncher.exe cannot be killed without administrator privileges.

----

#### Using Oculus Killer:

Version 2.0.0 is standalone, meaning that running "Oculus Killer.exe" is the only necessary action. You can build the executable yourself (see **Compiling Oculus Killer** below), or download the latest release from the repository's [Releases](https://github.com/sulliops/OculusKiller/releases) page.

The executable can be placed in any valid directory, including the Start Menu folder (for easy access).

----

#### Compiling Oculus Killer

The "Oculus Killer.exe" executable file is compiled from the source "Oculus Killer.ps1" PowerShell script and the source "Oculus Killer.ico" icon file. Clone or download the repository to proceed.

To compile the executable, first open an Administrator PowerShell session.

**Installing [PS2EXE](https://github.com/MScholtes/PS2EXE):**

```powershell
PS C:\> Install-Module ps2exe
```

PowerShell may ask you to confirm this installation and the addition of a new repository. Simply entering `Y` to both will continue the installation.

**Compiling Oculus Killer:**

```powershell
PS C:\> Invoke-ps2exe [PATH_TO_OCULUS_KILLER_PS1] [PATH_TO_OUTPUT_EXE] -iconFile [PATH_TO_OCULUS_KILLER_ICO] -noConsole -requireAdmin
```

Command breakdown:

* *Invoke-ps2exe*: Calls the ps2exe PowerShell module that was previously installed.
* *[PATH_TO_OCULUS_KILLER_PS1]*: The full path to the downloaded "Oculus Killer.ps1" file, surrounded by quotation marks.
* *[PATH_TO_OUTPUT_EXE]*: The full path to the executable file you're compiling.
* *-iconFile [PATH_TO_OCULUS_KILLER_ICO]*: A flag for ps2exe which sets a custom icon file, followed by the full path to the downloaded "Oculus Killer.ico" file (surrounded by quotation marks).
* *-noConsole*: Flag for ps2exe which disables PowerShell window feedback while the script is running.
* *-requireAdmin*: Flag for ps2exe which forces the executable to request elevated privileges.

Example command:

```powershell
PS C:\> Invoke-ps2exe "C:\Users\Owen\Downloads\Oculus Killer.ps1" "C:\Users\Owen\Downloads\Oculus Killer.exe" -iconFile "C:\Users\Owen\Downloads\Oculus Killer.ico" -noConsole -requireAdmin
```

