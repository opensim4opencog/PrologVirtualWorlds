----------------------------------------------------------------------------
                           Virtual Worlds 1.5 README
                            http://www.vworlds.org/
                                 February 2000
----------------------------------------------------------------------------

               Copyright © 2000 Microsoft Corp. All Rights Reserved.


This document contains information about Virtual Worlds, Version 1.5.


--------
Contents
--------
What is Virtual Worlds?
Important Notes
Credit Clause
Supported Configurations/Platforms
Software Requirements
Known Issues
Installation Packages
Installation Notes
Uninstall
What is the Basic Client?
Creating A World Quickly
Server Notes
Directory Services
Troubleshooting
Product Support 
Acknowledgements

-----------------------
What is Virtual Worlds?
-----------------------
Virtual Worlds is a research platform, originally developed by Microsoft Research, that supports a variety of multi-user environments for creating online communities.  It provides a persisted and distributed object oriented architecture that frees developers from managing client-server communications and database storage.  

We augment the platform with tools and samples that are available separately from the vworlds.org web site, http://www.vworlds.org/.


---------------
Important Notes
---------------
Virtual Worlds is not for commercial use at this time.  Please contact info@vworlds.org for commercial use opportunities.

Portions of this read me are for optional install packages.

Virtual Worlds 1.5 requires Internet Explorer 5 or above, and your browser must be set to accept cookies.

Although supported on all platforms, Virtual Worlds servers on Microsoft Windows 98 are for test purposes only.  Some security and performance features are not enabled on Win9X and best performance is on Microsoft Windows 2000.

During the installation of Virtual Worlds, you accepted the conditions of the End User License Agreement (EULA).  Please read it carefully.


-------------
Credit Clause
-------------
We encourage developers to create virtual worlds and applications using the platform.  When you mention Virtual Worlds, or advertise your projects, you must credit the platform by including, "Built with Virtual Worlds technology developed by Microsoft Research and contributors (http://www.vworlds.org/)."


----------------------------------------
MINIMUM Supported Configuration/Platform
----------------------------------------
The minimum platform requirements depend highly upon the types of worlds and the number of users.  For 3D graphical clients we recommend:

- Pentium 200MHz (350MHz recommended)
- 64 MB of RAM (128 MB recommended)
- 16 bit color video or greater (3D hardware acceleration recommended for graphical applications - please check the newsgroup for current recommended cards and/or chipsets)
- A high speed network connection (56kbps MINIMUM, LAN speeds (10Mbps/100Mbps) recommended)
- Sound Card and Speakers
- Mouse
- 60 MB free of disk space


---------------------
Software Requirements
---------------------
- Microsoft Windows 98 2nd Edition or Microsoft Windows 2000 Pro (Note that non graphical clients and the Virtual Worlds server can run on Windows NT 4.0 SP5)

- Internet Explorer 5 or above (Full Installation required)


---------------------
Installation Packages
---------------------
source.exe     - Source code for Virtual Worlds platform.
platform.exe   - Binaries for Virtual Worlds platform.
samples.exe    - Tools and Samples (SDK) for Virtual Worlds.
ack.exe        - Avatar Construction Kit to create sprite files for Virtual Worlds avatars.
viewpt.exe     - Selected prelicensed Viewpoint 3D models, in DirectX format, for use with Virtual Worlds.  Required for Tutorials.


------------------
Installation Notes
------------------
Make sure you have installed the full installation of Internet Explorer 5 or above.  

It is highly recommended that you install all files in their default locations.  

Always uninstall any previous installations of Virtual Worlds via Add/Remove Programs before installing newer versions.  This clears registry keys and erases old files.  Note that any user data you would like to save during the upgrade process should be backed up.

Even though Virtual Worlds installs in multiple installation packages, there is only one uninstall function.

You can uninstall by clicking the Start Menu, then the Control Panel, then Add/Remove Programs, and then selecting one of the two Microsoft Virtual Worlds entries.


---------
Uninstall
---------
Virtual Worlds supports the Control Panel's Add/Remove Programs functionality.

Please remember to unshare any directories in the Virtual Worlds path, which includes removing any IIS virtual directories or shares.  These will allow the uninstall process to complete its tasks.

There are two types of uninstalls provided in the Add/Remove Programs window:

- Uninstall (Preserve User Data) only deletes the Virtual Worlds components and some of the other files that the user can not modify.  Any avatars, worlds, and directories in which users may have modified or added files, are left intact.  

- Uninstall (Erase ALL) deletes all files in the installation path (default: C:\Program Files\Microsoft Virtual Worlds\) and clears all product-generated registry keys.  Any avatar or world that was not backed up prior to executing this type of uninstall will not be recoverable.


-------------------------
What is the Basic Client?
-------------------------
The Tools and Samples install package now installs a sample client called the Basic Client which is accessed from the Start Menu shortcut "Samples" in the Virtual Worlds program group.

Previous versions of the Virtual Worlds SDK came with what is now called the "Authoring Client" which includes tools and was written/organized in a complex manner.  The new "Basic Client" was written from scratch to serve as an end-user client SAMPLE for developers to base customized clients on.  A lot of the file organization was simplified, better separation of script from HTML, more inline comments, more consistent coding, and the removal of tools code has resulted in an easier to understand client.

The Authoring Client should be used to manipulate 3D geometry and object properties, even on worlds with customized end user clients.  Developers won't likely distribute the Authoring Client and should utilize the Basic Client (or any of the other Virtual Worlds sample applications) as a foundation for developing their own applications rather than using the Authoring Client.


------------------------
Creating a World Quickly
------------------------
As part of the SDK, we have included a "Create a World Wizard" that will let you very quickly build a world from scratch.  

1. Start the server by clicking the Start Menu, then Programs, then Microsoft Virtual Worlds, and then Virtual Worlds Server.

2. Launch the Wizard by clicking New World on the Server's File menu.

3. Follow the instructions and conclude by clicking finish.  If you want to run the server locally (on the same computer), use "localhost" as your server name and make sure your server is running.


------------
Server Notes
------------
We have made recommendations throughout this README but here is an overview with some additional details.

Our server architecture was not designed to be scalable for hundreds of users.  However, we have made improvements to the server from 1.0/1.1 performance levels to address scalability concerns.  To maximize performance, we have some recommendations for system administrators:

- Different types of worlds, and the activity level of users, (i.e. the number of messages generated by, and sent to, each user) will affect server performance.  We have been conducting most of our tests using graphical worlds where automated test users are chatting, moving, creating/editing objects, and moving through portals.  This scenario is more active than a typically deployed world with human users.

- We recommend limiting the number of users per room to 20-25.  However, you can have multiple rooms within a world, and multiple worlds on a server.  As a rough benchmark, with our PII-400MHz with 128MB RAM, the server can handle approximately 5 rooms of 20-25 very active users.  This will scale larger with less active users, more memory, or multi-processor machines.

- Keep in mind that network traffic increases heavily the more users you have in a room.  As each user moves, her position and orientation properties must get propagated to every other user in the room.  The data packet sizes are rather large so a 56kbps modem user will only be able to be connected in a room with approximately 15-17 other users if they are active.  On the server side, since it is dealing with multiple rooms and worlds, bandwidth concerns need to be carefully considered.  A server running over a modem should only be attempted for test purposes with a handful of users.  

- Due to its design, the server can only work as fast as the slowest connection.  If the server begins to lag, it will disconnect a slow or lagged connection without warning.

- We have optimized performance for Windows 2000 Pro.  Servers on NT4 will have reduced performance.  Servers on Win95/98 will have 1.0/1.1 levels of performance.  We recommend that all servers be run on Windows 2000.

- We do support multiprocessor machines.

- We recommend you dedicate a server for a heavily used Virtual Worlds server as it is memory intensive.

- You should limit the number of server-side scripts as these will lock the World thread while they are running.  Most scripts should be set to run client-side if possible, to take advantage of client CPU cycles.


------------------
Directory Services
------------------
Directory Services, or the ability for each server to list their hosted worlds in a central 
directory for clients to select worlds from, is implemented in basic form but requires somewhat 
lengthy additional setup steps.  Please contact info@vworlds.org for instructions and additional
files if you would like to set up a directory service.

Without an active Directory Service, the "Worlds" buttons in the clients and the "Show World in Directory Service" menu item in the server, will be disabled or result in a message box saying the functionality is disabled.

To run a Directory Service, you should be prepared to setup an NT Server machine with IIS and SiteServer to host the service.  This software is not free.  You will also need to install ADSI 2.5 on any machine that runs a VWorlds server and which wants to be listed in the Directory Service.  All client machines will need a registry key set to point at the new Directory Service Server.  ADSI can be installed from http://www.microsoft.com/ntserver/nts/downloads/other/ADSI25/default.asp.


------------
Known Issues
------------
- Some display cards will exhibit some visual problems in the way they draw the Virtual Worlds graphics.  We require DirectX 6.0 or later, so upgrading to the latest version from http://www.microsoft.com/directx/download.asp is mandatory if you are running older versions.  Another option is to search for and install updated drivers from the web site of the manufacturer of your graphics card.  Finally, there are some display options in the Options page from either client that will help fix certain problems.  We do know that at this time we have no hardware acceleration support for Savage4D based cards although the final release of Windows 2000 may include updated drivers to support this chipset.

- Many graphics issues can be resolved by dropping your color bit depth to 16 bit, or toggling the Force Back Buffer display option.

- In the Avatar Construction Kit, JPG image palettes may not appear correct until the avatar is saved.  Some colors appear inverted (reds appear as blue, etc.)

- We do not support animated GIFs that have multiple palettes (different palettes for certain frames).

- Exported scripts of worlds with relative URLs for script modules will error because the CreateScriptModule call is performed before the set RootURL call.  The workaround is to edit the  exported VBS file and change the relative pointer to the scripts to absolute URLs, or to move the rootURL line before the CreateScriptModule line(s).

- If you are programming script module in JScript, you must remember to include the statement, $ENGINE="JavaScript", at the beginning of the module.

- Visual Studio 6 users should make sure they have Service Pack 3 installed if they encounter problems with PDM.DLL.

- Running a client on a multiprocessor machine is not recommended.


---------------
Troubleshooting
---------------
This section only covers Installation and Setup issues.  Please refer to the product's "Getting Additional Help" section under the "Frequently Asked Questions" page.

Q. When installing on Windows NT, I get an error during the installation process about a file not being found, or access being denied in a TEMP directory.

A. Check that the user you've logged in as has administrative privileges on that computer.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Q. When installing some of the software, I get a message telling me that setup is trying to replace a file that is newer than the one being installed, and it asks me if I want to keep the newer file.  

A. Answer yes, or yes-to-all if this option is available.  In other words, you should keep the newer file.  
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Q. During extraction of the content.exe and/or sdk.exe WinZip files in the Tools and Samples install, a file cannot be created.

A. Typically, this is because the file already exists, or write permission is not available for some reason (the file is read-only).  Check the permissions for the directory indicated.  If all else fails, attempt to erase all directories created by the failed setup procedure, and reinitiate setup.  You can change the read-write permission on a file or directory by right-clicking the file/directory and selecting "Properties".  At the bottom of the Properties dialog will be checkboxes for file/directory permissions.

You will need to rerun this self-extracting Zip file after Virtual Worlds has finished installing.  Find "content.exe" and/or "sdk.exe" in C:\Program Files\Microsoft Virtual Worlds and double-click to run it.  Otherwise, you will not unpack all the required files.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Q. What directories are created by setup?

A. The main Virtual Worlds software will be installed in C:\"Program Files"\"Microsoft Virtual Worlds" and its subdirectories.  Additional software (Internet Explorer, Direct X, etc...) will be installed according to their setup instructions.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Q. When I run the basic client, I see some right click menu items that don't seem to do anything.

A. Some authoring menu items (available to authors but not regular users) appear based solely 
on a) whether you're an author, and b) whether the SDK "is installed" registry bit is set.  When 
you distribute a client based on the basic client, you won't likely enable either case, thereby 
eliminating this problem.  Keep in mind that the basic client is a sample for developers and isn't as
robust in error case checking.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Q. My graphics window comes up really small with hardware acceleration.
Q. I don't see some avatar graphics.
Q. I see a system error message: InitializeSprite: Failed to create texture from image file <path>.

A. You are likely out of video memory and should switch your desktop display properties to a lower bit depth (e.g. from 32 bit to 16 bit).
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Q. I'm connecting over a modem and I'm getting a "Server creation failed...." when I try to launch the server.

A. You should check to make sure you have TCP/IP support installed in Control Panel -> Network -> Protocols.  You'll need your Windows install disk(s).
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Q. I created a new method with the same name as an existing system method and now I can't access it.  What should I do?

A. True, we do not warn you if you try to create a method with the same name.  The workaround is to use object.InvokeMethod("methodname") to invoke it, though ideally you should create with a unique name.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Q. I don't get a geometry preview window on the geometry property pages.

A. If you have DX6 debug installed, the geometry preview window (when editing object properties) may not appear.  Normally a 3D rotating thumbnail of the object is presented.  Make sure you have the release version of DX6.1 (or later) installed and have the update to DirectAnimation (available via Microsoft Windows Update).  If you wish to retain the debug version of DX6, you may need to repeatedly right-click refresh to get the preview window to appear (as many as a dozen times).
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Q. I created a world on a remote server but how do I (make it public/delete it/back it up)?

A. Unfortunately, we don't have a convenient way to remotely administer servers.  If you need to remotely administer your Virtual Worlds Server, we recommend using Windows NT Terminal Server.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Q. I run my graphical client for many hours and I eventually run out of "Virtual Memory" and the client starts running very slowly.  Is there any way I can fix this?

A. Depending on your operating system, you can increase your virtual memory size which should help memory intensive applications run better.  Also, if you're low on free hard drive space on your main drive (usually where the memory swap file is located), you can try to create more free space by uninstalling or moving applications to different drives, or move the swap file location to a drive with more space.


---------------
Product Support 
---------------
Please visit http://www.vworlds.org for more information about how to join the Virtual Worlds mailing lists.

Please e-mail info@vworlds.org for all other issues.


----------------
Acknowledgements 
----------------
Some 3D models (c) Copyright 1996 Viewpoint Datalabs International, Inc.
Portions of this product were created using LEADTOOLS (c) 1991-1997 LEAD Technologies, Inc. ALL RIGHTS RESERVED.
Portions of this software are based in part on the work of the Independent JPEG Group.
Portions of this product were created using libraries (c) Copyright 1995 by Autodesk, Inc.
