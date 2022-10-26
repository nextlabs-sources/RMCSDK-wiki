RMCSampleApp sample application for SkyDRM Client SDK for Windows
=================================================================



Building the sample application
-------------------------------

1. If you installed SkyDRM Client SDK for Windows on your development
   machine using an installation directory that is different from the
   default, you'll need to adjust the settings "Additional Include
   Directories" and "Additional Library Directories" in the
   RMCSampleApp project properties page in Visual Studio.

2. A Rights Management applications (RM applications) or a Rights
   Management Extensions (RMXes) need a security string when
   communicating with Rights Protection Manager at runtime.  The
   RMCSampleApp source code contains a dummy security string:
        std::string security = "{00000000-0000-0000-0000-000000000000}";
   Please obtain a valid security string from NextLabs, and replace
   the dummy string with it, before you build the sample application.

3. If you want to customize the test directory path and test file
   names, please modify the variables "testDir" and "testFile<N>" in
   the source code.

4. Build the application.  You'll find the executable RMCSampleApp.exe
   under the corresponding platform/Configuration directory
   (e.g. x64\Debug).



Setting up the target environment
---------------------------------

1. Install SkyDRM Rights Protection Manager (RPM) onto your target
   machine.

2. Create the test directory C:\test (or your custom test directory)
   on the target machine.

3. Unzip the included test files "file<N>.txt" to the test directory.
   (Or you can use your custom test files.)

4. Copy RMCSampleApp.exe to somewhere under "C:\Program Files" or
   "C:\Program Files (x86)".  For example, "C:\Program
   Files\MyCorp\MySampleApp".

5. Add the full path of the RMCSampleApp.exe to the <TRUSTEDAPPS>
   section in the file register.xml in "C:\Program
   Files\NextLabs\SkyDRM\RPM\conf" directory.  For example:
        <REGISTER>
            <TRUSTEDAPPS>
                <IMAGEPATH>C:\Program Files\MyCorp\MySampleApp\RMCSampleApp.exe</IMAGEPATH>
            </TRUSTEDAPPS>
        </REGISTER>
   You will need Admin privilege to do this.

6. Reboot the machine.



Running the sample application
------------------------------

1. Launch RMCSampleApp.exe.

2. If you have not already logged in to SkyDRM, RMCSampleApp.exe will
   ask Rights Protection Manager to display the SkyDRM login dialog
   box.  At the same time, RMCSampleApp.exe will exit.

3. Log in to SkyDRM using your SkyDRM Server URL, email address and
   password.

4. After login finishes, Rights Protection Manager will automatically
   launch RMCSampleApp.exe again.

5. Let the application continue, and observe the output.
