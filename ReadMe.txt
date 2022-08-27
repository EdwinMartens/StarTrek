INTRODUCTION

\++================================|                    _=_
 \_________________________________/              ___/==+++==\___
              """\__      \"""       |======================================/
                    \__    \_          / ..  . _/--===+_____+===--""
                       \__   \       _/.  .. _/         `+'
                          \__ \   __/_______/                      \ /
                         ___-\_\-'---==+____|                  ---==O=-
                   __--+" .    . .        "==_                     / \
                   /  |. .  ..     -------- | \
                   "==+_    .   .  -------- | /
                        ""\___  . ..     __=="
                              """"--=--""


     Credits go to Paramount pictures for the star - trek concept and universe.

This game is actually a very advanced version of the BASIC STAR-TREK text game. see https://en.wikipedia.org/wiki/Star_Trek_%28text_game%29. Many years ago before the WebBrowser wars there was a book SpecTrek (Walther Schoonenberg). It tought how to write a bit more ´graphical´ version of the STAR-TREK text game. It was still turn based and the ´graphics´ where reprogrammed character codes in a text screen (wich is impossible with a PC). The book was for the ZX Spectrum computer, hence its name. Reading the book I ported everyting to GW-Basic and improved on it. A second ( real time) version, still with a text command interface, but with true sprites was written in Delphi 3.0 using Delphi X. The third version (The second real time version) was written in C++ using Allegro 4.x and OpenLayer, and a big 50 * 50 universe. This version (V3) ( third of the real time versions) has a whopping 200 * 200 sectors universe, a sector is now what a Quadrant was in the text version, making it more in line with the startrek universe. It uses the great Allegro 5 library wich enables a lot of cool features. Ships can now Cloak, and actually use it in a smart way.
WORKINGS:

At the start of a new game, a random universe is created. Size is defined by UNIVERSE_SIZE in types.h A sector is the actual "Playing field" you see in the main view. This is where the action happens. Every time the Enterprise enters a new sector, the old sector is stored in the universe, saving everything that happened in that sector. See Universe.h and universe.cpp. And a new "playing field" is built, reading the sector at the new sector position of the enterprise. A sector has a size of SECTORSIZE * SECTORSIZE (10000 * 10000) pixels. So when the Enterprise passes the borders, the Sector position changes and the procedure described above takes place. A sector is built by adding TSprite objects: SpaceObjects, ships, etc. ( all derived from TSprite in Engine.h) to the Engine. When an object is added to the engine, the engine OWNS that object and also takes care of destruction !

All ships are derived from TShip, which has a LOT of basic ship functionality. Adding a new ship class to the game is quite easy The Universe will be dynamic, with rules for moving ships to other sectors, and results of "Simulated battles" and other events.
GAME

You are placed in the role of James Tiberius Kirk, captain of the USS Enterprise NCC-1701. The Enterprise is a big spaceship with a lot of systems and equipment. Everything on your ship uses energy to function. This energy is taken from the WARPCORE energy buffer. The Warp core generates energy by a matter-antimatter reaction producing a huge amount of energy. Under normal circumstances more than enough to power everything on your ship and enough to make a warp jump through multiple sectors. However If the WARPCORE is damaged the energy output will decrease. Energy is also needed to maintain a containment field around the WARPCORE. If the warpcore is damaged and there is no energy left the warpcore will fail. Resulting in the destruction of your ship ! Damaged systems can be repaired at the Engineering screen. The repair crew can only fix one item at a time. At a federation starbase repairs can be made much quicker. The Navigation screen shows the Known universe, revealing more when new sectors are explored or when scanned with a probe (probes not implenemted yet)

The game is far from finished yet. However you can already explore a HUGE universe with Klingon and Romulan empires. For testing purposes A Klingon Task force will invade your home sector at the start of a game. To disable this.. go to Universe.cpp and comment out:

a_Sector.m_nKlingonBC = 2; a_Sector.m_nKlingonBOP = 2; a_Sector.m_nFederation1 = 3;

In Universe::GenerateHomeSector(Sector & a_Sector).
COMBAT

The Enterprise has 2 weapons, Photon torpedoes and Phasers. PHOTON TORPEDOES torpedoes explode when they hit something resulting in massive damage over all systems. The damage is not dependent on the distance, there is only a maximum range. you only have a certain amount of torpedoes, they can be replenished at a starbase only ! When COMPUTER and SENSORS are in good shape, they will precalculate a firing solution for a selected target.

PHASERS only consume energy. Over time, the Phaserbanks are charged with energy from the warpcore buffer. The damage a phaser does is dependend on the distance to the target. The further away, the less damage. PHASERS are much more precise and can take out one single system.

The Enterprise is also equipped with shields. When hit, the shield energy will drop using energy to block the incoming weapon. The shieldsystem will immediately begin to recharge the shield, taking energy from the WARPCORE buffer. If the shieldsystem is damaged,it will become less efficient in charging the shield. (Converting Warpcore energy into shield energy will be less efficient)
CONTROLS

The standard controls are:

  Arrow UP     --->  Speed up
  Arrow DOWN   --->  Slow Down
  Arrow LEFT   --->  Turn Left
  Arrow RIGHT  --->  Turn Right
  T            --->  Select nearest Enemy Target
  F            --->  Select nearest Friend
  TAB          --->  Fire front photon tube
  L SHIFT      --->  Fire aft photon tube
  P            --->  Launch a probe
  SPACE        --->  Fire Phaser
  1..5         --->  Warp 1 tm Warp 5
  T            --->  target nearest enemy
  F            --->  target nearest friend
  D            --->  Dock at starbase (when in range)
  R            --->  Release from dock.
  F1           --->  Main screen
  F2           --->  Engineering
  F3           --->  Astro Navigation

  F5           --->  Navigation Exploration
  F6           --->  Navigation Tactical mode
  F7           --->  Navigation Political mode

  F12          --->  Take a screenshot saved in screenshot folder
