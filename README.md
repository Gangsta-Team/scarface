
# Scarface The World Is Yours
This project is intended for changing and fixing the game issues and extending the game functionality!

## Folder Structure
|  Folder | Description |
| - | - |
| [code](https://github.com/1h3a3x7/scarface/tree/main/code)  | Contains the code for the main mod and also code for the RCF Explorer and P3D Explorer |
| [documents](https://github.com/1h3a3x7/scarface/tree/main/documents) | Contains the documentation for game natives, structures and general information about Scarface |
| [kaitai](https://github.com/1h3a3x7/scarface/tree/main/kaitai) | Contains the Kaitai Structures and generated Header and Code files for C++ |
| [web-rcf-explorer](https://1h3a3x7.github.io/scarface/web-rcf-explorer/ui.html)  | Contains and small Tool for online exploring .RCF files |

## Modification Files
When you launch the game for first time with the ASI plugin, a folder in `%appdata%` <br> will be created with the name `%appdata%/Gangsta`. Inside of this folder will be existing two files: `config.json` and `log.txt`. <br>In `log.txt` when logging enabled will be every Log Entry in the Console saved! But `config.json` on the other<br>hand is a little bit more complicated.<br>
### Example of config.json
 ```json
 {
"DebugConsole": false,
"ImGuiTextSize": 20.0, // Original is 16.f
"Log": false, // Not used for now
"ReleaseMode": false, // Default false -> spoofs your Game into Dev Mode
"WindowedSpoof": true // if on true your game will be forced into a borderless window mode!
}
```

# Contribution
Everyone can contribute on this project, if you want to colaborate contact us through discord (`Soon ™#7513` or `PatchByte#9884`)
