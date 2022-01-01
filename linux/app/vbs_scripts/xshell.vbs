Sub Main
    xsh.Screen.Send("./mixer -n 1 -r 2 -o -d 512 -P 0_0_640_480 -w 1_1")
    xsh.Screen.Send(VbCr)

    do while cmd<1000
        xsh.Screen.Send("s 0 1280 720")
        xsh.Screen.Send(VbCr)
        xsh.Session.Sleep(3000)
        xsh.Screen.Send("s 0 1056 1056")
        xsh.Screen.Send(VbCr)
        xsh.Session.Sleep(3000)
        xsh.Screen.Send("s 0 704 576")
        xsh.Screen.Send(VbCr)
        cmd = cmd+1
    loop

End Sub
