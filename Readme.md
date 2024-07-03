# Snake Game

### running locally

dependencies are

1. SDL2
1. make
1. c++ compiler of you choice

snake can runned with different algorithims by passing the `ai` arg with make

```
make ai=simple
```

following algorithims are avaliable

1. user
1. simple
1. simple_cached

use `user` for playing game yourself

### controls

| Key        | function       |
| ---------- | -------------- |
| q          | quit           |
| space      | toggle pause   |
| arrow keys | movement       |
| , comma    | decrease speed |
| . period   | increase speed |

you can change defaults in `defs.h` for frame rate, grid size, pixel size etc.  
