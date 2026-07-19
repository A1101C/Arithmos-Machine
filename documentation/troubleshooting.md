# Troubleshooting

## Useful troubleshooting commands or tools:

#### Very verbose debugging output in the Debug Console
This goes in the body of a launch config.
```
"logging": {
    "engineLogging": true,
    "trace": true,
    "traceResponse": true
},
```

#### Disable remote debug symbol downloads:
This goes in the setupCommands of your launch config.
```
{
    "description": "Disable remote downloads",
    "text": "-gdb-set debuginfod enabled off",
    "ignoreFailures": true
},
```

#### Spawn the program as a child of the debugger rather than spawning it in a new shell
This goes in the setupCommands of your launch config.
```
{
    "description": "Disable startup with shell",
    "text": "set startup-with-shell off",
    "ignoreFailures": false
}
```

# Issues I have run into:

## Debugger Issues

### GDB stalling while downloading remote debug symbols

I ran into an issue with GDB stalling while trying to download additional debug info despite passing arguments into MIEngine in the launch.json config with the disable remote downloads setup command above. Using the verbose debugging output I was able to see that the disable remote downloads command was being passed to gdb correctly but then MIEngine was injecting the command to enable remote downloads again some time later. This would cause gdb to stall for several minutes while it attempted to download the remote debug symbols before eventually failing. 

This seems to be a very common and well known issue with cppdbg on linux but I had a hard time finding a solution that worked for me. My initial investigation and research found that gbd was being spawned as a child of sh rather than of code or any part of the debugger. I initially tried to fix that with the Disable startup with shell setupCommand above but it still spawned as a child of sh. I then verified that it was being spawned with bash, and that I could call it manually to get into the gdb command line tool. After verifying that gdb was in fact working I turned my attention back to preventing remote downloads.

**Things I did to try and prevent remote downloads that failed**
- Tried to use the setupCommand above to disable remote downloads.
- Put both the following in settings.json and restarted vscode
```
"terminal.integrated.env.linux": {
    "DEBUGINFOD_URLS": ""
},
"terminal.integrated.automationEnv.linux": {
    "DEBUGINFOD_URLS": ""
}
```
- Created .gdbinit in my home directory and put "set debuginfod enabled off" in it and restarted my computer.
- commented out "#https://debuginfod.ubuntu.com" in /etc/debuginfod/elfutils.urls and restarted my computer.

**What finally worked**
None of the above worked, MIEngine kept injecting the command to download debug symbols into gdb after my disable command was passed to it. I was finally able to prevent gdb from downloading remote sources by opening /etc/environment and adding "DEBUGINFOD_URLS= " and restarting my computer:
```
DEBUGINFOD_URLS=
```
gdb still tries to download remote sources but it finally doesn't have a url to download from and immediately skips that step. It is possible to manually download remote sources by doing the following:

### Manually download remote debug symbols.
##### My project does not currently requite any remote debug symbols but this is still useful information.
Run this from the project root to see every library it depends on, along with its build-id:

```
for lib in $(ldd output/engine | awk '{print $3}' | grep '^/'); do
    echo "== $lib =="
    file "$lib" | grep -o 'BuildID\[sha1\]=[a-f0-9]*'
done
```
`ldd output/engine` lists all the shared libraries the binary is linked to. The rest of the command grabs the build id for those shared libraries and prints them.

You can put the build-id in this command to query the debuginfod server and fetch the debug files which get dropped into `~/.cache/debuginfod_client/,`
```
DEBUGINFOD_URLS="https://debuginfod.ubuntu.com" debuginfod-find debuginfo <build-id>
```
If you are able to successfully download the libraries you need you can re-enable gdb trying to fetch them by editing /etc/environment and either deleting the debuginfod line or adding the url to it:
```
DEBUGINFOD_URLS=https://debuginfod.ubuntu.com
```
You do have to restart your computer for this to take effect.

