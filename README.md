# Roller
Whiplash/Fatal Racing track editor and resource converter

![alt text](https://github.com/Zizin13/Roller/blob/master/TrackEditor/images/screenshot.png)

## Track Editor Features: 
* Open, render, edit, and save Whiplash tracks
* Multiple tracks open at once in tabs
* Export tracks to FBX format
* Track geometry data can be edited and track chunks can be added and removed
* Additional surface data such as grip level and AI data can be edited
* Surface textures can be edited
* Signs can be added, removed, repositioned, and edited
* Audio triggers can be added, removed, and edited
* Moving stunts can be added, removed, and edited
* Track global settings such as texture files and lap data can be edited
* Surface backface textures can be edited
* Undo/Redo and Cut/Copy/Paste between tracks
* Fine control over Copy/Paste behavior
* Toggle display of each individual section of track

## FbxExporter Features:
* Exports all Whiplash tracks and cars to FBX format

## External dependencies used:
* Qt 5.15.2: https://download.qt.io/archive/qt/5.15/5.15.2/single/qt-everywhere-src-5.15.2.zip.mirrorlist
* Glew 2.1.0: https://sourceforge.net/projects/glew/files/glew/2.1.0/
* GLM 1.0.1: https://github.com/g-truc/glm
* FBX 2020.3.7: https://aps.autodesk.com/developer/overview/fbx-sdk (libs are too big to include on github, must install SDK to /external/FBX)

## Remaining issues:
- TRACK1: right wall at the end of final tunnel is shaped wrong (483-486)
- TRACK4: light green outer walls - bit 31? (217-241)
- TRACK5: light green outer walls right before start/finish, roofs before jump (363-366, 81, 87, 93, 99)
- TRACK9: buildings clip into track in tunnel (137)
- TRACK12: wall geometry wrong (106-107)
- TRACK13: building floating over roof (428-431)
- TRACK15: pits need doubled texture (0-46)
- TRACK16: floating tree (203)
- TRACK20: has the geen walls too
- TRACK22: end of tunnel walls are gone facing backwards (335)
- TRACK24: moving pothole animates too quickly (27)
- outer floor at ends of corkscrews
- left lane/right lane belong to combined surface
- z fighting is annoying
- advanced car models have wrong mirror color
- mission car lacks pink color changing
- polygons have four bytes of data that do something unknown
- "places" arrays are unknown
- support backface textures in FBX export (is this possible?)
- support stunt animation in FBX export
- support wheel texture animation in FBX export
