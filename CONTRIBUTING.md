# Contributing to Strata Unreal Engine Plugin

_**Note:** Currently tested and working on Unreal Engine Version(s): 5.7_

## Setup

1. Navigate to your Unreal Engine project's plugins folder (Ex: `cd "C:\Documents\Unreal Projects\<project-name>\Plugins"`)
2. Run `git clone https://github.com/CS4091-Capstone-II-Team-13/StrataPluginUE`
3. Enter the directory: `cd StrataPluginUE`

## Making Changes

### Changing code

1. Disable Live Coding by disabling the checkbox at Edit -> Editor Preferences... -> General -> Live Coding -> General -> Enable Live Coding
2. In Unreal Engine, open Visual Studio by clicking on Tools -> Open Visual Studio
3. Ensure that your configuration at the top is set to `Development Editor` and `Win64`
4. In the soluton explorer, navigate to `Games/<project-name>/Plugins/StrataPluginUE/Source/Strata`
5. Make your changes to the file(s) of your choice...
6. Ensure your changes build correctly by pressing F7 or by clicking on Build -> Build Solution

### Testing your changes within your Unreal Engine project

1. Ensure your changes build correctly (see previous section)
2. Recompile the Strata plugin: Tools -> Debug -> Modules -> Strata -> Recompile

## Creating Pull Requests

1. Before beginning your feature, run `git checkout main; git pull` to ensure your version is up to date
2. Create a new branch from main: `git checkout -b <branch-name>`. Branch name formatting: `[feature, docs, fix, test]/title`
3. Add your changes: `git add .`
4. Commit your changes: `git commit -m "Descriptive description"`
5. Push your changes: `git push -u origin <branch-name>`
6. Visit [https://github.com/CS4091-Capstone-II-Team-13/StrataPluginUE](https://github.com/CS4091-Capstone-II-Team-13/StrataPluginUE) in your browser and click "Create Pull Request"
