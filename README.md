# Oculus Killer

### Version 3.1.1

*Created by Owen Sullivan, [sulliops.co](https://sulliops.co)*

----

#### What is Oculus Killer?

Oculus Killer is a program written in C++ designed to kill all Oculus Client processes at once, invalidating the need to open Task Manager and kill each process individually. Oculus Killer will automatically request elevation, as OVRServiceLauncher.exe cannot be killed without administrator privileges.

----

#### Using Oculus Killer:

Version 3.1.1 uses a standard application installer from [Inno Setup](https://jrsoftware.org/isinfo.php), but the pre-compiled release can also be downloaded and extracted to any valid directory for use. You can build the executable yourself (see **Compiling Oculus Killer** below), or download the latest release from the repository's [Releases](https://github.com/sulliops/OculusKiller/releases) page.

----

#### Compiling Oculus Killer

v3.1.1 is compiled using Visual Studio 2022, and cannot be compiled outside of Visual Studio due to limitations with the [windows-kill-library](https://github.com/ElyDotDev/windows-kill/tree/master/windows-kill-library) dependency.

The solution (.sln file) can be opened in Visual Studio, and Oculus Killer can be built against the Release x64 target.
