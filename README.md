# Tinky Winkey: A Windows Operating System Experiment

Welcome to Tinky Winkey, where we're about to embark on an exciting journey into the inner workings of the Windows operating system. But don't worry, this is no ordinary tech talk; we're going to dive into the world of Windows Services and Keyloggers while keeping it engaging and accessible. Let's break down the magic of Windows into bite-sized pieces!

## What's All the Buzz About?

### Services: The Hidden Heroes of Windows

Alright, let's talk services. Imagine them as those unsung heroes working behind the scenes in a movie production. They're the ones who make sure everything runs smoothly without stealing the limelight. In the Windows world, services are the unsung heroes that keep your system ticking.

**What are services?**
- **Background Agents**: Services are programs that operate quietly in the background, much like backstage crew members.
- **Managed by SCM**: They are managed by the Service Control Manager (SCM), a part of the Windows operating system.
- **Variety of Tasks**: Services can perform a wide range of tasks, from managing hardware devices to ensuring network services run smoothly.
- **Start Automatically**: Some services start automatically when your computer boots up, without requiring any input from you.

**Why Are We Talking About Services?**
We're going to create our own service called "tinky," and it's going to be a real star in our Windows drama.

**The "tinky" Service**
- **Name**: Our service is named "tinky," inspired by the Teletubbies character.
- **Superpower**: Once it starts running, "tinky" can impersonate a SYSTEM token (yes, that's a big deal), and it will also launch our trusty Keylogger in the background.

### Keyloggers: The Sleuths of Keystrokes

Now, let's introduce the star of our show - the Keylogger!

**What's a Keylogger?**
- **Keystroke Tracker**: A Keylogger is like a digital detective that secretly tracks and records every keystroke you make on your keyboard.
- **Spyware Relative**: Although they sound a bit like spyware, our intentions are purely educational here. We're not going to the dark side of hacking.

**Meet "winkey," Our Keylogger**
- **Name**: We've named our Keylogger "winkey," because it's going to help us capture those "winning" keystrokes.
- **Smart Observer**: "winkey" is pretty smart; it knows which program you're using and only records keystrokes related to that program.
- **Silent Operative**: It works silently in the background, just like a top-notch secret agent.

### The Windows API (WinAPI): Our Toolkit

To pull off our amazing tricks, we need the right toolkit, and that's where the Windows API (WinAPI) comes into play.

**What Is the Windows API (WinAPI)?**
- **The Windows Magician's Kit**: The Windows API, often called WinAPI, is like a magician's kit for developers. It's a set of functions and procedures provided by the Windows operating system.
- **Interaction with Windows**: These functions allow us to interact with and control various aspects of the Windows environment, including services and low-level keyboard input.
- **Abstraction Layer**: Think of it as an abstraction layer that lets developers work with Windows without needing to know all the complex inner workings.
- **Extensibility**: It empowers developers to extend Windows' functionality by creating custom applications and services.

### How We Impersonated the Token of "winlogon"

Impersonating the token of the "winlogon" process is a crucial step in your project, as it allows your service to run with elevated privileges, specifically the SYSTEM level privileges. Here's how it's done:

```c
// Step 1: Establish a connection to the Service Control Manager (SCM).
OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

// Step 2: Open the "winlogon" service.
OpenService(scmHandle, _T("winlogon"), SERVICE_ALL_ACCESS);

// Step 3: Send a control code to the "winlogon" service.
ControlService(serviceHandle, SERVICE_CONTROL_INTERROGATE, &status);

// Step 4: Create a duplicate of the "winlogon" token.
DuplicateTokenEx(token, MAXIMUM_ALLOWED, NULL, SecurityImpersonation, TokenImpersonation, &newToken);

// Step 5: Impersonate the "winlogon" token, granting SYSTEM-level privileges.
ImpersonateLoggedOnUser(newToken);
```

These steps outline the process of impersonating the token of "winlogon" to elevate the privileges of your service.
How We Started the "winkey" Executable

```c
// Step 1: Start a new process, the "winkey" executable.
CreateProcess(NULL, _T("path_to_winkey.exe arguments"), NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo);

// Step 2: Close the handles to the process and thread to clean up resources properly.
CloseHandle(processInfo.hProcess);
CloseHandle(processInfo.hThread);
```

By following these steps, you have successfully impersonated the token of the "winlogon" process and started the "winkey" executable with elevated privileges. This allows your "winkey" keylogger to operate with the necessary permissions to capture keystrokes and interact with the Windows environment. Please note that this explanation is a simplified overview, and actual implementation may require additional error handling and security considerations.
Getting the Party Started
Prerequisites

Before we jump into the action, let's make sure you're prepared for the show:

    Windows System: You'll need a Windows 10 or higher system. We want to ensure everything works like a charm, just like our rehearsals.
    Virtual Machine: Set up a Virtual Machine (VM). We want to avoid any accidents on your primary system.
    C/C++ Skills: Familiarity with C or C++ programming is essential; that's the language we're using.
    CL Compiler: Get ready to use the CL compiler with specific flags. We like things done just right.

Installation

    Clone the Repository: Start by cloning this repository to your virtual machine.
    Compile the Programs: Compile the svc and winkey programs using your favorite compiler.
    Follow the Script: Check out the examples in Chapter V of our script to install, start, stop, and delete the "tinky" service.

