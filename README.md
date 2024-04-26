# raylib Minesweeper
Это индивидуальный проект по информатике, написанный с использованием библиотеки [raylib](https://github.com/raysan5/raylib).
## Описание игры
Сапёр - игра, цель которой в раскрытии поля плиток, заполненного минами. Если игрок открывает мину, то он проигрывает, если игрок открывает все плитки, не являющиеся минами, он выигрывает. Плитки можно помечать флажками, такие плитки нельзя открыть. Открытые плитки могут иметь цифры, показывающие количество мин вокруг.
## Управление
`1..3` - переключение режимов игры:
- `1` - 9 на 9 плиток, 10 мин.
- `2` - 16 на 16 плиток, 40 мин.
- `3` - 16 на 30 плиток, 99 мин.
`R` - новая игра (перегенерировать поле).
`+` - увеличить масштаб.
`-` - уменьшить масштаб.
`ЛКМ` - открыть плитку.
`ПКМ` - пометить плитку флагом.
## Компиляция
Предполагается сборка для платформ Linux, Windows (32 бита) и Windows (64 бита). <br>
Компиляция может быть осуществлена при помощи следующих комманд:
- ``` make -f linux.mk ``` или ``` cc mines.c -std=c99 -I raylib/raylib-5.0/include -L raylib/raylib-5.0/lib/linux_amd64 -lraylib -lm -o rl-mines ``` - компиляция под Linux.
- ``` make -f win64_mingw-w64.mk ``` или ``` cc mines.c -std=c99 -I raylib/raylib-5.0/include -mwindows -m64 -L raylib/raylib-5.0/lib/win64_mingw-w64 -lraylib -lopengl32 -lgdi32 -lwinmm -o rl-mines.exe ``` - компиляция под Windows (64 бита).
- ``` make -f win32_mingw-w64.mk ``` или ``` cc mines.c -std=c99 -I raylib/raylib-5.0/include -mwindows -m32 -L raylib/raylib-5.0/lib/win32_mingw-w64 -lraylib -lopengl32 -lgdi32 -lwinmm -o rl-mines.exe ``` - компиляция под Windows (32 бита).
Во всех вариантах первая команда - использование системы сборки `make`, а вторая - вызов компилятора, который нужно указать вручную (заменить `cc` на то, что требуется).
Компилятор также можно указать в `make` как: `make -f file CC=gcc`.
