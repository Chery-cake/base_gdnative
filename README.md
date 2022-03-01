# base_gdnative

To start the project go to the folder "native" and create a build folder quer you can execute a cmake command referecing the "code" folder. You may need to update the godot "api.json" file at the "lib/api" folder with the command "godot --gdnative-generate-json-api api.json".

To import any library or CMake project add it at the "lib" folder and execute your cmake command.

To add any code file just execute the cmake command.

You may need to link your library on the Godot Engine.
