<br><h1 align="center">LerawinInternal</h1>

<p align="center">
CS:GO ESP with pattern scanned offsets and ray casting visibility check written in C++.<br>
</p>

<p align="center">
  <a href="https://learn.microsoft.com/cpp/"><img src="https://img.shields.io/badge/%20-C++-%2300599C?logo=cplusplus&logoColor=white" alt="C++"></a>
  <a href="https://store.steampowered.com/app/730/CounterStrike_Global_Offensive/"><img src="https://img.shields.io/badge/%20-CS:GO-yellow?logo=counterstrike&logoColor=white" alt="CS:GO"></a>
</p>

<p>
  <img src="https://user-images.githubusercontent.com/54605544/209429882-f74673a7-651f-47e2-856b-cd17648f4ddd.jpg" alt="Program showcase">
</p><br><br>

## Features
* <b>ESP</b>
  * Box
  * Snaplines
  * Visibility Check
* <b>Force Crosshair</b>
  
## Installation
1. Clone the repository.
 ```sh
 git clone https://github.com/tomaszjagielka/LerawinInternal.git
 ```
2. Build the project using Visual Studio or a C++ compiler of your choice.

## Usage
1. Launch CS:GO.
2. Inject the DLL into the process using an injector of your choice.

## How it works?
1. It uses trampoline hooking to hook to the CS:GO process.
2. Then automatically updates offsets using pattern scanning.
3. Next, it checks if player is in the enemy team and casts rays to check if he's seen.
4. Finally, draws ESP on the screen using Direct3D 9.

## Acknowledgements
* GuidedHacking – knowledge and code snippets
* frk – hazedumper
