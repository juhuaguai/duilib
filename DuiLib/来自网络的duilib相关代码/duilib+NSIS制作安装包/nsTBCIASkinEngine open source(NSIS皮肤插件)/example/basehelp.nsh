
 
Function SetFocus
 
  Exch $0 ; Control Number
  Exch
  Exch $2 ; Page Handle
  Exch
  Exch 2
  Exch $3 ; Page INI File
  Exch 2
  Push $1
  Push $R0
  Push $R1
  Push $R2
  Push $R3
  Push $R4
  Push $R5
 
  IntOp $1 $0 + 1199
  GetDlgItem $1 $2 $1
 
  # Send WM_SETFOCUS message
  System::Call "user32::SetFocus(i r1, i 0x0007, i,i)i"
 
  ReadINIStr $R0 "$3" "Field $0" "Left"
  ReadINIStr $R1 "$3" "Field $0" "Right"
  ReadINIStr $R3 "$3" "Field $0" "Top"
  ReadINIStr $R4 "$3" "Field $0" "Bottom"
  IntOp $R2 $R1 - $R0
  IntOp $R5 $R4 - $R3
 
  System::Call "user32::CreateCaret(i r0, i, i R2, i R5)i"
  System::Call "user32::ShowCaret(i r0)i"
 
  Pop $R5
  Pop $R4
  Pop $R3
  Pop $R2
  Pop $R1
  Pop $R0
  Pop $1
  Pop $0
  Pop $2
  Pop $3
 
FunctionEnd


Function GetWindowsVersion
   Push $R0
   Push $R1

   ClearErrors

   ReadRegStr $R0 HKLM \
   "SOFTWARE\Microsoft\Windows NT\CurrentVersion" CurrentVersion

   IfErrors 0 lbl_winnt

   ; 不是 NT
   ReadRegStr $R0 HKLM \
   "SOFTWARE\Microsoft\Windows\CurrentVersion" VersionNumber

   StrCpy $R1 $R0 1
   StrCmp $R1 '4' 0 lbl_error

   StrCpy $R1 $R0 3

   StrCmp $R1 '4.0' lbl_win32_95
   StrCmp $R1 '4.9' lbl_win32_ME lbl_win32_98

   lbl_win32_95:
     StrCpy $R0 '98'
   Goto lbl_done

   lbl_win32_98:
     StrCpy $R0 '98'
   Goto lbl_done

   lbl_win32_ME:
     StrCpy $R0 '98'
   Goto lbl_done

   lbl_winnt:

   StrCpy $R1 $R0 1

   StrCmp $R1 '3' lbl_winnt_x
   StrCmp $R1 '4' lbl_winnt_x

   StrCpy $R1 $R0 3

   StrCmp $R1 '5.0' lbl_winnt_2000
   StrCmp $R1 '5.1' lbl_winnt_XP
   StrCmp $R1 '5.2' lbl_winnt_2003 lbl_error

   lbl_winnt_x:
     StrCpy $R0 "NT $R0" 6
   Goto lbl_done

   lbl_winnt_2000:
     Strcpy $R0 '2000'
   Goto lbl_done

   lbl_winnt_XP:
     Strcpy $R0 'XP'
   Goto lbl_done

   lbl_winnt_2003:
     Strcpy $R0 '2003'
   Goto lbl_done

   lbl_error:
     Strcpy $R0 ''
   lbl_done:

   Pop $R1
   Exch $R0
FunctionEnd

Function GetIEVersion
   Push $R0
   Push $R1

   ClearErrors

   ReadRegStr $R0 HKLM \
   "SOFTWARE\Microsoft\Internet Explorer" Version  ;获取当前IE的版本

   ;ifErrors lbl_error lbl_supportie

   ;lbl_supportie:
       StrCpy $R1 $R0 1
       StrCmp $R1 '6' lbl_ie6    ;如果相等则跳转
       StrCmp $R1 '7' lbl_ie7
       StrCmp $R1 '8' lbl_ie8    lbl_error    ;除了ie6和ie7之外，不支持别的版本的ie

   lbl_ie6:
       StrCpy $R0 '6'
       Goto lbl_done
   lbl_ie7:
       StrCpy $R0 '7'
       Goto lbl_done

   lbl_ie8:
       StrCpy $R0 '8'
       Goto lbl_done

   lbl_error:
       StrCpy $R0 ''

   lbl_done:

   Pop $R1
   Exch $R0   ;调用者通过读R0的值来判断是ie6还是ie7

FunctionEnd
