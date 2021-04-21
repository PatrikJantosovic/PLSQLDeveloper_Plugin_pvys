# PVYS Versioning
## _PL/SQL Developer Plugin for custom version control_

Plugin for Allround Automations PL/SQL Developer. 
Created from Visual Studio demo project provided by PL/SQL developer interface documentation.

This is used only as an interface to already existing project:
https://github.com/PatrikJantosovic/VersionControl

## Features

- Add version to edited source files
- Add version to only staged source files 
- Get list of edited files for given commit
- Get list of edited files for given task (special commit message expected)
- Get list of edited files for given task, only from last commit

## Installation
- Make sure java is on your PATH
- Copy .dll from Release section in this repository to Plugins folder of your PL/SQL Developer installation
- Go to the linked github project and install it

## Usage
- As of now, plugin expects you to have the Task ID / Commit hash stored in your clipboard
- This means that on menu item click, no other action is required and files are automatically edited / list of files is stored to your clipboard 

## License
MIT

