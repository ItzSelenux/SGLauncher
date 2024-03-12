# SGLauncher
Simple GTK Launcher

## Features:

- [x] Show system menu apps
- [x] Search On Web button
- [x] Run In Terminal button
- [x] Show calc answers
- [x] Show Actions

- [ ] have quick access buttons
- [ ] have a simple configuration tool


## Build
Main Program:

```
make
```

standalone Simple GTK Launcher with some features


Configuration:

| Item | Description | Possible values| Values explanation| example
| --- | --- | --- | --- | --- |
| order | change order of items | 0, 1, 2, 3 | 0: Horizontal - Apps at bottom <br> 1: Horizontal - Apps at top <br> 2: Vertical - Apps at left <br> 3: Vertical - Apps at right | order=1|
| cengine | custom engine | custom string | custom string between Quotation marks | cengine="https://you.com/search?q=" |
| wengine | select engine option | 0, 1, 2, 3 | 0: DuckDuckGo <br> 1: Google <br> 2: Bing <br> 3: use custom engine | wengine=1 |
| showweb | toggle show "Search on web option" | 0, 1 | 0: eneable <br> 1: disable | showweb=1
| showcmd | toggle show "Run In Terminal" | 0, 1 | 0: eneable <br> 1: disable | showcmd=1
| showcalc | toggle show answers of Math formula | 0, 1 | 0: eneable <br> 1: disable | showcalc=1

the conf defaults could be on /etc/sglauncher.conf, the personal conf on ~/.config/sglauncher

<p align="center" width="100%">
<img width=46% src="https://github.com/ItzSelenux/ItzSelenux.github.io/blob/main/res/pages/sgrandr/1.png?raw=true">
<img width=47% src="https://github.com/ItzSelenux/ItzSelenux.github.io/blob/main/res/pages/sgrandr/2.png?raw=true">
<img src="https://github.com/ItzSelenux/ItzSelenux.github.io/blob/main/res/pages/sgrandr/3.png?raw=true">
<img src="https://github.com/ItzSelenux/ItzSelenux.github.io/blob/main/res/pages/sgrandr/4.png?raw=true">
</p>



#About:

Created by ItzSelenux Using GTK3, in C with Geany
