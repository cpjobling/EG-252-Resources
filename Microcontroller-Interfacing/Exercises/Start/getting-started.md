---
title:  'Getting Started with Microcontroller Development'
subtitle: 'EG-252 Group Design Exercise -- Microcontroller Laboratory'
author:
  - Dr Tomothy Davies
affiliation: Swansea University
tags: [eg-252, microcontrollers laboratory, getting started]
date: October 2014
...

## Introduction

This is a simple program that performs the same function as [Exercise
1](../Exercise1/exercise1.html)  but it uses polling instead of interrupt.

## Procedure

View [Toggle.asm](https://github.com/cpjobling/EG-252-Resources/blob/master/Microcontroller-Interfacing/Exercises/Start/toggle2.asm) online. 
To download, right-click on the *Raw* button and save to your desktop.

Now run the Code Warrior IDE from the start/electrical/codewarrior
menu. Use version 6 initially.

1. Create new project
2. Name the project and store somewhere convenient e.g. "H" drive.
3. Set processor AW60, P&E multilink
4. Untick "C" programming language, tick "absolute assembly"
5. Finish

Replace `name/sources/main.asm` with the `toggle.asm` you downloaded
previously,

Delete the default `main.asm` and change the name from `toggle.asm` to
`main.asm`

 Now run Code Warrior, assemble the code, and call up the debug
 feature.

## Exercises 

1. Use the debug feature to step through the program observing what
happens when the button us pressed and when it is released.

2. Change the code so that the LEDs count up from 0 in binary each time the
   button is pressed.

3. Change the code so that button 1 counts up and button 2 counts
down. You will need to test each button separately.

4. Convert the code from assesmbler into C. 
