Кроме main еще функция, которая должна получать управление при создании потока

```c
DWORD WINAPI MyThreadFunc(LPVOID Arg) {
    ...
    return 0;
}
```

`CreateThread()`
