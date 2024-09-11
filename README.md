# SGLauncher
Simple GTK Launcher

## Features:

- [x] Show system menu apps
- [x] Search On Web button
- [x] Run In Terminal button
- [x] Show calc answers
- [x] Show Actions
- [x] have a simple configuration tool

## Build
Main Program:

```
make
```

Standalone Simple GTK Launcher with some features


## Configuration:

You can use the Settings (sglauncher --cfg) program to easily edit the values with a GUI

| Item | Description | Possible values| Values explanation| example
| --- | --- | --- | --- | --- |
| order | change order of items | 0, 1, 2, 3 | 0: Horizontal - Apps at bottom <br> 1: Horizontal - Apps at top <br> 2: Vertical - Apps at left <br> 3: Vertical - Apps at right | order=0|
| cengine | custom engine | custom string | custom string | cengine=https://search.disroot.org/search?q |
| wengine | select engine option | 0, 1, 2, 3 | 0: DuckDuckGo <br> 1: Google <br> 2: Bing <br> 3: use custom engine | wengine=1 |
| showda | toggle show Desktop actions | 0, 1 | 0: disabled <br> 1: eneable | showda=0
| showweb | toggle show "Search on web option" | 0, 1 | 0: disabled <br> 1: eneable | showweb=1
| showcmd | toggle show "Run In Terminal" | 0, 1 | 0: disabled <br> 1: eneable | showcmd=1
| showcalc | toggle show answers of Math formula | 0, 1 | 0: disabled <br> 1: eneable | showcalc=1
| showscientific | toggle show math answer in scientific format | 0, 1 | 0: disabled <br> 1: eneable | showscientific=1

Config file should be in ~/.config/sglauncher

## About:

Created by ItzSelenux Using GTK3, in C with Geany