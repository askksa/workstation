dim cmd

cmd=inputbox("1:1280x720 2:1056x1056 3:704x576 0:exit")
cmd=int(cmd)

main()

Sub main
    do while cmd>0
        select case cmd
            case 1
                msgbox "s 0 1280 720"
            case 2
                msgbox "s 0 1056 1056"
            case 3
                msgbox "s 0 704 576"
            case else
                msgbox "input err !!!!!"
        end select
        cmd=inputbox("1:1280x720 2:1056x1056 3:704x576 0:exit")
        cmd=int(cmd)
    loop
End Sub
