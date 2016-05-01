# UE4_SusiPlugin

Integrates Sofia University's Susi system into UE4.

The repo is structured as a standard UE4 project, the plugin itself is in */Plugins/Susi/*.

##### Disclaimer

This is meant to be an educational resource, not a production-ready plugin.
There are may be bugs and/or missing features, pull requests are welcomed.
Keep in mind that the code must be above all easy to understand.

## Plugin structure

### SusiLoader

Handles connection to susi using the RESTful API provided by https://github.com/nikoladimitroff/SusiParser
Can log multiple users and provides the data back using an instance of `FStudentInfo`. It's `Tick` method
must be called or otherwise no requests will go through.

### `SSusiLoginScreen`

A custom Slate widget used for logging users into Susi.
Has 3 states:
- Prelogin - displays a username / password form and a button
- Logging - displays a loading screen
- Postlogin - displays the user's data.

### `USusiComponent`

An `ActorComponent` that can be attached to actors and have them log in
Susi. You must set its `SusiUsername` property to make things work. It
internally reads the user's password from a *Content/PasswordFile.txt*
so make sure you write the credentials there. Reading from file is neccessary
as you don't want to write your plain password in the Component's field
+ the file may be encrypted in a future version.

### `USusiSettings`

Provides common settings for the plugin such as text / font used
for `SSusiLoginScreen` or change the location of the password file.
Note that not all of these settings are meaningful
as production settings but they are there to show how settings can be
exported. Settings can be changed via config files or from *Edit* -> *Project Settings* -> *Plugins* -> *Susi*.

### `FSusiModule`

The module's public API implementation. Implements the logging category and also
registers the plugin's settings to the editor. See `RegisterSettings` in *Susi.cpp*.

