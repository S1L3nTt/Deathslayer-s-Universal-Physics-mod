Universal physics mod
==========================

WIP

Discord server:
https://discord.gg/wsWJ69d8ub

So far Biological, chemical and physcial reactions/basic nuclear reactions

proper documentation of additions needed, here is the link to a placeholder, https://docs.google.com/document/d/1yisGkPztXYYpDkmZkM6_jZcCM6gIrnptQuHc87NFUSY/edit?usp=sharing


Get the latest version [from the Powder Toy website](https://powdertoy.co.uk/Download.html).

To use online features such as saving, you need to [register an account](https://powdertoy.co.uk/Register.html).
You can also visit [the official TPT forum](https://powdertoy.co.uk/Discussions/Categories/Index.html).

Have you ever wanted to blow something up? Or maybe you always dreamt of operating an atomic power plant? Do you have a will to develop your own CPU? The Powder Toy lets you to do all of these, and even more!

The Powder Toy is a free physics sandbox game, which simulates air pressure and velocity, heat, gravity and a countless number of interactions between different substances! The game provides you with various building materials, liquids, gases and electronic components which can be used to construct complex machines, guns, bombs, realistic terrains and almost anything else. You can then mine them and watch cool explosions, add intricate wirings, play with little stickmen or operate your machine. You can browse and play thousands of different saves made by the community or upload your own – we welcome your creations!

There is a Lua API – you can automate your work or even make plugins for the game. The Powder Toy is free and the source code is distributed under the GNU General Public License, so you can modify the game yourself or help with development. TPT is compiled using scons.

Build instructions
===========================================================================

See the _Powder Toy Development Help_ section [on the main page of the wiki](https://powdertoy.co.uk/Wiki/W/Main_Page.html).

Thanks
===========================================================================

* Stanislaw K Skowronek - Designed the original
* Simon Robertshaw
* Skresanov Savely
* cracker64
* Catelite
* Bryan Hoyle
* Nathan Cousins
* jacksonmj
* Felix Wallin
* Lieuwe Mosch
* Anthony Boot
* Matthew "me4502"
* MaksProg
* jacob1
* mniip
* LBPHacker
* Deathslayer
* Silent
* Bowserinator
* Circuit
* firefreak11
* DanielGalrito
* And all the amazing testers

Libraries and other assets used
===========================================================================

* [BSON](https://github.com/interactive-matter/bson-c)
* [bzip2](http://www.bzip.org/)
* [FFTW](http://fftw.org/)
* [JsonCpp](https://github.com/open-source-parsers/jsoncpp)
* [libcurl](https://curl.se/libcurl/)
* [Lua](https://www.lua.org/)
* [LuaJIT](https://luajit.org/)
* [LuaSocket](http://w3.impa.br/~diego/software/luasocket/)
* [Mallangche](https://github.com/JammPark/Mallangche)
* [SDL](https://libsdl.org/)
* [zlib](https://www.zlib.net/)

Instructions
===========================================================================

Click on the elements with the mouse and draw in the field, like in MS Paint. The rest of the game is learning what happens next.


Controls
===========================================================================

| Key                     | Action                                                          |
| ----------------------- | --------------------------------------------------------------- |
| TAB                     | Switch between circle/square/triangle brush                     |
| Space                   | Pause                                                           |
| Q / Esc                 | Quit                                                            |
| Z                       | Zoom                                                            |
| S                       | Save stamp (use with Ctrl when STK2 is out)                     |
| L                       | Load last saved stamp                                           |
| K                       | Stamp library                                                   |
| 0-9                     | Set view mode                                                   |
| P / F2                  | Save screenshot as .png                                         |
| E                       | Bring up element search                                         |
| F                       | Pause and step to next frame                                    |
| G                       | Increase grid size                                              |
| Shift + G               | Decrease grid size                                              |
| H                       | Show/Hide HUD                                                   |
| Ctrl + H / F1           | Show intro text                                                 |
| D / F3                  | Debug mode (use with Ctrl when STK2 is out)                     |
| I                       | Invert Pressure and Velocity map                                |
| W                       | Cycle gravity modes (use with Ctrl when STK2 is out)            |
| Y                       | Cycle air modes                                                 |
| B                       | Enter decoration editor menu                                    |
| Ctrl + B                | Toggle decorations on/off                                       |
| N                       | Toggle Newtonian Gravity on/off                                 |
| U                       | Toggle ambient heat on/off                                      |
| Ctrl + I                | Install powder toy, for loading saves/stamps by double clicking |
| Backtick                | Toggle console                                                  |
| =                       | Reset pressure and velocity map                                 |
| Ctrl + =                | Reset Electricity                                               |
| \[                      | Decrease brush size                                             |
| \]                      | Increase brush size                                             |
| Alt + \[                | Decrease brush size by 1                                        |
| Alt + \]                | Increase brush size by 1                                        |
| Ctrl + C/V/X            | Copy/Paste/Cut                                                  |
| Ctrl + Z                | Undo                                                            |
| Ctrl + Y                | Redo                                                            |
| Ctrl + Cursor drag      | Rectangle                                                       |
| Shift + Cursor drag     | Line                                                            |
| Middle click            | Sample element                                                  |
| Alt + Left click        | Sample element                                                  |
| Mouse scroll            | Change brush size                                               |
| Ctrl + Mouse scroll     | Change vertical brush size                                      |
| Shift + Mouse scroll    | Change horizontal brush size                                    |
| Shift + R               | Horizontal mirror for selected area when pasting stamps         |
| Ctrl + Shift + R        | Vertical mirror for selected area when pasting stamps           |
| R                       | Rotate selected area counterclockwise when pasting stamps       |



Command Line
---------------------------------------------------------------------------

| Command               | Description                                      | Example                           |
| --------------------- | ------------------------------------------------ | --------------------------------- |
| `scale:SIZE`          | Change window scale factor                       | `scale:2`                         |
| `kiosk`               | Fullscreen mode                                  |                                   |
| `proxy:SERVER[:PORT]` | Proxy server to use                              | `proxy:wwwcache.lancs.ac.uk:8080` |
| `open FILE`           | Opens the file as a stamp or game save           |                                   |
| `ddir DIRECTORY`      | Directory used for saving stamps and preferences |                                   |
| `ptsave:SAVEID`       | Open online save, used by ptsave: URLs           | `ptsave:2198`                     |
| `disable-network`     | Disables internet connections                    |                                   |
| `redirect`            | Redirects output to stdout.txt / stderr.txt      |                                   |



Animal creature guide.


Press D/f13 to show advanced properties to see detailed information about each cell
PROP tool to change specific properties

Biology and chemistry properties
energy(tmpcity[3])
health(tmp3)
nutrients(carbons)
oxygen(oxygens)
CO2(hydrogens)
kinda of waste atm(nitrogens)
water(water)

metabolism speed property = lower is faster
everytime tick count / metabolism == 0, cell uses 1 oxygens + 1 carbons to make 1 energy.

energy capacity is 100
if energy > 99, uses 1 water and 1 energy 
if energy < 1 or water < 1, takes 1 health of damage.
if health < 1, dies and decays into BCTR or other stuff

Animal cells  have diffusion properties, mixing the properties on contact.
WATR, HCL and OIL have similar properties, but they can dissolve solids and other things as well, like SUGR or FLSH.

LUNG with O2 to absorb and to release CO2. If totally sealed, CO2 will build up on the closed lung and suffocate it overtime.

STMH preferably sealed because the HCL(also produces WATR) it produces damages anything other than STMH.
Absorbs nutriens, oxygen and water from WATR and HCL on contact,

POPS on the bottom to release excess and waste materials/properties. Each combination of excess properties(water, oxygens, carbons, nitrogens, hydrogens) will yield a different waste material. Eg, carbons + water = OIL. carbons + oxygens = GAS(pretty arbitrary rn)

BVSL and BLOD are used for better transport of the properties(water, oxygens, carbons, nitrogens, hydrogens) faster and better for distribution. if BVSL surrounded by 2 more BVLS, it turns into BLOD inside the BVSL.
BLOD color indicates it's contents, more RED = O2, more BLUE = nutrients, more GREEN = CO2

Example:
https://media.discordapp.net/attachments/937827232133558281/974926971437985792/unknown.png
